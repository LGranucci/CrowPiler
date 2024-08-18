 .global main
main:
movq $2, %rax
not %rax
push %rax
movq $3, %rax
pop %rcx
addq %rcx, %rax
ret
