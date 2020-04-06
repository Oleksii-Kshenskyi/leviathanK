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
typedef void (*foreach_handler_func)(struct List* element, void* data);

extern struct List* list_create_head(Memory* memory, void* new_value);
extern void list_insert_tail(Memory* memory, struct List* list, void* new_value);
extern void list_for_each(struct List* list, foreach_handler_func handler, void* data);
// extern struct List* list_find_first_if()
// extern void list_delete_node()

#endif