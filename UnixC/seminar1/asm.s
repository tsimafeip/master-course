.intel_syntax noprefix

.text
    .globl _start

_start:
	mov rcx, 1000000
    getpid:
    	push rcx
	    mov rax, 39          
	    syscall
    	pop rcx
    	dec rcx
    	jnz getpid
    mov rax, 60         # system call 60 is exit
    mov rdi, 0          # we want return code 0
    syscall             # invoke operating system to exit

