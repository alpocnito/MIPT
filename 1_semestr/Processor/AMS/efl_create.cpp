#include <Compil_consts.h>
#include <sort_lines.h>
#include <stack.h>
#include <color.h>
#pragma GCC diagnostic ignored "-Wmultichar"

struct Label {
   
   size_t position;
   char* name;
};
int G_COMPILATION_SUCCESSFULL = 1;  // 1 if compilation successfull, 0 if compilation failed



void Error (int level, const char* const prompt, const char* const file_name, int number_line = 0, const char* command = "-");
int ReadCommand(char* line, String* command, char COMMAND_NUMBER, \
                Label* list_labels, int number_labels, char* temp_str);
String FirstCompilation(char* compile_line, int number_line, const char* file_name, Label** list_labels, \
                int* number_labels, size_t current_position_in_program, bool second_compilation = 0);
void WriteProgram(char* efl_program, size_t program_size, FILE* efl_file, const char* efl_file_name);
void PrintCommandInterpretation(String ams, String efl, int number_command);
void ConcantinateStrings(char* str1, char* str2, size_t len1, size_t len2);
void GoThroughLine(char* efl_program, String compile_line_s, int number_line, const char* ams_file_name, \
                   Label** list_labels, int* number_labels, size_t* total_bytes, bool second_compilation = 0);
char* Interpretate(String* list_of_lines, int number_of_lines, size_t* total_bytes, const char* ams_file_name, size_t max_len_program);
int Compile(const char* ams_file_name, const char* efl_file_name);
void DeleteComments(String* list_of_lines, int number_of_lines);





//-----------------------------------------------------------------------------------
//!
//! Program for Analysing errors 
//!
//! level = 0 - abort
//! level = 1 - not compilated
//! level = 2 - warning
//-----------------------------------------------------------------------------------
void Error (int level, const char* const prompt, const char* const file_name, int number_line, const char* command) {
   assert(level >= 0);
   assert(level <= 3);
   assert(prompt != NULL);

   if (level == 0) {
      PRINT_RED_E(BLINK("%s\n"), prompt);
      
      PRINT_WHITE_E(BOLD("%s\n"), file_name);
      abort();
   }

   if (level == 1) {
      
      PRINT_WHITE_E(BOLD("%s:"), file_name);
      PRINT_WHITE_E(BOLD("%d "), number_line);
      PRINT_RED_E(BOLD("%s\n"), prompt);
      

      //append /0 in the end of line
      int i = 0;
      printf("     ");

      while (command[i] != '\n')
        putchar(command[i++]); 
      
      printf("\n");

      G_COMPILATION_SUCCESSFULL = 0;
   }
}

