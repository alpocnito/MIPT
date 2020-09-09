#include "Compil_consts.h"

#include "sort_lines.h"
#include "stack.h"
#include "color.h"

int G_PROGRAM_COMPILATED = 1; // 0 if compilation failed

//-----------------------------------------------------------------------------------
//!
//! Program for Analysing errors 
//!
//! level = 0 - abort
//! level = 1 - not compilated
//! level = 2 - warning
//-----------------------------------------------------------------------------------
void Error(int level, const char *prompt, const char* file_name, int number_line = 0, char* command = "-") {
   assert(level >= 0);
   assert(level <= 3);
   assert(prompt != NULL);

   if (level == 0) {
      PRINT_RED_E(BLINK("%s\n"), prompt);
      
      PRINT_WHITE_E(BOLD("%s\n"), file_name);
      abort();
   }

   if (level == 1) {
      PRINT_RED_E(BOLD("%s\n"), prompt);
      
      PRINT_WHITE_E(BOLD("%s:"), file_name);
      PRINT_WHITE_E(BOLD("%d\n"), number_line);
      
      //append /0 in the end of line
      int i = 0;
      while (command[i] != '\n') i++;
      command[i] = '\0';

      printf("     %s\n", command);
      G_PROGRAM_COMPILATED = 0;
   }
}


//-----------------------------------------------------------------------------------
//!
//! Function check if beginning of the efl file is correct
//!
//-----------------------------------------------------------------------------------
void CheckEflCode(char* buffer, size_t buffer_size, char* file_name) {
   assert(buffer != NULL);
   assert(buffer_size > 0);
    
   //check SIGNA
   if   (buffer[0] != 'I' ||
         buffer[1] != 'R' ||
         buffer[2] != 'O' ||
         buffer[3] != 'N')
         Error(0, "Invalid Signa", file_name);

   //check VERSION
   if (! (buffer[4] == VERSION) )
      Error(0, "Invalid version", file_name);
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
   
   for (int i = len1; i < len1 + len2; ++i)
      str1[i] = str2[i - len1];
}

//-----------------------------------------------------------------------------------
//!
//! Function for interpretate efl command
//!
//-----------------------------------------------------------------------------------
void InterpretateLine(char* line, char* efl_file, char* efl, size_t *total_bytes, int* PC, const char* NAME, int COMMAND_NUMBER) {
   assert(efl_file    != NULL);
   assert(total_bytes != NULL); 
   assert(efl         != NULL);
   assert(NAME        != NULL);
   assert(*PC >= 0);
   
   (*PC)++;                                                                  
   //PUSH 1
   if (COMMAND_NUMBER == 1) {                                                   

      int number_print_bytes = sprintf(line, "%s %lg", NAME, *((Elem_t*)(efl_file + *PC)));  
      *PC += sizeof(Elem_t);                                               
                                                                                     
      ConcantinateStrings(efl, line, *total_bytes, number_print_bytes);   
      *total_bytes += number_print_bytes;                                 
   }            

   //PUSH ax || POP ax
   else if (COMMAND_NUMBER == 101 || COMMAND_NUMBER == 102) {                                                   

      int number_print_bytes = sprintf( line, "%s %c%c", NAME, *((char*)(efl_file + *PC)), *((char*)(efl_file + *PC + 1)) );  
      *PC += LEN_REGISTER_NAME;                                               
                                                                                     
      ConcantinateStrings(efl, line, *total_bytes, number_print_bytes);   
      *total_bytes += number_print_bytes;                                 
   } 
   
   //CALL, JMP, JA, JBE...
   else if (COMMAND_NUMBER >= 9 && COMMAND_NUMBER <= 16) {

      int number_print_bytes = sprintf( line, "%s %d", NAME, *((Label_t*)(efl_file + *PC)));  
      *PC += sizeof(Label_t);                                               
                                                                                     
      ConcantinateStrings(efl, line, *total_bytes, number_print_bytes);   
      *total_bytes += number_print_bytes;                                 
   }

   //Other command
   else 
   {                                                                 
      int number_print_bytes = sprintf(line, "%s", NAME);                
                                                                                     
      ConcantinateStrings(efl, line, *total_bytes, number_print_bytes);   
      *total_bytes += number_print_bytes;                                 
   }                                                                      
                                                                                     
   ConcantinateStrings(efl, "\n", *total_bytes, sizeof(char));            
   (*total_bytes)++;                                                    
}


//-----------------------------------------------------------------------------------
//!
//! Program for interpretate efl code
//!
//-----------------------------------------------------------------------------------
char* InterpretateEfl(size_t* total_bytes, const char* efl_file_name, char* efl_file, size_t efl_file_size) {
   assert(total_bytes   != NULL);
   assert(efl_file_name != NULL);
   assert(efl_file      != NULL);
   assert(efl_file_size > 0);

   //creating ams array with size = number_bytes
   char* ams = (char*)calloc(efl_file_size * MAX_LEN_COMMAND, sizeof(char));
   
   //temp variable for reading one command
   char line[MAX_LEN_COMMAND] = "";      

   int PC = sizeof(unsigned long long) + sizeof(VERSION) + sizeof(SIGNATURE);
  
   #define DEF_CMD(NAME, COMMAND_NUMBER, CODE)                                               \
           else if (efl_file[PC] == char(COMMAND_NUMBER)) {                                  \
                                                                                             \
              InterpretateLine(line, efl_file, ams, total_bytes, &PC, #NAME, COMMAND_NUMBER);\
                                                                                             \
          }                                                                                  \
   
  
   while (PC < efl_file_size) {
      
      
      // CODE
         if (0) ;
            #include "commands.h"
         else
            PC++;
      // CODE


   }

   #undef DEF_CMD

   return ams;
}


//-----------------------------------------------------------------------------------
//!
//! Function for writing program in the ams file
//!
//-----------------------------------------------------------------------------------
void WriteProgram(char* ams_program, size_t program_size, FILE* ams_file, const char* ams_file_name) {
   assert(ams_program != NULL);
   assert(ams_file    != NULL);
   assert(program_size > 0);

   if ( fwrite(ams_program, sizeof(char), program_size, ams_file) != program_size )
      Error(0, "Can not write ams file", ams_file_name);
}

//-----------------------------------------------------------------------------------
//!
//! Efl to ams interpretating function
//!
//-----------------------------------------------------------------------------------
int EflToAms(char* efl_file_name, const char* ams_file_name) {
   assert(ams_file_name != NULL);
   assert(efl_file_name != NULL);

   //open ams file
   FILE* ams_file = NULL;
   if (OpenFile(ams_file_name, "wb", &ams_file))
      Error(0, "Can not open ams file", ams_file_name);

   //Reading efl file
   char* buffer = NULL;
   size_t buffer_size = ReadFile(efl_file_name, &buffer);
   if (buffer == NULL || buffer_size < 1)
      Error(0, "Can not read file", efl_file_name);
   
   CheckEflCode(buffer, buffer_size, efl_file_name);

   //efl -> ams
   size_t program_size  = 0;
   char* ams_program = InterpretateEfl(&program_size, efl_file_name, buffer, buffer_size);
   
   //Write, if all okey
   WriteProgram(ams_program, program_size, ams_file, ams_file_name);
   PRINT_GREEN(BOLD("\nEfl code -> ams succesfull\n"));
      
   free(ams_program);
   fclose(ams_file);
   return 0;
}


 
int main(int args, char* argv[]) {

   if (args < 2) {
      Error(0, "No name of the efl file", "-");
      return 1;
   }

   return EflToAms(argv[1], "ams_from_efl");
}

  
