/*
 * linkedhashsettest.c
 *
 *  Created on: 21 Jun. 2018
 *      Author: vlad
 */
#include <string.h>
#include "linkedhashsettest.h"
#include "linkedhashset.h"
#include "linkedlist.h"

static void check_construction(tester *tst) {
	linked_hash_set *set = linked_hash_set_new();
	tester_assert(tst, set, "check_construction");
	linked_hash_set_destroy(set);
}

static void check_add(tester *tst) {
	linked_hash_set *set = linked_hash_set_new();
	tester_assert(tst, linked_hash_set_get_size(set) == 0, "check_add");
	linked_hash_set_add(set, "apple");
	tester_assert(tst, linked_hash_set_get_size(set) == 1, "check_add");
	linked_hash_set_add(set, "apple");
	tester_assert(tst, linked_hash_set_get_size(set) == 1, "check_add");
	linked_hash_set_destroy(set);
}

static void visit(void *state, void *value) {
	linked_list_add((HLINKEDLIST) state, value);
}

static void check_traverse(tester *tst) {
	linked_hash_set *set = linked_hash_set_new();
	HLINKEDLIST list = linked_list_create();

	linked_hash_set_add(set, "apple");
	linked_hash_set_add(set, "banana");
	linked_hash_set_add(set, "pear");
	linked_hash_set_add(set, "apple");
	tester_assert(tst, linked_hash_set_get_size(set) == 3, "check_traverse");
	linked_hash_set_traverse(set, list, visit);
	tester_assert(tst, linked_list_get_size(list) == 3, "check_traverse");
	tester_assert(tst, !strcmp("apple", linked_list_get(list, 0)),
			"check_traverse");
	tester_assert(tst, !strcmp("banana", linked_list_get(list, 1)),
			"check_traverse");
	tester_assert(tst, !strcmp("pear", linked_list_get(list, 2)),
			"check_traverse");
	linked_list_destroy(list);
	linked_hash_set_destroy(set);
}

void test_linked_hash_set(tester *tst) {
	tester_new_group(tst, "test_git");
	check_construction(tst);
	check_traverse(tst);
	check_add(tst);
}
