#include <stdlib.h>
#include <stddef.h>
#include "linkedlist.h"

/* Linked list entry structure */
struct entry {
	struct entry *prev_entry;
	struct entry *next_entry;
	void *value;
};

struct tagHLINKEDLIST {
	struct entry *first_entry;
	int size;
};

HLINKEDLIST linked_list_create() {
	HLINKEDLIST list = malloc(sizeof(struct tagHLINKEDLIST));
	list->first_entry = NULL;
	list->size = 0;
	return list;
}

void linked_list_add(HLINKEDLIST list, void *value) {
	if (list == NULL)
		return;
	struct entry *last_entry, *second_last_entry;
	last_entry = malloc(sizeof(struct entry));
	if (list->first_entry == NULL) {
		list->first_entry = last_entry;
		last_entry->prev_entry = last_entry;
	} else {
		/* this becomes second last element */
		second_last_entry = list->first_entry->prev_entry;
		list->first_entry->prev_entry = last_entry;
		second_last_entry->next_entry = last_entry;
		last_entry->prev_entry = second_last_entry;
	}
	last_entry->next_entry = NULL;
	last_entry->value = value;
	list->size++;
	return;
}

void *linked_list_get(HLINKEDLIST list, int index) {
	if (list == NULL)
		return NULL;
	if (index < 0 || index >= list->size)
		return NULL;
	struct entry *e;
	for (e = list->first_entry; e; e = e->next_entry) {
		if (!index--)
			return e->value;
	}
	return NULL;
}

void *linked_list_remove_first(HLINKEDLIST list) {
	void *removed_value;
	struct entry *fe = list->first_entry;
	/* If the first and only entry is NULL, there is nothing to remove */
	if (fe == NULL)
		return NULL;
	removed_value = fe->value;
	if (fe->next_entry == NULL) {
		free(fe);
		list->first_entry = NULL;
	} else {
		list->first_entry = fe->next_entry;
		free(fe);
		list->first_entry->prev_entry = list->first_entry;
	}
	list->size--;
	return removed_value;
}

void *linked_list_remove_last(HLINKEDLIST list) {
	struct entry *second_last_entry;
	struct entry *fe = list->first_entry;
	void *removed_value;
	if (fe == NULL)
		return NULL;
	if (fe->next_entry == NULL) {
		removed_value = fe->value;
		free(fe);
		list->first_entry = NULL;
	} else {
		removed_value = fe->prev_entry->value;
		/* this becomes last element */
		second_last_entry = fe->prev_entry->prev_entry;
		free(fe->prev_entry);
		fe->prev_entry = second_last_entry;
		second_last_entry->next_entry = NULL;
	}
	list->size--;
	return removed_value;
}

int linked_list_get_size(HLINKEDLIST list) {
	return list != NULL ? list->size : -1;
}

HLINKEDLIST linked_list_clone(HLINKEDLIST list) {
	if (list == NULL)
		return NULL;
	HLINKEDLIST clone = linked_list_create();
	struct entry *e;
	for (e = list->first_entry; e; e = e->next_entry)
		linked_list_add(clone, e->value);
	return clone;
}

void linked_list_traverse(HLINKEDLIST list, void *state,
		void (*handle)(void *state, void *value))
{
	struct entry *e;
	for (e = list->first_entry; e; e = e->next_entry)
		handle(state, e->value);
}

void linked_list_clear(HLINKEDLIST list) {
	struct entry *e, *f;
	for (e = list->first_entry; e;) {
		f = e;
		e = e->next_entry;
		free(f);
	}
	list->first_entry = NULL;
	list->size = 0;
}

void linked_list_destroy(HLINKEDLIST list) {
	linked_list_clear(list);
	free(list);
}
