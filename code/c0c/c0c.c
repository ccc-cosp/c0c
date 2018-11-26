#include "compiler.h"

int readText(char *fileName, char *text, int size) {
  FILE *file = fopen(fileName, "r");
  int len = fread(text, 1, size, file);
  text[len] = '\0';
  fclose(file);
  return len;
}

char code[TMAX];

int main(int argc, char * argv[]) {
  char *file = argv[1], cFileName[SMAX], pFileName[SMAX], asmFileName[SMAX]; 

  sprintf(cFileName, "%s.c0", file);
  sprintf(pFileName, "%s.p0", file);
  sprintf(asmFileName, "%s.o0", file);
  FILE *pFile = fopen(pFileName, "wt");

  readText(cFileName, code, TMAX);
  puts(code);
  lex(code);
  compile(code);
  fclose(pFile);
}