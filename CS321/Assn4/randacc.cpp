// Duane Shaffer
// CS321
// March 27, 2018
// HW 4 B
// Based off example code from Glenn G. Chappell

////////////////////////////////////////////////////////////////
/* A complete C++ program to do random-access I/O on a file. 
 * The file contains variable amount of records, each holding 
 * an integer. The user is repeatedly allowed the choice of 
 * reading and printing the value in a specified record, writing 
 * a user-provided value to a specified record, or viewing all 
 * records. Data written by the program is available for reading 
 * the next time the program is executed.
 * 
 * All file I/O is done using mmap
 * 
 * There are constants for the size and name of the record file
 *//////////////////////////////////////////////////////////////

#include <iostream>
using std::cout;
using std::endl;
using std::cin;
#include <cstddef>
using std::size_t;
#include <string>
using std::string;
using std::to_string;
#include <sstream>
#include <unistd.h>
// _exit, write, close
#include <sys/mman.h>
// mmap, munmap, related constants
#include <fcntl.h>
// open, related constants
#include <sys/types.h>
// ssize_t
#include <sys/stat.h> 
#include <limits>



// To change the size of the record, delete the record and change this constant
static const size_t NUMSINFILE = 100;
// To use your own name for the file
static string FILENAME = "Records";


void showMenu(string outputString = "", bool showFooter = true) {
    std::system("clear");
    cout << "***********************************" << endl;
    cout << "--- Welcome to Record Management --" << endl;
    cout << "***********************************\n" << endl;

    cout << outputString << endl;
    if (showFooter) {
        cout << "Choose one of the following" << endl;
        cout << "1: Read a record " << endl;
        cout << "2: Write a record" << endl;
        cout << "3: Show all records" << endl;
        cout << "4: Quit" << endl;
    }
}

int getRecord() {
    int record = 0;
    string getRecordString = "Enter a record (0 - " + to_string(NUMSINFILE) + ")";
    showMenu(getRecordString, false);
    cin >> record;
    cin.ignore();
    while (record > (int)NUMSINFILE || record < 0) {
        showMenu(getRecordString, false);
        cin >> record;
        cin.ignore();
    }
    return record;
}

void wait_exit(int code){
    cout << "Press ENTER to quit ";
    while (cin.get() != '\n') ;
    _exit(code);
}

//closeit
//Pre: none
//Post: Closes a file descriptor.
void closeit(int fd) {
    int close_failure = close(fd);
    if (close_failure) {
        cout << "Close Failed" << endl;
        wait_exit(1);
    }
}

//getFileSize
//Pre:: none
//Post:: Returns the filesize of the passed filename.
off_t getFileSize(const string & fileName){
    struct stat st;
    if (stat(fileName.c_str(), &st) == 0){
        return st.st_size;
    }
    return -1;
}

//readFile
//Pre::none
//Post::Reads the input given at a specific record number
void readFile(string & fileName){
    int fd = open(
        fileName.c_str(),
        O_CREAT|O_RDONLY,
        0644);
        if (fd == -1) {
            cout << "Could not open file" << endl;
            _exit(1);
        }

        int record;
        string input;
        record = getRecord();
        void *addr = mmap(
            nullptr,
            getFileSize(fileName),
            PROT_READ,MAP_SHARED,
            fd,
            0);
        if(addr == MAP_FAILED){
		cout << "Failed to get memory" << endl;
		closeit(fd);
		wait_exit(1);
        }

        int *value = (int*)addr;
        string output = "The value at " + to_string(record) + " is " + to_string(value[record]) + "\n";
        showMenu(output);
      
        int munmap_failure = munmap(addr, getFileSize(fileName));
        if (munmap_failure) {
            cout << "Failed to deallocate" << endl;
            closeit(fd);
            wait_exit(1);
        }
        closeit(fd);
}

