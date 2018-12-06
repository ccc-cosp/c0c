#include "vm.h"

int vmCodeTop = 0, argIdx = 0;

void vmInit() {
  vmCodeTop = 0;
}

VmCode *vmLabel(char *label) {
  VmCode c = { .label = label };
  vmCodes[vmCodeTop] = c;
  /*
  vmEmit("%s:\n", label);
  xLabel(label);
  */
  return &vmCodes[vmCodeTop++];
}

VmCode *vmCode(char *op, char *d, char *p1, char *p2) {
  VmCode c = { .op=op, .d=d, .p1=p1, .p2=p2};
  // vmEmit("    %-10s %-10s %-10s %-10s\n", op, d, p1, p2);
  /*
  if (strcmp(op, "global") == 0) {
    mapAdd(&symGlobalMap, d, p1);
  } else if (strcmp(op, "local") == 0) {
    framePos[symLocalMap.top] = frameSize;
    char fpos[SMAX];
    sprintf(fpos, "%d", frameSize);
    c.p2 = strTableAdd(fpos);
    mapAdd(&symLocalMap, d, &framePos);
    // symTableAdd(d, "local", frameSize);
    frameSize += 4;
  }
  */
  vmCodes[vmCodeTop] = c;
  /*
  xCode(op, d, p1, p2);
  if (strcmp(op, "arg") == 0) { argIdx ++; }
  */
  return &vmCodes[vmCodeTop++];
}

void vmDump() {
  printf("=============vmDump()==============\n");
  for (int i=0; i<vmCodeTop; i++) {
    VmCode *c = &vmCodes[i];
    if (c->label != NULL)
      printf("%-10s:\n", c->label);
    else
      printf("  %-10s %-10s %-10s %-10s\n", c->op, c->d, c->p1, c->p2);
  }
}

void vmToAsm(char *file) {
  xInit(file);
  printf("=============vmToAsm()==============\n");
  for (int i=0; i<vmCodeTop; i++) {
    VmCode *c = &vmCodes[i];
    xAsm(c->label, c->op, c->d, c->p1, c->p2);
  }
  xClose();
}