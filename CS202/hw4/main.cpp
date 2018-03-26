// Box_test.cpp
// Chris Hartman, 9/23/2010
//
// For CS 202 Fall 2010
// Test program for class Box
// Used in Assignment 2,

// Includes for code to be tested
#include "box.h"   // For class Box
#include "box.h"   // Double inclusion test

// Includes for testing package & code common to all test programs
#include <iostream>     // for std::cout, std::endl, std::cin
#include <string>       // for std::string
#include <stdexcept>    // for std::runtime_error

// Additional includes for this test program
#include <sstream>      // for std::ostringstream
#include <memory>       // for std::unique_ptr

// ************************************************************************
// Testing Package:
//     Class Tester - For Tracking Tests
// ************************************************************************


// class Tester
// For extremely simple unit testing.
// Keeps track of number of tests and number of passes.
// Use test (with success/failure parameter) to do a test.
// Get results with numTests, numPassed, numFailed, allPassed.
// Restart testing with reset.
// Invariants:
//     countTests_ == number of tests (calls to test) since last reset.
//     countPasses_ == number of times function test called with true param
//      since last reset.
//     0 <= countPasses_ <= countTests_.
//     tolerance_ >= 0.
class Tester {

	// ***** Tester: ctors, dctor, op= *****
public:

	// Default ctor
	// Sets countTests_, countPasses_ to zero, tolerance_ to given value
	// Pre: None.
	// Post:
	//     numTests == 0, countPasses == 0, tolerance_ == abs(theTolerance)
	// Does not throw (No-Throw Guarantee)
	Tester(double theTolerance = 0.0000001)
		:countTests_(0),
		countPasses_(0),
		tolerance_(theTolerance >= 0 ? theTolerance : -theTolerance)
	{}

	// Compiler-generated copy ctor, copy op=, dctor are used

	// ***** Tester: general public functions *****
public:

	// test
	// Handles single test, param indicates pass/fail
	// Pre: None.
	// Post:
	//     countTests_ incremented
	//     countPasses_ incremented if (success)
	//     Message indicating test name (if given)
	//      and pass/fail printed to cout
	// Does not throw (No-Throw Guarantee)
	//  - Assuming exceptions have not been turned on for cout.
	void test(bool success,
		const std::string & testName = "")
	{
		++countTests_;
		if (success) ++countPasses_;

		std::cout << "    ";
		if (testName != "")
		{
			std::cout << "Test: "
				<< testName
				<< " - ";
		}
		std::cout << (success ? "passed" : "********** FAILED **********")
			<< std::endl;
	}

	// ftest
	// Does single floating-point test.
	// Tests passes iff difference of first two values is <= tolerance.
	// Pre: None.
	// Post:
	//     countTests_ incremented
	//     countPasses_ incremented if (abs(val1-val2) <= tolerance_)
	//     Message indicating test name (if given)
	//      and pass/fail printed to cout
	// Does not throw (No-Throw Guarantee)
	void ftest(double val1,
		double val2,
		const std::string & testName = "")
	{
		test(val1 - val2 <= tolerance_ && val2 - val1 <= tolerance_, testName);
	}

	// reset
	// Resets *this to default constructed state
	// Pre: None.
	// Post:
	//     countTests_ == 0, countPasses_ == 0
	// Does not throw (No-Throw Guarantee)
	void reset()
	{
		countTests_ = 0;
		countPasses_ = 0;
	}

	// numTests
	// Returns the number of tests that have been done since last reset 
	// Pre: None.
	// Post:
	//     return == countTests_
	// Does not throw (No-Throw Guarantee)
	int numTests() const
	{
		return countTests_;
	}

	// numPassed
	// Returns the number of tests that have passed since last reset
	// Pre: None.
	// Post:
	//     return == countPasses_
	// Does not throw (No-Throw Guarantee)
	int numPassed() const
	{
		return countPasses_;
	}

	// numFailed
	// Returns the number of tests that have not passed since last reset
	// Pre: None.
	// Post:
	//     return + countPasses_ == numTests_
	// Does not throw (No-Throw Guarantee)
	int numFailed() const
	{
		return countTests_ - countPasses_;
	}