//-----------------------------------------------------------------------------------
//!
//! interpretate one command
//!
//! return 0 - skip command
//! return 1 - All right
//! return 2 - Label not found
//-----------------------------------------------------------------------------------
int ReadCommand(char* line, String* command, char COMMAND_NUMBER, \
                Label* list_labels, int number_labels, char* temp_str) {

   assert(line        != NULL);
   assert(command     != NULL);
   assert(list_labels != NULL);

   //PUSH
   if (COMMAND_NUMBER == 1) {

      //Sscanf variables
      Elem_t push_value = 0; 
      char registe[MAX_LEN_COMMAND] = "";
      
      //if push 3
      if (sscanf(line, "%s" STACK_SYMBOL, temp_str, &push_value) == 2) { 
         command->len = COMMAND_SIZE + sizeof(Elem_t);

         command->str[0] = COMMAND_NUMBER;
         sscanf(command->str + 1, "%lg", &push_value);
         *((Elem_t*)(command->str + 1)) = push_value;                                      
         
         return 1;
      }
      
      //if push ax
      if (sscanf(line, "%s %s", temp_str, registe) == 2) { 
         if (registe[1] == 'x') {
           command->len = COMMAND_SIZE + LEN_REGISTER_NAME - 1;
         
           command->str[0] = char(COMMAND_NUMBER + REGISTER_SHIFT);
           command->str[1] = registe[0];
           command->str[2] = registe[1];
         }
         if (registe[0] == 'v') {
           command->len = COMMAND_SIZE + LEN_REGISTER_NAME;
         
           command->str[0] = char(COMMAND_NUMBER + OPERATIVE_SHIFT);
           command->str[1] = registe[0];
           command->str[2] = registe[1];
           command->str[3] = registe[2];
         }
         return 1;
      }

      return 0;
   } 
   
   //POP
   if (COMMAND_NUMBER == 2) {

      //For sscanf
      char registe[MAX_LEN_COMMAND];
      command->len = COMMAND_SIZE;
      command->str[0] = COMMAND_NUMBER ;


      if (sscanf(line, "%s %s", temp_str, registe) == 2) { 
         if (registe[1] == 'x') {
           command->len = COMMAND_SIZE + LEN_REGISTER_NAME - 1;
         
           command->str[0] = char(COMMAND_NUMBER + REGISTER_SHIFT);
           command->str[1] = registe[0];
           command->str[2] = registe[1];
         }
         if (registe[0] == 'v') {
           command->len = COMMAND_SIZE + LEN_REGISTER_NAME;
         
           command->str[0] = char(COMMAND_NUMBER + OPERATIVE_SHIFT);
           command->str[1] = registe[0];
           command->str[2] = registe[1];
           command->str[3] = registe[2];
         }
         return 1;
      }
   }

   //CALL, JMP, JA, JB, JAE...
   if (COMMAND_NUMBER >= 9 && COMMAND_NUMBER <= 16) {
      
      char label[MAX_LEN_LABEL] = "";
      if (sscanf(line, "%s %s", temp_str, label) != 2)
         return 0;
      
      command->len = COMMAND_SIZE + sizeof(Label_t);
      
      //search label in the list_labels
      for (int i = 0; i < number_labels; ++i)
         if (strcmp(label, list_labels[i].name) == 0) {

            *(size_t*)(command->str + 1) = list_labels[i].position;
            command->str[0] = COMMAND_NUMBER;
            return 1;
         }
      
      //label is not found
      //*(int*)(command->str + 1) = -1;
      command->str[0] = COMMAND_NUMBER;
      return 2;
   }
    
   //Other functions
   command->len = COMMAND_SIZE;
   command->str[0] = COMMAND_NUMBER;
   
   return 1;
}

