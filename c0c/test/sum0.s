# extern printf  
# file "sum.c"  
	.file "sum.c"
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
# global a int 
	.comm	_a, 4, 2
# global b int 
	.comm	_b, 4, 2
# global c int 
	.comm	_c, 4, 2
# function sum int 6
	.text
	.globl	_sum
	.def	_sum;	.scl	2;	.type	32;	.endef
_sum:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
	subl	$32, %esp
# param n int P0
# local s int L0
# = s 0 
	movl	$0, %ebx
	movl	%ebx, _s
# local i int L1
# = i 1 
	movl	$1, %ebx
	movl	%ebx, _i
# local j int L2
# = j 2 
	movl	$2, %ebx
	movl	%ebx, _j
# local k int L3
# = k 3 
	movl	$3, %ebx
	movl	%ebx, _k
# local l int L4
# = l 4 
	movl	$4, %ebx
	movl	%ebx, _l
# = s 0 
	movl	$0, %ebx
	movl	%ebx, _s
# = i 1 
	movl	$1, %ebx
	movl	%ebx, _i
# label _WBEGIN0  
__WBEGIN0:
# local t0  L5
# <= t0 i n
	movl _i, %eax
	movl _n, %ebx
	cmpl %ebx, %eax
	setle %al
	movsbl	%al, %eax
	movl	%eax, _t0
# jz _WEND1 t0 
	movl	_t0, %eax
	cmpl	$0, %eax
	je	_WEND1
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
# jmp _WBEGIN0  
	jmp	_WBEGIN0
# label _WEND1  
__WEND1:
# return s  
	movl _s, %eax
	leave
	ret
# -function sum  
	movl $0, %eax
	leave
	ret
# -file "sum.c"  
	.ident	"c0c: 0.0.1"
	.def	_puts;	.scl	2;	.type	32;	.endef
