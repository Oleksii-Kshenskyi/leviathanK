#include "list.h"

struct List* list_create_head(Memory* memory, void* new_value)
{
   struct List* new_list = memory_allocate(memory, sizeof(struct List));
   new_list->head = new_list;
   new_list->tail = new_list;
   new_list->next = NULL;
   new_list->previous = NULL;
   new_list->value = new_value;

   return new_list;
}