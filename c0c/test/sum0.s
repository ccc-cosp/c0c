# file "sum.c"  
	.file "sum.c"
	.def	___main;	.scl	2;	.type	32;	.endef
# global a int 
	.comm	_a, 4, 2
# global b int 
	.comm	_b, 4, 2
# global c int 
	.comm	_c, 4, 2
# function sum int 7
	.text
	.globl	_sum
	.def	_sum;	.scl	2;	.type	32;	.endef
_sum:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
	subl	$32, %esp
# param n int 
  # do nothing
# local s int 0
# = s 0 
	movl	$0, %ebx
	movl	%ebx, _s
# local i int 1
# = i 1 
	movl	$1, %ebx
	movl	%ebx, _i
# local j int 2
# = j 2 
	movl	$2, %ebx
	movl	%ebx, _j
# local k int 3
# = k 3 
	movl	$3, %ebx
	movl	%ebx, _k
# local l int 4
# = l 4 
	movl	$4, %ebx
	movl	%ebx, _l
# = s 0 
	movl	$0, %ebx
	movl	%ebx, _s
# = i 1 
	movl	$1, %ebx
	movl	%ebx, _i
# label WBEGIN0  
_WBEGIN0:
# local t0  5
# <= t0 i n
	movl _i, %eax
	movl _n, %ebx
	cmpl %eax, %ebx
	setle %al
	movzbl	%al, %eax
	movl	%eax, _t0
# jnz WEND1 t0 
	movl	_t0, %eax
	cmpl	%eax, $0
	jne	_WEND1
# + t0 s i
	movl	_s, %eax
	addl	_i, %eax
	movl %eax, _t0
# = s t0 
	movl	_t0, %ebx
	movl	%ebx, _s
# + t0 i 1
	movl	_i, %eax
	addl	$1, %eax
	movl %eax, _t0
# = i t0 
	movl	_t0, %ebx
	movl	%ebx, _i
# jmp WBEGIN0  
	jmp	_WBEGIN0
# label WEND1  
_WEND1:
# return s  
	movl _s, %eax
	ret
# -function sum  
	leave
	ret
# -file "sum.c"  
	.ident	"c0c: 0.0.1"
	.def	_puts;	.scl	2;	.type	32;	.endef
