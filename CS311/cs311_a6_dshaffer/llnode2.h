// llnode2.h
// Glenn G. Chappell
// 5 Nov 2017
//
// For CS 311 Fall 2017
// Open Access Singly Linked List Node + Associated Functionality
// Used in Assignment 6, Exercises A, B
// Based on list2.cpp
// There is no associated source file.

#ifndef FILE_LLNODE2_H_INCLUDED
#define FILE_LLNODE2_H_INCLUDED

#include <iostream>
using std::ostream;
using std::cout;
using std::endl;
#include <cstddef>
using std::size_t;
#include <memory>
using std::shared_ptr;
using std::make_shared;


// struct LLNode2
// Linked List node with client-specified value type. It is expected
// that LLNode2 objects will be created with make_shared, and always
// held by a shared_ptr. An empty list is represented by an empty
// shared_ptr.
// Invariants:
//     Either _next is empty, or _next points to an empty-ptr-terminated
//      Linked List of LLNode2.
// Requirements on Types:
//     ValType must have a copy ctor and a dctor.
//     ValType dctor must not throw.
template <typename ValType>
struct LLNode2 {
    ValType             _data;  // Data for this node
    shared_ptr<LLNode2> _next;  // Ptr to next node, or empty if none

    // The following simplify creation & destruction

    // 1- & 2-param ctor
    // Pre: None.
    // Post:
    //     _data == data.
    //     If _next passed, then _next == next. Otherwise, _next is
    //      empty.
    // Strong guarantee
    // Throws what & when a ValType operation throws.
    // Exception neutral
    explicit LLNode2(const ValType & data,
                     shared_ptr<LLNode2> next = nullptr)
        :_data(data),
         _next(next)
    {}

    ~LLNode2() = default;
};


// size
// Given shared_ptr to Linked List, return its size (number of nodes).
// Pre:
//     head is ptr to empty-ptr-terminated Linked List, or is empty.
// Post:
//     Return is size of list (0 if head is empty).
// Requirements on Types:
//     ValType must have a copy ctor and a dctor.
//     ValType dctor must not throw.
// NOTE: The above are the requirements for LLNode2<ValType>; no member
// functions of ValType are actually used here.
// No-Throw Guarantee
template <typename ValType>
size_t size(shared_ptr<LLNode2<ValType> > head)
{
    auto p = head;  // Iterates through list
    size_t n = 0;   // Number of nodes so far
    while (p)
    {
        p = p->_next;
        ++n;
    }
    return n;
}


// isEmpty
// Given ptr to Linked List, returns true if the list is empty, false
// otherwise.
// Pre:
//     head points to empty-ptr-terminated Linked List, or is an empty
//      shared_ptr.
// Post:
//     Return is true if list is nonempty. Otherwise, false.
// Requirements on Types:
//     ValType must have a copy ctor and a dctor.
//     ValType dctor must not throw.
// No Throw Guarantee
template <typename ValType>
bool isEmpty(shared_ptr<LLNode2<ValType> > head)
{
    return !head;
}


// push_front
// Given ptr to Linked List, and a ValType item, push the item onto
// the front of the list. head becomes a pointer to the new list.
// Pre:
//     head points to empty-ptr-terminated Linked List, or is empty.
// Post:
//     List has been altered as required.
//     head points to the new list.
// Requirements on Types:
//     ValType must have a copy ctor and a dctor.
//     ValType dctor must not throw.
// NOTE: The above are the requirements for LLNode2<ValType>; no member
// functions of ValType are actually used here.
// Strong guarantee
// Throws what & when a ValType operation throws.
// Exception neutral
template <typename ValType>
void push_front(shared_ptr<LLNode2<ValType> > & head,
                const ValType & item)
{
    head = make_shared<LLNode2<ValType> >(item, head);
}


// print_list
// Given shared_ptr to Linked List, prints it to given output stream --
// or cout, if none is given -- a blank separating items, with a single
// trailing newline.
// Pre:
//     head points to empty-terminated Linked List, or is empty.
// Post:
//     Values have been printed, as required.
// Requirements on Types:
//     ValType must have a copy ctor and a dctor.
//     ValType dctor must not throw.
//     ValType must have operator<< (stream insertion).
// Strong Guarantee
// Throws what & when a ValType operation throws.
// Exception neutral
template <typename ValType>
void print_list(shared_ptr<LLNode2<ValType> > head,
                ostream & outstr=cout)
{
    while (head)
    {
        outstr << head->_data;
        head = head->_next;
        if (head)
            outstr << " ";
    }
    outstr << endl;
}


// pop_front
// Given shared_ptr to Linked List, removes first item from list, if
// list is nonempty. If list is empty, does nothing.
// Pre:
//     head points to empty-ptr-terminated Linked List, or is an empty
//      shared_ptr.
// Post:
//     List has been altered as required.
//     head points to the new list.
// Requirements on Types:
//     ValType must have a copy ctor and a dctor.
//     ValType dctor must not throw.
// NOTE: The above are the requirements for LLNode2<ValType>; no member
// functions of ValType are actually used here.
// No Throw Guarantee
template <typename ValType>
void pop_front(shared_ptr<LLNode2<ValType> > & head)
{
    if (head)
        head = head->_next;
}


#endif //#ifndef FILE_LLNODE2_H_INCLUDED

