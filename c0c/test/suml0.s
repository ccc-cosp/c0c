
#  extern printf  

#  file "suml.c"  
	.file "suml.c"
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

#  str _Str0 "i=%d s=%d n=%d\n" 
	.section .rdata,"dr"
_Str0:
	.ascii "i=%d s=%d n=%d\n\0"
	.text

#  str _Str3 "i=%d s=%d\n" 
	.section .rdata,"dr"
_Str3:
	.ascii "i=%d s=%d\n\0"
	.text

#  str _Str4 "sum(10)=%d\n" 
	.section .rdata,"dr"
_Str4:
	.ascii "sum(10)=%d\n\0"
	.text

#  function sum int 
	.text
	.globl	_sum
	.def	sum;	.scl	2;	.type	32;	.endef
_sum:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
	subl	$48, %esp

#  param n int 

#  local s int 

#  = s 0 
	movl	$0, %ebx
	movl	%ebx, -4(%ebp)

#  local i int 

#  = i 1 
	movl	$1, %ebx
	movl	%ebx, -8(%ebp)

#  arg _Str0  
	movl $_Str0, %eax
	movl %eax, 0(%esp)

#  arg i  
	movl -8(%ebp), %eax
	movl %eax, 4(%esp)

#  arg s  
	movl -4(%ebp), %eax
	movl %eax, 8(%esp)

#  arg n  
	movl 8(%ebp), %eax
	movl %eax, 12(%esp)

#  local t0  

#  call t0 printf 
	call _printf
	movl %eax, -12(%ebp)

#  label _WBEGIN1  
_WBEGIN1:

#  <= t0 i n
	movl -8(%ebp), %eax
	movl 8(%ebp), %ebx
	cmpl %ebx, %eax
	setle %al
	movsbl	%al, %eax
	movl	%eax, -12(%ebp)

#  jz _WEND2 t0 
	movl	-12(%ebp), %eax
	cmpl	$0, %eax
	je	_WEND2

#  + t0 s i
	movl	-4(%ebp), %eax
	addl	-8(%ebp), %eax
	movl %eax, -12(%ebp)

#  = s t0 
	movl	-12(%ebp), %ebx
	movl	%ebx, -4(%ebp)

#  arg _Str3  
	movl $_Str3, %eax
	movl %eax, 0(%esp)

#  arg i  
	movl -8(%ebp), %eax
	movl %eax, 4(%esp)

#  arg s  
	movl -4(%ebp), %eax
	movl %eax, 8(%esp)

#  call t0 printf 
	call _printf
	movl %eax, -12(%ebp)

#  + t0 i 1
	movl	-8(%ebp), %eax
	addl	$1, %eax
	movl %eax, -12(%ebp)

#  = i t0 
	movl	-12(%ebp), %ebx
	movl	%ebx, -8(%ebp)

#  jmp _WBEGIN1  
	jmp	_WBEGIN1

#  label _WEND2  
_WEND2:

#  return s  
	movl -4(%ebp), %eax
	leave
	ret

#  -function sum  
	movl $0, %eax
	leave
	ret

#  function main int 
	.text
	.globl	_main
	.def	main;	.scl	2;	.type	32;	.endef
_main:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
	subl	$48, %esp
	call	___main

#  local total int 

#  arg 10  
	movl $10, %eax
	movl %eax, 0(%esp)

#  local t0  

#  call t0 sum 
	call _sum
	movl %eax, -8(%ebp)

#  = total t0 
	movl	-8(%ebp), %ebx
	movl	%ebx, -4(%ebp)

#  arg _Str4  
	movl $_Str4, %eax
	movl %eax, 0(%esp)

#  arg total  
	movl -4(%ebp), %eax
	movl %eax, 4(%esp)

#  call t0 printf 
	call _printf
	movl %eax, -8(%ebp)

#  -function main  
	movl $0, %eax
	leave
	ret

#  -file "suml.c"  
	.ident	"c0c: 0.0.1"
	.def	_puts;	.scl	2;	.type	32;	.endef
