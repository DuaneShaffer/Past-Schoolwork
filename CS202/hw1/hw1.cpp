// Duane Shaffer
// CS202
// Spring 2017
// This contains the programs for hw1 Exercise A and B

#include "hw1.h"

bool testLineForName(const string & nameToFind, std::istream & inputName, std::ostream &idToUser, std::ifstream &fileToSearch)
{
	string tempLine;
	fileToSearch >> tempLine;				//Pull a Name
	if (!tempLine.compare(nameToFind)) {	//If the name is matched
		fileToSearch >> tempLine;			//Get the ID
		idToUser << tempLine << '\n';		//Send ID Out
		return true;						//I'm done get out of the function
	}
	else
		fileToSearch >> tempLine;			//If Name doesn't match dump the associated ID
	return false; //If I couldn't find the name, that's fine. The function will be called again with a new line.
}

int getIdFromFile(const string &fileName, std::istream &inputName, std::ostream &idOut) // For Exercise A
{
	std::ifstream inputFile(fileName); //Try to open the file given
	if (!inputFile) //Checking that the file name exists and opened properly
		return -1;
	string nameToFind;
	idOut.clear();
	getline(inputName, nameToFind);	//Get the first name you want me to find
	while (inputName) {				//While there are still names to find
		inputFile.clear();			//Clear inputFile flags and reset the file to the beginning for each new name
		inputFile.seekg(0L, std::ios::beg);
		while (inputFile) {			//While there is still file left to search
			if (testLineForName(nameToFind, inputName, idOut, inputFile)) //If I found the name then break out of the loop
				break;
			if (!inputFile) {		//If I couldn't Find the name in File
				idOut << "error\n";
			}
		}
		getline(inputName, nameToFind); // set nameToFind to next name
	}
	return 0;
}

void numberChase(const string &fileName, std::ostream &numsOut)	// For Exercise B
{
	std::ifstream inputFile(fileName, std::ios::binary);		// Open File
	int tempNum = 0;
	while (tempNum >= 0) {	//Until I found a negative number
		inputFile.read(reinterpret_cast<char*>(&tempNum), sizeof(int)); // read a number
		numsOut << tempNum << '\n';	// Output that number
		if (tempNum >= 0) {			// So long as its positive, |
			long position = sizeof(int)*tempNum;//				V
			inputFile.seekg(position, std::ios::beg);// Change position in file to position dictated by number
		}
	}
}
