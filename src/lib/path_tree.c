#include <string.h>
#include <assert.h>

#include "path_tree.h"

struct PathTree* path_tree_create(Memory* memory)
{
   assert(memory);

   struct PathTree* root = memory_allocate(memory, sizeof(struct PathTree));
   root->children = NULL;
   root->node_value = NULL;
   root->node_name = memory_allocate(memory, strlen(ROOT_NAME) + 1);
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

   new_node->node_name = util_string_create(memory, new_node_name, 0);

   if(!util_string_is_null_or_empty(path))
      new_node->node_value = NULL;
   else
      new_node->node_value = value;

   new_node->children = NULL;

   if(!tree->children)
      tree->children = list_create_head(
         memory, (struct PathTree*) new_node
      );
   else if(!list_is_empty(tree->children))
      list_insert_tail(memory, tree->children, new_node);

   path_tree_create_path(memory,
                         (struct PathTree*) tree->children->tail->value,
                         path, value
   );
}

static int path_tree_insertion_foreach_callback(struct List* child, void* path_equal_to_this)
{
   struct PathTree* tree_element = (struct PathTree*)child->value;
   return (!strcmp(tree_element->node_name, (char*)path_equal_to_this));
}

struct CreationPointInternal
{
   Memory* throwaway_memory;
   char* creation_point;
   char* path;
   char* scanned_path;
};

static struct PathTree* path_tree_find_starting_point_for_path_creation(
            Memory* application_memory,
            struct CreationPointInternal* buffers,
            struct PathTree* tree)
{
   assert(application_memory);
   assert(buffers);
   assert(tree);
   assert(buffers->path);

   if(util_string_is_null_or_empty(buffers->path))
      return NULL;

   char* looking_for_this = util_chop_current_name_off_path(buffers->throwaway_memory, &buffers->path);
   util_build_path_prefix_noalloc(&buffers->scanned_path, looking_for_this);

   struct List* node_on_correct_path = 
               list_find_first_if(tree->children,
                                  path_tree_insertion_foreach_callback,
                                  looking_for_this);

   if(!buffers->creation_point)
   {
      buffers->creation_point = util_string_create(buffers->throwaway_memory, "", 0.2 * buffers->throwaway_memory->capacity);
      strcpy(buffers->creation_point, "");
   }

   if(node_on_correct_path)
      util_build_path_prefix_noalloc(&buffers->creation_point, ((struct PathTree*)node_on_correct_path->value)->node_name);
   else if(!util_string_is_null_or_empty(buffers->creation_point) && util_check_unbuilds_to_equal_nonempty(buffers->scanned_path, buffers->creation_point))
   {
      strcpy(buffers->creation_point, buffers->scanned_path);
      char* last_name_on_scanned_path = util_chop_off_last_name_from_path_noalloc(buffers->scanned_path);
      buffers->path = util_build_path_prefix(buffers->throwaway_memory, last_name_on_scanned_path, buffers->path);
      return tree;
   }

   if(!node_on_correct_path && 
      !util_string_is_null_or_empty(buffers->creation_point) && 
      util_check_unbuilds_to_equal_nonempty(buffers->scanned_path, buffers->creation_point))
   {
      return tree;
   }

   if((node_on_correct_path && !((struct PathTree*)node_on_correct_path->value)->children) ||
       node_on_correct_path && !strcmp(buffers->path, ""))
      return (struct PathTree*) node_on_correct_path->value;

   return (node_on_correct_path) ? 
            path_tree_find_starting_point_for_path_creation(
               application_memory, buffers, 
               (struct PathTree*)node_on_correct_path->value
            ) :
            NULL;
}

void path_tree_insert(Memory* memory, struct PathTree* tree, char* path, char* value)
{
   assert(memory);
   assert(path);
   assert(tree);
   char* original_path = memory_allocate(memory, strlen(path) + 1);
   strcpy(original_path, path);

   if(!tree->children)
   {
      path_tree_create_path(memory, tree, path, value);
      return;
   }

   Memory throwaway_memory = memory_create(strlen(path) * 5);
   struct CreationPointInternal buffers = {
      .throwaway_memory = &throwaway_memory,
      .creation_point = NULL,
      .path = NULL,
      .scanned_path = NULL
   };
   buffers.path = memory_allocate(&throwaway_memory, strlen(path) + 1);
   buffers.scanned_path = memory_allocate(&throwaway_memory, strlen(path) + 1);
   strcpy(buffers.path, path);
   strcpy(buffers.scanned_path, "");
   struct PathTree* creation_point = 
                        path_tree_find_starting_point_for_path_creation(
                              memory, &buffers, tree
                        );
   if(!creation_point)
   {
      // TODO: extract this into a separate function
      // call it something like "path_tree_create_new_branch_on_this_node()"
      struct PathTree* new_node = path_tree_create(memory);
      new_node->node_name = util_chop_current_name_off_path(memory, &original_path);
      new_node->node_value = NULL;
      new_node->children = NULL;
      list_insert_tail(memory, tree->children, new_node);
      // end of path_tree_create_new_branch_on_this_node()

      path_tree_create_path(memory, tree->children->tail->value, original_path, value);
   }
   else
      path_tree_create_path(memory, creation_point, buffers.path, value);

   free(throwaway_memory.pointer);
}

// TODO: remove copy buffer from the code
struct PrintTreeInternal
{
   Memory* throwaway_memory;
   char* current_path_prefix;
   char* copy_buffer;
};

static int path_tree_are_internal_print_bufs_null(struct PrintTreeInternal* bufs)
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
      printf("%s: %s [%p]\n", 
             buffers->current_path_prefix, 
             tree_element->node_value,
             tree_element);
   else
      printf("%s: [EMPTY] [%p]\n", buffers->current_path_prefix, tree_element);

   if(tree_element->children)
      path_tree_print_internal(tree_element, buffers);

   util_unbuild_path_prefix_once(buffers->current_path_prefix);
}

static void path_tree_print_internal(struct PathTree* tree, struct PrintTreeInternal* buffers)
{
   if(tree->children &&
      path_tree_are_internal_print_bufs_null(buffers))
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