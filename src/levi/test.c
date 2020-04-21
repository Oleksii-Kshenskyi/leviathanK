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

   assert(!strcmp(string_one, "string one"));
   assert(!strcmp(string_two, "string two"));
   assert(!strcmp(string_three, "string three"));

   printf("%s > %s > %s\n", string_one, string_two, string_three);

   memory_clear(&the_mem);

   char* string_four = memory_allocate(&the_mem, 1 * KB);
   strcpy(string_four, "string four");
   char* string_five = memory_allocate(&the_mem, 1 * KB);
   strcpy(string_five, "string five");
   char* string_six = memory_allocate(&the_mem, 1 * KB);
   strcpy(string_six, "string six");

   assert(!strcmp(string_four, "string four"));
   assert(!strcmp(string_five, "string five"));
   assert(!strcmp(string_six, "string six"));

   printf("%s > %s > %s\n", string_four, string_five, string_six);

   // this should core dump the application
   // this allocation goes over the allocated 3KB for the memory
   /*char* string_err = memory_allocate(&the_mem, 1);
   strcpy(string_err, "string err");*/

   printf("main_test_memory_before_growth: OK!\n");
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
   printf("main_list_creates_head: OK!\n");
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
   if(!element->value)
      printf("element = [EMPTY]\n");
   else
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

   printf("main_inserts_a_bunch_into_list_tail: OK!\n");
   free(mem.pointer);
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

   printf("main_checks_foreach: OK!\n");   
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

   printf("main_checks_find_if: OK!\n");
   free(mem.pointer);
   return 0;
}

int equal_to_int(struct List* element, void* equal_to_what)
{
   int what = *(int*)equal_to_what;
   return (*(int*)element->value == what) ? TRUE : FALSE;
}

int equal_to_pointer(struct List* element, void* equal_to_what)
{
   return (element->value == equal_to_what) ? TRUE : FALSE;
}

int main_deletes_head()
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
   int equal_to_int_this = 132;
   list_delete_first_if(&the_list, equal_to_int, (void*) &equal_to_int_this);
   printf("\n==== AFTER DELETION ====\n");
   assert(*(int*)the_list->head->value == 428);
   assert(*(int*)the_list->tail->value == 500348);
   list_for_each(the_list, print_element, NULL);
   printf("\n");
   printf("\n==== AFTER DELETION REVERSE ====\n");
   list_reverse_for_each(the_list, print_element, NULL);
   printf("\n");

   equal_to_int_this = 428;
   list_delete_first_if(&the_list, equal_to_int, (void*) &equal_to_int_this);
   printf("\n==== AFTER DELETION 2 ====\n");
   assert(*(int*)the_list->head->value == 201);
   assert(*(int*)the_list->tail->value == 500348);
   list_for_each(the_list, print_element, NULL);
   printf("\n");
   printf("\n==== AFTER DELETION 2 REVERSE ====\n");
   list_reverse_for_each(the_list, print_element, NULL);
   printf("\n");

   printf("main_deletes_head: OK!\n");
   free(mem.pointer);
   return 0;
}

int main_deletes_tail()
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
   int equal_to_int_this = 500348;
   list_delete_first_if(&the_list, equal_to_int, (void*) &equal_to_int_this);
   printf("\n==== AFTER DELETION ====\n");
   assert(*(int*)the_list->head->value == 132);
   assert(*(int*)the_list->tail->value == 9001);
   list_for_each(the_list, print_element, NULL);
   printf("\n");
   printf("\n==== AFTER DELETION REVERSE ====\n");
   list_reverse_for_each(the_list, print_element, NULL);
   printf("\n");

   equal_to_int_this = 9001;
   list_delete_first_if(&the_list, equal_to_int, (void*) &equal_to_int_this);
   printf("\n==== AFTER DELETION 2 ====\n");
   assert(*(int*)the_list->head->value == 132);
   assert(*(int*)the_list->tail->value == 201);
   list_for_each(the_list, print_element, NULL);
   printf("\n");
   printf("\n==== AFTER DELETION 2 REVERSE ====\n");
   list_reverse_for_each(the_list, print_element, NULL);
   printf("\n");

   equal_to_int_this = 201;
   list_delete_first_if(&the_list, equal_to_int, (void*) &equal_to_int_this);
   printf("\n==== AFTER DELETION 3 ====\n");
   assert(*(int*)the_list->head->value == 132);
   assert(*(int*)the_list->tail->value == 428);
   list_for_each(the_list, print_element, NULL);
   printf("\n");
   printf("\n==== AFTER DELETION 3 REVERSE ====\n");
   list_reverse_for_each(the_list, print_element, NULL);
   printf("\n");

   printf("main_deletes_tail: OK!\n");
   free(mem.pointer);
   return 0;
}

int main_deletes_middle_element()
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
   int equal_to_int_this = 428;
   list_delete_first_if(&the_list, equal_to_int, (void*) &equal_to_int_this);
   printf("\n==== AFTER DELETION ====\n");
   assert(*(int*)the_list->head->value == 132);
   assert(*(int*)the_list->tail->value == 500348);
   list_for_each(the_list, print_element, NULL);
   printf("\n");
   printf("\n==== AFTER DELETION REVERSE ====\n");
   list_reverse_for_each(the_list, print_element, NULL);
   printf("\n");

   equal_to_int_this = 201;
   list_delete_first_if(&the_list, equal_to_int, (void*) &equal_to_int_this);
   printf("\n==== AFTER DELETION 2 ====\n");
   assert(*(int*)the_list->head->value == 132);
   assert(*(int*)the_list->tail->value == 500348);
   list_for_each(the_list, print_element, NULL);
   printf("\n");
   printf("\n==== AFTER DELETION 2 REVERSE ====\n");
   list_reverse_for_each(the_list, print_element, NULL);
   printf("\n");

   equal_to_int_this = 9001;
   list_delete_first_if(&the_list, equal_to_int, (void*) &equal_to_int_this);
   printf("\n==== AFTER DELETION 3 ====\n");
   assert(*(int*)the_list->head->value == 132);
   assert(*(int*)the_list->tail->value == 500348);
   list_for_each(the_list, print_element, NULL);
   printf("\n");
   printf("\n==== AFTER DELETION 3 REVERSE ====\n");
   list_reverse_for_each(the_list, print_element, NULL);
   printf("\n");

   printf("main_deletes_middle_element: OK!\n");
   free(mem.pointer);
   return 0;
}

