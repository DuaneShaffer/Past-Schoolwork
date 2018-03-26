// counthsr.cpp
// Duane Shaffer
// October 7, 2017
// CS311
// 
// This is the source file for assignment 4, "Holey Spider Run"
// demonstrating recursive backtracking.
// This file follows some of the formatting used by Glen Chappell's example code.

#include "counthsr.h"
#include <algorithm>
using std::swap;
#include <iostream>

std::vector<HSR_Board_Class *> HSR_Board_Class::_allBoards; //std::vector creates an empty vector
int HSR_Board_Class::_numSolutions_allBoards = 0;


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

// operator ==
// Determines if one HSR_Board_Class is equivalent to another
bool operator == (const HSR_Board_Class &lhs, const HSR_Board_Class &rhs) {
	if (lhs.getCurrentPosition() != rhs.getCurrentPosition())
		return false;
	for (int index = 0; index < lhs.getTotalSquares(); ++index) {
		if (lhs.getSquareVal(index) != rhs.getSquareVal(index))
			return false;
	}
	return true;
}


// *********************************************************************
// class HSR_Board_Class - Definitions of member functions
// *********************************************************************

// ctor for HSR_Board_Class
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
	_runMultiplier = 1;
}

// copy ctor for HSR_Board_Class
// (see header for docs.)
HSR_Board_Class::HSR_Board_Class(const HSR_Board_Class &other) : _boardDimX(other._boardDimX), _boardDimY(other._boardDimY),
																_current_x(other._current_x), _current_y(other._current_y),
																_numSquaresLeft(other._numSquaresLeft), _isValidBoard(other._isValidBoard),
																_runningTotal(other._runningTotal) {
	_runMultiplier += other._runMultiplier;
	_boardArray = new int[_boardDimX * _boardDimY];
	for (int index = 0; index < (_boardDimX * _boardDimY); ++index) {
		_boardArray[index] = other._boardArray[index];
	}
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

// calculateRuns
// checks if _isValidBoard is true and if so, calls _countHSR_recurse()
void HSR_Board_Class::calculateRuns() {
	if (!_isValidBoard)
		return;
	if (_numSquaresLeft < 12) {
		_countHSR_recurse();
	}
	else {
		_countHSR_recurse_withDepth(3); // Populate the _allBoards array with partial solutions of the first 3 moves possible
		std::cout << "Just after initial 3 moves.   There are: " << _allBoards.size() << "objects stored." << std::endl;
		while (_allBoards.size() > 0) { // While there is anything stored in the array of all boards
			std::cout << "Going through recurse_depth(2).   There are: " << _allBoards.size() << "objects stored." << std::endl;
			auto tempSize = _allBoards.size();
			for (auto index = 0; index < tempSize; ++index) {
				_allBoards[index]->_countHSR_recurse_withDepth(2); // Make 2 more moves and store partial solutions
			}
			_allBoards.erase(_allBoards.begin(), (_allBoards.begin() + tempSize)); // Not actually sure that this works properly but it should delete all elements 
		}																			// That just recursed with depth
	} 
}

// _countHSR_recurse
// (see header for docs.)
void HSR_Board_Class::_countHSR_recurse() {
	// *** Implement method to make the recursion compact ***


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
	if (_current_x != 0) { // If we can move left
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

// _countHSR_recurse_withDepth
// (see header for docs.)
void HSR_Board_Class::_countHSR_recurse_withDepth(int depth) {
	if (depth != 0)
		--depth;
	if (_numSquaresLeft > 0 && depth != 0) {
		std::cout << "First check true. Depth: " << depth << std::endl;
		// Right side checks
		if ((_current_x + 1) != _boardDimX) {
			// check right
			// if(location to the right is valid move)
			if (_boardArray[_current_x + 1 + _current_y *_boardDimX] == 0) {
				_boardArray[_current_x + _current_y * _boardDimX] = 1;								// Current location no longer valid
				++_current_x;																		// move
				--_numSquaresLeft;
				_countHSR_recurse_withDepth(depth);													// Recursive Call
				_boardArray[_current_x + _current_y * _boardDimX] = 0;								// undo validity
				++_numSquaresLeft; 																	// unmove
				--_current_x;
			}
			// check right-down
			// if (can move down && right/down is valid move)
			if (_current_y != 0 && _boardArray[_current_x + 1 + (_current_y - 1) *_boardDimX] == 0) {
				_boardArray[_current_x + _current_y * _boardDimX] = 1;									// Current location no longer valid
				++_current_x;																	// move
				--_current_y;
				--_numSquaresLeft;
				_countHSR_recurse_withDepth(depth);														// Recursive Call
				_boardArray[_current_x + _current_y * _boardDimX] = 0;									// undo validity
				--_current_x;																	// unmove
				++_current_y;
				++_numSquaresLeft;
			}
			// check right-up
			// if (can move up && right-up is a valid move)
			if (((_current_y + 1) != _boardDimY) && _boardArray[(_current_x + 1) + (_current_y + 1) *_boardDimX] == 0) {
				_boardArray[_current_x + _current_y * _boardDimX] = 1;									// Current location no longer valid
				++_current_y;																// move
				++_current_x;
				--_numSquaresLeft;
				_countHSR_recurse_withDepth(depth);											// Recursive Call
				_boardArray[_current_x + _current_y * _boardDimX] = 0;									// undo validity
				--_current_y;																// unmove
				--_current_x;
				++_numSquaresLeft;
			}
		}
		// Left side checks
		if (_current_x != 0) { // If we can move left
			 // check left
			 // if (left is a valid move)
			if (_boardArray[_current_x - 1 + _current_y *_boardDimX] == 0) {
				_boardArray[_current_x + _current_y * _boardDimX] = 1;									// Current location no longer valid
				--_current_x;																	// move
				--_numSquaresLeft;
				_countHSR_recurse_withDepth(depth);												// Recursive Call
				_boardArray[_current_x + _current_y *_boardDimX] = 0;									// Undo validity
				++_current_x;																	// unmove
				++_numSquaresLeft;
			}
			// check left-down
			// if (can move down && left-down is valid move)
			if (_current_y != 0 && _boardArray[(_current_x - 1) + (_current_y - 1) *_boardDimX] == 0) {
				_boardArray[_current_x + _current_y *_boardDimX] = 1;									// Current location no longer valid
				--_current_y;																	// move
				--_current_x;
				--_numSquaresLeft;
				_countHSR_recurse_withDepth(depth);												// Recursive Call
				_boardArray[_current_x + _current_y * _boardDimX] = 0;									// undo validity
				++_current_y;																	// unmove
				++_current_x;
				++_numSquaresLeft;
			}
			// check left-up
			// if (can move up && left-up is a valid move)
			if (((_current_y + 1) != _boardDimY) && _boardArray[_current_x - 1 + (_current_y + 1) *_boardDimX] == 0) {
				_boardArray[_current_x + _current_y *_boardDimX] = 1;									// Current location no longer valid
				--_current_x;																	// move
				++_current_y;
				--_numSquaresLeft;
				_countHSR_recurse_withDepth(depth);												// Recursive Call
				_boardArray[_current_x + _current_y * _boardDimX] = 0;									// Undo validity
				++_current_x;																	// unmove
				--_current_y;
				++_numSquaresLeft;
			}
		}
		// check up
		// if (can move up && up is a valid move)
		if (((_current_y + 1) != _boardDimY) && _boardArray[_current_x + (_current_y + 1) *_boardDimX] == 0) {
			_boardArray[_current_x + _current_y * _boardDimX] = 1;									// Current location no longer valid
			++_current_y;																// move
			--_numSquaresLeft;
			_countHSR_recurse_withDepth(depth);											// Recursive Call
			_boardArray[_current_x + _current_y * _boardDimX] = 0;									// undo validity
			--_current_y;																	// unmove
			++_numSquaresLeft;
		}

		// check down
		// if (can move down && down is valid move)
		if (_current_y != 0 && _boardArray[_current_x + (_current_y - 1) *_boardDimX] == 0) {
			_boardArray[_current_x + _current_y * _boardDimX] = 1;									// Current location no longer valid
			--_current_y;																	// move
			--_numSquaresLeft;
			_countHSR_recurse_withDepth(depth);												// Recursive Call
			_boardArray[_current_x + _current_y *_boardDimX] = 0;									// undo validity
			++_current_y;																	// unmove
			++_numSquaresLeft;
		}

	}
	
	if (_numSquaresLeft == 0) {
		std::cout << "Second check true" << std::endl;
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

		_numSolutions_allBoards += _runningTotal * _runMultiplier;
	}

	if (depth == 0) {
		std::cout << "Hit the else" << std::endl;
		_allBoards.push_back(this); // Store the object in the global array
	}
	return;
}

// _reduce
// (see header for docs.)
void HSR_Board_Class::_reduce() {
	// This is an altered version of std::unique
	auto first = _allBoards.begin();
	auto last = _allBoards.end();
	std::cout << "Reducing the multiples" << std::endl;
		if (first == last)
			return;
		std::sort(_allBoards.begin(), _allBoards.end());
		auto result = first;
		while (++first != last) {
			if (*result == *first)
				(*first)->incrementRunMult(1);
			if (!(*result == *first) && ++result != first) { // I think I can turn this into an else if statement and eliminate the check for result==first
				*result = std::move(*first);				// I'll test this after I get it working.
			}
		}
		_allBoards.erase(result, _allBoards.end()); // erase everything that was determined to be a copy
		return;
}


// _HSR_Swap
// private member function that swaps 2 HSR objects
void HSR_Board_Class::_HSR_Swap(HSR_Board_Class &other) noexcept {
	swap(_current_x, other._current_x);
	swap(_current_y, other._current_y);
	swap(_runningTotal, other._runningTotal);
	swap(_boardArray, other._boardArray);
	swap(_isValidBoard, other._isValidBoard);
	swap(_numSquaresLeft, other._numSquaresLeft);
	swap(_runMultiplier, other._runMultiplier);
}


  // ************************************************************
  // Class HSR_Board_Class - Definition of associated global operators
  // ************************************************************


	// operator !=
	// canonical function based on op==
	// not necessary for implementation of HSR_Board_Class but silly to not include
bool operator != (const HSR_Board_Class &lhs, const HSR_Board_Class &rhs) { return !(lhs == rhs); }

  // operator <
  // Determines if one HSR_Board_Class is less than another
bool operator< (const HSR_Board_Class &lhs, const HSR_Board_Class &rhs) {
	for (int index = 0; index < lhs.getTotalSquares(); ++index) {
		if (lhs.getSquareVal(index) > rhs.getSquareVal(index))
			return false;
	}
	if (lhs == rhs)
		return false;
	return true;
}

	// operators >, <=, and >=
	// canonical functions based on op< that return a boolean describing the relation of a lhs and rhs
	// not necessary for implementation of HSR_Board_Class but silly to not include
bool operator> (const HSR_Board_Class & lhs, const HSR_Board_Class& rhs) { return rhs < lhs; }
bool operator<=(const HSR_Board_Class& lhs, const HSR_Board_Class& rhs) { return !(lhs > rhs); }
bool operator>=(const HSR_Board_Class& lhs, const HSR_Board_Class& rhs) { return !(lhs < rhs); }
