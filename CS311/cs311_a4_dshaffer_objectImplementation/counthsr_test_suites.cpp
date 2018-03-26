// counthsr_test_suites.cpp
// Glenn G. Chappell
// 6 Oct 2017
//
// For CS 311 Fall 2017
// Test program for function countHSR
// Used in Assignment 3, Exercise A
// Requires counthsr_test_main.cpp, catch.hpp, counthsr.h, counthsr.cpp

// Includes for code to be tested
#include "counthsr.h"      // For function countHSR
#include "counthsr.h"      // Double inclusion test

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


// (None)


// *********************************************************************
// Test Cases
// *********************************************************************


TEST_CASE( "n x 1 and 1 x n boards",
           "[values]" )
{
    SECTION( "All 3 x 1 boards" )
    {
        REQUIRE( countHSR(3,1, 0,0, 1,0, 2,0) == 1 );
        REQUIRE( countHSR(3,1, 0,0, 2,0, 1,0) == 1 );
        REQUIRE( countHSR(3,1, 1,0, 0,0, 2,0) == 0 );
        REQUIRE( countHSR(3,1, 1,0, 2,0, 0,0) == 0 );
        REQUIRE( countHSR(3,1, 2,0, 0,0, 1,0) == 1 );
        REQUIRE( countHSR(3,1, 2,0, 1,0, 0,0) == 1 );
    }

    SECTION( "All 1 x 3 boards" )
    {
        REQUIRE( countHSR(1,3, 0,0, 0,1, 0,2) == 1 );
        REQUIRE( countHSR(1,3, 0,0, 0,2, 0,1) == 1 );
        REQUIRE( countHSR(1,3, 0,1, 0,0, 0,2) == 0 );
        REQUIRE( countHSR(1,3, 0,1, 0,2, 0,0) == 0 );
        REQUIRE( countHSR(1,3, 0,2, 0,0, 0,1) == 1 );
        REQUIRE( countHSR(1,3, 0,2, 0,1, 0,0) == 1 );
    }

    SECTION( "All 4 x 1 boards" )
    {
        REQUIRE( countHSR(4,1, 0,0, 1,0, 2,0) == 0 );
        REQUIRE( countHSR(4,1, 0,0, 1,0, 3,0) == 1 );
        REQUIRE( countHSR(4,1, 0,0, 2,0, 1,0) == 0 );
        REQUIRE( countHSR(4,1, 0,0, 2,0, 3,0) == 0 );
        REQUIRE( countHSR(4,1, 0,0, 3,0, 1,0) == 1 );
        REQUIRE( countHSR(4,1, 0,0, 3,0, 2,0) == 0 );
        REQUIRE( countHSR(4,1, 1,0, 0,0, 2,0) == 0 );
        REQUIRE( countHSR(4,1, 1,0, 0,0, 3,0) == 0 );
        REQUIRE( countHSR(4,1, 1,0, 2,0, 0,0) == 0 );
        REQUIRE( countHSR(4,1, 1,0, 2,0, 3,0) == 0 );
        REQUIRE( countHSR(4,1, 1,0, 3,0, 0,0) == 0 );
        REQUIRE( countHSR(4,1, 1,0, 3,0, 2,0) == 0 );
        REQUIRE( countHSR(4,1, 2,0, 0,0, 1,0) == 0 );
        REQUIRE( countHSR(4,1, 2,0, 0,0, 3,0) == 0 );
        REQUIRE( countHSR(4,1, 2,0, 1,0, 0,0) == 0 );
        REQUIRE( countHSR(4,1, 2,0, 1,0, 3,0) == 0 );
        REQUIRE( countHSR(4,1, 2,0, 3,0, 0,0) == 0 );
        REQUIRE( countHSR(4,1, 2,0, 3,0, 1,0) == 0 );
        REQUIRE( countHSR(4,1, 3,0, 0,0, 1,0) == 0 );
        REQUIRE( countHSR(4,1, 3,0, 0,0, 2,0) == 1 );
        REQUIRE( countHSR(4,1, 3,0, 1,0, 0,0) == 0 );
        REQUIRE( countHSR(4,1, 3,0, 1,0, 2,0) == 0 );
        REQUIRE( countHSR(4,1, 3,0, 2,0, 0,0) == 1 );
        REQUIRE( countHSR(4,1, 3,0, 2,0, 1,0) == 0 );
    }

    SECTION( "All 1 x 4 boards" )
    {
        REQUIRE( countHSR(1,4, 0,0, 0,1, 0,2) == 0 );
        REQUIRE( countHSR(1,4, 0,0, 0,1, 0,3) == 1 );
        REQUIRE( countHSR(1,4, 0,0, 0,2, 0,1) == 0 );
        REQUIRE( countHSR(1,4, 0,0, 0,2, 0,3) == 0 );
        REQUIRE( countHSR(1,4, 0,0, 0,3, 0,1) == 1 );
        REQUIRE( countHSR(1,4, 0,0, 0,3, 0,2) == 0 );
        REQUIRE( countHSR(1,4, 0,1, 0,0, 0,2) == 0 );
        REQUIRE( countHSR(1,4, 0,1, 0,0, 0,3) == 0 );
        REQUIRE( countHSR(1,4, 0,1, 0,2, 0,0) == 0 );
        REQUIRE( countHSR(1,4, 0,1, 0,2, 0,3) == 0 );
        REQUIRE( countHSR(1,4, 0,1, 0,3, 0,0) == 0 );
        REQUIRE( countHSR(1,4, 0,1, 0,3, 0,2) == 0 );
        REQUIRE( countHSR(1,4, 0,2, 0,0, 0,1) == 0 );
        REQUIRE( countHSR(1,4, 0,2, 0,0, 0,3) == 0 );
        REQUIRE( countHSR(1,4, 0,2, 0,1, 0,0) == 0 );
        REQUIRE( countHSR(1,4, 0,2, 0,1, 0,3) == 0 );
        REQUIRE( countHSR(1,4, 0,2, 0,3, 0,0) == 0 );
        REQUIRE( countHSR(1,4, 0,2, 0,3, 0,1) == 0 );
        REQUIRE( countHSR(1,4, 0,3, 0,0, 0,1) == 0 );
        REQUIRE( countHSR(1,4, 0,3, 0,0, 0,2) == 1 );
        REQUIRE( countHSR(1,4, 0,3, 0,1, 0,0) == 0 );
        REQUIRE( countHSR(1,4, 0,3, 0,1, 0,2) == 0 );
        REQUIRE( countHSR(1,4, 0,3, 0,2, 0,0) == 1 );
        REQUIRE( countHSR(1,4, 0,3, 0,2, 0,1) == 0 );
    }

    SECTION( "Various n x 1 boards with n > 4" )
    {
        REQUIRE( countHSR(  5,1,   0,0,   1,0,   4,0) == 1 );
        REQUIRE( countHSR(  5,1,   2,0,   0,0,   4,0) == 0 );
        REQUIRE( countHSR(  7,1,   2,0,   0,0,   4,0) == 0 );
        REQUIRE( countHSR(  8,1,   7,0,   6,0,   0,0) == 1 );
        REQUIRE( countHSR( 20,1,   0,0,  19,0,   1,0) == 1 );
        REQUIRE( countHSR( 25,1,  24,0,   0,0,  22,0) == 0 );
        REQUIRE( countHSR(299,1, 298,0,   0,0, 297,0) == 1 );
        REQUIRE( countHSR(299,1, 297,0, 298,0,   0,0) == 0 );
    }

    SECTION( "Various 1 x n boards with n > 4" )
    {
        REQUIRE( countHSR(1,  5, 0,  4, 0,  3, 0,0) == 1 );
        REQUIRE( countHSR(1,  5, 0,  0, 0,  1, 0,4) == 1 );
        REQUIRE( countHSR(1,  6, 0,  1, 0,  5, 0,0) == 0 );
        REQUIRE( countHSR(1,  6, 0,  5, 0,  0, 0,4) == 1 );
        REQUIRE( countHSR(1, 33, 0, 15, 0,  0, 0,1) == 0 );
        REQUIRE( countHSR(1, 50, 0,  0, 0, 49, 0,1) == 1 );
        REQUIRE( countHSR(1,298, 0,296, 0,297, 0,0) == 0 );
        REQUIRE( countHSR(1,299, 0,  0, 0,298, 0,1) == 1 );
    }
}


