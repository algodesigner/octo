#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

/* Improvements I should consider:
 *   o Re-hashing
 *   o Better hashing algorithm
 */

struct map_entry {
	struct map_entry *next;
	char *key;
	void *value;
};

CLASS(HHASHMAP) {
	struct map_entry *buckets[HASHSIZE];
	int size;
	float load_factor;
};

static unsigned hash(char *key) {
	unsigned h;
	for (h = 0; *key; key++)
		h = *key + 31 * h;
	return h % HASHSIZE;
}

static struct map_entry *lookup(HHASHMAP map, char *key) {
	struct map_entry *me;
	for (me = map->buckets[hash(key)]; me != NULL; me = me->next) {
		if (strcmp(key, me->key) == 0)
			return me;
	}
	return NULL;
}

HHASHMAP hash_map_create() {
	return hash_map_create_ex(HASHSIZE, LOADFACTOR);
}

HHASHMAP hash_map_create_ex(int initSize, float loadFactor) {
	HHASHMAP map = malloc(sizeof(struct tagHHASHMAP));
	if (!map)
		return NULL;
	for (int i = 0; i < HASHSIZE; i++)
		map->buckets[i] = NULL;
	map->size = 0;
	return map;
}

void *hash_map_get(HHASHMAP map, char *key) {
	struct map_entry *me = lookup(map, key);
	return me != NULL ? me->value : NULL;
}

void *hash_map_put(HHASHMAP map, char *key, void *value) {
	struct map_entry *me;
	unsigned h; /* Hash value */
	if ((me = lookup(map, key)) == NULL) {
		me = malloc(sizeof(*me));
		/* TODO the cloned key is not garbage-collected! */
		if (me == NULL || (me->key = strdup(key)) == NULL)
			return NULL;
		h = hash(key);
		me->next = map->buckets[h];
		map->buckets[h] = me;
		map->size++;
	} else {
		/*        free(me->value); */
	}
	return me->value = value;
}

/* I am not sure what this method should return */
void *hash_map_remove(HHASHMAP map, char *key) {
	struct map_entry *me, *prev_me;
	unsigned h = hash(key);
	for (me = map->buckets[h], prev_me = NULL; me != NULL;
			prev_me = me, me = me->next)
	{
		if (strcmp(key, me->key) == 0) {
			if (prev_me == NULL)
				map->buckets[h] = me->next;
			else
				prev_me = me->next;
            free((void *)me->key);
			free((void *)me);
			map->size--;
			return key;
		}
	}
	return NULL;
}

char **hash_map_get_keys(HHASHMAP map) {
	char **keys = malloc(sizeof(char *) * (map->size + 1));
	char **cursor = keys;
	struct map_entry *me;
	if (!keys)
		return NULL;
	for (int i = 0; i < HASHSIZE; i++) {
		if (map->buckets[i] == NULL)
			continue;
		for (me = map->buckets[i]; me; me = me->next)
			*cursor++ = me->key;
	}
	*cursor = NULL;
	return keys;
}

int hash_map_get_size(HHASHMAP map) {
	return map->size;
}

void hash_map_traverse(HHASHMAP map, void *inst,
		void (*visit)(void *, char *, void *))
{
	struct map_entry *me;
	for (int i = 0; i < HASHSIZE; i++) {
		for (me = map->buckets[i]; me; me = me->next)
			visit(inst, me->key, me->value);
	}
}

/* Recursively removes map entries linked in a linked list */
static void free_map_entry(struct map_entry *me) {
	if (me->next != NULL)
		free_map_entry(me->next);
    free(me->key);
	free(me);
}

void hash_map_clear(HHASHMAP map) {
	struct map_entry *me;
	for (int i = 0; i < HASHSIZE; i++) {
		me = map->buckets[i];
		if (me != NULL)
			free_map_entry(me);
		map->buckets[i] = NULL;
	}
	map->size = 0;
}

void hash_map_destroy(HHASHMAP map) {
	hash_map_clear(map);
	free(map);
}
