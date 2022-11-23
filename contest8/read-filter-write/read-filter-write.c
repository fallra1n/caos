#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFER_SIZE_READ 64 * 1024
#define BUFFER_SIZE_WRITE 64 * 1024

int HandleWrite(int write_fd, char *write_buf, int *write_len, char symbol) {
  int return_value = 0;
  write_buf[*write_len] = symbol;
  ++(*write_len);

  if (*write_len == BUFFER_SIZE_WRITE) {
    int write_count = write(write_fd, write_buf, *write_len);
    *write_len = 0;
        
    if (write_count <= 0) {
      return_value = 3;
    }
  }

  return return_value;
}

int main(int argc, char **argv) {
  if (argc < 4) {
    return 3;
  }

  char* first = argv[1];
  char* second = argv[2];
  char* third = argv[3];

  int return_value = 0;
  int read_fd = -1, write_digit_fd = -1, write_ndigit_fd = -1;

  read_fd = open(first, O_RDONLY, 0640);
  if (read_fd == -1) {
    return_value = 1;
    goto Exit;
  }

  write_digit_fd = open(second, O_WRONLY | O_CREAT | O_EXCL, 0640);
  if (write_digit_fd == -1) {
    return_value = 2;
    goto Exit;
  }

  write_ndigit_fd = open(third, O_WRONLY | O_CREAT | O_EXCL, 0640);
  if (write_ndigit_fd == -1) {
    return_value = 2;
    goto Exit;
  }

  char read_buf[BUFFER_SIZE_READ];
  char write_digit_buf[BUFFER_SIZE_WRITE];
  char write_ndigit_buf[BUFFER_SIZE_WRITE];

  int write_digit_len = 0;
  int write_ndigit_len = 0;
  int read_count = read(read_fd, read_buf, BUFFER_SIZE_READ);

  while (read_count > 0) {
    for (int i = 0; i < read_count; ++i) {
      int res = 0;
      if (isdigit(read_buf[i])) {
        res = HandleWrite(write_digit_fd, write_digit_buf, &write_digit_len, read_buf[i]);
      } else {
        res = HandleWrite(write_ndigit_fd, write_ndigit_buf, &write_ndigit_len, read_buf[i]);
      }

      if (res != 0) {
        return_value = res;
        goto Exit;
      }
    }

    read_count = read(read_fd, read_buf, BUFFER_SIZE_READ);
  }

  int write_count = write(write_digit_fd, write_digit_buf, write_digit_len);
  if (write_count <= 0) {
    return_value = 3;
    goto Exit;
  }

  write_count = write(write_ndigit_fd, write_ndigit_buf, write_ndigit_len);
  if (write_count <= 0) {
    return_value = 3;
    goto Exit;
  }

Exit:
  close(read_fd);
  close(write_digit_fd);
  close(write_ndigit_fd);

  return return_value;
}