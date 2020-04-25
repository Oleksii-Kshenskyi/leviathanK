#include <assert.h>

#include "list.h"

struct List* list_create_head(struct Memory* memory, void* new_value)
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

struct List* list_create_empty(struct Memory* memory)
{
   assert(memory);

   struct List* new_list = memory_allocate(memory, sizeof(struct List));
   new_list->head = NULL;
   new_list->tail = NULL;
   new_list->next = NULL;
   new_list->previous = NULL;
   new_list->value = NULL;
}

int list_is_empty(struct List* list)
{
   assert(list);

   return list->head == NULL && 
          list->tail == NULL &&
          list->next == NULL &&
          list->previous == NULL &&
          list->value == NULL;
}

static void list_insert_into_empty(struct List* list, void* new_value)
{
   assert(list);

   list->head = list;
   list->tail = list;
   list->next = NULL;
   list->previous = NULL;
   list->value = new_value;
}

void list_insert_tail(struct Memory* memory, struct List* list, void* new_value)
{
   assert(memory);
   assert(list);

   if(list_is_empty(list))
   {
      list_insert_into_empty(list, new_value);
      return;
   }

   struct List* new_node = memory_allocate(memory, sizeof(struct List));
   new_node->value = new_value;
   new_node->previous = list->tail;
   new_node->next = NULL;
   new_node->head = list->head;
   new_node->tail = new_node;

   if(list->tail)
      list->tail->next = new_node;
   list->tail = new_node;
}

void list_for_each(struct List* list, foreach_handler_func handler, void* data)
{
   assert(list);
   assert(handler);

   if(list_is_empty(list))
   {
      printf("[WARNING] List is empty, list_for_each() did nothing...\n");
      return;
   }

   struct List* current = list->head;

   while(current)
   {
      handler(current, data);
      current = current->next;
   }
}

void list_reverse_for_each(struct List* list, foreach_handler_func handler, void* data)
{
   assert(list);
   assert(handler);

   if(list_is_empty(list))
   {
      printf("[WARNING] List is empty, list_reverse_for_each() did nothing...\n");
      return;
   }

   struct List* current = list->tail;

   while(current)
   {
      handler(current, data);
      current = current->previous;
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

static void list_delete_only_element(struct List* list)
{
   assert(list);

   list->head = NULL;
   list->next = NULL;
   list->previous = NULL;
   list->tail = NULL;
   list->value = NULL;
}

static void list_delete_head(struct List** list_ptr)
{
   assert(list_ptr);

   (*list_ptr)->next->tail = (*list_ptr)->tail;

   *list_ptr = (*list_ptr)->next;
   (*list_ptr)->head = *list_ptr;
   (*list_ptr)->head->previous = NULL;
}

static void list_delete_tail(struct List* list)
{
   assert(list);

   struct List* new_tail = list->tail->previous;
   list->tail = new_tail;
   new_tail->next = NULL;
}

static void list_delete_middle_element(struct List* delete_me)
{
   assert(delete_me);

   struct List* before_me = delete_me->previous;
   struct List* after_me = delete_me->next;
   assert(before_me);
   assert(after_me);

   before_me->next = after_me;
   after_me->previous = before_me;
}

extern void list_delete_first_if(struct List** list_ptr, list_delete_predicate predicate, void* data)
{
   assert(list_ptr);
   struct List* list = *list_ptr;
   assert(list);
   assert(predicate);

   struct List* delete_me = list_find_first_if(list, predicate, data);
   if(!delete_me)
      return;

   if(delete_me == list->head && delete_me == list->tail && delete_me == list)
      list_delete_only_element(list);
   else if(delete_me == list->head && delete_me == list)
      list_delete_head(list_ptr);
   else if(delete_me == list->tail)
      list_delete_tail(list);
   else
      list_delete_middle_element(delete_me);
}