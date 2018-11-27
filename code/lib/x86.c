#include "x86.h"

char temp[SMAX];

void as(char *x, char *rx) {
  if (isdigit(*x))
    sprintf(rx, "$%s", x);
  else if (isalpha(*x))
    sprintf(rx, "_%s", x);
  else
    sprintf(rx, "%s", x);
}

void xLabel(char *label) {
  xEmit("%s:\n", label);
}

void xCode(char *op, char *_d, char *_p1, char *_p2) {
  char d[SMAX], p1[SMAX], p2[SMAX];
  as(_d, d); as(_p1, p1); as(_p2, p2);

  xEmit("# %s %s %s %s\n", op, d, p1, p2);
  if (strcmp(op, "function")==0) {
    xEmit("	.text\n	.globl	%s\n	.def	%s;	.scl	2;	.type	32;	.endef\n%s:\n", d, d, d, d);
    xEmit("	pushl	%%ebp\n	movl	%%esp, %%ebp\n");
  }
  if (strcmp(op, "fend") == 0) { xEmit("	ret\n"); }
  if (strcmp(op, "call") == 0) { xEmit("	call %s\n", p1); }

  if (strcmp(op, "global") == 0) { xEmit("	.comm	%s, 4, 2\n", d); }
  if (strcmp(op, "jz") == 0) { xEmit("	jeq	%s	%s\n", d, p1); }
  if (strcmp(op, "jnz") == 0) { xEmit("	jne	%s	%s\n", d, p1); }
  if (strcmp(op, "jmp") == 0) { xEmit("	jmp	%s\n", d); }
  if (strcmp(op, "return") == 0) { xEmit("	ret	%s\n", d); }
  if (strcmp(op, "push") == 0) { xEmit("	push	%s\n", d); }
  if (strcmp(op, "str") == 0) { xEmit("%s:\n	.ascii %s\n", d, p1); }
  if (*op=='=') {
    xEmit("	movl	%s, eax\n", p1);
    xEmit("	movl	eax, %s\n", d);
  }
  if (strchr("+-*/", *op) != NULL) {
    xEmit("	movl	%s, eax\n", p1);
    switch (*op) {
      case '+': xEmit("	addl	%s, eax\n", p2); break;
      case '-': xEmit("	subl	%s, eax\n", p2); break;
      case '*': xEmit("	mull	%s, eax\n", p2); break;
      case '/': xEmit("	divl	%s, eax\n", p2); break;
    }
    xEmit("	movl	eax, %s\n", d);
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