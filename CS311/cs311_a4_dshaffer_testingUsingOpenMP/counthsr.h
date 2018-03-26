// counthsr.h
// Duane Shaffer
// December 8, 2017
// CS301
// Project 2
// 
// This is the adapted source file of assignment 4, "Holey Spider Run" in CS311,
// demonstrating recursive backtracking by finding all paths possible on a rectangular board
// given a start, finish, and a hole. The path must not backtrack on itself and cannot go 
// over a hole. 
// The original file follows some of the formatting used by Glen Chappell's example code.



#ifndef COUNTHSR_H_INCLUDED
#define COUNTHSR_H_INCLUDED

#include <iostream>
#include <omp.h>

// Short readme regarding this program:
/* This program uses code from a 311 assignment and attempts to improve its speed by utilizing openMP. Most of the new stuff 
is towards the bottom and functions are appropriately named (ex: countHSR_openMP). It then takes a look at timing of the program
to determine the effectiveness of openMP. */



// Conclusion of the project:
/* The openMP version of my algorithm (exhaustively searching for all paths) averages a 50% speedup +- 10% if the work is 
entirely worst case or best case (note, these averages throw out the small boards that have a trivial amount of work and
hence shouldn't be executed using threads anyways).

Although taking half the time is nice, when searching a 30 square board still ends up taking 10 minutes, the correct answer
here is not to make the code parallel and instead focus efforts on finding a better algorithm.

openMP is nice because it handles the thread generation and scheduling for you. Implementing it can seem trivial but the 
real trick comes when having to alter code to work with it. In particular, I ran into issues that were time consuming when
working with code that I had written and as such I was very familiar with how it worked. Therefore the time I took was much
less than if somebody else were to attempt to do the same project with my code. If I were to be changing code that was written 
by somebody else, this project could easily become a bear. 

Some issues that I wouldn't run into again:
how to set up structured blocks
issues when trying to privatize object data members
		- note, the issue of having to take object oriented code and use openMP with it would still exist
getting visual studio to work with openMP

One issue that cropped up (but luckily was quickly squashed) was, after changing the algorithm to work with the sections
directive, the function incorrectly calculated boards that were 1x3. The fact that taking somebody's code and making it 
parallel can make it compute incorrect answers is something that I need to be mindful of. Ideally, they will have a 
thorough testing program like I had in order to discover errors and help correct them.
*/



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
		const int finish_x, const int finish_y) : _boardDimX(boardDimX), _boardDimY(boardDimY),
		_current_x(start_x), _current_y(start_y),
		_runningTotal(0) {
		_boardArray = new int[boardDimX * boardDimY];
		_numSquaresLeft = boardDimX * boardDimY - 3;
		_isValidBoard = _validateBoard(hole_x, hole_y, finish_x, finish_y);
		// Fill the board with a partial solution
		for (int index = 0; index < (boardDimX*boardDimY); ++index) {
			_boardArray[index] = 0;
		}
		// Place hole
		_boardArray[hole_x + hole_y*boardDimX] = 1;
		// Place finish
		_boardArray[finish_x + finish_y*boardDimX] = 2;
	} // End of HSR_Board_Class ctr

	  // dctor
	~HSR_Board_Class() {
		delete[] _boardArray;
	}

	// getTotal
	// returns the running total that is calculated by the class using HSR_Board_Class.calculateRuns()
	double getTotal() {
		return _runningTotal;
	}

	// calculateRuns
	// checks if _isValidBoard is true and if so, calls _countHSR_recurse()
	void calculateRuns() {
		if (_isValidBoard)
			_countHSR_recurse();
	}
	// Don't allow copy/move ctor, copy/move op=, default ctor
	HSR_Board_Class() = delete;
	HSR_Board_Class(const HSR_Board_Class & other) = delete;
	HSR_Board_Class(HSR_Board_Class && other) = delete;
	HSR_Board_Class & operator=(const HSR_Board_Class & other) = delete;
	HSR_Board_Class & operator=(HSR_Board_Class && other) = delete;


	//*******************************************
	// check/move functions necessary for openMP
	//*******************************************

	bool _moveRight() {
		// check right
		// if(location to the right is valid move)
		if (((_current_x + 1) != _boardDimX) && !_boardArray[_current_x + 1 + _current_y *_boardDimX]) {
			_boardArray[_current_x + _current_y * _boardDimX] = 1;								// Current location no longer valid
			++_current_x;																		// move
			--_numSquaresLeft;
			return true;
		}
		return false;
	}
	bool _moveRightDown() {
		// check right-down
		// if (can move down && right/down is valid move)
		if (((_current_x + 1) != _boardDimX) && (_current_y != 0) && !_boardArray[_current_x + 1 + (_current_y - 1) *_boardDimX]) {
			_boardArray[_current_x + _current_y * _boardDimX] = 1;									// Current location no longer valid
			++_current_x;																	// move
			--_current_y;
			--_numSquaresLeft;
			return true;
		}
		return false;
	}
	bool _moveRightUp() {
		// check right-up
		// if (can move up && right-up is a valid move)
		if ((((_current_x + 1) != _boardDimX) && (_current_y + 1) != _boardDimY) && !_boardArray[(_current_x + 1) + (_current_y + 1) *_boardDimX]) {
			_boardArray[_current_x + _current_y * _boardDimX] = 1;									// Current location no longer valid
			++_current_y;																// move
			++_current_x;
			--_numSquaresLeft;
			return true;
		}
		return false;
	}
	bool _moveLeft() {
		// check left
		// if (left is a valid move)
		if ((_current_x) && !_boardArray[_current_x - 1 + _current_y *_boardDimX]) {
			_boardArray[_current_x + _current_y * _boardDimX] = 1;									// Current location no longer valid
			--_current_x;																	// move
			--_numSquaresLeft;
			return true;
		}
		return false;
	}
	bool _moveLeftDown() {
		// check left-down
		// if (can move down && left-down is valid move)
		if ((_current_x) && _current_y && !_boardArray[(_current_x - 1) + (_current_y - 1) *_boardDimX]) {
			_boardArray[_current_x + _current_y *_boardDimX] = 1;									// Current location no longer valid
			--_current_y;																	// move
			--_current_x;
			--_numSquaresLeft;
			return true;
		}
		return false;
	}
	bool _moveLeftUp() {
		// check left-up
		// if (can move up && left-up is a valid move)
		if (((_current_x) && (_current_y + 1) != _boardDimY) && !_boardArray[_current_x - 1 + (_current_y + 1) *_boardDimX]) {
			_boardArray[_current_x + _current_y *_boardDimX] = 1;									// Current location no longer valid
			--_current_x;																	// move
			++_current_y;
			--_numSquaresLeft;
			return true;
		}
		return false;
	}
	bool _moveUp() {
		// check up
		// if (can move up && up is a valid move)
		if (((_current_y + 1) != _boardDimY) && !_boardArray[_current_x + (_current_y + 1) *_boardDimX]) {
			_boardArray[_current_x + _current_y * _boardDimX] = 1;									// Current location no longer valid
			++_current_y;																// move
			--_numSquaresLeft;
			return true;
		}
		return false;
	}
	bool _moveDown() {
		// check down
		// if (can move down && down is valid move)
		if (_current_y && !_boardArray[_current_x + (_current_y - 1) *_boardDimX]) {
			_boardArray[_current_x + _current_y * _boardDimX] = 1;									// Current location no longer valid
			--_current_y;																	// move
			--_numSquaresLeft;
			return true;
		}
		return false;
	}

