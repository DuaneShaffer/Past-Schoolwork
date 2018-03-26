// da3_test_suites.cpp
// Glenn G. Chappell
// 24 Sep 2017
//
// For CS 311 Fall 2017
// Test program for Assignment 3 Functions & Function Templates
// Used in Assignment 3, Exercises A, B, C, D
// Requires da3_test_main.cpp, catch.hpp, da3.h, da3.cpp

// Includes for code to be tested
#include "da3.h"           // For Assignment 3 Functions & Templates
#include "da3.h"           // Double inclusion test

#define CATCH_CONFIG_FAST_COMPILE
                           // Disable some features for faster compile
#include "catch.hpp"       // For the "Catch" unit-testing framework

// Additional includes for this test program
#include <stdexcept>
using std::out_of_range;
using std::runtime_error;
#include <vector>
using std::vector;
#include <algorithm>
using std::equal;
#include <deque>
using std::deque;
#include <utility>
using std::pair;
#include <string>
using std::string;
#include <initializer_list>
using std::initializer_list;


// *********************************************************************
// Helper Functions/Classes for This Test Program
// *********************************************************************


// class TypeCheck
// This class exists in order to have static member function "check",
// which takes a parameter of a given type, by reference. Objects of
// type TypeCheck<T> cannot be created.
// Usage:
//     TypeCheck<MyType>::check(x)
//      returns true if the type of x is (MyType) or (const MyType),
//      otherwise false.
// Invariants: None.
// Requirements on Types: None.
template<typename T>
class TypeCheck {

private:

    // No default ctor
    TypeCheck() = delete;

    // Uncopyable. Do not define copy/move ctor/assn.
    TypeCheck(const TypeCheck &) = delete;
    TypeCheck(TypeCheck &&) = delete;
    TypeCheck<T> & operator=(const TypeCheck &) = delete;
    TypeCheck<T> & operator=(TypeCheck &&) = delete;

    // Compiler-generated dctor is used (but irrelevant).
    ~TypeCheck() = default;

public:

    // check
    // The function and function template below simulate a single
    // function that takes a single parameter, and returns true iff the
    // parameter has type T or (const T).

    // check (reference-to-const T)
    // Pre: None.
    // Post:
    //     Return is true.
    // Does not throw (No-Throw Guarantee)
    static bool check(const T & param)
    { return true; }

    // check (reference-to-const non-T)
    // Pre: None.
    // Post:
    //     Return is false.
    // Requirements on types: None.
    // Does not throw (No-Throw Guarantee)
    template <typename OtherType>
    static bool check(const OtherType & param)
    { return false; }

};  // End class TypeCheck


// vector call_list
// For storing integers representing function calls.
// Used in testing function callBetween.
vector<int> call_list;


// CheckCalls
// Check whether vector call_list holds given values.
bool checkCalls(const initializer_list<int> & vals)
{
    // Note: We could use the 4-iterator version of std::equal, but that
    // requires C++14.

    if (call_list.size() != vals.size())
        return false;

    return equal(vals.begin(), vals.end(),
                 call_list.begin());
}


// Function fnothrow1
// Function that does not throw #1.
void fnothrow1()
{
    call_list.push_back(11);
}


// Function fnothrow2
// Function that does not throw #2.
void fnothrow2()
{
    call_list.push_back(12);
}


// Function fnothrow3
// Function that does not throw #3.
void fnothrow3()
{
    call_list.push_back(13);
}


// Function object fonothrow1
// Function object that does not throw #1.
auto fonothrow1 = []()
{
    call_list.push_back(21);
};


// Function object fonothrow2
// Function object that does not throw #2.
auto fonothrow2 = []()
{
    call_list.push_back(22);
};


// Function object fonothrow3
// Function object that does not throw #3.
auto fonothrow3 = []()
{
    call_list.push_back(23);
};


// Function fthrowrt1
// Function that throws std::runtime_error #1.
void fthrowrt1()
{
    call_list.push_back(31);
    throw runtime_error("");
}


// Function fthrowrt2
// Function that throws std::runtime_error #2.
void fthrowrt2()
{
    call_list.push_back(32);
    throw runtime_error("");
}


// Function fthrowint1
// Function that throws int #1.
void fthrowint1()
{
    call_list.push_back(41);
    throw 0;
}


// Function fthrowint2
// Function that throws int #2.
void fthrowint2()
{
    call_list.push_back(42);
    throw 0;
}


