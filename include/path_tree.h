#ifndef _PATH_TREE_H_
#define _PATH_TREE_H_

#include "list.h"
#include "memory.h"
#include "util.h"

#define ROOT_NAME "[ROOT]"

struct PathTree
{
   char* node_name;
   char* node_value;
   struct List* children;
};

extern struct PathTree* path_tree_create(Memory* memory);
extern void path_tree_insert(Memory* memory, char* path, char* value);
extern void path_tree_print(struct PathTree* tree);

#endif