#include "../Stack/stack.cpp"
#include "../SortLines/sort_lines.cpp"
#include <string.h>

const unsigned int MAX_LEN_COMMAND = 1000;

int Compute_line(Stack_t* stack, String line, unsigned int number_of_line, char* file_name);
unsigned int ExecuteProgram (char* file_name, Elem_t* answer);
bool AnalyseLine(String line, Elem_t* value, char command[MAX_LEN_COMMAND], unsigned int number_of_line, const char* file_name);
bool AddCommand(Stack_t* stack, Elem_t* result);
void InvalidCommand(char command[MAX_LEN_COMMAND], unsigned int number_of_line, const char* file_name);
void CommandError(const char* prompt, unsigned int number_of_line, const char* file_name);


int main() {
   char* file_name = "prog";
   
   Elem_t answer = 0;
   unsigned int done_commands = ExecuteProgram(file_name, &answer);

   printf("Done commands: %d\n", done_commands);
   printf("Answer: %d\n", answer);

   return 0;
}

unsigned int ExecuteProgram(char* file_name, Elem_t* answer) {
   assert(file_name != NULL);
   assert(answer != NULL);
   
   //Reading file
   char* buffer = NULL;
   int buffer_size = ReadFile(file_name, &buffer);
   if (buffer == NULL || buffer_size < 1) {
      perror("Problem with reading file\n");
      return 1;
   }
   
   //Separating lines
   String* list_of_lines = NULL;
   int number_of_lines = SeparateLines(buffer, buffer_size, &list_of_lines);
   if (number_of_lines < 1) {
      perror("Empty file\n");
      return 1;
   }
   
   //Creating stack
   Stack_t stack = {};
   StackInit(&stack);
   
   //do program
   unsigned int done_commands = 0;
   for (int i = 0; i < number_of_lines; ++i) {
      int status = Compute_line(&stack, list_of_lines[i], i, file_name);

      if (status == 1)
         done_commands++;
      
      if (status == 2)
         break;
   }
   
   //giving answer
   if (stack.size < 1) {
      *answer = 0;
      printf("No answer, stack is empty\n");
      return done_commands;
   }
   else {
      bool error = 0;
      *answer = StackPop(&stack, &error);
      
      if (error == 1)
         printf("some problems with popping an answer\n");

      return done_commands;
   }
}

int Compute_line(Stack_t* stack, String line, unsigned int number_of_line, char* file_name) {
   assert(stack != NULL);
   assert(file_name != NULL);
   assert(line.str != NULL);

   
   Elem_t value = 0;
   char command[MAX_LEN_COMMAND] = {};
   
   //anslysing line
   int analyse_error = AnalyseLine(line, &value, command, number_of_line, file_name);
   if (analyse_error)
      return 0;
      
   //analysing command
   if (strcmp(command, "end") == 0) {
      return 2;
   }

   if (strcmp(command, "push") == 0) {
      if (StackPush(stack, value))
         return 1;
      else {
         CommandError("Can not push elements in the stack", number_of_line, file_name);
         return 0;
      }
   }

   if (strcmp(command, "pop") == 0) {
      bool error = 0;

      StackPop(stack, &error);
      if (error == 2) {
         CommandError("No elements to pop", number_of_line, file_name);
         return 0;
      }
      if (error == 2) {
         CommandError("No elements to pop", number_of_line, file_name);
         return 0;
      }  
      
      return 1;
   }

   if (strcmp(command, "add") == 0) {
      Elem_t result = 0;
            
      if (AddCommand(stack, &result)) {
         StackPush(stack, result);
         return 1;
      }
      else {
         CommandError("Can not add elements, stack is empty", number_of_line, file_name);
         return 0;
      }
         
   }
   

   InvalidCommand(command, number_of_line + 1, file_name);
   return 0;
}
 


bool AnalyseLine(String line, Elem_t* value, char command[MAX_LEN_COMMAND], unsigned int number_of_line, const char* file_name) {
   assert(value != NULL);
   assert(line.str != NULL);
   assert(line.len > 0);
   
   line.str[line.len] = '\0';
   
   if (sscanf(line.str, "%s", command) != 1) {
      InvalidCommand(command, number_of_line + 1, file_name);
      return 1;
   }

   if (strcmp("push", command) == 0)
      if (sscanf(line.str, "%s %d", command, value) != 2) {
         InvalidCommand(command, number_of_line + 1, file_name);
         return 1;
      }

   return 0;
}


bool AddCommand(Stack_t* stack, Elem_t* result) {
   assert(stack != NULL);
   
   bool error = 0;
   Elem_t first_term = StackPop(stack, &error);
   if (error) {
      return 0;
   }

   Elem_t second_term = StackPop(stack, &error);
   if (error) {
      return 0;
   }
   
   *result = first_term + second_term;
   return 1;
   
}

void InvalidCommand(char command[MAX_LEN_COMMAND], unsigned int number_of_line, const char* file_name) {
   assert(file_name != NULL);
   assert(command != NULL);

   printf("\033[1;31mError:\033[0m Invalid command in ");
   printf("\033[1;1m%s:%d\033[0m\n", file_name, number_of_line);
   printf("   %s\n\n", command);
}


void CommandError(const char* prompt, unsigned int number_of_line, const char* file_name) {
   assert(prompt != NULL);
   assert(file_name != NULL);

   printf("\033[1;31mError:\033[0m %s \033[1;1m%s:%d\033[0m\n", prompt, file_name, number_of_line);

}
