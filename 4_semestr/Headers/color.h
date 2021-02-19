#define BLINK(word)      ";5m" word
#define BRIGHT(word)     ";1m" word
#define BOLD(word)       ";1m" word
#define DIM(word)        ";2m" word
#define UNDERSCORE(word) ";4m" word
#define REVERSE(word)    ";7m" word
#define N(word)            "m" word

#define BLACK   "30"
#define RED     "31"
#define GREEN   "32"
#define ORANGE  "33"
#define BLUE    "34"
#define MAGENTA "35"
#define CYAN    "36"
#define WHITE   "37"

#define PRINT(COLOR, word, ...)   printf("\033[0;" COLOR word "\033[0m", ##__VA_ARGS__);

#define START(COLOR, FORMAT) printf("\033[0;" #COLOR FORMAT);
#define STOP                 printf("\033[0m");
