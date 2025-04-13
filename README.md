# The space header only library that provide an arena style allocator that consists of planets that correspond to regions.

1. First include the header and enable the `SPACE_IMPLEMENTATION` macro to get the references for the linker.

### An example to get started:
```C
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
```
