#ifndef _PATH_TREE_H_
#define _PATH_TREE_H_

#include "list.h"
#include "memory.h"
#include "util.h"

#define ROOT_NAME "[ROOT]"
#define THROWAWAY_MEMORY_SIZE_FOR_PRINT 10 * KB

struct PathTree
{
   char* node_name;
   char* node_value;
   struct List* children;
};

extern struct PathTree* path_tree_create(Memory* memory);
extern int path_tree_is_empty(struct PathTree* tree);
extern void path_tree_insert(Memory* memory, struct PathTree* tree, char* path, char* value);
extern void path_tree_print(struct PathTree* tree);

#endif