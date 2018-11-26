#ifndef __COMPILER_H__
#define __COMPILER_H__

#include "util.h"
#include "analyzer.h"
#include "vm.h"

enum { Global, Local, Param, Inner };
extern char *scopeName[];
extern char code[];

extern char *F();
extern char *E();
extern void WHILE();
extern void IF();
extern void BLOCK(int scope);
extern void RETURN();
extern void STMT();
extern void VAR(int scope);
extern char *CALL(char *id);
extern void PROG();
void compile(char *code);
void compileFile(char *file, char *ext);

#endif