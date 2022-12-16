.text
.global main
.global I
.global O

main:
  sub sp, sp, 16
  str x30, [sp, 8]

  adr x0, I
  add x1, sp, 0
  add x2, sp, 4
  bl scanf

  ldr w1, [sp, 0]
  ldr w2, [sp, 4]
  add w1, w1, w2
  adr x0, O
  bl printf

  ldr x30, [sp, 8]
  add sp, sp, 16
  mov x0, 0
  ret

.data
  I: .string "%d\n%d"
  O: .string "%d"
