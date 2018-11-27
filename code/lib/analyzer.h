#ifndef __ANALYZER_H__
#define __ANALYZER_H__

#include <assert.h>
#include "strTable.h"
#include "lexer.h"

#define TMAX (1024*1024)

#define isEnd() (*token == '\0')

extern char strTable[];

extern int  isNext(char *set);
extern int  isNextType(int checkType);
extern char *next();
extern void error(char *filter);
extern char *skipType(int checkType);
extern char *skip(char *set);

#endif