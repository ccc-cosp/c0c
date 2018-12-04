#ifndef __VM_H__
#define __VM_H__

#include "strTable.h"
#include "map.h"
#include "util.h"
#include "x86.h"

extern FILE *pFile; // 虛擬碼檔案
extern int labelIdx, frameSize, tempIdx; // 臨時變數數量
extern Pair symLocalList[], symGlobalList[];
extern Map symLocalMap, symGlobalMap;

#define vmEmit(...) do { printf( __VA_ARGS__ ); fprintf(pFile, __VA_ARGS__); } while (0)
extern void vmInit();
extern void vmLabel(char *label);
extern void vmCode(char *op, char *d, char *p1, char *p2);
extern char *vmNextLabel(char *prefix);
extern char *vmNextTemp();

#endif