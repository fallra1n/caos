.text
.global main

// x20 - len
// x21 - cur char
// x22 - start arr

main:
  mov x19, x30
  mov x20, 0
  mov x22, 0

LoopScanBegin:
  bl getchar_unlocked
  mov x21, x0
  cmp x21, -1
  beq LoopPrintBegin

  add x20, x20, 1
  mov x2, 4
  mul x1, x20, x2
  mov x0, x22
  bl realloc
  mov x22, x0
  mov x23, x20
  sub x23, x23, 1
  add x0, x0, x23
  str x21, [x0]
  b LoopScanBegin

LoopPrintBegin:
  cmp x20, 0
  beq LoopPrintEnd
  sub x20, x20, 1
  mov x23, x22
  add x23, x23, x20
  ldr x0, [x23]
  bl putchar_unlocked
  b LoopPrintBegin

LoopPrintEnd:
  mov x0, x22
  bl free
  mov x0, 0
  mov x30, x19
  ret