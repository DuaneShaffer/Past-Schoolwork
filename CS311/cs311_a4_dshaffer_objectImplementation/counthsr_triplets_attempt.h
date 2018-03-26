// counthsr.h
// Duane Shaffer
// October 7, 2017
// CS311
// 
// This is the header file for assignment 4, "Holey Spider Run"
// demonstrating recursive backtracking.
// This file follows some of the formatting used by Glen Chappell's example code.

#ifndef COUNTHSR_H_INCLUDED
#define COUNTHSR_H_INCLUDED

int countHSR(int dim_x, int dim_y,
			int hole_x, int hole_y,
			int start_x, int start_y,
			int finish_x, int finish_y);

// *********************************************************************
// class HSR_Board_Class - Class definition
// *********************************************************************

// class HSR_Board_Class
// RAII Holey spider run class. Allocates an array for the HSR board and calculates 
// the number of valid paths from start (x,y) to finish (x,y) avoiding the hole(x,y)
// Invariants:
//		_boardArray points to a dynamically allocated array
//		All stored integer values are non-negative
//		0 <= int _numSquaresLeft <= (_boardDimX*_boardDimY -3)
// throws when array allocation throws
class HSR_Board_Class {
public:

	// ctor using necessary data. Assumes that all data passed to it is valid
	// Pre: all values passed are non-negative integers
	HSR_Board_Class(const int boardDimX, const int boardDimY,
					const int hole_x, const int hole_y,
					const int start_x, const int start_y,
					const int finish_x, const int finish_y);
	// dctor
	~HSR_Board_Class() {
		delete [] _boardArray;
	}
	
	// getTotal
	// returns the running total that is calculated by the class using HSR_Board_Class.calculateRuns()
	int getTotal() {
		return _runningTotal;
	}

	// calculateRuns
	// checks if _isValidBoard is true and if so, calls _countHSR_recurse()
	void calculateRuns() {
		if (_isValidBoard)
			_startSearch();
	}
	// Don't allow copy/move ctor, copy/move op=, default ctor
	HSR_Board_Class() = delete;
	HSR_Board_Class(const HSR_Board_Class & other) = delete;
	HSR_Board_Class(HSR_Board_Class && other) = delete;
	HSR_Board_Class & operator=(const HSR_Board_Class & other) = delete;
	HSR_Board_Class & operator=(HSR_Board_Class && other) = delete;

private:
	int* _boardArray;
	const int _boardDimX;
	const int _boardDimY;
	int _current_x;
	int _current_y;
	int _runningTotal;
	int _numSquaresLeft;
	bool _isValidBoard;

// _validateBoard
// private member function that calculates whether or not the given board even has a solution
	bool _validateBoard(const int hole_x, const int hole_y, const int finish_x, const int finish_y);

// _countHSR_recurse
// private member function that does the recursive backtracking
	void _countHSR_recurse();

// _startSearch
// private member function that checks if there contiguous open squares around the start consisting of 2 sides and a corner
// if so, shortcuts can be made in the recursion. This function handles all of the initial moves from the start square.
// The shortcut consists of a triplet of moves that have the same board configuration and same current position. Once search can
// can be made from that current position and just multiplied by 2

// Here is some pseudo code to help understand the method employed. 
/**************************************************************************************************
* if up, right, and up-right are open ***upper right triplet***
*		do shortcut

* if right, down, and right-down are open *** lower right triplet ***
*		do shortcut

* if down, left, and left-down are open *** lower left triplet ***
*		do shortcut

* if left, up, and left-up are open *** upper left triplet ***
*		do shortcut

* Manually pathfind based off 1 move and make sure to not duplicate moves already tracked
* using triplets.
******************************************************************************************************/

	void _startSearch();

}; // end class HSR_Board_Class


#endif // COUNTHSR_H_INCLUDED