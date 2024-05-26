#ifndef TREE_ITERATOR_HPP
#define TREE_ITERATOR_HPP

#include <iterator>
#include <utility>
#include "TreeNode.hpp"

template< class Key, class T, class Compare >
class RedBlackTree;

template< class Key, class T, class Compare = std::less< Key > >
class TreeIterator : public std::iterator< std::bidirectional_iterator_tag, T >
{
    friend class RedBlackTree< Key, T, Compare >;
public:
    TreeIterator();
    TreeIterator(const TreeIterator< Key, T, Compare >&) = default;
    TreeIterator< Key, T, Compare >& operator=(const TreeIterator< Key, T, Compare >&) = default;
    ~TreeIterator() = default;
    TreeIterator< Key, T, Compare >& operator++();
    TreeIterator< Key, T, Compare >& operator--();
    TreeIterator< Key, T, Compare > operator++(int);
    TreeIterator< Key, T, Compare > operator--(int);
    bool operator==(const TreeIterator< Key, T, Compare >& rhs) const;
    bool operator!=(const TreeIterator< Key, T, Compare >& rhs) const;
    std::pair< Key, T >* operator->();
    std::pair< Key, T >& operator*();
    const std::pair< Key, T >* operator->() const;
    const std::pair< Key, T >& operator*() const;
private:
    detail::Node< Key, T >* node_;
    explicit TreeIterator(detail::Node< Key, T >* node);
};

template<class Key, class T, class Compare>
TreeIterator<Key, T, Compare>::TreeIterator() :
    node_(nullptr)
{}

template<class Key, class T, class Compare>
TreeIterator<Key, T, Compare>& TreeIterator<Key, T, Compare>::operator++()
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
TreeIterator<Key, T, Compare>& TreeIterator<Key, T, Compare>::operator--()
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
TreeIterator<Key, T, Compare> TreeIterator<Key, T, Compare>::operator++(int)
{
    TreeIterator< Key, T, Compare > result(*this);
    ++(*this);
    return result;
}

template<class Key, class T, class Compare>
TreeIterator<Key, T, Compare> TreeIterator<Key, T, Compare>::operator--(int)
{
    TreeIterator< Key, T, Compare > result(*this);
    --(*this);
    return result;
}

template<class Key, class T, class Compare>
bool TreeIterator<Key, T, Compare>::operator==(const TreeIterator<Key, T, Compare>& rhs) const
{
    return node_ == rhs.node_;
}

template<class Key, class T, class Compare>
bool TreeIterator<Key, T, Compare>::operator!=(const TreeIterator<Key, T, Compare>& rhs) const
{
    return !(node_ == rhs.node_);
}

template<class Key, class T, class Compare>
std::pair< Key, T >* TreeIterator<Key, T, Compare>::operator->()
{
    return std::addressof(node_->data);
}

template<class Key, class T, class Compare>
std::pair<Key, T>& TreeIterator<Key, T, Compare>::operator*()
{
    return node_->data;
}

template<class Key, class T, class Compare>
const std::pair<Key, T>* TreeIterator<Key, T, Compare>::operator->() const
{
    return std::addressof(node_->data);
}

template<class Key, class T, class Compare>
const std::pair<Key, T>& TreeIterator<Key, T, Compare>::operator*() const
{
    return node_->data;
}

template<class Key, class T, class Compare>
TreeIterator<Key, T, Compare>::TreeIterator(detail::Node< Key, T >* node) :
    node_(node)
{}

#endif