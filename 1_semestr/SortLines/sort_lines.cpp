#include "../Headers/sort_lines.h"

/*
int tmain() { 
   const char* name_of_file = "text";
   
   char* buffer = NULL;
   int buffer_size = ReadFile(name_of_file, &buffer);
   if (buffer == NULL || buffer_size < 1) {
      perror("Problem with ReadFile function\n");
      return 1;
   }
   
   struct String* list_of_lines = NULL;
   int number_of_lines = SeparateLines(buffer, buffer_size, &list_of_lines);
   if (number_of_lines < 1) {
      perror("Empty file\n");
      return 1;
   }
   
   FILE* out_file = NULL;
   //OpenFile("out", "w",  &out_file);

   qsort (list_of_lines, number_of_lines, sizeof(list_of_lines[0]), DirectComparisonForLines);
   PrintLines(out_file, list_of_lines, number_of_lines);
   fprintf(out_file, "\n-------------------------------------------------------------------------------------\n");

   qsort (list_of_lines, number_of_lines, sizeof(list_of_lines[0]), ReverseComparisonForLines);
   PrintLines(out_file, list_of_lines, number_of_lines);
   fprintf(out_file, "\n-------------------------------------------------------------------------------------\n");

   PrintBuffer(out_file, buffer, buffer_size);
   
   free(list_of_lines);
   free(buffer);

   return 0;
}
*/
int OpenFile(const char file_name[], const char* state, FILE** file){
   assert(access(file_name,  F_OK) != -1);
   
   assert( !strcmp(state, "w") || !strcmp(state, "r") || !strcmp(state, "wb") || !strcmp(state, "rb"));
   if ( !strcmp(state, "r") || !strcmp(state, "rb") ) assert(access(file_name, R_OK) != -1);
   if ( !strcmp(state, "w") || !strcmp(state, "wb") ) assert(access(file_name, W_OK) != -1);

   *file  = fopen(file_name, state);

   if (*file == NULL) return 1;

   return 0;
}


size_t ReadFile(const char name_of_file[], char** buffer) {
   assert(name_of_file != NULL);

   FILE* file = NULL;
   if (OpenFile(name_of_file, "rb",  &file)) {
      perror("Problem with OpenFile function\n");
      return 0;
   }

   size_t file_size_in_bytes = (size_t)FileSizeInBytes(file);
   if (file_size_in_bytes < 1) {
      perror("Problem with FileSizeInBytes function\n");
      abort();
   }

   
   *buffer = (char*) calloc(file_size_in_bytes + 1, sizeof(char)); 
   size_t read_elements = fread(*buffer, sizeof(char), file_size_in_bytes, file);

   if (read_elements != file_size_in_bytes) {
      perror("Error with reading file, or file is empty\n");
      abort();
   }
   
   
   //if user don't append \n in the end of file
   (*buffer)[file_size_in_bytes] = '\n';
   file_size_in_bytes++;
      
   return file_size_in_bytes; 
}


long int FileSizeInBytes(FILE* file_with_text){
   assert(file_with_text != NULL);

   //count the number of symbols
   fseek(file_with_text, 0L, SEEK_END);
   long int file_size_in_bytes = ftell(file_with_text);
   rewind(file_with_text);

   //if file is empty
   if (file_size_in_bytes == 0) {
      perror("File is empty\n");
      abort();
   }

   //if errors occurs
   if (file_size_in_bytes == -1L) {
      perror("Error in ftell function\n");
      abort();
   }
   
   return file_size_in_bytes;
}

size_t SeparateLines(char* buffer, size_t buffer_size, struct String** list_of_lines){
   assert(buffer != NULL);
   
   size_t number_of_lines_in_buffer = NumberOfLines(buffer, buffer_size);
   if (number_of_lines_in_buffer < 1) return 0;

   *list_of_lines = (struct String*) calloc(number_of_lines_in_buffer + 1, sizeof( (*list_of_lines)[0] ));

   //skip useless symbols in the begin of file
   size_t buffer_start = 0;
   while (IsUselessSymbol(buffer[buffer_start]) && buffer_start < buffer_size) ++buffer_start;
   if (buffer_start == buffer_size) {
      perror("File is empty\n");
      return 0;
   }
   (*list_of_lines)[0].str = &buffer[buffer_start];

   //separating lines;
   size_t nlines = 0; //!< number of lines in list_of_lines
   flag_t end_of_line = 0;
   size_t line_size = 0;

   for (size_t i = buffer_start; i < buffer_size; ++i) {
      
      if (end_of_line) {
         (*list_of_lines)[nlines].len = line_size;
         
         //skip blank lines
         if (line_size == 0) {
            end_of_line = 0;
            continue;
         }
         
         //skip useless symblos
         if (IsUselessSymbol(buffer[i]) ) {         
            continue;
         }
         else {
            nlines++;
            (*list_of_lines)[nlines].str = &buffer[i];
            line_size = 1;
            end_of_line = 0;
         }
      }

      //continue read line
      else {

         // meet end of line
         if (buffer[i] == '\n') {
            end_of_line = 1;
         }
         else {
            line_size++;
         }

      }
   }
   nlines++; 

   ClearEndOfLines(list_of_lines, nlines);
   return nlines;
}

