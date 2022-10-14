.text
.global temp

temp:
  mov x20, x30

LoopBegin:
  bl getchar_unlocked
  cmp x0, -1
  beq LoopEnd

  cmp x0, 57 // 57 is code of nine
  bgt LoopBegin

  cmp x0, 48 // 48 is code of zero
  blt LoopBegin

  bl putchar_unlocked
  b LoopBegin

LoopEnd:
  mov x0, 0
  mov x30, x20
  ret

.data
  CODE_OF_ZERO: .zero 8
  CODE_OF_NINE: .zero 8