private:
	int* _boardArray;
	const int _boardDimX;
	const int _boardDimY;
	int _current_x;
	int _current_y;
	double _runningTotal;
	int _numSquaresLeft;
	bool _isValidBoard;

	// *********************************************************************
	// class HSR_Board_Class - Definitions of member functions
	// *********************************************************************

	// _validateBoard
	// private member function that calculates whether or not the given board even has a solution
	bool _validateBoard(const int hole_x, const int hole_y, const int finish_x, const int finish_y) {
		if (_boardDimX == 1) {
			if ((_current_y < hole_y) && (hole_y < finish_y))
				return false;
			else if ((hole_y < _current_y) && (finish_y < hole_y))
				return false;
			else if ((hole_y == 0) && ((_current_y - 1) != hole_y) && ((finish_y - 1) != hole_y))
				return false;
			else if ((hole_y == 0) && (_current_y != _boardDimY - 1) && (finish_y != _boardDimY - 1))
				return false;
			else if ((hole_y == _boardDimY - 1) && ((finish_y + 1) != hole_y) && ((_current_y + 1) != hole_y))
				return false;
			else if ((hole_y == _boardDimY - 1) && (finish_y != 0) && (_current_y != 0))
				return false;
			else
				return true;
		}
		if (_boardDimY == 1) {
			if ((_current_x < hole_x) && (hole_x < finish_x))
				return false;
			else if ((hole_x < _current_x) && (finish_x < hole_x))
				return false;
			else if ((hole_x == 0) && ((_current_x) != 1) && ((finish_x) != 1))
				return false;
			else if ((hole_x == 0) && (_current_x != _boardDimX - 1) && (finish_x != _boardDimX - 1))
				return false;
			else if ((hole_x == _boardDimX - 1) && ((finish_x + 1) != hole_x) && ((_current_x + 1) != hole_x))
				return false;
			else if ((hole_x == _boardDimX - 1) && (finish_x != 0) && (_current_x != 0))
				return false;
			else
				return true;
		}
		return true;
	} // end private member function _validateBoard

	  // _countHSR_recurse
	  // private member function that does the recursive backtracking
	void _countHSR_recurse() {
		// Right side checks
		if ((_current_x + 1) != _boardDimX) {
			// check right
			// if(location to the right is valid move)
			if (!_boardArray[_current_x + 1 + _current_y *_boardDimX]) {
				_boardArray[_current_x + _current_y * _boardDimX] = 1;								// Current location no longer valid
				++_current_x;																		// move
				--_numSquaresLeft;
				_countHSR_recurse();																// Recursive Call
				_boardArray[_current_x + _current_y * _boardDimX] = 0;								// undo validity
				++_numSquaresLeft; 																	// unmove
				--_current_x;
			}
			// check right-down
			// if (can move down && right/down is valid move)
			if ((_current_y != 0) && !_boardArray[_current_x + 1 + (_current_y - 1) *_boardDimX]) {
				_boardArray[_current_x + _current_y * _boardDimX] = 1;									// Current location no longer valid
				++_current_x;																	// move
				--_current_y;
				--_numSquaresLeft;
				_countHSR_recurse();																// Recursive Call
				_boardArray[_current_x + _current_y * _boardDimX] = 0;									// undo validity
				--_current_x;																	// unmove
				++_current_y;
				++_numSquaresLeft;
			}
			// check right-up
			// if (can move up && right-up is a valid move)
			if (((_current_y + 1) != _boardDimY) && !_boardArray[(_current_x + 1) + (_current_y + 1) *_boardDimX]) {
				_boardArray[_current_x + _current_y * _boardDimX] = 1;									// Current location no longer valid
				++_current_y;																// move
				++_current_x;
				--_numSquaresLeft;
				_countHSR_recurse();	// Recursive Call
				_boardArray[_current_x + _current_y * _boardDimX] = 0;									// undo validity
				--_current_y;																// unmove
				--_current_x;
				++_numSquaresLeft;
			}
		}
		// Left side checks
		if (_current_x) { // If we can move left
						  // check left
						  // if (left is a valid move)
			if (!_boardArray[_current_x - 1 + _current_y *_boardDimX]) {
				_boardArray[_current_x + _current_y * _boardDimX] = 1;									// Current location no longer valid
				--_current_x;																	// move
				--_numSquaresLeft;
				_countHSR_recurse();	// Recursive Call
				_boardArray[_current_x + _current_y *_boardDimX] = 0;									// Undo validity
				++_current_x;																	// unmove
				++_numSquaresLeft;
			}
			// check left-down
			// if (can move down && left-down is valid move)
			if (_current_y && !_boardArray[(_current_x - 1) + (_current_y - 1) *_boardDimX]) {
				_boardArray[_current_x + _current_y *_boardDimX] = 1;									// Current location no longer valid
				--_current_y;																	// move
				--_current_x;
				--_numSquaresLeft;
				_countHSR_recurse();	// Recursive Call
				_boardArray[_current_x + _current_y * _boardDimX] = 0;									// undo validity
				++_current_y;																	// unmove
				++_current_x;
				++_numSquaresLeft;
			}
			// check left-up
			// if (can move up && left-up is a valid move)
			if (((_current_y + 1) != _boardDimY) && !_boardArray[_current_x - 1 + (_current_y + 1) *_boardDimX]) {
				_boardArray[_current_x + _current_y *_boardDimX] = 1;									// Current location no longer valid
				--_current_x;																	// move
				++_current_y;
				--_numSquaresLeft;
				_countHSR_recurse();	// Recursive Call
				_boardArray[_current_x + _current_y * _boardDimX] = 0;									// Undo validity
				++_current_x;																	// unmove
				--_current_y;
				++_numSquaresLeft;
			}
		}
		// check up
		// if (can move up && up is a valid move)
		if (((_current_y + 1) != _boardDimY) && !_boardArray[_current_x + (_current_y + 1) *_boardDimX]) {
			_boardArray[_current_x + _current_y * _boardDimX] = 1;									// Current location no longer valid
			++_current_y;																// move
			--_numSquaresLeft;
			_countHSR_recurse();	// Recursive Call
			_boardArray[_current_x + _current_y * _boardDimX] = 0;									// undo validity
			--_current_y;																	// unmove
			++_numSquaresLeft;
		}

		// check down
		// if (can move down && down is valid move)
		if (_current_y && !_boardArray[_current_x + (_current_y - 1) *_boardDimX]) {
			_boardArray[_current_x + _current_y * _boardDimX] = 1;									// Current location no longer valid
			--_current_y;																	// move
			--_numSquaresLeft;
			_countHSR_recurse();	// Recursive Call
			_boardArray[_current_x + _current_y *_boardDimX] = 0;									// undo validity
			++_current_y;																	// unmove
			++_numSquaresLeft;
		}

		// Check to see if we have a valid solution
		// By this point of the function call, there are no more valid moves to make.
		// If we've been to every square then we might have a valid solution.

		// if (number of squares left = 0)
		if (!_numSquaresLeft) {
			//right side checks
			if (_current_x + 1 != _boardDimX) {
				if (_boardArray[(_current_x + 1) + _current_y*_boardDimX] == 2) // right
					++_runningTotal;
				else if (_current_y && (_boardArray[(_current_x + 1) + (_current_y - 1) *_boardDimX] == 2)) // right-down
					++_runningTotal;
				else if (((_current_y + 1) != _boardDimY) && _boardArray[(_current_x + 1) + (_current_y + 1) *_boardDimX] == 2) // up-right
					++_runningTotal;
			}

			// left side checks
			if (_current_x) {
				if (_boardArray[(_current_x - 1) + _current_y*_boardDimX] == 2) // left
					++_runningTotal;
				else if (_current_y && _boardArray[(_current_x - 1) + (_current_y - 1) *_boardDimX] == 2) // down-left
					++_runningTotal;
				else if (((_current_y + 1) != _boardDimY) && _boardArray[(_current_x - 1) + (_current_y + 1) *_boardDimX] == 2) //left-up
					++_runningTotal;
			}
			// check up
			if (((_current_y + 1) != _boardDimY) && _boardArray[_current_x + (_current_y + 1)*_boardDimX] == 2) // up
				++_runningTotal;
			// check down
			if (_current_y && _boardArray[_current_x + (_current_y - 1)*_boardDimX] == 2) // down
				_runningTotal++;
		}
	} // end private member function _countHSR_recurse

}; // end class HSR_Board_Class



   //***********************************************************************************
   // int _countHSR_openMP
   // wrapper function that does the recursive backtracking utilizing openMP
   //***********************************************************************************
