#include "vm.h"

int vmCodeTop = 0, argIdx = 0, localTop = 0, vmGlobalTop = 0;
VmCode vmCodes[10000], vmGlobalCodes[1000];

void vmInit() {
  vmCodeTop = 0;
  vmGlobalTop = 0;
}

VmCode *vmCode(char *op, char *d, char *p1, char *p2) {
  VmCode c = { .op=op, .d=d, .p1=p1, .p2=p2 };
  if (strcmp(op, "local")==0) localTop++;
  vmCodes[vmCodeTop] = c;
  return &vmCodes[vmCodeTop++];
}

VmCode *vmLabel(char *label) {
  printf("--- vmLabel:%s\n", label);
  return vmCode("label", label, "", "");
}

VmCode *vmGlobalCode(char *op, char *d, char *p1, char *p2) {
  VmCode c = { .op=op, .d=d, .p1=p1, .p2=p2 };
  vmGlobalCodes[vmGlobalTop] = c;
  return &vmGlobalCodes[vmGlobalTop++];
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
    VmCode *c = &vmGlobalCodes[i];
    xAsm(c->op, c->d, c->p1, c->p2);
  }
  for (int i=0; i<vmCodeTop; i++) {
    VmCode *c = &vmCodes[i];
    xAsm(c->op, c->d, c->p1, c->p2);
  }
  xClose();
}