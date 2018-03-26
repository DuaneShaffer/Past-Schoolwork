// da3.h
// Duane Shaffer
// Based off the skeleton file provided by Glenn G. Chappell
// 29 Sep 2017
//
// For CS 311 Fall 2017
// Header for Assignment 3 Functions

#ifndef FILE_DA3_H_INCLUDED
#define FILE_DA3_H_INCLUDED

#include <cstddef>
using std::size_t;
#include <functional>
using std::function;
#include <stdexcept>
using std::out_of_range;
#include <algorithm>
using std::sort;


// **************************************************************** // *
// Begin DO-NOT-CHANGE section                                      // *
// Do not alter the lines below                                     // *
// **************************************************************** // *
                                                                    // *
                                                                    // *
// struct LLNode                                                    // *
// Linked List node, with client-specified value type               // *
// Intended Invariants (breakable, as data members are public):     // *
//     Either _next is nullptr, or _next points to an LLNode,       // *
//      allocated with new, owned by *this.                         // *
// Requirements on Types:                                           // *
//     ValueType must have a copy ctor and a dctor.                 // *
template <typename ValueType>                                       // *
struct LLNode {                                                     // *
    ValueType _data;  // Data for this node                         // *
    LLNode *  _next;   // Ptr to next node, or nullptr if none      // *
                                                                    // *
    // The following simplify creation & destruction                // *
                                                                    // *
    // 1- & 2-param ctor                                            // *
    // Pre:                                                         // *
    //     theNext, if passed, is either nullptr, or else it        // *
    //      points to an LLNode, allocated with new, with           // *
    //      ownership transferred to *this.                         // *
    // Post:                                                        // *
    //     _data == data.                                           // *
    //     If _next passed, then _next == next. Otherwise, _next    // *
    //      is nullptr.                                             // *
    explicit LLNode(const ValueType & data,                         // *
                    LLNode * next = nullptr)                        // *
        :_data(data),                                               // *
         _next(next)                                                // *
    {}                                                              // *
                                                                    // *
    // dctor                                                        // *
    // Pre: None.                                                   // *
    // Post: None. (_next delete'd)                                 // *
    ~LLNode()                                                       // *
    { delete _next; }                                               // *
};  // End struct LLNode                                            // *
                                                                    // *
                                                                    // *
// **************************************************************** // *
// End DO-NOT-CHANGE section                                        // *
// Do not alter the lines above                                     // *
// **************************************************************** // *


// ******************************************************************************************
// Exercise A
// Templated function lookup
// This function acts like bracket operators. Given an index, it returns the corresponding 
// element of a linked list. It throws an exception for indices that are out-of-bounds
// -----------------------------------------
// Pre:
//     head is ptr to nullptr-terminated Linked List, or nullptr.
// Post:
//     Return is a ValueType corresponding to the data held at given index of a linked list
// May throw out_of_range
// Requirements on Types:
//     ValueType must have a copy ctor and a dctor.
//     ValueType dctor must not throw.
// NOTE: The above are the requirements for LLNode<ValType>; no member								
// functions of ValueType are actually used here.															
																										
template <typename ValueType>
ValueType lookUp(const LLNode<ValueType> * head,
                 size_t index)
{
	// Check for a negative index passed to function. When true, throws out_of_range.
	if (index < 0)
		throw out_of_range("'ValueType lookUp(const LLNode<ValueType> * head, size_t index)' was passed a negative index");

	// Check to see if we were passed a nullptr. When true, throw.
	if (head == nullptr)
		throw out_of_range("'ValueType lookUp(const LLNode<ValueType> * head, size_t index)' was passed an empty list.");
	
	// If asked to return first element, do so immediately.
	if (index == 0)
		return head->_data;

	// So long as the next node is not the nullptr, we can begin to iterate through the list.
	if (head->_next == nullptr)
		throw out_of_range("'ValueType lookUp(const LLNode<ValueType> * head, size_t index)' was passed an index larger than the size of the list.");

	// Set up for iteration and iterate.
	LLNode<ValueType> * locationOfData = head->_next;
	for (size_t counter = 1; counter < index; ++counter) { 
		locationOfData = locationOfData->_next;
		// Check if we reached the end of the list
		if (locationOfData == nullptr)
			throw out_of_range("'ValueType lookUp(const LLNode<ValueType> * head, size_t index)' was passed an index larger than the size of the list.");
	}

	return locationOfData->_data;
}


// ******************************************************************************************
// Exercise B
// callBetween
// This function is given three function objects and calls them. The assumption is that function start allocates 
// a resource and finish cleans it up. Function middle may throw and as such, function callBetween ensures proper
// clean up if middle throws.
// -----------------------------------------
// Pre:
//     start allocates a resource
//	   finish releases that resource
// Post:
//     Resources are released properly.
// Throws when start or middle throw.
															

void callBetween(const function<void()> & start,
                 const function<void()> & middle,
                 const function<void()> & finish);

// ******************************************************************************************
// Exercise C
// Templated function uniqueCount
// This function takes two random access iterators to a container. It returns the number of 
// unique objects contained within.
// -----------------------------------------
// Pre:
//     RAIter first and RAIter last are valid random access iterators to a container.
// Post:
//     Return is a size_t value of unique objects.
// Exception neutral.
// Requirements on Types:
//     RAIter must have a copy ctor and a dctor.
//     RAIter dctor must not throw.															

template <typename RAIter>
size_t uniqueCount(RAIter first,
                   RAIter last)
{
	// Check for an empty container
	if (first == last)
		return 0;

	size_t counter = 1;

	// Check for container with range of size 1
	if ( (first+1) == last)
		return counter;

	// Sort in ascending order
	sort(first, last);

	for (first+=1; first < last; ++first) {
		// Since the container is in ascending order, if the previous element is < current element
		// then they are different
		if (*(first-1) < *first)
			++counter;
	}
	return counter;
}


// ******************************************************************************************
// Exercise D
// function gcd
// This function recursively computes the greatest common divisor of two given integers. 
// -----------------------------------------
// Pre:
//     None.
// Post:
//     Return is the gcd of the given integers.
// No throw guarantee.	

// Implementation in source file
int gcd(int a,
        int b);


#endif  //#ifndef FILE_DA3_H_INCLUDED

