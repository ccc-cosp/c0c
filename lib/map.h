#ifndef __MAP_H__
#define __MAP_H__

#include <string.h>
#include <assert.h>

typedef char* string;

#define size(array) (sizeof(array)/sizeof(array[0]))

typedef struct _Pair {
  char *key;
  void *value;
} Pair;

typedef struct _Map {
  Pair *list;
  int size;
  int top;
} Map;

extern Map* mapNew(Map *map, Pair *list, int size);
extern Pair mapAdd(Map *map, char *key, void *value);
extern int mapFind(Map *map, char *key);
extern void* mapLookup(Map *map, char *key);

#endif
