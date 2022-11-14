#include <sys/syscall.h>

extern long syscall(long number, ...);

const long COUNT = 64 * 1024; // 64 Kb

void _start() {
  char text[COUNT];
  long success_bytes_r = 0;
  long success_bytes_w = 0;
  do {
    success_bytes_r = syscall(SYS_read, 0, text, COUNT);

    if (success_bytes_r != -1) {
      success_bytes_w = syscall(SYS_write, 1, text, success_bytes_r);
    }
  } while (success_bytes_r > 0 && success_bytes_w > 0);
  
  syscall(SYS_exit, 0);
}