#ifndef TREE_CONST_ITERATOR
#define TREE_CONST_ITERATOR

#include <iterator>
#include <utility>
#include "TreeNode.hpp"

template< class Key, class T, class Compare >
class RedBlackTree;

template< class Key, class T, class Compare = std::less< Key > >
class TreeConstIterator : public std::iterator< std::bidirectional_iterator_tag, T >
{
    friend class RedBlackTree< Key, T, Compare >;
public:
    TreeConstIterator();
    TreeConstIterator(const TreeConstIterator< Key, T, Compare >&) = default;
    TreeConstIterator< Key, T, Compare >& operator=(const TreeConstIterator< Key, T, Compare >&) = default;
    ~TreeConstIterator() = default;
    TreeConstIterator< Key, T, Compare >& operator++();
    TreeConstIterator< Key, T, Compare >& operator--();
    TreeConstIterator< Key, T, Compare > operator++(int);
    TreeConstIterator< Key, T, Compare > operator--(int);
    bool operator==(const TreeConstIterator< Key, T, Compare >& rhs) const;
    bool operator!=(const TreeConstIterator< Key, T, Compare >& rhs) const;
    const std::pair< Key, T >* operator->() const;
    const std::pair< Key, T >& operator*() const;
private:
    detail::Node< Key, T >* node_;
    explicit TreeConstIterator(const detail::Node< Key, T >* node);
};

template<class Key, class T, class Compare>
TreeConstIterator<Key, T, Compare>::TreeConstIterator() :
    node_(nullptr)
{}

template<class Key, class T, class Compare>
TreeConstIterator<Key, T, Compare>& TreeConstIterator<Key, T, Compare>::operator++()
{
    if (node_->right)
    {
        node_ = node_->right;
        while (node_->left)
        {
            node_ = node_->left;
        }
        return *this;
    }
    while (node_->parent && node_->parent->right == node_)
    {
        node_ = node_->parent;
    }
    node_ = node_->parent;
    return *this;
}

template<class Key, class T, class Compare>
TreeConstIterator<Key, T, Compare>& TreeConstIterator<Key, T, Compare>::operator--()
{
    if (node_->left)
    {
        node_ = node_->left;
        while (node_->right)
        {
            node_ = node_->right;
        }
        return *this;
    }
    while (node_->parent && node_->parent->left == node_)
    {
        node_ = node_->parent;
    }
    node_ = node_->parent;
    return *this;
}

template<class Key, class T, class Compare>
TreeConstIterator<Key, T, Compare> TreeConstIterator<Key, T, Compare>::operator++(int)
{
    TreeConstIterator< Key, T, Compare > result(*this);
    ++(*this);
    return result;
}

template<class Key, class T, class Compare>
TreeConstIterator<Key, T, Compare> TreeConstIterator<Key, T, Compare>::operator--(int)
{
    TreeConstIterator< Key, T, Compare > result(*this);
    --(*this);
    return result;
}

template<class Key, class T, class Compare>
bool TreeConstIterator<Key, T, Compare>::operator==(const TreeConstIterator<Key, T, Compare>& rhs) const
{
    return node_ == rhs.node_;
}

template<class Key, class T, class Compare>
bool TreeConstIterator<Key, T, Compare>::operator!=(const TreeConstIterator<Key, T, Compare>& rhs) const
{
    return !(node_ == rhs.node_);
}

template<class Key, class T, class Compare>
const std::pair<Key, T>* TreeConstIterator<Key, T, Compare>::operator->() const
{
    return std::addressof(node_->data);
}

template<class Key, class T, class Compare>
const std::pair<Key, T>& TreeConstIterator<Key, T, Compare>::operator*() const
{
    return node_->data;
}

template<class Key, class T, class Compare>
TreeConstIterator<Key, T, Compare>::TreeConstIterator(const detail::Node< Key, T >* node) :
    node_(const_cast<detail::Node< Key, T >*>(node))
{}

#endif