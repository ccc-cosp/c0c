#include "vm.h"

int labelIdx = 0, tempIdx = 0;

void vmLabel(char *label) {
  vmEmit("%s:\n", label);
  xLabel(label);
}

void vmCode(char *op, char *d, char *p1, char *p2) {
  vmEmit("    %-10s %-10s %-10s %-10s\n", op, d, p1, p2);
  xCode(op, d, p1, p2);
}

char *vmNextLabel(char *prefix) {
  char name[SMAX];
  sprintf(name, "%s%d", prefix, labelIdx++);
  return strTableAdd(name);
}

char *vmNextTemp() {
  char name[SMAX];
  sprintf(name, "t%d", tempIdx++);
  return strTableAdd(name);
}