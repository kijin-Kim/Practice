#include <cstddef>
#include <forward_list>
#include <list>
#include <ostream>
#include <iostream>
#include "../Common.h"

template<typename T>
class SingleLinkedList
{
public:
    struct Node
    {
        Node* Next = nullptr;
        T Value;
        friend std::ostream& operator<<(std::ostream& out, const Node& other)
        {
            out << other.Value;
            return out;
        }
    };

    struct Iterator
    {
        Iterator(Node* node)
            :Current(node)
        {

        }

        Iterator& operator++()
        {
            if (Current)
            {
                Current = Current->Next;
            }
            return *this;
        }

        bool operator!=(const Iterator& other) const
        {
            return Current != other.Current;
        }

        T& operator*()
        {
            return Current->Value;
        }
        Node* Current = nullptr;
    };

    SingleLinkedList(size_t size, const T& initialValue)
    {
        m_Head = new Node();
        Node* node = m_Head;
        for (int i = 0; i < size - 1; ++i)
        {
            node->Value = initialValue;
            node->Next = new Node();
            node = node->Next;
        }
        node->Value = initialValue;
    }

    ~SingleLinkedList()
    {
        Node* node = m_Head;
        while (node)
        {
            Node* nodeToErase = node;
            node = node->Next;
            delete nodeToErase;
        }
    }

    SingleLinkedList(const SingleLinkedList& other)
    {
        m_Head = new Node();
        Node* target = m_Head;
        Node* node = other.m_Head;
        while (node)
        {
            target->Value = node->Value;
            if (node->Next)
            {
                node = node->Next;
                target->Next = new Node();
                target = target->Next;
            }
            else
            {
                break;
            }
        }
    }

    Iterator insert_after(const Iterator& iterator, const T& value)
    {
        Node* newNode = new Node();
        Node* current = iterator.Current;
        newNode->Value = value;
        newNode->Next = current->Next;
        current->Next = newNode;

        return Iterator(newNode);
    }
    Iterator push_front(const T& value)
    {
        Node* newNode = new Node();
        newNode->Value = value;
        newNode->Next = m_Head;
        m_Head = newNode;
        return Iterator(newNode);
    }
    Iterator erase_after(const Iterator& iterator)
    {
        if (!iterator.Current->Next)
        {
            return Iterator(nullptr);
        }

        Node* current = iterator.Current;

        Node* nodeToErase = current->Next;
        current->Next = current->Next->Next;
        delete nodeToErase;

        return Iterator(nullptr);
    }
    void pop_front()
    {
        Node* nodeToErase = m_Head;
        m_Head = m_Head->Next;
        delete nodeToErase;
    }

    void resize(size_t newSize)
    {
        Node* node = m_Head;
        size_t size = 1;
        for (int i = 0; i < newSize - 1; ++i)
        {
            if (!node->Next)
            {
                break;
            }
            node = node->Next;
            size++;
        }

        if (size == newSize)
        {
            Node* current = node->Next;
            node->Next = nullptr;
            while (current)
            {
                Node* nodeToErase = current;
                current = current->Next;
                delete nodeToErase;
            }
        }
        else if (size < newSize)
        {
            Node* current = node;
            for (int i = 0; i < newSize - size; ++i)
            {
                current->Next = new Node();
                current = current->Next;
            }
        }

    }


    Iterator begin() const
    {
        return Iterator{ m_Head };
    }

    Iterator end() const
    {
        return Iterator{ nullptr };
    }

    void reverse()
    {
        Node* node = m_Head;
        Node* lastNode = nullptr;
        while (node)
        {
            Node* nextNode = node->Next;
            node->Next = lastNode;
            lastNode = node;
            node = nextNode;
        }
        m_Head = lastNode;
    }



private:
    Node* m_Head = nullptr;

};

//#define SINGLE_LINKED_LIST_TYPE std::forward_list
#define SINGLE_LINKED_LIST_TYPE SingleLinkedList



#include <iostream>

int main()
{

    SINGLE_LINKED_LIST_TYPE<int> a(3, 0);
    a.insert_after(a.begin(), 1);
    std::cout << "insert_after(singleLinkedList.begin(), 1)" << std::endl;
    Print(a, "Singly-LinkedList [A]");

    a.push_front(1);
    std::cout << "push_front(1)" << std::endl;
    Print(a, "Singly-LinkedList [A]");


    SINGLE_LINKED_LIST_TYPE<int> b = a;
    std::cout << "Deep Copy" << std::endl;
    Print(b, "Singly-LinkedList [B]");


    b.erase_after(b.begin());
    std::cout << "erase_after(b.begin())" << std::endl;
    Print(b, "Singly-LinkedList [B]");

    b.pop_front();
    std::cout << "pop_front()" << std::endl;
    Print(b, "Singly-LinkedList [B]");


    
    for (int i = 0; i < 10; ++i)
    {
        b.push_front(i);
    }
    std::cout << "push_front(0 ~ 9)" << std::endl;
    Print(b, "Singly-LinkedList [B]");

  
    b.resize(3);
    std::cout << "resize(3)" << std::endl;
    Print(b, "Singly-LinkedList [B]");

    b.resize(5);
    std::cout << "resize(5)" << std::endl;
    Print(b, "Singly-LinkedList [B]");


    b.resize(3);
    std::cout << "resize(3)" << std::endl;
    Print(b, "Singly-LinkedList [B]");

    b.reverse();
    std::cout << "reverse()" << std::endl;
    Print(b, "Singly-LinkedList [B]");


    return 0;
}