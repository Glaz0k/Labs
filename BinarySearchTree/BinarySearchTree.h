#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <ostream>
#include <stack>
#include <queue>

namespace detail
{
    template <class T>
    struct Node
    {
        Node(T key) :
            key_(key),
            left_(nullptr),
            right_(nullptr),
            p_(nullptr)
        {}
        Node(T key, Node* p) :
            key_(key),
            left_(nullptr),
            right_(nullptr),
            p_(p)
        {}
        Node(T key, Node* left, Node* right, Node* p) :
            key_(key),
            left_(left),
            right_(right),
            p_(p)
        {}
        T key_;
        Node* left_;
        Node* right_;
        Node* p_;
    };
}

template <class T>
class BinarySearchTree
{
public:
    BinarySearchTree();
    BinarySearchTree(const BinarySearchTree<T>& scr) = delete;
    BinarySearchTree(BinarySearchTree<T>&& src) noexcept;
    BinarySearchTree <T>& operator= (const BinarySearchTree <T>& src) = delete;
    BinarySearchTree <T>& operator= (BinarySearchTree <T>&& src) noexcept;
    ~BinarySearchTree();
    bool searchKeyIterative(const T& key) const;
    bool insertNode(const T& key);
    bool deleteNode(const T& key);
    void output(std::ostream& out) const;
    int getNumberOfNodes() const;
    int getHeight() const;
    void inorderWalkIterative(std::ostream& out) const;
    void inorderWalk(std::ostream& out) const;
    void walkByLevels(std::ostream& out) const;
    bool isSimilar(const BinarySearchTree<T>& other) const;
    bool isIdenticalKey(const BinarySearchTree<T>& other) const;
private:
    detail::Node<T>* root_;
    void clear(detail::Node<T>* treeNode) noexcept;
    detail::Node<T>* searchNodeIterative(const T& key) const;
    void output(std::ostream& out, const detail::Node<T>* root) const;
    int getNumberOfNodes(const detail::Node<T>* node) const;
    int getHeight(const detail::Node<T>* node) const;
    void inorderWalk(std::ostream& out, const detail::Node<T>* node) const;
    void inorderWalkStart(std::stack<detail::Node<T>*>& nodeStack) const;
    void inorderWalkStep(std::stack<detail::Node<T>*>& nodeStack) const;
};

template<class T>
inline BinarySearchTree<T>::BinarySearchTree() :
    root_(nullptr)
{}

template<class T>
inline BinarySearchTree<T>::BinarySearchTree(BinarySearchTree<T>&& src) noexcept :
    root_(src.root_)
{
    src.root_ = nullptr;
}

template<class T>
inline BinarySearchTree<T>& BinarySearchTree<T>::operator=(BinarySearchTree<T>&& src) noexcept
{
    if (this == &src)
        return *this;
    clear(root_);
    root_ = src.root_;
    src.root_ = nullptr;
    return *this;
}

template<class T>
inline BinarySearchTree<T>::~BinarySearchTree()
{
    clear(root_);
}

template<class T>
inline bool BinarySearchTree<T>::searchKeyIterative(const T& key) const
{
    return static_cast<bool>(searchNodeIterative(key));
}

template<class T>
inline bool BinarySearchTree<T>::insertNode(const T& key)
{
    detail::Node<T>* newNode = new detail::Node<T>(key);
    detail::Node<T>* curr = root_;
    detail::Node<T>* p = nullptr;
    bool inserted = true;
    while (curr && inserted)
    {
        if (key < curr->key_)
        {
            p = curr;
            curr = curr->left_;
        }
        else if (curr->key_ < key)
        {
            p = curr;
            curr = curr->right_;
        }
        else
            inserted = false;
    }
    if (inserted)
    {
        if (p)
        {
            newNode->p_ = p;
            if (key < p->key_)
                p->left_ = newNode;
            else
                p->right_ = newNode;
        }
        else
        {
            root_ = newNode;
        }
    }
    else
        delete newNode;
    return inserted;
}

template<class T>
inline bool BinarySearchTree<T>::deleteNode(const T& key)
{
    // search for node
    detail::Node<T>* deletedNode = searchNodeIterative(key);
    if (!deletedNode)
        return false;

    // re-build tree
    detail::Node<T>* inPlaceNode = nullptr;
    if (deletedNode->left_ && deletedNode->right_)
    {
        detail::Node<T>* minNode = deletedNode->right_;
        if (minNode->left_)
        {
            while (minNode->left_)
            {
                minNode = minNode->left_;
            }
            minNode->p_->left_ = minNode->right_;
            if (minNode->right_)
                minNode->right_->p_ = minNode->p_;
            minNode->right_ = deletedNode->right_;
        }

        minNode->left_ = deletedNode->left_;

        inPlaceNode = minNode;
    }
    else if (deletedNode->left_)
    {
        inPlaceNode = deletedNode->left_;
    }
    else if (deletedNode->right_)
    {
        inPlaceNode = deletedNode->right_;
    }

    // if not leaf
    if (inPlaceNode)
        inPlaceNode->p_ = deletedNode->p_;

    // parent assingment
    if (deletedNode->p_)
    {
        if (key < deletedNode->p_->key_)
            deletedNode->p_->left_ = inPlaceNode;
        else
            deletedNode->p_->right_ = inPlaceNode;
    }
    else
    {
        root_ = inPlaceNode;
    }

    // deletion
    delete deletedNode;
    return true;
}

template<class T>
inline void BinarySearchTree<T>::output(std::ostream& out) const
{
    output(out, root_);
}

