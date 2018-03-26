// product_test_suites.cpp
// Glenn G. Chappell
// 7 Sep 2017
//
// For CS 311 Fall 2017
// Tests for class Product: test suites
// For Assignment 1, Exercise A
// Uses the "Catch" unit-testing framework
// Requires product_test_main.cpp, catch.hpp, product.h, product.cpp

#include "product.h"       // For class Product
#include "product.h"       // Double inclusion test

#define CATCH_CONFIG_FAST_COMPILE
                           // Disable some features for faster compile
#include "catch.hpp"       // For the "Catch" unit-testing framework

// Additional includes for this test program
#include <string>
using std::string;
#include <ostream>
using std::ostream;
#include <sstream>
using std::ostringstream;
#include <cstddef>
using std::size_t;
#include <utility>
using std::move;


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


// *********************************************************************
// Test Cases
// *********************************************************************


TEST_CASE( "Function return types are correct",
           "[types]" )
{
    Product t1;
    Product t2;

    SECTION( "Copy assn returns Product by ref" )
    {
        Product & r(t1 = t2);  // By-reference check
        REQUIRE( TypeCheck<Product>::check(t1 = t2) );
    }

    SECTION( "Move assn returns Product by ref" )
    {
        Product t_movable;
        Product & r(t1 = move(t_movable));  // By-reference check
        REQUIRE( TypeCheck<Product>::check(t1 = move(t_movable)) );
    }

    SECTION( "Product::getName returns string by value" )
    {
        string && rr(t1.getName());  // by-value check
        REQUIRE( TypeCheck<string>::check(t1.getName()) );
    }

    SECTION( "Product::getSales returns int by value" )
    {
        int && rr(t1.getSales());  // by-value check
        REQUIRE( TypeCheck<int>::check(t1.getSales()) );
    }

    SECTION( "Product::toString returns string by value" )
    {
        string && rr(t1.toString());  // by-value check
        REQUIRE( TypeCheck<string>::check(t1.toString()) );
    }

    SECTION( "Product::operator++ [pre] returns Product by ref" )
    {
        Product & r(++t1);  // By-reference check
        REQUIRE( TypeCheck<Product>::check(++t1) );
    }

    SECTION( "Product::operator++ [post] returns Product by value" )
    {
        Product && rr(t1++);  // by-value check
        REQUIRE( TypeCheck<Product>::check(t1++) );
    }

    SECTION( "Product::operator-- [pre] returns Product by ref" )
    {
        Product & r(--t1);  // By-reference check
        REQUIRE( TypeCheck<Product>::check(--t1) );
    }

    SECTION( "Product::operator-- [post] returns Product by value" )
    {
        Product && rr(t1--);  // by-value check
        REQUIRE( TypeCheck<Product>::check(t1--) );
    }

    SECTION( "operator==(Product,Product) returns bool by value" )
    {
        bool && rr(t1 == t2);  // by-value check
        REQUIRE( TypeCheck<bool>::check(t1 == t2) );
    }

    SECTION( "operator!=(Product,Product) returns bool by value" )
    {
        bool && rr(t1 != t2);  // by-value check
        REQUIRE( TypeCheck<bool>::check(t1 != t2) );
    }

    SECTION( "operator<<(ostream,Product) returns ostream by ref" )
    {
        ostringstream ostr;
        ostream & r(ostr << t1);  // By-reference check
        REQUIRE( TypeCheck<ostream>::check(ostr << t1) );
    }
}


TEST_CASE( "Functions take const arguments (passes if it compiles)",
           "[types]" )
{
    SECTION( "Taking const Product arguments" )
    {
        const Product tc1;
        const Product tc2;

        string s;
        int w;
        s = tc1.getName();       // Product::getName
        w = tc1.getSales();      // Product::getSales
        s = tc1.toString();      // Product::toString

        Product t3 = tc1;        // Copy ctor
        t3 = tc1;                // Copy assn

        bool b1 = (tc1 == tc2);  // op==(Product,Product)
        bool b2 = (tc1 != tc2);  // op!=(Product,Product)
        ostringstream ostr;
        ostr << tc1;             // op<<(ostring,Product)

        REQUIRE( 0 == 0 );       // Section passes if it compiles
    }

    SECTION( "Taking const string arguments" )
    {
        const string s;

        Product t(s, 0);         // Data ctor
        t.setName(s);            // Product::setName

        REQUIRE( 0 == 0 );       // Section passes if it compiles
    }
}


