# file "print.c"  
	.file "print.c"
	.def	___main;	.scl	2;	.type	32;	.endef
	.comm	_t0, 4, 2
	.comm	_t1, 4, 2
	.comm	_t2, 4, 2
	.comm	_t3, 4, 2
	.comm	_t4, 4, 2
	.comm	_t5, 4, 2
	.comm	_t6, 4, 2
	.comm	_t7, 4, 2
	.comm	_t8, 4, 2
# global n int 
	.comm	_n, 4, 2
# function main int 
	.text
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
	subl	$32, %esp
	call	___main
# = n 3 
	movl	$3, %ebx
	movl	%ebx, _n
# str $S0 "Hello %d!\n" 
	.section .rdata,"dr"
S0:
	.ascii "Hello %d!\n\0"
	.text
# arg $S0  
	movl $S0, %eax
	movl %eax, 0(%esp)
# arg n  
	movl _n, %eax
	movl %eax, 4(%esp)
# call t0 printf 
	call _printf
	movl %eax, _t0
# -function main  
	leave
	ret
# -file "print.c"  
	.ident	"c0c: 0.0.1"
	.def	_puts;	.scl	2;	.type	32;	.endef
