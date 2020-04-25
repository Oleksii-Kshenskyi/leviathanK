#ifndef _UTIL_H_
#define _UTIL_H_

#include <assert.h>

#include "memory.h"

#define SPLIT_SKIP_EMPTY 1
#define SPLIT_KEEP_EMPTY 0

// C string utility functions
extern char* util_string_create(struct Memory* memory, char* cstr_value, size_t buffer_size);
extern int util_string_is_null_or_empty(const char* cstr);
extern char* util_string_split_step(char** string_ptr, char separator, int skip_empty);

// path tree utility functions
extern char* util_build_path_prefix(struct Memory* memory, char* old_path, char* new_name);
extern void util_build_path_prefix_noalloc(char** old_path_ptr, char* new_name);
extern void util_unbuild_path_prefix_once(char* path);
extern int util_check_unbuilds_to_equal_nonempty(char* path_to_check, char* equals_to);
extern char* util_chop_current_name_off_path_noalloc(char** path_ptr);
extern char* util_chop_off_last_name_from_path_noalloc(char* path);

#endif