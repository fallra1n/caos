#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char **argv) {
  char *first = argv[1];
  
  int return_value = 0;
  int write_fd = -1;

  write_fd = open(first, O_WRONLY, 0640);
  if (write_fd == -1) {
    return 1;
  }

  int value = 4;
  uint32_t next_pointer = 16;

  write(write_fd, &value, 4);
  write(write_fd, &next_pointer, 4);

  value = 5;
  next_pointer = 0;
  write(write_fd, &value, 4);
  write(write_fd, &next_pointer, 4);

  value = 6;
  next_pointer = 8;
  write(write_fd, &value, 4);
  write(write_fd, &next_pointer, 4);

  close(write_fd);
  return return_value;
}