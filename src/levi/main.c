#include <stdio.h>
#include <string.h>

#include "liblevi.h"
#include "shell.h"

void print_element(struct List* element, void* data)
{
   if(!element->value)
      printf("[DEBUG] command_name = [EMPTY]\n");
   else
      printf("[DEBUG] command_name = %s\n", ((struct ShellCommand*)element->value)->command_name);
}

void main_levi_shell_loop(struct Memory* application_memory)
{
   struct List* command_list = shell_create_command_list(application_memory);
   list_for_each(command_list, print_element, NULL);
   while(TRUE)
   {
      struct Memory throwaway_memory = memory_create(1 * KB);
      printf("|LEVI|> ");
      char* command_string = memory_allocate(&throwaway_memory, 1 * KB);
      size_t zero = 0;
      getline(&command_string, &zero, stdin);
      command_string[strlen(command_string) - 1] = '\0';
      
      struct InitialCommandData data = shell_pack_initial_data(application_memory, command_string);
      shell_process_command(command_list, &data);

      free(throwaway_memory.pointer);
   }
}

int main()
{
   printf("Welcome to leviathanK, a compact path tree explorer!\n");
   printf("Current version is: [0.5.0]\n");
   printf("Available commands: [TO BE IMPLEMENTED]\n");
   struct Memory application_memory = memory_create(1 * MB);
   main_levi_shell_loop(&application_memory);
}