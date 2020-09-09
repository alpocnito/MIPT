#include "Compil_consts.h"

#include "sort_lines.h"
#include "stack.h"
#include "color.h"

enum ERRORS {  
   INVALID_SIGNATURE = 1,
   INVALID_VERSION   = 2,
   INVALID_SIZE      = 3,
};


//---------------------------------------------------------------------
//
// Main CPU structure
//
//---------------------------------------------------------------------
struct CPU {
   Elem_t ax;
   Elem_t bx;
   Elem_t cx;
   Elem_t dx;
   
   double* operativ;

   Stack_t stack;
};

//---------------------------------------------------------------------
//
// The begin of MachineCode file: signa + version
//
//---------------------------------------------------------------------
#pragma pack(1)
struct MachineCode {
   int signature;
   char version;
};


void ExecutionError(const char* prompt, const char* file_name, int number_line, CPU cpu, int PC = 0) {
   
   PRINT_RED_E(BOLD("%s "), prompt);

   PRINT_WHITE_E(BOLD("%s:"), file_name);
   PRINT_WHITE_E(BOLD("%d\n"), number_line);
   
   printf("%d: \nax: " STACK_SYMBOL "\nbx: " STACK_SYMBOL "\ncx: " STACK_SYMBOL "\ndx: " STACK_SYMBOL "\n", \
          PC, cpu.ax, cpu.bx, cpu.cx, cpu.dx);
   PrintStack(&(cpu.stack));
   
   printf("---------------------------------------------\n");
   for (int i = 0; i < SIZE_OPERATIVE; ++i)
     printf("var_%d: %lf\n", i, cpu.operativ[i]);

   abort();
}


int CheckProgram(char* buffer, size_t buffer_size) {
   assert(buffer != NULL);
   assert(buffer_size >= 5);
   
   MachineCode *machine_code = (MachineCode*)(buffer);

   if ( machine_code->signature != SIGNATURE ) 
      return INVALID_SIGNATURE;

   if ( machine_code->version != VERSION )
      return INVALID_VERSION;
   
   if ( buffer_size == sizeof(MachineCode) )
      return INVALID_SIZE;

   return 0;
}

void ReadProgramError(int state) {
   assert(state >= 0);

   switch (state) {

      case 0:
         perror("No errors\n");
         break;

      case INVALID_SIGNATURE:
         perror("Invalid signature in the begin of file\n");
         break;

      case INVALID_VERSION:
         perror("Invalid version\n");
         break;

      case INVALID_SIZE:
         perror("Program is empty\n");
         break;

      default:
         perror("Invalid state of command\n");
         break;

   }
}

void AnalyseProgram(char* buffer, size_t buffer_size, const char* file_name) {
   assert(buffer != NULL);
   assert(file_name != NULL);
   assert(buffer_size >= 0);

   CPU cpu = {};
   cpu.operativ = (double*)calloc(SIZE_OPERATIVE, sizeof(double));
   StackInit(&(cpu.stack));

   size_t PC = sizeof(SIGNATURE) + sizeof(VERSION); 
   int number_line = 1;

   #define DEF_CMD(NAME, COMMAND_NUM, CODE)      \
     case COMMAND_NUM:                           \
         PC += COMMAND_SIZE;                     \
         CODE;                                   \
         number_line++;                          \
         break;                                  \


   while (PC < buffer_size - 1) {
      switch (buffer[PC]) {
      // CODE
      #include "commands.h"
      // CODE
      
      default:
         PRINT_RED_E( N("Unknown: %d\n"), buffer[PC]);
         PC++;
         break;
      }
    }


   #undef DEF_CMD
   
   free(cpu.operativ);
   StackDisinit(&(cpu.stack));
}


int main(int argc, char* argv[]) {
   
   if (argc < 2) {
      perror("No name of executable file\n");
      return 1;
   }
   char*  buffer = NULL;
   size_t buffer_size = ReadFile(argv[1], &buffer);

   if (buffer_size <= 0) {
      perror("Can not read file or it's empty\n");
      return 1;
   }

   int state = CheckProgram(buffer, buffer_size);
    
   if (state == 0) AnalyseProgram(buffer, buffer_size, argv[1]);
   else ReadProgramError(state);
   
   free(buffer);
   return 0;
}  
 
