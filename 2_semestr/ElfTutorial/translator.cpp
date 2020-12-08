#include <elf.h>
#include <cassert>
#include <cstdio> 
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#define B1 __uint8_t
#define B2 __uint16_t
#define B4 __uint32_t
#define B8 __uint64_t

#define FATAL(text) do {\
                        printf("FATAL on %d in %s: ", __LINE__, __PRETTY_FUNCTION__); \
                        printf("%s\n", text); \
                    } while (0);

struct ELF_Header {
  B4 ei_MAG         = 0x464C457F;       // signature
  B1 ei_CLASS       = ELFCLASS64;       // 64 bit format
  B1 ei_DATA        = ELFDATA2LSB;      // little endian
  B2 ei_VERSION     = 0;                // signature
  B2 ei_OSABI       = ELFOSABI_NONE;    // UNIX System V ABI
  B4 ei_OSABIVER    = 0x00000000;       // signature
  B2 E_TYPE         = ET_EXEC;          // executable file
  B2 E_MACHINE      = EM_X86_64;        // x86_64 AMD
  B4 E_VERSION      = EV_CURRENT;       // signature
  B4 E_ENTRY        = 0;          // entry point
  B4 E_PHOFF        = 0;          // start pf program header table
  B4 E_SHOFF        = 0;          // start of the section header table
  B4 E_FLAGS        = 0x00000000; // signature
  B2 E_EHSIZE       = 0x0040;     // size of this header
  B2 E_PHENTSIZE    = 0x0038;     // size of program header table 
  B2 E_PHNUM        = 0x0001;     // number of entries in the progtam header file
  B2 E_SHENTSIZE    = 0x0040;     // size of the section header table
  B2 E_SHNUM        = 0x0005;     // number of entries in the section header table
  B2 E_SHSTRNDX     = 0x0004;     // section header, that contains section names
};

struct Section_Header {
  B4 sh_name      = 0;
  B4 sh_type      = 0;
  B4 sh_flags     = 0;
  B4 sh_addr      = 0;
  B4 sh_offset    = 0;
  B4 sh_size      = 0;
  B4 sh_link      = 0;
  B4 sh_info      = 0;
  B4 sh_addraligh = 0;
  B4 sh_entsize   = 0;
};

struct Rela_Text {
  B4 r_offset = 0;
  B4 r_info   = 0;
  B4 r_addend = 0;
};

struct Symtab {
  B4 st_name  = 0;
  B4 st_value = 0;
  B4 st_size  = 0;
  B1 st_info  = 0;
  B1 st_other = 0;
  B2 st_shndx = 0;
};

// Debug function - prints bytes
void PrintBytes(char* buf, size_t num_bytes)
{
  assert(buf);
  
  for (size_t i = 0; i < num_bytes; ++i)
  {
    printf("%02x ", (unsigned int)buf[i]);
  }
  printf("\n");
}

// Small scrpt to make std::string from char*
std::string GetString(char* str)
{
  size_t i = 0;
  std::string ans{};
  while (str[i] != '\0')
    ans.push_back(str[i++]);
  return ans;
}


//---------------------------------------------------------------------------- 
//! Writes data to the output file
//!
//! @param [in] out  out  - where to write
//! @param [in] cose code - what write 
//---------------------------------------------------------------------------- 
void DumpAnswer(FILE* out, std::vector<std::string>* code)
{
  for (size_t i = 0; i < code->size(); ++i)
    fprintf(out, "%s\n", (*code)[i].c_str());
  printf("\n-------------------------------------\n");
  
  return;
}

//---------------------------------------------------------------------------- 
//! Reads data from input file
//!
//! @param [in]  file file - where to read from
//! @param [out] size size - where to save the read size
//!
//! @return pointer to the buffer with read data. BUFFER MUST BE FREED 
//---------------------------------------------------------------------------- 
char* ReadData(char* file, long int* size)
{
  assert(file);

  FILE* input_file  = fopen(file, "rb");
  if (!input_file)
    FATAL("Incorrect input file");
  
  // get size of file
  fseek(input_file, 0L, SEEK_END);
  long int input_size = ftell(input_file);
  if (input_size < 1)
    FATAL("Input file is small!\n");
  fseek(input_file, 0L, SEEK_SET);
  
  // buffer for data
  char* buffer = (char*)calloc((size_t)input_size, sizeof(buffer[0]));
  if (!buffer)
    FATAL("No memory\n");
  
  size_t read_buffer_size = fread(buffer, 1, (size_t)input_size, input_file);
  if ((size_t)input_size != read_buffer_size)
    FATAL("Error in reading input file");
  
  if (fclose(input_file) != 0)
    FATAL("Cannot close input file\n");
  
  *size = input_size;
  return buffer;
}

