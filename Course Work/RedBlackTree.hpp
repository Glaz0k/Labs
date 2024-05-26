#ifndef RED_BLACK_TREE_HPP
#define RED_BLACK_TREE_HPP

#include <functional>
#include "TreeNode.hpp"
#include "TreeConstIterator.hpp"
#include "TreeIterator.hpp"

#include <iostream>

template< class Key, class T, class Compare = std::less< Key > >
class RedBlackTree
{
public:
    using Iterator = TreeIterator< Key, T >;
    using ConstIterator = TreeConstIterator< Key, T >;
    using ValueType = std::pair< Key, T >;

    RedBlackTree();
    RedBlackTree(const RedBlackTree< Key, T, Compare >& rhs);
    RedBlackTree< Key, T, Compare >& operator=(const RedBlackTree< Key, T, Compare >& rhs);
    RedBlackTree(RedBlackTree< Key, T, Compare >&& rhs) noexcept;
    RedBlackTree< Key, T, Compare >& operator=(RedBlackTree< Key, T, Compare >&& rhs) noexcept;
    ~RedBlackTree();

    template< class InputIt >
    RedBlackTree(InputIt first, InputIt last);

    Iterator begin();
    Iterator end();
    ConstIterator cbegin() const;
    ConstIterator cend() const;

    void clear();
    bool empty() const noexcept;

    std::pair< Iterator, bool > insert(const ValueType& value);
    std::pair< Iterator, bool > insert(ValueType&& value);
    template< class InputIt >
    void insert(InputIt first, InputIt last);

    Iterator erase(const Key& key);
    Iterator erase(Iterator pos);
    Iterator erase(Iterator first, Iterator last);
    Iterator erase(ConstIterator pos);
    Iterator erase(ConstIterator first, ConstIterator last);

    void swap(RedBlackTree& rhs) noexcept;
    size_t size() const noexcept;

    Iterator find(const Key& key);
    ConstIterator find(const Key& key) const;

    T& at(const Key& key);
    const T& at(const Key& key) const;
    T& operator[](const Key& key);
private:
    detail::Node< Key, T >* root_;
    Compare cmp_;
    size_t size_;

    void clearSubtree(detail::Node< Key, T >* subtree);

    detail::Node< Key, T >* findKey(const Key& key) const ;
    detail::Node< Key, T >* findToSwap(detail::Node< Key, T >* subtree);
    detail::Node< Key, T >* findGrand(detail::Node< Key, T >* subtree);
    detail::Node< Key, T >* findUncle(detail::Node< Key, T >* subtree);
    detail::Node< Key, T >* findBrother(detail::Node< Key, T >* subtree);

    template< detail::Color C >
    bool hasColor(detail::Node< Key, T >* node);
    template< detail::Color C >
    void recolor(detail::Node< Key, T >* node);

    bool isLeaf(detail::Node< Key, T >* node);
    bool isLeftChild(detail::Node< Key, T >* node);
    bool isRightChild(detail::Node< Key, T >* node);

    void turnLeft(detail::Node< Key, T >* subtree);
    void turnRight(detail::Node< Key, T >* subtree);

    void insertBalanceStep1(detail::Node< Key, T >* subtree);
    void insertBalance2(detail::Node< Key, T >* subtree);
    void insertBalance3(detail::Node< Key, T >* subtree);
    void insertBalance4(detail::Node< Key, T >* subtree);
    void insertBalance5(detail::Node< Key, T >* subtree);

    void eraseBalance1(detail::Node< Key, T >* subtree);
    void eraseBalance2(detail::Node< Key, T >* subtree);
    void eraseBalance3(detail::Node< Key, T >* subtree);
    void eraseBalance4(detail::Node< Key, T >* subtree);
    void eraseBalance5(detail::Node< Key, T >* subtree);
};

template< class Key, class T, class Compare >
RedBlackTree< Key, T, Compare >::RedBlackTree() :
    root_(nullptr),
    size_(0)
{}

template< class Key, class T, class Compare >
RedBlackTree< Key, T, Compare >::RedBlackTree(const RedBlackTree< Key, T, Compare >& rhs) :
    RedBlackTree(rhs.cbegin(), rhs.cend())
{}

