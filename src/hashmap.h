#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include "classes.h"

#define HASHSIZE 101
#define LOADFACTOR (0.75f)

NEWHANDLE(HHASHMAP);

/* Constructs a new instance of this hash map class */
HHASHMAP hash_map_create(void);

/* Constructs a new instance of this hash map class */
HHASHMAP hash_map_create_ex(int, float);

/* Returns a value from this map associated with a key */
void *hash_map_get(HHASHMAP, char *);

/* Puts a value into this map associated with a key */
void *hash_map_put(HHASHMAP, char *, void *);

/* Removes a value from this map associated with a key */
void *hash_map_remove(HHASHMAP, char *);

/*
 * Returns the keys stored in this map. The last char pointer is guaranteed
 * to be NULL
 */
char **hash_map_get_keys(HHASHMAP);

/* Returns the size of this hash map */
int hash_map_get_size(HHASHMAP);

/* Traverses the map entries */
void hash_map_traverse(HHASHMAP, void *, void (*)(void *, char *, void *));

/* Removes all the key value pairs (mappings) from this hash map */
void hash_map_clear(HHASHMAP);

/* Destructs this hash map */
void hash_map_destroy(HHASHMAP);

#endif /* _HASHMAP_H_ */
