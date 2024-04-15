#ifndef DICTIONARY_LIST_HPP
#define DICTIONARY_LIST_HPP

#include <exception>
#include <ostream>

template < class KeyT >
class DictionaryList
{
public:
	DictionaryList() :
		head_(nullptr)
	{}
	DictionaryList(const DictionaryList&) = delete;
	DictionaryList& operator=(const DictionaryList&) = delete;
	DictionaryList(DictionaryList&& other) noexcept :
		head_(other.head_)
	{
		other.head_ = nullptr;
	}
	DictionaryList& operator=(DictionaryList&& other) noexcept
	{
		if (this != &other)
		{
			std::swap(head_, other.head_);
		}
		return *this;
	}
	~DictionaryList()
	{
		if (head_)
		{
			Node* nextNode = head_->next;
			delete head_;
			while (nextNode)
			{
				head_ = nextNode;
				nextNode = head_->next;
				delete head_;
			}
		}
	}
	bool insert(const KeyT& k)
	{
		Node* newNode = new Node(k);
		Node** currentNodePtr = &head_;
		while (*currentNodePtr)
		{
			if (k < (*currentNodePtr)->data)
			{
				Node* nextNode = *currentNodePtr;
				*currentNodePtr = newNode;
				newNode->next = nextNode;
				return true;
			}
			if (k == (*currentNodePtr)->data)
			{
				delete newNode;
				return false;
			}
			currentNodePtr = &((*currentNodePtr)->next);
		}
		*currentNodePtr = newNode;
		return true;
	}
	bool remove(const KeyT& k)
	{
		Node** currentNodePtr = &head_;
		while (*currentNodePtr)
		{
			if (k == (*currentNodePtr)->data)
			{
				Node* nextNode = (*currentNodePtr)->next;
				delete (*currentNodePtr);
				*currentNodePtr = nextNode;
				return true;
			}
			currentNodePtr = &((*currentNodePtr)->next);
		}
		return false;
	}
	bool search(const KeyT& k) const
	{
		Node* currentNode = head_;
		while (currentNode)
		{
			if (k < currentNode->data)
			{
				return false;
			}
			if (k == currentNode->data)
			{
				return true;
			}
			currentNode = currentNode->next;
		}
		return false;
	}
	void merge(DictionaryList& other)
	{
		if (!other.head_)
		{
			return;
		}
		Node** thisNodePtr = &head_;
		Node* thisNode = nullptr;
		Node* mergedNode = other.head_;
		Node* mergedNextNode = nullptr;
		while (*thisNodePtr && mergedNode)
		{
			if (mergedNode->data < (*thisNodePtr)->data)
			{
				thisNode = *thisNodePtr;
				*thisNodePtr = mergedNode;
				while (mergedNode->next && mergedNode->next->data < thisNode->data)
				{
					mergedNode = mergedNode->next;
				}
				mergedNextNode = mergedNode->next;
				mergedNode->next = thisNode;
				mergedNode = mergedNextNode;
				thisNodePtr = &thisNode;
			}
			else if (mergedNode->data == (*thisNodePtr)->data)
			{
				mergedNextNode = mergedNode->next;
				delete mergedNode;
				mergedNode = mergedNextNode;
			}
			else
			{
				thisNodePtr = &((*thisNodePtr)->next);
			}
		}
		if (mergedNode)
		{
			*thisNodePtr = mergedNode;
		}
		other.head_ = nullptr;
	}
	void subtract(const DictionaryList& other)
	{
		if (!(head_ && other.head_))
		{
			return;
		}
		Node* removedNode = other.head_;
		Node** currentNodePtr = &head_;
		Node* nextNode = nullptr;
		while (*currentNodePtr && removedNode)
		{
			if (removedNode->data < (*currentNodePtr)->data)
			{
				removedNode = removedNode->next;
			}
			else if (removedNode->data == (*currentNodePtr)->data)
			{
				nextNode = (*currentNodePtr)->next;
				delete (*currentNodePtr);
				*currentNodePtr = nextNode;
			}
			else
			{
				currentNodePtr = &(*currentNodePtr)->next;
			}
		}
	}
	friend DictionaryList intersect(const DictionaryList& first, const DictionaryList& second)
	{
		DictionaryList intersectList;
		Node** intersectPtr = &(intersectList.head_);
		Node* node1 = first.head_;
		Node* node2 = second.head_;
		while (node1 && node2)
		{
			if (node1->data < node2->data)
			{
				node1 = node1->next;
			}
			else if (node1->data > node2->data)
			{
				node2 = node2->next;
			}
			else
			{
				try
				{
					*intersectPtr = new Node(node1->data);
				}
				catch (const std::bad_alloc&)
				{

					throw;
				}
				intersectPtr = &((*intersectPtr)->next);
				node1 = node1->next;
				node2 = node2->next;
			}
		}
		return intersectList;
	}
	friend std::ostream& operator<<(std::ostream& out, const DictionaryList& list)
	{
		if (list.head_)
		{
			Node* currentNode = list.head_;
			out << currentNode->data;
			currentNode = currentNode->next;
			while (currentNode)
			{
				out << ' ' << currentNode->data;
				currentNode = currentNode->next;
			}
		}
		return out;
	}
private:
	struct Node
	{
		Node(const KeyT& newData) :
			data(newData),
			next(nullptr)
		{}
		KeyT data;
		Node* next;
	};
	Node* head_;
};

#endif