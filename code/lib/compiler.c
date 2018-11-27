#include "compiler.h"

FILE *pFile, *sFile; 
char code[TMAX];
char *scopeName[] = {"global", "local", "param", "inner"};

// INDEX = id [ E ]
char *INDEX(char *a) {
  skip("[");
  char *e = EXP();
  skip("]");
  char *t = vmNextTemp();
  vmCode("[]", t, a, e);
  return t;
}

// F = (E) | Number | Literal | Id | CALL | Id [E]
char *F() {
  char *f;
  if (isNext("(")) { // '(' E ')'
    skip("("); // (
    f = E();
    skip(")"); // )
  } else if (isNextType(Literal)) { // ex: Literal : "hello ...."
    char *str = next();
    f = vmNextLabel("S");
    vmCode("str", f, str, "");
  } else if (isNextType(Number)) { // ex: Number: 347
    f = next();
  } else if (isNextType(Id)) {
    char *id = next();
    if (isNext("(")) { // CALL ex: sum(n)
      f = CALL(id);
    } else if (isNext("[")) {
      f = INDEX(id);
    } else { // id
      f = id;
    }
  } else {
    error("F(): next error!");
  }
  return f;
}

// E = F (op F)*
char *E() {
  char *f = F();
  while (isNext("+ - * / & | < > <= >= != ==")) {
    char *op = next();
    char *f2 = F();
    char *t = vmNextTemp();
    vmCode(op, t, f, f2);
    f = t;
  }
  return f;
}

char *EXP() {
  tempIdx = 0; // 每個運算式 E 都會從 t0 開始設立臨時變數，這樣才能知道每個函數到底需要多少個臨時變數。
  E();
}

// while (E) STMT
void WHILE() {
  char *whileBegin = vmNextLabel("WBEGIN");
  char *whileEnd = vmNextLabel("WEND");
  vmLabel(whileBegin);
  skip("while");
  skip("(");
  char *e = EXP();
  vmCode("jnz", whileEnd, e, "");
  skip(")");
  STMT();
  vmCode("jmp", whileBegin, "", "");
  vmLabel(whileEnd);
}

// if (E) STMT (else STMT)?
void IF() {
  char *elseBegin = vmNextLabel("ELSE");
  char *ifEnd = vmNextLabel("ENDIF");
  skip("if");
  skip("(");
  char *e = EXP();
  vmCode("jz", elseBegin, e, "");
  skip(")");
  STMT();
  vmCode("jmp", ifEnd, "", "");
  if (isNext("else")) {
    skip("else");
    vmLabel(elseBegin);
    STMT();
    vmLabel(ifEnd);
  }
}

// BLOCK = { VARLIST STMT* }
void BLOCK(int scope) {
  skip("{");
  while (isNextType(Type)) {
    VAR(scope);
    skip(";");
  }
  while (!isNext("}")) {
    STMT();
  }
  skip("}");
}

// return E;
void RETURN() {
  skip("return");
  char *e = EXP();
  vmCode("return", e, "", "");
  skip(";");
}


// CALL = id ( ELIST )
char *CALL(char *id) {
  skip("(");
  if (!isNext(")")) {
    char *e = EXP();
    vmCode("push", e, "", "");
    while (isNext(",")) {
      skip(",");
      e = EXP();
      vmCode("push", e, "", "");
    }
  }
  skip(")");
  char *t = vmNextTemp();
  vmCode("call", t, id, "");
  return t;
}


// STMT = WHILE | IF | BLOCK | RETURN | (ASSIGN | CALL);
void STMT() {
  if (isNext("while"))
    WHILE();
  else if (isNext("if"))
    IF();
  else if (isNext("{"))
    BLOCK(Inner);
  else if (isNext("return"))
    RETURN();
  else {
    char *id = skipType(Id);
    if (isNext("(")) { // CALL: id (...)
      CALL(id);
    } else if (isNext("=")) { // ASSIGN: id = E
      skip("=");
      char *e = EXP();
      vmCode("=", id, e, "");
    }
    skip(";");
  }
}

// VAR = Type idList
void VAR(int scope) {
  char *type = skipType(Type); // type = var | char | int
  char *star = "";
  if (isNext("*")) star = skip("*");
  char *id = skipType(Id);
  vmCode(scopeName[scope], id, type, "");
  while (isNext(",")) {
    skip(",");
    star = "";
    if (isNext("*")) star = skip("*");
    id = skipType(Id);
    vmCode(scopeName[scope], id, type, "");
  }
  // skip(";");
}

void compile(char *code) {
  printf("============ compile =============\n");
  lexInit(code);
  strTableInit(strTable);
  PROG();
}

void compileFile(char *file, char *ext) {
  char cFileName[SMAX], pFileName[SMAX], sFileName[SMAX]; 

  sprintf(cFileName, "%s.%s", file, ext);
  sprintf(pFileName, "%s.p", file);
  sprintf(sFileName, "%s.s", file);
  pFile = fopen(pFileName, "wt");
  sFile = fopen(sFileName, "wt");

  readText(cFileName, code, TMAX);
  puts(code);
  lex(code);
  compile(code);
  fclose(pFile);
  fclose(sFile);
}