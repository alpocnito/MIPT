#include <stdio.h>
#include <string.h>
#include <assert.h>

////////////////////////////////////////////////////////
//
// This program is just copy of echo program in Linux, with
//  only one parameter: -n
//
////////////////////////////////////////////////////////


struct Params
{
  char newline_char;
  int first_word_pos;
};

void AnalyseOpts(int argc, char** argv, Params* params)
{
  assert(argc > 0);
  assert(argv);

  for (int i = 1; i < argc && argv[i][0] == '-'; ++i) 
  {
    if (argv[i][1] == 'n')
    {
      params->first_word_pos++;
      params->newline_char = '\0';
    }
  }

  return;
}

void PrintWords(int argc, char** argv, Params* params)
{
  assert(argc > 0);
  assert(argv);
  
  // printing first argument
  if (params->first_word_pos < argc)
    printf("%s", argv[params->first_word_pos]);
  
  //printing other arguments
  for (int i = params->first_word_pos + 1; i < argc; ++i)
    printf(" %s", argv[i]);

  return;
}

int main(int argc, char** argv) 
{
  Params params{};

  params.newline_char = '\n';
  params.first_word_pos = 1;
  
  if (argc > 1)
  {
    AnalyseOpts(argc, argv, &params);
    PrintWords(argc, argv, &params); 
  }

  if (params.newline_char != '\0')
    putchar(params.newline_char);
  
  return 0;
}
