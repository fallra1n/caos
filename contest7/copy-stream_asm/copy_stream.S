#include <sys/syscall.h>

  .intel_syntax noprefix
  .text
  .global _start

#define BUFF_SIZE 65536
#define READ_NUMBER 0
#define WRITE_NUMBER 1
#define RETURN_CODE 0

_start:
  push rbp
  mov rbp, rsp
  sub rsp, BUFF_SIZE

LoopReadWriteBegin:
  mov rax, SYS_read
  mov rdi, READ_NUMBER
  mov rsi, rsp
  mov rdx, BUFF_SIZE
  syscall

  cmp rax, 0
  jle LoopEnd

  mov rdx, rax
  mov rax, SYS_write
  mov rdi, WRITE_NUMBER
  mov rsi, rsp
  syscall

  cmp rax, -1
  jz LoopEnd
  
  jmp LoopReadWriteBegin

LoopEnd:
  mov rax, SYS_exit
  mov rdi, RETURN_CODE
  mov rsp, rbp
  pop rbp
  syscall