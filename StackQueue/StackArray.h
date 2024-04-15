#ifndef STACK_ARRAY_H
#define STACK_ARRAY_H

#include "Stack.h"
#include <utility>
#include <ostream>

template <class T>
class StackArray : public Stack<T>
{
public:
	StackArray(std::size_t);
	StackArray(const StackArray<T>& other);
	StackArray(StackArray&& other) noexcept;
	StackArray<T>& operator=(const StackArray<T>& other);
	StackArray<T>& operator=(StackArray<T>&& other) noexcept;
	virtual ~StackArray();
	virtual void push(const T& e) override;
	virtual T pop() override;
	virtual bool isEmpty() const override;
	void swap(StackArray<T>& other) noexcept;
private:
	T* array_;
	std::size_t size_;
	std::size_t head_;
};

template<class T>
inline StackArray<T>::StackArray(std::size_t size) :
	size_(size),
	head_(0)
{
	if (size < 1)
	{
		throw WrongStackSize();
	}
	array_ = new T[size]{};
}

template<class T>
inline StackArray<T>::StackArray(const StackArray<T>& other) :
	size_(other.size_),
	head_(other.head_)
{
	array_ = new T[other.size_]{};
	try
	{
		for (std::size_t i = 0; i < other.head_; ++i)
		{
			array_[i] = other.array_[i];
		}
	}
	catch (...)
	{
		delete[] array_;
		throw;
	}
}

template<class T>
inline StackArray<T>::StackArray(StackArray&& other) noexcept :
	array_(other.array_),
	size_(other.size_),
	head_(other.head_)
{
	other.array_ = nullptr;
}

template<class T>
inline StackArray<T>::~StackArray()
{
	delete[] array_;
}

template<class T>
inline void StackArray<T>::push(const T& e)
{
	if (head_ == size_)
	{
		throw StackOverflow();
	}
	array_[head_] = e;
	++head_;
}

template<class T>
inline T StackArray<T>::pop()
{
	if (head_ == 0)
	{
		throw StackUnderflow();
	}
	T result = array_[head_ - 1];
	--head_;
	return result;
}

template<class T>
inline bool StackArray<T>::isEmpty() const
{
	return (head_ == 0);
}

template<class T>
inline StackArray<T>& StackArray<T>::operator=(const StackArray<T>& other)
{
	if (this == &other)
	{
		return *this;
	}
	StackArray<T> temp = other;
	swap(temp);
	return *this;
}

template<class T>
inline StackArray<T>& StackArray<T>::operator=(StackArray<T>&& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}
	swap(other);
	return *this;
}

template<class T>
inline void StackArray<T>::swap(StackArray<T>& other) noexcept
{
	std::swap(array_, other.array_);
	std::swap(size_, other.size_);
	std::swap(head_, other.head_);
}

#endif // !STACK_ARRAY_H