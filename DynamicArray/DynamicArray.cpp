#include <iostream>
#include <vector>
#include <algorithm>
#include <ostream>
#include "../Common.h"

template<class T>
class DynamicArray final
{
public:
	DynamicArray(size_t size)
		: m_Size(size)
		, m_Capacity(std::ceil(m_Size * 1.5f))
	{
		m_Ptr = new T[m_Capacity]();
	}

	~DynamicArray()
	{
		if (m_Ptr)
		{
			delete[] m_Ptr;
		}
	}

	DynamicArray(const DynamicArray& other)
		: m_Size(other.m_Size)
		, m_Capacity(other.m_Capacity)
	{
		m_Ptr = new T[m_Capacity]();
		std::copy(other.m_Ptr, other.m_Ptr + other.m_Size, m_Ptr);
	}

	DynamicArray& operator=(const DynamicArray& other)
	{
		if (this == &other)
		{
			return *this;
		}

		if (m_Ptr)
		{
			delete[] m_Ptr;
		}

		m_Size = other.m_Size;
		m_Capacity = other.m_Capacity;


		m_Ptr = new T[m_Capacity]();
		std::copy(other.m_Ptr, other.m_Ptr + other.m_Size, m_Ptr);
		return *this;
	}

	void push_back(T element)
	{
		resize(m_Size + 1);
		m_Ptr[m_Size - 1] = element;
	}

	void insert(T* ptr, T element)
	{
		size_t index = ptr - m_Ptr;
		resize(m_Size + 1);
		ptr = m_Ptr + index;
		std::copy_backward(ptr, m_Ptr + m_Size - 1, m_Ptr + m_Size);
		m_Ptr[index] = element;
	}

	void erase(T* ptr)
	{
		std::copy(ptr + 1, m_Ptr + m_Size, ptr);
		m_Size--;
	}

	void reserve(size_t i)
	{
		if (m_Capacity >= i)
		{
			return;
		}
		m_Capacity = i;
		T* ptr = new T[m_Capacity];
		std::copy(m_Ptr, m_Ptr + m_Size, ptr);
		delete[] m_Ptr;
		m_Ptr = ptr;
	}

	void resize(size_t i)
	{
		if (i >= m_Capacity)
		{
			reserve(std::ceil(i * 1.5f));
		}

		m_Size = i;
	}

	size_t size() const
	{
		return m_Size;
	}

	size_t capacity() const
	{
		return m_Capacity;
	}

	const T* begin() const
	{
		return m_Ptr;
	}

	T* begin()
	{
		return m_Ptr;
	}

	const T* end() const
	{
		return m_Ptr + m_Size;
	}

	T* end()
	{
		return m_Ptr + m_Size;
	}

	const T& operator[](size_t i) const
	{
		return m_Ptr[i];
	}

	T& operator[](size_t i)
	{
		return m_Ptr[i];
	}


private:
	T* m_Ptr = nullptr;
	size_t m_Size = 0;
	size_t m_Capacity = 0;
};


template<>
class DynamicArray<bool>
{
public:
	class BoolReference
	{
		friend class DynamicArray<bool>;

	public:
		BoolReference() = default;
		BoolReference(uint8_t* base, uint8_t offset)
			: m_Base(base)
			, m_Offset(offset)
		{
		}

		BoolReference operator+(size_t offset)
		{
			m_Base += offset / 8;
			m_Offset += offset % 8;
			return *this;
		}

		BoolReference operator=(bool element)
		{
			uint8_t mask = 1 << (7 - m_Offset);
			if (element)
			{
				m_Base[0] |= mask;
			}
			else
			{
				m_Base[0] &= ~mask;
			}

			return *this;
		}

		bool getBool() const
		{
			uint8_t mask = 1 << (7 - m_Offset);
			return (m_Base[0] & mask) == mask;
		}

		friend std::ostream& operator<<(std::ostream& out, const BoolReference& boolReference)
		{
			out << boolReference.getBool();
			return out;
		}

	private:
		uint8_t* m_Base = nullptr;
		uint8_t m_Offset = 0;
	};

public:
	DynamicArray(size_t size)
		: m_Size(size)
		, m_Capacity(std::ceil(m_Size * 1.5f))
	{
		m_Ptr = new uint8_t[GetPerByte(m_Capacity)]();
	}

	~DynamicArray()
	{
		if (m_Ptr)
		{
			delete[] m_Ptr;
		}
	}

	DynamicArray(const DynamicArray& other)
		: m_Size(other.m_Size)
		, m_Capacity(other.m_Capacity)
	{
		m_Ptr = new uint8_t[GetPerByte(m_Capacity)]();
		std::copy(other.m_Ptr, other.m_Ptr + GetPerByte(m_Size), m_Ptr);
	}

	DynamicArray& operator=(const DynamicArray& other)
	{
		if (this == &other)
		{
			return *this;
		}

		if (m_Ptr)
		{
			delete[] m_Ptr;
		}


		m_Size = other.m_Size;
		m_Capacity = other.m_Capacity;

		m_Ptr = new uint8_t[GetPerByte(other.m_Capacity)]();
		std::copy(other.m_Ptr, other.m_Ptr + GetPerByte(other.m_Size), m_Ptr);


		return *this;
	}

