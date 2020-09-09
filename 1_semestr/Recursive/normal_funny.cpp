#include "language.h"

int main(int argc, char** argv) {
  
  char* buffer = NULL;
  int buf_size = ReadFile(argv[1], &buffer);
  FILE* funny_file = fopen("Programs/funny_program", "w");
  
  int it = 0;
  while (it < buf_size) {
    
    if (isalpha(buffer[it])) {
      char word[MAX_LEN_SYSTEM_NAME] = "kill";
      int number_read = 0;

      sscanf(buffer + it, "%[^()^\n ]%n", word, &number_read);
      it += number_read;

      int printed = 0;
      for (int i = 0; i < NUMBER_FUN; ++i) {
        if (strcmp(word, norm_words[i]) == 0) {
          printed = 1;

          fprintf(funny_file, "%s", fun_words[i]);
          break;
        }
      }

      if (!printed)
        fprintf(funny_file, "%s", word);
    }
    else if (buffer[it] == '-' || buffer[it] == '+' || buffer[it] == '/' || buffer[it] == '*' || buffer[it] == '^' || buffer[it] == '=') {
      
      if (buffer[it + 1] == '=' || buffer[it - 1] == '>' || buffer[it - 1] == '<' || buffer[it - 1] == '=') {
        fprintf(funny_file, "%c", buffer[it++]);
        continue;
      }

      for (int i = 0; i < NUMBER_FUN; ++i)
        if (buffer[it] == norm_words[i][0]) 
          fprintf(funny_file, "%s", fun_words[i]);
      it++;
    }
    else
      fprintf(funny_file, "%c", buffer[it++]);

  }

  return 0;
}