// *********************************************************************
// Test Cases
// *********************************************************************


TEST_CASE( "Function return types are correct",
           "[types]" )
{
    int throw_stat;  // 0: no throw, 1: throw, 2: throw other

    SECTION( "lookUp returns value type by value" )
    {
        LLNode<int> * headi = nullptr;
        headi = new LLNode<int>(1, headi);
        try
        {
            int && ii(lookUp(headi, 0));
            throw_stat = 0;
        }
        catch (...)
        {
            throw_stat = 1;
        }

        {
        INFO( "lookUp does not throw" );
        REQUIRE( throw_stat == 0 );
        }
        {
        INFO( "lookup returns int" );
        REQUIRE( TypeCheck<int>::check(lookUp(headi, 0)) );
        }

        LLNode<double> * headd = nullptr;
        headd = new LLNode<double>(1.1, headd);
        try
        {
            double && dd(lookUp(headd, 0));
            throw_stat = 0;
        }
        catch (...)
        {
            throw_stat = 1;
        }

        {
        INFO( "lookUp does not throw" );
        REQUIRE( throw_stat == 0 );
        }
        {
        INFO( "lookup returns double" );
        REQUIRE( TypeCheck<double>::check(lookUp(headd, 0)) );
        }
    }

    SECTION( "uniqueCount returns size_t by value" )
    {
        vector<int> v { 1, 2, 3 };
        size_t && st(uniqueCount(v.begin(), v.end()));
        INFO( "uniqueCount must return std::size_t" );
        REQUIRE( TypeCheck<size_t>::check(uniqueCount(v.begin(), v.end())) );
    }

    SECTION( "gcd returns int by value" )
    {
        int && i(gcd(2,3));
        INFO( "gcd must return int" );
        REQUIRE( TypeCheck<int>::check(gcd(2,3)) );
    }
}