int main_creates_empty_list_and_inserts_into_it()
{
   Memory mem = memory_create(0.5 * KB);

   struct List* empty_list = list_create_empty(&mem);

   int* val1 = memory_allocate(&mem, 1 * sizeof(int));
   *val1 = 3;
   int* val2 = memory_allocate(&mem, 1 * sizeof(int));
   *val2 = 8;
   int* val3 = memory_allocate(&mem, 1 * sizeof(int));
   *val3 = 0;

   //trying to foreach an empty list
   list_for_each(empty_list, print_element, NULL);
   list_reverse_for_each(empty_list, print_element, NULL);

   list_insert_tail(&mem, empty_list, val1);
   printf("\n==== AFTER INSERTION 1 ====\n");
   assert(*(int*)empty_list->head->value == 3);
   assert(*(int*)empty_list->tail->value == 3);
   list_for_each(empty_list, print_element, NULL);
   printf("\n==== AFTER INSERTION 1 REVERSE ====\n");
   list_reverse_for_each(empty_list, print_element, NULL);
   printf("\n");

   list_insert_tail(&mem, empty_list, val2);
   printf("\n==== AFTER INSERTION 2 ====\n");
   assert(*(int*)empty_list->head->value == 3);
   assert(*(int*)empty_list->tail->value == 8);
   list_for_each(empty_list, print_element, NULL);
   printf("\n==== AFTER INSERTION 2 REVERSE ====\n");
   list_reverse_for_each(empty_list, print_element, NULL);
   printf("\n");

   list_insert_tail(&mem, empty_list, val3);
   printf("\n==== AFTER INSERTION 3 ====\n");
   assert(*(int*)empty_list->head->value == 3);
   assert(*(int*)empty_list->tail->value == 0);
   list_for_each(empty_list, print_element, NULL);
   printf("\n==== AFTER INSERTION 3 REVERSE ====\n");
   list_reverse_for_each(empty_list, print_element, NULL);
   printf("\n");

   printf("main_creates_empty_list_and_inserts_into_it: OK\n");
   free(mem.pointer);
   return 0;
}

int main_deletes_the_only_list_element()
{
   Memory mem = memory_create(0.5 * KB);

   struct List* empty_list = list_create_empty(&mem);

   int* val1 = memory_allocate(&mem, 1 * sizeof(int));
   *val1 = 111;
   list_insert_tail(&mem, empty_list, val1);

   printf("\n==== BEFORE DELETION ====\n");
   list_for_each(empty_list, print_element, NULL);
   assert(empty_list->head->value == val1);

   printf("\n==== AFTER DELETION ====\n");
   list_delete_first_if(&empty_list, equal_to_int, val1);
   list_for_each(empty_list, print_element, NULL);
   printf("\n");

   printf("\n==== AFTER DELETION REVERSE ====\n");
   list_reverse_for_each(empty_list, print_element, NULL);
   printf("\n");

   printf("main_deletes_the_only_list_element: OK\n");
   free(mem.pointer);
   return 0;
}

int main_checks_subsequent_deletion_and_insertion()
{
   Memory mem = memory_create(0.5 * KB);

   printf("\nmain_checks_subsequent_deletion_and_insertion:\n");
   printf("[STATUS] creating list with 3 elements: 33, 5151, 70815...\n");
   int val1 = 33; int val2 = 5151; int val3 = 70815;
   struct List* list = list_create_empty(&mem);
   list_insert_tail(&mem, list, (void*)&val1);
   list_insert_tail(&mem, list, (void*)&val2);
   list_insert_tail(&mem, list, (void*)&val3);
   assert(*(int*)list->head->value == val1);
   assert(*(int*)list->head->next->value == val2);
   assert(*(int*)list->head->next->next->value == val3);

   printf("[STATUS] insertion OK, deleting all the elements...\n");
   list_delete_first_if(&list, equal_to_int, (void*)&val1);
   assert(*(int*)list->head->value == val2);
   list_delete_first_if(&list, equal_to_int, (void*)&val2);
   assert(*(int*)list->head->value == val3);
   list_delete_first_if(&list, equal_to_int, (void*)&val3);
   assert(list_is_empty(list));
   printf("[STATUS] Deleted everything, checked that the list is empty...\n");

   printf("[STATUS] Now inserting 3 different ints...\n");
   printf("[STATUS] Inserting 480, 0, 871515...\n");
   int val4 = 480; int val5 = 0; int val6 = 871515;
   list_insert_tail(&mem, list, (void*)&val4);
   assert(*(int*)list->head->value == val4);
   assert(*(int*)list->tail->value == val4);
   list_insert_tail(&mem, list, (void*)&val5);
   assert(*(int*)list->head->value == val4);
   assert(*(int*)list->tail->value == val5);
   list_insert_tail(&mem, list, (void*)&val6);
   assert(*(int*)list->head->value == val4);
   assert(*(int*)list->tail->value == val6);
   printf("[STATUS] Inserted the 3 values, checked each step, all OK...\n");

   printf("[STATUS] And now back to an empty list...\n");
   list_delete_first_if(&list, equal_to_int, (void*)&val4);
   assert(*(int*)list->head->value == val5);
   list_delete_first_if(&list, equal_to_int, (void*)&val5);
   assert(*(int*)list->head->value == val6);
   list_delete_first_if(&list, equal_to_int, (void*)&val6);
   assert(list_is_empty(list));
   printf("[STATUS] List is empty again, everything looks fine!\n");

   printf("main_checks_subsequent_deletion_and_insertion: OK\n");
   free(mem.pointer);
   return 0;
}

