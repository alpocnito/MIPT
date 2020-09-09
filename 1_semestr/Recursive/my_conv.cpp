int my_conv() {
    
  char rus_alphabit[] = "יצףךוםדרשחץפûגאןנמכהז‎ÿקסלטעüב‏";
  char end_alphabet[] = "ycukenghhzhfivaproldgaihsmutibu";

  char* buffer = NULL;
  size_t buffer_size = ReadFile("HelpFiles/frontend.tree", &buffer) - 1;

  for (int i = 0; i < buffer_size; ++i)
    for (int j = 0; j < 33; ++j)
      if (buffer[i] == rus_alphabit[j])
        buffer[i] = end_alphabet[j];
  
  FILE* output = NULL;
  OpenFile("HelpFiles/frontend.tree", "w", &output);
  fprintf(output, "%s", buffer);
  fclose(output);

  return 1;
}
