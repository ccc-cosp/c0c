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

// Intel 指令集參考 -- https://web.itu.edu.tr/kesgin/mul06/intel/index.html
void xCode(char *op, char *_d, char *_p1, char *_p2) {
  char d[SMAX], p1[SMAX], p2[SMAX];
  as(_d, d); as(_p1, p1); as(_p2, p2);

  xEmit("# %s %s %s %s\n", op, d, p1, p2);
  if (strcmp(op, "fend") == 0) { xEmit("	ret\n"); }
  else if (strcmp(op, "call") == 0) { xEmit("	call %s\n", p1); }
  else if (strcmp(op, "global") == 0) { xEmit("	.comm	%s, 4, 2\n", d); }
  else if (strcmp(op, "jmp") == 0) { xEmit("	jmp	%s\n", d); }
  else if (strcmp(op, "return") == 0) { xEmit("	ret	%s\n", d); }
  else if (strcmp(op, "push") == 0) { xEmit("	push	%s\n", d); }
  else if (strcmp(op, "str") == 0) { xEmit("%s:\n	.ascii %s\n", d, p1); }
  else if (strcmp(op, "jz") == 0) { xEmit("	cmp	%s, 0\n	jeq	%s\n", p1, d); }
  else if (strcmp(op, "jnz") == 0) { xEmit("	cmp	%s, 0\n	jne	%s\n", p1, d); }
  else if (*op=='=') { xEmit("	movl	%s, eax\n	movl	eax, %s\n", p1, d); } 
  else if (strcmp(op, "function")==0) {
    xEmit("	.text\n	.globl	%s\n	.def	%s;	.scl	2;	.type	32;	.endef\n%s:\n", d, d, d, d);
    xEmit("	pushl	%%ebp\n	movl	%%esp, %%ebp\n");
  } else if (isMember(op, "~ + - * / & | ^ && ||")) {
    xEmit("	movl	%s, eax\n", p1);
    switch (*op) {
      // uniary operator
      case '~': xEmit("	notl	eax\n"); break;
      // binary operator
      case '+': xEmit("	addl	%s, eax\n", p2); break;
      case '-': xEmit("	subl	%s, eax\n", p2); break;
      case '*': xEmit("	imull	%s, eax\n", p2); break;
      case '/': xEmit("	cltd\n	idivl	%s, eax\n", p2); break; // ??　	movl	_a, %eax	movl	_b, %ecx	cltd	idivl	%ecx	 # D.1567
      case '&': xEmit("	andl	%s, eax\n", p2); break;
      case '|': xEmit("	orl	%s, eax\n", p2); break;
      case '^': xEmit("	xorl	%s, eax\n", p2); break;
    }
  } else if (isMember(op, "< > == != >= <=")) {
    // logical operator : 這在 x86 必須用跳躍指令處理 (參考程式最後的註解)，似乎如果有內建的 a lop b 就可以加速很多！
    xEmit("	movl %s, eax\n	movl %s, ebx\n	cmp eax, ebx\n", p1, p2);
    if (strcmp(op, "==")==0) xEmit("	sete al");
    if (strcmp(op, "!=")==0) xEmit("	setne al");
    if (strcmp(op, "<")==0) xEmit("	setl al");
    if (strcmp(op, "<=")==0) xEmit("	setle al");
    if (strcmp(op, ">")==0) xEmit("	setg al");
    if (strcmp(op, ">=")==0) xEmit("	setge al");
    xEmit("\n	movzbl	al, eax\n	movl	eax, %s\n", d);
  } else {
    xEmit(" --- not handle yet ! ----\n");
  }
}

/*
/*
 	movl	_a, %eax	 # a, D.1567
	testl	%eax, %eax	 # D.1567
	je	L2	 #,
	movl	_b, %eax	 # b, D.1567
	testl	%eax, %eax	 # D.1567
	je	L2	 #,
	movl	$1, %eax	 #, D.1567
	jmp	L3	 #
L2:
	movl	$0, %eax	 #, D.1567
L3:
	movl	%eax, _land	 # D.1567, land
*/