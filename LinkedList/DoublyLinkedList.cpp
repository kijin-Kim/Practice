#include <cstddef>
#include <forward_list>
#include <list>
#include <ostream>
#include <iostream>
#include "../Common.h"

template<typename T>
class LinkedList
{
public:
    struct Node
    {
        Node* Prev = nullptr;
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
            : Current(node)
        {

        }

        Iterator operator++()
        {
            if (Current)
            {
                Current = Current->Next;
            }
            return *this;
        }

        Iterator operator--()
        {
            if (Current)
            {
                Current = Current->Prev;
            }
            return *this;
        }

        T& operator*()
        {
            return Current->Value;
        }

        bool operator!=(const Iterator& other)
        {
            return Current != other.Current;
        }

        Node* Current = nullptr;
    };

    LinkedList(size_t size, int initialValue)
        : m_Size(size)
    {
        m_Head = new Node();
        Node* node = m_Head;
        for (int i = 0; i < size; ++i)
        {
            node->Value = initialValue;
            if (i != size - 1)
            {
                node->Next = new Node();
                node->Next->Prev = node;
                node = node->Next;
            }
        }
        m_Tail = node;
    }

    ~LinkedList()
    {
        Node* node = m_Head;
        while (node)
        {
            Node* nodeToErase = node;
            node = node->Next;
            delete nodeToErase;
        }
    }

    LinkedList(const LinkedList& other)
        : m_Size(other.m_Size)
    {
        m_Head = new Node();
        Node* node = other.m_Head;
        Node* target = m_Head;
        while (true)
        {
            target->Value = node->Value;
            if (node->Next)
            {
                node = node->Next;
                target->Next = new Node();
                target->Next->Prev = target;
                target = target->Next;
            }
            else
            {
                break;
            }
        }
        m_Tail = target;
    }

    Iterator insert(Iterator iterator, const T& value)
    {
        Node* newNode = new Node();

        Node* current = iterator.Current;
        Node* prev = current->Prev;
        Node* next = current->Next;


        newNode->Value = value;
        newNode->Prev = prev;
        newNode->Next = current;
        if (prev)
        {
            prev->Next = newNode;
        }
        current->Prev = newNode;
        m_Size++;
        if (current == m_Head)
        {
            m_Head = newNode;
        }
        return Iterator{ newNode };
    }

    Iterator push_front(const T& value)
    {
        return insert(Iterator{ m_Head }, value);
    }

    Iterator erase(Iterator iterator)
    {
        Node* target = iterator.Current;
        Node* prev = target->Prev;
        Node* next = target->Next;

        if (prev)
        {
            prev->Next = next;
        }
        else
        {
            m_Head = next;
        }

        if (next)
        {
            next->Prev = prev;
        }
        else
        {
            m_Tail = prev;
        }

        m_Size--;
        delete target;
        return Iterator{ next };
    }

    void reverse()
    {
        Node* node = m_Head;
        while (node)
        {
            Node* nextNode = node->Next;
            Node* temp = node->Prev;
            node->Prev = node->Next;
            node->Next = temp;
            node = nextNode;
        }
        Node* temp = m_Head;
        m_Head = m_Tail;
        m_Tail = temp;

    }


    Iterator pop_back()
    {
        return erase(Iterator{ m_Tail });
    }


    Iterator begin() const
    {
        return Iterator{ m_Head };
    }

    Iterator end() const
    {
        return Iterator{ nullptr };
    }


private:
    Node* m_Head = nullptr;
    Node* m_Tail = nullptr;
    size_t m_Size = 0;
};

//#define LINKED_LIST_TYPE std::list
#define LINKED_LIST_TYPE LinkedList


int main()
{
    LINKED_LIST_TYPE<int> a(3, 0);
    a.insert(a.begin(), 3);
    a.insert(a.begin(), 2);
    a.push_front(1);
    std::cout << "insert(linkedList.begin(), 3), insert(linkedList.begin(), 2), push_front(1)" << std::endl;
    Print(a, "Doubly-LinkedList [A]");

    LINKED_LIST_TYPE<int> b = a;
    std::cout << "Deep Copy" << std::endl;
    Print(b, "Doubly-LinkedList [B]");


    b.erase(b.begin());
    std::cout << "erase(a.begin())" << std::endl;
    Print(b, "Doubly-LinkedList [B]");


    b.pop_back();
    std::cout << "pop_back()" << std::endl;
    Print(b, "Doubly-LinkedList [B]");

    b.reverse();
    std::cout << "reverse()" << std::endl;
    Print(b, "Doubly-LinkedList [B]");


    return 0;
}
