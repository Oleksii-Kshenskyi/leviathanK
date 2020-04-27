#include "shell.h"
#include "command.h"
#include "util.h"

struct List* shell_create_command_list(struct Memory* memory)
{
   struct List* command_list = list_create_empty(memory);

   shell_add_command(memory, command_list, "test", command_test_execute, command_test_process_result, command_test_create_data_capsule);
   shell_add_command(memory, command_list, "exit", command_exit_execute, command_exit_process_result, command_exit_create_data_capsule);

   return command_list;
}

void shell_add_command(struct Memory* memory, struct List* command_list, char* command_name, ExecutorFunc executor, ExecutionResultProcessor result_processor, DataCapsuleCreator data_capsule_creator)
{
   assert(memory);
   assert(command_name);
   assert(executor);
   assert(command_list);

   struct ShellCommand* new_command = memory_allocate(memory, sizeof(struct ShellCommand));
   new_command->command_name = command_name;
   new_command->executor = executor;
   new_command->result_processor = result_processor;
   new_command->data_capsule_creator = data_capsule_creator;

   list_insert_tail(memory, command_list, new_command);
}

static void shell_execute_command(struct ShellCommand* command, struct InitialCommandData* data)
{
   assert(command);
   assert(command->command_name);
   assert(command->executor);
   assert(command->result_processor);
   assert(command->data_capsule_creator);

   command->result_processor(command->executor(command->data_capsule_creator(command, data)));
}

static int shell_lookup_command_predicate(struct List* list, void* command_name)
{
   assert(list);
   assert(command_name);

   struct ShellCommand* command = list->value;
   char* char_name = command_name;
   return !strcmp(command->command_name, char_name);
}

void shell_lookup_and_execute(struct List* command_list, char* command_name, struct InitialCommandData* data)
{
   assert(command_list);
   assert(command_name);
   
   struct List* list_command = list_find_first_if(command_list, shell_lookup_command_predicate, command_name);
   if(!list_command)
   {
      if(strcmp(command_name, ""))
         printf("[?] No command [%s] found.\n", command_name);
      return;
   }
   struct ShellCommand* command = (struct ShellCommand*) list_command->value;
   shell_execute_command(command, data);
}

void shell_process_command(struct List* command_list, struct InitialCommandData* command_data)
{
   assert(command_data);
   assert(command_data->application_memory);
   assert(command_data->command_string);
 
   char* command_name = util_string_split_step(&command_data->command_string, ' ', SPLIT_SKIP_EMPTY);
   shell_lookup_and_execute(command_list, command_name, command_data);
}


struct InitialCommandData shell_pack_initial_data(struct Memory* application_memory, char* command_string)
{
   assert(application_memory);
   assert(command_string);

   return (struct InitialCommandData) {
      .application_memory = application_memory,
      .original_getline = (const char*) command_string,
      .command_string = command_string
   };
}