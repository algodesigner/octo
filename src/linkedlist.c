#include <stdlib.h>
#include <stddef.h>
#include "linkedlist.h"

/* Linked list entry structure */
struct entry {
	struct entry *prevEntry;
	struct entry *nextEntry;
	void *value;
};

struct tagHLINKEDLIST {
	struct entry *firstEntry;
	int size;
};

HLINKEDLIST LinkedListCreate() {
	HLINKEDLIST list = (HLINKEDLIST)malloc(sizeof(struct tagHLINKEDLIST));
	list->firstEntry = NULL;
	list->size = 0;
	return list;
}

void LinkedListAdd(HLINKEDLIST list, void *value) {
	if (list == NULL)
		return;
	struct entry *lastEntry, *secondLastEntry;
	lastEntry = (struct entry *)malloc(sizeof(struct entry));
	if (list->firstEntry == NULL) {
		list->firstEntry = lastEntry;
		lastEntry->prevEntry = lastEntry;
	} else {
		secondLastEntry = list->firstEntry->prevEntry; /* this becomes second last element */
		list->firstEntry->prevEntry = lastEntry;
		secondLastEntry->nextEntry = lastEntry;
		lastEntry->prevEntry = secondLastEntry;
	}
	lastEntry->nextEntry = NULL;
	lastEntry->value = value;
	list->size++;
	return;
}

void *LinkedListGet(HLINKEDLIST list, int index) {
	if (list == NULL)
		return NULL;
	if (index < 0 || index >= list->size)
		return NULL;
	struct entry *e;
	for (e = list->firstEntry; e; e = e->nextEntry) {
		if (!index--)
			return e->value;
	}
	return NULL;
}

void *LinkedListRemoveFirst(HLINKEDLIST list) {
	void *removedValue;
	struct entry *fe = list->firstEntry;
	/* If the first and only entry is NULL, there is nothing to remove */
	if (fe == NULL)
		return NULL;
	removedValue = fe->value;
	if (fe->nextEntry == NULL) {
		free(fe);
		list->firstEntry = NULL;
	} else {
		list->firstEntry = fe->nextEntry;
		free(fe);
		list->firstEntry->prevEntry = list->firstEntry;
	}
	list->size--;
	return removedValue;
}

void *LinkedListRemoveLast(HLINKEDLIST list) {
	struct entry *secondLastEntry;
	struct entry *fe = list->firstEntry;
	void *removedValue;
	if (fe == NULL)
		return NULL;
	if (fe->nextEntry == NULL) {
		removedValue = fe->value;
		free(fe);
		list->firstEntry = NULL;
	} else {
		removedValue = fe->prevEntry->value;
		secondLastEntry = fe->prevEntry->prevEntry; /* this becomes last element */
		free(fe->prevEntry);
		fe->prevEntry = secondLastEntry;
		secondLastEntry->nextEntry = NULL;
	}
	list->size--;
	return removedValue;
}

int LinkedListGetSize(HLINKEDLIST list) {
	return list != NULL ? list->size : -1;
}

HLINKEDLIST LinkedListClone(HLINKEDLIST list) {
	if (list == NULL)
		return NULL;
	HLINKEDLIST clone = LinkedListCreate();
	struct entry *e;
	for (e = list->firstEntry; e; e = e->nextEntry)
		LinkedListAdd(clone, e->value);
	return clone;
}

void LinkedListTraverse(HLINKEDLIST list, void *state,
		void (*handle)(void *state, void *value))
{
	struct entry *e;
	for (e = list->firstEntry; e; e = e->nextEntry)
		handle(state, e->value);
}

void LinkedListClear(HLINKEDLIST list) {
	struct entry *e, *f;
	for (e = list->firstEntry; e;) {
		f = e;
		e = e->nextEntry;
		free(f);
	}
	list->firstEntry = NULL;
	list->size = 0;
}

void LinkedListDestroy(HLINKEDLIST list) {
	LinkedListClear(list);
	free(list);
}
