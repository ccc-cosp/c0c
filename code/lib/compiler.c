#include "compiler.h"

char code[TMAX];
char *scopeName[] = {"global", "local", "param", "inner"};


// INDEX = id [ E ]
char *INDEX(char *a) {
  skip("[");
  char *e = E();
  skip("]");
  char *t = nextTemp();
  emit("[]", t, a, e);
  return t;
}

// F = (E) | Number | Literal | Id | CALL | Id [E]
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
    BLOCK(Inner);
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

// VAR = Type idList
void VAR(int scope) {
  char *type = skipType(Type); // type = var | char | int
  char *star = "";
  if (isNext("*")) star = skip("*");
  char *id = skipType(Id);
  emit(scopeName[scope], id, type, "");
  while (isNext(",")) {
    skip(",");
    star = "";
    if (isNext("*")) star = skip("*");
    id = skipType(Id);
    emit(scopeName[scope], id, type, "");
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
  char cFileName[SMAX], pFileName[SMAX], asmFileName[SMAX]; 

  sprintf(cFileName, "%s.%s", file, ext);
  sprintf(pFileName, "%s.p0", file);
  sprintf(asmFileName, "%s.o0", file);
  pFile = fopen(pFileName, "wt");

  readText(cFileName, code, TMAX);
  puts(code);
  lex(code);
  compile(code);
  fclose(pFile);
}