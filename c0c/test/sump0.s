# function total int 4
	.text
	.globl	_total
	.def	_total;	.scl	2;	.type	32;	.endef
_total:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
	subl	$32, %esp
# param a int* 
 --- not handle yet ! ----
# param size int 
 --- not handle yet ! ----
# local s int 
# = s 0 
	movl	$0, %ebx
	movl	%ebx, _s
# local i int 
# = i 0 
	movl	$0, %ebx
	movl	%ebx, _i
_WBEGIN0:
# < t0 i size
	movl _i, %eax
	movl _size, %ebx
	cmpl %eax, %ebx
	setl %al
	movzbl	%al, %eax
	movl	%eax, _t0
# jnz WEND1 t0 
	movl	_t0, %eax
	cmpl	%eax, $0
	jne	_WEND1
# *ptr t0 a 
 --- not handle yet ! ----
# + t1 s t0
	movl	_s, %eax
	addl	_t0, %eax
	movl %eax, _t1
# = s t1 
	movl	_t1, %ebx
	movl	%ebx, _s
# ++ a  
# + _a _a 1
	movl	_a, %eax
	addl	$1, %eax
	movl %eax, _a
# jmp WBEGIN0  
	jmp	_WBEGIN0
_WEND1:
# return s  
	movl _s, %eax
	ret
# -function total  
	leave
	ret
# -file "sump.c"  
	.ident	"c0c: 0.0.1"
	.def	_puts;	.scl	2;	.type	32;	.endef