TEST_CASE( "Small boards",
           "[values]" )
{
    SECTION( "Various small boards" )
    {
        REQUIRE( countHSR(2,2, 0,0, 1,0, 1,1) ==   1 );
        REQUIRE( countHSR(2,2, 1,0, 1,1, 0,0) ==   1 );
        REQUIRE( countHSR(8,2, 1,0, 1,1, 0,0) ==   0 );
        REQUIRE( countHSR(8,2, 1,0, 0,0, 5,0) ==  96 );
        REQUIRE( countHSR(2,8, 0,1, 1,1, 0,0) ==   0 );
        REQUIRE( countHSR(2,8, 0,1, 0,0, 1,4) ==  64 );
        REQUIRE( countHSR(3,3, 0,0, 0,2, 1,1) ==   6 );
        REQUIRE( countHSR(4,4, 0,2, 2,1, 3,3) == 181 );
        REQUIRE( countHSR(3,5, 0,0, 1,1, 2,4) == 355 );
    }
}


TEST_CASE( "Large boards",
           "[values]" )
{
    SECTION( "Various large boards" )
    {
        REQUIRE( countHSR(3, 6, 0,0, 1,0, 1,1) ==  945 );
        REQUIRE( countHSR(4, 5, 3,1, 2,0, 0,0) == 5076 );
        REQUIRE( countHSR(5, 4, 2,2, 0,0, 1,3) == 5488 );
        REQUIRE( countHSR(2,10, 0,1, 0,0, 1,0) ==    0 );
        REQUIRE( countHSR(6, 3, 5,2, 4,1, 1,2) == 1047 );
        REQUIRE( countHSR(9, 2, 0,1, 4,0, 1,0) ==  128 );
    }
}


TEST_CASE( "Slide Examples",
           "[values]" )
{
    SECTION( "Examples from \"Thoughts on Assignment 4\" slides" )
    {
        REQUIRE( countHSR(4,2, 1,1, 0,1, 3,0) ==     4 );
        REQUIRE( countHSR(3,2, 2,1, 1,1, 2,0) ==     2 );
        REQUIRE( countHSR(4,1, 1,0, 0,0, 3,0) ==     0 );
        REQUIRE( countHSR(5,4, 4,0, 4,1, 0,3) == 40887 );
    }
}

