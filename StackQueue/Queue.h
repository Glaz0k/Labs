#ifndef QUEUE_H
#define QUEUE_H

#include <exception>

template <class T>
class Queue
{
public:
	virtual ~Queue() = default;
	virtual void enQueue(const T& e) = 0;
	virtual T deQueue() = 0;
	virtual bool isEmpty() const = 0;
};

class QueueOverflow : public std::exception
{
	const char* what() const noexcept override
	{
		return "Queue overflow";
	}
};

class QueueUnderflow : public std::exception
{
	const char* what() const noexcept override
	{
		return "Queue underflow";
	}
};
class WrongQueueSize : public std::exception
{
	const char* what() const noexcept override
	{
		return "Invalid queue size";
	}
};

#endif // !QUEUE_H