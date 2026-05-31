//오늘의 숙제
//- 트리
//- 기본적인 이진 탐색 트리
//- 찾기, 삽입, 삭제
//- 전위, 중위, 후위
//- 위 순회 특징을 이용해서 트리 복사, 트리 자체 삭제(소멸자 or clear)까지 구현


// 찾기 방법: 루트값과 비교하면서 왼쪽 또는 오른쪽으로 순회함.
// 삽입 방법: 루트값과 비교하면서 왼쪽 또는 오른쪽으로 순회함. 더 이상 갈 수 없을 경우 (말단 노드인 경우) 해당 노드의 왼쪽 또는 오른쪽에 삽입
// 삭제 방법: 루트값과 비교하면서 왼쪽 또는 오른쪽으로 순회함. 해당 노드를 찾아서 삭제할 경우 다음 세가지를 통해 트리 구조를 유지함
//	1) 자식 노드가 없는 경우 -> 해당 노드 삭제 후, 해당 노드를 가르키고 있는 부모 노드의 포인터를 nullptr로 업데이트.
//	2) 자식 노드가 하나인 경우 -> 해당 노드 삭제 후, 해당 노드를 가르키고 있는 부모 노드의 포인터를 해당 자식노드로 함.
//	3) 자식 노드가 두 개인 경우 -> 왼쪽 서브트리의 Right-most Ancestor의 값으로 대체하고 Right-most Ancestor를 삭제함.

// copy construct & copy assign operator: 전위 우선탐색
// print: 중위 우선 탐색
// destructor: 후위 우선탐색


#include <iostream>
struct TreeNode
{
public:
	TreeNode() = default;
	TreeNode(int value)
		: Value(value)
	{
	}

	int Value = 0;
	TreeNode* Left = nullptr;
	TreeNode* Right = nullptr;
};


class BinarySearchTree
{
public:
	BinarySearchTree()
	{
	}

	~BinarySearchTree()
	{
		DestructRecursive(m_RootNode);
	}

	BinarySearchTree(const BinarySearchTree& other)
	{
		*this = other;
	}

	BinarySearchTree& operator=(const BinarySearchTree& other)
	{
		DestructRecursive(m_RootNode);
		m_RootNode = CopyRecursive(m_RootNode, other.m_RootNode);
		return *this;
	}

	void Insert(int value)
	{
		m_RootNode = InsertRecursive(m_RootNode, value);
	}

	TreeNode* Search(int value)
	{
		return SearchRecursive(m_RootNode, value);
	}

	void Erase(int value)
	{
		m_RootNode = EraseRecursive(m_RootNode, value);
	}

	void Print()
	{
		PrintRecursive(m_RootNode);
		std::cout << std::endl;
	}

private:
	TreeNode* InsertRecursive(TreeNode* current, int value)
	{
		if (!current)
		{
			current = new TreeNode(value);
			return current;
		}

		if (current->Value > value)
		{
			current->Left = InsertRecursive(current->Left, value);
		}
		else
		{
			current->Right = InsertRecursive(current->Right, value);
		}
		return current;
	}


	TreeNode* SearchRecursive(TreeNode* current, int value)
	{
		if (!current)
		{
			return nullptr;
		}

		if (current->Value == value)
		{
			return current;
		}

		if (current->Value > value)
		{
			return SearchRecursive(current->Left, value);
		}

		return SearchRecursive(current->Right, value);
		
	}

	TreeNode* EraseRecursive(TreeNode* current, int value)
	{
		if (current->Value != value)
		{
			if (current->Value > value)
			{
				current->Left = EraseRecursive(current->Left, value);
			}
			else
			{
				current->Right = EraseRecursive(current->Right, value);
			}
			return current;
		}

		if (!current->Left && !current->Right)
		{
			delete current;
			return nullptr;
		}

		if (current->Left && current->Right)
		{
			TreeNode* rightMost = current->Left;
			TreeNode* last = nullptr;
			while (rightMost->Right)
			{
				last = rightMost;
				rightMost = rightMost->Right;
			}

			current->Value = rightMost->Value;
			delete rightMost;
			if (last)
			{
				last->Right = nullptr;
			}

			return current;
		}

		if (current->Left)
		{
			TreeNode* temp = current->Left;
			delete current;
			return temp;
		}

		if (current->Right)
		{
			TreeNode* temp = current->Right;
			delete current;
			return temp;
		}


	}

	void DestructRecursive(TreeNode* current)
	{
		if (!current)
		{
			return;
		}

		DestructRecursive(current->Left);
		DestructRecursive(current->Right);
		delete current;
	}

	TreeNode* CopyRecursive(TreeNode* copyDest, TreeNode* copySource)
	{
		if (copySource)
		{
			copyDest = new TreeNode(copySource->Value);
			copyDest->Left = CopyRecursive(copyDest->Left, copySource->Left);
			copyDest->Right = CopyRecursive(copyDest->Right, copySource->Right);
		}

		return copyDest;
	}

	void PrintRecursive(TreeNode* current)
	{
		if (!current)
		{
			return;
		}

		PrintRecursive(current->Left);
		std::cout << current->Value << " ";
		PrintRecursive(current->Right);
	}

private:
	TreeNode* m_RootNode = nullptr;
};

int main()
{
	BinarySearchTree bst;
	bst.Insert(4);
	bst.Insert(2);
	bst.Insert(8);
	bst.Insert(1);
	bst.Insert(3);
	bst.Insert(6);
	bst.Insert(9);
	bst.Insert(5);
	bst.Insert(7);
	bst.Insert(10);

	std::cout << "-------------- Insert [1 ~ 10] --------------" << std::endl;
	bst.Print();
	std::cout << std::endl;
	
	BinarySearchTree bst1 = bst;
	BinarySearchTree bst2;
	bst2 = bst1;

	std::cout << "---- copy construct & copy assign (deep) ----" << std::endl;
	bst2.Print();
	std::cout << std::endl;
	
	
	TreeNode* serachedNode = bst2.Search(10);
	std::cout << "----------------- Search 10 -----------------" << std::endl;
	std::cout << serachedNode->Value << std::endl;
	std::cout << std::endl;
	

	std::cout << "---------- Erase [1, 3] (말단 노드) ---------" << std::endl;
	bst2.Erase(1);
	bst2.Erase(3);
	bst2.Print();
	std::cout << std::endl;

	std::cout << "---------- Erase 9 (자식 노드가 1개) --------" << std::endl;
	bst2.Erase(10);
	bst2.Print();
	std::cout << std::endl;


	std::cout << "-------- Erase 8 (자식 노드가 2개) ----------" << std::endl;
	bst2.Erase(8);
	bst2.Print();
	std::cout << std::endl;
}
