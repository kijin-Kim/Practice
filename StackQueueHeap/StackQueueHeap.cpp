#include <iostream>
#include <assert.h>
#include "Stack/QueueBasedStack.h"
#include "Queue/StackBasedQueue.h"
#include "Heap/Heap.h"

template<template <typename> class StackContainerType>
void TestStack()
{
	StackContainerType<int> stack;

	// 1. 새 스택은 비어 있어야 한다.
	assert(stack.empty());
	assert(stack.size() == 0);

	// 2. push 후 top으로 마지막 원소를 확인할 수 있어야 한다.
	stack.push(10);
	stack.push(20);
	stack.push(30);

	assert(stack.size() == 3);
	assert(stack.top() == 30);

	// 3. pop하면 가장 마지막에 넣은 원소가 제거되어야 한다.
	stack.pop();

	assert(stack.size() == 2);
	assert(stack.top() == 20);

	// 4. 모두 pop하면 다시 비어 있어야 한다.
	stack.pop();
	stack.pop();

	assert(stack.empty());
	assert(stack.size() == 0);
}

template<template <typename> class QueueContainerType>
void TestQueue()
{
	QueueContainerType<int> queue;

	// 1. 새 큐는 비어 있어야 한다.
	assert(queue.empty());
	assert(queue.size() == 0);

	// 2. enqueue한 순서대로 저장되어야 한다.
	queue.enqueue(10);
	queue.enqueue(20);
	queue.enqueue(30);

	assert(queue.size() == 3);
	assert(queue.front() == 10);

	// 3. dequeue하면 가장 먼저 넣은 원소가 제거되어야 한다.
	queue.dequeue();

	assert(queue.size() == 2);
	assert(queue.front() == 20);

	// 4. 모두 dequeue하면 다시 비어 있어야 한다.
	queue.dequeue();
	queue.dequeue();

	assert(queue.empty());
	assert(queue.size() == 0);
}

template<typename Compare>
void TestHeap(
	const std::array<int, 5>& expectedTopAfterPush,
	const std::array<int, 5>& expectedPopOrder)
{
	Heap<int, 16, Compare> heap;

	// 1. 새 힙은 비어 있어야 한다.
	assert(heap.empty());
	assert(heap.size() == 0);

	const std::array<int, 5> values = { 10, 30, 20, 50, 40 };

	// 2. 삽입할 때마다 top이 기대값과 같은지 확인한다.
	for (std::size_t i = 0; i < values.size(); ++i)
	{
		heap.push(values[i]);

		assert(!heap.empty());
		assert(heap.size() == i + 1);
		assert(heap.top() == expectedTopAfterPush[i]);
	}

	// 3. pop할 때 우선순위 순서대로 제거되는지 확인한다.
	for (std::size_t i = 0; i < expectedPopOrder.size(); ++i)
	{
		assert(heap.top() == expectedPopOrder[i]);

		heap.pop();

		assert(heap.size() == expectedPopOrder.size() - i - 1);
	}

	assert(heap.empty());
}


#include <queue>
int main()
{
	TestStack<Stack>();
	TestQueue<Queue>();
	TestQueue<CircularQueue>();
	TestStack<QueueBasedStack>();
	TestQueue<StackBasedQueue>();


	// std::less<int> : 큰 값이 top인 최대 힙
	TestHeap<std::less<int>>(
		{ 10, 30, 30, 50, 50 },   // 각 push 직후 top
		{ 50, 40, 30, 20, 10 }    // pop되는 순서
	);

	// std::greater<int> : 작은 값이 top인 최소 힙
	TestHeap<std::greater<int>>(
		{ 10, 10, 10, 10, 10 },   // 각 push 직후 top
		{ 10, 20, 30, 40, 50 }    // pop되는 순서
	);
}
