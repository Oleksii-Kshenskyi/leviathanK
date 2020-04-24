#include "shell.h"

struct List* shell_create_command_list(Memory* memory)
{
   return list_create_empty(memory);
}

void shell_add_command(Memory* memory, struct List* command_list, char* command_name, executor_func executor, execution_result_processor result_processor)
{
   assert(memory);
   assert(command_name);
   assert(executor);
   assert(command_list);

   struct ShellCommand* new_command = memory_allocate(memory, sizeof(struct ShellCommand));
   new_command->command_name = command_name;
   new_command->executor = executor;
   new_command->result_processor = result_processor;

   list_insert_tail(memory, command_list, new_command);
}

static void shell_execute_command(struct ShellCommand* command, void* data)
{
   assert(command);
   assert(command->command_name);
   assert(command->executor);
   assert(command->result_processor);

   command->result_processor(command->executor(data));
}

static int shell_lookup_command_predicate(struct List* list, void* command_name)
{
   assert(list);
   assert(command_name);

   struct ShellCommand* command = list->value;
   char* char_name = command_name;
   return !strcmp(command->command_name, char_name);
}

void shell_lookup_and_execute(struct List* command_list, char* command_name, void* data)
{
   assert(command_list);
   assert(command_name);
   
   struct List* list_command = list_find_first_if(command_list, shell_lookup_command_predicate, data);
   if(!list_command)
   {
      printf("[?] No command %s found.\n", command_name);
      return;
   }
   struct ShellCommand* command = (struct ShellCommand*) list_command;
   shell_execute_command(command, data);
}