//---------------------------------------------------------------------------- 
//! Return array of all sections
//!
//! @param [in]  buffer       buffer       - elf format text
//! @param [in]  buffer_size  buffer_size  - size of elf format text
//! @param [out] num_sections num_sections - number of read sections
//!
//! @return pointer to the start of section headers
//---------------------------------------------------------------------------- 
Section_Header* GiveSections(char* buffer, long int buffer_size, size_t* num_sections)
{
  assert(buffer);

  if (buffer_size < (long int)sizeof(ELF_Header))
    FATAL("File don't have Elf Header\n");

  ELF_Header* elfHeader = (ELF_Header*) buffer;
  B4 start_sections     = elfHeader->E_SHOFF;
  B2 num_of_sections    = elfHeader->E_SHNUM;  
  
  if (elfHeader->E_MACHINE != 0xF3)
    FATAL("Is it not RISC-V");

  if ((long int)(start_sections + num_sections) < buffer_size)
    FATAL("File don't have all sections\n");
  
  Section_Header* sectionHeaders = (Section_Header*)(start_sections + buffer);
  
  *num_sections = num_of_sections;
  return sectionHeaders;
}

//---------------------------------------------------------------------------- 
//! Gives pointer to the start of names
//!
//! @param [in]  buffer      buffer      - elf format text
//! @param [in]  buffer_size buffer_size - size of elf format text
//! @param [out] names_size  names_size  - size of names array in bytes
//!
//! @return pointer to the start of array with all names
//---------------------------------------------------------------------------- 
char* GiveNames(char* buffer, long int buffer_size, size_t* names_size = NULL)
{
  assert(buffer);

  if (buffer_size < 64)
    FATAL("File don't have Elf Header\n");

  ELF_Header* elfHeader = (ELF_Header*) buffer;
  B2 section_names      = elfHeader->E_SHSTRNDX; // index in the array of all section headers
    
  size_t num_sections = 0;
  Section_Header* sectionHeaders = GiveSections(buffer, buffer_size, &num_sections);
  
  size_t strtab_start  = sectionHeaders[section_names].sh_offset;
  size_t size_of_names = sectionHeaders[section_names].sh_size;

  if (buffer_size < (long int)(size_of_names + strtab_start) )
    FATAL("ELF don't have all names\n");
  
  if (names_size != NULL)
    *names_size = size_of_names;

  return buffer + strtab_start;
}

//---------------------------------------------------------------------------- 
//! Gives pointer to the start of <section_name> section
//!
//! @param [in]  buffer       buffer       - elf format text
//! @param [in]  buffer_size  buffer_size  - size of elf format text
//! @param [in]  section_name section_name - name of the desired section
//! @param [out] section_size section_size - size of desired section in bytes
//!
//! @return pointer to the start of section, NULL, if sections doesn't exist
//---------------------------------------------------------------------------- 
char* GiveSection(char* buffer, long int buffer_size, const char* section_name, size_t* section_size)
{
  size_t section_nums = 0;
  Section_Header* sectionHeaders = GiveSections(buffer, buffer_size, &section_nums);
  char*           names          = GiveNames(buffer, buffer_size);

  bool find = false;
  size_t find_index = 0;
  for (size_t i = 0; i < section_nums; ++i)
  {
    size_t index = (size_t)sectionHeaders[i].sh_name;
    
    char* cur_str = names + index;

    // if section_name == current name
    if (strncmp(cur_str, section_name, sizeof(section_name) - 1) == 0) 
    {
      find = true;
      find_index = i;
      break;
    }
  }
  if (!find)
    return NULL;

  *section_size = sectionHeaders[find_index].sh_size;
  return buffer + sectionHeaders[find_index].sh_offset;
}

//---------------------------------------------------------------------------- 
//! Insert in the code number of lines
//!
//! @param [out] code          code         - empty array of lines
//! @param [in]  num_of_lines  num_of_lines - number of code lines
//---------------------------------------------------------------------------- 
void FillCodeWithNumberOfLines(std::vector<std::string>* code, unsigned num_of_lines)
{
  for (unsigned i = 0; i < num_of_lines; ++i)
  {
    char num_command[20] = {};
    sprintf(num_command, "[%08x]: ", i*4);
    code->push_back(num_command);
  }
}

