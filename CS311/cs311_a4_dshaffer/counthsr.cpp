// counthsr.cpp
// Duane Shaffer
// October 7, 2017
// CS311
// 
// This is the source file for assignment 4, "Holey Spider Run"
// demonstrating recursive backtracking.


#include "counthsr.h"

int countHSR(int dim_x, int dim_y, 
			int hole_x, int hole_y, 
			int start_x, int start_y, 
			int finish_x, int finish_y)
{
	// Impossible board check
	// if (one wide && hole is not on edge)
	if ((dim_x == 1) && !((hole_y == 0) || ((hole_y+1) == dim_y))) 
		return 0;
	if ((dim_y == 1) && !((hole_x == 0) || ((hole_x + 1) == dim_x)))
		return 0;
	// if (one wide && start or finish are not on the edges)
	if (dim_x == 1) {
		if (hole_y == 0) {
			if (start_y != 1 && finish_y != 1)
				return false;
			if ((start_y + 1) != dim_y && ((finish_y + 1) != dim_y))
				return false;
		}
		if ((hole_y + 1) == dim_y) {
			if (start_y != 0 && finish_y != 0)
				return false;
			if ((start_y + 2) != dim_y && ((finish_y + 2) != dim_y))
				return false;
		}
	}
	if (dim_y == 1) {
		if (hole_x == 0) {
			if (start_x != 1 && finish_x != 1)
				return false;
			if ((start_x + 1) != dim_x && ((finish_x + 1) != dim_x))
				return false;
		}
		if ((hole_x + 1) == dim_x) {
			if (start_x != 0 && finish_x != 0)
				return false;
			if ((start_x + 2) != dim_x && ((finish_x + 2) != dim_x))
				return false;
		}
	}
	int* boardArray = new int[dim_x * dim_y];
	int squaresLeft = dim_x * dim_y - 3;
	int runningTotal = 0;
	// Make partial solution
	//		On the board, if a square is a valid move, its value is 0
	//		Therefore, the start and hole squares are 1
	//		The finish square is 2 (anything other than 0 is considered "true" for boolean checks)
	for (int index = 0; index < (dim_x*dim_y); ++index) {
		boardArray[index] = 0;
	}
	boardArray[start_x + start_y*dim_x] = 1;
	boardArray[hole_x + hole_y*dim_x] = 1;
	boardArray[finish_x + finish_y*dim_x] = 2;


	// Call the workhorse function
	countHSR_recurse(boardArray, &dim_x, &dim_y, &start_x, &start_y, &runningTotal, &squaresLeft);
	delete[] boardArray;
	return runningTotal;
}

