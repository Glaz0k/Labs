#include "StackUtils.h"
#include "StackArray.h"
#include <stdexcept>

bool checkBalanceBrackets(const char* text, const int maxDeep)
{
	if (maxDeep < 1)
	{
		throw std::invalid_argument("Invalid maximum depth");
	}
	StackArray<char> stack(maxDeep);
	for (std::size_t i = 0; text[i] != '\0'; ++i)
	{
		try
		{
			switch (text[i])
			{
			case '(':
				stack.push(')');
				break;
			case '[':
				stack.push(']');
				break;
			case '{':
				stack.push('}');
				break;
			case ')':
			case ']':
			case '}':
				if (stack.isEmpty() || stack.pop() != text[i])
				{
					return false;
				}
				break;
			default:
				break;
			}
		}
		catch (const StackOverflow&)
		{
			throw std::runtime_error("Maximum depth exceeded");
		}
	}
	return stack.isEmpty();
}