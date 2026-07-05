template <typename T, size_t Capacity = 1000, typename Compare = std::less<T>>
class Heap
{
public:
	Heap() = default;

	Heap(const Compare& compare)
		:m_Compare(compare)
	{
	}

	void push(const T& value)
	{
		m_Elements[m_Size] = value;
		siftUp(m_Size++);
	}
	void pop()
	{
		m_Elements[0] = m_Elements[m_Size - 1];
		m_Size--;
		siftDown(0);
	}

	const T& top() const
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
	void siftUp(size_t index)
	{
		if (index == 0)
		{
			return;
		}

		const size_t parentIndex = (index - 1) / 2;
		if (!m_Compare(m_Elements[parentIndex], m_Elements[index]))
		{
			return;
		}

		std::swap(m_Elements[parentIndex], m_Elements[index]);
		siftUp(parentIndex);
	}


	void siftDown(size_t index)
	{
		const size_t leftChildIndex = index * 2 + 1;
		if (leftChildIndex >= m_Size)
		{
			return;
		}

		const size_t rightChildIndex = index * 2 + 2;

		size_t largerElementIndex = leftChildIndex;
		if (rightChildIndex < m_Size
			&& m_Compare(m_Elements[largerElementIndex], m_Elements[rightChildIndex]))
		{
			largerElementIndex = rightChildIndex;
		}

		if (m_Compare(m_Elements[index], m_Elements[largerElementIndex]))
		{
			std::swap(m_Elements[index], m_Elements[largerElementIndex]);
			siftDown(largerElementIndex);
		}
	}

private:
	std::array<T, Capacity> m_Elements;
	size_t m_Size = 0;
	Compare m_Compare;
};