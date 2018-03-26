// Duane Shaffer
// CS 311
// Assignment 1
// September 12, 2017
// This is the header file that defines the "Product" class


#ifndef FILE_PRODUCT_H_INCLUDED
#define FILE_PRODUCT_H_INCLUDED
#include "catch.hpp"
#include <ostream> // For std::ostream
#include <string> // For std::string
#include <sstream> // For string op<<


// *********************************************************************
// class Product - Class definition
// *********************************************************************


// class Product
// Simple class holding a string for product name and an associated integer
// that represents sales.
// Invariants:
//     _numberSold is a positive integer

class Product {

public:
    // Default ctor
    // Set _productName to an empty string and _numberSold to 0
    // Pre: None.
    // Post: _productName is an empty string and _numberSold is zero.
    Product(): _productName(""),_numberSold(0)
        {}

	// Ctor with data.
	// Pre: Sales must be positive.
	// Post: _productName is the name passed and _numberSold is the given sales.
    Product (std::string name, int sales)
        : _productName(name), _numberSold(sales)
        {}

    // Use auto-generated copy/move ctor, dctor, copy/move op=
    Product(const Product & other) = default;
    Product(Product && other) = default;
    ~Product () = default;
    Product & operator=(const Product & other) = default;
    Product & operator=(Product && other) = default;

	// getName
	// Pre: None.
	// Post: returns the product name as a string
    std::string getName() const {
        return _productName;
    }

	// getSales
	// Pre: None.
	// Post: returns the sales as an integer.
    int getSales () const {
    return _numberSold;
    }

	// setName
	// set name to given string 
	// Pre: None.
	// Post: _productName holds the given string.
    void setName(std::string inputName) {
		_productName = inputName;
			return;
    }

	// setSales
	// set sales to given int 
	// Pre: int passed must be positive.
	// Post: _numberSold holds the given int.
    void setSales (int inputSales) {
		_numberSold = inputSales;
			return;
    }

	// toString
	// returns a string representation of the stored information in the form
	//		_productName (sales: _numberSold)
	// Pre: None.
	// Post: gives data held in the product class as a string.
    std::string toString () const {
		std::stringstream output;
		output << _productName << " (sales: " << _numberSold << ")";
		return output.str(); 
    }

	// op++ [pre]
	// Pre: None.
	// Post:
	//     Sales has been incremented by 1.
	//     Return is *this.
    Product & operator++ () {
        ++_numberSold;
        return * this;
    }

	// op++ [post]
	// Pre: None.
	// Post:
	//     Sales has been incremented by 1.
	//     Return is former value of *this.
     Product operator++(int dummy)
    {
        Product save = *this;
        ++*this;
        return save;
    }

	 // op-- [pre]
	 // Pre: None.
	 // Post:
	 //     Sales have been decremented by 1. If _numberSold < 1 nothing happens.
	 //     Return is *this.
    Product & operator-- () {
        if(_numberSold > 0)
			--_numberSold;
		
        return * this;
    }

	// op-- [post]
	// Pre: None.
	// Post:
	//     Sales have been decremented by 1. If _numberSold < 1 nothing happens.
	//     Return is former value of *this.
     Product operator--(int dummy)
    {
        Product save = *this;
        --*this;
        return save;
    }




private:
	std::string _productName; // Name of the product
	int _numberSold; // Sales for given product


}; // End class Product



// *********************************************************************
// class Product - Declarations of associated global operators
// *********************************************************************


	// op<< (ostream,Product)
	// Prints given Product object as "_productName (sales: _numberSold)".
	// Pre: None.
	// Post: Given Product object printed to given stream as described. 
	//			Return is given stream.
	std::ostream & operator<<(std::ostream & str, const Product & obj);

	// op==
	// determines if two products are logically equal. (same name and sales).
	// Pre: None.
	// Post: returns a boolean describing the equality of two products
	bool operator== (const Product & lhs, const Product & rhs);

	// op!=
	// determines if two products are logically inequal. (different name or sales).
	// Pre: None.
	// Post: returns a boolean describing the inequality of two products.
	bool operator!= (const Product & lhs, const Product & rhs);

#endif // FILE_PRODUCT_H_INCLUDED
