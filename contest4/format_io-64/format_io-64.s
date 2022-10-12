.text
.global func_for_testing
.global I
.global O

func_for_testing:
  mov x19, x30
  mov x20, sp
  sub sp, sp, 16
  

  adr x0, I
  sub x1, x20, 4
  sub x2, x20, 0
  bl scanf

  ldr w1, [x20, -4]
  ldr w2, [x20, 0]
  add w1, w1, w2
  adr x0, O
  bl printf

  mov sp, x20
  mov x30, x19
  mov x0, 0
  ret

.data
  I: .string "%d\n%d"
  O: .string "%d"