	// allPassed
	// Returns true if all tests since last reset have passed
	// Pre: None.
	// Post:
	//     return == (countPasses_ == countTests_)
	// Does not throw (No-Throw Guarantee)
	bool allPassed() const
	{
		return countPasses_ == countTests_;
	}

	// setTolerance
	// Sets tolerance_ to given value
	// Pre: None.
	// Post:
	//     tolerance_ = abs(theTolerance)
	// Does not throw (No-Throw Guarantee)
	void setTolerance(double theTolerance)
	{
		tolerance_ = (theTolerance >= 0 ? theTolerance : -theTolerance);
	}

	// ***** Tester: data members *****
private:

	int countTests_;    // Number of tests done since last reset
	int countPasses_;   // Number of tests passed since last reset
	double tolerance_;  // Tolerance for floating-point near-equality tests

};  // end class Tester


	// ************************************************************************
	// Testing Package:
	//     Class TypeCheck - Helper Class for Type Checking
	// ************************************************************************


	// class TypeCheck
	// This class exists in order to have static member function check, which
	// takes a parameter of a given type, by reference. Objects of type
	// TypeCheck<T> cannot be created.
	// Usage:
	//     TypeCheck<MyType>::check(x)
	//     returns true if the type of x is (MyType) or (const MyType),
	//     otherwise false.
	// Invariants: None.
	// Requirements on Types: None.
template<typename T>
class TypeCheck {

private:

	// Uncopyable class. Do not define copy ctor, copy assn.
	TypeCheck(const TypeCheck &);
	TypeCheck<T> & operator=(const TypeCheck &);

	// Compiler-generated dctor is used (but irrelevant).

public:

	// check
	// The function and function template below simulate a single function
	// that takes a single parameter, and returns true iff the parameter has
	// type T or (const T).

	// check (reference-to-const T)
	// Pre: None.
	// Post:
	//     Return is true.
	// Does not throw (No-Throw Guarantee)
	static bool check(const T & param)
	{
		return true;
	}

	// check (reference-to-const non-T)
	// Pre: None.
	// Post:
	//     Return is false.
	// Requirements on types: None.
	// Does not throw (No-Throw Guarantee)
	template <typename OtherType>
	static bool check(const OtherType & param)
	{
		return false;
	}

};  // End class TypeCheck


	// ************************************************************************
	// Testing Package:
	//     Class Counter - Helper Class for Counting Calls & Objects, Throwing
	// ************************************************************************


	// class Counter
	// Item type for counting ctor, dctor, op= calls, counting existing
	//  objects, and possibly throwing on copy. Has operator< (which always
	//  returns false), allowing it to be the value type of a sorted container.
	// If static member copyThrow_ is set, then copy ctor and copy assn throw
	//  std::runtime_error. Exception object's "what" member is set to "C" by
	//  the copy ctor and "A" by copy assn.
	// Increments static data member ctorCount_ on default construction and
	//  successful copy construction. Increments static data member assnCount_
	//  on successful copy assignment. Increments static data member
	//  dctorCount_ on destruction.
	// Increments static data member existing_ on construction, and decrements
	//  it on destruction.
	// Static data member maxExisting_ is highest value of existing_ since last
	//  reset, or start of program if reset has never been called.
	// Invariants:
	//     Counter::existing_ is number of existing objects of this class.
	//     Counter::ctorCount_ is number of successful ctor calls since
	//      most recent call to reset, or start of program if reset has never
	//      been called.
	//     Counter::dctorCount_ is (similarly) number of dctor calls.
	//     Counter::assnCount_ is (similarly) number of copy assn calls.
	//     Counter::maxExisting_ is (similarly) highest value existing_ has
	//      assumed.
class Counter {

	// ***** Counter: Ctors, dctor, op= *****
public:

	// Default ctor
	// Pre: None.
	// Post:
	//     (ctorCount_ has been incremented.)
	//     (existing_ has been incremented.)
	// Does not throw (No-Throw Guarantee)
	Counter()
	{
		++existing_;
		if (existing_ > maxExisting_)
			maxExisting_ = existing_;
		++ctorCount_;
	}