template< class Key, class T, class Compare >
RedBlackTree< Key, T, Compare >::RedBlackTree(RedBlackTree< Key, T, Compare >&& rhs) noexcept :
    root_(rhs.root_),
    size_(rhs.size_)
{
    rhs.root_ = nullptr;
    rhs.size_ = 0;
}

template< class Key, class T, class Compare >
RedBlackTree< Key, T, Compare >::~RedBlackTree()
{
    clear();
}

template< class Key, class T, class Compare >
template< class InputIt >
RedBlackTree< Key, T, Compare >::RedBlackTree(InputIt first, InputIt last) :
    root_(nullptr)
{
    while (first != last)
    {
        try
        {
            insert(*(first++));
        }
        catch (...)
        {
            clear();
        }
    }
}

template< class Key, class T, class Compare >
RedBlackTree< Key, T, Compare >& RedBlackTree< Key, T, Compare >::operator=(const RedBlackTree< Key, T, Compare >& rhs)
{
    return operator=(RedBlackTree< Key, T, Compare >(rhs));
}

template< class Key, class T, class Compare >
RedBlackTree< Key, T, Compare >& RedBlackTree< Key, T, Compare >::operator=(RedBlackTree< Key, T, Compare >&& rhs) noexcept
{
    if (std::addressof(rhs) != this)
    {
        RedBlackTree< Key, T, Compare > temp(std::move(rhs));
        swap(temp);
    }
    return *this;
}

template< class Key, class T, class Compare >
TreeIterator< Key, T > RedBlackTree< Key, T, Compare >::begin()
{
    detail::Node< Key, T >* result = root_;
    while (result && result->left)
    {
        result = result->left;
    }
    return Iterator(result);
}

template< class Key, class T, class Compare >
TreeIterator< Key, T > RedBlackTree< Key, T, Compare >::end()
{
    return Iterator(nullptr);
}

template< class Key, class T, class Compare >
TreeConstIterator< Key, T > RedBlackTree< Key, T, Compare >::cbegin() const
{
    const detail::Node< Key, T >* result = root_;
    while (result && result->left)
    {
        result = result->left;
    }
    return ConstIterator(result);
}

template< class Key, class T, class Compare >
TreeConstIterator< Key, T > RedBlackTree< Key, T, Compare >::cend() const
{
    return ConstIterator(nullptr);
}

template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::clear()
{
    clearSubtree(root_);
    root_ = nullptr;
    size_ = 0;
}

template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::clearSubtree(detail::Node< Key, T >* subtree)
{
    if (!subtree)
    {
        return;
    }
    clearSubtree(subtree->left);
    clearSubtree(subtree->right);
    delete subtree;
}

template< class Key, class T, class Compare >
std::pair< TreeIterator< Key, T >, bool > RedBlackTree< Key, T, Compare >::insert(ValueType&& value)
{
    detail::Node< Key, T >* node = root_;
    if (empty())
    {
        root_ = new detail::Node< Key, T >(nullptr, nullptr, nullptr, std::move(value));
        node = root_;
    }
    else
    {
        detail::Node< Key, T >* prev = node;
        while (node)
        {
            prev = node;
            bool left = cmp_(value.first, node->data.first);
            if (!left && !cmp_(node->data.first, value.first))
            {
                return std::make_pair(Iterator(node), false);
            }
            else if (left)
            {
                node = node->left;
            }
            else
            {
                node = node->right;
            }
        }
        node = new detail::Node< Key, T >(prev, nullptr, nullptr, std::move(value));
        try
        {
            if (cmp_(node->data.first, prev->data.first))
            {
                prev->left = node;
            }
            else
            {
                prev->right = node;
            }
        }
        catch (...)
        {
            delete node;
            throw;
        }
    }
    insertBalanceStep1(node);
    ++size_;
    return std::make_pair(Iterator(node), true);
}

template< class Key, class T, class Compare >
std::pair< TreeIterator< Key, T >, bool > RedBlackTree< Key, T, Compare >::insert(const ValueType& value)
{
    return insert(ValueType(value));
}

template< class Key, class T, class Compare >
template< class InputIt >
void RedBlackTree< Key, T, Compare >::insert(InputIt first, InputIt last)
{
    while (first != last)
    {
        insert(*(first++));
    }
}