TEST_CASE( "Function template lookUp",
           "[ex-a]" )
{
    int throw_stat;  // 0: no throw, 1: throw, 2: throw other
    bool nonempty_what;  // Valid only if throw_stat == 1

    LLNode<int> * headi;     // Head ptr for all int Linked Lists
    LLNode<double> * headd;  // Head ptr for all double Linked Lists

    int ival;                // int value
    double dval;             // double value

    SECTION( "Size 0" )
    {
        // Construct list
        headi = nullptr;

        // Index 0
        try
        {
            int && ii = lookUp(headi, 0);
            throw_stat = 0;
        }
        catch (out_of_range & e)
        {
            throw_stat = 1;
            nonempty_what = (string("") != e.what());
        }
        catch (...)
        {
            throw_stat = 2;
        }

        {
        INFO( "lookUp must throw on out-of-range index" );
        REQUIRE( throw_stat != 0 );
        }
        {
        INFO( "lookUp must throw only std::out_of_range" );
        REQUIRE( throw_stat == 1 );
        }
        {
        INFO( "lookUp must throw exception with nonempty .what()" );
        REQUIRE( nonempty_what );
        }

        // Index 1
        try
        {
            int && ii = lookUp(headi, 1);
            throw_stat = 0;
        }
        catch (out_of_range & e)
        {
            throw_stat = 1;
            nonempty_what = (string("") != e.what());
        }
        catch (...)
        {
            throw_stat = 2;
        }

        {
        INFO( "lookUp must throw on out-of-range index" );
        REQUIRE( throw_stat != 0 );
        }
        {
        INFO( "lookUp must throw only std::out_of_range" );
        REQUIRE( throw_stat == 1 );
        }
        {
        INFO( "lookUp must throw exception with nonempty .what()" );
        REQUIRE( nonempty_what );
        }
    }

    SECTION( "Size 1" )
    {
        // Construct list
        headd = nullptr;
        headd = new LLNode<double>(3.2, headd);

        // Index 0
        try
        {
            double && dd = lookUp(headd, 0);
            throw_stat = 0;
            dval = dd;
        }
        catch (...)
        {
            throw_stat = 1;
        }

        {
        INFO( "lookUp must not throw on in-range index" );
        REQUIRE( throw_stat == 0 );
        }
        {
        INFO( "lookUp must return 3.2" );
        REQUIRE( dval == 3.2 );
        }

        // Index 1
        try
        {
            double && dd = lookUp(headd, 1);
            throw_stat = 0;
        }
        catch (out_of_range & e)
        {
            throw_stat = 1;
            nonempty_what = (string("") != e.what());
        }
        catch (...)
        {
            throw_stat = 2;
        }

        {
        INFO( "lookUp must throw on out-of-range index" );
        REQUIRE( throw_stat != 0 );
        }
        {
        INFO( "lookUp must throw only std::out_of_range" );
        REQUIRE( throw_stat == 1 );
        }
        {
        INFO( "lookUp must throw exception with nonempty .what()" );
        REQUIRE( nonempty_what );
        }
    }

    SECTION( "Size 9" )
    {
        // Construct list
        headi = nullptr;
        headi = new LLNode<int>(64, headi);
        headi = new LLNode<int>(49, headi);
        headi = new LLNode<int>(36, headi);
        headi = new LLNode<int>(25, headi);
        headi = new LLNode<int>(16, headi);
        headi = new LLNode<int>(9, headi);
        headi = new LLNode<int>(4, headi);
        headi = new LLNode<int>(1, headi);
        headi = new LLNode<int>(0, headi);

        // Index 0
        try
        {
            int && ii = lookUp(headi, 0);
            throw_stat = 0;
            ival = ii;
        }
        catch (...)
        {
            throw_stat = 1;
        }

        {
        INFO( "lookUp must not throw on in-range index" );
        REQUIRE( throw_stat == 0 );
        }
        {
        INFO( "lookUp must return 0" );
        REQUIRE( ival == 0 );
        }

        // Index 5
        try
        {
            int && ii = lookUp(headi, 5);
            throw_stat = 0;
            ival = ii;
        }
        catch (...)
        {
            throw_stat = 1;
        }

        {
        INFO( "lookUp must not throw on in-range index" );
        REQUIRE( throw_stat == 0 );
        }
        {
        INFO( "lookUp must return 25" );
        REQUIRE( ival == 25 );
        }

        // Index 8
        try
        {
            int && ii = lookUp(headi, 8);
            throw_stat = 0;
            ival = ii;
        }
        catch (...)
        {
            throw_stat = 1;
        }

        {
        INFO( "lookUp must not throw on in-range index" );
        REQUIRE( throw_stat == 0 );
        }
        {
        INFO( "lookUp must return 64" );
        REQUIRE( ival == 64 );
        }

        // Index 9
        try
        {
            int && ii = lookUp(headi, 9);
            throw_stat = 0;
        }
        catch (out_of_range & e)
        {
            throw_stat = 1;
            nonempty_what = (string("") != e.what());
        }
        catch (...)
        {
            throw_stat = 2;
        }

        {
        INFO( "lookUp must throw on out-of-range index" );
        REQUIRE( throw_stat != 0 );
        }
        {
        INFO( "lookUp must throw only std::out_of_range" );
        REQUIRE( throw_stat == 1 );
        }
        {
        INFO( "lookUp must throw exception with nonempty .what()" );
        REQUIRE( nonempty_what );
        }

        // Index 100
        try
        {
            int && ii = lookUp(headi, 100);
            throw_stat = 0;
        }
        catch (out_of_range & e)
        {
            throw_stat = 1;
            nonempty_what = (string("") != e.what());
        }
        catch (...)
        {
            throw_stat = 2;
        }

        {
        INFO( "lookUp must throw on out-of-range index" );
        REQUIRE( throw_stat != 0 );
        }
        {
        INFO( "lookUp must throw only std::out_of_range" );
        REQUIRE( throw_stat == 1 );
        }
        {
        INFO( "lookUp must throw exception with nonempty .what()" );
        REQUIRE( nonempty_what );
        }
    }
}


