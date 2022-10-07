.text
.global memset

// x0 - ptr
// x1 - value
// x2 - size

memset:
  LoopBegin:
    cmp x2, xzr
    beq LoopEnd
    str x1, [x0] 
    add x0, x0, 4
    sub x2, x2, 1
    b LoopBegin
  LoopEnd:
    ret