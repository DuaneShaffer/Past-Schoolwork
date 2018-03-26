// Duane Shaffer
// CS311
// Assignment 2
// ksarray.h
// This header file for class ksarray (kinda smart array)
// There is no associated source file
// This code uses modified versions of example code presented in cs311 by Dr. Chappell
// Requires C++14 or later

#ifndef FILE_KSARRAY_H_INCLUDED
#define FILE_KSARRAY_H_INCLUDED

#include <vector> // for std::swap
#include <cstddef> // for std::size_t
#include <algorithm> // for std::copy

// ***************************************************************
// class KSArray - Class definition
// ***************************************************************

// class KSArray
// Simple RAII class holding a dynamic array of templated type.
// A const KSArray does not allow modification of its array items.
// Invariants:
//		_dataPtr points to an array of DataType, allocated with new[], owned by *this.
// Requirements on Types:
//     DataType must have a copy ctor, dctor, and comparison operators.						 
//     DataType dctor must not throw.

template <typename DataType> // Type of each item in this array

class KSArray {

// ********** KSArray: Types ***********
public:

	using size_type = std::size_t;  // Unsigned type for size of array
	using value_type = DataType;

// ********** KSArray: General public operators ******
public:

	// Default ctor
	// Set _size = 10 and default construct the array
	// Pre: None
	// Post: _dataPtr is a dynamically allocated array of size 10
	// Throws what & when a DataType operation throws.
	KSArray() : _size(10), _dataPtr(new value_type[10])
	{}

	// One-Parameter ctor
	// Not an implicit type conversion
	// set _size = to given non-negative integer and default construct the array
	// Pre: given size must be a non-negative integer
	// Post: _dataPtr is a dynamically allocated array of given size
	explicit KSArray(size_type size) : _size(size), _dataPtr(new value_type[size]) 
	{}

	// Copy ctor
	// Create a deep copy of given KSArray								
	// Pre: None
	// Post: a copy of KSArray has been created
	KSArray(const KSArray & other) : _size(other._size), _dataPtr(new value_type[other._size]) {
//		std::copy(other.begin(), other.end(), _dataPtr);
		for (size_type index = 0; index < _size; ++index) {
			_dataPtr[index] = other[index];
		}
	}

	// move ctor
	// Pre: other must be destructible
	// Post: values from other have been moved to current KSArray
	KSArray (KSArray && other) noexcept : _size(other._size), _dataPtr(other._dataPtr) {
		other._size = 0;
		other._dataPtr = nullptr;
	}

	// Dctor
	// deletes the memory that was allocated
	// Pre: None.
	// Post: Array referenced by _dataPtr has been destroyed

	~KSArray() {
		delete[] _dataPtr;
	}

	// Copy op=
	// Pre: None
	// Post: KSArray has been assigned
	KSArray & operator= (const KSArray &rhs) {
		if (this == &rhs)
			return *this;
		KSArray copy_of_rhs(rhs);
		ksaSwap(copy_of_rhs);
		return *this;
	}

	// move op= 
	// Pre: None
	// Post: Values from given KSArray have been moved to current KSArray
	KSArray & operator= (KSArray && rhs) noexcept {
		if (this == &rhs) // Check for self-assignment
			return *this;
		ksaSwap(rhs);
		return *this;
	}

	// op [] - const & non-const
	// Pre: 0 <= index < (size of allocated array)
	// Post: Return is reference to item [index] of allocated array.

	value_type & operator[](size_type index) {
		return _dataPtr[index];
	}
	const value_type & operator[](size_type index) const{
		return  _dataPtr[index];
	}

	// member function size
	// Pre: None
	// Post: returns the number of items in the array
	size_type size() const {
		return _size;
	}

	// member function begin 
	// const and non-const versions
	// Pre: None
	// Post: returns the address of item 0 in the array
	value_type * begin() {
		return _dataPtr;
	}

	const value_type * begin() const {
		return _dataPtr;
	}