	// Copy ctor
	// Throws std::runtime_error if copyThrow_.
	// Pre: None.
	// Post:
	//     (ctorCount_ has been incremented.)
	//     (existing_ has been incremented.)
	// May throw std::runtime_error
	// Strong Guarantee
	Counter(const Counter & other)
	{
		if (copyThrow_)
			throw std::runtime_error("C");
		++existing_;
		if (existing_ > maxExisting_)
			maxExisting_ = existing_;
		++ctorCount_;
	}

	// Copy assignment
	// Throws std::runtime_error if copyThrow_.
	// Pre: None.
	// Post:
	//     Return value is *this.
	//     (assnCount_ has been incremented.)
	// May throw std::runtime_error
	// Strong Guarantee
	Counter & operator=(const Counter & rhs)
	{
		if (copyThrow_)
			throw std::runtime_error("A");
		++assnCount_;
		return *this;
	}

	// Dctor
	// Pre: None.
	// Post:
	//     (dctorCount_ has been incremented.)
	//     (existing_ has been decremented.)
	// Does not throw (No-Throw Guarantee)
	~Counter()
	{
		--existing_;
		++dctorCount_;
	}

	// ***** Counter: Functions dealing with count *****
public:

	// reset
	// Pre: None.
	// Post:
	//     maxExisting_ == existing_.
	//     ctorCount_ == 0.
	//     dctorCount_ == 0.
	//     assnCount_ == 0.
	//     copyThrow_ == shouldThrow.
	// Does not throw (No-Throw Guarantee)
	static void reset(bool shouldThrow = false)
	{
		maxExisting_ = existing_;
		ctorCount_ = 0;
		dctorCount_ = 0;
		assnCount_ = 0;
		copyThrow_ = shouldThrow;
	}

	// getExisting
	// Pre: None.
	// Post:
	//     return == existing_.
	// Does not throw (No-Throw Guarantee)
	static int getExisting()
	{
		return existing_;
	}

	// getMaxExisting
	// Pre: None.
	// Post:
	//     return == maxExisting_.
	// Does not throw (No-Throw Guarantee)
	static int getMaxExisting()
	{
		return maxExisting_;
	}

	// getCtorCount
	// Pre: None.
	// Post:
	//     return == ctorCount_.
	// Does not throw (No-Throw Guarantee)
	static int getCtorCount()
	{
		return ctorCount_;
	}

	// getDctorCount
	// Pre: None.
	// Post:
	//     return == dctorCount_.
	// Does not throw (No-Throw Guarantee)
	static int getDctorCount()
	{
		return dctorCount_;
	}

	// getAssnCount
	// Pre: None.
	// Post:
	//     return == assnCount_.
	// Does not throw (No-Throw Guarantee)
	static int getAssnCount()
	{
		return assnCount_;
	}

	// setCopyThrow
	// Pre: None.
	// Post:
	//     copyThrow_ == shouldThrow
	// Does not throw (No-Throw Guarantee)
	static void setCopyThrow(bool shouldThrow)
	{
		copyThrow_ = shouldThrow;
	}

	// ***** Counter: Data Members *****
private:

	static int existing_;     // # of existing objects
	static int maxExisting_;  // Max # of existing objects
	static int ctorCount_;    // # of successful (non-throwing) ctor calls
	static int dctorCount_;   // # of dctor calls
	static int assnCount_;    // # of successful (non-throwing) copy = calls
	static bool copyThrow_;   // true if copy operations (ctor, =) throw

};  // End class Counter

	// Definition of static data member of class Counter
int Counter::existing_ = 0;
int Counter::maxExisting_ = 0;
int Counter::ctorCount_ = 0;
int Counter::dctorCount_ = 0;
int Counter::assnCount_ = 0;
bool Counter::copyThrow_ = false;


// operator< (Counter)
// Dummy-ish operator<, forming a strict weak order for Counter class
// Returns false (which is legal for a strict weak order; all objects of
//  type Counter are equivalent).
// Pre: None.
// Post:
//     Return value == false.
// Does not throw (No-Throw Guarantee)
bool operator<(const Counter & a,
	const Counter & b)
{
	return false;
}


// ************************************************************************
// Test Suite Functions
// ************************************************************************


