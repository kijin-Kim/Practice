#pragma once
#include "../Stack/Stack.h"

template<typename T, size_t Capacity = 1000>
class StackBasedQueue
{
public:
	void enqueue(const T& value)
	{
		Stack<T, Capacity> tempStack;
		while (!m_Stack.empty())
		{
			tempStack.push(m_Stack.top());
			m_Stack.pop();
		}

		m_Stack.push(value);

		while (!tempStack.empty())
		{
			m_Stack.push(tempStack.top());
			tempStack.pop();
		}
	}
	void dequeue()
	{
		m_Stack.pop();
	}

	const T& front() const
	{
		return m_Stack.top();
	}

	bool empty() const
	{
		return m_Stack.empty();
	}
	size_t size() const
	{
		return m_Stack.size();
	}

private:
	Stack<T, Capacity> m_Stack;

};