	void push_back(bool element)
	{
		resize(m_Size + 1);
		operator[](m_Size - 1) = element;
	}

	void insert(BoolReference ref, bool element)
	{
		resize(m_Size + 1);
		size_t blockOffset = ref.m_Base - m_Ptr;
		for (int i = m_Size - 1; i > blockOffset * 8 + ref.m_Offset; --i)
		{
			operator[](i) = operator[](i - 1).getBool();
		}

		ref = element;
	}


	void erase(BoolReference ref)
	{
		size_t blockOffset = ref.m_Base - m_Ptr;
		for (int i = blockOffset * 8 + ref.m_Offset; i < m_Size; ++i)
		{
			operator[](i) = operator[](i + 1).getBool();
		}
		m_Size--;
	}

	void reserve(size_t i)
	{
		if (m_Capacity >= i)
		{
			return;
		}

		m_Capacity = i;
		uint8_t* ptr = new uint8_t[GetPerByte(m_Capacity)]();
		std::copy(m_Ptr, m_Ptr + GetPerByte(m_Size), ptr);
		delete[] m_Ptr;
		m_Ptr = ptr;
	}

	void resize(size_t i)
	{
		if (i >= m_Capacity)
		{
			reserve(std::ceil(i * 1.5f));
		}

		m_Size = i;
	}

	size_t size() const
	{
		return m_Size;
	}

	size_t capacity() const
	{
		return m_Capacity;
	}

	const BoolReference begin() const
	{
		return BoolReference{ m_Ptr, 0 };
	}

	BoolReference begin()
	{
		return BoolReference{ m_Ptr, 0 };
	}

	const BoolReference end() const
	{
		size_t base = m_Size / 8;
		uint8_t offset = m_Size % 8;
		return BoolReference{ m_Ptr + base, offset };
	}

	BoolReference end()
	{
		size_t base = m_Size / 8;
		uint8_t offset = m_Size % 8;
		return BoolReference{ m_Ptr + base, offset };
	}

	const BoolReference operator[](size_t i) const
	{
		size_t base = i / 8;
		uint8_t offset = i % 8;
		return BoolReference{ m_Ptr + base, offset };
	}

	BoolReference operator[](size_t i)
	{
		size_t base = i / 8;
		uint8_t offset = i % 8;
		return BoolReference{ m_Ptr + base, offset };
	}




private:
	size_t GetPerByte(size_t i) const
	{
		return (i + 7) / 8;
	}


private:
	uint8_t* m_Ptr = nullptr;
	size_t m_Size = 0;
	size_t m_Capacity = 0;
};



//#define DYNAMIC_ARRAY_TYPE std::vector
#define DYNAMIC_ARRAY_TYPE DynamicArray


void TestInt()
{
	DYNAMIC_ARRAY_TYPE<int> a(2);
	a[0] = 1;
	a[1] = 2;
	std::cout << "Initialize elements " << std::endl;
	Print(a, "Array [A]");

	DYNAMIC_ARRAY_TYPE<int> b(0);
	b = a;
	std::cout << "Deep Copy" << std::endl;
	Print(b, "Array [B]");



	a.push_back(4);
	a.push_back(5);
	std::cout << "push_back(4), push_back(5) " << std::endl;
	Print(a, "Array [A]");
	Print(b, "Array [b]");


	std::cout << "insert(a.begin() + 2, 3) " << std::endl;
	a.insert(a.begin() + 2, 3);
	Print(a, "Array [A]");


	std::cout << "erase(a.begin() + 2) " << std::endl;
	a.erase(a.begin() + 2);
	Print(a, "Array [A]");


	a.resize(2);
	std::cout << "a.resize(2)" << std::endl;
	Print(a, "Array [A]");

}

void TestBool()
{
	DYNAMIC_ARRAY_TYPE<bool> a(8);
	a[0] = true;
	a[1] = false;
	a[2] = true;
	a[3] = true;
	a[4] = false;
	a[5] = true;
	a[6] = true;
	a[7] = true;
	std::cout << "Initialize elements " << std::endl;
	Print(a, "Array [A]");


	std::cout << "Deep Copy" << std::endl;
	DYNAMIC_ARRAY_TYPE<bool> b = a;
	Print(b, "Array [b]");


	a.push_back(false);
	a.push_back(true);
	a.push_back(true);
	a.push_back(true);

	std::cout << "push_back(false), push_back(true), push_back(true), push_back(true) " << std::endl;
	Print(a, "Array [A]");
	Print(b, "Array [b]");


	std::cout << "insert(a.begin() + 2, true) " << std::endl;
	a.insert(a.begin() + 2, false);
	Print(a, "Array [A]");


	std::cout << "erase(a.begin() + 2) " << std::endl;
	a.erase(a.begin() + 2);
	Print(a, "Array [A]");

	a.resize(15);
	std::cout << "a.resize(15)" << std::endl;
	Print(a, "Array [A]");

	a.resize(2);
	std::cout << "a.resize(2)" << std::endl;
	Print(a, "Array [A]");
}


int main()
{
	TestInt();
	std::cout << "--------------------" << std::endl;
	TestBool();
}