#include <assert.h>
#include <string.h>

#define SPACE_IMPLEMENTATION
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
  void *ptr = space_realloc(&space, NULL, 0, 1024);
  space_realloc(&space, ptr, 1024, 500);
  void *ptr1 = space_realloc(&space, NULL, 0, 2024);
  ptr1 = space_realloc(&space, ptr1, 2024, 800);
  space_malloc(&space, 100);
  ptr1 = space_realloc(&space, ptr1, 800, 800 + 200);

  space_free_space(&space);
  return 0;
}
