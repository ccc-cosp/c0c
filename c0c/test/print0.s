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
# str $S2 "n>5\n" 
	.section .rdata,"dr"
S2:
	.ascii "n>5\n\0"
	.text
# str $S3 "n<=5\n" 
	.section .rdata,"dr"
S3:
	.ascii "n<=5\n\0"
	.text
# str $S4 "Hello %d!\n" 
	.section .rdata,"dr"
S4:
	.ascii "Hello %d!\n\0"
	.text
# function main int 2
	.text
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
	subl	$32, %esp
	call	___main
# = n 7 
	movl	$7, %ebx
	movl	%ebx, _n
# local t0  0
  # do nothing
# > t0 n 5
	movl _n, %eax
	movl $5, %ebx
	cmpl %ebx, %eax
	setg %al
	movsbl	%al, %eax
	movl	%eax, _t0
# jz ELSE0 t0 
	movl	_t0, %eax
	cmpl	$0, %eax
	je	_ELSE0
# arg $S2  
	movl $S2, %eax
	movl %eax, 0(%esp)
# call t0 printf 
	call _printf
	movl %eax, _t0
# return 7  
	movl $7, %eax
	leave
	ret
# jmp ENDIF1  
	jmp	_ENDIF1
# label ELSE0  
_ELSE0:
# arg $S3  
	movl $S3, %eax
	movl %eax, 0(%esp)
# call t0 printf 
	call _printf
	movl %eax, _t0
# label ENDIF1  
_ENDIF1:
# arg $S4  
	movl $S4, %eax
	movl %eax, 0(%esp)
# arg n  
	movl _n, %eax
	movl %eax, 4(%esp)
# call t0 printf 
	call _printf
	movl %eax, _t0
# return 8  
	movl $8, %eax
	leave
	ret
# -function main  
	movl $0, %eax
	leave
	ret
# -file "print.c"  
	.ident	"c0c: 0.0.1"
	.def	_puts;	.scl	2;	.type	32;	.endef