// countHSR_recurse
// Recursive workhorse function that takes a partial solution for a holey spider run
// that has at least one valid solution and returns the number of valid solutions.
// This function implements recursive backtracking.
// Pre: board is a partial solution with at least one solution.
// No throw guarantee
void countHSR_recurse(int* board, const int* boardDimX, const int* boardDimY, int* current_x, int* current_y, int* runningTotal, int* numSquaresLeft)
{
	// The following checks for movement begin at the right side of the current square and work their way 
	// clockwise around the surrounding squares checking for the edge of the board and that the square has
	// not been previously moved to (remember, the hole is considered an invalid move and this is recorded
	// simply as if that location has already been moved to)

	// Check right
	// if(not at the edge && location to the right is valid move)
	if (((*current_x + 1) != *boardDimX) && !board[*current_x + 1 + *current_y ** boardDimX]) {
		board[*current_x + *current_y * *boardDimX] = 1;									// Current location no longer valid
		*current_x += 1;																	// move
		*numSquaresLeft -= 1;
		countHSR_recurse(board, boardDimX, boardDimY, current_x, current_y, runningTotal, numSquaresLeft);	// Recursive Call
		board[*current_x + *current_y * *boardDimX] = 0;									// undo validity
		*current_x -= 1;																	// unmove
		*numSquaresLeft += 1;
	}

	// Check diagonally right-down
	// if (can move right && can move down && right/down is valid move)
	if (((*current_x + 1) != *boardDimX) && ((*current_y) != 0) && !board[*current_x + 1 + (*current_y - 1) ** boardDimX]) {
		board[*current_x + *current_y * *boardDimX] = 1;									// Current location no longer valid
		*current_x += 1;																	// move
		*current_y -= 1;
		*numSquaresLeft -= 1;
		countHSR_recurse(board, boardDimX, boardDimY, current_x, current_y, runningTotal, numSquaresLeft);	// Recursive Call
		board[*current_x + *current_y * *boardDimX] = 0;									// undo validity
		*current_x -= 1;																	// unmove
		*current_y += 1;
		*numSquaresLeft += 1;
	}
	// Check down
	// if (can move down && down is valid move)
	if (((*current_y) != 0) && !board[*current_x + (*current_y - 1) ** boardDimX]) {
		board[*current_x + *current_y * *boardDimX] = 1;									// Current location no longer valid
		*current_y -= 1;																	// move
		*numSquaresLeft -= 1;
		countHSR_recurse(board, boardDimX, boardDimY, current_x, current_y, runningTotal, numSquaresLeft);	// Recursive Call
		board[*current_x + *current_y * *boardDimX] = 0;									// undo validity
		*current_y += 1;																	// unmove
		*numSquaresLeft += 1;
	}

	// Check diagonally down-left
	// if (can move down && can move left && down-left is valid move)
	if (((*current_y) != 0) && ((*current_x) != 0) && !board[(*current_x - 1) + (*current_y - 1) ** boardDimX]) {
		board[*current_x + *current_y * *boardDimX] = 1;									// Current location no longer valid
		*current_y -= 1;																	// move
		*current_x -= 1;
		*numSquaresLeft -= 1;
		countHSR_recurse(board, boardDimX, boardDimY, current_x, current_y, runningTotal, numSquaresLeft);	// Recursive Call
		board[*current_x + *current_y * *boardDimX] = 0;									// undo validity
		*current_y += 1;																	// unmove
		*current_x += 1;
		*numSquaresLeft += 1;
	}

	// Check left
	// if (can move left && left is a valid move)
	if (((*current_x) != 0) && !board[*current_x - 1 + *current_y ** boardDimX]) {
		board[*current_x + *current_y * *boardDimX] = 1;									// Current location no longer valid
		*current_x -= 1;																	// move
		*numSquaresLeft -= 1;
		countHSR_recurse(board, boardDimX, boardDimY, current_x, current_y, runningTotal, numSquaresLeft);	// Recursive Call
		board[*current_x + *current_y * *boardDimX] = 0;									// Undo validity
		*current_x += 1;																	// unmove
		*numSquaresLeft += 1;
	}
	// Check left-up
	// if (can move left && can move up && left-up is a valid move)
	if (((*current_x) != 0) && ((*current_y + 1) != *boardDimY) && !board[*current_x - 1 + (*current_y + 1) ** boardDimX]) {
		board[*current_x + *current_y * *boardDimX] = 1;									// Current location no longer valid
		*current_x -= 1;																	// move
		*current_y += 1;
		*numSquaresLeft -= 1;
		countHSR_recurse(board, boardDimX, boardDimY, current_x, current_y, runningTotal, numSquaresLeft);	// Recursive Call
		board[*current_x + *current_y * *boardDimX] = 0;									// Undo validity
		*current_x += 1;																	// unmove
		*current_y -= 1;
		*numSquaresLeft += 1;
	}
	// Check up
	// if (can move up && up is a valid move)
	if (((*current_y + 1) != *boardDimY) && !board[*current_x + (*current_y + 1) ** boardDimX]) {
		board[*current_x + *current_y * *boardDimX] = 1;									// Current location no longer valid
		*current_y += 1;																	// move
		*numSquaresLeft -= 1;
		countHSR_recurse(board, boardDimX, boardDimY, current_x, current_y, runningTotal, numSquaresLeft);	// Recursive Call
		board[*current_x + *current_y * *boardDimX] = 0;									// undo validity
		*current_y -= 1;																	// unmove
		*numSquaresLeft += 1;
	}

	// Check up-right
	// if (can move up && can move right && up-right is a valid move)
	if (((*current_y + 1) != *boardDimY) && ((*current_x + 1) != *boardDimX) && !board[(*current_x + 1) + (*current_y + 1) ** boardDimX]) {
		board[*current_x + *current_y * *boardDimX] = 1;									// Current location no longer valid
		*current_y += 1;																	// move
		*current_x += 1;
		*numSquaresLeft -= 1;
		countHSR_recurse(board, boardDimX, boardDimY, current_x, current_y, runningTotal, numSquaresLeft);	// Recursive Call
		board[*current_x + *current_y * *boardDimX] = 0;									// undo validity
		*current_y -= 1;																	// unmove
		*current_x -= 1;
		*numSquaresLeft += 1;
	}
	// If no moves left (we made it here)
	//		check if we were one away from finish square
	//		If so, increment runningTotal
	if (*numSquaresLeft == 0) {
		// Check surroundings for the finish square
		if (((*current_x + 1) != *boardDimX) && (board[(*current_x + 1) + *current_y**boardDimX] == 2)) //right
			*runningTotal += 1;
		else if (((*current_x + 1) != *boardDimX) && ((*current_y) != 0) && board[(*current_x + 1) + (*current_y - 1) ** boardDimX] == 2) // right-down
			*runningTotal += 1;
		else if (((*current_y) != 0) && board[*current_x + (*current_y - 1)**boardDimX] == 2) // down
			*runningTotal += 1;
		else if (((*current_y) != 0) && ((*current_x) != 0) && board[(*current_x - 1) + (*current_y - 1) ** boardDimX] == 2) // down-left
			*runningTotal += 1;
		else if (((*current_x) != 0) && board[(*current_x - 1) + *current_y**boardDimX] == 2) // left
			*runningTotal += 1;
		else if (((*current_x) != 0) && ((*current_y + 1) != *boardDimY) && board[(*current_x - 1) + (*current_y + 1) ** boardDimX] == 2) // left-up
			*runningTotal += 1;
		else if (((*current_y + 1) != *boardDimY) && board[*current_x + (*current_y + 1)**boardDimX] == 2) // up
			*runningTotal += 1;
		else if (((*current_y + 1) != *boardDimY) && board[(*current_x + 1) + (*current_y + 1) ** boardDimX] == 2) // up-right
			*runningTotal += 1;
	}
	return;
}