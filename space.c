#include <assert.h>
#include <string.h>

#define PLANET_IMPLEMENTATION
#include "space.h"

Space space = {0};
void print(const char *text) {
  size_t len = strlen(text) + 1;
  char *data = space_malloc(&space, len * sizeof(char));
  if (!data) {
    assert(0 && "The data is NULL");
  }
  memcpy(data, text, len);
  printf("%s\n", data);
}


int main() {
  print("Hello, World!");
  print("you are free");

  space_free_space(&space);
  return 0;
}