int main_tries_to_delete_from_empty_list()
{
   Memory mem = memory_create(0.1 * KB);

   struct List* empty_list = list_create_empty(&mem);
   assert(list_is_empty(empty_list));

   printf("\nmain_tries_to_delete_from_empty_list:\n");
   printf("[STATUS] Inserting 1 into the list, but trying to delete 2...\n");
   int temp = 1; int wrong_value = 2;
   list_insert_tail(&mem, empty_list, (void*)&temp);
   list_delete_first_if(&empty_list, equal_to_int, (void*)&wrong_value);
   printf("[STATUS] Checking that 1 is still there and is the only element...\n");
   assert(*(int*)empty_list->head->value == temp);
   assert(*(int*)empty_list->tail->value == temp);
   printf("[STATUS] Check successful, deleting the 1...\n");
   list_delete_first_if(&empty_list, equal_to_int, (void*)&temp);
   assert(list_is_empty(empty_list));
   printf("[STATUS] Deletion successful, list is empty...\n");

   printf("[STATUS] Trying to delete a 1 from an empty list...\n");
   list_delete_first_if(&empty_list, equal_to_int, (void*)&temp);
   assert(list_is_empty(empty_list));
   printf("[STATUS] All OK, list is still empty...\n");

   printf("[STATUS] Now trying to delete a NULL from an empty list...\n");
   list_delete_first_if(&empty_list, equal_to_int, NULL);
   assert(list_is_empty(empty_list));
   printf("[STATUS] All OK, list is still empty!\n");

   printf("main_tries_to_delete_from_empty_list: OK\n");
   free(mem.pointer);
   return 0;
}

int main_deletes_null_elements_from_list()
{
   Memory mem = memory_create(0.5 * KB);

   printf("\nmain_deletes_null_elements_from_list:\n");
   printf("[STATUS] Creating a list of elements: [1, NULL, 3, NULL]\n");
   int val1 = 1; int val3 = 3;
   struct List* list = list_create_empty(&mem);
   list_insert_tail(&mem, list, (void*)&val1);
   list_insert_tail(&mem, list, NULL);
   list_insert_tail(&mem, list, (void*)&val3);
   list_insert_tail(&mem, list, NULL);
   assert(*(int*)list->head->value == val1);
   assert(list->head->next->value == NULL);
   assert(*(int*)list->head->next->next->value == val3);
   assert(list->head->next->next->next->value == NULL);
   printf("[STATUS] Insertion successful, list created as expected...\n");

   printf("[STATUS] Trying to print out a list with NULLs...\n");
   list_for_each(list, print_element, NULL);

   printf("[STATUS] Deleting the first NULL from the list...\n");
   list_delete_first_if(&list, equal_to_pointer, NULL);
   assert(*(int*)list->head->value == val1);
   assert(*(int*)list->head->next->value == val3);
   assert(list->head->next->next->value == NULL);
   printf("[STATUS] Deleting the first NULL sucessful...\n");

   printf("[STATUS] Deleting the second NULL from the list...\n");
   list_delete_first_if(&list, equal_to_pointer, NULL);
   assert(*(int*)list->head->value == val1);
   assert(*(int*)list->head->next->value == val3);
   assert(*(int*)list->head->value == val1);
   assert(*(int*)list->tail->value == val3);
   printf("[STATUS] Deleting the second NULL sucessful...\n");

   printf("[STATUS] Printing out the resulting list...\n");
   list_for_each(list, print_element, NULL);

   printf("[STATUS] Printing out the resulting list in reverse...\n");
   list_reverse_for_each(list, print_element, NULL);

   printf("[STATUS] All seems to be OK...\n");
   printf("main_deletes_null_elements_from_list: OK\n");
   free(mem.pointer);
   return 0;
}

int main_creates_root_path_tree_node()
{
   printf("\nmain_creates_root_path_tree_node:\n");
   printf("[STATUS] Allocating memory and creating path tree root...\n");
   Memory mem = memory_create(0.5 * KB);
   struct PathTree* root = path_tree_create(&mem);

   printf("[STATUS] Making sure root fields have expected values...\n");
   assert(root->children == NULL);
   assert(root->node_value == NULL);
   assert(!strcmp(root->node_name, ROOT_NAME));

   path_tree_print(root);
   printf("[STATUS] Path tree root seems to have been created properly!\n");
   printf("main_creates_root_path_tree_node: OK\n");
   free(mem.pointer);
   return 0;
}

int main_util_builds_paths_correctly()
{
   printf("main_util_builds_paths_correctly:\n");
   printf("[STATUS] Creating 2 KB memory...\n");
   Memory mem = memory_create(2 * KB);

   printf("[STATUS] Checking old + new names == \"\"...\n");
   char* path = util_build_path_prefix(&mem, "", "");
   assert(!strcmp(path, ""));

   printf("[STATUS] Checking old_path == \"\"...\n");
   path = util_build_path_prefix(&mem, "", "new path");
   assert(!strcmp(path, "new path"));

   printf("[STATUS] Checking new_name == \"\"...\n");
   path = util_build_path_prefix(&mem, "old/one/two", "");
   assert(!strcmp(path, "old/one/two"));

   printf("[STATUS] Sequentially building path one/two/three/four/five/siz...\n");
   path = util_build_path_prefix(&mem, "", "old");
   assert(!strcmp(path, "old"));
   path = util_build_path_prefix(&mem, path, "one");
   assert(!strcmp(path, "old/one"));
   path = util_build_path_prefix(&mem, path, "two");
   assert(!strcmp(path, "old/one/two"));
   path = util_build_path_prefix(&mem, path, "three");
   assert(!strcmp(path, "old/one/two/three"));
   path = util_build_path_prefix(&mem, path, "four");
   assert(!strcmp(path, "old/one/two/three/four"));
   path = util_build_path_prefix(&mem, path, "five");
   assert(!strcmp(path, "old/one/two/three/four/five"));
   path = util_build_path_prefix(&mem, path, "siz");
   assert(!strcmp(path, "old/one/two/three/four/five/siz"));

   memory_usage_status(&mem);
   printf("main_util_builds_paths_correctly: OK\n");
   free(mem.pointer);
   return 0;
}

