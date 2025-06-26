#include "map.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 100

struct Map {
  int (*is_equal)(void *key1, void *key2);
  size_t (*hash_function)(void *key);
  List *buckets[HASH_SIZE];
};

size_t default_hash(void *key) {
  // Asume que las claves son cadenas de texto
  char *str = (char *)key;
  size_t hash = 0;
  while (*str)
    hash = hash * 31 + *str++;
  return hash;
}

Map *map_create(int (*is_equal)(void *, void *)) {
  Map *map = (Map *)malloc(sizeof(Map));
  map->is_equal = is_equal;
  map->hash_function = default_hash;
  for (int i = 0; i < HASH_SIZE; i++)
    map->buckets[i] = list_create();
  return map;
}

void multimap_insert(Map *map, void *key, void *value) {
  size_t index = map->hash_function(key) % HASH_SIZE;
  MapPair *pair = (MapPair *)malloc(sizeof(MapPair));
  pair->key = key;
  pair->value = value;
  list_pushBack(map->buckets[index], pair);
}

void map_insert(Map *map, void *key, void *value) {
  if (map_search(map, key) != NULL) return;
  multimap_insert(map, key, value);
}

MapPair *map_search(Map *map, void *key) {
  size_t index = map->hash_function(key) % HASH_SIZE;
  List *bucket = map->buckets[index];
  for (MapPair *pair = list_first(bucket); pair != NULL; pair = list_next(bucket)) {
    if (map->is_equal(pair->key, key))
      return pair;
  }
  return NULL;
}

MapPair *map_remove(Map *map, void *key) {
  size_t index = map->hash_function(key) % HASH_SIZE;
  List *bucket = map->buckets[index];
  for (MapPair *pair = list_first(bucket); pair != NULL; pair = list_next(bucket)) {
    if (map->is_equal(pair->key, key)) {
      list_popCurrent(bucket);
      return pair;
    }
  }
  return NULL;
}

void map_clean(Map *map) {
  for (int i = 0; i < HASH_SIZE; i++)
    list_clean(map->buckets[i]);
}

static int current_bucket = 0;

MapPair *map_first(Map *map) {
  current_bucket = 0;
  while (current_bucket < HASH_SIZE) {
    MapPair *pair = list_first(map->buckets[current_bucket]);
    if (pair) return pair;
    current_bucket++;
  }
  return NULL;
}

MapPair *map_next(Map *map) {
  MapPair *pair = list_next(map->buckets[current_bucket]);
  if (pair) return pair;
  current_bucket++;
  while (current_bucket < HASH_SIZE) {
    pair = list_first(map->buckets[current_bucket]);
    if (pair) return pair;
    current_bucket++;
  }
  return NULL;
}
