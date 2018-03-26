/*
 * hw1test.cpp
 *
 *  Created on: Feb 4, 2011
 *      Author: hartman
 */

// Includes for code to be tested
#include "hw1.h"
#include "hw1.h"   // Double inclusion test

// Includes for testing package & code common to all test programs
#include <iostream>     // for std::cout, std::endl, std::cin
#include <string>       // for std::string
#include <stdexcept>    // for std::runtime_error

// Additional includes for this test program
#include <sstream>      // for std::ostringstream


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
    { test(val1-val2 <= tolerance_ && val2-val1 <= tolerance_, testName); }

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
    { return countTests_; }

    // numPassed
    // Returns the number of tests that have passed since last reset
    // Pre: None.
    // Post:
    //     return == countPasses_
    // Does not throw (No-Throw Guarantee)
    int numPassed() const
    { return countPasses_; }

    // numFailed
    // Returns the number of tests that have not passed since last reset
    // Pre: None.
    // Post:
    //     return + countPasses_ == numTests_
    // Does not throw (No-Throw Guarantee)
    int numFailed() const
    { return countTests_ - countPasses_; }

    // allPassed
    // Returns true if all tests since last reset have passed
    // Pre: None.
    // Post:
    //     return == (countPasses_ == countTests_)
    // Does not throw (No-Throw Guarantee)
    bool allPassed() const
    { return countPasses_ == countTests_; }

    // setTolerance
    // Sets tolerance_ to given value
    // Pre: None.
    // Post:
    //     tolerance_ = abs(theTolerance)
    // Does not throw (No-Throw Guarantee)
    void setTolerance(double theTolerance)
    { tolerance_ = (theTolerance >= 0 ? theTolerance : -theTolerance); }

// ***** Tester: data members *****
private:

    int countTests_;    // Number of tests done since last reset
    int countPasses_;   // Number of tests passed since last reset
    double tolerance_;  // Tolerance for floating-point near-equality tests

};  // end class Tester


// ************************************************************************
// Test Suite Functions
// ************************************************************************

// test_getIdFromFile
// Test suite for getIdFromFile
void test_getIdFromFile(Tester & t)
{
   std::cout << "TESTS FOR getIdFromFile" << std::endl;
   std::cout << "Tests with file having final newline:" << std::endl;
   std::ostringstream os; // Holds output
   std::istringstream is; // Holds input to getIdFromFile
   string expectedOut;
   int res;

   os.clear();
   os.str("");
   is.clear();
   is.str("sclaus\n");
   expectedOut="12\n";
   res = getIdFromFile("users.dat",is,os);
   t.test(res==0, "getIdFromFile return, file exists");
   t.test(os.str() == expectedOut, "getIdFromFile output, one name, in file");

   os.clear();
   os.str("");
   is.clear();
   is.str("jblack\nsclaus\n");
   expectedOut="12\n";
   res = getIdFromFile("notthere.dat",is,os);
   t.test(res==-1, "getIdFromFile return, can't open file");

   os.clear();
   os.str("");
   is.clear();
   is.str("jblack\nsclaus\n");
   expectedOut="179\n12\n";
   getIdFromFile("users.dat",is,os);
   t.test(os.str() == expectedOut, "getIdFromFile output, two names, in file");

   os.clear();
   os.str("");
   is.clear();
   is.str("sclaus\nofnord\njblack");
   expectedOut="12\nerror\n179\n";
   getIdFromFile("users.dat",is,os);
   t.test(os.str() == expectedOut, "getIdFromFile output, three names, one not in file");

   os.clear();
   os.str("");
   is.clear();
   is.str("dadams\njstalin\nsjobs\njstalin\netel\njgalt\nsclaus\n");
   expectedOut="42\n666\n123\n666\nerror\n0\n12\n";
   getIdFromFile("users.dat",is,os);
   t.test(os.str() == expectedOut, "getIdFromFile output, several names, some repeated or out of order or not in file");

   std::cout << "Tests with file NOT having final newline:" << std::endl;
   os.clear();
   os.str("");
   is.clear();
   is.str("sclaus\n");
   expectedOut="12\n";
   res = getIdFromFile("users.dat",is,os);
   t.test(res==0, "getIdFromFile return, file exists");
   t.test(os.str() == expectedOut, "getIdFromFile output, one name, in file");

   os.clear();
   os.str("");
   is.clear();
   is.str("jblack\nsclaus\n");
   expectedOut="12\n";
   res = getIdFromFile("notthere.dat",is,os);
   t.test(res==-1, "getIdFromFile return, can't open file");

   os.clear();
   os.str("");
   is.clear();
   is.str("jblack\nsclaus\n");
   expectedOut="179\n12\n";
   getIdFromFile("users.dat",is,os);
   t.test(os.str() == expectedOut, "getIdFromFile output, two names, in file");

   os.clear();
   os.str("");
   is.clear();
   is.str("sclaus\nofnord\njblack");
   expectedOut="12\nerror\n179\n";
   getIdFromFile("users.dat",is,os);
   t.test(os.str() == expectedOut, "getIdFromFile output, three names, one not in file");

   os.clear();
   os.str("");
   is.clear();
   is.str("dadams\njstalin\nsjobs\njstalin\netel\njgalt\nsclaus\n");
   expectedOut="42\n666\n123\n666\nerror\n0\n12\n";
   getIdFromFile("users.dat",is,os);
   t.test(os.str() == expectedOut, "getIdFromFile output, several names, some repeated or out of order or not in file");
}

void test_numberChase(Tester & t)
{
std::cout << "TESTS FOR numberChase" << std::endl;
std::ostringstream os; // Holds output
string expectedOut;

os.clear();
os.str("");

expectedOut="3\n2\n-77\n";
numberChase("goseek1.dat",os);
t.test(os.str() == expectedOut, "numberChase output, short example from assignment");

os.clear();
os.str("");

std::ostringstream exOut; // Holds output
for(int ii=0;ii<99;++ii)
   exOut << ii+1 << "\n";
exOut << -1 << "\n";

numberChase("goseek2.dat",os);
t.test(os.str() == exOut.str(), "numberChase output, longer example, in order");

os.clear();
os.str("");
exOut.clear();
exOut.str("");

int dest=3;
for(int ii=0;ii<99;++ii)
   {
   exOut << dest << "\n";
   (dest += 3) %= 100;
   }
exOut << -13 << "\n";

numberChase("goseek3.dat",os);
t.test(os.str() == exOut.str(), "numberChase output, longer example, jumps around");
}

// ************************************************************************
// Main program
// ************************************************************************


// main
// Runs class Box test suite, prints results to cout.
int main()
{
    Tester t;
    test_getIdFromFile(t);
    test_numberChase(t);

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
    while (std::cin.get() != '\n') ;

    return 0;
}

