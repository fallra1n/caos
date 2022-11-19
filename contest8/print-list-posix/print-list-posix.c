#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  char *first = argv[1];

  int return_value = 0;
  int read_fd = -1;

  read_fd = open(first, O_RDONLY, 0640);
  if (read_fd == -1) {
    return_value = 1;
    goto Exit;
  }

  int value = 0;
  uint32_t next_pointer = 0;

  do {
    int read_count = read(read_fd, &value, sizeof(value));
    if (read_count == 0) {
      goto Exit;
    }

    if (read_count == -1) {
      return_value = 1;
      goto Exit;
    }

    read_count = read(read_fd, &next_pointer, sizeof(next_pointer));
    if (read_count <= 0) {
      return_value = 1;
      goto Exit;
    }

    printf("%d\n", value);
    int offset = lseek(read_fd, next_pointer, SEEK_SET);
    if (offset == -1) {
      return_value = -1;
      goto Exit;
    }

  } while (next_pointer != 0);

Exit:
  close(read_fd);

  return return_value;
}