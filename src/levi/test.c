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

int main/*_creates_root_path_tree_node*/()
{
   printf("\nmain_creates_root_path_tree_node:\n");
   printf("[STATUS] Allocating memory and creating path tree root...\n");
   Memory mem = memory_create(0.5 * KB);
   struct PathTree* root = path_tree_create(&mem);

   printf("[STATUS] Making sure root fields have expected values...\n");
   assert(root->children == NULL);
   assert(root->node_value == NULL);
   assert(!strcmp(root->node_name, ROOT_NAME));

   printf("[STATUS] Path tree root seems to have been created properly!\n");
   printf("main_creates_root_path_tree_node: OK\n");
   free(mem.pointer);
   return 0;
}