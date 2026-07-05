#pragma once
#include <array>

template<typename T, size_t Capacity = 1000>
class Stack
{
public:
	void push(const T& value)
	{
		m_Elements[m_Size++] = value;
	}
	void pop()
	{
		m_Size--;
	}
	const T& top() const
	{
		return m_Elements[m_Size - 1];
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