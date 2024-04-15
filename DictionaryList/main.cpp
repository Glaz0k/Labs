#include <iostream>
#include "DictionaryList.h"

void testDictionary()
{
	std::cout << "T1: Insert\n";
	DictionaryList<int> first;
	const int insertSize = 8;
	int toInsert[insertSize] = { 2, 8, 7, -2, 7, 3, 10, 2 };
	bool isInsert[insertSize] = {};
	for (int i = 0; i < insertSize; ++i)
	{
		isInsert[i] = first.insert(toInsert[i]);
	}
	std::cout << "Tested result:\n\t" << first << "\n\t";
	for (int i = 0; i < insertSize; ++i)
	{
		std::cout << isInsert[i] << ' ';
	}
	std::cout << '\n';
	std::cout << "Expected result:\n\t" << "-2 2 3 7 8 10\n\t" << "1 1 1 1 0 1 1 0\n";

	std::cout << "T2: Remove\n";
	const int removeSize = 5;
	int toRemove[removeSize] = {7, 6, -2, 10, 12};
	bool isRemove[removeSize] = {};
	for (int i = 0; i < removeSize; ++i)
	{
		isRemove[i] = first.remove(toRemove[i]);
	}
	std::cout << "Tested result:\n\t" << first << "\n\t";
	for (int i = 0; i < removeSize; ++i)
	{
		std::cout << isRemove[i] << ' ';
	}
	std::cout << '\n';
	std::cout << "Expected result:\n\t" << "2 3 8\n\t" << "1 0 1 1 0\n";

	std::cout << "T3: Search\n";
	int toSearch[] = { -1, 5, 2, 12, 8 };
	std::cout << "Tested result:\n\t";
	for (int x : toSearch)
	{
		std::cout << first.search(x) << ' ';
	}
	std::cout << '\n';
	std::cout << "Expected result:\n\t" << "0 0 1 0 1\n";

	std::cout << "T4: Merge\n";
	DictionaryList<int> second;
	int toMerge[] = { -5, -3, 0, 3, 4, 5, 9, 12, 15 };
	for (int x : toMerge)
	{
		second.insert(x);
	}
	first.merge(second);
	std::cout << "Tested result:\n\t" << first << "\n\t" << second << '\n';
	std::cout << "Expected result:\n\t" << "-5 -3 0 2 3 4 5 8 9 12 15\n\t\n";

	std::cout << "T5: Subtract\n";
	int toSubtract[] = { -5, -1, 0, 3, 4, 12, 17 };
	for (int x : toSubtract)
	{
		second.insert(x);
	}
	first.subtract(second);
	std::cout << "Tested result:\n\t" << first << "\n\t" << second << '\n';
	std::cout << "Expected result:\n\t" << "-3 2 5 8 9 15\n\t" << "-5 -1 0 3 4 12 17\n";

	std::cout << "T6: Intersect\n";
	int toIntersect[] = { 5, 9 };
	for (int x : toIntersect)
	{
		second.insert(x);
	}
	DictionaryList<int> intersected = intersect(first, second);
	std::cout << "Tested result:\n\t" << intersected << '\n';
	std::cout << "Expected result:\n\t" << "5 9\n";
}

int main()
{
	testDictionary();
	return 0;
}
