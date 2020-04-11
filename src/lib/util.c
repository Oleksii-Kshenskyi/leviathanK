#include <string.h>

#include "util.h"

int util_string_is_null_or_empty(char* cstr)
{
   return !cstr || !strcmp(cstr, "");
}

extern char* util_string_create(Memory* memory, char* cstr_value, size_t buffer_size)
{
   assert(memory);
   assert(cstr_value);
   if(buffer_size > 0)
      assert(buffer_size >= strlen(cstr_value) + 1);

   char* created = memory_allocate(memory, (buffer_size) ?  buffer_size : (strlen(cstr_value) + 1));
   strcpy(created, cstr_value);

   return created;
}

char* util_build_path_prefix(Memory* memory, char* old_path, char* new_name)
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