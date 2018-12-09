#include "vm.h"

int localTop = 0, tempTop = 0;
int argIdx = 0, labelIdx = 0, tempIdx = 0;
int vmCodeTop = 0, vmGlobalTop = 0;
VmCode vmCodes[10000], vmGlobals[1000], *fCode = NULL;

void vmInit() {
  vmCodeTop = 0;
  vmGlobalTop = 0;
}

VmCode *vmCode(char *op, char *d, char *p1, char *p2) {
  VmCode c = { .op=op, .d=d, .p1=p1, .p2=p2 };
  vmCodes[vmCodeTop] = c;
  VmCode *pCode = &vmCodes[vmCodeTop++];
  if (strcmp(op, "function")==0) {
    localTop = 0;
    tempTop = 0;
    fCode = pCode;
  }
  if (strcmp(op, "local")==0) localTop++;
  if (strcmp(op, "-function")==0) {
    fCode->p2 = stPrint("%d", localTop);
  }
  return pCode;
}

VmCode *vmLabel(char *label) {
  return vmCode("label", label, "", "");
}

VmCode *vmGlobal(char *op, char *d, char *p1, char *p2) {
  VmCode c = { .op=op, .d=d, .p1=p1, .p2=p2 };
  vmGlobals[vmGlobalTop] = c;
  return &vmGlobals[vmGlobalTop++];
}

char *vmNextLabel(char *prefix) {
  return stPrint("%s%d", prefix, labelIdx++);
}

char *vmNextTemp() {
  char *temp = stPrint("t%d", tempIdx++);
  if (tempIdx > tempTop) {
    char *local = stPrint("%d", localTop);
    vmCode("local", temp, "", local);
    tempTop = tempIdx;
  }
  return temp;
}

void vmDump(VmCode *codes, int top) {
  printf("=============vmDump()==============\n");
  for (int i=0; i<top; i++) {
    VmCode *c = &codes[i];
    printf("%-10s %-10s %-10s %-10s\n", c->op, c->d, c->p1, c->p2);
  }
}

void vmToAsm(char *file) {
  xInit(file);
  printf("=============vmToAsm()==============\n");
  for (int i=0; i<vmGlobalTop; i++) {
    VmCode *c = &vmGlobals[i];
    xAsm(c->op, c->d, c->p1, c->p2);
  }
  for (int i=0; i<vmCodeTop; i++) {
    VmCode *c = &vmCodes[i];
    xAsm(c->op, c->d, c->p1, c->p2);
  }
  xClose();
}