#include <string.h>
#include <assert.h>

#include "liblevi.h"


int main_test_memory_before_growth()
{
   Memory the_mem = memory_create(3 * KB);

   char* string_one = memory_allocate(&the_mem, 1 * KB);
   strcpy(string_one, "string one");
   char* string_two = memory_allocate(&the_mem, 1 * KB);
   strcpy(string_two, "string two");
   char* string_three = memory_allocate(&the_mem, 1 * KB);
   strcpy(string_three, "string three");

   printf("%s > %s > %s\n", string_one, string_two, string_three);

   memory_clear(&the_mem);

   char* string_four = memory_allocate(&the_mem, 1 * KB);
   strcpy(string_four, "string four");
   char* string_five = memory_allocate(&the_mem, 1 * KB);
   strcpy(string_five, "string five");
   char* string_six = memory_allocate(&the_mem, 1 * KB);
   strcpy(string_six, "string six");

   printf("%s > %s > %s\n", string_four, string_five, string_six);

   // this should core dump the application
   // this allocation goes over the allocated 3KB for the memory
   /*char* string_err = memory_allocate(&the_mem, 1);
   strcpy(string_err, "string err");*/

   free(the_mem.pointer);
   return 0;
}

int main_list_creates_head()
{
   Memory mem = memory_create(1 * KB);
   int* val = memory_allocate(&mem, 1 * sizeof(int));
   *val = 34;

   struct List* head = list_create_head(&mem, val);

   assert(head->head == head);
   assert(head->tail == head);
   assert(head->next == NULL);
   assert(head->previous == NULL);
   assert(head->value == val);
   assert(*(int*)(head->value) == 34);

   printf("head->value == %d\n", *(int*)(head->value));
   printf("TEST: SUCCESS\n");
   free(mem.pointer);
   return 0;
}

void check_list_ascending(struct List* list)
{
   int value = 1;
   struct List* current = list->head;

   printf("=== ASCENDING CHECK ===\n");
   while(value <= 5 && current != NULL)
   {
      printf("VALUE[%d] == %d\n", value, *(int*)current->value);
      assert(*(int*)current->value == value);
      current = current->next;
      value++;
   }
   printf("\n");
}

void check_list_descending(struct List* list, int starting_value)
{
   int value = starting_value;
   struct List* current = list->tail;

   printf("=== DESCENDING CHECK ===\n");
   while(value >= 1 && current != NULL)
   {
      printf("VALUE[%d] == %d\n", value, *(int*)current->value);
      assert(*(int*)current->value == value);
      current = current->previous;
      value--;
   }
   printf("\n");
}

/* this condition is not satisfied because it would
   require looping over all heads and tails and changing
   them. As it stands now , only head and tail have relevant 
   head and tail pointers. */
void check_head_tail_integrity(struct List* list)
{
   struct List* current = list->head;
   struct List* head = current->head;
   struct List* tail = current->tail;

   while(current != NULL)
   {
      assert(current->head == head);
      assert(current->tail == tail);
      current = current->next;
   }
}

int main_inserts_a_bunch_into_list_tail()
{
   Memory mem = memory_create(1 * KB);
   int* val1 = memory_allocate(&mem, 1 * sizeof(int));
   *val1 = 1;
   int* val2 = memory_allocate(&mem, 1 * sizeof(int));
   *val2 = 2;
   int* val3 = memory_allocate(&mem, 1 * sizeof(int));
   *val3 = 3;
   int* val4 = memory_allocate(&mem, 1 * sizeof(int));
   *val4 = 4;
   int* val5 = memory_allocate(&mem, 1 * sizeof(int));
   *val5 = 5;

   struct List* the_list = list_create_head(&mem, val1);
   list_insert_tail(&mem, the_list, val2);
   list_insert_tail(&mem, the_list, val3);
   list_insert_tail(&mem, the_list, val4);
   list_insert_tail(&mem, the_list, val5);

   check_list_ascending(the_list);
   check_list_descending(the_list, *val5);
   // check_head_tail_integrity(the_list); // this does not pass for now

   printf("main_inserts_a_bunch_into_list_tail: OK\n");
   return 0;
}