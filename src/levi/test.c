#include "liblevi.h"
#include <string.h>

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