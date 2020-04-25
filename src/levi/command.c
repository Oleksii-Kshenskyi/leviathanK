#include <string.h>
#include <assert.h>

#include "command.h"
#include "util.h"

struct TestDataCapsule
{
   char* test_argument;
   struct Memory* throwaway_memory;
   char* command_output;
};

void* command_test_create_data_capsule(struct ShellCommand* command, struct InitialCommandData* initial_data)
{
   assert(command);
   assert(initial_data);

   size_t arg_len = strlen(initial_data->command_string);
   size_t output_size = 20 + arg_len;

   struct Memory* throwaway_memory = memory_create_heap(output_size + sizeof(struct TestDataCapsule) + 120);
   struct TestDataCapsule* capsule = memory_allocate(throwaway_memory, sizeof(struct TestDataCapsule));

   if(!strcmp(initial_data->command_string, ""))
      capsule->test_argument = NULL;
   else 
      capsule->test_argument = util_string_create(throwaway_memory, initial_data->command_string, 0);
   capsule->throwaway_memory = throwaway_memory;
   capsule->command_output = memory_allocate(throwaway_memory, output_size);
   strcpy(capsule->command_output, "");

   return capsule;
}

void* command_test_execute(void* data_capsule)
{
   struct TestDataCapsule* test_data_capsule = data_capsule;
   if(test_data_capsule->test_argument)
      sprintf(test_data_capsule->command_output, "[TEST] Argument provided: [%s]\n", test_data_capsule->test_argument);
   else
      sprintf(test_data_capsule->command_output, "[TEST] No argument provided.\n");
   return data_capsule;
}

void command_test_process_result(void* data_capsule)
{
   struct TestDataCapsule* test_data_capsule = data_capsule;

   printf(test_data_capsule->command_output);

   free(test_data_capsule->throwaway_memory->pointer);
   free(test_data_capsule->throwaway_memory);
}