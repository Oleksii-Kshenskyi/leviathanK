#include <string.h>
#include <assert.h>

#include "path_tree.h"

struct PathTree* path_tree_create(Memory* memory)
{
   assert(memory);

   struct PathTree* root = memory_allocate(memory, sizeof(struct PathTree));
   root->children = NULL;
   root->node_value = NULL;
   root->node_name = memory_allocate(memory, strlen(ROOT_NAME + 1));
   strcpy(root->node_name, ROOT_NAME);

   return root;
}

void path_tree_insert(Memory* memory, char* path, char* value)
{
   assert(memory);
   assert(path);
}

void path_tree_print(struct PathTree* tree)
{
   assert(tree);
}