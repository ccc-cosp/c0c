
#include "strTable.h"

char *strTable, *strTableEnd;
int maxLen;

char *strTableInit(char *pStrTable, int pMaxLen) {
  strTable = pStrTable;
  maxLen = pMaxLen;
  strTableEnd = strTable;
}

char *strTableAdd(char *str) {
  char *strPtr = strTableEnd;
  strcpy(strTableEnd, str);
  strTableEnd += (strlen(str)+1);
  return strPtr;
}