TEST_CASE( "Default-constructed Product object: blank name, 0 sales",
           "[objs]" )
{
    Product t1;

    REQUIRE( t1.getName() == "" );
    REQUIRE( t1.getSales() == 0 );

    SECTION( "Set name" )
    {
        t1.setName("abc");

        REQUIRE( t1.getName() == "abc" );
        REQUIRE( t1.getSales() == 0 );
    }

    SECTION( "Set sales" )
    {
        t1.setSales(20);

        REQUIRE( t1.getName() == "" );
        REQUIRE( t1.getSales() == 20 );
    }

    SECTION( "Set name, sales" )
    {
        t1.setName("def");
        t1.setSales(30);

        REQUIRE( t1.getName() == "def" );
        REQUIRE( t1.getSales() == 30 );
    }

    SECTION( "Set sales, name" )
    {
        t1.setSales(40);
        t1.setName("ghi");

        REQUIRE( t1.getName() == "ghi" );
        REQUIRE( t1.getSales() == 40 );
    }
}


TEST_CASE( "Data-constructed Product object",
           "[objs]" )
{
    Product t1("xyz", 100);

    REQUIRE( t1.getName() == "xyz" );
    REQUIRE( t1.getSales() == 100 );

    SECTION( "Set name" )
    {
        t1.setName("abc");

        REQUIRE( t1.getName() == "abc" );
        REQUIRE( t1.getSales() == 100 );
    }

    SECTION( "Set sales" )
    {
        t1.setSales(20);

        REQUIRE( t1.getName() == "xyz" );
        REQUIRE( t1.getSales() == 20 );
    }

    SECTION( "Set name, sales" )
    {
        t1.setName("def");
        t1.setSales(30);

        REQUIRE( t1.getName() == "def" );
        REQUIRE( t1.getSales() == 30 );
    }

    SECTION( "Set sales, name" )
    {
        t1.setSales(40);
        t1.setName("ghi");

        REQUIRE( t1.getName() == "ghi" );
        REQUIRE( t1.getSales() == 40 );
    }
}


TEST_CASE( "Copied Product objects",
           "[objs]" )
{
    SECTION( "Copy-constructed Product object" )
    {
        Product t1("xyz", 100);
        Product t2 = t1;

        REQUIRE( t1.getName() == "xyz" );
        REQUIRE( t1.getSales() == 100 );
        REQUIRE( t2.getName() == "xyz" );
        REQUIRE( t2.getSales() == 100 );

        t2.setName("abc");
        t2.setSales(10);

        REQUIRE( t1.getName() == "xyz" );
        REQUIRE( t1.getSales() == 100 );
        REQUIRE( t2.getName() == "abc" );
        REQUIRE( t2.getSales() == 10 );

        t1.setName("def");
        t1.setSales(20);

        REQUIRE( t1.getName() == "def" );
        REQUIRE( t1.getSales() == 20 );
        REQUIRE( t2.getName() == "abc" );
        REQUIRE( t2.getSales() == 10 );
    }

    SECTION( "Copy-assigned Product object" )
    {
        Product t1("xyz", 100);
        Product t2;
        t2 = t1;

        REQUIRE( t1.getName() == "xyz" );
        REQUIRE( t1.getSales() == 100 );
        REQUIRE( t2.getName() == "xyz" );
        REQUIRE( t2.getSales() == 100 );

        t2.setName("abc");
        t2.setSales(10);

        REQUIRE( t1.getName() == "xyz" );
        REQUIRE( t1.getSales() == 100 );
        REQUIRE( t2.getName() == "abc" );
        REQUIRE( t2.getSales() == 10 );

        t1.setName("def");
        t1.setSales(20);

        REQUIRE( t1.getName() == "def" );
        REQUIRE( t1.getSales() == 20 );
        REQUIRE( t2.getName() == "abc" );
        REQUIRE( t2.getSales() == 10 );
    }
}


TEST_CASE( "String conversions",
           "[strings]" )
{
    Product t1;
    Product t2("xyz", 100);

    SECTION( "Product::toString" )
    {
        REQUIRE( t1.toString() == " (sales: 0)" );
        REQUIRE( t2.toString() == "xyz (sales: 100)" );
    }

    SECTION( "operator<<(ostream,Product)" )
    {
        ostringstream ostr1;
        ostr1 << t1;
        REQUIRE( ostr1.str() == " (sales: 0)" );
        ostringstream ostr2;
        ostr2 << t2;
        REQUIRE( ostr2.str() == "xyz (sales: 100)" );
    }
}


