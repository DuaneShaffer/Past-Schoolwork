// Duane Shaffer
// CS202
// Spring 2017

#ifndef hw1_h
#define hw1_h

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using std::string;

//This is where my function definitions will go for hw1.
bool testLineForName(const string &nameToFind, std::istream &inputName, std::ostream &idToUser, std::ifstream &fileToSearch);

//This is the function for Exercise A
int getIdFromFile (const string &fileName, std::istream &inputName, std::ostream &idOut);

//This is the function for Exercise B
void numberChase (const string &fileName, std::ostream &numsOut);


#endif
