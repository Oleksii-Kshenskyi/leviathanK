#include <string.h>
#include <assert.h>
#include <stdlib.h>

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

struct ExitDataCapsule
{
   const char* original_getline;
   struct Memory* application_memory;
   struct Memory* throwaway_memory;
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

void* command_exit_create_data_capsule(struct ShellCommand* command, struct InitialCommandData* initial_data)
{
   assert(command);
   assert(initial_data);

   struct Memory* throwaway_memory = memory_create_heap(sizeof(struct ExitDataCapsule));
   struct ExitDataCapsule* capsule = memory_allocate(throwaway_memory, sizeof(struct ExitDataCapsule));

   capsule->throwaway_memory = throwaway_memory;
   capsule->original_getline = initial_data->original_getline;
   capsule->application_memory = initial_data->application_memory;
}

void* command_exit_execute(void* data_capsule)
{
   return data_capsule;
}

void command_exit_process_result(void* data_capsule)
{
   struct ExitDataCapsule* exit_data_capsule = data_capsule;
   printf("[EXIT] Application memory usage on exit:\n\t");
   memory_usage_status(exit_data_capsule->application_memory);
   printf("[EXIT] Freeing all memory and exiting...\n");
   printf("[LEVI] Bye!\n");

   struct Memory* own_memory = exit_data_capsule->throwaway_memory;
   const char* original_getline = exit_data_capsule->original_getline;
   struct Memory* app_memory = exit_data_capsule->application_memory;

   free(own_memory->pointer);
   free(own_memory);
   free((void*)original_getline);
   free(app_memory->pointer);
   exit(0);
}