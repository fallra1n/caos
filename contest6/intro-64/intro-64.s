  .global very_important_function
  .intel_syntax noprefix
  .text

  # N - rdi
  # A* - rsi
  # B* - rdx
  # R* - rcx

very_important_function:
  push rbp
  mov rbp, rsp

  mov rax, 0    # rax - counter
  vxorps ymm1, ymm1, ymm1
  vxorps ymm2, ymm2, ymm2
  vxorps ymm3, ymm3, ymm3
  vxorps ymm4, ymm4, ymm4
  vxorps ymm5, ymm5, ymm5

LoopBegin:
  # move aligned packed single-precision values
  vmovaps ymm1, [rsi + rax * 4]
  vmovaps ymm2, [rdx + rax * 4]

  vaddps ymm1, ymm1, ymm2 # ymm1 = ymm1 + ymm2
  vaddps ymm3, ymm3, ymm1 # ymm3 - sum of R

  vmovaps [rcx + rax * 4], ymm1

  add rax, 8
  cmp rax, rdi
  jl LoopBegin

  vperm2f128 ymm4, ymm4, ymm3, 3

  # складываем все 8 частей ymm3 в одну 
  vhaddps ymm3, ymm3, ymm4
  vhaddps ymm3, ymm3, ymm5
  vhaddps ymm3, ymm3, ymm5

  cvtsi2sd xmm0, rdi  # кастуем int -> double
  cvtss2sd xmm3, xmm3 # вытаскиваем сумму всех чисел

  vdivsd xmm3, xmm3, xmm0
  vmovapd xmm0, xmm3

  mov rsp, rbp
  pop rbp
  ret