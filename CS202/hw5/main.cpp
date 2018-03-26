/*
* hw6test.cpp
*
*  Created on: Nov 26, 2012
*      Author: hartman
*/
// based on:
//
// Glenn G. Chappell
// 30 Oct 2009
//

// Includes for code to be tested
#include "hw5.h"   // For function template printVector
#include "hw5.h"   // Double inclusion test

// Includes for testing package & code common to all test programs
#include <iostream>     // for std::cout, std::endl, std::cin
#include <string>       // for std::string
#include <stdexcept>    // for std::runtime_error

// Additional includes for this test program
#include <algorithm>    // for std::copy, std::equal
#include <stdexcept>    // for std::runtime_error
#include <string>       // for std::string
#include <cstdlib>      // for std::size_t
#include <vector>
#include <sstream>

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


// test_printVector
// Test suite for function template printVector
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_printVector(Tester & t)
{
	std::cout << "Test Suite: function template printVector" << std::endl;

	std::vector<int> vi;
	std::ostringstream os;

	// Check correctness
	printVector(os, vi);
	t.test(os.str() == "", "Empty vector<int>");
	vi.push_back(1);
	vi.push_back(2);
	vi.push_back(3);
	vi.push_back(4);

	os.str("");
	printVector(os, vi);
	t.test(os.str() == "1\n2\n3\n4\n", "vector<int> with 4 items");

	std::vector<double> vd;
	vd.push_back(1.1);
	vd.push_back(1.2);
	vd.push_back(1.3);
	vd.push_back(1.4);

	os.str("");
	printVector(os, vd);
	t.test(os.str() == "1.1\n1.2\n1.3\n1.4\n", "vector<double> with 4 items");

	os.str("");
	const std::vector<double> cvd(vd);
	printVector(os, cvd);
	t.test(os.str() == "1.1\n1.2\n1.3\n1.4\n", "const vector<double> with 4 items");

}


// test_RAIIPtr
// Test suite for function template printVector
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_RAIIPtr(Tester & t)
{
	std::cout << "Test Suite: class template RAIIPtr" << std::endl;

	RAIIPtr<int> pi(new int(13));

	t.test(true, "Can create RAIIPtr"); //succeeds if previous line compiles
	bool correctType = TypeCheck<int>::check(*pi);
	t.test(correctType, "operator* on RAIIPtr has correct return type.");
	t.test(*pi == 13, "operator* on RAIIPtr has correct value.");
	*pi = 7;
	t.test(*pi == 7, "operator* on RAIIPtr has correct value after changing value.");
	{
		RAIIPtr<Counter> pc(new Counter);
		t.test(Counter::getCtorCount() == 1, "RAIIPtr doesn't create new object.");
	}
	t.test(Counter::getExisting() == 0 && Counter::getDctorCount() == 1, "RAIIPtr deletes pointer when going out of scope.");
	RAIIPtr<Counter> pc2(new Counter);
	t.test(pc2->getExisting() == 1, "RAIIPtr operator-> works.");

}

// test_hw6
// Test suite for homework #6
// Uses other test-suite functions
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_hw6(Tester & t)
{
	// Do all the test suites
	std::cout << "TEST SUITES FOR CLASS TSmArray" << std::endl;
	test_printVector(t);
	test_RAIIPtr(t);
}


// ************************************************************************
// Main program
// ************************************************************************


// main
int main()
{
	Tester t;
	test_hw6(t);

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