// Check if subtree is the root
template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::insertBalanceStep1(detail::Node< Key, T >* subtree)
{
    using namespace detail;
    // Root must be black
    if (!subtree->parent)
    {
        recolor< BLACK >(subtree);
    }
    else
    {
        insertBalance2(subtree);
    }
}

// Check if parent of subtree is RED
template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::insertBalance2(detail::Node< Key, T >* subtree)
{
    using namespace detail;
    // Every RED node must have a BLACK parent
    if (hasColor< RED >(subtree->parent))
    {
        insertBalance3(subtree);
    }
}

// Check if uncle of subtree is RED
template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::insertBalance3(detail::Node< Key, T >* subtree)
{
    using namespace detail;
    Node< Key, T >* uncle = findUncle(subtree);
    // If uncle is RED
    if (uncle && hasColor< RED >(uncle))
    {
        // Recolor parent and uncle BLACK
        recolor< BLACK >(subtree->parent);
        recolor< BLACK >(uncle);
        Node< Key, T >* grand = findGrand(subtree);
        // Recolor grand RED and check balance
        recolor< RED >(grand);
        insertBalanceStep1(grand);
    }
    else
    {
        insertBalance4(subtree);
    }
}

// Check if preparing turn must be performed
template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::insertBalance4(detail::Node< Key, T >* subtree)
{
    using namespace detail;
    Node< Key, T >* grand = findGrand(subtree);
    if (isRightChild(subtree) && subtree->parent == grand->left)
    {
        turnLeft(subtree->parent);
        subtree = subtree->left;
    }
    else if (isLeftChild(subtree) && subtree->parent == grand->right)
    {
        turnRight(subtree->parent);
        subtree = subtree->right;
    }
    insertBalance5(subtree);
}

// Recolor and check which main turn must be performed
template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::insertBalance5(detail::Node< Key, T >* subtree)
{
    using namespace detail;
    Node< Key, T >* grand = findGrand(subtree);
    recolor< BLACK >(subtree->parent);
    recolor< RED >(grand);
    if (isLeftChild(subtree) && subtree->parent == grand->left)
    {
        turnRight(grand);
    }
    else
    {
        turnLeft(grand);
    }
}

template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::turnLeft(detail::Node< Key, T >* subtree)
{
    detail::Node< Key, T >* node = subtree->right;
    node->parent = subtree->parent;
    if (!node->parent)
    {
        root_ = node;
    }
    if (subtree->parent)
    {
        if (isLeftChild(subtree))
        {
            subtree->parent->left = node;
        }
        else
        {
            subtree->parent->right = node;
        }
    }
    subtree->right = node->left;
    if (node->left)
    {
        node->left->parent = subtree;
    }
    subtree->parent = node;
    node->left = subtree;
}

template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::turnRight(detail::Node< Key, T >* subtree)
{
    detail::Node< Key, T >* node = subtree->left;
    node->parent = subtree->parent;
    if (!node->parent)
    {
        root_ = node;
    }
    if (subtree->parent)
    {
        if (isLeftChild(subtree))
        {
            subtree->parent->left = node;
        }
        else
        {
            subtree->parent->right = node;
        }
    }
    subtree->left = node->right;
    if (node->right)
    {
        node->right->parent = subtree;
    }
    subtree->parent = node;
    node->right = subtree;
}

template< class Key, class T, class Compare >
TreeIterator< Key, T > RedBlackTree< Key, T, Compare >::erase(const Key& key)
{
    Iterator toDelete = find(key);
    if (toDelete == end())
    {
        return toDelete;
    }
    return erase(toDelete);
}

template< class Key, class T, class Compare >
TreeIterator< Key, T > RedBlackTree< Key, T, Compare >::erase(Iterator pos)
{
    detail::Node< Key, T >* toDelete = pos.node_;
    detail::Node< Key, T >* found = findToSwap(toDelete);
    if (toDelete == root_ && !found)
    {
        clear();
        return end();
    }
    else if (!found)
    {
        found = toDelete;
    }

    if (isLeftChild(found))
    {
        found->parent->left = found->left;
    }
    else if (isRightChild(found))
    {
        found->parent->right = found->right;
    }

    if (found != toDelete)
    {
        std::swap(found->data, toDelete->data);
    }

    eraseBalance1(found);
    auto iter = ++pos;
    delete found;
    --size_;
    return iter;
}

