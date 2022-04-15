#include "doubly-linked-list.h"
#include <iostream>

using namespace std;
DoublyLinkedList::Node::Node(DataType data)
{
    value = data;
    next = nullptr;
    prev = nullptr;
}

DoublyLinkedList::DoublyLinkedList()
{
    tail_ = nullptr;
    head_ = nullptr;
    size_ = 0;
}

DoublyLinkedList::~DoublyLinkedList()
{
    Node* currentNode = head_;
    while(currentNode != nullptr) // step through all of the nodes
    {
        Node* deletedNode = currentNode;
        currentNode = currentNode->next;
        deletedNode->next = nullptr;
        deletedNode->prev = nullptr;
        deletedNode->value = 0;

        delete deletedNode;
        deletedNode = nullptr;
    }
    delete currentNode;
    currentNode = nullptr;
}

unsigned int DoublyLinkedList::size() const
{
    return size_;
}

unsigned int DoublyLinkedList::capacity() const
{
    return CAPACITY;
}

bool DoublyLinkedList::empty() const
{
    if (size_ == 0) return true;
    else return false;
}

bool DoublyLinkedList::full() const
{
    if (size_ == CAPACITY) return true;
    else return false;
}

DoublyLinkedList::DataType DoublyLinkedList::select(unsigned int index) const
{
    if(index> size_ -1 ) return tail_->value; //invalid index
    if(size_ == 0) return (DataType)-999;
    if(index ==0) return head_->value;
    if(index== size_-1) return tail_->value;
    else
    {
        Node* currentNode;
        currentNode = head_;
        for (int i = 0; i < index; i++) // steps up to index since starting at head.
        {
            currentNode = currentNode->next;
        }
        return currentNode->value;
    }
}

unsigned int DoublyLinkedList::search(DataType value) const
{
    if (size_==0) return 0; //nothing to search
    else
    {
        Node* currentNode;
        currentNode = head_;
        for(int i = 0; i< size_ ; i++) //steps up to one less index than size
        {
            if (currentNode->value == value) return i;
            else currentNode = currentNode->next;
        }
        //value not found
        return size_;
    }
}

void DoublyLinkedList::print() const
{
    Node* currentNode = head_;
    for (int i = 0; i< size_; i ++)
    {
        cout<< currentNode->value << " ";
        currentNode = currentNode->next;
    }
}

DoublyLinkedList::Node* DoublyLinkedList::getNode(unsigned int index) const
{
    Node* currentNode = head_;
    for(int i = 0; i<index; i++)
    {
        currentNode = currentNode->next;
    }
    return currentNode;
}

bool DoublyLinkedList::insert(DataType value, unsigned int index)
{
    if (size_== CAPACITY || index > size_) return false; //unable to add (full or index too large)
    if (index == 0 || size_ == 0)
    {
        insert_front(value);
        return true;
    }
    if (index == size_)
    {
        insert_back(value);
        return true;
    }
    else
    {
       Node* curr = head_;
       Node* prev = nullptr;
       for (int i = 0; i < index ; i ++) //find insert location
       {
           prev = curr;
           curr = curr->next;
       }
       Node* newNode = new Node (value);
       prev->next = newNode;
       curr->prev = newNode;
       newNode->next = curr;
       newNode->prev = prev;
       size_++;
       curr = nullptr;
       prev = nullptr;
       return true;
    }
}

bool DoublyLinkedList::insert_front(DataType value)
{
    if (size_ == CAPACITY)
    {
        return false;
    }
    if(size_ == 0) //special case, change head and tail
    {
       Node* newNode = new Node( value);
       head_ = newNode;
       tail_ = newNode;
       newNode->next = nullptr;
       newNode->prev = nullptr;
       size_++;
       return true;
    }
    else
    {
        Node* newNode = new Node( value);
        newNode->next = head_; //shift all nodes right
        head_->prev = newNode;
        head_ = newNode;
        newNode->prev = nullptr;
        size_++;
        return true;
    }
}

bool DoublyLinkedList::insert_back(DataType value)
{
    if(size_ == 0)
    {
        insert_front(value); //since already coded
        return true;
    }
    else
    {
        Node* newNode = new Node(value);
        newNode->prev = tail_;
        tail_->next = newNode;
        tail_ = newNode;
        newNode->next = nullptr;
        size_++;
        return true;
    }
}

bool DoublyLinkedList::remove(unsigned int index)
{
    if (size_ == 0 || index > size_-1) return false; //invalid index
    if (index == size_ -1 || size_ == 1)
    {
        remove_back();
        return true;
    }
    if (index == 0)
    {
        remove_front();
        return true;
    }
    else
    {
        Node* remove = head_;
        Node* prev;
        Node* next = head_->next;
        for (int i = 0; i < index; i++) //find node to be removed, move all pointers
        {
            next = next->next;
            prev = remove;
            remove = remove->next;
        }
        next->prev = prev;
        prev->next = next;
        remove->prev = nullptr;
        remove->next = nullptr;
        size_ --;
        delete remove;
        remove = nullptr;
        return true;
    }
}

bool DoublyLinkedList::remove_front()
{
    if (size_ == 0) return false;
    if (size_ == 1)
    {
        head_ = NULL;
        tail_ = NULL;
        size_--;
        return true;
    }
    else
    {
        Node* remove;
        remove = head_;
        head_ = head_->next;
        head_->prev = nullptr;
        remove->next = nullptr;
        delete remove;
        remove = nullptr;
        size_--;
        return true;
    }
}

bool DoublyLinkedList::remove_back()
{
    if (size_ == 0) return false;
    if (size_ == 1)
    {
        remove_front();
        return true;
    }
    else
    {
        Node* remove = tail_;
        tail_ = tail_->prev;
        tail_->next = nullptr;
        remove->prev = nullptr;
        delete remove;
        remove = nullptr;
        size_--;
        return true;
    }
}

bool DoublyLinkedList::replace(unsigned int index, DataType value)
{
    if (index > size_ || size_ == 0 || index == size_) return false; //invalid index
    if (size_ == 1)
    {
        Node* temp = head_;
        Node* newNode = new Node(value);
        head_ = newNode;
        tail_ = newNode;
        delete temp;
        temp = nullptr;
        return true;
    }
    if (index == 0) //head needs to be adjusted
    {
        Node* next = head_->next;
        Node* newNode = new Node (value);
        Node* remove = head_;
        newNode->next = head_->next;
        next->prev = newNode;
        head_ = newNode;
        remove->next = NULL;
        head_->prev = NULL;
        delete remove;
        remove = nullptr;
        return true;
    }
    if (index == size_ -1) //tail needs to be adjusted
    {
        Node* before = tail_->prev;
        Node* newNode = new Node (value) ;
        Node* remove = tail_;
        newNode->prev = tail_->prev;
        before->next = newNode;
        tail_ = newNode;
        remove->prev = NULL;
        tail_->next = NULL;
        delete remove;
        remove = nullptr;
        return true;
    }
    else
    {
        Node* newNode = new Node (value);
        Node* replace = head_;
        Node* prev;
        Node* next = head_->next;
        for (int i = 0; i< index; i++) //step through and find node to be replaced.
        {
            prev = replace;
            replace = next;
            next = next->next;
        }
        prev->next = newNode;
        newNode->prev = prev;
        newNode->next = next;
        next->prev = newNode;
        replace->next = NULL;
        replace->prev = NULL;
        delete replace;
        replace = nullptr;
        return true;
    }
}

