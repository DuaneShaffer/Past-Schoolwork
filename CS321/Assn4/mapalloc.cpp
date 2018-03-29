#include "mapalloc.h"

#include <iostream>
#include <map>
#include <string>
using std::map;
using std::size_t;

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

