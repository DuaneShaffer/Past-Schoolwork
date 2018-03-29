// Duane Shaffer
// CS321
// 3/27/2019
// randacc.cpp
// HW 4 B





#include <iostream>
using std::cout;
using std::endl;
#include <cstddef>
using std::size_t;
#include <string>
using std::string;
#include <sstream>
#include <unistd.h>
// for _exit, write, close
#include <sys/mman.h>
// for mmap, munmap, related constants
#include <fcntl.h>
// for open, related constants
#include <sys/types.h>
// For ssize_t
#include<sys/stat.h> 


void wait_exit(int code)
{
    std::cout << "Press ENTER to quit ";
    while (std::cin.get() != '\n') ;
    _exit(code);
}

//closeit
//Pre:none
//Post:Closes a file discriptor.
void closeit(int fd) {
    std::cout << "Closing File!!!" << std::endl;
    int close_failure = close(fd);
    if (close_failure) {
        std::cout << "Close Failed" << std::endl;
        wait_exit(1);
    }
    else {
        std::cout << "Closed the File" << std::endl;
    }
}

//getFileSize
//Pre::none
//Post::Returns the filesize of the passed filename.
off_t getFileSize(const std::string & fileName){
    struct stat st;
    
    if (stat(fileName.c_str(), &st) == 0){
        return st.st_size;
    }

    return -1;

}

//readFile
//Pre::none
//Post::Reads the input given at a specific
//record number
void readFile(std::string & filePath) {
    int fd = open(
        filePath.c_str(),
        O_CREAT|O_RDONLY,
        0644);
        if (fd == -1) {
            std::cout << "Could not open file" << std::endl;
            _exit(1);
        }

        int num;
        std::string input;
        std::cout << "Enter a record (0 - 99)" << std::endl;
        std::cin >> num;
        void *addr = mmap(
            nullptr,
            getFileSize(filePath),
            PROT_READ,MAP_SHARED,
            fd,
            0);
        if(addr == MAP_FAILED){
		std::cout << "Failed to get memory" << std::endl;
		closeit(fd);
		wait_exit(1);
        }

        char *value = (char*)addr;
        std::cout << "The value at " << num << " is " << value[num] << std::endl;
      

        int munmap_failure = munmap(addr, getFileSize(filePath));

        if (munmap_failure) {
            std::cout << "Failed to deallocate" << std::endl;
            closeit(fd);
            wait_exit(1);
        }

        closeit(fd);
}

void writeFile(const std::string & fileName) {
    const int SIZE = 200;
    ssize_t dummy;
    //Open the file
    std::cout << "Opening file: " << fileName << std::endl;
    

    int fd = open(
        fileName.c_str(),        // Filename
        O_RDWR | O_CREAT | O_APPEND,
        0644);

    if (fd == -1){
        std::cout << "Could not open File" << std::endl;
        wait_exit(1);
    }

    std::cout << "File : " << fileName << " succeded" << std::endl;
    
    // Write initial contents of file
    std::cout << "Initializing file with 'write' system call" << std::endl;
    for (size_t i = 0; i < SIZE+10; ++i) {
        dummy = write(fd, "-", 1);
    }
    dummy = write(fd, "\n", 1);
    std::cout << std::endl;

    // Memory map the file
    std::cout << "Calling mmap (named file)" << std::endl;
    void * addr = mmap(
        nullptr,                 // Requested address (no request)
        SIZE,                    // How much memory
        PROT_READ | PROT_WRITE,  // Access flags
        MAP_SHARED,              // File & sharing flags
        fd,                      // File descriptor
        0);                      // Byte offset in file
    
    // Error check for mmap
    if (addr == MAP_FAILED)
    {
        std::cout << "- mmap FAILED" << std::endl;
        std::cout << std::endl;
        closeit(fd);
        std::cout << std::endl;
        wait_exit(1);
    }
    std::cout << "- mmap succeeded" << std::endl;
    std::cout << std::endl;

    int record;
    int newnum;
    std::cout << "Enter a Record to modify (0 - 99)" <<std::endl;
    std::cin >> record;
    std::cout << "Enter an Integer to put in" <<std::endl;
    std::cin >> newnum;

    // Write the memory
    cout << "Writing to mapped memory" << endl;
    char * p = (char *)addr;
    string msg = std::to_string(newnum);            // Int to store in memory
    p[record*sizeof(char)] = msg[0];
    cout << endl;

    // Unmap (free) the memory
    cout << "Calling munmap" << endl;
    int munmap_failure = munmap(
        addr,      // Address of mem (mmap return value)
        SIZE);     // How much mem (must be correct!)

    // Error check for munmap
    if (munmap_failure)
    {
        cout << "- munmap FAILED" << endl;
        cout << "- (Not sure what to do about it ...)" << endl;
        cout << endl;

        closeit(fd);
        cout << endl;

        wait_exit(1);
    }
    cout << "- munmap succeeded" << endl;
    cout << endl;

    // Close the file
    closeit(fd);
    cout << endl;

    // Comments to user
    cout << "Now look at contents of file '" << fileName << "'" << endl;
    cout << endl;

    wait_exit(0);

}




int main(){

    std::string fileName = "testing.txt";
    int n;
    std::string input;
    std::string filePath = "testing.txt";
    std::cout << "Choose one of the following" << std::endl;
    std::cout << "1: Read a record " << std::endl;
    std::cout << "2: Write a record" << std::endl;
    std::cout << "3: Quit" << std::endl;


    getline(std::cin, input);
    std::istringstream iss(input);
    iss >> n;

    switch(n){
        case 1:readFile(fileName);
        break;
        case 2:writeFile(fileName);
        break;
        case 3:wait_exit(1);
        break;
        default : std::cout << "You didn't choose an input" << std::endl;
        break;
    }
    
}