.text
.global main

// x20 - len
// x21 - cur char
// x22 - start arr
// x23 - capacity

main:
  sub sp, sp, 48
  str x30, [sp, 0]
  str x20, [sp, 8]
  str x21, [sp, 16]
  str x22, [sp, 24]
  str x23, [sp, 32]

  mov x20, 0
  mov x22, 0
  mov x23, 1

LoopScanBegin:
  bl getchar_unlocked
  mov x21, x0
  cmp x21, -1
  beq LoopPrintBegin

  add x20, x20, 1

  cmp x20, x23
  beq Realloc
  bne NoRealloc

Realloc:
  mov x3, 2
  mul x23, x23, x3
  mov x2, 4
  mul x2, x2, x3
  mul x1, x20, x2
  mov x0, x22
  bl realloc
  mov x22, x0
  bl NoRealloc

NoRealloc:
  mov x24, x20
  mov x2, 4
  mul x24, x24, x2
  sub x24, x24, 4
  add x22, x22, x24
  str x21, [x22]
  sub x22, x22, x24
  b LoopScanBegin

LoopPrintBegin:
  cmp x20, 0
  beq LoopPrintEnd
  sub x20, x20, 1
  mov x23, x22
  mov x2, 4
  mov x24, x20
  mul x24, x24, x2
  add x23, x23, x24
  ldr x0, [x23]
  bl putchar_unlocked
  b LoopPrintBegin

LoopPrintEnd:
  mov x0, x22
  bl free
  
  ldr x30, [sp, 0]
  ldr x20, [sp, 8]
  ldr x21, [sp, 16]
  ldr x22, [sp, 24]
  ldr x23, [sp, 32]
  add sp, sp, 48

  mov x0, 0
  ret