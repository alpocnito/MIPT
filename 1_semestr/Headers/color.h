#define BLINK(word)      ";5m" word
#define BRIGHT(word)     ";1m" word
#define BOLD(word)       ";1m" word
#define DIM(word)        ";2m" word
#define UNDERSCORE(word) ";4m" word
#define REVERSE(word)    ";7m" word
#define N(word)          "m" word

#define PRINT_BLACK(word)   printf("\033[0;30" word "\033[0m")
#define PRINT_RED(word)     printf("\033[0;31" word "\033[0m")
#define PRINT_GREEN(word)   printf("\033[0;32" word "\033[0m")
#define PRINT_ORANGE(word)  printf("\033[0;33" word "\033[0m")
#define PRINT_BLUE(word)    printf("\033[0;34" word "\033[0m")
#define PRINT_MAGENTA(word) printf("\033[0;35" word "\033[0m")
#define PRINT_CYAN(word)    printf("\033[0;36" word "\033[0m")
#define PRINT_WHITE(word)   printf("\033[0;37" word "\033[0m")

#define PRINT_BLACK_E(word, expression)   printf("\033[0;30" word "\033[0m", expression)
#define PRINT_RED_E(word, expression)     printf("\033[0;31" word "\033[0m", expression)
#define PRINT_GREEN_E(word, expression)   printf("\033[0;32" word "\033[0m", expression)
#define PRINT_ORANGE_E(word, expression)  printf("\033[0;33" word "\033[0m", expression)
#define PRINT_BLUE_E(word, expression)    printf("\033[0;34" word "\033[0m", expression)
#define PRINT_MAGENTA_E(word, expression) printf("\033[0;35" word "\033[0m", expression)
#define PRINT_WHITE_E(word, expression)   printf("\033[0;37" word "\033[0m", expression)
#define PRINT_CYAN_E(word, expression)    printf("\033[0;36" word "\033[0m", expression)

//Example 
//
//PRINT_BLACK(N("Hello, world"));
//
//PRINT_BLACK_E(N("%d"), i));
