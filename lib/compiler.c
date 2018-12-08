#include "compiler.h"

FILE *pFile, *sFile; 
char code[TMAX];
int labelIdx = 0, tempIdx = 0, tempMax = 0;

char *nextLabel(char *prefix) {
  return stPrint("%s%d", prefix, labelIdx++);
}

char *nextTemp() {
  char *temp = stPrint("t%d", tempIdx++);
  if (tempIdx > tempMax) {
    char *local = stPrint("%d", localTop);
    vmCode("local", temp, "", local);
    tempMax = tempIdx;
  }
  return temp;
}

char *typeStar(char *type, char *star) {
  return stPrint("%s%s", type, star);
}

// P = *? (++|--)? Id (++|--)? | CALL
char *P() {
  char *p, *star = NULL, *op1 = NULL, *op2 = NULL;
  if (isNext("*")) star = next();
  if (isNext("++ --")) op1 = next();
  char *id = skipType(Id);
  if (isNext("(")) { // CALL ex: sum(n)
    p = CALL(id);
  } else { // id
    if (op1 != NULL) vmCode(op1, id, "", "");
    p = id;
    if (isNext("++ --")) {
      op2 = next();
      vmCode(op2, id, "", "");
    }
  }
  return p;
}

// F = [&+-~!]? ((E) | Number | Literal | P) [E]*
char *F() {
  char *f, *id = NULL, *op0 = NULL, *op1 = NULL, *op2 = NULL;
  if (isNext("& + - ~ !")) op0 = next();
  if (isNext("(")) { // '(' E ')'
    skip("("); // (
    f = E();
    skip(")"); // )
  } else if (isNextType(Literal)) { // ex: Literal : "hello ...."
    char *str = next();
    f = nextLabel("$S");
    vmGlobalCode("str", f, str, "");
  } else if (isNextType(Number)) { // ex: Number: 347
    f = next();
  } else {
    f = P();
  }
  if (op0 != NULL) {
    char *t = nextTemp();
    vmCode(op0, t, f, "");
    f = t;
  }
  while (isNext("[")) {
    skip("[");
    char *e = E();
    skip("]");
    char *t = nextTemp();
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
    char *t = nextTemp();
    vmCode(op, t, f, f2);
    f = t;
  }
  return f;
}

// EXP = E
char *EXP() {
  tempIdx = 0; // 每個運算式 E 都會從 t0 開始設立臨時變數，這樣才能知道每個函數到底需要多少個臨時變數。
  char *e = E();
  return e;
}

// while (E) STMT
void WHILE() {
  char *whileBegin = nextLabel("WBEGIN");
  char *whileEnd = nextLabel("WEND");
  vmLabel(whileBegin);
  skip("while");
  skip("(");
  char *e = EXP();
  vmCode("jz", whileEnd, e, "");
  skip(")");
  STMT();
  vmCode("jmp", whileBegin, "", "");
  vmLabel(whileEnd);
}

// if (E) STMT (else STMT)?
void IF() {
  char *elseBegin = nextLabel("ELSE");
  char *ifEnd = nextLabel("ENDIF");
  skip("if");
  skip("(");
  char *e = EXP();
  vmCode("jz", elseBegin, e, "");
  skip(")");
  STMT();
  vmCode("jmp", ifEnd, "", "");
  vmLabel(elseBegin);
  if (isNext("else")) {
    skip("else");
    STMT();
  }
  vmLabel(ifEnd);
}

// BLOCK = { LIST<VAR> STMT* }
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

// CALL(id) = id ( LIST<E> )
char *CALL(char *id) {
  skip("(");
  if (!isNext(")")) {
    char *e = EXP();
    vmCode("arg", e, "", "");
    while (isNext(",")) {
      skip(",");
      e = EXP();
      vmCode("arg", e, "", "");
    }
  }
  skip(")");
  char *t = nextTemp();
  vmCode("call", t, id, "");
  return t;
}

// ASSIGN(id): id (++|--)? (= E)?
char *ASSIGN(char *id, int scope, char *type) {
  char *op = "";
  if (isNext("++ --")) op = next();
  if (*type != '\0') {
    char *local = stPrint("%d", localTop);
    vmCode("local", id, type, local);
  }
  if (isNext("=")) {
    skip("=");
    char *e = EXP();
    vmCode("=", id, e, ""); // ""=star ??
  }
  if (*op != '\0') vmCode(op, id, "", "");
  return id;
}

// DECL: *? ASSIGN
char *DECL(int scope, char *type) {
  char *star = isNext("*") ? skip("*") : "";
  char *id = skipType(Id);
  char *ptype = typeStar(type, star);
  return ASSIGN(id, scope, ptype);
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
      ASSIGN(id, Local, "");
    }
    skip(";");
  }
}

// VAR = Type LIST<DECL>
void VAR(int scope) {
  char *type = skipType(Type);
  DECL(scope, type);
  while (isNext(",")) {
    skip(",");
    DECL(scope, type);
  }
}

void compile(char *file, char *ext, char *code) {
  char pFileName[SMAX]; 
  printf("============ compile =============\n");
  sprintf(pFileName, "%s.p0", file);
  pFile = fopen(pFileName, "wt");
  stInit();
  scanInit(code);
  vmInit();
  char *path = stPrint("\"%s.%s\"", file, ext);
  vmGlobalCode("file", path, "", "");
  PROG();
  vmCode("-file", path, "", "");
  vmDump(vmGlobalCodes, vmGlobalTop);
  vmDump(vmCodes, vmCodeTop);
  vmToAsm(file);
  fclose(pFile);
}

void compileFile(char *file, char *ext) {
  char cFileName[SMAX]; 
  sprintf(cFileName, "%s.%s", file, ext);
  readText(cFileName, code, TMAX);
  puts(code);
  lex(code);
  compile(file, ext, code);
}
