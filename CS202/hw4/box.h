// Duane Shaffer
// CS202 Spring 2017
// Homework 3 "box.h"
// This file contains the header for a box class which can be displayed on the screen. 
// Boxes have a width and a height, and can be printed to the screen (or a file). They can be hollow or filled
// and now checkered! The various types of box are done using inheritance.

#ifndef box_h
#define box_h

#include <string>
#include <memory>

class Box {
public: 
	Box(int w = 1, int h = 1);
	Box(const Box&);
	virtual ~Box();
	virtual std::string type() const = 0;
	int getWidth() const;
	int getHeight() const;
	void setWidth(int);
	void setHeight(int);
	virtual void print(std::ostream &) const = 0;
	static int howMany();
protected:
	int _width;
	int _height;
	static int _numCreated;
};
class FilledBox : public Box {
public:
	FilledBox(int w = 1, int h = 1);
	~FilledBox() override;
	void print(std::ostream &) const override;
	std::string type() const override;
};
class HollowBox : public Box {
public: 
	HollowBox(int w = 1, int h = 1);
	~HollowBox() override;
	void print(std::ostream &) const override;
	std::string type() const override;
};
class CheckeredBox : public Box {
public: 
	CheckeredBox(int w = 1, int h = 1);
	~CheckeredBox() override;
	void print(std::ostream &) const override;
	std::string type() const override;
};
std::ostream & operator<<(std::ostream &out, const Box &rhs);
std::unique_ptr<Box> boxFactory(char, int, int);


#endif