//---------------------------------------------------------------------------- 
//! Gives pointer to the start of <section_name> section
//!
//! @param [out] code        code        - empty array of lines
//! @param [in]  names       names       - pointer to the start of array of names
//! @param [in]  symtabs     symtabs     - pointer to the start of array of symbols
//! @param [in]  symtab_nums symtab_nums - number of symbols in symtabs
//---------------------------------------------------------------------------- 
void FillCodeWithLabels(std::vector<std::string>* code, char* names, Symtab* symtabs, size_t symtab_nums)
{
  assert(code);
  assert(names);
  
  std::vector<unsigned> lines_with_label{};
  for (unsigned i = 0; i < (unsigned)symtab_nums; ++i)
  {
    if (symtabs[i].st_shndx == SHN_UNDEF || symtabs[i].st_shndx == SHN_COMMON)
      continue;

    B1 type = ELF32_ST_TYPE(symtabs[i].st_info); 
    if (type == STT_NOTYPE || type == STT_FUNC || type == STT_SECTION)
    {
      (*code)[symtabs[i].st_value / 4].push_back('<');
      (*code)[symtabs[i].st_value / 4].append(GetString(names + symtabs[i].st_name));
      (*code)[symtabs[i].st_value / 4].push_back('>');

      lines_with_label.push_back(symtabs[i].st_value / 4);
    }
  }
  
  // adjust all strings
  for (unsigned i = 0; i < (unsigned)code->size(); ++i)
  {
  
    bool find = false;
    for (unsigned j = 0; j < lines_with_label.size(); ++j)
      if (lines_with_label[j] == i)
        find = true;
    if (!find)
      (*code)[i].push_back('\t');

    (*code)[i].push_back('\t');
  }
}
//---------------------------------------------------------------------------- 
//! Main function. DisAssemble the elf file
//!
//! @param [in]  buffer       buffer       - elf format text
//! @param [in]  buffer_size  buffer_size  - size of elf format text
//!
//! @return array with of strings. Each string is the line of code
//---------------------------------------------------------------------------- 
std::vector<std::string>* DisAssembler(char* buffer, long int buffer_size)
{ 
  ////////////////////////////////////////// OPEN ALL SECTIONS ////////////////////////////////////////
  size_t text_size = 0;
  char* text = GiveSection(buffer, buffer_size, ".text", &text_size);
  if (!text)
    FATAL("File don't have .text");
  if (text_size % 4 != 0)
    FATAL("Code is not from RISC32\n");

  size_t symtab_size = 0;
  Symtab* symtabs = (Symtab*)GiveSection(buffer, buffer_size,".symtab", &symtab_size);
  if (!symtabs)
    FATAL("File don't have symtabs");
  size_t symtab_nums = symtab_size / sizeof(Symtab);
  
  ////////////////////////////////////////// MAIN ARRAY ////////////////////////////////////////////////
  std::vector<std::string>* code = new std::vector<std::string>;

  FillCodeWithNumberOfLines(code, (unsigned)(text_size / 4));
  
  char* names = GiveNames(buffer, buffer_size);
  FillCodeWithLabels(code, names, symtabs, symtab_nums);

   
  /* 
  size_t rela_text_size = 0;
  Rela_Text* relaTexts = (Rela_Text*)GiveSection(buffer, buffer_size, ".rela.text", &rela_text_size);
  size_t rela_text_nums = rela_text_size / sizeof(Rela_Text);
 
  if (relaTexts != NULL)
  {
    for (size_t i = 0; i < rela_text_nums; ++i)
    {
      printf("%d\n", relaTexts[i].r_offset);
      size_t symbol = ELF32_R_SYM(relaTexts[i].r_info);
      printf(symtabs[symbol].
    }
  }
  */

  return code;
}

int main(int argc, char** argv)
{
  if (argc != 3)
    FATAL("Incorrect input!\n");

  FILE* output_file = fopen(argv[2], "wt");
  if (!output_file)
    FATAL("Incorrect output file\n");

  long int buffer_size = 0;
  char* buffer = ReadData(argv[1], &buffer_size);
  
  std::vector<std::string>* output = DisAssembler(buffer, buffer_size);
  
  DumpAnswer(stdout, output);
  
  
  delete output;
  fclose(output_file);
  free(buffer);
  return 0;
}










