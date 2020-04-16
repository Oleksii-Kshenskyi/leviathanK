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

int path_tree_is_empty(struct PathTree* tree)
{
   assert(tree);

   return (!tree->children && !strcmp(tree->node_name, ROOT_NAME));
}

int path_tree_is_root_node(struct PathTree* node)
{
   assert(node);

   return (!strcmp(node->node_name, ROOT_NAME));
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

struct PrintTreeInternal
{
   Memory* throwaway_memory;
   char* current_path_prefix;
   char* copy_buffer;
};

static int path_tree_are_internal_print_bufs_empty(struct PrintTreeInternal* bufs)
{
   return !bufs->copy_buffer && !bufs->current_path_prefix;
}

static void path_tree_print_internal(struct PathTree* tree, struct PrintTreeInternal* buffers);

static void path_tree_print_element(struct List* element, void* data)
{
   assert(element);

   struct PathTree* tree_element = (struct PathTree*) element->value;
   struct PrintTreeInternal* buffers = (struct PrintTreeInternal*) data;

   if(!tree_element)
      return;

   assert(tree_element->node_name);
   util_build_path_prefix_noalloc(&buffers->current_path_prefix, 
                                  tree_element->node_name);
   if(tree_element->node_value)
      printf("%s: %s\n", 
             buffers->current_path_prefix, 
             tree_element->node_value);
   else
      printf("%s: [EMPTY]\n", tree_element->node_name);

   if(tree_element->children)
      path_tree_print_internal((struct PathTree*)tree_element->children->head->value, buffers);
}

static void path_tree_print_internal(struct PathTree* tree, struct PrintTreeInternal* buffers)
{
   if(tree->children &&
      path_tree_are_internal_print_bufs_empty(buffers))
   {
      buffers->current_path_prefix = memory_allocate(
                                       buffers->throwaway_memory,
                                       (THROWAWAY_MEMORY_SIZE_FOR_PRINT / 2 - 1)
                                     );
      buffers->copy_buffer = memory_allocate(
                               buffers->throwaway_memory,
                               (THROWAWAY_MEMORY_SIZE_FOR_PRINT / 2 - 1)
                             );
      memset(buffers->copy_buffer, 0, THROWAWAY_MEMORY_SIZE_FOR_PRINT / 2 - 1);
      memset(buffers->current_path_prefix, 0, THROWAWAY_MEMORY_SIZE_FOR_PRINT / 2 - 1);
   }

   if(tree->children)
      list_for_each(tree->children, path_tree_print_element, buffers);
}

void path_tree_print(struct PathTree* tree)
{
   assert(tree);

   if(path_tree_is_empty(tree))
   {
      printf("%s: [EMPTY]\n", tree->node_name);
      return;
   }

   Memory throwaway_memory = memory_create(THROWAWAY_MEMORY_SIZE_FOR_PRINT);
   struct PrintTreeInternal throwaway_buffers = {
      .throwaway_memory = &throwaway_memory,
      .current_path_prefix = NULL,
      .copy_buffer = NULL
   };
   path_tree_print_internal(tree, &throwaway_buffers);

   free(throwaway_memory.pointer);
}