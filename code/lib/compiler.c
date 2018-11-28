#include "compiler.h"

FILE *pFile, *sFile; 
char code[TMAX];
char Global[] = "global", Local[] = "local", Param[] = "param", Inner[] = "inner";

// F = [&*+-~!]? ((E) | Number | Literal | Id | CALL) [E]*
char *F() {
  char *f, *op0 = NULL;
  if (isNext("& * + - ~ !")) op0 = next();
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
  } else {
    char *id = next();
    if (isNext("(")) { // CALL ex: sum(n)
      f = CALL(id);
    } else { // id
      f = id;
    }
  }
  if (op0 != NULL) {
    char *t = vmNextTemp();
    if (strcmp(op0, "*") == 0) op0 = "*ptr";
    vmCode(op0, t, f, "");
    f = t;
  }
  while (isNext("[")) {
    skip("[");
    char *e = E();
    skip("]");
    char *t = vmNextTemp();
    vmCode("[]", t, f, e);
    f = t;
  }
  return f;
}

// E = F (op F)*
char *E() {
  char *f = F();
  while (isNext("+ - * / & | && || < > <= >= != ==")) {
    char *op = next();
    char *f2 = F();
    char *t = vmNextTemp();
    vmCode(op, t, f, f2);
    f = t;
  }
  return f;
}

// EXP = E
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

// BLOCK = { LIST<VAR> STMT* }
void BLOCK(char *scope) {
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

// CALL(id) = id ( LIST<E> )
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

// ASSIGN(id): id (++|--)? (= E)?
char *ASSIGN(char *id, char *scope, char *type, char *star) {
  char *op = "";
  if (isNext("++ --")) op = next();
  if (*type != '\0') vmCode(scope, id, type, star);
  if (isNext("=")) {
    skip("=");
    char *e = EXP();
    vmCode("=", id, e, star);
  }
  if (*op != '\0') vmCode(op, id, "", "");
  return id;
}

// DECL: *? ASSIGN
char *DECL(char *scope, char *type) {
  char *star = isNext("*") ? skip("*") : "";
  char *id = skipType(Id);
  return ASSIGN(id, scope, type, star);
}

// STMT = WHILE | IF | BLOCK | RETURN | VAR ; | (ASSIGN | CALL);
// ASSIGN: id = E
// CALL  : id (...)
void STMT() {
  if (isNext("while"))
    WHILE();
  else if (isNext("if"))
    IF();
  else if (isNext("{"))
    BLOCK(Inner);
  else if (isNext("return"))
    RETURN();
  else if (isNextType(Type)) { // VAR ;
    VAR(Local);
    skip(";");
  } else {
    char *id = skipType(Id);
    if (isNext("(")) {
      CALL(id);
    } else {
      ASSIGN(id, Local, "", "");
    }
    skip(";");
  }
}

// VAR = Type LIST<DECL>
void VAR(char *scope) {
  char *type = skipType(Type);
  DECL(scope, type);
  while (isNext(",")) {
    skip(",");
    DECL(scope, type);
  }
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
  sprintf(pFileName, "%s.p0", file);
  sprintf(sFileName, "%s.s0", file);
  pFile = fopen(pFileName, "wt");
  sFile = fopen(sFileName, "wt");
  vmCode("file", cFileName, "", "");
  readText(cFileName, code, TMAX);
  puts(code);
  lex(code);
  compile(code);
  fclose(pFile);
  fclose(sFile);
}