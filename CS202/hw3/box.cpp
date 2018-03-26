// Duane Shaffer
// CS202 Spring 2017
// Homework 3 "box.cpp"
// This file contains the cpp file for a box class which can be displayed on the screen. 
// Boxes have a width and a height, and can be printed to the screen (or a file). They can be hollow or filled
// and now checkered!

#include "box.h"
#include <iostream>

using std::string;

Box::Box(int width, int height, boxType type) // Defaults width = 1, height = 1, type = FILLED
{
	_width = width;
	_height = height;
	_type = type;
	_numCreated += 1;
}
Box::Box(const Box& copiedBox) {
	_height = copiedBox._height;
	_width = copiedBox._width;
	_type = copiedBox._type;
	_numCreated += 1;
}
Box::~Box() {
	_numCreated -= 1;
}
std::string Box::type() const
{
	if (_type == FILLED)
		return "Filled";
	else if (_type == HOLLOW)
		return "Hollow";
	else if (_type == CHECKERED)
		return "Checkered";
	else
		return "There was an error in the type";
}
int Box::getWidth() const
{
	return _width;
}
int Box::getHeight() const
{
	return _height;
}
void Box::setWidth(int width)
{
	if (width > 0)
		_width = width;
}
void Box::setHeight(int height)
{
	if (height > 0)
		_height = height;
}
void Box::print(std::ostream &outputStream) const
{
	for (int remainingHeight = 0; remainingHeight < _height; remainingHeight++) {
		for (int remainingWidth = 0; remainingWidth < _width; remainingWidth++) {
			if (_type == FILLED) {
				outputStream << "x";
			}
			else if (_type == HOLLOW) {
				if (remainingHeight == 0 || remainingHeight == _height - 1)
					outputStream << "x";
				else {
					if (remainingWidth == 0 || remainingWidth == _width - 1)
						outputStream << "x";
					else
						outputStream << " ";
				}
			}
			else if (_type == CHECKERED) {
				if (remainingHeight % 2) {					//If remainingHeight is odd
					if (remainingWidth % 2) {				//and remainingWidth is odd
						outputStream << "x";		//Output x
					}
					else {									//RemainingWidth is even
						outputStream << " ";		//Output space
					}
				}
				else {										//remainingHeight is even
					if (remainingWidth % 2) {				//and remainingWidth is odd
						outputStream << " ";		//Output space
					}
					else {									//RemainingWidth is even
						outputStream << "x";		//Output x
					}
				}
			}
			else // Unneccessary else that will make sure things default to a filled box
				outputStream << "x";
		}
		outputStream << "\n";
	}
}
/*void Box::print() const
{	
}*/
std::ostream & operator<<(std::ostream &out, const Box &rhs) {
	rhs.print(out);
	return out;
}
int Box::howMany() {
	return _numCreated;
}
int Box::_numCreated = 0;

