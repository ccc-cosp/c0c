#include "../lib/compiler.h"

// PROG = FUNCTION | DECL
// FUNCTION = type id (VAR_LIST) BLOCK
// DECL     = type id (, id)* ;
void PROG() {
  next();
  while (!isEnd()) {
    char *type = skipType(Type);
    char *star = "";
    if (isNext("*")) star = skip("*");
    char *id = skipType(Id);
    if (isNext("(")) { // FUNCTION = type id (VAR_LIST) BLOCK
      skip("(");
      if (!isNext(")")) {
        VAR(Param);
        while (!isNext(")")) {
          skip(",");
          VAR(Param);
        }
      }
      skip(")");
      BLOCK(Local);
    } else { // DECL = type *? id (, *? id)* ;
      emit("global", id, type, star);
      while (isNext(",")) {
        skip(",");
        if (isNext("*")) star = skip("*");
        id = skipType(Id);
        emit("global", id, type, "");
      }
      skip(";");
    }
  }
}

int main(int argc, char * argv[]) {
  compileFile(argv[1], "c0");
}