int main_util_builds_paths_noalloc_correctly()
{
   printf("\nmain_util_builds_paths_noalloc_correctly:\n");
   printf("[STATUS] Creating 2 KB memory...\n");
   Memory mem = memory_create(2 * KB);

   printf("[STATUS] Allocating path buffer...\n");
   char* path = memory_allocate(&mem, 1 * KB);

   printf("[STATUS] Checking old + new names == \"\"...\n");
   strcpy(path, "");
   util_build_path_prefix_noalloc(&path, "");
   assert(!strcmp(path, ""));

   printf("[STATUS] Checking old_path == \"\"...\n");
   util_build_path_prefix_noalloc(&path, "new path");

   assert(!strcmp(path, "new path"));

   printf("[STATUS] Checking new_name == \"\"...\n");
   strcpy(path, "old/one/two");
   util_build_path_prefix_noalloc(&path, "");
   assert(!strcmp(path, "old/one/two"));

   printf("[STATUS] Sequentially building path one/two/three/four/five/siz...\n");
   strcpy(path, "");
   util_build_path_prefix_noalloc(&path, "old");
   assert(!strcmp(path, "old"));
   util_build_path_prefix_noalloc(&path, "one");
   assert(!strcmp(path, "old/one"));
   util_build_path_prefix_noalloc(&path, "two");
   assert(!strcmp(path, "old/one/two"));
   util_build_path_prefix_noalloc(&path, "three");
   assert(!strcmp(path, "old/one/two/three"));
   util_build_path_prefix_noalloc(&path, "four");
   assert(!strcmp(path, "old/one/two/three/four"));
   util_build_path_prefix_noalloc(&path, "five");
   assert(!strcmp(path, "old/one/two/three/four/five"));
   util_build_path_prefix_noalloc(&path, "siz");
   assert(!strcmp(path, "old/one/two/three/four/five/siz"));

   memory_usage_status(&mem);
   printf("[STATUS] All seems fine, all asserts passed...\n");
   printf("main_util_builds_paths_noalloc_correctly: OK\n");
   free(mem.pointer);
   return 0;
}

int main_unbuilds_path_prefix()
{
   printf("\nmain_unbuilds_path_prefix:\n");
   Memory mem = memory_create(0.5 * KB);

   printf("[STATUS] Checking sequentially unbuilding the following path:\n");
   printf("         Hekek/a/pretty/long/path/that/doesn't do/anything/in/particular/honestly!\n");
   char* path = memory_allocate(&mem, strlen("Hekek/a/pretty/long/path/that/doesn't do/anything/in/particular/honestly!") + 1);
   
   strcpy(path, "Hekek/a/pretty/long/path/that/doesn't do/anything/in/particular/honestly!");

   util_unbuild_path_prefix_once(path);
   assert(!strcmp(path, "Hekek/a/pretty/long/path/that/doesn't do/anything/in/particular"));
   util_unbuild_path_prefix_once(path);
   assert(!strcmp(path, "Hekek/a/pretty/long/path/that/doesn't do/anything/in"));
   util_unbuild_path_prefix_once(path);
   assert(!strcmp(path, "Hekek/a/pretty/long/path/that/doesn't do/anything"));
   util_unbuild_path_prefix_once(path);
   assert(!strcmp(path, "Hekek/a/pretty/long/path/that/doesn't do"));
   util_unbuild_path_prefix_once(path);
   assert(!strcmp(path, "Hekek/a/pretty/long/path/that"));
   util_unbuild_path_prefix_once(path);
   assert(!strcmp(path, "Hekek/a/pretty/long/path"));
   util_unbuild_path_prefix_once(path);
   assert(!strcmp(path, "Hekek/a/pretty/long"));
   util_unbuild_path_prefix_once(path);
   assert(!strcmp(path, "Hekek/a/pretty"));
   util_unbuild_path_prefix_once(path);
   assert(!strcmp(path, "Hekek/a"));
   util_unbuild_path_prefix_once(path);
   assert(!strcmp(path, "Hekek"));
   util_unbuild_path_prefix_once(path);
   assert(!strcmp(path, ""));

   printf("[STATUS] Checking unbuilding \"\" to result in a \"\"...\n");
   util_unbuild_path_prefix_once(path);
   assert(!strcmp(path, ""));
   util_unbuild_path_prefix_once(path);
   assert(!strcmp(path, ""));

   printf("[STATUS] Checking a string with junk being unbuilt to \"\" correctly...\n");
   strcpy(path, "a bunch of @ random ! junk kek without a slash.");
   assert(!strcmp(path, "a bunch of @ random ! junk kek without a slash."));
   util_unbuild_path_prefix_once(path);
   assert(!strcmp(path, "")); 

   memory_usage_status(&mem);
   printf("main_unbuilds_path_prefix: OK\n");
   free(mem.pointer);
}

int main_checks_unbuilds_to_equal()
{
   printf("\nmain_checks_unbuild_to_equal:\n");
   Memory mem = memory_create(0.5 * KB);

   printf("[STATUS] Checking if [please/do/this/properly] unbuilds to [please/do/this]...\n");
   char* path = util_string_create(&mem, "please/do/this/properly", 64);
   char* check_equal_to = util_string_create(&mem, "please/do/this", 64);
   printf("         Answer is -> %s", (util_check_unbuilds_to_equal_nonempty(path, check_equal_to)) ? "yes!\n" : "no!\n");

   printf("[STATUS] Checking if [please] unbuilds to []...\n");
   strcpy(path, "so");
   strcpy(check_equal_to, "");
   printf("         Answer is -> %s", (util_check_unbuilds_to_equal_nonempty(path, check_equal_to)) ? "yes!\n" : "no!\n");

   printf("[STATUS] Checking if [] unbuilds to []...\n");
   strcpy(path, "");
   strcpy(check_equal_to, "");
   printf("         Answer is -> %s", (util_check_unbuilds_to_equal_nonempty(path, check_equal_to)) ? "yes!\n" : "no!\n");

   printf("[STATUS] Checking if [s/t] unbuilds to [s]...\n");
   strcpy(path, "s/t");
   strcpy(check_equal_to, "s");
   printf("         Answer is -> %s", (util_check_unbuilds_to_equal_nonempty(path, check_equal_to)) ? "yes!\n" : "no!\n");

   printf("[STATUS] Checking if [!@#$#$$!@#*&*&(&/@@@] unbuilds to [!@#$#$$!@#*&*&(&]...\n");
   strcpy(path, "!@#$#$$!@#*&*&(&/@@@");
   strcpy(check_equal_to, "!@#$#$$!@#*&*&(&");
   printf("         Answer is -> %s", (util_check_unbuilds_to_equal_nonempty(path, check_equal_to)) ? "yes!\n" : "no!\n");

   memory_usage_status(&mem);
   printf("main_checks_unbuild_to_equal: OK\n");
   free(mem.pointer);
}

