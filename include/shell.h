#ifndef _SHELL_H_
#define _SHELL_H_

#include <string.h>

#include "liblevi.h"

typedef void* (*executor_func)(void*);
typedef void (*execution_result_processor)(void*);

struct ShellCommand
{
   char* command_name;
   executor_func executor;
   execution_result_processor result_processor;
};

struct List* shell_create_command_list(Memory* memory);
void shell_add_command(Memory* memory, struct List* command_list, char* command_name, executor_func executor, execution_result_processor result_processor);
void shell_lookup_and_execute(struct List* command_list, char* command_name, void* data);

#endif