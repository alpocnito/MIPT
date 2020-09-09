#include <elf.h>
#include <assert.h>
#include <stdio.h> 
#include <stdlib.h>
#include <sort_lines.h>
#include <Compil_consts.h>
#include <vector>
#include <iostream>

#define verify(COND) \
  if (!(COND)) {     \
    printf("Failed %s on %s:%s\n", #COND, __FILE__, __LINE__);\
    exit(0);         \
  }

const size_t MAX_PROGRAM_SIZE = 10000;
const size_t PROGRAM_START = 0x00400000;
const size_t FIRST_AMS_VAR = 0x61; 

//------------------------ my stdlib functions ------------------------
const size_t PUT_function  = 3;
const size_t GET_function  = 1;
const size_t ROOT_function = 5;
//---------------------------------------------------------------------

enum ERRORS {  
   INVALID_SIGNATURE = 1,
   INVALID_VERSION   = 2,
   INVALID_SIZE      = 3,
};

//---------------------------------------------------------------------
//
//! The begin of MachineCode file: signa + version
//
//---------------------------------------------------------------------
#pragma pack(1)
struct MachineCode {
   int signature;
   char version;
};
//---------------------------------------------------------------------


//---------------------------------------------------------------------
//
//! For jmp's
//
//---------------------------------------------------------------------
struct Fixup {
  size_t x86_location;
  size_t ams_location;
  char*  spot_for_write;
  size_t jmp_instruction_size;
};

struct Fixup_Table {
  size_t ams_location; //!< operand location in ams
  size_t x86_location; //!< operand location in x86
};
//---------------------------------------------------------------------




#pragma pack(1)

#define B1 __uint8_t
#define B2 __uint16_t
#define B4 __uint32_t
#define B8 __uint64_t

#define PUT(HEADER) \
        for (unsigned int i = 0; i < sizeof(HEADER); ++i) { \
          verify(fputc(((char*)&HEADER)[i], file) != EOF); \
        }

struct ELF_Header {
  B4 ei_MAG         = 0x464C457F;       // signature
  B1 ei_CLASS       = ELFCLASS64;       // 64 bit format
  B1 ei_DATA        = ELFDATA2LSB;      // little endian
  B4 ei_VERSION     = 0x00000001;       // signature
  B2 ei_OSABI       = ELFOSABI_NONE;    // UNIX System V ABI
  B4 ei_OSABIVER    = 0x00000000;       // signature
  B2 E_TYPE         = ET_EXEC;          // executable file
  B2 E_MACHINE      = EM_X86_64;        // x86_64 AMD
  B4 E_VERSION      = EV_CURRENT;       // signature
  B8 E_ENTRY        = 0x0000000000400080; // entry point
  B8 E_PHOFF        = 0x0000000000000040; // start pf program header table
  B8 E_SHOFF        = 0x0000000000000000; // start of the section header table
  B4 E_FLAGS        = 0x00000000; // signature
  B2 E_EHSIZE       = 0x0040;     // size of this header
  B2 E_PHENTSIZE    = 0x0038;     // size of program header table 
  B2 E_PHNUM        = 0x0001;     // number of entries in the progtam header file
  B2 E_SHENTSIZE    = 0x0040;     // size of the section header table
  B2 E_SHNUM        = 0x0005;     // number of entries in the section header table
  B2 E_SHSTRNDX     = 0x0004;     // section header, that contains section names
};

struct Program_Header {
  B4 P_TYPE    = 0x00000000 | PT_LOAD;     // Segment will loaded in memory
  B4 P_FLAGS   = 0x00000000 | PF_R | PF_X; // Read and Execute
  B8 P_OFFSET  = 0x0000000000000000;       // Offset where it should be read
  B8 P_VADDR   = 0x0000000000400000;       // Virtual address where it should be loaded
  B8 P_PADDR   = 0x0000000000400000;       // Phsical address where it should be loaded
  B8 P_FILESZ  = 0x0000000000000080;       // Size on file
  B8 P_MEMSZ   = 0x0000000000000080;       // Size in memory (useless)
  B8 P_ALIGN   = 0x0000000000200000;       // P_VADDR = P_OFFSET % P_ALIGN ???
  B8 zeoroz    = 0x0000000000000000;
};



//=====================================================================
//!
//! Put in the ELF file headers:
//!    ELF_Header
//!    Program_Header
//!
//! @param [in] file file-place where headers write
//! @param [in] prog_size prog_size-size of write program
//!
//! @return 1, if all is OK
//=====================================================================
int AppendHeaders(FILE* file, const size_t prog_size) {
  assert(file);

  ELF_Header elf_h;
  
  Program_Header prgm_h;
    prgm_h.P_FILESZ += prog_size;
    prgm_h.P_MEMSZ += prog_size;
  PUT(elf_h);
  PUT(prgm_h);

  return 1;
} 


//=====================================================================
//!
//! Put my simple printf and scanf in the ELF file
//!
//! @param [in] file file-place where headers write
//! @param [in] stdlib_size stdlib_size-size of write program
//!
//! @return stdlib_size, if all is OK
//=====================================================================
int AppendProgramPutGet(FILE* file, const size_t stdlib_size) {
  assert(file);

  // reading my_stdlib
  FILE* my_stdlib = fopen("communication_funcs/my_stdlib", "rb");
  if (!my_stdlib) {
    printf("my_stdlib can not be opened\n");
    return 0;
  }
  
  B1* prog = (B1*)calloc(stdlib_size, sizeof(prog[0]));
  
  if (fread(prog, stdlib_size, 1, my_stdlib) != 1) {
    printf("fread don't read all file\n");
    return 0;
  }

  // writing my_stdlib
  for (unsigned int i = 0; i < stdlib_size; ++i) {
    verify(fputc(prog[i], file) != EOF);
  }
  
  free(prog);
  return stdlib_size;
}


//=====================================================================
//!
//! Put my main program in the ELF file
//!
//! @param [in] file file-place where headers write
//! @param [in] x86_p x86_p-pointer to the x86 program
//! @param [in] prog_size prog_size-size of x86_p buffer
//! @param [in] stdlib_size stdlib_size-size of my_stdlib program
//!
//! @return 1, if all is OK
//=====================================================================
int AppendProgram(FILE* file, const char* x86_p, \
                  const size_t prog_size, const size_t stdlib_size) {
  assert(file);
  assert(x86_p);
  assert(prog_size > 0);

  size_t write_size = AppendProgramPutGet(file, stdlib_size);
  assert(write_size);
  
  verify(fwrite(x86_p, prog_size, 1, file) != 0);
  
  return 1;
}

//=====================================================================
//!
//! Check AMS program for header, version and other
//!
//! @param [in] buffer buffer-pointer to program
//! @param [in] buffer_size buffer_size-size of program
//!
//! @return 1, if all is OK
//=====================================================================
int CheckProgram(char* buffer, size_t buffer_size) {
   assert(buffer != NULL);
   assert(buffer_size >= 1);
   
   MachineCode *machine_code = (MachineCode*)(buffer);

   if ( buffer_size <= sizeof(MachineCode) )
      return INVALID_SIZE;

   if ( machine_code->signature != SIGNATURE ) 
      return INVALID_SIGNATURE;

   if ( machine_code->version != VERSION )
      return INVALID_VERSION;

   return 0;
}

//=====================================================================
//!
//! Catcher of CheckProgram(char* buffer, size_t buffer_size) errors
//!
//=====================================================================
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

//=====================================================================
//!
//! The main translation program. Transalte from AMS to x86
//!
//! @param [out] output output-x86 program
//! @param [in] input input-ams program
//! @param [in] input_size input_size-size of ams program 
//! @param [in] stdlib_size stdlib_size-size of my_stdlib program
//!
//! @return x86_c, if all is OK
//=====================================================================
size_t TranslateProgram(char* output, char* input, \
                        size_t input_size, const size_t stdlib_size) {
  assert(output);
  assert(input);
  assert(input_size > 0);
  
  // reading ams program
  int state = CheckProgram(input, input_size);
  if (state != 0) ReadProgramError(state);
  
  size_t ams_c = sizeof(SIGNATURE) + sizeof(VERSION); // counter from ams
  size_t x86_c = 0;                                   // counter from x86

  size_t program = sizeof(ELF_Header) + sizeof(Program_Header) + PROGRAM_START;
  size_t offset = program + stdlib_size;    

  std::vector<Fixup>       fixups;
  std::vector<Fixup_Table> fixup_table; // bound (x86 command in x86 file) with (ams command in ams file)


  ////////////////////////////////////TRANSLATION////////////////////////////////
  #define SYMBOL(NAME, COMMAND_NUM, CODE)     \
    case COMMAND_NUM:                         \
      ams_c += COMMAND_SIZE;                  \
      CODE;                                   \
      break;                                  \
  
  while (ams_c < input_size - 1) {
    
    fixup_table.push_back({ams_c, x86_c});

    switch (input[ams_c]) {

    // CODE
    #include "alphabet.h"
    // CODE
      
    default:
      printf("Unknown program: %d on %d\n", input[ams_c], ams_c); 
      ams_c++;
      break;
    }
  }
  ////////////////////////////////////////////////////////////////////////////////

  // fill empty jump's
  for (int i = 0; i < fixups.size(); ++i) {
    int j = 0;
    while (fixup_table[j].ams_location != fixups[i].ams_location)
      ++j;
    
    *(int*)(fixups[i].spot_for_write) = fixup_table[j].x86_location - 
                                        fixups[i].x86_location - 
                                        fixups[i].jmp_instruction_size;
  }

  return x86_c;
}

//=====================================================================
//! Small function, that returns my_stdlib file size
//=====================================================================
size_t GetStdlibSize() {
  FILE* my_stdlib = fopen("communication_funcs/my_stdlib", "rb");
  if (!my_stdlib) {
    printf("my_stdlib can not be opened\n");
    return 0;
  }
  fseek(my_stdlib, 0L, SEEK_END);
  size_t stdlib_size = ftell(my_stdlib);
  fseek(my_stdlib, 0L, SEEK_SET);
  fclose(my_stdlib);

  return stdlib_size;
}


int main(int argc, char* argv[]) {

  if (argc != 2) {
    printf("Run me with ams file!\n");
    return 1;
  }
  
  char* ams_p = NULL;
  size_t ams_s = ReadFile(argv[1], &ams_p);
  char* x86_p = (char*)calloc(MAX_PROGRAM_SIZE, 1);
  assert(x86_p);
  assert(ams_p);
  
  FILE* x86_f = NULL;
  OpenFile("my.out", "wb", &x86_f);
  assert(x86_f);

  // Translate
  size_t stdlib_size   = GetStdlibSize();
  size_t prog_size = TranslateProgram(x86_p, ams_p, ams_s, stdlib_size);
  
  // Write
  verify(AppendHeaders(x86_f, prog_size + stdlib_size));
  verify(AppendProgram(x86_f, x86_p, prog_size + stdlib_size, stdlib_size));
  
  fclose(x86_f);
  free(ams_p);
  free(x86_p);
  
  return 0;
}