void writeFile(const string & fileName) {
    //Open the file
    int fd = open(
        fileName.c_str(),        // Filename
        O_RDWR | O_CREAT | O_APPEND,
        0644);

    if (fd == -1){
        cout << "Could not open File" << endl;
        wait_exit(1);
    }
    
    // Memory map the file
    void * addr = mmap(
        nullptr,                 // Requested address (no request)
        getFileSize(fileName),   // How much memory
        PROT_READ | PROT_WRITE,  // Access flags
        MAP_SHARED,              // File & sharing flags
        fd,                      // File descriptor
        0);                      // Byte offset in file
    
    // Error check for mmap
    if (addr == MAP_FAILED){
        cout << "- mmap FAILED" << endl;
        closeit(fd);
        wait_exit(1);
    }

    int record;
    int newnum;
    record = getRecord();
    showMenu("Enter an integer to put in", false);
    cin >> newnum;
    cin.ignore();

    // Write the memory
    int * p = (int *)addr;
    p[record] = newnum;

    // Unmap (free) the memory
    int munmap_failure = munmap(
        addr,      // Address of mem (mmap return value)
        getFileSize(fileName));     // How much mem (must be correct!)

    // Error check for munmap
    if (munmap_failure){
        cout << "- munmap FAILED" << endl;
        closeit(fd);
        wait_exit(1);
    }

    // Close the file
    closeit(fd);
    string output = "The value at " + to_string(record) + " is " + to_string(newnum) + "\n";
    showMenu(output);
}

void tableDivider () {
    for (auto i = 0; i < 81; ++i){
        if (i%16 ==0) {
            cout << "|";
            continue;
        }
        cout << "-";
    }
    cout << endl;
}

void showAllRecords(string & fileName){
    int fd = open(
        fileName.c_str(),
        O_CREAT|O_RDONLY,
        0644);
        if (fd == -1) {
            cout << "Could not open file" << endl;
            _exit(1);
        }

        void *addr = mmap(
            nullptr,
            getFileSize(fileName),
            PROT_READ,MAP_SHARED,
            fd,
            0);
        if(addr == MAP_FAILED){
		cout << "Failed to get memory" << endl;
		closeit(fd);
		wait_exit(1);
        }

        int *value = (int*)addr;

        showMenu("All Records:\n", false);
        for (unsigned int i = 0; i < 5; ++i)
            cout << "|Record\tValue\t";
        cout << "|" << endl;
        tableDivider();
        for (unsigned int i = 0; i < NUMSINFILE/5; ++i) {
            for (unsigned int ii = 0; ii < 5; ++ii) {
                int location = ii*NUMSINFILE/5 + i;
                cout << "|" << location << "\t" << value[location] << "\t";
            }
            cout << "|" << endl;
            if(i%5 ==0 && i != 0) {
                tableDivider();
            }
        }
        tableDivider();
        
        int munmap_failure = munmap(addr, getFileSize(fileName));
        if (munmap_failure) {
            cout << "Failed to deallocate" << endl;
            closeit(fd);
            wait_exit(1);
        }
        closeit(fd);
        cout << "Press ENTER to return to main menu ";
        while (cin.get() != '\n') ;
        showMenu();
}

void initFile (const string & fileName){
    //Open the file
    int fd = open(
        fileName.c_str(),        // Filename
        O_RDWR,
        0644);

    if (fd == -1){
        fd = open(
        fileName.c_str(),        // Filename
        O_RDWR | O_CREAT | O_TRUNC,
        0644);

        ssize_t dummy;
        for (size_t i = 0; i < NUMSINFILE; ++i){
            dummy = write(fd, 0 , 1);
        }
        dummy = write(fd, "\n", 1);
    }
    closeit(fd);
}

int main(){
    string fileName = FILENAME + ".321hw4Dat";
    int n;
    string input;

    initFile(fileName);
    showMenu();

    while (true) {
        getline(cin, input);
        std::istringstream iss(input);
        iss >> n;

        switch(n){
            case 1: readFile(fileName);
            break;
            case 2: writeFile(fileName);
            break;
            case 3: showAllRecords(fileName);
            break;
            case 4: std::system("clear"); 
                    cout << "****************************************" << endl;
                    cout << "- Thankyou for using Record Management -" << endl;
                    cout << "****************************************\n";
                    _exit(1);
            break;
            default : showMenu("You didn't choose an input \n");
            continue;
            break;
        }
        if (cin.fail()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        }
    }
}