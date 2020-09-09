#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int tolower(int c) {
 if (c >= 'A' && c <= 'Z')
  return c - 'a';

  return c;
}

int strncasecmp(const char* string1, const char* string2, size_t count) {
  assert(string1);
  assert(string2);

  for (int i = 0; i < count; ++i) {
    char c1 = tolower(string1);
    char c2 = tolower(string2);

    if ( c1 > c2 ) return -1;
    if ( c1 < c2 ) return +1;
    
    if (string2[i] == '\0') break;
  }

  return 0;
}

int main() {

  assert(strncasecmp("Hello", "Hello", 14) == 0);
  assert(strncasecmp("HeLlo", "hello", 3) == 0);
  assert(strncasecmp("a", "a", 3) == 0);
  assert(strncasecmp("A", "a", 3) == 0);
  assert(strncasecmp("Not", "not", 3) == 0);
  assert(strncasecmp("NOT", "Not", 3) == 0);

  return 0;
}
