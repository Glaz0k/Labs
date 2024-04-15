#include <iostream>
#include <exception>

#include "StackArray.h"
#include "QueueArray.h"
#include "StackUtils.h"

void stackTest()
{
    using Stack = StackArray<int>;
    std::cout << "--stackTest begin--\n";
    // 1. Zero length
    std::cout << "  1. Zero length\n";
    try
    {
        Stack zeroLength(0);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
    // 2. Base stack
    std::cout << "  2. Base stack\n";
    Stack stack1(10);
    stack1.push(3);
    stack1.push(-2);
    stack1.push(8);
    stack1.push(10);
    stack1.push(3043);
    stack1.push(-2854);
    // 3. Copy
    std::cout << "  3. Copy\n";
    Stack cpyStack(stack1);
    while (!cpyStack.isEmpty())
    {
        std::cout << cpyStack.pop() << ' ';
    }
    std::cout << '\n';
    cpyStack = stack1;
    while (!cpyStack.isEmpty())
    {
        std::cout << cpyStack.pop() << ' ';
    }
    std::cout << '\n';
    // 4. Move
    std::cout << "  4. Move\n";
    cpyStack = stack1;
    Stack moveStack(std::move(cpyStack));
    while (!moveStack.isEmpty())
    {
        std::cout << moveStack.pop() << ' ';
    }
    std::cout << '\n';
    cpyStack = stack1;
    moveStack = std::move(cpyStack);
    while (!moveStack.isEmpty())
    {
        std::cout << moveStack.pop() << ' ';
    }
    std::cout << '\n';
    // 5. Overflow
    std::cout << "  5. Overflow\n";
    Stack stack2(5);
    try
    {
        for (size_t i = 0; i < 10; ++i)
        {
            stack2.push(100);
        }
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
    // 6. Underflow
    std::cout << "  6. Underflow\n";
    try
    {
        for (size_t i = 0; i < 10; ++i)
        {
            std::cout << stack2.pop() << ' ';
        }
    }
    catch (const std::exception& e)
    {
        std::cout << '\n' << e.what() << '\n';
    }
    std::cout << "--stackTest end--\n";
}

void queueTest()
{
    using Queue = QueueArray<int>;
    std::cout << "--queueTest begin--\n";
    // 1. Zero length
    std::cout << "  1. Zero length\n";
    try
    {
        Queue zeroLength(0);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
    // 2. Base queue
    std::cout << "  2. Base queue\n";
    Queue queue1(10);
    queue1.enQueue(3);
    queue1.enQueue(-2);
    queue1.enQueue(8);
    queue1.enQueue(10);
    queue1.enQueue(3043);
    queue1.enQueue(-2854);
    // 3. Copy
    std::cout << "  3. Copy\n";
    Queue cpyQueue(queue1);
    while (!cpyQueue.isEmpty())
    {
        std::cout << cpyQueue.deQueue() << ' ';
    }
    std::cout << '\n';
    cpyQueue = queue1;
    while (!cpyQueue.isEmpty())
    {
        std::cout << cpyQueue.deQueue() << ' ';
    }
    std::cout << '\n';
    // 4. Move
    std::cout << "  4. Move\n";
    cpyQueue = queue1;
    Queue moveQueue(std::move(cpyQueue));
    while (!moveQueue.isEmpty())
    {
        std::cout << moveQueue.deQueue() << ' ';
    }
    std::cout << '\n';
    cpyQueue = queue1;
    moveQueue = std::move(cpyQueue);
    while (!moveQueue.isEmpty())
    {
        std::cout << moveQueue.deQueue() << ' ';
    }
    std::cout << '\n';
    // 5. Overflow
    std::cout << "  5. Overflow\n";
    Queue queue2(5);
    try
    {
        for (size_t i = 0; i < 10; ++i)
        {
            queue2.enQueue(100);
        }
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
    // 6. Underflow
    std::cout << "  6. Underflow\n";
    try
    {
        for (size_t i = 0; i < 10; ++i)
        {
            std::cout << queue2.deQueue() << ' ';
        }
    }
    catch (const std::exception& e)
    {
        std::cout << '\n' << e.what() << '\n';
    }
    // 7. Cyclic run
    std::cout << "  6. Cyclic run\n";
    Queue queue3(5);
    for (size_t i = 0; i < 30; i += 2)
    {
        queue3.enQueue(i);
        queue3.enQueue(i + 1);
        std::cout << queue3.deQueue() << ' ';
        std::cout << queue3.deQueue() << ' ';
        std::cout << queue3.isEmpty() << '\n';
    }
    std::cout << "--queueTest end--\n";
}

void bracketsTest()
{
    std::cout << "--bracketsTest begin--\n";
    const char* testBrackets[] = {
        "( ) { } [ ]",     // 1
        "( [ { } ] )",     // 1
        "",                // 1
        "( [ ) ]",         // 0
        "(",               // 0
        "]",               // 0
        "( ( ( ( ) ) ) )", // depth
        "( ) [ { [ {"      // depth
    };
    std::cout << "depth: 3\n";
    for (const char* test : testBrackets)
    {
        try
        {
            std::cout << checkBalanceBrackets(test, 3) << ' ' << test << '\n';
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << ' ' << test << '\n';
        }
    }
    std::cout << "--bracketsTest end--\n";
}

int main()
{
    stackTest();
    queueTest();
    bracketsTest();
    return 0;
}
