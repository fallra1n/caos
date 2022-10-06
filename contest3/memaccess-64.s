  .text
  .global sum

sum:
  LoopBegin:
    cmp x1, 0
    beq LoopEnd
    ldr w8, [x2]
    add w0, w0, w8
    add x2, x2, 4
    sub x1, x1, 1
    b LoopBegin
  LoopEnd:
    ret
