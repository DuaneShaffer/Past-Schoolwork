// Duane Shaffer
// CS202 Spring 2017
// Homework 3 "box.h"
// This file contains the header for a box class which can be displayed on the screen. 
// Boxes have a width and a height, and can be printed to the screen (or a file). They can be hollow or filled
// and now checkered!

#ifndef box_h
#define box_h

#include <string>

class Box {
public: 
	enum boxType { FILLED, HOLLOW, CHECKERED };	// To create another type of box add the type to the end of the enumerated type.
												// change std::string type() const; to include your new type of box.
												// Then create a method for outputting your box and include it in the print functions.

	Box(int w = 1, int h = 1, boxType _type = FILLED);
	Box(const Box&);
	~Box();
	std::string type() const;
	int getWidth() const;
	int getHeight() const;
	void setWidth(int);
	void setHeight(int);
	void print(std::ostream &) const;
//	void print() const;
	static int howMany();
private:
	int _width;
	int _height;
	boxType _type;
	static int _numCreated;
};
std::ostream & operator<<(std::ostream &out, const Box &rhs);


#endif
