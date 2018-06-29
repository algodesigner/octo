#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include "classes.h"

#define HASHSIZE 101
#define LOADFACTOR (0.75f)

NEWHANDLE(HHASHMAP);

/* Constructs a new instance of this hash map class */
HHASHMAP HashMapCreate(void);

/* Constructs a new instance of this hash map class */
HHASHMAP HashMapCreateEx(int, float);

/* Returns a value from this map associated with a key */
void *HashMapGet(HHASHMAP, char *);

/* Puts a value into this map associated with a key */
void *HashMapPut(HHASHMAP, char *, void *);

/* Removes a value from this map associated with a key */
void *HashMapRemove(HHASHMAP, char *);

/*
 * Returns the keys stored in this map. The last char pointer is guaranteed
 * to be NULL
 */
char **HashMapGetKeys(HHASHMAP);

/* Returns the size of this hash map */
int HashMapGetSize(HHASHMAP);

/* Traverses the map entries */
void HashMapTraverse(HHASHMAP, void *, void (*)(void *, char *, void *));

/* Removes all the key value pairs (mappings) from this hash map */
void HashMapClear(HHASHMAP);

/* Destructs this hash map */
void HashMapDestroy(HHASHMAP);

#endif /* _HASHMAP_H_ */
