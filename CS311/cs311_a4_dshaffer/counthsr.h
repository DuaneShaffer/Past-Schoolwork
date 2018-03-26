// counthsr.h
// Duane Shaffer
// October 7, 2017
// CS311
// 
// This is the header file for assignment 4, "Holey Spider Run"
// demonstrating recursive backtracking.

#ifndef COUNTHSR_H_INCLUDED
#define COUNTHSR_H_INCLUDED

int countHSR(int dim_x, int dim_y,
			int hole_x, int hole_y,
			int start_x, int start_y,
			int finish_x, int finish_y);

void countHSR_recurse(int* board, const int* boardDimX , const int* boardDimY, int* current_x, int* current_y, int* runningTotal, int* numSquaresLeft);




#endif // COUNTHSR_H_INCLUDED