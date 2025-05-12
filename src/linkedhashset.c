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
	obj->map = hash_map_create();
	obj->list = linked_list_create();
	return obj;
}

void linked_hash_set_add(linked_hash_set *obj, char *s) {
	if (hash_map_get(obj->map, s))
		return;
	hash_map_put(obj->map, s, s);
	linked_list_add(obj->list, s);
}

void linked_hash_set_traverse(linked_hash_set *obj, void *state,
		void (*handle)(void *state, void *value))
{
	linked_list_traverse(obj->list, state, handle);
}

int linked_hash_set_get_size(linked_hash_set *obj) {
	return hash_map_get_size(obj->map);
}

void linked_hash_set_destroy(linked_hash_set *obj) {
	linked_list_destroy(obj->list);
	hash_map_destroy(obj->map);
	free(obj);
}