TEST_CASE( "Function callBetween",
           "[ex-b]" )
{
    int throw_stat;  // 0: no throw, 1: throw, 2: throw other

    call_list.clear();

    SECTION( "Three nonthrowing functions" )
    {
        try
        {
            callBetween(fnothrow1, fnothrow2, fnothrow3);
            throw_stat = 0;
        }
        catch (...)
        {
            throw_stat = 1;
        }

        {
        INFO( "callBetween must not throw" );
        REQUIRE( throw_stat == 0 );
        }
        {
        INFO( "callBetween must make the required function calls" );
        REQUIRE( checkCalls({11, 12, 13}) );
        }
    }

    SECTION( "Three nonthrowing function objects" )
    {
        try
        {
            callBetween(fonothrow1, fonothrow2, fonothrow3);
            throw_stat = 0;
        }
        catch (...)
        {
            throw_stat = 1;
        }

        {
        INFO( "callBetween must not throw" );
        REQUIRE( throw_stat == 0 );
        }
        {
        INFO( "callBetween must make the required function calls" );
        REQUIRE( checkCalls({21, 22, 23}) );
        }
    }

    SECTION( "Throwing START: throws std::runtime_error" )
    {
        try
        {
            callBetween(fthrowrt1, fnothrow2, fnothrow3);
            throw_stat = 0;
        }
        catch (runtime_error & e)
        {
            throw_stat = 1;
        }
        catch (...)
        {
            throw_stat = 2;
        }

        {
        INFO( "callBetween must throw" );
        REQUIRE( throw_stat != 0 );
        }
        {
        INFO( "callBetween must throw std::runtime_error" );
        REQUIRE( throw_stat == 1 );
        }
        {
        INFO( "callBetween must make the required function calls" );
        REQUIRE( checkCalls({31}) );
        }
    }

    SECTION( "Throwing START: throws int" )
    {
        try
        {
            callBetween(fthrowint1, fnothrow2, fnothrow3);
            throw_stat = 0;
        }
        catch (int & e)
        {
            throw_stat = 1;
        }
        catch (...)
        {
            throw_stat = 2;
        }

        {
        INFO( "callBetween must throw" );
        REQUIRE( throw_stat != 0 );
        }
        {
        INFO( "callBetween must throw int" );
        REQUIRE( throw_stat == 1 );
        }
        {
        INFO( "callBetween must make the required function calls" );
        REQUIRE( checkCalls({41}) );
        }
    }

    SECTION( "Throwing MIDDLE: throws std::runtime_error" )
    {
        try
        {
            callBetween(fnothrow1, fthrowrt2, fnothrow3);
            throw_stat = 0;
        }
        catch (runtime_error & e)
        {
            throw_stat = 1;
        }
        catch (...)
        {
            throw_stat = 2;
        }

        {
        INFO( "callBetween must throw" );
        REQUIRE( throw_stat != 0 );
        }
        {
        INFO( "callBetween must throw std::runtime_error" );
        REQUIRE( throw_stat == 1 );
        }
        {
        INFO( "callBetween must make the required function calls" );
        REQUIRE( checkCalls({11,32,13}) );
        }
    }

    SECTION( "Throwing MIDDLE: throws int" )
    {
        try
        {
            callBetween(fnothrow1, fthrowint2, fnothrow3);
            throw_stat = 0;
        }
        catch (int & e)
        {
            throw_stat = 1;
        }
        catch (...)
        {
            throw_stat = 2;
        }

        {
        INFO( "callBetween must throw" );
        REQUIRE( throw_stat != 0 );
        }
        {
        INFO( "callBetween must throw int" );
        REQUIRE( throw_stat == 1 );
        }
        {
        INFO( "callBetween must make the required function calls" );
        REQUIRE( checkCalls({11,42,13}) );
        }
    }
}


