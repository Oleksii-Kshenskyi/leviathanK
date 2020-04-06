#include <assert.h>

#include "list.h"

struct List* list_create_head(Memory* memory, void* new_value)
{
   assert(memory);

   struct List* new_list = memory_allocate(memory, sizeof(struct List));
   new_list->head = new_list;
   new_list->tail = new_list;
   new_list->next = NULL;
   new_list->previous = NULL;
   new_list->value = new_value;

   return new_list;
}

void list_insert_tail(Memory* memory, struct List* list, void* new_value)
{
   struct List* new_node = memory_allocate(memory, sizeof(struct List));
   new_node->value = new_value;
   new_node->previous = list->tail;
   new_node->next = NULL;
   new_node->head = list->head;
   new_node->tail = new_node;

   list->tail->next = new_node;
   list->tail = new_node;
}

void list_for_each(struct List* list, foreach_handler_func handler, void* data)
{
   assert(list);
   assert(handler);

   struct List* current = list->head;

   while(current)
   {
      handler(current, data);
      current = current->next;
   }
}

struct List* list_find_first_if(struct List* list, list_find_predicate predicate, void* data)
{
   assert(list);
   assert(predicate);

   struct List* current = list->head;
   while(current && !predicate(current, data))
   {
      current = current->next;
   }

   return current;
}