#include <assert.h>
#include <malloc.h>

#include "memory.h"

Memory memory_create(long long capacity)
{
   assert(capacity > 0);

   return (Memory) {
      .capacity = capacity,
      .size = 0,
      .pointer = malloc(capacity)
   };
}

void* memory_allocate(Memory* memory, size_t alloc_size)
{
   assert(memory);
   assert(memory->size + alloc_size <= memory->capacity);

   void* return_pointer = memory->pointer + memory->size;
   memory->size += alloc_size;

   return return_pointer;
}

void memory_clear(Memory* memory)
{
   assert(memory);

   memory->size = 0;
}

extern void memory_usage_status(Memory* memory)
{
   assert(memory);

   printf("[Memory at %p status] [%dB/%luB] used.\n",
          memory->pointer,
          memory->size,
          memory->capacity);
}