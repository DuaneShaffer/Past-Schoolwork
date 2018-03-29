// Duane Shaffer
// Little main that uses mapAlloc and mapFree

#include <iostream>
using std::cout;
using std::endl;
#include <map>
#include <string>
using std::map;
using std::size_t;
#include <stdio.h>
#include <cstddef> 
#include <sys/mman.h> 

size_t size;

void * mapAlloc(size_t bytes) {
    bytes += __SIZEOF_SIZE_T__;
    void * addr = mmap(
        nullptr,
        bytes,
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0);
    if (addr == MAP_FAILED){
        return nullptr;
    }
    size_t * location = (size_t *) addr;
    location[0] = bytes;
    location += __SIZEOF_SIZE_T__;
    return (void *) location;
}

void mapFree(void * memPtr) {
    size_t * p = (size_t *) memPtr - __SIZEOF_SIZE_T__;
    size_t fileSize = p[0];
    munmap(memPtr,fileSize);
}


int main () {
    int NUMNUMS = 30;

    void * location = mapAlloc(__SIZEOF_DOUBLE__*20);
    double * p = (double *) location;
    cout << "Reading: " << endl;
    for (int i = 0; i < 20; ++i) {
        cout << p[i];
    }
    cout << endl << "Writing" << endl;
    for (int i = 0; i < NUMNUMS; ++i) {
        p[i] = i;
    }
    mapFree(location);
    cout << "Trying to reallocate the memory" << endl;
    location = mapAllocRead(__SIZEOF_DOUBLE__*20);
    cout << "allocation done,now dereference it:" << endl;
    p = (double *) location;

    cout << endl << "Writing to freed memory" << endl;
    for (int i = 0; i < NUMNUMS; ++i) {
        p[i] = -i;
    }
    cout << endl;

    cout << "Reading again: " << endl;
    for (int i = 0; i < NUMNUMS; ++i) {
        cout << p[i];
    }
    cout << endl;
    mapFree(location);
    return 0;
}