TEST_CASE( "Function template uniqueCount",
           "[ex-c]" )
{
    deque<int> di {                  // Test data
        1, 1, 2, 1, 2, 2, 3, -1, -1, -1, 5, 3, 3, 3, 2, 2, 1, 1, 1
    };
    const size_t bigsize = 10000;  // Size of very long ranges
                                   //  Must be > 9000

    SECTION( "Empty range" )
    {
        size_t && result = uniqueCount(di.rbegin(), di.rbegin());
        INFO( "uniqueCount must return 0" );
        REQUIRE( result == 0 );
    }

    SECTION( "Range of size 1" )
    {
        size_t && result = uniqueCount(di.rbegin()+3, di.rbegin()+4);
        INFO( "uniqueCount must return 1" );
        REQUIRE( result == 1 );
    }

    SECTION( "Range of size 2, equal integers" )
    {
        size_t && result = uniqueCount(di.rbegin()+3, di.rbegin()+5);
        INFO( "uniqueCount must return 1" );
        REQUIRE( result == 1 );
    }

    SECTION( "Range of size 2, distinct integers" )
    {
        size_t && result = uniqueCount(di.rbegin()+4, di.rbegin()+6);
        INFO( "uniqueCount must return 2" );
        REQUIRE( result == 2 );
    }

    SECTION( "Range of size 3, equal integers" )
    {
        size_t && result = uniqueCount(di.rbegin()+9, di.rbegin()+12);
        INFO( "uniqueCount must return 1" );
        REQUIRE( result == 1 );
    }

    SECTION( "Range of size 3, two values" )
    {
        size_t && result = uniqueCount(di.rbegin()+8, di.rbegin()+11);
        INFO( "uniqueCount must return 2" );
        REQUIRE( result == 2 );
    }

    SECTION( "Range of size 3, all distinct" )
    {
        size_t && result = uniqueCount(di.rbegin()+7, di.rbegin()+10);
        INFO( "uniqueCount must return 3" );
        REQUIRE( result == 3 );
    }

    SECTION( "Longer range #1" )
    {
        size_t && result = uniqueCount(di.rbegin()+5, di.rbegin()+13);
        INFO( "uniqueCount must return 3" );
        REQUIRE( result == 3 );
    }

    SECTION( "Longer range #2" )
    {
        size_t && result = uniqueCount(di.rbegin()+3, di.rbegin()+15);
        INFO( "uniqueCount must return 4" );
        REQUIRE( result == 4 );
    }

    SECTION( "Longer range #3" )
    {
        size_t && result = uniqueCount(di.rbegin(), di.rend());
        INFO( "uniqueCount must return 5" );
        REQUIRE( result == 5 );
    }

    SECTION( "Very long range #1" )
    {
        deque<int> di2(bigsize, 3);
        size_t && result = uniqueCount(di2.rbegin(), di2.rend());
        INFO( "uniqueCount must return 1" );
        REQUIRE( result == 1 );
    }

    SECTION( "Very long range #2" )
    {
        deque<int> di2(bigsize, 4);
        di2[9000] = 3;
        size_t && result = uniqueCount(di2.rbegin(), di2.rend());
        INFO( "uniqueCount must return 2" );
        REQUIRE( result == 2 );
    }

    SECTION( "Very long range #3" )
    {
        deque<int> di2(bigsize, 5);
        for (size_t i = 0; i < bigsize; i += 2)
            di2[i] = 6;
        size_t && result = uniqueCount(di2.rbegin(), di2.rend());
        INFO( "uniqueCount must return 2" );
        REQUIRE( result == 2 );
    }

    SECTION( "Very long range #4" )
    {
        deque<size_t> dsi2(bigsize);
        for (size_t i = 0; i < bigsize; ++i)
            dsi2[i] = i;
        size_t && result = uniqueCount(dsi2.rbegin(), dsi2.rend());
        INFO( "uniqueCount must return " << bigsize );
        REQUIRE( result == bigsize );
    }

    SECTION( "Strings, equal" )
    {
        vector<string> vs { "abc", "abc" };
        size_t && result = uniqueCount(vs.begin(), vs.end());
        INFO( "uniqueCount must return 1" );
        REQUIRE( result == 1 );
    }

    SECTION( "Strings, distinct" )
    {
        vector<string> vs { "abc", "def" };
        size_t && result = uniqueCount(vs.begin(), vs.end());
        INFO( "uniqueCount must return 2" );
        REQUIRE( result == 2 );
    }
}


TEST_CASE( "Function gcd",
           "[ex-d]" )
{
    // Test data
    vector<vector<int> > tdata {
        {       0,      1,     1 },
        {       1,      0,     1 },
        {       0,  27384, 27384 },
        {   27384,      0, 27384 },
        {       1,      1,     1 },
        {       3,     10,     1 },
        {      10,      3,     1 },
        {       6,     15,     3 },
        {      15,      6,     3 },
        {     546,  49335,    39 },
        {   49335,    546,    39 },
        {       5, 100000,     5 },
        {  100000,      5,     5 },
        {       5, 100001,     1 },
        {  100001,      5,     1 },
        {     910,     42,    14 },
        {      42,    910,    14 },
        {  311850, 429975,  4725 },
        {  429975, 311850,  4725 },
        {  196418, 317811,     1 },
        {  317811, 196418,     1 },
        {  196418, 317812,     2 },
        {  317812, 196418,     2 },
    };

    for (const auto v : tdata)
    {
        int && result = gcd(v[0], v[1]);
        INFO( "The GCD of " << v[0] << " and " << v[1] << " is " << v[2]
                  << ", but calling gcd(" << v[0] << ", " << v[1]
                  << ") returned " << result << "." );
        REQUIRE( result == v[2] );
    }
}

