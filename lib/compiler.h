#ifndef __COMPILER_H__
#define __COMPILER_H__

#include "util.h"
#include "scan.h"
#include "vm.h"

#define SYMMAX 10000

enum {Global, Local, Inner};
typedef char*(*F0)();
extern char code[];
extern int labelIdx, tempIdx, tempMax; // 臨時變數數量

extern char *nextLabel(char *prefix);
extern char *nextTemp();
extern char *typeStar(char *type, char *star);
extern char *F();
extern char *E();
extern char *EXP();
extern void WHILE();
extern void IF();
extern void BLOCK(int scope);
extern void RETURN();
extern void STMT();
extern void VAR(int scope);
extern char *CALL(char *id);
extern void PROG();
extern void compile(char *file, char *ext, char *code);
extern void compileFile(char *file, char *ext);

#endif