// test_class_Box_default_ctor
// Test suite for class Box, default ctor
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_Box_default_ctor(Tester & t)
{
	std::cout << "Test Suite: class Box, default ctor" << std::endl;

	std::string s1;       // Holds type
	int i1;          // Holds width and height

					 // Default construct const
	const FilledBox con1;

	// Check default constructed const width
	i1 = con1.getWidth();
	t.test(i1 == 1, "FilledBox Default ctor, width");

	// Check default constructed const height
	i1 = con1.getHeight();
	t.test(i1 == 1, "FilledBox Default ctor, height");

	// Check Filled constructed type
	s1 = con1.type();
	t.test(s1 == "Filled", "FilledBox, type");

	const HollowBox con2;
	const CheckeredBox con3;

	s1 = con2.type();
	t.test(s1 == "Hollow", "HollowBox, type");

	s1 = con3.type();
	t.test(s1 == "Checkered", "CheckeredBox, type");
}


// test_class_Box_data_ctor
// Test suite for class Box, ctor from data
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_Box_data_ctor(Tester & t)
{
	std::cout << "Test Suite: class Box, ctor from data" << std::endl;

	std::string s1;       // Holds type
	int i1;          // Holds width and height

					 // data ctor param type
	FilledBox con0(2, 2);  // This only needs to compile
	t.test(true, "Filled Ctor from data, two parameters");

	// Construct from data #1
	const HollowBox con1(3, 7);
	t.test(true, "Hollow Ctor from data, three parameter");

	// Check constructed from data #1 type
	s1 = con1.type();
	t.test(s1 == "Hollow", "Hollow Ctor from data, type");

	// Check constructed from data #1 width
	i1 = con1.getWidth();
	t.test(i1 == 3, "Hollow Ctor from data, width");

	// Check constructed from data #1 height
	i1 = con1.getHeight();
	t.test(i1 == 7, "Hollow Ctor from data, height");

	// Copy Construct
	const HollowBox ccon1(con1);
	t.test(true, "HollowBox Copy Ctor");

	// Check constructed from data #1 type
	s1 = ccon1.type();
	t.test(s1 == "Hollow", "Hollow Copy Ctor, type");

	// Check constructed from data #1 width
	i1 = ccon1.getWidth();
	t.test(i1 == 3, "Hollow Copy Ctor, width");

	// Check constructed from data #1 height
	i1 = ccon1.getHeight();
	t.test(i1 == 7, "Hollow Copy Ctor, height");

	// Construct from data #2 (const)
	const FilledBox con2(4, 2);

	// Check constructed from data #2 type
	s1 = con2.type();
	t.test(s1 == "Filled", "Filled Ctor from data, const, type");

	// Check constructed from data #2 height
	i1 = con2.getHeight();
	t.test(i1 == 2, "Filled Ctor from data, const, height");

	// Check constructed from data #2 width
	i1 = con2.getWidth();
	t.test(i1 == 4, "Filled Ctor from data, const, width");

	// Construct from data #2 (const)
	const CheckeredBox con3(2, 2);

	// Check constructed from data #2 type
	s1 = con3.type();
	t.test(s1 == "Checkered", "Checkered Ctor from data, const, type");
}


// test_class_Box_get_and_set_functions
// Test suite for class Box, get & set functions
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_Box_get_and_set_functions(Tester & t)
{
	std::cout << "Test Suite: class Box, get and set functions (uses FilledBox)" << std::endl;

	std::string s1;  // Holds type
	int i1;          // Holds width and height
	bool typeCheck;  // Holds typecheck result

	FilledBox con1(4, 5);
	const FilledBox & con1r(con1);
	FilledBox con2(6, 7);

	// setWidth parameter type
	FilledBox con0;
	con0.setWidth(2);  // This only needs to compile
	t.test(true, "setWidth param type = int");

	// Check getWidth return type non-const
	typeCheck = TypeCheck<int>::check(con1.getWidth());
	t.test(typeCheck, "getWidth non-const, return type = int");

	// Check getWidth return type const
	typeCheck = TypeCheck<int>::check(con1r.getWidth());
	t.test(typeCheck, "getWidth const, return type = int");

	// Check getHeight return type non-const
	typeCheck = TypeCheck<int>::check(con1.getHeight());
	t.test(typeCheck, "getHeight non-const, return type = int");

	// Check getHeight return type const
	typeCheck = TypeCheck<int>::check(con1r.getHeight());
	t.test(typeCheck, "getHeight const, return type = int");

	// Check type return type non-const
	typeCheck = TypeCheck<std::string>::check(con1.type());
	t.test(typeCheck, "type non-const, return type = int");

	// Check type return type const
	typeCheck = TypeCheck<std::string>::check(con1r.type());
	t.test(typeCheck, "type const, return type = int");

	// Check setWidth #1
	con1.setWidth(13);
	i1 = con1.getWidth();
	t.test(i1 == 13, "setWidth #1");

	// Check setHeight #1
	con1.setHeight(14);
	i1 = con1.getHeight();
	t.test(i1 == 14, "setHeight #1");
}


