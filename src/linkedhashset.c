/*
 * linkedhashset.c
 *
 *  Created on: 21 Jun. 2018
 *      Author: vlad
 */
#include <stdlib.h>
#include "linkedhashset.h"
#include "hashmap.h"
#include "linkedlist.h"

struct linked_hash_set_st {
	HHASHMAP map;
	HLINKEDLIST list;
};

linked_hash_set *linked_hash_set_new() {
	linked_hash_set *obj = malloc(sizeof(struct linked_hash_set_st));
	obj->map = HashMapCreate();
	obj->list = linked_list_create();
	return obj;
}

void linked_hash_set_add(linked_hash_set *obj, char *s) {
	if (HashMapGet(obj->map, s))
		return;
	HashMapPut(obj->map, s, s);
	linked_list_add(obj->list, s);
}

void linked_hash_set_traverse(linked_hash_set *obj, void *state,
		void (*handle)(void *state, void *value))
{
	linked_list_traverse(obj->list, state, handle);
}

int linked_hash_set_get_size(linked_hash_set *obj) {
	return HashMapGetSize(obj->map);
}

void linked_hash_set_destroy(linked_hash_set *obj) {
	free(obj->map);
	free(obj->list);
	free(obj);
}
