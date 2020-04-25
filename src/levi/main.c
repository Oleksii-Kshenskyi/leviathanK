#include <stdio.h>
#include <string.h>

#include "liblevi.h"

void main_levi_shell_loop(Memory* application_memory)
{
   while(TRUE)
   {
      Memory throwaway_memory = memory_create(1 * KB);
      printf("|LEVI|> ");
      char* command_string = memory_allocate(&throwaway_memory, 1 * KB);
      size_t zero = 0;
      getline(&command_string, &zero, stdin);
      command_string[strlen(command_string) - 1] = '\0';
      printf("[DEBUG] Your argument string is [%s]\n", command_string);
      printf("[DEBUG] String lentgh is [%u]\n", strlen(command_string));
      printf("[DEBUG] (strlen - 1)'th character is [%d]\n", command_string[strlen(command_string) - 1]);
      free(throwaway_memory.pointer);
   }
}

int main()
{
   Memory application_memory = memory_create(1 * MB);
   main_levi_shell_loop(&application_memory);
}