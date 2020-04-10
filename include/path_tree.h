#ifndef _PATH_TREE_H_
#define _PATH_TREE_H_

#include "list.h"
#include "memory.h"

#define ROOT_NAME "[ROOT]"

struct PathTree
{
   char* node_name;
   void* node_value;
   struct List* children;
};

extern struct PathTree* path_tree_create(Memory* memory);

#endif