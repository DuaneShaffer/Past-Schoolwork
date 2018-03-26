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
#include <vector> //for std::vector and std::swap

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
	// Copy ctor
	// Create a deep copy of given HSR_Board_Class
	HSR_Board_Class(const HSR_Board_Class &other);

	// dctor
	~HSR_Board_Class() {
		delete [] _boardArray;
	}

	// move ctor
	// Pre: other must be destructible
	// Post: values from other have been moved to current KSArray
	HSR_Board_Class(HSR_Board_Class && other) noexcept : _boardDimX(other._boardDimX), _boardDimY(other._boardDimY),
														_current_x(other._current_x), _current_y(other._current_y),
														_runningTotal(other._runningTotal), _runMultiplier(other._runMultiplier),
														_boardArray(other._boardArray), _numSquaresLeft (other._numSquaresLeft),
														_isValidBoard(other._isValidBoard) {
		other._boardArray = nullptr;
	}

	// move op= 
	// Pre: None
	// Post: Values from given HSR_Board_Class have been moved to current HSR_Board_Class
	HSR_Board_Class & operator= (HSR_Board_Class && rhs) noexcept {
		if (this == &rhs) // Check for self-assignment
			return *this;
		_HSR_Swap(rhs);
		return *this;
	}
	
	// getTotal
	// returns the running total that is calculated by the class using HSR_Board_Class.calculateRuns()
	int getTotal() {
		return _runningTotal * _runMultiplier;
	}

	// calculateRuns
	// checks if _isValidBoard is true and if so, calls _countHSR_recurse()
	void calculateRuns();

	// getTotalSquares
	// returns number of squares in the gameboard
	int getTotalSquares() const {
		int totalSquares = _boardDimX * _boardDimY;
		return totalSquares;
	}

	// getSquareVal
	// returns the value held in the x,y coordinates that are passed to the function
	int getSquareVal(int x, int y) const {
		return _boardArray[x + y*_boardDimX];
	}

	// getSquareVal
	// returns the value held in the array element passed to the function
	int getSquareVal(int element) const {
		return _boardArray[element];
	}

	// getCurrentPosition
	// returns the array element corresponding to the xy coordinate held by _current_x and _current_y;
	int getCurrentPosition () const {
		return _current_x + _current_y * _boardDimX;
	}

	// incrementRunMultiplier
	// increments the run multpiplier by the given int
	void incrementRunMult(int num) {
		_runMultiplier += num;
	}

	// Don't allow default ctor, copy op=
	HSR_Board_Class() = delete;
	HSR_Board_Class & operator=(const HSR_Board_Class & other) = delete;

private:
	int* _boardArray;
	const int _boardDimX;
	const int _boardDimY;
	int _current_x;
	int _current_y;
	int _runningTotal;
	int _numSquaresLeft;
	bool _isValidBoard;
	int _runMultiplier;
	static std::vector<HSR_Board_Class *> _allBoards;
	static int _numSolutions_allBoards;

// _validateBoard
// private member function that calculates whether or not the given board even has a solution
	bool _validateBoard(const int hole_x, const int hole_y, const int finish_x, const int finish_y);

// _countHSR_recurse
// private member function that does the recursive backtracking
	void _countHSR_recurse();

// _countHSR_recurse_withDepth
// private member function that does a set number of recursions
	void _countHSR_recurse_withDepth(int);

// _HSR_Swap
// private member function that swaps 2 HSR objects
	void _HSR_Swap(HSR_Board_Class &other) noexcept;

// _reduce
// private member function that sorts the global array and then uses a modified version of std::unique to eliminate repeated paths
// When duplicates are found, the first one (the one that isn't eliminated) has its runMultiplier incremented.
	void _reduce();

}; // end class HSR_Board_Class


#endif // COUNTHSR_H_INCLUDED