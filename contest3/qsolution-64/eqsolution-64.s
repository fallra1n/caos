  .text
  .global solve

solve:
  mov w8, 0

  LoopBegin:
    madd w9, w0, w8, w1
    madd w9, w9, w8, w2
    madd w9, w9, w8, w3
    cmp w9, 0
    beq LoopEnd
    add w8, w8, 1
    b LoopBegin
  LoopEnd:
    mov w0, w8
    ret