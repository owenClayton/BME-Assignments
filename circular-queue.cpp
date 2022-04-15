#include "circular-queue.h"
#include <iostream>

const CircularQueue::QueueItem CircularQueue::EMPTY_QUEUE = -999;

CircularQueue::CircularQueue()
{
    items_ = new CircularQueue::QueueItem [16];
    capacity_ = 16;
    size_ = 0;
    head_ = NULL;
    tail_ = NULL;
}

CircularQueue::CircularQueue(unsigned int capacity)
{

}

CircularQueue::~CircularQueue()
{

}

bool CircularQueue::empty() const
{
    if (head_ == tail_) return true;
    else return false;
}

bool CircularQueue::full() const
{
    if (size_ == capacity_) return true;
    else return false;
}

int CircularQueue::size() const
{

}

bool CircularQueue::enqueue(QueueItem value)
{
    if (full()) return false;
    else{

    }
}

CircularQueue::QueueItem CircularQueue::dequeue()
{

}

CircularQueue::QueueItem CircularQueue::peek() const
{

}

void CircularQueue::print() const
{

}