// test_class_Box_print
// Test suite for class Box, print
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_Box_print(Tester & t)
{
	std::cout << "Test Suite: class Box, print" << std::endl;

	std::ostringstream os; // Holds output

						   // Test #1
	FilledBox con1;

	// Check print #1
	os << con1;

	t.test(os.str() == "x\n", "print, default box");

	// Test #2 (const)
	const FilledBox con2(4, 3);

	// Check print #2
	os.str(""); //reset output holder
	os << con2;
	t.test(os.str() == "xxxx\nxxxx\nxxxx\n", "print 4x3 box, const");

	// Test #3 (1x1 hollow)
	HollowBox con3(1, 1);

	// Check print #3
	os.str(""); //reset output holder
	os << con3;
	t.test(os.str() == "x\n", "print 1x1 hollow box");

	// Test #4 (2x2 hollow)
	HollowBox con4(2, 2);

	// Check print #4
	os.str(""); //reset output holder
	os << con4;
	t.test(os.str() == "xx\nxx\n", "print 2x2 hollow box");

	// Test #5 (8x3 hollow)
	con4.setWidth(8);
	con4.setHeight(3);

	// Check print #5
	os.str(""); //reset output holder
	os << con4;
	t.test(os.str() == "xxxxxxxx\nx      x\nxxxxxxxx\n", "print 8x3 hollow box");

	// Test #6 (2x2 checkered)
	CheckeredBox con6(2, 2);

	// Check print #6
	os.str(""); //reset output holder
	os << con6;
	t.test(os.str() == "x \n x\n", "print 2x2 checkered box");

	// Test #7 (5x3 checkered)
	std::unique_ptr<Box> bptr = boxFactory('c', 5, 3);

	// Check print #7
	os.str(""); //reset output holder
	bptr->print(os);
	t.test(os.str() == "x x x\n x x \nx x x\n", "print 5x3 checkered box from factory");

	// Check pring #8
	bptr = boxFactory('h', 8, 3);
	// Check boxFactory hollow
	os.str(""); //reset output holder
	os << *bptr << "test";
	t.test(os.str() == "xxxxxxxx\nx      x\nxxxxxxxx\ntest", "print 8x3 hollow box from factory using cascaded <<");
}

class dBox : public FilledBox {
public:
	static bool destructedAny;
	~dBox() {
		destructedAny = true;
	}
};

bool dBox::destructedAny = false;

// test_class_Box_virtual_destructor
// Test suite for class Box, virtual destructor
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_Box_virtual_destructor(Tester & t)
{
	{
		std::unique_ptr<Box>(new dBox);
	}
	t.test(dBox::destructedAny, "Box class has virtual destructor.");
}

// test_class_Box
// Test suite for class Box
// Uses other test-suite functions
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate have been messages printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_Box(Tester & t)
{
	// Do all the test suites
	std::cout << "TEST SUITES FOR CLASS Box" << std::endl;
	test_class_Box_default_ctor(t);
	test_class_Box_data_ctor(t);
	test_class_Box_get_and_set_functions(t);
	test_class_Box_print(t);
	test_class_Box_virtual_destructor(t);
}


// ************************************************************************
// Main program
// ************************************************************************


// main
// Runs class Box test suite, prints results to cout.
int main()
{
	Tester t;
	test_class_Box(t);

	std::cout << std::endl;
	if (t.allPassed())
	{
		std::cout << "All tests successful"
			<< std::endl;
	}
	else
	{
		std::cout << "Tests ********** UNSUCCESSFUL **********"
			<< std::endl;
	}
	std::cout << std::endl;

	std::cout << "Press ENTER to quit ";
	while (std::cin.get() != '\n');

	return 0;
}
