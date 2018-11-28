#include "../lib/compiler.h"

void PARAM() {
  char *type = skipType(Type);
  char *star = "";
  if (isNext("*")) star = skip("*");
  char *id = skipType(Id);
  vmCode(Param, id, type, star);
}

// PROG = INCLUDE | FUNCTION | DECL
// FUNCTION = type id (PARAM_LIST) BLOCK
// DECL     = type id (, id)* ;
// INCLUDE  = #...>
void PROG() {
  next();
  while (!isEnd()) {
    if (isNext("#")) { // INCLUDE
      while (!isNext(">")) next();
      skip(">");
      continue;
    }
    char *type = skipType(Type);
    char *star = "";
    if (isNext("*")) star = skip("*");
    char *id = skipType(Id);
    if (isNext("(")) { // FUNCTION = type id (PARAM_LIST) BLOCK
      vmCode("function", id, type, star);
      skip("(");
      if (!isNext(")")) {
        PARAM();
        while (!isNext(")")) {
          skip(",");
          PARAM();
        }
      }
      skip(")");
      BLOCK(Local);
      vmCode("fend", id, "", "");
    } else { // DECL = type *? id (, *? id)* ;
      vmCode(Global, id, type, star);
      while (isNext(",")) {
        skip(",");
        star = "";
        if (isNext("*")) star = skip("*");
        id = skipType(Id);
        vmCode(Global, id, type, star);
      }
      skip(";");
    }
  }
}

int main(int argc, char * argv[]) {
  compileFile(argv[1], "c");
}