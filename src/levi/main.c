#include <stdio.h>
#include <string.h>

#include "liblevi.h"
#include "shell.h"

void print_command(struct List* element, void* data)
{
   printf("%s", ((struct ShellCommand*)element->value)->command_name);
   if(element->next)
      printf(", ");
}

void print_available_commands(struct List* command_list)
{
   printf("[");
   list_for_each(command_list, print_command, NULL);
   printf("]");
}

void main_levi_shell_loop(struct List* command_list, struct Memory* application_memory)
{
   while(TRUE)
   {
      printf("|LEVI|> ");
      char* command_string = NULL;
      size_t zero = 0;
      getline(&command_string, &zero, stdin);
      command_string[strlen(command_string) - 1] = '\0';
      
      struct InitialCommandData data = shell_pack_initial_data(application_memory, command_string);
      shell_process_command(command_list, &data);

      free(command_string);
   }
}

int main()
{
   printf("Welcome to leviathanK, a compact path tree explorer!\n");
   printf("Current version is: [0.6.0]\n");

   struct Memory application_memory = memory_create(1 * MB);
   struct List* command_list = shell_create_command_list(&application_memory);

   printf("Available commands: ");
   print_available_commands(command_list);
   printf("\n");

   main_levi_shell_loop(command_list, &application_memory);

}