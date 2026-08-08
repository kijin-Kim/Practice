#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

void SiftDown(std::vector<int>& vec, int low, int current, int endExculsive)
{
	while (true)
	{
		const int left = low + (current - low) * 2 + 1;
		if (left >= endExculsive)
		{
			return;
		}

		const int right = left + 1;
		int largerIndex = left;
		if (right < endExculsive && vec[right] > vec[largerIndex])
		{
			largerIndex = right;
		}

		if (vec[largerIndex] <= vec[current])
		{
			return;
		}

		std::swap(vec[current], vec[largerIndex]);
		current = largerIndex;
	}

}

void Heapify(std::vector<int>& vec, int low, int high)
{
	const int rangeSize = high - low + 1;
	if (rangeSize <= 1)
	{
		return;
	}

	const int lastParent = low + rangeSize / 2 - 1;
	for (int i = lastParent; i >= low; --i)
	{
		SiftDown(vec, low, i, high + 1);
	}

}

void HeapSort(std::vector<int>& vec, int low, int high)
{
	if (low >= high)
	{
		return;
	}

	Heapify(vec, low, high);

	for (int end = high; end > low; --end)
	{
		std::swap(vec[low], vec[end]);
		SiftDown(vec, low, low, end);
	}
}


void InsertionSort(std::vector<int>& vec, int low, int high)
{
	for (int i = low + 1; i <= high; ++i)
	{
		const int value = vec[i];
		int j = i - 1;
		while (j >= low && vec[j] > value)
		{
			vec[j + 1] = vec[j];
			j--;
		}
		vec[j + 1] = value;
	}
}

int Partition(std::vector<int>& vec, int low, int high)
{
	const int pivot = vec[high];
	int i = low;
	for (int j = low; j < high; ++j)
	{
		if (vec[j] < pivot)
		{
			std::swap(vec[j], vec[i++]);
		}
	}

	std::swap(vec[high], vec[i]);
	return i;
}


void IntroSortInternal(std::vector<int>& vec, int low, int high, int depthLimit)
{
	if (low >= high)
	{
		return;
	}

	if (high - low + 1 <= 16)
	{
		std::cout << "삽입 정렬 " << low << " ~ " << high << std::endl;
		InsertionSort(vec, 0, vec.size() - 1);
		return;
	}

	if (depthLimit <= 0)
	{
		std::cout << "힙 정렬 " << low << " ~ " << high << std::endl;
		HeapSort(vec, low, high);
		return;
	}



	const int pivot = Partition(vec, low, high);
	IntroSortInternal(vec, low, pivot - 1, depthLimit - 1);
	IntroSortInternal(vec, pivot + 1, high, depthLimit - 1);
}

void IntroSort(std::vector<int>& vec)
{
	int depthLimit = std::log2(vec.size());
	IntroSortInternal(vec, 0, vec.size() - 1, depthLimit);
}


int main()
{
	std::vector<int> vec(100);
	std::iota(vec.begin(), vec.end(), 1);
	std::random_device rd;
	std::mt19937 generator(rd());
	std::shuffle(vec.begin(), vec.end(), generator);

	std::cout << "정렬 전:" << std::endl;

	for (const int value : vec)
	{
		std::cout << value << ' ';
	}

	std::cout << std::endl;
	std::cout << std::endl;
	

	IntroSort(vec);

	std::cout << std::endl;
	std::cout << "정렬 후:" << std::endl;

	for (const int value : vec)
	{
		std::cout << value << ' ';
	}

	std::cout << std::endl;
	std::cout << std::endl;

	if (std::is_sorted(vec.begin(), vec.end()))
	{
		std::cout << "정렬 성공" << std::endl;
	}


	return 0;
}