void chop_off_single_pass_noalloc(char** full_path, 
                      char* chopped_off_expected, char* full_path_expected)
{
   // printf("\n==== CHOP OFF ITERATION ====\n");
   char* chopped_off = util_chop_current_name_off_path_noalloc(full_path);
   // printf("Chopped off = [%s]\nPath = [%s]\n", chopped_off, *full_path);
   // printf("Chopped off expected = [%s]\nPath expected = [%s]\n", chopped_off_expected, full_path_expected);
   assert(!strcmp(chopped_off, chopped_off_expected));
   assert(!strcmp(*full_path, full_path_expected));
}

int main_chops_names_off_paths_noalloc()
{
   printf("\nmain_chops_names_off_paths_noalloc:\n");
   Memory mem = memory_create(0.5 * KB);

   printf("[STATUS] Checking chopping off empty string.\n");
   printf("         Both chopped off name and path should be empty...\n");
   char* full_path = util_string_create(&mem, "", 10);
   assert(!strcmp(full_path, ""));
   char* chopped_off = util_chop_current_name_off_path_noalloc(&full_path);
   assert(!strcmp(chopped_off, ""));
   assert(!strcmp(full_path, ""));

   printf("[STATUS] Now checking full path without path separator [/].\n");
   printf("         Chopped off value should be equal to the original path,\n");
   printf("         while the path itself should be empty...\n");
   full_path = util_string_create(&mem, "!@#\"\%@#^\%&\%*()_\\+nwb\';l\'lbweg", 0);
   assert(!strcmp(full_path, "!@#\"\%@#^\%&\%*()_\\+nwb\';l\'lbweg"));
   chopped_off = util_chop_current_name_off_path_noalloc(&full_path);
   assert(!strcmp(chopped_off, "!@#\"\%@#^\%&\%*()_\\+nwb\';l\'lbweg"));
   assert(!strcmp(full_path, ""));

   printf("[STATUS] Now checking sequentially chopping chunks off the following path:\n");
   printf("         Hekek/a/pretty/long/path/that/doesn't do/anything/in/particular/honestly!\n");
   full_path = util_string_create(&mem, "Hekek/a/pretty/long/path/that/doesn\'t do/anything/in/particular/honestly!", 0);
   assert(!strcmp(full_path, "Hekek/a/pretty/long/path/that/doesn\'t do/anything/in/particular/honestly!"));

   chop_off_single_pass_noalloc(&full_path, "Hekek", 
                        "a/pretty/long/path/that/doesn\'t do/anything/in/particular/honestly!");
   chop_off_single_pass_noalloc(&full_path, "a", 
                        "pretty/long/path/that/doesn\'t do/anything/in/particular/honestly!");
   chop_off_single_pass_noalloc(&full_path, "pretty", 
                        "long/path/that/doesn\'t do/anything/in/particular/honestly!");
   chop_off_single_pass_noalloc(&full_path, "long", 
                        "path/that/doesn\'t do/anything/in/particular/honestly!");
   chop_off_single_pass_noalloc(&full_path, "path", 
                        "that/doesn\'t do/anything/in/particular/honestly!");
   chop_off_single_pass_noalloc(&full_path, "that", 
                        "doesn\'t do/anything/in/particular/honestly!");
   chop_off_single_pass_noalloc(&full_path, "doesn\'t do", 
                        "anything/in/particular/honestly!");
   chop_off_single_pass_noalloc(&full_path, "anything", 
                        "in/particular/honestly!");
   chop_off_single_pass_noalloc(&full_path, "in", 
                        "particular/honestly!");
   chop_off_single_pass_noalloc(&full_path, "particular", 
                        "honestly!");
   chop_off_single_pass_noalloc(&full_path, "honestly!", 
                        "");
   chop_off_single_pass_noalloc(&full_path, "", 
                        "");
   chop_off_single_pass_noalloc(&full_path, "", 
                        "");

   printf("\n[STATUS] All the chopping off seems to have been done correctly!\n");
   memory_usage_status(&mem);
   printf("main_chops_names_off_paths_noalloc: OK\n");
   free(mem.pointer);
   return 0;
}

int main_investigate_double_alloc_bug()
{
   printf("main_investigate_double_alloc_bug:\n");
   Memory mem = memory_create(10);

   memory_usage_status(&mem);
   printf("main_investigate_double_alloc_bug: OK\n");

   free(mem.pointer);
   return 0;
}

