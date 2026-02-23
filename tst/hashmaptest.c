/*
 * hashmaptest.c
 *
 *  Created on: 7 Jul. 2018
 *      Author: vlad
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hashmap.h"

void test_hash_map() {
	HHASHMAP map = hash_map_create();

	assert(!hash_map_get_size(map));

	hash_map_put(map, "elegant", "Java");
	hash_map_put(map, "complex", "C++");
	assert(hash_map_get_size(map) == 2);

	printf("elegant->%s\n", (char *)hash_map_get(map, "elegant"));
	printf("complex->%s\n", (char *)hash_map_get(map, "complex"));
	printf("size = %d\n", hash_map_get_size(map));

	/* Test look-ups */
	assert(!strcmp(hash_map_get(map, "elegant"), "Java"));
	assert(!strcmp(hash_map_get(map, "complex"), "C++"));

	puts("Removing one element from the map...");
	hash_map_remove(map, "elegant");
	assert(hash_map_get_size(map) == 1);
	assert(hash_map_get(map, "elegant") == NULL);
	assert(!strcmp(hash_map_get(map, "complex"), "C++"));

	printf("elegant->%s\n", (char *)hash_map_get(map, "elegant"));
	printf("complex->%s\n", (char *)hash_map_get(map, "complex"));
	printf("size = %d\n", hash_map_get_size(map));

	puts("Printing keys...");
	char **keys = hash_map_get_keys(map);
	char **keysp;
	for (keysp = keys; *keysp; keysp++)
		printf("key: %s\n", *keysp);
	printf("Destroying the map...\n");
	hash_map_destroy(map);
}
