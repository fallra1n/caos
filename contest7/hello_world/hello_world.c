#include <sys/syscall.h>

extern long syscall(long number, ...);

const char HELLO_WORLD[] = "Hello, World!";

void _start() {
  syscall(SYS_write, 1, HELLO_WORLD, sizeof(HELLO_WORLD) - 1);
  syscall(SYS_exit, 0);
}