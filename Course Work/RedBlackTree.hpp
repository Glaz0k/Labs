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

    Iterator begin();
    Iterator end();
    ConstIterator cbegin() const;
    ConstIterator cend() const;

    void clear();
    bool empty() const noexcept;

    std::pair< Iterator, bool > insert(const ValueType& value);
    std::pair< Iterator, bool > insert(ValueType&& value);
    Iterator insert(Iterator pos, const ValueType& value);
    Iterator insert(Iterator pos, ValueType&& value);
    template< class InputIt>
    void insert(InputIt first, InputIt last);

    Iterator erase(const Key& key);
    Iterator erase(Iterator pos);
    Iterator erase(ConstIterator pos);
    Iterator erase(Iterator first, Iterator last);
    Iterator erase(ConstIterator first, ConstIterator last);

    void swap(RedBlackTree& rhs) noexcept;
    size_t size() const noexcept;
    size_t count(const Key& key) const;

    Iterator find(const Key& key);
    ConstIterator find(const Key& key) const;

    T& at(const Key& key);
    const T& at(const Key& key) const;
private:
    detail::Node< Key, T >* root_;
    Compare cmp_;
    size_t size_;

    void clear_subtree(detail::Node< Key, T >* subtree);
    detail::Node< Key, T >* find_to_change_erased(detail::Node< Key, T >* subtree);
    detail::Node< Key, T >* find_grandparent(detail::Node< Key, T >* subtree);
    detail::Node< Key, T >* find_uncle(detail::Node< Key, T >* subtree);
    detail::Node< Key, T >* find_brother(detail::Node< Key, T >* subtree);
    template< detail::color_t C >
    bool is_color(detail::Node< Key, T >* node);
    template< detail::color_t C >
    void colorize(detail::Node< Key, T >* node);
    bool is_leaf(detail::Node< Key, T >* node);
    void rotate_left(detail::Node< Key, T >* subtree);
    void rotate_right(detail::Node< Key, T >* subtree);
    void insert_balance_case1(detail::Node< Key, T >* subtree);
    void insert_balance_case2(detail::Node< Key, T >* subtree);
    void insert_balance_case3(detail::Node< Key, T >* subtree);
    void insert_balance_case4(detail::Node< Key, T >* subtree);
    void insert_balance_case5(detail::Node< Key, T >* subtree);
    void erase_balance_case1(detail::Node< Key, T >* subtree);
    void erase_balance_case2(detail::Node< Key, T >* subtree);
    void erase_balance_case3(detail::Node< Key, T >* subtree);
    void erase_balance_case4(detail::Node< Key, T >* subtree);
    void erase_balance_case5(detail::Node< Key, T >* subtree);
    void erase_balance_case6(detail::Node< Key, T >* subtree);
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
    clear_subtree(root_);
    root_ = nullptr;
    size_ = 0;
}



template< class Key, class T, class Compare >
std::pair< TreeIterator< Key, T >, bool > RedBlackTree< Key, T, Compare >::insert(const ValueType& value)
{
    return insert(ValueType(value));
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
            if (!cmp_(value.first, node->data.first) && !cmp_(node->data.first, value.first))
            {
                return std::make_pair(Iterator(node), false);
            }
            else if (cmp_(value.first, node->data.first))
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
    insert_balance_case1(node);
    ++size_;
    return std::make_pair(Iterator(node), true);
}

template< class Key, class T, class Compare >
TreeIterator< Key, T > RedBlackTree< Key, T, Compare >::insert(Iterator pos, const ValueType& value)
{
    return insert(pos, ValueType(value));
}

