#ifndef _PATH_TREE_H_
#define _PATH_TREE_H_

#include "list.h"
#include "memory.h"
#include "util.h"

#define ROOT_NAME "[ROOT]"
#define THROWAWAY_MEMORY_SIZE_FOR_PRINT 3 * KB
#define PRINT_VERBOSE TRUE
#define PRINT_NONVERBOSE FALSE

struct PathTree
{
   char* node_name;
   char* node_value;
   struct List* children;
};

extern struct PathTree* path_tree_create(Memory* memory);
extern int path_tree_is_empty(struct PathTree* tree);
extern int path_tree_insert(Memory* memory, struct PathTree* tree, char* path, char* value);

extern void path_tree_print_choose_verbosity(struct PathTree* tree, int verbosity);
static inline void path_tree_print(struct PathTree* tree)
{
   path_tree_print_choose_verbosity(tree, PRINT_NONVERBOSE);
}
static inline void path_tree_print_verbose(struct PathTree* tree)
{
   path_tree_print_choose_verbosity(tree, PRINT_VERBOSE);
}

extern struct PathTree* path_tree_find_node_by_path(struct PathTree* tree, char* path);
extern void path_tree_find_and_print_node(struct PathTree* node, char* path);

#endif