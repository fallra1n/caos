  .global dot_product
  .intel_syntax noprefix
  .text

  # rdi - N
  # rsi - A 
  # rdx - B 
  # ymm2 - res
  # rcx - counter

dot_product:
  push rbp
  mov rbp, rsp

  mov rcx, 0
  vxorps ymm0, ymm0, ymm0 # for A
  vxorps ymm1, ymm1, ymm1 # for B
  vxorps ymm2, ymm2, ymm2
  xorps xmm0, xmm0

LoopBegin:
  cmp rdi, 8
  jl Loop

  vmovups ymm0, [rsi + rax * 4]
  vmovups ymm1, [rdx + rax * 4]

  vmulps ymm0, ymm0, ymm1
  vaddps ymm2, ymm2, ymm0

  add rcx, 8
  cmp rcx, rdi
  jge LoopEnd

  jmp LoopBegin


LoopEnd:
  vperm2f128 ymm3, ymm3, ymm2, 3
  vhaddps ymm2, ymm2, ymm3
  vhaddps ymm2, ymm2, ymm4
  vhaddps ymm2, ymm2, ymm4
  vmovapd xmm0, xmm2

  cmp rdi, rcx
  jz End
  sub rcx, 8
  jnz Loop

Loop:
  movsd xmm1, [rsi + rdi * 4]
  movsd xmm2, [rdx + rdi * 4]
  addsd xmm1, xmm2
  addsd xmm0, xmm1
  inc rdi
  cmp rdi, rcx
  jz End

  jmp Loop

End:
  mov rsp, rbp
  pop rbp