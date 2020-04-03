#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define KB 1024
#define MB (KB * 1024)
#define GB (MB * 1024)

typedef struct
{
   size_t capacity;
   size_t size;
   void* pointer;
} Memory;

extern Memory memory_create(long long capacity);
extern void* memory_allocate(Memory* memory, size_t alloc_size);
extern void memory_clear(Memory* memory);

#endif