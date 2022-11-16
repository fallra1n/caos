#include <sys/syscall.h>

  .intel_syntax noprefix
  .text
  .global _start

#define BUFF_SIZE 65536
#define READ_NUMBER 0
#define WRITE_NUMBER 1
#define RETURN_CODE 0

_start:
  push rbx // cur char(8bit)
  push r12 // ptr to head of heap
  push r13 // size of cur line
  push r14 // start of heap
  push r15 // count of bytes
  push rbp
  mov rbp, rsp

  mov rax, SYS_brk
  mov rdi, 0
  syscall // get ptr to start heap

  mov r12, rax 
  mov r13, 0 
  mov r14, rax
  mov r15, BUFF_SIZE // 64Kb
  mov rcx, 0         // cur char (64bit)

AllocateMemory:
  mov rax, SYS_brk
  mov rdi, r12
  add rdi, r15
  syscall

Read:
  mov rax, SYS_read
  mov rdi, READ_NUMBER
  mov rsi, r12
  mov rdx, r15
  syscall
  
  cmp rax, -1
  jz End

  add r12, rax

  cmp rax, 0
  jnz AllocateMemory

Write:
  mov r13, 0

CharacterCount:
  // if (r12 == r14) => end of file
  cmp r12, r14
  jz LastPrint

  inc r13
  dec r12

  mov bl, BYTE PTR [r12] // get 1 byte from heap, bl - 8 bit of rbx
  movzx rcx, bl          // cast 1 byte -> 8 byte
  cmp rcx, 10            // 10 - \n
  jnz CharacterCount

Print:
  mov rax, SYS_write
  mov rdi, WRITE_NUMBER
  mov rsi, r12
  mov rdx, r13
  syscall

  mov r13, 0
  cmp r12, r14
  jnz CharacterCount
  jmp End

LastPrint:               // add line feed
  mov BYTE PTR [r12 + r13], 10
  mov rax, SYS_write
  mov rdi, WRITE_NUMBER
  mov rsi, r12
  add rsi, r13
  mov rdx, 1
  syscall
  jmp Print

End:
  mov rax, SYS_exit
  mov rdi, RETURN_CODE
  mov rsp, rbp
  pop r15
  pop r14
  pop r13
  pop r12
  pop rbp
  pop rbx
  syscall
