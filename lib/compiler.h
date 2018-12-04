#ifndef __COMPILER_H__
#define __COMPILER_H__

#include "util.h"
#include "analyzer.h"
#include "vm.h"

typedef char*(*F0)();
extern char Global[], Local[], Param[], Inner[];
extern char code[];
extern Pair symLocalList[], symGlobalList[];
extern Map symLocalMap, symGlobalMap;

extern char *F();
extern char *E();
extern char *EXP();
extern void WHILE();
extern void IF();
extern void BLOCK(char *scope);
extern void RETURN();
extern void STMT();
extern void VAR(char *scope);
extern char *CALL(char *id);
extern void PROG();
void compile(char *code);
void compileFile(char *file, char *ext);

#endif