#ifndef RED_BLACK_TREE_HPP
#define RED_BLACK_TREE_HPP

#include <stdexcept>
#include <utility>
#include "TreeNode.hpp"
#include "TreeConstIterator.hpp"
#include "TreeIterator.hpp"

template< class Key, class T, class Compare = std::less< Key > >
class RedBlackTree
{
public:
    using Iterator = TreeIterator< Key, T >;
    using ConstIterator = TreeConstIterator< Key, T >;
    using ValueType = std::pair< Key, T >;

    RedBlackTree();
    RedBlackTree(const RedBlackTree< Key, T, Compare >& rhs);
    RedBlackTree(RedBlackTree< Key, T, Compare >&& rhs) noexcept;
    RedBlackTree< Key, T, Compare >& operator=(RedBlackTree< Key, T, Compare >&& rhs) noexcept;
    RedBlackTree< Key, T, Compare >& operator=(const RedBlackTree< Key, T, Compare >& rhs);
    ~RedBlackTree();

    template< class InputIt >
    RedBlackTree(InputIt first, InputIt last);

    Iterator begin();
    Iterator end();
    ConstIterator cbegin() const;
    ConstIterator cend() const;

    void clear();
    bool empty() const noexcept;

    template< class InputIt >
    void insert(InputIt first, InputIt last);
    std::pair< Iterator, bool > insert(const ValueType& value);
    std::pair< Iterator, bool > insert(ValueType&& value);

    Iterator erase(const Key& key);
    Iterator erase(Iterator pos);
    Iterator erase(ConstIterator pos);

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

    void insertBalanceStep1(detail::Node< Key, T >* subtree);
    void insertBalanceStep2(detail::Node< Key, T >* subtree);
    void insertBalanceStep3(detail::Node< Key, T >* subtree);
    void insertBalanceStep4(detail::Node< Key, T >* subtree);
    void insertBalanceStep5(detail::Node< Key, T >* subtree);

    void turnLeft(detail::Node< Key, T >* subtree);
    void turnRight(detail::Node< Key, T >* subtree);

    TreeIterator< Key, T > eraseNode1(detail::Node< Key, T >* subtree);
    TreeIterator< Key, T > eraseNode2(detail::Node< Key, T >* subtree);
    TreeIterator< Key, T > eraseNode3(detail::Node< Key, T >* subtree);
    void doubleBlack(detail::Node< Key, T >* subtree);

