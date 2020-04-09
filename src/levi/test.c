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

void print_element(struct List* element, void* data)
{
   printf("element = %d\n", *(int*)element->value);
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

   memory_clear(&mem);
   val1 = memory_allocate(&mem, 1 * sizeof(int));
   *val1 = 365;
   val2 = memory_allocate(&mem, 1 * sizeof(int));
   *val2 = 1028;
   val3 = memory_allocate(&mem, 1 * sizeof(int));
   *val3 = 333165;
   the_list = list_create_head(&mem, val1);
   list_insert_tail(&mem, the_list, val2);
   list_insert_tail(&mem, the_list, val3);

   printf("==== CHECKING RANDOM INTS ====\n");
   struct List* current = the_list->head;
   assert(*(int*)current->value == 365);
   current = current->next;
   assert(*(int*)current->value == 1028);
   current = current->next;
   assert(*(int*)current->value == 333165);
   list_for_each(the_list, print_element, NULL);
   printf("\n");

   free(mem.pointer);
   printf("main_inserts_a_bunch_into_list_tail: OK\n");
   return 0;
}

void increment_value(struct List* element, void* data)
{
   (*(int*)element->value)++;
}

int main_checks_foreach()
{
   Memory mem = memory_create(0.5 * KB);
   int* val1 = memory_allocate(&mem, 1 * sizeof(int));
   *val1 = 100;
   int* val2 = memory_allocate(&mem, 1 * sizeof(int));
   *val2 = 200;
   int* val3 = memory_allocate(&mem, 1 * sizeof(int));
   *val3 = 300;
   int* val4 = memory_allocate(&mem, 1 * sizeof(int));
   *val4 = 400;
   int* val5 = memory_allocate(&mem, 1 * sizeof(int));
   *val5 = 500;
   struct List* the_list = list_create_head(&mem, val1);
   list_insert_tail(&mem, the_list, val2);
   list_insert_tail(&mem, the_list, val3);
   list_insert_tail(&mem, the_list, val4);
   list_insert_tail(&mem, the_list, val5);

   printf("==== BEFORE INCREMENT: ====\n");
   list_for_each(the_list, print_element, NULL);
   printf("\n");
   list_for_each(the_list, increment_value, NULL);
   printf("==== INCREMENTED VALUES: ====\n");
   list_for_each(the_list, print_element, NULL);
   printf("\n");

   assert(*(int*)(the_list->head->value) == 101);
   assert(*(int*)(the_list->head->next->value) == 201);
   assert(*(int*)(the_list->head->next->next->value) == 301);
   assert(*(int*)(the_list->head->next->next->next->value) == 401);
   assert(*(int*)(the_list->head->next->next->next->next->value) == 501);

   free(mem.pointer);
   return 0;
}

int greater_than(struct List* element, void* greater_than_what)
{
   int what = *(int*)greater_than_what;
   return (*(int*)element->value > what) ? TRUE : FALSE;
}

int main_checks_find_if()
{
   Memory mem = memory_create(0.5 * KB);
   int* val1 = memory_allocate(&mem, 1 * sizeof(int));
   *val1 = 1;
   int* val2 = memory_allocate(&mem, 1 * sizeof(int));
   *val2 = 3;
   int* val3 = memory_allocate(&mem, 1 * sizeof(int));
   *val3 = 2305;
   int* val4 = memory_allocate(&mem, 1 * sizeof(int));
   *val4 = 5605;
   int* val5 = memory_allocate(&mem, 1 * sizeof(int));
   *val5 = 300202;
   struct List* the_list = list_create_head(&mem, val1);
   list_insert_tail(&mem, the_list, val2);
   list_insert_tail(&mem, the_list, val3);
   list_insert_tail(&mem, the_list, val4);
   list_insert_tail(&mem, the_list, val5);

   void* greater_than_this = memory_allocate(&mem, sizeof(int));
   *(int*)greater_than_this = 4;

   struct List* found = list_find_first_if(the_list, greater_than, greater_than_this);

   assert(found);
   assert(*(int*)found->value == 2305);
   assert(*(int*)found->previous->value == 3);
   assert(*(int*)found->next->value == 5605);

   printf("Condition: X > %d;\n", *(int*)greater_than_this);
   printf("X = ");
   if(found)
      printf("%d\n", *(int*)found->value);
   else
      printf("NULL\n");

   *(int*)greater_than_this = 301000;
   struct List* not_found = list_find_first_if(the_list, greater_than, greater_than_this);
   assert(!not_found);

   free(mem.pointer);
   return 0;
}

int equal_to(struct List* element, void* equal_to_what)
{
   int what = *(int*)equal_to_what;
   return (*(int*)element->value == what) ? TRUE : FALSE;
}

int main/*_deletes_head*/()
{
   Memory mem = memory_create(0.5 * KB);
   int* val1 = memory_allocate(&mem, 1 * sizeof(int));
   *val1 = 132;
   int* val2 = memory_allocate(&mem, 1 * sizeof(int));
   *val2 = 428;
   int* val3 = memory_allocate(&mem, 1 * sizeof(int));
   *val3 = 201;
   int* val4 = memory_allocate(&mem, 1 * sizeof(int));
   *val4 = 9001;
   int* val5 = memory_allocate(&mem, 1 * sizeof(int));
   *val5 = 500348;
   struct List* the_list = list_create_head(&mem, val1);
   list_insert_tail(&mem, the_list, val2);
   list_insert_tail(&mem, the_list, val3);
   list_insert_tail(&mem, the_list, val4);
   list_insert_tail(&mem, the_list, val5);

   printf("==== BEFORE DELETION ====\n");
   assert(*(int*)the_list->head->value == 132);
   assert(*(int*)the_list->tail->value == 500348);
   list_for_each(the_list, print_element, NULL);
   printf("\n==== BEFORE DELETION REVERSE ====\n");
   list_reverse_for_each(the_list, print_element, NULL);
   int equal_to_this = 132;
   list_delete_first_if(&the_list, equal_to, (void*) &equal_to_this);
   printf("\n==== AFTER DELETION ====\n");
   assert(*(int*)the_list->head->value == 428);
   assert(*(int*)the_list->tail->value == 500348);
   list_for_each(the_list, print_element, NULL);
   printf("\n");
   printf("\n==== AFTER DELETION REVERSE ====\n");
   list_reverse_for_each(the_list, print_element, NULL);
   printf("\n");

   equal_to_this = 428;
   list_delete_first_if(&the_list, equal_to, (void*) &equal_to_this);
   printf("\n==== AFTER DELETION 2 ====\n");
   assert(*(int*)the_list->head->value == 201);
   assert(*(int*)the_list->tail->value == 500348);
   list_for_each(the_list, print_element, NULL);
   printf("\n");
   printf("\n==== AFTER DELETION 2 REVERSE ====\n");
   list_reverse_for_each(the_list, print_element, NULL);
   printf("\n");

   /*equal_to_this = 201;
   list_delete_first_if(&the_list, equal_to, (void*) &equal_to_this);
   printf("\n==== AFTER DELETION 3 ====\n");
   list_for_each(the_list, print_element, NULL);
   printf("\n");*/
}