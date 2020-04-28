#ifndef _SHELL_H_
#define _SHELL_H_

#include <string.h>

#include "liblevi.h"
#include "command.h"

struct List* shell_create_command_list(struct Memory* memory);
void shell_add_command(struct Memory* memory, struct List* command_list, char* command_name, ExecutorFunc executor, ExecutionResultProcessor result_processor, DataCapsuleCreator data_capsule_creator);
void shell_process_command(struct List* command_list, struct InitialCommandData* command_data);
struct InitialCommandData shell_pack_initial_data(struct Memory* application_memory, char* command_string, struct PathTree* application_tree_root);

#endif