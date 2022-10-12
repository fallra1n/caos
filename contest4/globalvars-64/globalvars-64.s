  .text
  .global calculate
  .global R

calculate:
  ldr x1, A
  ldr x2, B
  ldr x3, C
  ldr x4, D

  mul x3, x3, x4
  madd x0, x1, x2, x3

  adr x5, R
  str x0, [x5]
  ret 

.data
R: .zero 8