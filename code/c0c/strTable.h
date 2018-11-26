#ifndef __STRTABLE_H__
#define __STRTABLE_H__

#include <string.h>

#define TMAX (1024*1024)
extern char *strTableInit();
extern char *strTableAdd(char *str);

#endif