double countHSR(int dim_x, int dim_y,
	int hole_x, int hole_y,
	int start_x, int start_y,
	int finish_x, int finish_y) {
	volatile double total = 0;
#pragma omp parallel sections reduction (+ : total) 
	{
 #pragma omp section	
		{
			// check right
			HSR_Board_Class boardArrayR(dim_x, dim_y, hole_x, hole_y, start_x, start_y, finish_x, finish_y);
			if (boardArrayR._moveRight()) {
				boardArrayR.calculateRuns();
				total += boardArrayR.getTotal();
			}
		}

#pragma omp section 
		{
			// check right-down
			HSR_Board_Class boardArrayRD(dim_x, dim_y, hole_x, hole_y, start_x, start_y, finish_x, finish_y);
			if (boardArrayRD._moveRightDown()) {
				boardArrayRD.calculateRuns();
				total += boardArrayRD.getTotal();
			}
		}

#pragma omp section 
		{
			// check right-up
			HSR_Board_Class boardArrayRU(dim_x, dim_y, hole_x, hole_y, start_x, start_y, finish_x, finish_y);
			if (boardArrayRU._moveRightUp()) {
				boardArrayRU.calculateRuns();
				total += boardArrayRU.getTotal();
			}
		}

#pragma omp section 
		{
			// check left
			HSR_Board_Class boardArrayL(dim_x, dim_y, hole_x, hole_y, start_x, start_y, finish_x, finish_y);
			if (boardArrayL._moveLeft()) {
				boardArrayL.calculateRuns();
				total += boardArrayL.getTotal();
			}
		}

#pragma omp section 
		{
			// check left-down
			HSR_Board_Class boardArrayLD(dim_x, dim_y, hole_x, hole_y, start_x, start_y, finish_x, finish_y);
			if (boardArrayLD._moveLeftDown()) {
				boardArrayLD.calculateRuns();
				total += boardArrayLD.getTotal();
			}
		}

#pragma omp section 
		{
			// check left-up
			HSR_Board_Class boardArrayLU(dim_x, dim_y, hole_x, hole_y, start_x, start_y, finish_x, finish_y);
			if (boardArrayLU._moveLeftUp()) {
				boardArrayLU.calculateRuns();
				total += boardArrayLU.getTotal();
			}
		}

#pragma omp section 
		{
			// check up
			HSR_Board_Class boardArrayU(dim_x, dim_y, hole_x, hole_y, start_x, start_y, finish_x, finish_y);
			if (boardArrayU._moveUp()) {
				boardArrayU.calculateRuns();
				total += boardArrayU.getTotal();
			}
		}

#pragma omp section 
		{
			// check down
			HSR_Board_Class boardArrayD(dim_x, dim_y, hole_x, hole_y, start_x, start_y, finish_x, finish_y);
			if (boardArrayD._moveDown()) {
				boardArrayD.calculateRuns();
				total += boardArrayD.getTotal();
			}
		}
	} // End of omp sections

	if (total == 0) {
		HSR_Board_Class boardArray(dim_x, dim_y, hole_x, hole_y, start_x, start_y, finish_x, finish_y);
		boardArray.calculateRuns();
		return boardArray.getTotal();
	}
	return total;
} // end private member function _countHSR_openMP




  // countHSR - RENAMED to countHSR_dif so that the test program calls the openMP version
  // Wrapper function for a HSR_Board_Class that is used for holey spider run assignment
  // throws what & when HSR_Board_Class throws (allocation of array fails)
