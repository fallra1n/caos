#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

struct Item {
  int value;
  uint32_t next_pointer;
};

int main(int argc, char **argv) {
  if (argc < 2) {
    return 1;
  }

  char *first = argv[1];

  int return_value = 0;
  int read_fd = -1;

  read_fd = open(first, O_RDONLY);
  if (read_fd == -1) {
    return_value = 1;
    goto Exit;
  }

  struct Item item;

  struct stat st;
  if (fstat(read_fd, &st) == -1) {
    return_value = 1;
    goto Exit;
  }

  char *data = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, read_fd, 0);
  close(read_fd);

  if (data == MAP_FAILED) {
    return_value = 1;
    goto Exit;
  }

  item.next_pointer = 0;

  do {
    item = *((struct Item*)(data + item.next_pointer));

    printf("%d\n", item.value);
  } while (item.next_pointer != 0);

  Exit:
  if (munmap(data, st.st_size) != 0) {
    return_value = 1;
  }

  return return_value;
}