template< class Key, class T, class Compare >
TreeIterator< Key, T > RedBlackTree< Key, T, Compare >::insert(Iterator pos, ValueType&& value)
{
    detail::Node< Key, T >* node = pos.node_;
    detail::Node< Key, T >* prev = node;
    while (node)
    {
        prev = node;
        if (cmp_(value.first, node->data.first))
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
    insert_balance_case1(node);
    ++size_;
    return Iterator(node);
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
TreeIterator< Key, T > RedBlackTree< Key, T, Compare >::erase(const Key& key)
{
    detail::Node< Key, T >* node = root_;
    detail::Node< Key, T >* to_delete = nullptr;
    while (!to_delete && node)
    {
        if (node->data.first == key)
        {
            to_delete = node;
        }
        else if (cmp_(key, node->data.first))
        {
            node = node->left;
        }
        else
        {
            node = node->right;
        }
    }
    if (!to_delete)
    {
        return end();
    }
    return erase(Iterator(to_delete));
}

template< class Key, class T, class Compare >
TreeIterator< Key, T > RedBlackTree< Key, T, Compare >::erase(Iterator pos)
{
    detail::Node< Key, T >* to_delete = pos.node_;
    detail::Node< Key, T >* found = find_to_change_erased(to_delete);
    if (to_delete == root_ && !found)
    {
        clear();
        return end();
    }
    else if (!found)
    {
        found = to_delete;
    }
    if (found->parent->left == found)
    {
        found->parent->left = found->left;
    }
    else if (found->parent->right == found)
    {
        found->parent->right = found->right;
    }
    if (found != to_delete)
    {
        std::swap(found->data, to_delete->data);
    }
    erase_balance_case1(found);
    auto iter = ++pos;
    delete found;
    --size_;
    return iter;
}

template< class Key, class T, class Compare >
TreeIterator< Key, T > RedBlackTree< Key, T, Compare >::erase(ConstIterator pos)
{
    return TreeConstIterator< Key, T >(erase(Iterator(pos.node_)));
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
size_t RedBlackTree< Key, T, Compare >::count(const Key& key) const
{
    auto citer = cbegin();
    size_t size = 0;
    while (citer != cend())
    {
        if (!cmp_(key, citer->first) && !cmp_(citer->first, key))
        {
            ++size;
        }
    }
    return size;
}

template< class Key, class T, class Compare >
TreeIterator< Key, T > RedBlackTree< Key, T, Compare >::find(const Key& key)
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
            return Iterator(node);
        }
    }
    return end();
}

template< class Key, class T, class Compare >
TreeConstIterator< Key, T > RedBlackTree< Key, T, Compare >::find(const Key& key) const
{
    const detail::Node< Key, T >* node = root_;
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
            return ConstIterator(node);
        }
    }
    return cend();
}

template< class Key, class T, class Compare >
T& RedBlackTree< Key, T, Compare >::at(const Key& key)
{
    TreeIterator< Key, T > iter = find(key);
    if (iter == end())
    {
        throw std::out_of_range("Out of range in element access");
    }
    return iter->second;
}

template< class Key, class T, class Compare >
const T& RedBlackTree< Key, T, Compare >::at(const Key& key) const
{
    TreeConstIterator< Key, T > iter = find(key);
    if (iter == cend())
    {
        throw std::out_of_range("Out of range in element access");
    }
    return iter->second;
}

template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::clear_subtree(detail::Node< Key, T >* subtree)
{
    if (!subtree)
    {
        return;
    }
    clear_subtree(subtree->left);
    clear_subtree(subtree->right);
    delete subtree;
}

template< class Key, class T, class Compare >
detail::Node< Key, T >* RedBlackTree< Key, T, Compare >::find_to_change_erased(detail::Node< Key, T >* subtree)
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
template< detail::color_t C >
bool RedBlackTree< Key, T, Compare >::is_color(detail::Node< Key, T >* node)
{
    return (node->color == C);
}

template< class Key, class T, class Compare >
template< detail::color_t C >
void RedBlackTree< Key, T, Compare >::colorize(detail::Node< Key, T >* node)
{
    node->color = C;
}

template< class Key, class T, class Compare >
bool RedBlackTree< Key, T, Compare >::is_leaf(detail::Node< Key, T >* node)
{
    return !(node->left && node->right);
}