int main_inserts_into_path_tree_and_prints_it()
{
   // a gdb print command to see the second element of the tree:
   // print *(struct PathTree*)((*(struct List*)(*(struct PathTree*)tree->children->value)->children)->value)

   printf("\nmain_inserts_into_path_tree_and_prints_it:\n");
   Memory mem = memory_create(10 * KB);

   printf("\n[STATUS] Printing an empty newly created tree...\n");
   struct PathTree* tree = path_tree_create(&mem);
   path_tree_print_verbose(tree);

   printf("\n[STATUS] Inserting to path [c] and printing out...\n");
   path_tree_insert(&mem, tree, "c", "ccc");
   path_tree_print(tree);

   printf("\n[STATUS] Inserting to path [c/a/b] and printing out...\n");
   path_tree_insert(&mem, tree, "c/a/b", "KEKW");
   path_tree_print(tree);

   printf("\n[STATUS] Inserting three nodes:\n");
   printf("           [one/kek],\n");
   printf("           [this/is/a/diversion],\n");
   printf("           [one/two/three], and printing that out...\n");
   printf("[STATUS] node 'one/kek' will be equal to 'w' and node one/two/three to 3...\n"); // '.../diversion' to 'bgg'
   printf("[NOTE] Each will be printed with separate print calls...\n");

   printf("\n[STATUS] Inserting [one/kek] and printing out...\n");
   path_tree_insert(&mem, tree, "one/kek", "w");
   path_tree_print(tree);

   printf("\n[STATUS] Inserting [one/kek/safe] and printing out...\n");
   path_tree_insert(&mem, tree, "one/kek/safe", "safe");
   path_tree_print(tree);

   printf("\n[STATUS] Inserting [this/is/a/diversion] and printing out...\n");
   path_tree_insert(&mem, tree, "this/is/a/diversion", "bgg");
   path_tree_print(tree);

   printf("\n[STATUS] Inserting [one/two/three] and printing out...\n");
   path_tree_insert(&mem, tree, "one/two/three", "3");
   path_tree_print(tree);
   printf("\n");

   printf("\n[STATUS] Trying to insert a value of [2!!] into an existing node [one/two]...\n");
   path_tree_insert(&mem, tree, "one/two", "2!!");
   path_tree_print(tree);
   printf("\n");

   printf("\n[STATUS] Inserting a completely unrelated node [I/am/a/bit/impressed!]...\n");
   path_tree_insert(&mem, tree, "I/am/a/bit/impressed!", "No I am not lol");
   path_tree_print(tree);
   printf("\n");

   printf("\n[STATUS] Inserting a related node [one/two/super/cool/big piece of shit!] now...\n");
   path_tree_insert(&mem, tree, "one/two/super/cool/big piece of shit!", "shit");
   path_tree_print(tree);
   printf("\n");

   printf("\n[STATUS] Inserting a diversion node [Hekek/a/pretty/long/path/that/doesn\'t do/shit] now...\n");
   path_tree_insert(&mem, tree, "Hekek/a/pretty/long/path/that/doesn\'t do/shit", "AHHHAHA11!!");
   path_tree_print(tree);
   printf("\n");

   printf("\n[STATUS] Inserting a gigantic path node [Hekek/a/pretty/long/path/that/doesn\'t do/anything/in/particular/honestly!] now...\n");
   path_tree_insert(&mem, tree, "Hekek/a/pretty/long/path/that/doesn\'t do/anything/in/particular/honestly!", "No I\'m lying lol");
   path_tree_print(tree);
   printf("\n");

   printf("\n[STATUS] Trying to re-insert [Hekek/a/pretty/long/path/that/doesn\'t do/shit] now...\n");
   path_tree_insert(&mem, tree, "Hekek/a/pretty/long/path/that/doesn\'t do/shit", "NEW VAL11!");
   path_tree_print(tree);
   printf("\n");

   printf("\n[STATUS] Trying to insert a value of [WOOW] into an existing node [Hekek/a/pretty]...\n");
   path_tree_insert(&mem, tree, "Hekek/a/pretty", "WOOW");
   path_tree_print(tree);
   printf("\n");

   printf("\n[STATUS] Now trying to print verbosely and see what changes...\n");
   path_tree_print_verbose(tree);
   printf("\n");

   memory_usage_status(&mem);
   printf("main_inserts_into_path_tree_and_prints_it: OK\n");
   free(mem.pointer);
   return 0;
}

struct PathTree* creates_tree_and_inserts_two_nodes(Memory* mem)
{
   printf("\ncreates_tree_and_inserts_two_nodes:\n");

   printf("\n[STATUS] Creating a new tree and inserting [the/first/path] = [1st]...\n");
   struct PathTree* tree = path_tree_create(mem);
   path_tree_insert(mem, tree, "the/first/path", "1st");
   path_tree_print(tree);

   printf("\n[STATUS] Inserting a cheeky node [the/cheeky/path] = [cheekst]...\n");
   path_tree_insert(mem, tree, "the/cheeky/path", "cheekst");
   path_tree_print(tree);

   return tree;
}

int main_tests_path_tree_insertion_and_going_out_of_context()
{
   printf("\nmain_tests_path_tree_insertion_and_going_out_of_context:\n");
   Memory mem = memory_create(1 * KB);

   printf("[STATUS] Calling a separate function to create a tree for me...\n");
   struct PathTree* tree = creates_tree_and_inserts_two_nodes(&mem);

   printf("\n[STATUS] main is printing out a tree it got from the function...\n");
   path_tree_print(tree);

   printf("main_tests_path_tree_insertion_and_going_out_of_context: OK\n");
   memory_usage_status(&mem);
   free(mem.pointer);
   return 0;
}

int main_tests_inserting_empty_values()
{
   printf("\nmain_tests_inserting_empty_values:\n");
   Memory mem = memory_create(10 * KB);

   printf("[STATUS] Inserting three nodes:\n");
   printf("         [the/mighty/kek/of/keks],\n");
   printf("         [the/mighty/artist/of/death],\n");
   printf("         [the/impossible/riddle].\n");
   struct PathTree* tree = path_tree_create(&mem);
   path_tree_insert(&mem, tree, "the/mighty/kek/of/keks", "KEKW");
   path_tree_insert(&mem, tree, "the/mighty/artist/of/death", "Boo!!");
   path_tree_insert(&mem, tree, "the/impossible/riddle", "hmm...");
   printf("[STATUS] Now printing the tree out...\n");
   path_tree_print(tree);

   printf("\n[STATUS] Now updating value of [the/mighty] with NULL...\n");
   path_tree_insert(&mem, tree, "the/mighty", NULL);
   printf("[STATUS] Nothing should change...\n");
   path_tree_print(tree);

   printf("\n[STATUS] Now updating value of [the/mighty] with \"\"...\n");
   path_tree_insert(&mem, tree, "the/mighty", "");
   printf("[STATUS] Should not show [EMPTY] near [the/mighty] now...\n");
   path_tree_print(tree);

   memory_usage_status(&mem);
   printf("main_tests_inserting_empty_values: OK\n\n");
   return 0;
}

