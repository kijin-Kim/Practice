#pragma once

template<typename T, size_t Capacity=1000>
class Queue
{
public:
	void enqueue(const T& value)
	{
		m_Elements[m_Size++] = value;
	}
	void dequeue()
	{
		for (int i = 0; i < m_Size - 1; ++i)
		{
			m_Elements[i] = m_Elements[i + 1];
		}
		m_Size--;
	}

	const T& front() const
	{
		return m_Elements.front();
	}

	bool empty() const
	{
		return m_Size == 0;
	}
	size_t size() const
	{
		return m_Size;
	}

private:
	std::array<T, Capacity> m_Elements;
	size_t m_Size = 0;
};


template<typename T, size_t Capacity = 1000>
class CircularQueue
{
public:
	void enqueue(const T& value)
	{
		m_Elements[m_TailIndex] = value;
		m_TailIndex = (m_TailIndex + 1) % Capacity;
		m_Size++;
	}
	void dequeue()
	{
		m_HeadIndex = (m_HeadIndex + 1) % Capacity;
		m_Size--;
	}

	const T& front() const
	{
		return m_Elements[m_HeadIndex];
	}

	bool empty() const
	{
		return m_Size == 0;
	}
	size_t size() const
	{
		return m_Size;
	}

private:
	std::array<T, Capacity> m_Elements;
	size_t m_HeadIndex = 0;
	size_t m_TailIndex = 0;
	size_t m_Size = 0;
};