template< class Key, class T, class Compare >
detail::Node< Key, T >* RedBlackTree< Key, T, Compare >::findToSwap(detail::Node< Key, T >* subtree)
{
    if (subtree->left)
    {
        subtree = subtree->left;
        while (subtree->right)
        {
            subtree = subtree->right;
        }
    }
    else if (subtree->right)
    {
        subtree = subtree->right;
        while (subtree->left)
        {
            subtree = subtree->left;
        }
    }
    else
    {
        subtree = nullptr;
    }
    return subtree;
}

template< class Key, class T, class Compare >
TreeIterator< Key, T > RedBlackTree< Key, T, Compare >::erase(ConstIterator pos)
{
    return ConstIterator(erase(Iterator(pos.node_)));
}

template< class Key, class T, class Compare >
TreeIterator< Key, T > RedBlackTree< Key, T, Compare >::erase(Iterator first, Iterator last)
{
    Iterator result;
    while (first != last)
    {
        result = erase(first++);
    }
    return result;
}

template< class Key, class T, class Compare >
TreeIterator< Key, T > RedBlackTree< Key, T, Compare >::erase(ConstIterator first, ConstIterator last)
{
    return erase(Iterator(first.node_), Iterator(last.node_));
}

// Check if erased subtree node is BLACK
template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::eraseBalance1(detail::Node< Key, T >* subtree)
{
    using namespace detail;
    // Deletion of RED node or LEAF has no effect on balance
    if (hasColor< BLACK >(subtree) && !isLeaf(subtree))
    {
        Node< Key, T >* child = (subtree->right) ? subtree->left : subtree->right;
        if (hasColor< RED >(child))
        {
            recolor< BLACK >(child);
        }
        else if (subtree->parent)
        {
            eraseBalance2(child);
        }
    }
}

// Check if brother of subtree is RED
template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::eraseBalance2(detail::Node< Key, T >* subtree)
{
    using namespace detail;
    Node< Key, T >* brother = findBrother(subtree);
    // Make turn and recolor new brother BLACK
    if (hasColor< RED >(brother))
    {
        recolor< RED >(subtree->parent);
        recolor< BLACK >(brother);
        if (isLeftChild(subtree))
        {
            turnLeft(subtree->parent);
        }
        else
        {
            turnRight(subtree->parent);
        }
    }
    eraseBalance3(subtree);
}

// Check if all brother's child BLACK
template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::eraseBalance3(detail::Node< Key, T >* subtree)
{
    using namespace detail;
    Node< Key, T >* brother = findBrother(subtree);
    bool isBrotherBlack = hasColor< BLACK >(brother);
    bool brotherCondition = isBrotherBlack && hasColor< BLACK >(brother->left) && hasColor< BLACK >(brother->right);
    if (hasColor< BLACK >(subtree->parent) && brotherCondition)
    {
        recolor< RED >(brother);
        eraseBalance1(subtree->parent);
    }
    else if (hasColor< RED >(subtree->parent) && brotherCondition)
    {
        recolor< RED >(brother);
        recolor< BLACK >(subtree->parent);
    }
    else
    {
        eraseBalance4(subtree);
    }
}

// Check which brother's child BLACK and which RED and perform turn
template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::eraseBalance4(detail::Node< Key, T >* subtree)
{
    using namespace detail;
    Node< Key, T >* brother = findBrother(subtree);
    if (hasColor< BLACK >(brother))
    {
        bool isLeftBlack = hasColor< BLACK >(brother->left);
        bool isRightBlack = hasColor< BLACK >(brother->right);
        if (isLeftChild(subtree) && !isLeftBlack && isRightBlack)
        {
            recolor< RED >(brother);
            recolor< BLACK >(brother->left);
            turnRight(brother);
        }
        else if (isRightChild(subtree) && isLeftBlack && !isRightBlack)
        {
            recolor< RED >(brother);
            recolor< BLACK >(brother->right);
            turnLeft(brother);
        }
    }
    eraseBalance5(subtree);
}

// Recolor and perform last turn
template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::eraseBalance5(detail::Node< Key, T >* subtree)
{
    using namespace detail;
    Node< Key, T >* brother = findBrother(subtree);
    brother->color = subtree->parent->color;
    recolor< BLACK >(subtree->parent);
    if (isLeftChild(subtree))
    {
        recolor< BLACK >(brother->right);
        turnLeft(subtree->parent);
    }
    else
    {
        recolor< BLACK >(brother->left);
        turnRight(subtree->parent);
    }
}