int main_tests_trying_to_insert_into_incorrect_paths()
{
   printf("\nmain_tests_corner_cases_of_tree_insertion_and_printing:\n");
   Memory mem = memory_create(10 * KB);
   int return_code = -2;

   printf("[STATUS] Inserting three nodes:\n");
   printf("         [the/mighty/kek/of/keks],\n");
   printf("         [the/mighty/artist/of/death],\n");
   printf("         [the/impossible/riddle].\n");
   struct PathTree* tree = path_tree_create(&mem);
   return_code = path_tree_insert(&mem, tree, "the/mighty/kek/of/keks", "KEKW");
   printf("[STATUS] Return code after first insert = %d\n", return_code);
   return_code = path_tree_insert(&mem, tree, "the/mighty/artist/of/death", "Boo!!");
   printf("[STATUS] Return code after second insert = %d\n", return_code);
   return_code = path_tree_insert(&mem, tree, "the/impossible/riddle", "hmm...");
   printf("[STATUS] Return code after third insert = %d\n", return_code);
   printf("[STATUS] Now printing the tree out...\n");
   path_tree_print(tree);

   printf("\n[STATUS] Trying to insert [/the/mighty]...\n");
   printf("[STATUS] Nothing should change...\n");
   return_code = path_tree_insert(&mem, tree, "/the/mighty", "???");
   assert(return_code == -1);
   path_tree_print(tree);

   printf("\n[STATUS] Trying to insert [the/mighty/]...\n");
   printf("[STATUS] Nothing should change...\n");
   return_code = path_tree_insert(&mem, tree, "the/mighty/", "???");
   assert(return_code == -1);
   path_tree_print(tree);

   printf("\n[STATUS] Trying to insert [//the/mighty]...\n");
   printf("[STATUS] Nothing should change...\n");
   return_code = path_tree_insert(&mem, tree, "//the/mighty", "???");
   assert(return_code == -1);
   path_tree_print(tree);

   printf("\n[STATUS] Trying to insert [the/mighty///]...\n");
   printf("[STATUS] Nothing should change...\n");
   return_code = path_tree_insert(&mem, tree, "the/mighty///", "???");
   assert(return_code == -1);
   path_tree_print(tree);

   printf("\n[STATUS] Trying to insert [the/mighty/kek/of//keks]...\n");
   printf("[STATUS] Nothing should change...\n");
   return_code = path_tree_insert(&mem, tree, "the/mighty/kek/of//keks", "???");
   assert(return_code == -1);
   path_tree_print(tree);

   printf("\n[STATUS] Trying to insert [the//mighty//kek//of//keks]...\n");
   printf("[STATUS] Nothing should change...\n");
   return_code = path_tree_insert(&mem, tree, "the//mighty//kek//of//keks", "???");
   assert(return_code == -1);
   path_tree_print(tree);

   printf("\n[STATUS] Trying to insert [the//mighty/kek/of/keks]...\n");
   printf("[STATUS] Nothing should change...\n");
   return_code = path_tree_insert(&mem, tree, "the//mighty/kek/of/keks", "???");
   assert(return_code == -1);
   path_tree_print(tree);

   printf("\n[STATUS] Trying to insert into empty path...\n");
   printf("[STATUS] Nothing should change...\n");
   return_code = path_tree_insert(&mem, tree, "", "???");
   assert(return_code == -1);
   path_tree_print(tree);

   printf("\n[STATUS] Finally inserting a proper node [the/mighty/kek/of/POGGERS]...\n");
   return_code = path_tree_insert(&mem, tree, "the/mighty/kek/of/POGGERS", ":-O");
   assert(return_code == 0);
   path_tree_print(tree);

   memory_usage_status(&mem);
   printf("main_tests_corner_cases_of_tree_insertion_and_printing: OK\n\n");
   return 0;
}

int main_small_print_verbose_test()
{
   printf("main_small_print_verbose_test:\n");
   Memory mem = memory_create(1 * KB);

   printf("\n[STATUS] Creating a tree and inserting 4 small nodes:\n");
   printf("[a/smol/lil/node], [a/big/lil/node], [a/smol/cool/pp], [a/big/lil/ahaha kek]...\n");
   struct PathTree* tree = path_tree_create(&mem);
   path_tree_insert(&mem, tree, "a/smol/lil/node", "ln");
   path_tree_insert(&mem, tree, "a/big/lil/node", "bn");
   path_tree_insert(&mem, tree, "a/smol/cool/pp", "pp");
   path_tree_insert(&mem, tree, "a/big/lil/ahaha kek", "ak");

   printf("\n[STATUS] Now printing it verbosely...\n");
   path_tree_print_verbose(tree);

   printf("\n[STATUS] Now changing [a/big/lil/ahaha kek]'s value to [KEKW]...\n");
   path_tree_insert(&mem, tree, "a/big/lil/ahaha kek", "KEKW");

   printf("\n[STATUS] Now printing it non-verbosely...\n");
   path_tree_print(tree);

   memory_usage_status(&mem);
   printf("\nmain_small_print_verbose_test: OK\n");
   free(mem.pointer);
   return 0;

}

int main_checks_nonverbose_printing_of_empty_trees()
{
   printf("main_checks_nonverbose_printing_of_empty_trees:\n");
   Memory mem = memory_create(1 * KB);

   printf("\n[STATUS] Creating an empty tree and printing\n");
   printf("         with both verbose and non-verbose prints...\n");
   struct PathTree* tree = path_tree_create(&mem);
   printf("[STATUS] Now printing with a non-verbose print...\n");
   path_tree_print(tree);
   printf("[STATUS] Now printing with a verbose print...\n");
   path_tree_print_verbose(tree);

   printf("\n[STATUS] inserting 4 small nodes:\n");
   printf("[a/smol/lil/node], [a/big/lil/node], [a/smol/cool/pp], [a/big/lil/ahaha kek]...\n");
   printf("[STATUS] All those nodes have a NULL value...\n");
   path_tree_insert(&mem, tree, "a/smol/lil/node", NULL);
   path_tree_insert(&mem, tree, "a/big/lil/node", NULL);
   path_tree_insert(&mem, tree, "a/smol/cool/pp", NULL);
   path_tree_insert(&mem, tree, "a/big/lil/ahaha kek", NULL);

   printf("\n[STATUS] Now printing it verbosely...\n");
   path_tree_print_verbose(tree);

   printf("\n[STATUS] Now printing it non-verbosely...\n");
   path_tree_print(tree);

   memory_usage_status(&mem);
   printf("\nmain_checks_nonverbose_printing_of_empty_trees: OK\n");
   free(mem.pointer);
   return 0;
}

