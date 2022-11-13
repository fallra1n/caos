#include <sys/syscall.h>

extern long syscall(long number, ...);

const long COUNT = 512 * 1024; // 0,5 Mb

void _start() {
  char text[COUNT];
  syscall(SYS_brk, text + COUNT);
  long success_bytes = 0; 
  do {
    success_bytes = syscall(SYS_read, 0, text, COUNT);
    syscall(SYS_write, 1, text, success_bytes);
  } while (success_bytes != 0);
  
  syscall(SYS_exit, 0);
}