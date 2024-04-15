#ifndef STACK_H
#define STACK_H

#include <exception>

template <class T>
class Stack
{
public:
	virtual ~Stack() = default;
	virtual void push(const T& e) = 0;
	virtual T pop() = 0;
	virtual bool isEmpty() const = 0;
};

class StackOverflow : public std::exception
{
	const char* what() const noexcept override
	{
		return "Stack overflow";
	}
};

class StackUnderflow : public std::exception
{
	const char* what() const noexcept override
	{
		return "Stack underflow";
	}
};
class WrongStackSize : public std::exception
{
	const char* what() const noexcept override
	{
		return "Invalid stack size";
	}
};

#endif