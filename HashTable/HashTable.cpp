#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

template<typename T>
class HashTable
{
public:
	HashTable(size_t bucketCount)
		: m_BucketCount(bucketCount == 0 ? 1 : bucketCount)
		, m_Size(0)
		, m_Buckets(m_BucketCount)

	{
	}

	void insert(const T& value)
	{
		if (contains(value))
		{
			return;
		}

		if (shouldResizeBeforeInsert())
		{
			resize(m_BucketCount * 2);
		}

		const size_t index = hash(value);
		m_Buckets[index].push_back(value);
		m_Size++;
	}

	void erase(const T& value)
	{
		const size_t index = hash(value);
		auto& chainList = m_Buckets[index];
		auto it = std::find(chainList.begin(), chainList.end(), value);
		if (it != chainList.end())
		{
			chainList.erase(it);
			m_Size--;
		}
	}

	bool contains(const T& value) const
	{
		const size_t index = hash(value);
		const auto& chainList = m_Buckets[index];
		return std::find(chainList.begin(), chainList.end(), value) != chainList.end();
	}


	size_t size() const { return m_Size; }
	float loadFactor() const { return static_cast<float>(m_Size) / m_BucketCount; }
	size_t bucketCount() const { return m_BucketCount; }

private:
	void resize(size_t newBucketCount)
	{
		std::vector<std::list<T>> newBuckets(newBucketCount);
		for (const auto& chainList : m_Buckets)
		{
			for (const auto& e : chainList)
			{
				const size_t index = hash(e, newBucketCount);
				newBuckets[index].push_back(e);
			}
		}
		m_Buckets = newBuckets;
		m_BucketCount = newBucketCount;
	}


	size_t hash(const T& value, size_t baseBucketCount) const
	{
		return std::hash<T>{}(value) % baseBucketCount;
	}

	size_t hash(const T& value) const
	{
		return hash(value, m_BucketCount);
	}



	bool shouldResizeBeforeInsert() const
	{
		return static_cast<float>(m_Size + 1) / m_BucketCount > s_LoadFactorThreshold;
	}

private:
	size_t m_BucketCount;
	size_t m_Size;
	std::vector<std::list<T>> m_Buckets;
	inline static constexpr float s_LoadFactorThreshold = 0.75f;
};

struct Student
{
	int id;
	std::string name;
	bool operator==(const Student& other) const
	{
		return id == other.id && name == other.name;
	}
};


template<>
struct std::hash<Student>
{
	size_t operator()(const Student& student)const
	{
		const size_t idHash = std::hash<int>{}(student.id);
		const size_t nameHash = std::hash<std::string>{}(student.name);

		size_t seed = idHash;
		seed ^= nameHash + 0x9e3779b9 + (seed << 6) + (seed >> 2);

		return seed;
	}
};

int main()
{
	HashTable<Student> students(4);

	students.insert({ 1, "Kim" });
	students.insert({ 1, "Kim" });
	students.insert({ 1, "Lee" });
	students.insert({ 2, "Kim" });

	std::cout << students.size() << '\n';
	std::cout << std::boolalpha << students.contains({ 1, "Kim" }) << '\n';
	std::cout << students.contains({ 3, "Kim" }) << '\n';

	students.erase({ 1, "Lee" });

	std::cout << students.size() << '\n';
	std::cout << students.contains({ 1, "Lee" }) << '\n';
}