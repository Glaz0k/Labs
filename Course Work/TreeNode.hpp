#ifndef TREE_NODE_HPP
#define TREE_NODE_HPP

#include <utility>
#include <cstddef>

namespace detail
{
    enum Color
    {
        BLACK,
        RED
    };

    template< class Key, class T >
    struct Node
    {
        std::pair< Key, T > data;
        Node* parent;
        Node* left;
        Node* right;
        Color color;

        template< class... Args >
        Node(Node* nodeParent, Node* nodeLeft, Node* nodeRight, Args&&... args);
        ~Node() = default;
    };

    template< class Key, class T >
    template< class... Args >
    Node< Key, T >::Node(Node* nodeParent, Node* nodeLeft, Node* nodeRight, Args&&... args) :
        data(std::forward< Args... >(args...)),
        parent(nodeParent),
        left(nodeLeft),
        right(nodeRight),
        color(Color::RED)
    {}
}

#endif