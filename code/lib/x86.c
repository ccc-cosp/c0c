#include "x86.h"

void xLabel(char *label) {
  xEmit("(%s)\n", label);
}

void xCode(char *op, char *d, char *p1, char *p2) {
  xEmit("# %-10s %-10s %-10s %-10s\n", op, d, p1, p2);
  if (*op=='=') {
    xEmit("movl %s eax\n", p1);
    xEmit("movl eax %s\n", d);
  }
  if (strchr("+-*/", *op) != NULL) {
    xEmit("movl %s eax\n", p1);
    switch (*op) {
      case '+': xEmit("addl %s eax\n", p2); break;
      case '-': xEmit("subl %s eax\n", p2); break;
      case '*': xEmit("mull %s eax\n", p2); break;
      case '/': xEmit("divl %s eax\n", p2); break;
    }
    xEmit("movl eax %s\n", d);
  }
}

/*
	.text
	.globl	_sum
	.def	_sum;	.scl	2;	.type	32;	.endef
_sum:
	pushl	%ebp	 #
	movl	%esp, %ebp	 #,
	subl	$16, %esp	 #,
	movl	$0, -4(%ebp)	 #, s
	movl	$1, -8(%ebp)	 #, i
	jmp	L2	 #
L3:
	movl	-8(%ebp), %eax	 # i, tmp89
	addl	%eax, -4(%ebp)	 # tmp89, s
	addl	$1, -8(%ebp)	 #, i
L2:
	movl	-8(%ebp), %eax	 # i, tmp90
	cmpl	8(%ebp), %eax	 # n, tmp90
	jle	L3	 #,
	movl	-4(%ebp), %eax	 # s, D.1492
	leave
	ret
	.ident	"GCC: (tdm-1) 5.1.0"
*/