template<class T>
inline int BinarySearchTree<T>::getNumberOfNodes() const
{
    return getNumberOfNodes(root_);
}

template<class T>
inline int BinarySearchTree<T>::getHeight() const
{
    return std::max(0, getHeight(root_) - 1);
}

template<class T>
inline void BinarySearchTree<T>::inorderWalkIterative(std::ostream& out) const
{
    using NodeT = detail::Node<T>;
    std::stack<NodeT*> nodeStack;
    NodeT* topNode = root_;
    while (topNode || !nodeStack.empty())
    {
        if (!nodeStack.empty())
        {
            topNode = nodeStack.top();
            nodeStack.pop();
            out << topNode->key_ << ' ';
            topNode = topNode->right_;
        }
        while (topNode)
        {
            nodeStack.push(topNode);
            topNode = topNode->left_;
        }
    }
}

template<class T>
inline void BinarySearchTree<T>::inorderWalk(std::ostream& out) const
{
    inorderWalk(out, root_);
}

template<class T>
inline void BinarySearchTree<T>::walkByLevels(std::ostream& out) const
{
    if (!root_)
        return;
    using NodeT = detail::Node<T>;
    std::queue<NodeT*> nodeQueue;
    nodeQueue.push(root_);
    while (!nodeQueue.empty())
    {
        NodeT* topNode = nodeQueue.front();
        nodeQueue.pop();
        out << topNode->key_ << ' ';
        if (topNode->left_)
            nodeQueue.push(topNode->left_);
        if (topNode->right_)
            nodeQueue.push(topNode->right_);
    }
}

template<class T>
inline bool BinarySearchTree<T>::isSimilar(const BinarySearchTree<T>& other) const
{
    using NodeT = detail::Node<T>;
    std::stack<NodeT*> thisNodeStack;
    std::stack<NodeT*> otherNodeStack;
    inorderWalkStart(thisNodeStack);
    other.inorderWalkStart(otherNodeStack);
    bool similar = true;
    while (similar && !thisNodeStack.empty() && !otherNodeStack.empty())
    {
        if (thisNodeStack.top()->key_ == otherNodeStack.top()->key_)
        {
            inorderWalkStep(thisNodeStack);
            other.inorderWalkStep(otherNodeStack);
        }
        else
            similar = false;
    }
    if (!thisNodeStack.empty() || !otherNodeStack.empty())
        similar = false;
    return similar;
}

template<class T>
inline bool BinarySearchTree<T>::isIdenticalKey(const BinarySearchTree<T>& other) const
{
    using NodeT = detail::Node<T>;
    std::stack<NodeT*> thisNodeStack;
    std::stack<NodeT*> otherNodeStack;
    inorderWalkStart(thisNodeStack);
    other.inorderWalkStart(otherNodeStack);
    bool identicalKey = false;
    while (!identicalKey && !thisNodeStack.empty() && !otherNodeStack.empty())
    {
        NodeT* thisTop = thisNodeStack.top();
        NodeT* otherTop = otherNodeStack.top();
        if (thisTop->key_ < otherTop->key_)
        {
            inorderWalkStep(thisNodeStack);
        }
        else if (thisTop->key_ > otherTop->key_)
        {
            other.inorderWalkStep(otherNodeStack);
        }
        else
            identicalKey = true;
    }
    return identicalKey;
}

template<class T>
inline void BinarySearchTree<T>::clear(detail::Node<T>* treeNode) noexcept
{
    if (!treeNode)
        return;
    clear(treeNode->left_);
    clear(treeNode->right_);
    delete treeNode;
}

template<class T>
inline detail::Node<T>* BinarySearchTree<T>::searchNodeIterative(const T& key) const
{
    detail::Node<T>* curr = root_;
    while (curr)
    {
        if (key < curr->key_)
            curr = curr->left_;
        else if (curr->key_ < key)
            curr = curr->right_;
        else
            return curr;
    }
    return nullptr;
}

template<class T>
inline void BinarySearchTree<T>::output(std::ostream& out, const detail::Node<T>* root) const
{
    if (root)
    {
        out << '(' << root->key_;
        output(out, root->left_);
        output(out, root->right_);
        out << ')';
    }
}

template<class T>
inline int BinarySearchTree<T>::getNumberOfNodes(const detail::Node<T>* node) const
{
    if (!node)
        return 0;
    return 1 + getNumberOfNodes(node->left_) + getNumberOfNodes(node->right_);
}

template<class T>
inline int BinarySearchTree<T>::getHeight(const detail::Node<T>* node) const
{
    if (!node)
        return 0;
    return 1 + std::max(getHeight(node->left_), getHeight(node->right_));
}

template<class T>
inline void BinarySearchTree<T>::inorderWalk(std::ostream& out, const detail::Node<T>* node) const
{
    if (!node)
        return;

    inorderWalk(out, node->left_);
    out << node->key_ << ' ';
    inorderWalk(out, node->right_);
}

template<class T>
inline void BinarySearchTree<T>::inorderWalkStart(std::stack<detail::Node<T>*>& nodeStack) const
{
    detail::Node<T>* topNode = root_;
    while (topNode)
    {
        nodeStack.push(topNode);
        topNode = topNode->left_;
    }
}

template<class T>
inline void BinarySearchTree<T>::inorderWalkStep(std::stack<detail::Node<T>*>& nodeStack) const
{
    if (nodeStack.empty())
        return;
    detail::Node<T>* topNode = nodeStack.top();
    nodeStack.pop();
    topNode = topNode->right_;
    while (topNode)
    {
        nodeStack.push(topNode);
        topNode = topNode->left_;
    }
}

#endif