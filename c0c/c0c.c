#include "../lib/compiler.h"

void PARAM() {
  char *type = skipType(Type);
  char *star = "";
  if (isNext("*")) star = skip("*");
  char *id = skipType(Id);
  vmCode(Param, id, typeStar(type, star), "");
}

// PROG = INCLUDE* DECL* FUNCTION*
// INCLUDE  = #...>
// FUNCTION = type id (PARAM_LIST) BLOCK
// DECL     = type id (, id)* ;
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
      VmCode *fCode = vmCode("function", id, typeStar(type, star), "");
      tempMax = 0;
      mapNew(&symLocalMap, symLocalList, SYMMAX);
      // VmCode *frameCode = vmCode("frame", "", "", "");
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
      int frameSize = symLocalMap.top + tempMax;
      char fsize[SMAX];
      sprintf(fsize, "%d", frameSize);
      fCode->p2 = strTableAdd(fsize);
      mapDump(&symLocalMap);
      printf("  tempMax=%d\n", tempMax);
      vmCode("-function", id, "", "");
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