#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef char flag_t;

//! A structure for Strings
struct String {
   char* str; //!< the pointer to the start of string
   size_t len;   //!< the FULL length of line
};

//-----------------------------------
//! open file
//!
//! @param [in] state state-the state of opening file. Can be only w or r
//! @param [in] file_name file_name-name of file
//! @param [out] file file-reference to file
//!
//! @return 0, if files opened, 1 otherwise
//-----------------------------------
int OpenFile(const char file_name[], const char* state, FILE** file);

//-----------------------------------
//! return size of file
//!
//! @param [in] file_with_text file_with_text-file
//!
//! @return size of file, -1 if error occurs
//-----------------------------------
long int FileSizeInBytes(FILE* file_with_text);

//-----------------------------------
//! separate text of lines 
//!
//! @param [in] buffer buffer-text
//! @param [in] buffer_size buffer_size-size of text
//! @param [out] list_of_lines list_of_lines-array with lines
//!
//! @return number of lines, -1 with error occurs
//-----------------------------------
size_t SeparateLines(char* buffer, size_t buffer_size, struct String** list_of_lines);

//-----------------------------------
//! read given file very fast 
//!
//! @param [in] name_of_file name_of_file-just name of the file
//! @param [out] buffer buffer-text from file
//!
//! @return number of read elements, -1 otherwise
//-----------------------------------
size_t ReadFile(const char name_of_file[], char** buffer);

//-----------------------------------
//! print lines of given array in the file
//!
//! @param [in] file file-where to write lines
//! @param [in] list_of_lines list_of_lines-array with lines
//! @param [in] number_of_lines number_of_lines-number of lines
//-----------------------------------
void PrintLines(FILE* file, struct String* list_of_lines, int number_of_lines);

//-----------------------------------
//! print text from buffer in the file 
//!
//! @param [in] file file-where to write buffer
//! @param [in] list_of_lines list_of_lines-array with lines
//! @param [in] number_of_lines number_of_lines-number of lines
//-----------------------------------
void PrintBuffer(FILE* file, char* buffer, size_t buffer_size);

//-------------------------------------------------------------------------
//! 
//! Clear Ueless symbols from the end of line 
//! 
//! @param [in] list_of_lines list_of_lines-array with all lines
//! @param [in] nlines nlines-number of lines
//-------------------------------------------------------------------------
void ClearEndOfLines(struct String** list_of_lines, size_t nlines);

//-----------------------------------
//! Check is symbol is Useless ('\n', '\t', ' ', '\0')
//!
//! @param [in] symbol symbol-symbol to check
//!
//! @return 1 if symbol is Ueless, 0 otherwise
//-----------------------------------
int IsUselessSymbol(char symbol);

//-----------------------------------
//! Count number of lines in the buffer
//!
//! @param [in] buffer buffer-buffer with text
//! @param [in] buffer_size buffer_size-size of buffer in bytes
//!
//! @return number of lines in the buffer, 0 if error occurs
//-----------------------------------
size_t NumberOfLines(char* buffer, size_t buffer_size);

//-----------------------------------
//! Check if symbol is letter ([A-Z] || [a-z])
//!
//! @param [in] symbol symbol-symbol to check 
//!
//! @return 1 if symbol is letter, 0 otherwise
//-----------------------------------
int isletter(char symbol);

void NormalizeStrings(struct String** list_of_lines, int number_of_lines);

size_t DirectComparisonForLines(const void* a, const void* b);
size_t ReverseComparisonForLines(const void* a, const void* b);
