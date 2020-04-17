#ifndef _UTIL_H_
#define _UTIL_H_

#include <assert.h>

#include "memory.h"

// C string utility functions
extern char* util_string_create(Memory* memory, char* cstr_value, size_t buffer_size);
extern int util_string_is_null_or_empty(char* cstr);

// path tree utility functions
extern char* util_build_path_prefix(Memory* memory, char* old_path, char* new_name);
extern void util_build_path_prefix_noalloc(char** old_path_ptr, char* new_name);
extern void util_unbuild_path_prefix_once(char* path);
extern char* util_chop_current_name_off_path(Memory* memory, char** path_ptr);

#endif