#ifndef __COMPILER_H__
#define __COMPILER_H__

#include "util.h"
#include "scan.h"
#include "vm.h"
#include "map.h"

#define SYMMAX 10000

typedef char*(*F0)();
extern char Global[], Local[], Param[], Inner[];
extern char code[];
extern int labelIdx, tempIdx, tempMax; // 臨時變數數量 // argIdx, 
extern Pair symLocalList[], symGlobalList[];
extern Map symLocalMap, symGlobalMap;

extern char *nextLabel(char *prefix);
extern char *nextTemp();
extern char *typeStar(char *type, char *star);
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