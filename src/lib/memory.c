#include <assert.h>
#include <malloc.h>

#include "memory.h"

struct Memory memory_create(size_t capacity)
{
   assert(capacity > 0);

   return (struct Memory) {
      .capacity = capacity,
      .size = 0,
      .pointer = malloc(capacity)
   };
}

struct Memory* memory_create_heap(size_t capacity)
{
   assert(capacity > 0);

   struct Memory* memory = malloc(sizeof(struct Memory));
   memory->capacity = capacity;
   memory->size = 0;
   memory->pointer = malloc(capacity);

   return memory;
}

void* memory_allocate(struct Memory* memory, size_t alloc_size)
{
   assert(memory);
   assert(memory->size + alloc_size <= memory->capacity);

   void* return_pointer = memory->pointer + memory->size;
   memory->size += alloc_size;

   return return_pointer;
}

void memory_clear(struct Memory* memory)
{
   assert(memory);

   memory->size = 0;
}

extern void memory_usage_status(struct Memory* memory)
{
   assert(memory);

   printf("[Memory at %p status] [%dB/%luB] used.\n",
          memory->pointer,
          memory->size,
          memory->capacity);
}