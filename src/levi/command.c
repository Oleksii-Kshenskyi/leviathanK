#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "command.h"
#include "liblevi.h"

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

struct SetDataCapsule
{
   struct Memory* application_memory;
   struct Memory* throwaway_memory;
   struct PathTree* application_tree_root;
   char* new_node_path;
   char* new_node_value;
   int insert_exit_code;
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

void* command_set_create_data_capsule(struct ShellCommand* command, struct InitialCommandData* initial_data)
{
   assert(command);
   assert(initial_data);

   struct Memory* throwaway_memory = memory_create_heap(1 * KB);
   struct SetDataCapsule* data_capsule = memory_allocate(throwaway_memory, sizeof(struct SetDataCapsule));

   data_capsule->application_memory = initial_data->application_memory;
   data_capsule->application_tree_root = initial_data->application_tree_root;
   data_capsule->throwaway_memory = throwaway_memory;
   data_capsule->insert_exit_code = -2;

   data_capsule->new_node_path = util_string_split_step(&initial_data->command_string, ' ', SPLIT_SKIP_EMPTY);
   if(path_tree_is_path_malformed(data_capsule->new_node_path))
      data_capsule->new_node_path = NULL;

   if(util_string_is_null_or_empty(initial_data->command_string))
      data_capsule->new_node_value = NULL;
   else
      data_capsule->new_node_value = util_string_create(data_capsule->application_memory, initial_data->command_string, 0);

   return data_capsule;
}

void* command_set_execute(void* data_capsule)
{
   struct SetDataCapsule* set_data_capsule = data_capsule;
   if(!set_data_capsule->new_node_path)
      return set_data_capsule;

   set_data_capsule->insert_exit_code = path_tree_insert(
                                          set_data_capsule->application_memory,
                                          set_data_capsule->application_tree_root,
                                          set_data_capsule->new_node_path,
                                          set_data_capsule->new_node_value
                                        );

   return set_data_capsule;
}

void command_set_process_result(void* data_capsule)
{
   struct SetDataCapsule* set_data_capsule = data_capsule;
   if(!set_data_capsule->new_node_path || set_data_capsule->insert_exit_code == -1)
      printf("[SET] The path to the node is malformed. Not setting anything.\n");
   else
      printf("[SET] Node set successfully.\n");

   struct Memory* throwaway_memory = set_data_capsule->throwaway_memory;
   free(throwaway_memory->pointer);
   free(throwaway_memory);
}

void* command_print_create_data_capsule(struct ShellCommand* command, struct InitialCommandData* initial_data)
{
   printf("[DEBUG] [command_print_create_data_capsule()]\n");
}

void* command_print_execute(void* data_capsule)
{
   printf("[DEBUG] [command_print_execute()]\n");
}

void command_print_process_result(void* data_capsule)
{
   printf("[DEBUG] [command_print_process_result()]\n");
}