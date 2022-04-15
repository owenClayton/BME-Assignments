#include "dynamic-stack.h"
#include <iostream>
using namespace std;

const DynamicStack::StackItem DynamicStack::EMPTY_STACK = -999;

DynamicStack::DynamicStack() // default constructor using 16 as capacity
{
    items_ = new DynamicStack::StackItem [16];
    capacity_ = 16;
    init_capacity_ = 16;
    size_ = 0;
}

DynamicStack::DynamicStack(unsigned int capacity) //parametric constructor
{
    items_ = new DynamicStack::StackItem [capacity];
    capacity_ = capacity;
    init_capacity_ = capacity;
    size_ = 0;
}

DynamicStack::~DynamicStack() //destructor since "new" is used
{
    delete [] items_;
}

bool DynamicStack::empty() const
{
    if(size_<=0) return true;
    else return false;
}

int DynamicStack::size() const
{
    return size_;
}

void DynamicStack::push(StackItem value)
{
   if (size_ == capacity_) // resize
    {
        DynamicStack::StackItem* temp = new DynamicStack::StackItem[capacity_*2];
        for (int i = 0; i<capacity_; i++) // copy items into new double capacity array
        {
            items_[i] = temp[i];
        }
        delete[] items_;
        items_ = temp;
        capacity_ = capacity_*2;
        items_[size_] = value;
        size_++;
    }
    else
    {
        items_[size_] = value;
        size_++;
    }
}

DynamicStack::StackItem DynamicStack::pop()
{
    if (empty()) return EMPTY_STACK;
    else
    {
        //pop
        int x = items_[size_-1];
        size_ --;
        //resize
        if (size_ <= (capacity_/4) && (capacity_/2)>=init_capacity_) //check if it needs to be resized and if allowed
        {
            DynamicStack::StackItem* temp = new DynamicStack::StackItem[capacity_/2]; // new temp array
            for (int i = 0; i<size_; i++) // copy items over into capacity/2 array
            {
                temp[i] = items_[i];
            }
            delete[] items_;
            items_ = temp;
            capacity_ = capacity_/2;
        }
        return x;
    }
}

DynamicStack::StackItem DynamicStack::peek() const
{
    if (empty()) return EMPTY_STACK;
    else return items_[size_-1]; //return top item
}

void DynamicStack::print() const
{
    for (int i = 0; i< size_; i++)
    {
        cout<< items_[i]<< " ";
    }
}
