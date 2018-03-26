// Duane Shaffer
// CS202 Spring 2017
// Homework 3 "box.cpp"
// This file contains the cpp file for a box class which can be displayed on the screen. 
// Boxes have a width and a height, and can be printed to the screen (or a file). They can be hollow or filled
// and now checkered! The various types are done using inheritance.

#include "box.h"
#include <iostream>
#include <memory>
#include <stdexcept>

using std::string;
using std::unique_ptr;


std::unique_ptr<Box> boxFactory(char c, int w, int h) {
	switch (c) {
	case 'f':
		return std::make_unique<FilledBox>(w, h);
	case 'h':
		return std::make_unique<HollowBox>(w, h);
	case 'c':
		return std::make_unique<CheckeredBox>(w, h);
	default:
		throw std::runtime_error("unique_ptr<Box> boxFactory had an improper value passed to it.");
	}
	throw 0;
}



Box::Box(int width, int height) : _width(width),_height(height) // Defaults width = 1, height = 1
{
	_numCreated += 1;
}
Box::Box(const Box& copiedBox) {
	_height = copiedBox._height;
	_width = copiedBox._width;
	_numCreated += 1;
}
Box::~Box() {
	_numCreated -= 1;
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
std::ostream & operator<<(std::ostream &out, const Box &rhs) {
	rhs.print(out);
	return out;
}
int Box::howMany() {
	return _numCreated;
}
int Box::_numCreated = 0;
FilledBox::FilledBox(int width, int height) : Box(width, height){	// Defaults width = 1, height = 1
}
FilledBox::~FilledBox()
{
}

void FilledBox::print(std::ostream &outputStream) const
{
	for (int remainingHeight = 0; remainingHeight < _height; remainingHeight++) {
		for (int remainingWidth = 0; remainingWidth < _width; remainingWidth++) 
			outputStream << "x";
		outputStream << "\n";
	}
}

std::string FilledBox::type() const
{
	return "Filled";
}
HollowBox::HollowBox(int width, int height) : Box(width, height) {	// Defaults width = 1, height = 1
}
HollowBox::~HollowBox()
{
}

void HollowBox::print(std::ostream &outputStream) const{

	for (int remainingHeight = 0; remainingHeight < _height; remainingHeight++) {
		for (int remainingWidth = 0; remainingWidth < _width; remainingWidth++) {
			if (remainingHeight == 0 || remainingHeight == _height - 1)
				outputStream << "x";
			else {
				if (remainingWidth == 0 || remainingWidth == _width - 1)
					outputStream << "x";
				else
					outputStream << " ";
			}
		}
		outputStream << "\n";
	}
}

std::string HollowBox::type() const
{
	return "Hollow";
}
CheckeredBox::CheckeredBox(int width, int height) : Box(width, height) {	// Defaults width = 1, height = 1
}
CheckeredBox::~CheckeredBox()
{
}

void CheckeredBox::print(std::ostream & outputStream) const
{

	for (int remainingHeight = 0; remainingHeight < _height; remainingHeight++) {
		for (int remainingWidth = 0; remainingWidth < _width; remainingWidth++) {
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
		outputStream << "\n";
	}
}

std::string CheckeredBox::type() const
{
	return "Checkered";
}
