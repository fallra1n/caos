#include <sys/syscall.h>

  .intel_syntax noprefix
  .text
  .global _start

_start:
  push rbx
  push rbp
  push r12
  push r13
  push r14
  push r15
  mov rbp, rsp
  sub rsp, 65536 # 64 Kb

LoopReadWriteBegin:
  mov rax, SYS_read
  mov rdi, 0
  mov rsi, rsp
  mov rdx, 65536 # 64 Kb
  syscall

  cmp rax, 0
  jle LoopEnd

  mov rdx, rax
  mov rax, SYS_write
  mov rdi, 1
  mov rsi, rsp
  syscall

  cmp rax, -1
  jz LoopEnd
  
  jmp LoopReadWriteBegin

LoopEnd:
  mov rax, SYS_exit
  mov rdi, 0
  mov rsp, rbp
  pop r15
  pop r14
  pop r13
  pop r12
  pop rbp
  pop rbx
  syscall