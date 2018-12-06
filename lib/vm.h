#ifndef __VM_H__
#define __VM_H__

#include "strTable.h"
#include "map.h"
#include "util.h"
#include "x86.h"

typedef struct _VmCode {
  char *label, *op, *d, *p1, *p2; 
} VmCode;

extern FILE *pFile; // 虛擬碼檔案
extern int vmCodeTop, argIdx;
extern VmCode vmCodes[10000];

#define vmEmit(...) do { printf( __VA_ARGS__ ); fprintf(pFile, __VA_ARGS__); } while (0)
extern void vmInit();
extern VmCode *vmLabel(char *label);
extern VmCode *vmCode(char *op, char *d, char *p1, char *p2);
extern void vmDump();
extern void vmToAsm(char *file);

#endif