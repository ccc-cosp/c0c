#include "compiler.h"

FILE *pFile, *sFile; 
char code[TMAX];
char Global[] = "global", Local[] = "local", Param[] = "param", Inner[] = "inner";
int labelIdx = 0, tempIdx = 0, tempMax = 0; // argIdx = 0, 
Pair symLocalList[10000], symGlobalList[10000];
Map symLocalMap, symGlobalMap;
VmCode vmCodes[10000];

char *nextLabel(char *prefix) {
  char name[SMAX];
  sprintf(name, "%s%d", prefix, labelIdx++);
  return strTableAdd(name);
}

char *nextTemp() {
  char name[SMAX];
  sprintf(name, "t%d", tempIdx++);
  char *temp = strTableAdd(name);
  // mapAdd(&symLocalMap, temp, "");
  return temp;
}

char *typeStar(char *type, char *star) {
  char tstar[SMAX];
  sprintf(tstar, "%s%s", type, star);
  return strTableAdd(tstar);
}

// F = [&*+-~!]? ((E) | Number | Literal | (++|--)? Id (++|--)? | CALL) [E]*
char *F() {
  char *f, *id = NULL, *op0 = NULL, *op1 = NULL, *op2 = NULL;
  if (isNext("& * + - ~ !")) op0 = next();
  if (isNext("(")) { // '(' E ')'
    skip("("); // (
    f = E();
    skip(")"); // )
  } else if (isNextType(Literal)) { // ex: Literal : "hello ...."
    char *str = next();
    f = nextLabel("$S");
    vmCode("str", f, str, "");
  } else if (isNextType(Number)) { // ex: Number: 347
    f = next();
  } else {
    if (isNext("++ --")) op1 = next();
    id = next();
    if (isNext("(")) { // CALL ex: sum(n)
      f = CALL(id);
    } else { // id
      if (op1 != NULL) vmCode(op1, id, "", "");
      f = id;
      if (isNext("++ --")) op2 = next();
    }
  }
  if (op0 != NULL) {
    char *t = nextTemp();
    if (strcmp(op0, "*") == 0) op0 = "*ptr";
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
  if (op2 != NULL) vmCode(op2, id, "", "");
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
  if (tempIdx > tempMax) tempMax = tempIdx;
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
  vmCode("jnz", whileEnd, e, "");
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
char *ASSIGN(char *id, char *scope, char *type) {
  char *op = "";
  if (isNext("++ --")) op = next();
  if (*type != '\0') vmCode(scope, id, type, "");
  if (isNext("=")) {
    skip("=");
    char *e = EXP();
    vmCode("=", id, e, ""); // ""=star ??
  }
  if (*op != '\0') vmCode(op, id, "", "");
  return id;
}

// DECL: *? ASSIGN
char *DECL(char *scope, char *type) {
  char *star = isNext("*") ? skip("*") : "";
  char *id = skipType(Id);
  char *ptype = typeStar(type, star);
  (strcmp(scope, Global) == 0) ? mapAdd(&symGlobalMap, id, ptype) : mapAdd(&symLocalMap, id, ptype);
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
  scanInit(code);
  mapNew(&symGlobalMap, symGlobalList, SYMMAX);
  vmInit();
  PROG();
  vmDump();
}

void compileFile(char *file, char *ext) {
  char cFileName[SMAX], pFileName[SMAX], path[SMAX]; 

  sprintf(cFileName, "%s.%s", file, ext);
  sprintf(path, "\"%s\"", cFileName);
  sprintf(pFileName, "%s.p0", file);
  pFile = fopen(pFileName, "wt");
  vmCode("file", path, "", "");
  readText(cFileName, code, TMAX);
  puts(code);
  lex(code);
  compile(code);
  vmCode("-file", path, "", "");
  vmToAsm(file);
  fclose(pFile);
}