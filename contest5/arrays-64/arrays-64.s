  .global sum
  .intel_syntax noprefix
  .text

# N - rdi
# A - rsi
# B - rdx
# R - rcx(r8)

sum:
  push rbx
  mov r8, rcx
  mov rcx, rdi
.Loop:
  mov ebx, 0
  mov r9, rcx
  dec r9
  add ebx, [rsi + 4 * r9]
  add ebx, [rdx + 4 * r9]
  mov [r8 + 4 * r9], ebx
  loop .Loop
 
  pop rbx
  ret
