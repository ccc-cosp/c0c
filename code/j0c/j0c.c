#include "../lib/compiler.h"

// FUNC = function id (idLIST) BLOCK
void FUNC() {
  skip("function");
  char *id = skipType(Id);
  emit("function", id, "", "");
  skip("(");
  if (!isNext(")")) {
    char *p = skipType(Id);
    emit("param", p, "", "");
    while (isNext(",")) {
      skip(",");
      p = skipType(Id);
      emit("param", p, "", "");
    }
  }
  skip(")");
  BLOCK(Local);
}

// PROG = FUNCTION | VAR; | STMT
void PROG() {
  next();
  while (!isEnd()) {
    if (isNext("function"))
      FUNC();
    else if (isNext("var")) {
      VAR(Global);
      skip(";");
    } else
      STMT();
  }
}

int main(int argc, char * argv[]) {
  compileFile(argv[1], "j0");
}