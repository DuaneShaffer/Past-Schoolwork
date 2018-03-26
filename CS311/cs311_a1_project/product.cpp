// Duane Shaffer
// CS 311
// Assignment 1
// September 12, 2017
// Source for class Product

#include "catch.hpp"
#include "product.h"

#include <ostream>
using std::ostream;

// *********************************************************************
// class Product - Definitions of associated global operators
// *********************************************************************


	// operator<< (ostream,Product)
	// (See header for docs.)
	ostream & operator<<(ostream & str, const Product & obj)
	{
	    str << obj.toString();
	    return str;
	}

	// operator== (Product,Product)
	// (See header for docs.)
	bool operator== (const Product & lhs, const Product & rhs) {
		return lhs.getName() == rhs.getName() && lhs.getSales() == rhs.getSales();
	}

	// operator!= (Product,Product)
	// (See header for docs.)
	bool operator!= (const Product & lhs, const Product & rhs) {
		return !(lhs == rhs);
	}