template< class Key, class T, class Compare >
detail::Node< Key, T >* RedBlackTree< Key, T, Compare >::find_grandparent(detail::Node< Key, T >* subtree)
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
detail::Node< Key, T >* RedBlackTree< Key, T, Compare >::find_uncle(detail::Node< Key, T >* subtree)
{
    detail::Node< Key, T >* node = find_grandparent(subtree);
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
detail::Node< Key, T >* RedBlackTree< Key, T, Compare >::find_brother(detail::Node< Key, T >* subtree)
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

template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::rotate_left(detail::Node< Key, T >* subtree)
{
    detail::Node< Key, T >* node = subtree->right;
    node->parent = subtree->parent;
    if (!node->parent)
    {
        root_ = node;
    }
    if (subtree->parent)
    {
        if (subtree->parent->left == subtree)
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
void RedBlackTree< Key, T, Compare >::rotate_right(detail::Node< Key, T >* subtree)
{
    detail::Node< Key, T >* node = subtree->left;
    node->parent = subtree->parent;
    if (!node->parent)
    {
        root_ = node;
    }
    if (subtree->parent)
    {
        if (subtree->parent->left == subtree)
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
void RedBlackTree< Key, T, Compare >::insert_balance_case1(detail::Node< Key, T >* subtree)
{
    using namespace detail;
    if (!subtree->parent)
    {
        colorize< BLACK >(subtree);
    }
    else
    {
        insert_balance_case2(subtree);
    }
}

template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::insert_balance_case2(detail::Node< Key, T >* subtree)
{
    using namespace detail;
    if (is_color< RED >(subtree->parent))
    {
        insert_balance_case3(subtree);
    }
}

template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::insert_balance_case3(detail::Node< Key, T >* subtree)
{
    using namespace detail;
    Node< Key, T >* uncle = find_uncle(subtree);
    if (uncle && is_color< RED >(uncle))
    {
        colorize< BLACK >(subtree->parent);
        colorize< BLACK >(uncle);
        Node< Key, T >* grand = find_grandparent(subtree);
        colorize< RED >(grand);
        insert_balance_case1(grand);
    }
    else
    {
        insert_balance_case4(subtree);
    }
}

template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::insert_balance_case4(detail::Node< Key, T >* subtree)
{
    using namespace detail;
    Node< Key, T >* grand = find_grandparent(subtree);
    if (subtree == subtree->parent->right && subtree->parent == grand->left)
    {
        rotate_left(subtree->parent);
        subtree = subtree->left;
    }
    else if (subtree == subtree->parent->left && subtree->parent == grand->right)
    {
        rotate_right(subtree->parent);
        subtree = subtree->right;
    }
    insert_balance_case5(subtree);
}

template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::insert_balance_case5(detail::Node< Key, T >* subtree)
{
    using namespace detail;
    Node< Key, T >* grand = find_grandparent(subtree);
    colorize< BLACK >(subtree->parent);
    colorize< RED >(grand);
    if (subtree == subtree->parent->left && subtree->parent == grand->left)
    {
        rotate_right(grand);
    }
    else
    {
        rotate_left(grand);
    }
}

template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::erase_balance_case1(detail::Node< Key, T >* subtree)
{
    using namespace detail;
    if (is_color< BLACK >(subtree) && !is_leaf(subtree))
    {
        Node< Key, T >* child = (subtree->right) ? subtree->left : subtree->right;
        if (is_color< RED >(child))
        {
            colorize< BLACK >(child);
        }
        else if (subtree->parent)
        {
            erase_balance_case2(child);
        }
    }
}

template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::erase_balance_case2(detail::Node< Key, T >* subtree)
{
    using namespace detail;
    Node< Key, T >* brother = find_brother(subtree);
    if (is_color< RED >(brother))
    {
        colorize< RED >(subtree->parent);
        colorize< BLACK >(brother);
        if (subtree == subtree->parent->left)
        {
            rotate_left(subtree->parent);
        }
        else
        {
            rotate_right(subtree->parent);
        }
    }
    erase_balance_case3(subtree);
}

template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::erase_balance_case3(detail::Node< Key, T >* subtree)
{
    using namespace detail;
    Node< Key, T >* brother = find_brother(subtree);
    bool is_any_child_black = is_color< BLACK >(brother->left) && is_color< BLACK >(brother->right);
    if (is_color< BLACK >(subtree->parent) && is_color< BLACK >(brother) && is_any_child_black)
    {
        colorize< RED >(brother);
        erase_balance_case1(subtree->parent);
    }
    else
    {
        erase_balance_case4(subtree);
    }
}

template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::erase_balance_case4(detail::Node< Key, T >* subtree)
{
    using namespace detail;
    Node< Key, T >* brother = find_brother(subtree);
    bool is_any_child_black = is_color< BLACK >(brother->left) && is_color< BLACK >(brother->right);
    if (is_color< RED >(subtree->parent) && is_color< BLACK >(brother) && is_any_child_black)
    {
        colorize< RED >(brother);
        colorize< BLACK >(subtree->parent);
    }
    else
    {
        erase_balance_case5(subtree);
    }
}

template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::erase_balance_case5(detail::Node< Key, T >* subtree)
{
    using namespace detail;
    Node< Key, T >* brother = find_brother(subtree);
    if (is_color< BLACK >(brother))
    {
        if (subtree == subtree->parent->left && is_color< BLACK >(brother->right) && is_color< RED >(brother->left))
        {
            colorize< RED >(brother);
            colorize< BLACK >(brother->left);
            rotate_right(brother);
        }
        else if (subtree == subtree->parent->right && is_color< RED >(brother->right) && is_color< BLACK >(brother->left))
        {
            colorize< RED >(brother);
            colorize< BLACK >(brother->right);
            rotate_left(brother);
        }
    }
    erase_balance_case6(subtree);
}

template< class Key, class T, class Compare >
void RedBlackTree< Key, T, Compare >::erase_balance_case6(detail::Node< Key, T >* subtree)
{
    using namespace detail;
    Node< Key, T >* brother = find_brother(subtree);
    brother->color = subtree->parent->color;
    colorize< BLACK >(subtree->parent);
    if (subtree == subtree->parent->left)
    {
        colorize< BLACK >(brother->right);
        rotate_left(subtree->parent);
    }
    else
    {
        colorize< BLACK >(brother->left);
        rotate_right(subtree->parent);
    }
}

#endif