    template< detail::Color C >
    bool hasColor(const detail::Node< Key, T >* node) const;
    template< detail::Color C >
    void recolor(detail::Node< Key, T >* node);
    detail::Node< Key, T >* findKey(const Key& key) const;
    detail::Node< Key, T >* getNext(detail::Node< Key, T >* subtree);
    detail::Node< Key, T >* getGrand(detail::Node< Key, T >* subtree);
    detail::Node< Key, T >* getUncle(detail::Node< Key, T >* subtree);
    detail::Node< Key, T >* getBrother(detail::Node< Key, T >* subtree);
    bool isLeftChild(detail::Node< Key, T >* node);
    bool isRightChild(detail::Node< Key, T >* node);
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
    if (!node)
    {
        root_ = new detail::Node< Key, T >(nullptr, nullptr, nullptr, std::move(value));
        node = root_;
    }
    else
    {
        detail::Node< Key, T >* prev = nullptr;
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

template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::insertBalanceStep1(detail::Node< Key, T >* subtree)
{
    if (!subtree->parent)
    {
        recolor< detail::BLACK >(subtree);
    }
    else
    {
        insertBalanceStep2(subtree);
    }
}

template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::insertBalanceStep2(detail::Node< Key, T >* subtree)
{
    if (hasColor< detail::RED >(subtree->parent))
    {
        insertBalanceStep3(subtree);
    }
}

// 1st case
template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::insertBalanceStep3(detail::Node< Key, T >* subtree)
{
    using namespace detail;
    Node< Key, T >* uncle = getUncle(subtree);
    if (!uncle)
    {
        insertBalanceStep4(subtree);
    }
    else if (hasColor< RED >(uncle))
    {
        recolor< BLACK >(subtree->parent);
        recolor< BLACK >(uncle);
        Node< Key, T >* grand = getGrand(subtree);
        recolor< RED >(grand);
        insertBalanceStep1(grand);
    }
    else
    {
        insertBalanceStep5(subtree);
    }
}

// no uncle
template< class Key, class T, class Compare >
void RedBlackTree<Key, T, Compare>::insertBalanceStep4(detail::Node< Key, T >* subtree)
{
    if (isRightChild(subtree->parent))
    {
        if (isLeftChild(subtree))
        {
            turnRight(subtree->parent);
        }
        else
        {
            subtree = subtree->parent;
        }
        turnLeft(subtree->parent);
        recolor < detail::RED >(subtree->left);
    }
    else
    {
        if (isRightChild(subtree))
        {
            turnLeft(subtree->parent);
        }
        else
        {
            subtree = subtree->parent;
        }
        turnRight(subtree->parent);
        recolor < detail::RED >(subtree->right);
    }
    recolor < detail::BLACK >(subtree);
}

// 2nd case
template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::insertBalanceStep5(detail::Node< Key, T >* subtree)
{
    using namespace detail;
    Node< Key, T >* uncle = getUncle(subtree);
    if (isRightChild(uncle))
    {
        if (isRightChild(subtree))
        {
            turnLeft(subtree->parent);
        }
        else
        {
            subtree = subtree->parent;
        }
        turnRight(subtree->parent);
        recolor< BLACK >(subtree);
        recolor< RED >(subtree->right);
    }
    else
    {
        if (isLeftChild(subtree))
        {
            turnRight(subtree->parent);
        }
        else
        {
            subtree = subtree->parent;
        }
        turnLeft(subtree->parent);
        recolor< BLACK >(subtree);
        recolor< RED >(subtree->left);
    }
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
TreeIterator< Key, T > RedBlackTree< Key, T, Compare >::erase(Iterator pos)
{
    using namespace detail;
    Node< Key, T >* posNode = pos.node_;
    Node< Key, T >* toSwap = nullptr;
    if (posNode->left && posNode->right)
    {
        toSwap = getNext(posNode);
        std::swap(toSwap->data, posNode->data);
    }
    else
    {
        toSwap = posNode;
    }
    --size_;
    return eraseNode1(toSwap);
}

template<class Key, class T, class Compare>
TreeIterator< Key, T > RedBlackTree<Key, T, Compare>::eraseNode1(detail::Node<Key, T>* subtree)
{
    if (hasColor< detail::RED >(subtree))
    {
        if (isLeftChild(subtree))
        {
            subtree->parent->left = nullptr;
        }
        else
        {
            subtree->parent->right = nullptr;
        }
        Iterator next(subtree);
        ++next;
        delete subtree;
        return next;
    }
    return eraseNode2(subtree);
}

template<class Key, class T, class Compare>
TreeIterator< Key, T > RedBlackTree<Key, T, Compare>::eraseNode2(detail::Node<Key, T>* subtree)
{
    detail::Node< Key, T >* child = (subtree->left) ? subtree->left : subtree->right;
    if (child)
    {
        Iterator next(subtree);
        ++next;
        if (next.node_ == child)
        {
            next = Iterator(subtree);
        }

        std::swap(subtree->data, child->data);
        if (subtree->left)
        {
            subtree->left = nullptr;
        }
        else
        {
            subtree->right = nullptr;
        }
        delete child;
        return next;
    }
    return eraseNode3(subtree);
}

template<class Key, class T, class Compare>
TreeIterator<Key, T> RedBlackTree<Key, T, Compare>::eraseNode3(detail::Node<Key, T>* subtree)
{
    Iterator next(subtree);
    ++next;
    doubleBlack(subtree);
    if (subtree == root_)
    {
        root_ = nullptr;
    }
    else if (isLeftChild(subtree))
    {
        subtree->parent->left = nullptr;
    }
    else
    {
        subtree->parent->right = nullptr;
    }
    delete subtree;
    return next;
}

template<class Key, class T, class Compare>
void RedBlackTree<Key, T, Compare>::doubleBlack(detail::Node<Key, T>* subtree)
{
    using namespace detail;
    if (subtree == root_)
    {
        return;
    }
    Node< Key, T >* brother = getBrother(subtree);
    if (hasColor< RED >(brother))
    {
        recolor< BLACK >(brother);
        recolor< RED >(subtree->parent);
        if (isRightChild(brother))
        {
            turnLeft(brother->parent);
        }
        else
        {
            turnRight(brother->parent);
        }
        doubleBlack(subtree);
    }
    else
    {
        bool brotherLeftBlack = (!brother->left || hasColor< BLACK >(brother->left));
        bool brotherRightBlack = (!brother->right || hasColor< BLACK >(brother->right));
        if (brotherLeftBlack && brotherRightBlack)
        {
            recolor< RED >(brother);
            if (hasColor< RED >(subtree->parent))
            {
                recolor< BLACK >(subtree->parent);
            }
            else
            {
                doubleBlack(subtree->parent);
            }
            return;
        }
        if (brotherRightBlack)
        {
            recolor< RED >(brother);
            recolor< BLACK >(brother->left);
            turnRight(brother);
        }
        brother->color = brother->parent->color;
        recolor< BLACK >(brother->parent);
        turnLeft(brother->parent);
        if (brother->right)
        {
            recolor< BLACK >(brother->right);
        }
    }
}

template< class Key, class T, class Compare >
detail::Node< Key, T >* RedBlackTree< Key, T, Compare >::getNext(detail::Node< Key, T >* subtree)
{
    subtree = subtree->left;
    while (subtree->right)
    {
        subtree = subtree->right;
    }
    return subtree;
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
TreeIterator< Key, T > RedBlackTree< Key, T, Compare >::erase(ConstIterator pos)
{
    return erase(Iterator(pos.node_));
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

template< class Key, class T, class Compare >
detail::Node<Key, T>* RedBlackTree< Key, T, Compare >::findKey(const Key& key) const
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
bool RedBlackTree< Key, T, Compare >::hasColor(const detail::Node< Key, T >* node) const
{
    return (node->color == C);
}

template< class Key, class T, class Compare >
template< detail::Color C >
void RedBlackTree< Key, T, Compare >::recolor(detail::Node< Key, T >* node)
{
    node->color = C;
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
detail::Node< Key, T >* RedBlackTree< Key, T, Compare >::getGrand(detail::Node< Key, T >* subtree)
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
detail::Node< Key, T >* RedBlackTree< Key, T, Compare >::getUncle(detail::Node< Key, T >* subtree)
{
    detail::Node< Key, T >* grand = getGrand(subtree);
    if (!grand)
    {
        return nullptr;
    }
    if (grand->right == subtree->parent)
    {
        return grand->left;
    }
    else
    {
        return grand->right;
    }
}

template< class Key, class T, class Compare >
detail::Node< Key, T >* RedBlackTree< Key, T, Compare >::getBrother(detail::Node< Key, T >* subtree)
{
    if (isLeftChild(subtree))
    {
        return subtree->parent->right;
    }
    else
    {
        return subtree->parent->left;
    }
}

#endif