#ifdef DEBUG
void StartDebug();
void StopDebug();

#define Calloc(number, size_of_one, var) calloc(number, size_of_one); {\
    FILE* calloced_file = fopen("calloced.debug", "a");\
    fprintf(calloced_file, "%p-%s:%d\n", var, __FILE__, __LINE__);\
    fclose(calloced_file);\
  };

#define Free(pointer) {\
    FILE* freed_file = fopen("freed.debug", "a");\
    fprintf(freed_file, "%p-%s:%d\n", pointer, __FILE__, __LINE__);\
    fclose(freed_file);\
    free(pointer);\
  };

void StartDebug() {
  
  FILE* cal = fopen("calloced.debug", "w");
  fprintf(cal, "\n");
  fclose(cal);
  
  FILE* fre = fopen("freed.debug", "w");
  fprintf(fre, "\n");
  fclose(fre);
}

void StopDebug() {
  
  char* cal_b = 0;
  char* fre_b = 0;

  size_t cal_size = ReadFile("calloced.debug", &cal_b);
  size_t fre_size = ReadFile("freed.debug",    &fre_b);

  struct String* cal_strings = {};
  struct String* fre_strings = {};

  int cal_number = SeparateLines(cal_b, cal_size, &cal_strings);
  int fre_number = SeparateLines(fre_b, fre_size, &fre_strings);
    
  NormalizeStrings(&cal_strings, cal_number);
  NormalizeStrings(&fre_strings, fre_number);

  if (cal_number == fre_number) {
    PRINT_GREEN(BOLD("All is freed\n"));
    return;
  }
  
  if (fre_number > cal_number) {
    PRINT_RED(BOLD("Free more than calloc???\n"));
    return;
  }
  
  PRINT_RED(BOLD("Unfreed calloces:\n"));

  for (int i = 0; i < cal_number; ++i) {
    
    int was_freed = 0;
    for (int j = 0; j < fre_number; ++j) {
      if (strncmp(cal_strings[i].str, fre_strings[j].str, 14) == 0) {
        was_freed = 1;
        break;
      }
    }
    
    if (was_freed == 0) printf("  %s\n", cal_strings[i].str);
    
  }

  free(cal_strings);
  free(fre_strings);
  
  free(cal_b);
  free(fre_b);
}

#else

#define Calloc(number, size_of_one, var) calloc(number, size_of_one);
#define Free(pointer) free(pointer);

void StartDebug() {;}
void StopDebug() {;}

#endif