	// member function end
	// const and non-const versions
	// Pre: None
	// Post: returns the address of 1 past the last item in the array
	value_type * end() { 
		return _dataPtr + _size;
	}

	const value_type * end() const {
		return _dataPtr + _size;
	}

// ********** KSArray: Data members ***********	
private:

	size_type _size; // Size of the array
	value_type * _dataPtr; // Pointer to the data of the array

	// private member function ksaSwap
	// Swap function for use in asssignment operators.
	// Pre: None.
	// Post: Member variables of two KSArray objects have been swapped.
	void ksaSwap(KSArray & other) noexcept {
		std::swap(_size, other._size);
		std::swap(_dataPtr, other._dataPtr);
	}



};  // End of class declaration KSArray

// ************************************************************
// Class KSArray - Definition of associated global operators
// ************************************************************

	// op==
	// Determines if two KSArrays are logically equal. (same size and data)
	// Pre: The given KSArrays must have the same value_type
	// Post: returns a boolean describing the equality of two KSArrays.
	// Requirements on Types:
	//     DataType must have a copy ctor, dctor, and comparison operators.						 
	//     DataType dctor must not throw.
template <typename DataType>
bool operator== (const KSArray<DataType> &lhs, const KSArray<DataType> &rhs) {
	if (lhs.size() != rhs.size())
		return false;
	for (std::size_t index = 0; index < lhs.size(); ++index) {
		if (lhs[index] != rhs[index])
			return false;
	}
	return true;														
}

// op!=
// Determines if two KSArrays are logically not equal
// Pre: The given KSArrays must have the same value_type
// Post: returns a boolean describing the inequality of two KSArrays
// Requirements on Types:
//     DataType must have a copy ctor, dctor, and comparison operators.						 
//     DataType dctor must not throw.
template <typename DataType>
bool operator!= (const KSArray<DataType> &lhs, const KSArray<DataType> &rhs) {
	return !(lhs == rhs);
}

// operator <
// Determines if one KSArray is less than another
// Pre: None
// Post: returns a boolean describing the relation of two KSArrays
// Requirements on Types:
//     DataType must have a copy ctor, dctor, and comparison operators.						 
//     DataType dctor must not throw.
template <typename DataType>
bool operator< (const KSArray<DataType> &lhs, const KSArray<DataType> &rhs) {
	if (lhs == rhs) // Copy check
		return false;
		for (std::size_t index = 0; index < lhs.size(); ++index) {
			if (index == rhs.size())		// If we made it to the end of rhs stop and dont go outside its bounds
				return false;
			if (lhs[index] > rhs[index])
				return false;
			if (lhs[index] < rhs[index])
				return true;
		}
		return true;
}

// operator >
// Determines if one KSArray is less than another
// Pre: None
// Post: returns a boolean describing the relation of two KSArrays
// Requirements on Types:
//     DataType must have a copy ctor, dctor, and comparison operators.						 
//     DataType dctor must not throw.
template <typename DataType>
bool operator> (const KSArray<DataType> & lhs, const KSArray<DataType>& rhs) { 
	return rhs < lhs; 
	}

// operator <=
// Determines if one KSArray is less than another
// Pre: None
// Post: returns a boolean describing the relation of two KSArrays
// Requirements on Types:
//     DataType must have a copy ctor, dctor, and comparison operators.						 
//     DataType dctor must not throw.
template <typename DataType>
bool operator<=(const KSArray<DataType>& lhs, const KSArray<DataType>& rhs) { 
	return !(lhs > rhs); 
	}

// operator >=
// Determines if one KSArray is less than another
// Pre: None
// Post: returns a boolean describing the relation of two KSArrays
// Requirements on Types:
//     DataType must have a copy ctor, dctor, and comparison operators.						 
//     DataType dctor must not throw.
template <typename DataType>
bool operator>=(const KSArray<DataType>& lhs, const KSArray<DataType>& rhs) { 
	return !(lhs < rhs); 
	}


#endif // KS_ARRAY_H_INCLUDED

