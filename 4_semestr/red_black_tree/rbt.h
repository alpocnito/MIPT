#include "color.h"

typedef unsigned char color_t;
#define PRINTF_COLOR "%d"
const color_t Red   = 0;
const color_t Black = 1;


typedef int data_t;
#define PRINTF_DATA "%d"

typedef int error_t;
enum procedures 
{
  NO_ERROR = 0,
  ERROR_DETECTED,
  ERROR_PTR_IS_NULL,
  ERROR_WITH_CALLOC,
  ERROR_WITH_COLOR,
  ERROR_EOF
};

#define CHECK_ERROR(DATA_TO_CHECK, PROCEDURA)                            \
  do {                                                                   \
    PRINT(RED, BOLD("Critical error\n"));                                \
    if (PROCEDURA == ERROR_PTR_IS_NULL)                                  \
    {                                                                    \
      if (DATA_TO_CHECK == NULL)                                         \
        PRINT(MAGENTA, BOLD("Can not be NULL: %s\n"), #DATA_TO_CHECK);   \
      else                                                               \
        break;                                                           \
    }                                                                    \
    if (PROCEDURA == ERROR_WITH_CALLOC)                                  \
    {                                                                    \
      if (DATA_TO_CHECK == NULL)                                         \
        PRINT(MAGENTA, BOLD("Can not be calloced: %s\n"), #DATA_TO_CHECK);\
      else                                                               \
        break;                                                           \
    }                                                                    \
    if (PROCEDURA == ERROR_WITH_COLOR)                                   \
    {                                                                    \
      if (DATA_TO_CHECK != Red && DATA_TO_CHECK != Black)                \
        PRINT(MAGENTA, BOLD("Invalid color:" PRINTF_COLOR "\n"), COLOR); \
      else                                                               \
        break;                                                           \
    }                                                                    \
    if (PROCEDURA == ERROR_WITH_CLOSE)                                   \
    {                                                                    \
      if (DATA_TO_CHECK == EOF)                                          \
        PRINT(MAGENTA, BOLD("Can not close: %s = "), #FFILE);            \
      else                                                               \
        break;                                                           \
    }                                                                    \
    PRINT(MAGENTA, BOLD("In the %s:"), __PRETTY_FUNCTION__);             \
    PRINT(MAGENTA, BOLD("%d in "), __LINE__);                            \
    PRINT(MAGENTA, BOLD("%s\n"), __FILE__);                              \
    return ERROR_DETECTED;                                               \
  } while (0);
         
#define WARNING(MSG, ...)                                    \
  do {                                                       \
    PRINT(RED, N("Warning\n"));                              \
    PRINT(MAGENTA, BOLD(MSG), ##__VA_ARGS__)                 \
    PRINT(MAGENTA, BOLD("In the %s:"), __PRETTY_FUNCTION__); \
    PRINT(MAGENTA, BOLD("%d in "), __LINE__);                \
    PRINT(MAGENTA, BOLD("%s\n"), __FILE__);                  \
  } while(0);








