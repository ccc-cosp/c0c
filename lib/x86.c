#include "x86.h"

void xInit(char *file) {
  char sFileName[SMAX];
  sprintf(sFileName, "%s0.s", file);
  sFile = fopen(sFileName, "wt");
}

void xClose() {
  fclose(sFile);
}

void as(char *x, char *rx) {
  if (isdigit(*x))
    sprintf(rx, "$%s", x);
  else if (isalpha(*x))
    sprintf(rx, "_%s", x);
  else
    sprintf(rx, "%s", x);
}

// Intel 指令集參考 -- https://web.itu.edu.tr/kesgin/mul06/intel/index.html
// https://www.aldeid.com/wiki/X86-assembly/Instructions
void xCode(char *op, char *_d, char *_p1, char *_p2) {
  char d[SMAX], p1[SMAX], p2[SMAX];
  as(_d, d); as(_p1, p1); as(_p2, p2);

  xEmit("# %s %s %s %s\n", op, _d, _p1, _p2);
  if (strcmp(op, "label") == 0) {
    xEmit("_%s:\n", _d);
  } else if (strcmp(op, "file") == 0) {
    xEmit("	.file %s\n", d);
    xEmit("	.def	___main;	.scl	2;	.type	32;	.endef\n");
    xEmit("	.comm	_t0, 4, 2\n	.comm	_t1, 4, 2\n	.comm	_t2, 4, 2\n	.comm	_t3, 4, 2\n	.comm	_t4, 4, 2\n	.comm	_t5, 4, 2\n	.comm	_t6, 4, 2\n	.comm	_t7, 4, 2\n	.comm	_t8, 4, 2\n");
  } else if (strcmp(op, "-file") == 0) {
    xEmit("	.ident	\"c0c: 0.0.1\"\n	.def	_puts;	.scl	2;	.type	32;	.endef\n");
  } else if (strcmp(op, "global") == 0) {
    xEmit("	.comm	%s, 4, 2\n", d);
  } else if (strcmp(op, "jmp") == 0) {
    xEmit("	jmp	%s\n", d);
  } else if (strcmp(op, "call") == 0) {
    xEmit("	call %s\n	movl %%eax, %s\n", p1, d);
    argIdx = 0;
  } else if (strcmp(op, "arg") == 0) {
    xEmit("	movl %s, %%eax\n", d);
    xEmit("	movl %%eax, %d(%%esp)\n", argIdx*4);
    argIdx ++;
  } else if (strcmp(op, "str") == 0) {
    char dt[SMAX], str[SMAX];
    sscanf(_p1, "\"%[^\"]*\"", str);
    // printf(" .... str:%s\n", tStr);
    sscanf(d, "$%s", dt);
    xEmit("	.section .rdata,\"dr\"\n%s:\n	.ascii \"%s\\0\"\n	.text\n", dt, str);
  } else if (strcmp(op, "jz") == 0) {
    xEmit("	movl	%s, %%eax\n	cmpl	$0, %%eax\n	je	%s\n", p1, d);
  } else if (strcmp(op, "jnz") == 0) {
    xEmit("	movl	%s, %%eax\n	cmpl	$0, %%eax\n	jne	%s\n", p1, d);
  } else if (strcmp(op, "++")==0) {
    xCode("+", d, d, "1");
  } else if (strcmp(op, "--")==0) {
    xCode("-", d, d, "1");
  } else if (*op=='=') {
    xEmit("	movl	%s, %%ebx\n	movl	%%ebx, %s\n", p1, d);
  } else if (strcmp(op, "function")==0) {
    xEmit("	.text\n	.globl	%s\n	.def	%s;	.scl	2;	.type	32;	.endef\n%s:\n", d, d, d, d);
    xEmit("	pushl	%%ebp\n	movl	%%esp, %%ebp\n");
    xEmit("	andl	$-16, %%esp\n	subl	$%d, %%esp\n", 32); // ((d+15)/16)*16
    if (strcmp(d, "_main") == 0) { xEmit("	call	___main\n"); }
  } else if (strcmp(op, "return") == 0) {
    xEmit("	movl %s, %%eax\n	leave\n	ret\n", d);
  } else if (strcmp(op, "-function") == 0) {
    xEmit("	movl $0, %%eax\n	leave\n	ret\n"); // leave = movl %ebp, %esp + popl %ebp
  } else if (strcmp(op, "param")==0) {
    xEmit("  # do nothing\n");
  } else if (strcmp(op, "local") == 0) {
    xEmit("  # do nothing\n");
  } else if (isMember(op, "~ + - * / & | ^ && ||")) {
    xEmit("	movl	%s, %%eax\n", p1);
    switch (*op) {
      // uniary operator
      case '~': xEmit("	notl	%%eax\n"); break;
      // binary operator
      case '+': xEmit("	addl	%s, %%eax\n", p2); break;
      case '-': xEmit("	subl	%s, %%eax\n", p2); break;
      case '*': xEmit("	imull	%s, %%eax\n", p2); break;
      case '/': xEmit("	cltd\n	idivl	%s, %%eax\n", p2); break; // ??　	movl	_a, %eax	movl	_b, %ecx	cltd	idivl	%ecx	 # D.1567
      case '&': xEmit("	andl	%s, %%eax\n", p2); break;
      case '|': xEmit("	orl	%s, %%eax\n", p2); break;
      case '^': xEmit("	xorl	%s, %%eax\n", p2); break;
    }
    xEmit("	movl %%eax, %s\n", d);
  } else if (isMember(op, "< > == != >= <=")) {
    // logical operator : 這在 x86 必須用跳躍指令處理 (參考程式最後的註解)，似乎如果有內建的 a lop b 就可以加速很多！
    // http://www.fermimn.gov.it/linux/quarta/x86/cmp.htm
    // CMP subtracts the second operand from the first but, unlike the SUB instruction, does not store the result; only the flags are changed.
    // CMP is typically used in conjunction with conditional jumps and the SETcc instruction. 
    // https://c9x.me/x86/html/file_module_x86_id_288.html (注意有號數和無號數的比較方法是不同的)
    // The terms "above" and "below" are associated with the CF flag and refer to the relationship between two unsigned integer values. 
    // The terms "greater" and "less" are associated with the SF and OF flags and refer to the relationship between two signed integer values.
    xEmit("	movl %s, %%eax\n	movl %s, %%ebx\n	cmpl %%ebx, %%eax\n", p1, p2); // cmpl %%ebx, %%eax 才是 eax-ebx
    if (strcmp(op, "==")==0) xEmit("	sete %%al");
    if (strcmp(op, "!=")==0) xEmit("	setne %%al");
    if (strcmp(op, "<")==0) xEmit("	setl %%al");
    if (strcmp(op, "<=")==0) xEmit("	setle %%al");
    if (strcmp(op, ">")==0) xEmit("	setg %%al");
    if (strcmp(op, ">=")==0) xEmit("	setge %%al");
    xEmit("\n	movsbl	%%al, %%eax\n	movl	%%eax, %s\n", d); // 不能用 movzbl, 應該用 movsbl (move and sign extend)
  } else {
    xEmit(" --- not handle yet ! ----\n");
  }
}

void xAsm(char *op, char *d, char *p1, char *p2) {
  xCode(op, d, p1, p2);
}