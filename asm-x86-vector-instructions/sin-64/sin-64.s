  .global avx_sin
  .intel_syntax noprefix
  .text

  # xmm0 - x
  # xmm1 - -1
  # xmm2 - cur result
  # xmm3 - old res
  # xmm4 - x ^ i
  # xmm5 - cur x ^ i
  # rdx - counter for fact
  # rsi - cur fact
  # r8 - flag

avx_sin:
  push rbp
  mov rbp, rsp

  mov rsi, 1
  mov rdx, 1
  mov rcx, 0
  mov r8, 1
  movsd xmm2, xmm0
  movsd xmm3, xmm0
  movsd xmm4, xmm0

  mov rdi, -1

LoopBegin:
  inc rcx
  cmp rcx, 15
  jz LoopEnd
  mulsd xmm4, xmm0
  mulsd xmm4, xmm0
  add rdx, 1
  imul rsi, rdx
  add rdx, 1
  imul rsi, rdx

  cvtsi2sd xmm6, rsi
  movsd xmm5, xmm4
  divsd xmm5, xmm6

  cmp r8, 1
  jnz Adding
  jz Subtract

Adding:
  mov r8, 1
  addsd xmm2, xmm5
  ucomisd xmm2, xmm3
  jz LoopEnd
  movsd xmm3, xmm2
  jmp LoopBegin

Subtract:
  mov r8, 0
  subsd xmm2, xmm5
  ucomisd xmm2, xmm3
  jz LoopEnd
  movsd xmm3, xmm2
  jmp LoopBegin

LoopEnd:
  movsd xmm0, xmm2
  mov rsp, rbp
  pop rbp
  ret