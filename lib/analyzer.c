#include "analyzer.h"

char strTable[TMAX];

int isNext(char *set) {
  return isMember(token, set);
}

int isNextType(int checkType) {
  return (*token != '\0' && type == checkType);
}

char *next() {
  // printf("    token=%-10s tokenIdx=%d\n", token, tokenIdx);
  char *t = strTableAdd(token);
  lexScan();
  return t;
}

void error(char *filter) {
  printf("skip(%s) got %s fail!\n", filter, token);
  printf("-- line: %d pos: %d\n", line, pos);
  char errorCode[SMAX];
  strncpy(errorCode, start, SMAX-1);
  errorCode[SMAX-1] = '\0';
  printf("-- error code: %s\n", errorCode);
  assert(0);
}

char *skipType(int checkType) {
  if (type != checkType) error(typeName[checkType]);
  return next();
}

char *skip(char *set) {
  if (!isNext(set)) error(set);
  return next();
}
