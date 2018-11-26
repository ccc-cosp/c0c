#ifndef __VM_H__
#define __VM_H__

#include "strTable.h"
#include "util.h"

extern FILE *pFile;
extern int  labelIdx; 

#define emitPrint(...) do { printf( __VA_ARGS__ ); fprintf(pFile, __VA_ARGS__); } while (0)
extern void emitLabel(char *label);
extern void emit(char *op, char *d, char *p1, char *p2);
extern char *nextLabel(char *prefix);
extern char *nextTemp();

#endif