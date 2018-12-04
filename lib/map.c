#include "map.h"

Map* mapNew(Map *map, Pair *list, int size) {
  map->list = list;
  map->size = size;
  map->top = size;
  return map;
}

Pair mapAdd(Map *map, char *key, void *value) {
  assert(map->top < map->size);
  Pair p = { key, value };
  map->list[map->top++] = p;
  return p;
}

int mapFind(Map *map, char *key) {
  for (int i=0; i<map->top; i++) {
    if (strcmp(map->list[i].key, key)==0)
      return i;
  }
  return -1;
}

void* mapLookup(Map *map, char *key) {
  int i = mapFind(map, key);
  if (i==-1) return NULL;
  return map->list[i].value;
}

/*

Map* c6mapNew(Map *map, Pair *list, int size) {
  map->list = list;
  map->size = size;
  map->top = size;
  return map;
}

Pair c6mapAdd(Map *map, char *key, void *value) {
  assert(map->top < map->size);
  Pair p = { key, value };
  map->list[map->top++] = p;
  return p;
}

int c6mapFind(Map *map, char *key) {
  for (int i=0; i<map->top; i++) {
    if (strcmp(map->list[i].key, key)==0)
      return i;
  }
  return -1;
}

void* c6mapLookup(Map *map, char *key) {
  int i = c6mapFind(map, key);
  if (i==-1) return NULL;
  return map->list[i].value;
}
*/