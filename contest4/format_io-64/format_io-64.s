.text
.global solve
.global I
.global O

solve:
  sub sp, sp, 16
  str x30, [sp, 12]

  adr x0, I
  add x1, sp, 4
  add x2, sp, 8
  bl scanf

  ldr w1, [sp, 4]
  ldr w2, [sp, 8]
  add w1, w1, w2
  adr x0, O
  bl printf

  ldr x30, [sp, 12]
  add sp, sp, 16
  mov x0, 0
  ret

.data
  I: .string "%d\n%d"
  O: .string "%d"