int countHSR_dif(int dim_x, int dim_y,
	int hole_x, int hole_y,
	int start_x, int start_y,
	int finish_x, int finish_y) {
	// Create a HSR_Board_Class which is a partial solution to the Holey Spider Run problem
	HSR_Board_Class boardArray(dim_x, dim_y, hole_x, hole_y, start_x, start_y, finish_x, finish_y);
	boardArray.calculateRuns();
	return boardArray.getTotal();
}

/*
void foo() {
	// Parameters for calling the two countHSR functions:
	// (dim_x,dim_y  , hole_x,hole_y  ,  start_x,start_y  ,  finish_x,finish_y)

	//countHSR(6,4, 1,1, 2,1, 0,0);// consistently takes around 465631008.15 ns/call or ~.46 s/call
	//countHSR_openMP(6,4, 1,1, 2,1, 0,0);// 					159711122.51 ns/call or ~.16 s/call

	//countHSR (9,3, 1,1, 2,1, 0,0); // The max that can be done with regular countHSR before timing out
	double answer = countHSR_openMP(9, 3, 1, 1, 2, 1, 0, 0); // The max that can be done with countHSR_openMP before... It just stops working??? 
															 // To see, change (9,3 ... ) to (7,4, ... ) 
															 // UPDATE: After running this program on my own machine and timing it.... the openMP function took 
															 // 			98 seconds to come up with over a million different paths! Still strange that netrun 
															 // 			doesn't tell me that I timed out.
															 // Also strange.... On my own machine there is actually a longer time required to calculate solutions 
															 // using openMP
	std::cout << answer << std::endl;
	return;
	
}*/

#endif // COUNTHSR_H_INCLUDED