#ifndef __X86_H__
#define __X86_H__

#include "strTable.h"
#include "map.h"
#include "util.h"

extern FILE *sFile; // x86 組合語言檔案

#define xEmit(...) do { fprintf(sFile, __VA_ARGS__); } while (0)

extern void xLabel(char *label);
extern void xCode(char *op, char *d, char *p1, char *p2);

#endif