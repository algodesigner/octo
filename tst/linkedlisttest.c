/*
 * linkedlisttest.c
 *
 *  Created on: 6 Jul. 2018
 *      Author: vlad
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "linkedlist.h"
#include "linkedlisttest.h"

inline static void print_element(void *inst, void *value) {
	printf(" %s", (char *)value);
}

static void print_list(HLINKEDLIST list) {
	if (!list)
		return;
	printf("[");
	linked_list_traverse(list, NULL, print_element);
	printf(" ]\n");
}

void test_linked_list() {
	const static char *tokens[] = { "apples", "mangoes", "pears" };
	char *value;
	register int i;

	HLINKEDLIST list = linked_list_create();

	// Test LinkedListAdd and LinkedListGetSize
	assert(linked_list_get_size(list) == 0);
	linked_list_add(list, "apples");
	assert(linked_list_get_size(list) == 1);
	linked_list_add(list, "mangoes");
	assert(linked_list_get_size(list) == 2);
	linked_list_add(list, "pears");
	assert(linked_list_get_size(list) == 3);
	print_list(list);

	// Test LinkedListGet on the original list
	for (i = 0; i < 3; i++) {
		value = linked_list_get(list, i);
		printf("LinkedListGet(list, %i) = '%s'\n", i, value);
		assert(!strcmp(value, tokens[i]));
	}

	assert(linked_list_get(list, -1) == NULL);
	assert(linked_list_get(list, -15) == NULL);
	assert(linked_list_get(list, 3) == NULL);
	assert(linked_list_get(list, 100) == NULL);

	// Test LinkedListClone
	HLINKEDLIST listCopy = linked_list_clone(list);
	assert(linked_list_get_size(listCopy) == 3);
	printf("Cloned list: ");
	print_list(listCopy);

	// LinkListGetSize(NULL) must be -1
	assert(linked_list_get_size(NULL) == -1);

	// A clone of NULL is NULL
	assert(linked_list_clone(NULL) == NULL);

	// Test LinkedListRemove on the original list
	i = linked_list_get_size(list);
	while ((value = (char *)linked_list_remove_first(list)) != NULL) {
		printf("list value: %s, remaining list size, %d\n", value,
				linked_list_get_size(list));
		assert(!strcmp(value, tokens[3 - i]));
		assert(linked_list_get_size(list) == --i);
	}

	// Finally destroy the original link list
	linked_list_destroy(list);

	// Test LinkedListRemoveLast on the cloned list
	i = linked_list_get_size(listCopy);
	while ((value = (char *)linked_list_remove_last(listCopy)) != NULL) {
		printf("list value: %s, remaining list size, %d\n", value,
				linked_list_get_size(listCopy));
		assert(linked_list_get_size(listCopy) == --i);
	}

	// Refill the cloned list and check the size (just in case)
	linked_list_add(listCopy, "apples");
	linked_list_add(listCopy, "mangoes");
	linked_list_add(listCopy, "pears");
	assert(linked_list_get_size(listCopy) == 3);

	// Test LinkedListClear on the cloned list
	linked_list_clear(listCopy);
	assert(linked_list_get_size(listCopy) == 0);

	linked_list_destroy(listCopy);
}
