#ifndef _LIST_H_
#define _LIST_H_

#include "memory.h"

#define TRUE 1
#define FALSE 0

struct List
{
   struct List* head;
   struct List* tail;
   struct List* next;
   struct List* previous;
   void* value;
};

typedef void (*foreach_handler_func)(struct List* element, void* data);
typedef int (*list_find_predicate)(struct List* element, void* data);
#define list_delete_predicate list_find_predicate

extern struct List* list_create_head(Memory* memory, void* new_value);
extern void list_insert_tail(Memory* memory, struct List* list, void* new_value);
extern void list_for_each(struct List* list, foreach_handler_func handler, void* data);
extern struct List* list_find_first_if(struct List* list, list_find_predicate predicate, void* data);
extern void list_delete_first_if(struct List** list_ptr, list_delete_predicate predicate, void* data);

#endif