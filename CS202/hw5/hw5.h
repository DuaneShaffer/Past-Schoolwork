#ifndef HW5_H
#define HW5_H
//Duane Shaffer
//CS202 Spring
//Header file for homework 5 

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

template <typename T>
 std::ostringstream & printVector(std::ostringstream & inputOstream, const std::vector<T> & inputVector) {
	for (unsigned int index = 0; index < inputVector.size(); ++index)
		inputOstream << inputVector.at(index) << "\n";
	return inputOstream;
}

template <typename Objtype>
class RAIIPtr {
public:
	~RAIIPtr();
	RAIIPtr();
	RAIIPtr (const Objtype &);
	RAIIPtr(Objtype *);
	Objtype & operator*();
	Objtype * operator->();
private:
	Objtype * _ptr;
};

template <typename Objtype>
RAIIPtr<Objtype>::~RAIIPtr() {
	delete _ptr;
}

template <typename Objtype>
RAIIPtr<Objtype>::RAIIPtr() :_ptr(new Objtype) {
}

template <typename Objtype>
RAIIPtr<Objtype>::RAIIPtr(const Objtype &o) {
	_ptr = Objtype(o);
}
template <typename Objtype>
RAIIPtr<Objtype>::RAIIPtr(Objtype *o) {
	_ptr = o;
}
template <typename Objtype>
Objtype & RAIIPtr<Objtype>::operator*() {
	return * _ptr;
}

template <typename Objtype>
Objtype * RAIIPtr<Objtype>::operator->() {
	return _ptr;
}
#endif