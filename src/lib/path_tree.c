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

static void path_tree_create_path(Memory* memory, struct PathTree* tree, char* path, char* value)
{
   assert(memory);
   assert(tree);
   assert(path);

   if(util_string_is_null_or_empty(path))
      return;

   char* new_node_name = util_chop_current_name_off_path(
      memory, &path
   );
   struct PathTree* new_node = memory_allocate(
      memory, sizeof(struct PathTree)
   );

   new_node->node_name = new_node_name;

   if(!util_string_is_null_or_empty(path))
      new_node->node_value = NULL;
   else
      new_node->node_value = value;

   new_node->children = NULL;
   tree->children = list_create_head(
      memory, (struct PathTree*) new_node
   );

   path_tree_create_path(memory,
                         (struct PathTree*) tree->children->value,
                         path, value
   );
}

static int path_tree_insertion_foreach_callback(struct List* child, void* path_equal_to_this)
{
   struct PathTree* tree_element = (struct PathTree*)child->value;
   return (!strcmp(tree_element->node_name, (char*)path_equal_to_this));
}

static struct PathTree* path_tree_find_starting_point_for_path_creation(
            Memory* application_memory,
            Memory* throwaway_memory,
            struct PathTree* tree,
            char** path_ptr)
{
   assert(application_memory);
   assert(tree);
   assert(path_ptr);
   assert(*path_ptr);

   if(util_string_is_null_or_empty(*path_ptr))
      return NULL;

   if(!throwaway_memory)
      *throwaway_memory = memory_create(strlen(*path_ptr) * 3);

   char* looking_for_this = util_chop_current_name_off_path(throwaway_memory, path_ptr);
   struct List* node_on_correct_path = 
               list_find_first_if(tree->children,
                                  path_tree_insertion_foreach_callback,
                                  looking_for_this);

   if((node_on_correct_path && !((struct PathTree*)node_on_correct_path->value)->children) ||
       node_on_correct_path && !strcmp(*path_ptr, ""))
      return (struct PathTree*) node_on_correct_path->value;

   return (node_on_correct_path) ? 
            path_tree_find_starting_point_for_path_creation(
               application_memory, throwaway_memory, 
               (struct PathTree*)node_on_correct_path->value, 
               path_ptr
            ) :
            NULL;
}

void path_tree_insert(Memory* memory, struct PathTree* tree, char* path, char* value)
{
   assert(memory);
   assert(path);
   assert(tree);
   char* original_path = path;

   if(!tree->children)
   {
      path_tree_create_path(memory, tree, path, value);
      return;
   }

   struct PathTree* creation_point = 
                        path_tree_find_starting_point_for_path_creation(
                              memory, NULL, tree, &path
                        );
   if(!creation_point)
      path_tree_create_path(memory, tree, original_path, value);
   else
      path_tree_create_path(memory, creation_point, path, value);
}

void path_tree_print(struct PathTree* tree)
{
   assert(tree);
}