TEST_CASE( "Increment & decrement",
           "[operators]" )
{
    Product t1;
    Product t2("xyz", 10);

    SECTION( "Increment ordinary object" )
    {
        Product t3 = (++t2);
        REQUIRE( t2.getName() == "xyz" );
        REQUIRE( t2.getSales() == 11 );
        REQUIRE( t3.getName() == "xyz" );
        REQUIRE( t3.getSales() == 11 );

        Product t4 = (t2++);
        REQUIRE( t2.getName() == "xyz" );
        REQUIRE( t2.getSales() == 12);
        REQUIRE( t4.getName() == "xyz" );
        REQUIRE( t4.getSales() == 11 );

        ++(++t2);
        REQUIRE( t2.getName() == "xyz" );
        REQUIRE( t2.getSales() == 14 );
    }

    SECTION( "Increment object with zero sales" )
    {
        ++t1;
        REQUIRE( t1.getName() == "" );
        REQUIRE( t1.getSales() == 1 );

        t1++;
        REQUIRE( t1.getName() == "" );
        REQUIRE( t1.getSales() == 2 );
    }

    SECTION( "Decrement ordinary object" )
    {
        Product t3 = (--t2);
        REQUIRE( t2.getName() == "xyz" );
        REQUIRE( t2.getSales() == 9 );
        REQUIRE( t3.getName() == "xyz" );
        REQUIRE( t3.getSales() == 9 );

        Product t4 = (t2--);
        REQUIRE( t2.getName() == "xyz" );
        REQUIRE( t2.getSales() == 8 );
        REQUIRE( t4.getName() == "xyz" );
        REQUIRE( t4.getSales() == 9 );

        --(--t2);
        REQUIRE( t2.getName() == "xyz" );
        REQUIRE( t2.getSales() == 6 );
    }

    SECTION( "Decrement object with zero sales" )
    {
        --t1;
        REQUIRE( t1.getName() == "" );
        REQUIRE( t1.getSales() == 0 );

        t1--;
        REQUIRE( t1.getName() == "" );
        REQUIRE( t1.getSales() == 0 );
    }
}


TEST_CASE( "Equality & inequality",
           "[operators]" )
{
    Product t1("abc", 10);  // Base value
    Product t2("abc", 10);  // Same
    Product t3("def", 10);  // Different name
    Product t4("abc", 20);  // Different sales
    Product t5("def", 20);  // Different name & sales

    SECTION( "Equality" )
    {
        REQUIRE( t1 == t1 );
        REQUIRE( t1 == t2 );
        REQUIRE( !(t1 == t3) );
        REQUIRE( !(t1 == t4) );
        REQUIRE( !(t1 == t5) );
    }

    SECTION( "Inequality" )
    {
        REQUIRE( !(t1 != t1) );
        REQUIRE( !(t1 != t2) );
        REQUIRE( t1 != t3 );
        REQUIRE( t1 != t4 );
        REQUIRE( t1 != t5 );
    }
}


TEST_CASE( "Large values",
           "[other]" )
{
    const size_t bigname_len = 1000001;
    string bigname = "";       // We make this into a long string
    for (size_t i = 0; i < bigname_len; ++i)
    {
        bigname.push_back('x');
    }

    int bigsales = 2147483641;  // Almost greatest 32-bit signed value

    ostringstream ostr;
    ostr << bigname << " (sales: " << bigsales << ")";
    string bigstringform = ostr.str();
                            // String form of Product(bigname, bigsales)

    SECTION( "Construction from large values" )
    {
        Product t1(bigname, bigsales);

        REQUIRE( t1.getName() == bigname );
        REQUIRE( t1.getSales() == bigsales );

        REQUIRE( t1.toString() == bigstringform );
    }

    SECTION( "Set functions, passing large values" )
    {
        Product t1;
        t1.setName(bigname);
        t1.setSales(bigsales);

        REQUIRE( t1.getName() == bigname );
        REQUIRE( t1.getSales() == bigsales );

        REQUIRE( t1.toString() == bigstringform );
    }
}


TEST_CASE( "Same-object tests",
           "[other]" )
{
    SECTION("Copy assignment returns *this")
    {
        Product t1;
        Product t2;
        Product & r(t1 = t2);
        REQUIRE( &r == &t1 );
    }

    SECTION("Move assignment returns *this")
    {
        Product t1;
        Product t_movable;
        Product & r(t1 = move(t_movable));
        REQUIRE( &r == &t1 );
    }

    SECTION("pre-increment returns *this")
    {
        Product t1;
        Product & r(++t1);
        REQUIRE( &r == &t1 );
    }

    SECTION("pre-decrement returns *this")
    {
        Product t1;
        Product & r(--t1);
        REQUIRE( &r == &t1 );
    }

    SECTION("post-increment returns copy")
    {
        Product t1;
        Product && rr(t1++);
        REQUIRE( &rr != &t1 );
    }

    SECTION("post-decrement returns copy")
    {
        Product t1;
        Product && rr(t1--);
        REQUIRE( &rr != &t1 );
    }

    SECTION("getName returns copy")
    {
        Product t1;
        string && rr1(t1.getName());
        string && rr2(t1.getName());
        REQUIRE( &rr1 != &rr2 );
    }

    SECTION("getSales returns copy")
    {
        Product t1;
        int && rr1(t1.getSales());
        int && rr2(t1.getSales());
        REQUIRE( &rr1 != &rr2 );
    }

    SECTION("operator<< returns original stream")
    {
        ostringstream ostr;
        Product t1;
        ostream & r(ostr << t1);
        REQUIRE( &r == &ostr );
    }
}

