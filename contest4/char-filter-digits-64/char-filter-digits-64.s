.text
.global main

main:
  mov x20, x30

LoopBegin:
  bl getchar_unlocked
  cmp x0, -1
  beq LoopEnd

  cmp x0, 57 // code of nine
  bgt LoopBegin

  cmp x0, 48 // code of zero
  blt LoopBegin

  bl putchar_unlocked
  b LoopBegin

LoopEnd:
  mov x0, 0
  mov x30, x20
  ret