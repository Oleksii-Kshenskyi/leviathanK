#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "memory.h"

struct ShellCommand;
struct InitialCommandData;

typedef void* (*ExecutorFunc)(void*);
typedef void (*ExecutionResultProcessor)(void*);
typedef void* (*DataCapsuleCreator)(struct ShellCommand *command, struct InitialCommandData* data);

struct ShellCommand
{
   char* command_name;
   ExecutorFunc executor;
   ExecutionResultProcessor result_processor;
   DataCapsuleCreator data_capsule_creator;
};

struct InitialCommandData
{
   struct Memory* application_memory;
   const char* original_getline;
   char* command_string;
};

void* command_test_create_data_capsule(struct ShellCommand* command, struct InitialCommandData* initial_data);
void* command_test_execute(void* data_capsule);
void command_test_process_result(void* data_capsule);

void* command_exit_create_data_capsule(struct ShellCommand* command, struct InitialCommandData* initial_data);
void* command_exit_execute(void* data_capsule);
void command_exit_process_result(void* data_capsule);

void* command_set_create_data_capsule(struct ShellCommand* command, struct InitialCommandData* initial_data);
void* command_set_execute(void* data_capsule);
void command_set_process_result(void* data_capsule);

#endif