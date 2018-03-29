#ifndef MAPALLOC_H_INCLUDED
#define MAPALLOC_H_INCLUDED

#include <stdio.h>
#include <cstddef> 
#include <sys/mman.h> 

void * mapAlloc(size_t);
void mapFree(void *);

#endif // MAPALLOC_H_INCLUDED