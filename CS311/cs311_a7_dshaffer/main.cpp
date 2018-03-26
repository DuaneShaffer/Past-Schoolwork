// treesort_test_main.cpp
// Glenn G. Chappell
// 15 Nov 2017
//
// For CS 311 Fall 2017
// Tests for function template treesort: main program
// For Assignment 7, Exercise A
// Uses the "Catch" unit-testing framework
// Requires treesort_test_suites.cpp, catch.hpp, treesort.h

#define CATCH_CONFIG_FAST_COMPILE
                             // Disable some features for faster compile
#define CATCH_CONFIG_RUNNER  // We write our own main
#include "catch.hpp"         // For the "Catch" unit-testing framework
#include <iostream>
using std::cout;
using std::endl;
using std::cin;
#include <string>
using std::string;


// Main program
// Run all tests. Prompt for ENTER before exiting.
int main(int argc, char *argv[])
{
    Catch::Session session;  // Primary Catch object
    int catchresult;         // Catch return code; for return by main

    // Handle command line
    catchresult = session.applyCommandLine(argc, argv);

    if (!catchresult)  // Continue only if no command-line error
    {
        // Message strings
        string msg1 = "tests for function template treesort";
        string msg2 = "CS 311 Assn 7, Ex A";

        // Run test suites
        cout << "BEGIN " << msg1 << " - " << msg2 << endl;
        cout << endl;
        catchresult = session.run();
        cout << "END " << msg1 << " - " << msg2 << endl;
        cout << endl;
    }

    // Wait for user
    std::cout << "Press ENTER to quit ";
    while (std::cin.get() != '\n') ;

    // Program return value is return code from Catch
    return catchresult;
}