//-----------------------------------------------------------------------------------
//!
//! Interpretating one line in the ams
//!
//-----------------------------------------------------------------------------------
String FirstCompilation(char* compile_line, int number_line, const char* file_name, Label** list_labels, \
                        int* number_labels, size_t current_position_in_program, bool second_compilation) {
   assert(compile_line != NULL);
   assert(file_name    != NULL);
   assert(list_labels  != NULL);

   //our command
   String command;
   command.str = (char*)calloc(MAX_LEN_COMMAND + MAX_LEN_LABEL, sizeof(char));
   
   char* temp_str = (char*)calloc(MAX_LEN_COMMAND, sizeof(char));
   if (sscanf(compile_line, "%s", temp_str) != 1) {
      Error(1, "Empty line", file_name, number_line);
      command.str = NULL;
      return command;
   }

   #define DEF_CMD(NAME, COMMAND_NUMBER, CODE)                                        \
           if (strcasecmp(temp_str, #NAME) == 0) {                                    \
                                                                                      \
               int state = ReadCommand(compile_line, &command, COMMAND_NUMBER,        \
                                       *list_labels, *number_labels, temp_str);       \
                                                                                      \
               if ( state == 0 ) {                                                    \
                  Error(1, "Invalid second ardument", file_name,                      \
                        number_line, compile_line);                                   \
                  command.str = NULL;                                                 \
               }                                                                      \
               if ( state == 2 && second_compilation ) {                              \
                  Error(1, "Undefined label", file_name,                              \
                        number_line, compile_line);                                   \
                  command.str = NULL;                                                 \
               }                                                                      \
               free(temp_str);                                                        \
               return command;                                                        \
                                                                                      \
           }                                                                          \
   
  
   // CODE
      #include "commands.h"
   // CODE


   #undef DEF_CMD
         
   //Analyse label
   if (second_compilation && temp_str[0] == ':') {
      free(temp_str);
      free(command.str);
      command.str = NULL;
      return command;
   }
   if (temp_str[0] == ':') {
     
      if (sscanf(compile_line, "%s", temp_str) != 1) { 
         Error(1, "Invalid label", file_name, number_line, compile_line);
         free(temp_str);
         free(command.str);
         command.str = NULL;
         return command;
      }

      (*list_labels)[*number_labels].name     = temp_str + 1;
      
      (*list_labels)[*number_labels].position = current_position_in_program + sizeof(SIGNATURE) + sizeof(VERSION);

      (*number_labels)++;
      
      free(command.str);
      command.str = NULL;
      return command;
   }

   free(temp_str);
   free(command.str);
   //function doesn't return sth
   Error(1, "Unknown command", file_name, number_line, compile_line);
   command.str = NULL;
   return command;
}

//-----------------------------------------------------------------------------------
//!
//! My function for concatinating lines
//!
//-----------------------------------------------------------------------------------
void ConcantinateStrings(char* str1, char* str2, size_t len1, size_t len2) {
   assert(str1 != NULL);
   assert(str2 != NULL);
   assert(len1 >= 0);
   assert(len2 > 0);

   for (size_t i = len1; i < len1 + len2; ++i)
      str1[i] = str2[i - len1];
}

//-----------------------------------------------------------------------------------
//!
//! For Debug. Prints all ams lines with machine interpretation
//!
//-----------------------------------------------------------------------------------
void PrintCommandInterpretation(String ams, String efl, int number_command) {
   assert(ams.str != NULL);
   assert(efl.str != NULL);
   
   printf("%2d: ", number_command);
   unsigned int it = 0;
   for (it = 0; it < ams.len; ++it)
      printf("%c", ams.str[it]);

   for (it = it; it < MAX_LEN_COMMAND; ++it)
      printf(" ");
   
   for (it = 0; it < efl.len; ++it)
      printf("%02X ", (unsigned char) efl.str[it]);
   printf("\n");
}

//-----------------------------------------------------------------------------------
//!
//! Remove comments from ams
//!
//-----------------------------------------------------------------------------------
void DeleteComments(String* list_of_lines, int number_of_lines) {
   assert(list_of_lines != NULL);
   assert(number_of_lines > 0);

   for (int i = 0; i < number_of_lines; ++i)

      for (size_t j = 0; j < list_of_lines[i].len; ++j)
         if (list_of_lines[i].str[j] == ';') {

            list_of_lines[i].len = j;
            break;
         }

}

void GoThroughLine(char* efl_program, String compile_line_s, int number_line, const char* ams_file_name, \
                   Label** list_labels, int* number_labels, size_t* total_bytes, bool second_compilation) { 
    
   char* compile_line = compile_line_s.str;

   if (compile_line[0] == ';') return;
   String command = FirstCompilation(compile_line, number_line, ams_file_name, \
                                     list_labels, number_labels, *total_bytes, second_compilation);
   if (command.str == NULL) return;
    
   if (second_compilation == 1) PrintCommandInterpretation(compile_line_s, command, number_line);

   *total_bytes += command.len;
   ConcantinateStrings(efl_program, command.str, *total_bytes - command.len, command.len);

   free(command.str);
}
 
//-----------------------------------------------------------------------------------
//!
//! Program for interpretate ams code
//!
//-----------------------------------------------------------------------------------
char* Interpretate(String* list_of_lines, int number_of_lines, size_t* total_bytes, const char* ams_file_name, size_t max_len_program) {
   assert(list_of_lines   != NULL);
   assert(total_bytes     != NULL);
   assert(ams_file_name   != NULL);
   assert(max_len_program > 0);
   assert(number_of_lines > 0);
   
   DeleteComments(list_of_lines, number_of_lines);

   //creating program array with size = number_bytes
   char* efl_program  = (char*)calloc(max_len_program, sizeof(char));
   *total_bytes       = 0;
   
   //for labels
   Label* list_labels = (Label*)calloc(MAX_AMOUNT_LABELS, sizeof(char)*MAX_LEN_LABEL + sizeof(list_labels->position));
   int number_labels  = 0;
   

   //First Compilation
   for (int i = 0; i < number_of_lines; ++i) {
      GoThroughLine(efl_program, list_of_lines[i], i, ams_file_name, \
                    &list_labels, &number_labels, total_bytes, 0);
   }

   //Second compilation
   *total_bytes = 0;
   for (int i = 0; i < number_of_lines; ++i) {
      GoThroughLine(efl_program, list_of_lines[i], i, ams_file_name, \
                    &list_labels, &number_labels, total_bytes, 1);
   }
    
   for (int i = 0; i < number_labels; ++i)
    free(list_labels[i].name - 1);

   free(list_labels);
   return efl_program;
}


//-----------------------------------------------------------------------------------
//!
//! Function for writing program in the efl file
//!
//-----------------------------------------------------------------------------------
void WriteProgram(char* efl_program, size_t program_size, FILE* efl_file, const char* efl_file_name) {
   assert(efl_program   != NULL);
   assert(efl_file      != NULL);
   assert(efl_file_name != NULL);
   
   size_t signa_size = sizeof(VERSION) + sizeof(SIGNATURE);
   
   //All machine file
   char* all_program = (char*) calloc(signa_size + program_size, sizeof(char));
   all_program[0] = 'I';
   all_program[1] = 'R';
   all_program[2] = 'O';
   all_program[3] = 'N';

   //VERSION
   all_program[4] = 1;
   
   ConcantinateStrings(all_program, efl_program, signa_size, program_size);
   
   //WRTING
   if ( fwrite(all_program, sizeof(char), program_size + signa_size, efl_file) != signa_size + program_size )
      Error(0, "Can not write efl file", efl_file_name);
   free(all_program);
}

//-----------------------------------------------------------------------------------
//!
//! Main compilation program
//!
//-----------------------------------------------------------------------------------
int Compile(const char* ams_file_name, const char* efl_file_name) {
   assert(ams_file_name != NULL);
   assert(efl_file_name != NULL);

   //open efl file
   FILE* efl_file = NULL;
   if (OpenFile(efl_file_name, "wb", &efl_file))
      Error(0, "Can not open efl file", efl_file_name);

   //Reading ams file
   char* buffer = NULL;
   size_t buffer_size = ReadFile(ams_file_name, &buffer);
   if (buffer == NULL || buffer_size < 1)
      Error(0, "Can not read file", ams_file_name);
   
   //Separating lines
   String* list_of_lines = NULL;
   size_t number_of_lines = SeparateLines(buffer, buffer_size, &list_of_lines);
   if (number_of_lines < 1)
      Error(0, "File is empty", ams_file_name);

   //ams -> machine
   size_t program_size  = 0;
   char* efl_program = Interpretate(list_of_lines, (int)number_of_lines, &program_size, ams_file_name, buffer_size);
   free(list_of_lines);

   free(buffer);
   //Write, if all okey
   if (G_COMPILATION_SUCCESSFULL == 1) {
      WriteProgram(efl_program, program_size, efl_file, efl_file_name);
      free(efl_program);
      PRINT_GREEN(BOLD("Compilation succesfull\n\n"));
      
      fclose(efl_file);
      return 0;
   } 
   else {
      PRINT_RED(REVERSE("Not compilated\n\n"));
      free(efl_program);

      fclose(efl_file);
      return -1;
   }
}


 
int main(int args, char* argv[]) {

   if (args < 3) {
      Error(0, "No name of the compile file", "-");
      return 1;
   }

   return Compile(argv[1], argv[2]);
}  
