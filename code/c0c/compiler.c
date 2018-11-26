#include <assert.h>
#include "compiler.h"

char strTable[TMAX];
char lastToken[SMAX];
int labelIdx = 0; // tempIdx = 0, 
FILE *pFile;

char *E();
void STMT();
void VAR();
char *CALL(char *id);

#define emitPrint(...) do{ printf( __VA_ARGS__ ); fprintf(pFile, __VA_ARGS__); } while( 0 )

char *nextLabel(char *prefix) {
  char name[SMAX];
  sprintf(name, "%s%d", prefix, labelIdx++);
  return strTableAdd(name);
}

char *nextTemp() {
  return nextLabel("t");
}

void emitLabel(char *label) {
  emitPrint("(%s)\n", label);
}

void emit(char *op, char *d, char *p1, char *p2) {
  emitPrint("    %-10s %-10s %-10s %-10s\n", op, d, p1, p2);
}

int isNext(char *set) {
  char eset[SMAX], etoken[SMAX];
  sprintf(eset, " %s ", set);
  sprintf(etoken, " %s ", token);
  return (*token != '\0' && strstr(eset, etoken) != NULL);
}

int isNextType(int checkType) {
  return (*token != '\0' && type == checkType);
}

char *next() {
  // printf("    token=%-10s tokenIdx=%d\n", token, tokenIdx);
  char *t = strTableAdd(token);
  // strcpy(lastToken, token);
  lexScan();
  return t;
}

void error(char *filter) {
  printf("skip(%s) got %s fail!\n", filter, token);
  printf("-- line: %d pos: %d\n", line, pos);
  char errorCode[SMAX];
  strncpy(errorCode, start, SMAX-1);
  errorCode[SMAX-1] = '\0';
  printf("-- error code: %s\n", errorCode);
  assert(0);
}

char *skipType(int checkType) {
  if (type != checkType) error(typeName[checkType]);
  return next();
}

char *skip(char *set) {
  if (!isNext(set)) error(set);
  return next();
}

// F = (E) | Number | Literal | Id | CALL
char *F() {
  char *f;
  if (isNext("(")) { // '(' E ')'
    skip("("); // (
    f = E();
    skip(")"); // )
  } else if (isNextType(Literal) || isNextType(Number)) { // ex: Literal : "hello ....", Number: 347
    char *constant = next();
    f = nextTemp();
    emit("=", f, constant, "");
  } else if (isNextType(Id)) {
    char *id = next();
    if (isNext("(")) { // CALL ex: sum(n)
      f = CALL(id);
    } else { // id
      f = id;
    }
  } else {
    error("F(): next error!");
  }
  return f;
}

// E = F (op E)*
char *E() {
  char *f = F();
  while (isNext("+ - * / & | < > <= >= != ==")) {
    char *op = next();
    char *e = E();
    char *t = nextTemp();
    emit(op, t, f, e);
    f = t;
  }
  return f;
}

// while (E) STMT
void WHILE() {
  char *whileBegin = nextLabel("WBEGIN");
  char *whileEnd = nextLabel("WEND");
  emitLabel(whileBegin);
  skip("while");
  skip("(");
  char *e = E();
  emit("jnz", whileEnd, e, "");
  skip(")");
  STMT();
  emit("jmp", whileBegin, "", "");
  emitLabel(whileEnd);
}

// if (E) STMT (else STMT)?
void IF() {
  char *elseBegin = nextLabel("ELSE");
  char *ifEnd = nextLabel("ENDIF");
  skip("if");
  skip("(");
  char *e = E();
  emit("jz", elseBegin, e, "");
  skip(")");
  STMT();
  emit("jmp", ifEnd, "", "");
  if (isNext("else")) {
    skip("else");
    emitLabel(elseBegin);
    STMT();
    emitLabel(ifEnd);
  }
}

// BLOCK = { STMT* }
void BLOCK() {
  skip("{");
  while (isNext("var")) VAR();
  while (!isNext("}")) {
    STMT();
  }
  skip("}");
}

// return E;
void RETURN() {
  skip("return");
  char *e = E();
  emit("return", e, "", "");
  skip(";");
}


// CALL = id ( ELIST )
char *CALL(char *id) {
  skip("(");
  if (!isNext(")")) {
    char *e = E();
    emit("push", e, "", "");
    while (isNext(",")) {
      skip(",");
      e = E();
      emit("push", e, "", "");
    }
  }
  skip(")");
  char *t = nextTemp();
  emit("call", t, id, "");
  return t;
}

// STMT = WHILE | IF | BLOCK | RETURN | (ASSIGN | CALL);
void STMT() {
  if (isNext("while"))
    WHILE();
  else if (isNext("if"))
    IF();
  else if (isNext("{"))
    BLOCK();
  else if (isNext("return"))
    RETURN();
  else {
    char *id = skipType(Id);
    if (isNext("(")) { // CALL: id (...)
      CALL(id);
    } else if (isNext("=")) { // ASSIGN: id = E
      skip("=");
      char *e = E();
      emit("=", id, e, "");
    }
    skip(";");
  }
}

// VAR = var idList;
void VAR() {
  skip("var");
  char *id = skipType(Id); // 宣告變數
  emit("var", id, "", "");
  while (isNext(",")) {
    skip(",");
    id = skipType(Id);
    emit("var", id, "", "");
  }
  skip(";");
}

// FUNC = def id (idLIST) BLOCK
void FUNC() {
  skip("def");
  char *id = skipType(Id);
  emit("function", id, "", "");
  skip("(");
  if (!isNext(")")) {
    char *name = skipType(Id); // 宣告變數
    emit("param", name, "", "");
    while (isNext(",")) {
      skip(",");
      name = skipType(Id);
      emit("param", name, "", "");
    }
  }
  skip(")");
  BLOCK();
}

// PROG = STMT
void PROG() {
  next();
  while (*token != '\0') {
    if (isNext("def"))
      FUNC();
    else if (isNext("var"))
      VAR();
    else
      STMT();
  }
  // STMT();
}

void compile(char *code) {
  printf("============ compile =============\n");
  lexInit(code);
  strTableInit(strTable);
  PROG();
}
