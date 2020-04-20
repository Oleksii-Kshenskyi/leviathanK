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

   return (!strcmp(node->node_name, ROOT_NAME) && !node->node_value);
}

static void path_tree_create_path(Memory* memory, struct PathTree* tree, char* path, char* value)
{
   assert(memory);
   assert(tree);
   assert(path);

   if(util_string_is_null_or_empty(path))
   {
      tree->node_value = value;
      return;
   }

   char* new_node_name = util_chop_current_name_off_path_noalloc(
      &path
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

   char* looking_for_this = util_chop_current_name_off_path_noalloc(&buffers->path);
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

static void path_tree_create_new_branch_on_this_node(
   Memory* memory,
   struct PathTree* tree_to_insert_into,
   char* new_node_name
)
{
   assert(memory);
   assert(tree_to_insert_into);
   assert(tree_to_insert_into->children);
   assert(new_node_name);

   struct PathTree* new_node = path_tree_create(memory);
   new_node->node_name = new_node_name;
   new_node->node_value = NULL;
   new_node->children = NULL;
   list_insert_tail(memory, tree_to_insert_into->children, new_node);
}

static int path_tree_is_path_malformed(const char* path)
{
   assert(path);
   size_t path_len = strlen(path);

   if(path[0] == '/' || path[path_len - 1] == '/' || util_string_is_null_or_empty(path))
      return TRUE;

   int successive_separators = 0;
   for(int index = 0; index < path_len; index++)
   {
      successive_separators = (path[index] == '/') ? successive_separators + 1 : 0;
      if(successive_separators > 1)
         return TRUE;
   }

   return FALSE;
}

int path_tree_insert(Memory* memory, struct PathTree* tree, char* path, char* value)
{
   assert(memory);
   assert(path);
   assert(tree);

   if(path_tree_is_path_malformed(path))
      return -1;

   char* original_path = memory_allocate(memory, strlen(path) + 1);
   strcpy(original_path, path);

   if(!tree->children)
   {
      path_tree_create_path(memory, tree, original_path, value);
      return 0;
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
      char* new_node_name = util_chop_current_name_off_path_noalloc(&original_path);
      path_tree_create_new_branch_on_this_node(memory, tree, new_node_name);

      path_tree_create_path(memory, tree->children->tail->value, original_path, value);
   }
   else
      path_tree_create_path(memory, creation_point, buffers.path, value);

   free(throwaway_memory.pointer);
   return 0;
}

struct PrintTreeInternal
{
   Memory* throwaway_memory;
   char* current_path_prefix;
   int verbosity_level;
};

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
      if(buffers->verbosity_level == PRINT_VERBOSE)
         printf("%s: %s [%p]\n", 
                buffers->current_path_prefix, 
                tree_element->node_value,
                tree_element);
      else
         printf("%s: %s\n", 
                buffers->current_path_prefix, 
                tree_element->node_value);
   else
      if(buffers->verbosity_level == PRINT_VERBOSE)
         printf("%s: [EMPTY] [%p]\n", buffers->current_path_prefix, tree_element);

   if(tree_element->children)
      path_tree_print_internal(tree_element, buffers);

   util_unbuild_path_prefix_once(buffers->current_path_prefix);
}

static void path_tree_print_internal(struct PathTree* tree, struct PrintTreeInternal* buffers)
{
   if(tree->children &&
      !buffers->current_path_prefix)
   {
      buffers->current_path_prefix = memory_allocate(
                                       buffers->throwaway_memory,
                                       (THROWAWAY_MEMORY_SIZE_FOR_PRINT)
                                     );

      memset(buffers->current_path_prefix, 0, THROWAWAY_MEMORY_SIZE_FOR_PRINT);
   }

   if(tree->children)
      list_for_each(tree->children, path_tree_print_element, buffers);
}

void path_tree_print_choose_verbosity(struct PathTree* tree, int verbosity)
{
   assert(tree);

   if(path_tree_is_empty(tree))
   {
      if(verbosity == PRINT_VERBOSE)
         printf("%s: [EMPTY]\n", tree->node_name);
      return;
   }

   Memory throwaway_memory = memory_create(THROWAWAY_MEMORY_SIZE_FOR_PRINT);
   struct PrintTreeInternal throwaway_buffers = {
      .throwaway_memory = &throwaway_memory,
      .current_path_prefix = NULL,
      .verbosity_level = verbosity
   };
   path_tree_print_internal(tree, &throwaway_buffers);

   free(throwaway_memory.pointer);
}