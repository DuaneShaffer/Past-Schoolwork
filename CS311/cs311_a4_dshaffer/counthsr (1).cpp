#include "counthsr.h"
#include <iostream>
#include <vector>


/*
is the recursion function. Checks if the vector is in range and moves the x and y position
*/

int SpiderCount::countHSR_recurse(std::vector<int> &ary,
	int dim_x, int dim_y,
	int finish_x, int finish_y,
	int pos_x, int pos_y,
	int _squaresLeft)
{
	/*
	BASE CASE RETURNS IF SQUARES LEFT = 0 AND ON THE FINISH LINE INCREMENT TOTAL
	*/
	if ((_squaresLeft == 0) && (pos_x == finish_x) && (pos_y == finish_y))
	{
		return 1;
	}

	int total = 0;
	/*
	MOVE RIGHT
	*/
	if (isCell(pos_x + 1, pos_y, dim_x, dim_y))
	{
		if (ary[(pos_x + 1)*(dim_y) + pos_y] == 0) // means we havent been there
		{
			--_squaresLeft; // decrement the squares left
			(pos_x) += 1; // move our position
			ary[(pos_x)*(dim_y) + (pos_y)] = 1; // set that position to visited

			for (int i = 0; i < ary.size(); i++)
			{
				std::cout << ary[i];
				if(i % (dim_x) == 0)
				{
					std::cout << std::endl;
				}
			}

			std::cout << std::endl;

			total +=countHSR_recurse(ary, dim_x, dim_y, finish_x, finish_y, pos_x, pos_y, _squaresLeft); // recursion
			ary[(pos_x)*(dim_y) + (pos_y)] = 0; // backtracking set our position to 0
			++_squaresLeft; // increment our squares left
			pos_x -= 1; // move back
		}
	}
	// MOVE UP
	if (isCell(pos_x, pos_y + 1, dim_x, dim_y))
	{
		if (ary[(pos_x)*(dim_y) + (pos_y + 1)] == 0)
		{
			--_squaresLeft;
			pos_y += 1;
			ary[(pos_x)*(dim_y) + (pos_y)] = 1;

			for (int i = 0; i < ary.size(); i++)
			{
				std::cout << ary[i];
				if (i % (dim_x) == 0)
				{
					std::cout << std::endl;
				}
			}
			std::cout << std::endl;

			total +=countHSR_recurse(ary, dim_x, dim_y, finish_x, finish_y, pos_x, pos_y, _squaresLeft);
			ary[(pos_x)*(dim_y) + (pos_y)] = 0;
			++_squaresLeft;
			pos_y -= 1;
			
		}
	}

	// MOVE LEFT
	if (isCell(pos_x - 1, pos_y, dim_x, dim_y))
	{
		if (ary[(pos_x)*(dim_y) + pos_y ] == 0)
		{
			--_squaresLeft;
			pos_x += -1;
			ary[(pos_x)*(dim_y) + (pos_y)] = 1;

			for (int i = 0; i < ary.size(); i++)
			{
				std::cout << ary[i];
				if (i % (dim_x) == 0)
				{
					std::cout << std::endl;
				}
			}

			std::cout << std::endl;
			total +=countHSR_recurse(ary, dim_x, dim_y, finish_x, finish_y, pos_x, pos_y, _squaresLeft);
			ary[(pos_x)*(dim_y) + (pos_y)] = 0;
			++_squaresLeft;
			pos_x += 1;
		}
	}

	// MOVE DOWN
	if (isCell(pos_x, pos_y - 1, dim_x, dim_y))
	{
		if (ary[(pos_x)*(dim_y) + (pos_y - 1)] == 0)
		{
			--_squaresLeft;
			pos_y += -1;
			ary[(pos_x)*(dim_y) + (pos_y)] = 1;

			for (int i = 0; i < ary.size(); i++)
			{
				std::cout << ary[i];
				if (i % (dim_x) == 0)
				{
					std::cout << std::endl;
				}
			}

			std::cout << std::endl;

			total +=countHSR_recurse(ary, dim_x, dim_y, finish_x, finish_y, pos_x, pos_y, _squaresLeft);
			ary[(pos_x)*(dim_y) + (pos_y)] = 0;
			++_squaresLeft;
			pos_y += 1;
		}
	}

	//MOVE LEFT/DOWN
	if (isCell(pos_x - 1, pos_y - 1, dim_x, dim_y))
	{
		if (ary[(pos_x-1)*(dim_y)+(pos_y - 1)] == 0)
		{
			--_squaresLeft;
			pos_y += -1;
			pos_x += -1;
			ary[(pos_x)*(dim_y)+(pos_y)] = 1;

			for (int i = 0; i < ary.size(); i++)
			{
				std::cout << ary[i];
				if (i % (dim_x) == 0)
				{
					std::cout << std::endl;
				}
			}

			std::cout << std::endl;
			total +=countHSR_recurse(ary, dim_x, dim_y, finish_x, finish_y, pos_x, pos_y, _squaresLeft);
			ary[(pos_x)*(dim_y)+(pos_y)] = 0;
			++_squaresLeft;
			pos_y += 1;
			pos_x += 1;
		}
	}

	// MOVE RIGHT/DOWN
	if (isCell(pos_x + 1, pos_y - 1, dim_x, dim_y))
	{
		if (ary[(pos_x+1)*(dim_y)+(pos_y - 1)] == 0)
		{
			--_squaresLeft;
			pos_y += -1;
			pos_x += 1;
			ary[(pos_x)*(dim_y)+(pos_y)] = 1;

			for (int i = 0; i < ary.size(); i++)
			{
				std::cout << ary[i];
				if (i % (dim_x) == 0)
				{
					std::cout << std::endl;
				}
			}
			std::cout << std::endl;
			total +=countHSR_recurse(ary, dim_x, dim_y, finish_x, finish_y, pos_x, pos_y, _squaresLeft);
			ary[(pos_x)*(dim_y)+(pos_y)] = 0;
			++_squaresLeft;
			pos_y += 1;
			pos_x += -1;
		}
	}

	//MOVE RIGHT/UP
	if (isCell(pos_x + 1, pos_y + 1, dim_x, dim_y))
	{
		if (ary[(pos_x + 1)*(dim_y)+(pos_y + 1)] == 0)
		{
			--_squaresLeft;
			pos_y += 1;
			pos_x += 1;
			ary[(pos_x)*(dim_y)+(pos_y)] = 1;

			for (int i = 0; i < ary.size(); i++)
			{
				std::cout << ary[i];
				if (i % (dim_x) == 0)
				{
					std::cout << std::endl;
				}
			}
			std::cout << std::endl;
			total +=countHSR_recurse(ary, dim_x, dim_y, finish_x, finish_y, pos_x, pos_y, _squaresLeft);
			ary[(pos_x)*(dim_y)+(pos_y)] = 0;
			++_squaresLeft;
			pos_y += -1;
			pos_x += -1;
		}
	}
	
	//MOVE LEFT/UP
	if (isCell(pos_x - 1, pos_y + 1, dim_x, dim_y))
	{
		if (ary[(pos_x - 1)*(dim_y)+(pos_y + 1)] == 0)
		{
			--_squaresLeft;
			pos_y += +1;
			pos_x += -1;
			ary[(pos_x)*(dim_y)+(pos_y)] = 1;

			for (int i = 0; i < ary.size(); i++)
			{
				std::cout << ary[i];
				if (i % (dim_x) == 0)
				{
					std::cout << std::endl;
				}
			}
			std::cout << std::endl;
			total +=countHSR_recurse(ary, dim_x, dim_y, finish_x, finish_y, pos_x, pos_y, _squaresLeft);
			ary[(pos_x)*(dim_y)+(pos_y)] = 0;
			++_squaresLeft;
			pos_y += -1;
			pos_x += 1;
		}
	}
	
	return total;
}
 /*
 Creates the board and calls our recursion function to to print out the count. This is our wrapper function
 */
int SpiderCount::countHSR(int dim_x, int dim_y, int hole_x, int hole_y, int start_x, int start_y, int finish_x, int finish_y)
{
	int pos_x = start_x; //create are start x
	int pos_y = start_y; // create are start y
	int _squaresleft = (dim_y*dim_x)-2; // because we start off with two squares taken away
	size = dim_x*dim_y; // size of the array
	std::vector<int> ary(dim_x*dim_y); // make the board
	ary[hole_x*dim_y + hole_y] = 1; // set our obstacle
	ary[start_x*dim_y + start_y] = 1; // set our start path as visited.
	return countHSR_recurse(ary, dim_x, dim_y, finish_x, finish_y, pos_x, pos_y, _squaresleft);
	

}

// checks if the vector is in range returns false if its not in range.
bool SpiderCount::isCell(int pos_x, int pos_y, int dim_x, int dim_y)
{
	if ((pos_x >= dim_x) || (pos_y >= dim_y)||(pos_x*dim_y + pos_y < 0))
		return false;
	return true;
}


int main()
{
	// run a test
	SpiderCount test;
	int count = test.countHSR(4, 2, 1, 1, 0, 1, 3, 0); // should return 4 but returns 3.
	std::cout << count;
}





