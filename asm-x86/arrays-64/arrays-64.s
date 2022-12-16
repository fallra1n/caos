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
  mov ebx, [rsi + 4 * rcx - 4]
  add ebx, [rdx + 4 * rcx - 4]
  mov [r8 + 4 * rcx - 4], ebx
  loop .Loop
 
  pop rbx
  ret
