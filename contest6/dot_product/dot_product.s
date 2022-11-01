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
  vxorps ymm3, ymm3, ymm3
  vxorps ymm4, ymm4, ymm4
  xorps xmm0, xmm0

LoopBegin:
  cmp rdi, 8
  jl Loop

  mov r8, rcx
  add r8, 8
  cmp r8, rdi
  jg LoopEnd

  vmovups ymm0, [rsi + rcx * 4]
  vmovups ymm1, [rdx + rcx * 4]

  vmulps ymm0, ymm0, ymm1
  vaddps ymm2, ymm2, ymm0

  add rcx, 8

  jmp LoopBegin


LoopEnd:
  vperm2f128 ymm3, ymm3, ymm2, 3
  vhaddps ymm2, ymm2, ymm3
  vhaddps ymm2, ymm2, ymm4
  vhaddps ymm2, ymm2, ymm4
  vmovapd xmm0, xmm2

  cmp rdi, rcx
  jz End
  jmp Loop

Loop:
  movss xmm1, [rsi + rcx * 4]
  movss xmm2, [rdx + rcx * 4]
  mulss xmm1, xmm2
  addss xmm0, xmm1
  inc rcx
  cmp rdi, rcx
  jz End

  jmp Loop

End:
  mov rsp, rbp
  pop rbp
  ret