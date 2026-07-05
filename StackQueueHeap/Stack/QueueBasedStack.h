#pragma once
#include "../Queue/Queue.h"

template<typename T, size_t Capacity = 1000>
class QueueBasedStack
{
public:
	void push(const T& value)
	{
		m_Queue.enqueue(value);
		for (int i = 0; i < m_Queue.size() - 1; ++i) 
		{
			m_Queue.enqueue(m_Queue.front());
			m_Queue.dequeue();
		}
	}

	void pop()
	{
		m_Queue.dequeue();
	}
	const T& top() const
	{
		return m_Queue.front();
	}

	bool empty() const
	{
		return m_Queue.empty();
	}

	size_t size() const
	{
		return m_Queue.size();
	}

private:
	Queue<T, Capacity> m_Queue;
};