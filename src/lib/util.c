#include <string.h>

#include "util.h"

int util_string_is_null_or_empty(const char* cstr)
{
   return !cstr || !strcmp(cstr, "");
}

extern char* util_string_create(struct Memory* memory, char* cstr_value, size_t buffer_size)
{
   assert(memory);
   assert(cstr_value);
   if(buffer_size > 0)
      assert(buffer_size >= strlen(cstr_value) + 1);

   char* created = memory_allocate(memory, (buffer_size) ?  buffer_size : (strlen(cstr_value) + 1));
   strcpy(created, cstr_value);

   return created;
}

char* util_build_path_prefix(struct Memory* memory, char* old_path, char* new_name)
{
   assert(memory);
   assert(old_path);
   assert(new_name);

   if(util_string_is_null_or_empty(old_path) &&
      util_string_is_null_or_empty(new_name))
      return util_string_create(memory, "", 0);
   else if (util_string_is_null_or_empty(old_path))
      return util_string_create(memory, new_name, 0);
   else if (util_string_is_null_or_empty(new_name))
      return util_string_create(memory, old_path, 0);

   int result_length = strlen(old_path) + strlen(new_name) + 2;
   char* result = util_string_create(memory, old_path, result_length);
   strcat(result, "/");
   strcat(result, new_name);

   return result;
}

void util_build_path_prefix_noalloc(char** old_path_ptr, char* new_name)
{
   assert(old_path_ptr);
   assert(*old_path_ptr);
   assert(new_name);

   if(util_string_is_null_or_empty(*old_path_ptr) && 
      util_string_is_null_or_empty(new_name))
      return;
   else if (util_string_is_null_or_empty(*old_path_ptr))
   {
      strcat(*old_path_ptr, new_name);
      return;
   }
   else if (util_string_is_null_or_empty(new_name))
      return;

   strcat(*old_path_ptr, "/");
   strcat(*old_path_ptr, new_name);
}

void util_unbuild_path_prefix_once(char* path)
{
   assert(path);

   char* last_separator = strrchr(path, '/');
   if(last_separator)
      *last_separator = '\0';
   else
      *path = '\0';
}

int util_check_unbuilds_to_equal_nonempty(char* path_to_check, char* equals_to)
{
   assert(path_to_check);
   assert(equals_to);

   if(util_string_is_null_or_empty(equals_to))
      return 0;

   char null_char = '\0';

   char* last_separator = strrchr(path_to_check, '/');
   if(!last_separator)
      return 0;

   *last_separator = '\0';
   int unbuilds_to_equal = !strcmp(path_to_check, equals_to);
   *last_separator = '/';

   return unbuilds_to_equal;
}

char* util_chop_current_name_off_path_noalloc(char** path_ptr)
{
   assert(path_ptr);
   assert(*path_ptr);
   char* original_path = *path_ptr;

   char* first_separator = strchr(original_path, '/');
   if(!first_separator)
   {
      *path_ptr += strlen(*path_ptr);
      return original_path;
   }
   *first_separator = '\0';
   *path_ptr = first_separator + 1;
   
   return original_path;
}

static char* util_string_split_step_keep_empty(char** string_ptr, char separator)
{
   char* original_string = *string_ptr;

   char* next_token = strchr(original_string, separator);
   if(!next_token)
   {
      *string_ptr += strlen(*string_ptr);
      return original_string;
   }
   *next_token = '\0';
   *string_ptr = next_token + 1;
   
   return original_string;
}

static void util_string_fast_forward_to_next_non_char(char** string_ptr, char char_to_ff)
{
   while(**string_ptr == char_to_ff)
   {
      **string_ptr = '\0';
      *string_ptr += 1;
   }
}

static char* util_string_split_step_skip_empty(char** string_ptr, char separator)
{
   util_string_fast_forward_to_next_non_char(string_ptr, separator);

   char* token_start = *string_ptr;
   char* next_separator = strchr(token_start, separator);
   if(!next_separator)
   {
      *string_ptr += strlen(*string_ptr);
      return token_start;
   }
   *string_ptr = next_separator;
   util_string_fast_forward_to_next_non_char(string_ptr, separator);
   return token_start;
}

char* util_string_split_step(char** string_ptr, char separator, int skip_empty)
{
   assert(string_ptr);
   assert(*string_ptr);
   assert(skip_empty == 0 || skip_empty == 1);

   if(skip_empty == SPLIT_KEEP_EMPTY)
      return util_string_split_step_keep_empty(string_ptr, separator);
   else if(skip_empty == SPLIT_SKIP_EMPTY)
      return util_string_split_step_skip_empty(string_ptr, separator);
}

char* util_chop_off_last_name_from_path_noalloc(char* path)
{
   assert(path);

   char* last_separator = strrchr(path, '/');
   return (last_separator) ? last_separator + 1 : path;
}