void prints_out_find_status(struct PathTree* tree, char* path, char* expected_name, char* expected_value)
{
   assert(tree);
   assert(path);

   struct PathTree* found = path_tree_find_node_by_path(tree, path);
   if(!found)
      printf("[FIND STATE] Node not found!\n");
   else
   {
      assert(!strcmp(found->node_name, expected_name));
      if(found->node_value)
         assert(!strcmp(found->node_value, expected_value));
      else
         assert(found->node_value == expected_value && !found->node_value);
      printf("[FIND STATE] Found node:\n");
      printf("             %s: [%s]\n", found->node_name, found->node_value);
   }
}

int main/*_finds_nodes_by_path*/()
{
   printf("\nmain_finds_nodes_by_path:\n");
   Memory mem = memory_create(2 * KB);

   printf("\n[STATUS] Creating a tree with 5 nodes...\n");
   struct PathTree* tree = path_tree_create(&mem);
   path_tree_insert(&mem, tree, "m", "W");
   path_tree_insert(&mem, tree, "me", "hello!");
   path_tree_insert(&mem, tree, "me/am/bad", "sry :(");
   path_tree_insert(&mem, tree, "the/actual/what/am/I/doing/right/now", "*shrugs*");
   path_tree_insert(&mem, tree, "the/actual/gibberish/is/going/on/right/now", "no clue ^_^");
   path_tree_insert(&mem, tree, "the/actual/gibberish/mwahaha/very/funny/yes", "I know Kappa");

   path_tree_print(tree);

   printf("\n[STATUS] Trying to find [m]...\n");
   prints_out_find_status(tree, "m", "m", "W");

   printf("\n[STATUS] Trying to find [me]...\n");
   prints_out_find_status(tree, "me", "me", "hello!");

   printf("\n[STATUS] Trying to find [me/am/bad]...\n");
   prints_out_find_status(tree, "me/am/bad", "bad", "sry :(");

   printf("\n[STATUS] Trying to find [me/am]...\n");
   prints_out_find_status(tree, "me/am", "am", NULL);

   printf("\n[STATUS] Trying to find [the/actual/gibberish/mwahaha/very/funny/yes]...\n");
   prints_out_find_status(tree, "the/actual/gibberish/mwahaha/very/funny/yes", "yes", "I know Kappa");

   printf("\n[STATUS] Trying to find [the/actual/gibberish/is/going/on/right/now]...\n");
   prints_out_find_status(tree, "the/actual/gibberish/is/going/on/right/now", "now", "no clue ^_^");

   printf("\n[STATUS] Trying to find [the/actual/what/am/I/doing/right/now]...\n");
   prints_out_find_status(tree, "the/actual/what/am/I/doing/right/now", "now", "*shrugs*");

   printf("\n[STATUS] App memory state: ");
   memory_usage_status(&mem);
   printf("\nmain_finds_nodes_by_path: OK\n");
   free(mem.pointer);
   return 0;
}

int main_tries_to_bork_find_node_by_path()
{
   printf("\nmain_tries_to_bork_find_node_by_path:\n");
   Memory mem = memory_create(2 * KB);

   printf("\n[STATUS] Creating a tree with 5 nodes...\n");
   struct PathTree* tree = path_tree_create(&mem);
   path_tree_insert(&mem, tree, "m", "W");
   path_tree_insert(&mem, tree, "me", "hello!");
   path_tree_insert(&mem, tree, "me/am/bad", "sry :(");
   path_tree_insert(&mem, tree, "the/actual/what/am/I/doing/right/now", "*shrugs*");
   path_tree_insert(&mem, tree, "the/actual/gibberish/is/going/on/right/now", "no clue ^_^");
   path_tree_insert(&mem, tree, "the/actual/gibberish/mwahaha/very/funny/yes", "I know Kappa");

   path_tree_print(tree);

   printf("\n[STATUS] Trying to find \"\". Found node should be NULL...\n");
   assert(!path_tree_find_node_by_path(tree, ""));

   printf("\n[STATUS] Trying to find \"/incorrect/path\". Found node should be NULL...\n");
   assert(!path_tree_find_node_by_path(tree, "/incorrect/path"));

   printf("\n[STATUS] Trying to find \"incorrect/path/\". Found node should be NULL...\n");
   assert(!path_tree_find_node_by_path(tree, "incorrect/path/"));

   printf("\n[STATUS] Trying to find \"incorrect//path\". Found node should be NULL...\n");
   assert(!path_tree_find_node_by_path(tree, "incorrect//path/"));

   printf("\n[STATUS] Trying to find \"///truly///incorrect//path//\". Found node should be NULL...\n");
   assert(!path_tree_find_node_by_path(tree, "incorrect//path/"));

   printf("\n[STATUS] Trying to find a non-existing node [i/am/not/here]. Found node should be NULL...\n");
   assert(!path_tree_find_node_by_path(tree, "i/am/not/here"));

   printf("\n[STATUS] Trying to find an empty node in an empty tree... Found node should be NULL...\n");
   struct PathTree* empty_tree = path_tree_create(&mem);
   assert(!path_tree_find_node_by_path(empty_tree, ""));

   printf("\n[STATUS] Trying to find [some/random/path] in an empty tree... Found node should be NULL...\n");
   assert(!path_tree_find_node_by_path(empty_tree, "some/random/path"));

   printf("\n[STATUS] Trying to find a short [a] node in a tree where it is the only node... Found node should be NULL...\n");
   path_tree_insert(&mem, empty_tree, "a", "K");
   prints_out_find_status(tree, "a", "a", "K");

   printf("\n[STATUS] App memory state: ");
   memory_usage_status(&mem);
   printf("\nmain_tries_to_bork_find_node_by_path: OK\n");
   free(mem.pointer);
   return 0;
}