template< class Key, class T, class Compare >
bool RedBlackTree< Key, T, Compare >::empty() const noexcept
{
    return (!root_);
}

template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::swap(RedBlackTree< Key, T, Compare >& rhs) noexcept
{
    std::swap(root_, rhs.root_);
}

template< class Key, class T, class Compare >
size_t RedBlackTree< Key, T, Compare >::size() const noexcept
{
    return size_;
}

template<class Key, class T, class Compare>
inline detail::Node<Key, T>* RedBlackTree<Key, T, Compare>::findKey(const Key& key) const
{
    detail::Node< Key, T >* node = root_; 
    while (node) 
    {
        if (cmp_(key, node->data.first))
        {
            node = node->left;
        }
        else if (cmp_(node->data.first, key))
        {
            node = node->right;
        }
        else
        {
            return node;
        }
    }
    return nullptr;
}

template< class Key, class T, class Compare >
TreeIterator< Key, T > RedBlackTree< Key, T, Compare >::find(const Key& key)
{
    return Iterator(findKey(key));
}

template< class Key, class T, class Compare >
TreeConstIterator< Key, T > RedBlackTree< Key, T, Compare >::find(const Key& key) const
{
    return ConstIterator(findKey(key));
}

template< class Key, class T, class Compare >
T& RedBlackTree< Key, T, Compare >::at(const Key& key)
{
    Iterator iter = find(key);
    if (iter == end())
    {
        throw std::out_of_range("Invalid element access");
    }
    return iter->second;
}

template< class Key, class T, class Compare >
const T& RedBlackTree< Key, T, Compare >::at(const Key& key) const
{
    ConstIterator iter = find(key);
    if (iter == cend())
    {
        throw std::out_of_range("Invalid element access");
    }
    return iter->second;
}

template< class Key, class T, class Compare >
T& RedBlackTree< Key, T, Compare >::operator[](const Key& key)
{
    Iterator iter = find(key);
    if (iter == end())
    {
        iter = insert(std::make_pair(key, T())).first;
    }
    return iter->second;
}

template< class Key, class T, class Compare >
template< detail::Color C >
bool RedBlackTree< Key, T, Compare >::hasColor(detail::Node< Key, T >* node)
{
    return (node->color == C);
}

template< class Key, class T, class Compare >
template< detail::Color C >
void RedBlackTree< Key, T, Compare >::recolor(detail::Node< Key, T >* node)
{
    node->color = C;
}

template< class Key, class T, class Compare >
bool RedBlackTree< Key, T, Compare >::isLeaf(detail::Node< Key, T >* node)
{
    return !(node->left && node->right);
}

template<class Key, class T, class Compare>
bool RedBlackTree<Key, T, Compare>::isLeftChild(detail::Node<Key, T>* node)
{
    return (node == node->parent->left);
}

template<class Key, class T, class Compare>
bool RedBlackTree<Key, T, Compare>::isRightChild(detail::Node<Key, T>* node)
{
    return (node == node->parent->right);
}

template< class Key, class T, class Compare >
detail::Node< Key, T >* RedBlackTree< Key, T, Compare >::findGrand(detail::Node< Key, T >* subtree)
{
    if (subtree && subtree->parent)
    {
        return subtree->parent->parent;
    }
    else
    {
        return nullptr;
    }
}

template< class Key, class T, class Compare >
detail::Node< Key, T >* RedBlackTree< Key, T, Compare >::findUncle(detail::Node< Key, T >* subtree)
{
    detail::Node< Key, T >* node = findGrand(subtree);
    if (!node)
    {
        return nullptr;
    }
    if (node->parent == node->left)
    {
        return node->right;
    }
    else
    {
        return node->left;
    }
}

template< class Key, class T, class Compare >
detail::Node< Key, T >* RedBlackTree< Key, T, Compare >::findBrother(detail::Node< Key, T >* subtree)
{
    if (subtree == subtree->parent->left)
    {
        return subtree->parent->right;
    }
    else
    {
        return subtree->parent->left;
    }
}

#endif