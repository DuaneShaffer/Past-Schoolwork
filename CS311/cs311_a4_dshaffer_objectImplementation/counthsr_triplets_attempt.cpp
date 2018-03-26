// counthsr.cpp
// Duane Shaffer
// October 7, 2017
// CS311
// 
// This is the source file for assignment 4, "Holey Spider Run"
// demonstrating recursive backtracking.
// This file follows some of the formatting used by Glen Chappell's example code.

#include "counthsr.h"


// countHSR
// Wrapper function for a HSR_Board_Class that is used for holey spider run assignment
// throws what & when HSR_Board_Class throws (allocation of array fails)
int countHSR(int dim_x, int dim_y,
			int hole_x, int hole_y,
			int start_x, int start_y,
			int finish_x, int finish_y)
{
	// Create a HSR_Board_Class which is a partial solution to the Holey Spider Run problem
	HSR_Board_Class boardArray (dim_x, dim_y, hole_x, hole_y, start_x, start_y, finish_x, finish_y);
	boardArray.calculateRuns();
	return boardArray.getTotal();
}

// *********************************************************************
// class HSR_Board_Class - Definitions of member functions
// *********************************************************************

// HSR_Board_Class::HSR_Board_Class
// (see header for docs.)
HSR_Board_Class::HSR_Board_Class(const int boardDimX, const int boardDimY,
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
}

