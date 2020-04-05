#ifndef _LIST_H_
#define _LIST_H_

#include "memory.h"

struct List
{
   struct List* head;
   struct List* tail;
   struct List* next;
   struct List* previous;
   void* value;
};

extern struct List* list_create_head(Memory* memory, void* new_value);

#endif