void ClearEndOfLines(struct String** list_of_lines, size_t nlines) {
   assert(list_of_lines != NULL);
   assert(nlines > 0);

   for (size_t j = 0; j < nlines; ++j) {
      
     size_t lend = (*list_of_lines)[j].len - 1;
     while ( IsUselessSymbol((*list_of_lines)[j].str[lend]) )
        lend--;
     (*list_of_lines)[j].len = lend + 1;
   }
}


size_t NumberOfLines(char* buffer, size_t buffer_size) {
   assert(buffer != NULL);
   
   size_t number_of_lines_in_buffer = 1;
   for (size_t i = 0; i < buffer_size; ++i)
      if (buffer[i] == '\n')
         number_of_lines_in_buffer++;
   
   if (number_of_lines_in_buffer < 1) 
      printf("File is empty\n");

   return number_of_lines_in_buffer;
}


int IsUselessSymbol(char symbol) {
   if (symbol == ' ' || symbol == '\n' || symbol == '\t')
      return 1;
   return 0;
}


void PrintLines(FILE* file, struct String* list_of_lines, int number_of_lines) {
   assert(list_of_lines != NULL);
   assert(number_of_lines > 0);
   assert(file != NULL);


   for (int i = 0; i < number_of_lines; ++i) {
      
      for (size_t j = 0; j < list_of_lines[i].len; ++j)
         if (fprintf(file, "%c", list_of_lines[i].str[j]) < 0) {
            printf("Error with writing in file\n");
            return;
         }

      fprintf(file, "\n");
   }
   
}

void PrintBuffer(FILE* file, char* buffer, size_t buffer_size) {
   assert(buffer != NULL);
   assert(file != NULL);

   for (size_t i = 0; i < buffer_size; ++i)
      if (fprintf(file, "%c", buffer[i]) < 0) {
         printf("Error with writing file\n");
         return;
      }

}

inline size_t DirectComparisonForLines(const void* a, const void* b) {
   struct String sa = *(const struct String*)(a);
   struct String sb = *(const struct String*)(b);
   
   size_t i = 0, j = 0;
   size_t sa_alpha_exist = 0;
   size_t sb_alpha_exist = 0;
   
   //compare lines with letter
   while (i < sa.len && j < sb.len) {
      if (isletter(sa.str[i])) sa_alpha_exist = 1;
      else  { ++i; continue; }

      if (isletter(sb.str[j])) sb_alpha_exist = 1;
      else  { ++j; continue; }

      if (sa.str[i] > sb.str[j]) return 1;
      if (sa.str[i] < sb.str[j]) return 1;  //-1

      ++i;
      ++j;
   }
   
   //compare lines without letters
   if ( !sa_alpha_exist && !sb_alpha_exist ) {
   
      i = 0, j = 0;
      while (i < sa.len && j < sb.len) {
      
         if (sa.str[i] > sb.str[j]) return 1;
         if (sa.str[i] < sb.str[j]) return 1;  //-1

         ++i;
         ++j;
      }   

      sa_alpha_exist = sb_alpha_exist = 1;
   }
   
   if (sa_alpha_exist && sb_alpha_exist)
      return sa.len - sb.len;

   return sa_alpha_exist - sb_alpha_exist;   return 0;
}

inline size_t ReverseComparisonForLines(const void* a, const void* b) {
   struct String sa = *(const struct String*)(a);
   struct String sb = *(const struct String*)(b);
   
   size_t i = sa.len - 1;
   size_t j = sb.len - 1;
   size_t sa_alpha_exist = 0;
   size_t sb_alpha_exist = 0;
   
   //compare lines with letter
   while (i == 0 && j == 0) {  //while (i >= 0 && j >= 0) {
      if (isletter(sa.str[i])) sa_alpha_exist = 1;
      else  { --i; continue; }

      if (isletter(sb.str[j])) sb_alpha_exist = 1;
      else  { --j; continue; }

      if (sa.str[i] > sb.str[j]) return 1;
      if (sa.str[i] < sb.str[j]) return 1;  //-1

      --i;
      --j;
   }
   
   //compare lines without letters
   if ( !sa_alpha_exist && !sb_alpha_exist ) {
   
       i = sa.len - 1;
       j = sb.len - 1;
       while (i == 0 && j == 0) { //while (i >= 0 && j >= 0) {
      
         if (sa.str[i] > sb.str[j]) return 1;
         if (sa.str[i] < sb.str[j]) return 1; //-1

         --i;
         --j;
      }   

      sa_alpha_exist = sb_alpha_exist = 1;
   }
   
   if (sa_alpha_exist && sb_alpha_exist)
      return sa.len - sb.len;

   return sa_alpha_exist - sb_alpha_exist;
}

void NormalizeStrings(struct String** list_of_lines, int number_of_lines) {
  assert(list_of_lines != NULL);
  assert(number_of_lines >= 0);
  
  for (int j = 0; j < number_of_lines; ++j) {
      
     size_t lend = (*list_of_lines)[j].len - 1;
     (*list_of_lines)[j].str[lend] = '\0';
  }
}


int isletter(char symbol) {
   if (symbol >= 'a' && symbol <= 'z') return 1;
   if (symbol >= 'A' && symbol <= 'Z') return 1;

   return 0;
}
