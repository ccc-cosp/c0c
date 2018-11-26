#include "vm.h"

FILE *pFile;
int labelIdx = 0;

void emitLabel(char *label) {
  emitPrint("(%s)\n", label);
}

void emit(char *op, char *d, char *p1, char *p2) {
  emitPrint("    %-10s %-10s %-10s %-10s\n", op, d, p1, p2);
}

char *nextLabel(char *prefix) {
  char name[SMAX];
  sprintf(name, "%s%d", prefix, labelIdx++);
  return strTableAdd(name);
}

char *nextTemp() {
  return nextLabel("t");
}