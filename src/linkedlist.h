#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include "classes.h"

NEWHANDLE(HLINKEDLIST);

/** TODO Provide comments! */

HLINKEDLIST LinkedListCreate(void);
void LinkedListAdd(HLINKEDLIST, void *);
void *LinkedListGet(HLINKEDLIST, int);
void *LinkedListRemoveFirst(HLINKEDLIST);
void *LinkedListRemoveLast(HLINKEDLIST);
int LinkedListGetSize(HLINKEDLIST);
HLINKEDLIST LinkedListClone(HLINKEDLIST);
void LinkedListTraverse(HLINKEDLIST, void *, void (*)(void *, void *));
void LinkedListClear(HLINKEDLIST);
void LinkedListDestroy(HLINKEDLIST);

#endif /* _LINKEDLIST_H_ */
