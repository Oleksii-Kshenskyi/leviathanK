#include <string.h>
#include <assert.h>

#include "command.h"
#include "util.h"

#define TEST_NO_ARGUMENT_RESPONSE "[TEST] No argument provided.\n"
#define TEST_RESPONSE_WITH_ARGUMENT "[TEST] Argument provided: [%s]\n"

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

   unsigned short is_command_string_empty = !strcmp(initial_data->command_string, "");
   size_t output_size = strlen(initial_data->command_string) + strlen(is_command_string_empty ? TEST_NO_ARGUMENT_RESPONSE : TEST_RESPONSE_WITH_ARGUMENT);
   size_t memory_size = output_size * 2 + sizeof(struct TestDataCapsule);

   struct Memory* throwaway_memory = memory_create_heap(memory_size);
   struct TestDataCapsule* capsule = memory_allocate(throwaway_memory, sizeof(struct TestDataCapsule));

   if(is_command_string_empty)
      capsule->test_argument = NULL;
   else 
      capsule->test_argument = util_string_create(throwaway_memory, initial_data->command_string, 0);
   capsule->throwaway_memory = throwaway_memory;
   capsule->command_output = util_string_create(throwaway_memory, "", output_size);

   return capsule;
}

void* command_test_execute(void* data_capsule)
{
   struct TestDataCapsule* test_data_capsule = data_capsule;
   if(test_data_capsule->test_argument)
      sprintf(test_data_capsule->command_output, TEST_RESPONSE_WITH_ARGUMENT, test_data_capsule->test_argument);
   else
      sprintf(test_data_capsule->command_output, TEST_NO_ARGUMENT_RESPONSE);
   return data_capsule;
}

void command_test_process_result(void* data_capsule)
{
   struct TestDataCapsule* test_data_capsule = data_capsule;

   printf(test_data_capsule->command_output);

   struct Memory* memory = test_data_capsule->throwaway_memory;
   free(memory->pointer);
   free(memory);
}