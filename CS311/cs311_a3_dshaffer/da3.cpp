// da3.cpp 
// Duane Shaffer
// Based off the skeleton file provided by Glenn G. Chappell
// 29 Sep 2017
//
// For CS 311 Fall 2017
// Source file for Assignment 3 Functions

#include "da3.h"       // For Assignment 3 prototypes & templates
#include <functional>
using std::function;

// function callBetween 
// See header for documentation.
void callBetween(const function<void()> & start,
                 const function<void()> & middle,
                 const function<void()> & finish)
{
		start();
	// If middle throws we need to clean up before giving the client the error.
	try {
		middle ();
	}
	catch (...)
	{
		finish ();
		throw;
	}
	finish();
}

// function gcd
// See header for documentation.
int gcd(int a,
        int b)
{
	// Base Case
	if (a == 0)
		return b;
	// Reorder the inputs for computation
	if (a > b)
		gcd(b, a);
	// Recursive call
	gcd((b%a), a);
}

