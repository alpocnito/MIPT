#include "language.h"

int ReadWFile(const char* file_name, char** buffer) {
  assert(file_name);
  
  FILE* file = fopen(file_name, "r");
  int size_file = FileSizeInBytes(file);

  *buffer = (char*)calloc(size_file + 1, sizeof(wchar_t));
  char read_char = fgetc(file);

  for (int i = 0; read_char != EOF && i <= size_file; ++i) {
    (*buffer)[i] = read_char;
    read_char = fgetc(file);
  }
  
  return size_file;
}


int main() {
 
  char* buffer = NULL;
  int buf_size = ReadWFile("Programs/funny_program", &buffer);
  FILE* norm_file = fopen("Programs/program", "w");
  
  int it = 0;
  while (it < buf_size) {
    if ( (strncmp(buffer + it, "_", 1) == 0) || (( buffer[it] >= 'à') && (buffer[it] <= 'ÿ')) ) {
      char word[MAX_LEN_SYSTEM_NAME * 2] = {};
      int number_read = 0;
      
      sscanf(buffer + it, "%[_à-ÿ,]%n", word, &number_read);
      
      printf("This is i read: %s:%d\n", word, it);
      it += number_read;

      int printed = 0;
      for (int i = 0; i < NUMBER_FUN; ++i) {
            if (strcmp(word, fun_words[i]) == 0) {
          printed = 1;

          fprintf(norm_file, "%s", norm_words[i]);
          break;
        }
      }

      if (!printed) {
        printf("Unknown words: %s\n", word); 
        //abort();
      }
    }
    else 
      fprintf(norm_file, "%c", buffer[it++]);
  }

  return 0;
}

