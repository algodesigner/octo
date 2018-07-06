#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include "classes.h"

NEWHANDLE(HLINKEDLIST);

/** TODO Provide comments! */

HLINKEDLIST linked_list_create();
void linked_list_add(HLINKEDLIST, void *);
void *linked_list_get(HLINKEDLIST, int);
void *linked_list_remove_first(HLINKEDLIST);
void *linked_list_remove_last(HLINKEDLIST);
int linked_list_get_size(HLINKEDLIST);
HLINKEDLIST linked_list_clone(HLINKEDLIST);
void linked_list_traverse(HLINKEDLIST, void *, void (*)(void *, void *));
void linked_list_clear(HLINKEDLIST);
void linked_list_destroy(HLINKEDLIST);

#endif /* _LINKEDLIST_H_ */
