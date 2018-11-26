#ifndef __COMPILER_H__
#define __COMPILER_H__

#include "lexer.h"
#include "strTable.h"

extern FILE *pFile;
extern void compile(char *code);

#endif