// HSR_Board_Class::_validateBoard
// (see header for docs.)
bool HSR_Board_Class::_validateBoard(const int hole_x, const int hole_y, const int finish_x, const int finish_y) {
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

// _startSearch
// (see header for docs.)
void HSR_Board_Class::_startSearch() {

	// Bools to track what needs to be done manually at the end
	bool upper_Right_Triplet = false;
	bool lower_Right_Triplet = false;
	bool lower_Left_Triplet = false;
	bool upper_Left_Triplet = false;

	// Bools to be used by shortcut
	// They store a value related to: have we done the manual operation for the given direction
	bool manualDown = false;
	bool manualDown_Right = false;
	bool manualDown_Left = false;
	bool manualUp = false;
	bool manualUp_Right = false;
	bool manualUp_Left = false;
	bool manualRight = false;
	bool manualLeft = false;

	// ***upper right triplet***
	if (((_current_y + 1) != _boardDimY) && ((_current_x + 1) != _boardDimX) // Bounds checking
		&& !_boardArray[(_current_x + 1) + (_current_y + 1) *_boardDimX] // location availability checks	//diagonal
		&& !_boardArray[(_current_x + 1) + (_current_y)*_boardDimX]											//right
		&& !_boardArray[(_current_x)+(_current_y + 1) *_boardDimX]) {										//up
	
		upper_Right_Triplet = true;
	//	implement shortcut
		_boardArray[(_current_x + 1) + (_current_y + 1) *_boardDimX] = 1;
		_boardArray[(_current_x + 1) + (_current_y) *_boardDimX] = 1;
		_boardArray[(_current_x) + (_current_y + 1) *_boardDimX] = 1;
		// Do shortcut based off triplet ending with square to the right of start
		int tempTotal = _runningTotal;
		_runningTotal = 0;

		++_current_x;
		_countHSR_recurse(); // Purposefully dont --_current_x (thinking ahead for next part)
		_runningTotal *= 2;
		_runningTotal += tempTotal;

		// Do shortcut based off triplet ending with square diagonally up-right
		tempTotal = _runningTotal;
		_runningTotal = 0;

		++_current_y;
		_countHSR_recurse();
		--_current_x; // --_current_x to prepare for next part of shortcut
		_runningTotal *= 2;
		_runningTotal += tempTotal;

		// Do shortcut based off triplet ending with square up
		tempTotal = _runningTotal;
		_runningTotal = 0;

		_countHSR_recurse();
		--_current_y;
		_runningTotal *= 2;
		_runningTotal += tempTotal;

	// afterwords...
		_boardArray[(_current_x + 1) + (_current_y + 1) *_boardDimX] = 0;
		_boardArray[(_current_x + 1)+(_current_y) *_boardDimX] = 0;
		_boardArray[(_current_x)+(_current_y + 1) *_boardDimX] = 0;
	} // end of upper right triplet

	  // ***lower right triplet***
	if (((_current_y - 1) != _boardDimY) && ((_current_x + 1) != _boardDimX) // Bounds checking
		&& !_boardArray[(_current_x + 1) + (_current_y - 1) *_boardDimX] // location availability checks	//diagonal
		&& !_boardArray[(_current_x + 1) + (_current_y)*_boardDimX]											//right
		&& !_boardArray[(_current_x)+(_current_y - 1) *_boardDimX]) {										//down

		lower_Right_Triplet = true;
		//	implement shortcut
		_boardArray[(_current_x + 1) + (_current_y - 1) *_boardDimX] = 1;	// diagonal
		_boardArray[(_current_x + 1) + (_current_y)*_boardDimX] = 1;		// right
		_boardArray[(_current_x)+(_current_y - 1) *_boardDimX] = 1;			// down
		// Do shortcut based off triplet ending with square to the right of start
		int tempTotal = _runningTotal;
		_runningTotal = 0;

		++_current_x;
		_countHSR_recurse(); // Purposefully dont --_current_x (thinking ahead for next part)
		_runningTotal *= 2;
		_runningTotal += tempTotal;

		// Do shortcut based off triplet ending with square diagonally down-right
		tempTotal = _runningTotal;
		_runningTotal = 0;

		--_current_y;
		_countHSR_recurse();
		--_current_x; // --_current_x to prepare for next part of shortcut
		_runningTotal *= 2;
		_runningTotal += tempTotal;

		// Do shortcut based off triplet ending with square down
		tempTotal = _runningTotal;
		_runningTotal = 0;

		_countHSR_recurse();
		++_current_y;
		_runningTotal *= 2;
		_runningTotal += tempTotal;

		// afterwords...
		_boardArray[(_current_x + 1) + (_current_y - 1) *_boardDimX] = 0;	// diagonal
		_boardArray[(_current_x + 1) + (_current_y)*_boardDimX] = 0;		// right
		_boardArray[(_current_x)+(_current_y - 1) *_boardDimX] = 0;			// down
	} // end of lower right triplet


	  // ***lower left triplet***
	if (((_current_y - 1) != _boardDimY) && ((_current_x - 1) != _boardDimX) // Bounds checking
		&& !_boardArray[(_current_x - 1) + (_current_y - 1) *_boardDimX] // location availability checks	//diagonal
		&& !_boardArray[(_current_x - 1) + (_current_y)*_boardDimX]											//left
		&& !_boardArray[(_current_x)+(_current_y - 1) *_boardDimX]) {										//down

		lower_Left_Triplet = true;
		//	implement shortcut
		_boardArray[(_current_x - 1) + (_current_y - 1) *_boardDimX] = 1;	// diagonal
		_boardArray[(_current_x - 1) + (_current_y)*_boardDimX] = 1;		// left
		_boardArray[(_current_x)+(_current_y - 1) *_boardDimX] = 1;			// down
		
		// Do shortcut based off triplet ending with square to the left of start
		int tempTotal = _runningTotal;
		_runningTotal = 0;

		--_current_x;
		_countHSR_recurse(); // Purposefully dont ++_current_x (thinking ahead for next part)
		_runningTotal *= 2;
		_runningTotal += tempTotal;

		// Do shortcut based off triplet ending with square diagonally down-left
		tempTotal = _runningTotal;
		_runningTotal = 0;

		--_current_y;
		_countHSR_recurse();
		++_current_x; // --_current_x to prepare for next part of shortcut
		_runningTotal *= 2;
		_runningTotal += tempTotal;

		// Do shortcut based off triplet ending with square down
		tempTotal = _runningTotal;
		_runningTotal = 0;

		_countHSR_recurse();
		++_current_y;
		_runningTotal *= 2;
		_runningTotal += tempTotal;

		// afterwords...
		_boardArray[(_current_x - 1) + (_current_y - 1) *_boardDimX] = 0;	// diagonal
		_boardArray[(_current_x - 1) + (_current_y)*_boardDimX] = 0;		// left
		_boardArray[(_current_x)+(_current_y - 1) *_boardDimX] = 0;			// down
	} // end of lower left triplet

	  // ***upper left triplet***
	if (((_current_y + 1) != _boardDimY) && ((_current_x - 1) != _boardDimX) // Bounds checking
		&& !_boardArray[(_current_x - 1) + (_current_y + 1) *_boardDimX] // location availability checks	//diagonal
		&& !_boardArray[(_current_x - 1) + (_current_y)*_boardDimX]											//left
		&& !_boardArray[(_current_x)+(_current_y + 1) *_boardDimX]) {										//up

		upper_Left_Triplet = true;
		//	implement shortcut
		_boardArray[(_current_x - 1) + (_current_y + 1) *_boardDimX] = 1;	// diagonal
		_boardArray[(_current_x - 1) + (_current_y)*_boardDimX] = 1;		// left
		_boardArray[(_current_x)+(_current_y + 1) *_boardDimX] = 1;			// up

		// Do shortcut based off triplet ending with square to the left of start
		int tempTotal = _runningTotal;
		_runningTotal = 0;

		--_current_x;
		_countHSR_recurse(); // Purposefully dont ++_current_x (thinking ahead for next part)
		_runningTotal *= 2;
		_runningTotal += tempTotal;

		// Do shortcut based off triplet ending with square diagonally up-left
		tempTotal = _runningTotal;
		_runningTotal = 0;

		++_current_y;
		_countHSR_recurse();
		++_current_x; // ++_current_x to prepare for next part of shortcut
		_runningTotal *= 2;
		_runningTotal += tempTotal;

		// Do shortcut based off triplet ending with square up
		tempTotal = _runningTotal;
		_runningTotal = 0;

		_countHSR_recurse();
		--_current_y;
		_runningTotal *= 2;
		_runningTotal += tempTotal;

		// afterwords...
		_boardArray[(_current_x - 1) + (_current_y + 1) *_boardDimX] = 0;	// diagonal
		_boardArray[(_current_x - 1) + (_current_y)*_boardDimX] = 0;		// left
		_boardArray[(_current_x)+(_current_y + 1) *_boardDimX] = 0;			// up
	} // end of upper left triplet


	//*********************************************************************************************************************************
	// What follows will be some non "DRY" code. I recommend just collapsing it and assuming that I implemented it properly
	// I need to manually do the pathways that weren't simplified
	//*********************************************************************************************************************************
	
	// *** DO THIS ***

} // end HSR_Board_Class::_startSearch()

// _countHSR_recurse
// (see header for docs.)
void HSR_Board_Class::_countHSR_recurse() {
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
		if (_current_y && !_boardArray[_current_x + 1 + (_current_y - 1) *_boardDimX]) {
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