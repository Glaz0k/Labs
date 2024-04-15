#ifndef QUEUE_ARRAY_H
#define QUEUE_ARRAY_H

#include "Queue.h"
#include <utility>

template <class T>
class QueueArray : public Queue<T>
{
public:
	QueueArray(std::size_t);
	QueueArray(const QueueArray<T>& other);
	QueueArray(QueueArray<T>&& other) noexcept;
	QueueArray<T>& operator=(const QueueArray<T>& other);
	QueueArray<T>& operator=(QueueArray<T>&& other) noexcept;
	virtual ~QueueArray();
	virtual void enQueue(const T& e) override;
	virtual T deQueue() override;
	virtual bool isEmpty() const override;
	void swap(QueueArray<T>& other) noexcept;
private:
	T* array_;
	std::size_t size_;
	std::size_t head_;
	std::size_t tail_;
};

template<class T>
inline QueueArray<T>::QueueArray(std::size_t size) :
	size_(size),
	head_(0),
	tail_(size)
{
	if (size < 1)
	{
		throw WrongQueueSize();
	}
	array_ = new T[size];
}

template<class T>
inline QueueArray<T>::QueueArray(const QueueArray<T>& other) :
	size_(other.size_),
	head_(other.head_),
	tail_(other.tail_)
{
	array_ = new T[size_]{};
	if (other.isEmpty())
	{
		return;
	}
	try
	{
		size_t i = other.tail_;
		while (i != other.head_)
		{
			array_[i] = other.array_[i];
			i = (i < other.size_) ? i + 1 : 0;
		}
	}
	catch (...)
	{
		delete[] array_;
		throw;
	}
}

template<class T>
inline QueueArray<T>::QueueArray(QueueArray<T>&& other) noexcept :
	array_(other.array_),
	size_(other.size_),
	head_(other.head_),
	tail_(other.tail_)
{
	other.array_ = nullptr;
}

template<class T>
inline QueueArray<T>::~QueueArray()
{
	delete[] array_;
}

template<class T>
inline void QueueArray<T>::enQueue(const T& e)
{
	if (head_ == tail_)
	{
		throw QueueOverflow();
	}
	array_[head_] = e;
	if (tail_ == size_)
	{
		tail_ = head_;
	}
	head_ = (head_ + 1 == size_) ? 0 : head_ + 1;
}

template<class T>
inline T QueueArray<T>::deQueue()
{
	if (tail_ == size_)
	{
		throw QueueUnderflow();
	}
	T result = array_[tail_];
	tail_ = (tail_ + 1 == size_) ? 0 : tail_ + 1;
	if (tail_ == head_)
	{
		tail_ = size_;
	}
	return result;
}

template<class T>
inline bool QueueArray<T>::isEmpty() const
{
	return (tail_ == size_);
}

template<class T>
inline QueueArray<T>& QueueArray<T>::operator=(const QueueArray<T>& other)
{
	if (this == &other)
	{
		return *this;
	}
	QueueArray<T> temp = other;
	swap(temp);
	return *this;
}

template<class T>
inline QueueArray<T>& QueueArray<T>::operator=(QueueArray<T>&& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}
	swap(other);
	return *this;
}

template<class T>
inline void QueueArray<T>::swap(QueueArray<T>& other) noexcept
{
	std::swap(array_, other.array_);
	std::swap(size_, other.size_);
	std::swap(head_, other.head_);
	std::swap(tail_, other.tail_);
}

#endif // !QUEUE_ARRAY_H