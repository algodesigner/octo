/*
 * linkedhashset.h
 *
 * A primitive implementation of linked hash set.
 *
 *  Created on: 21 Jun. 2018
 *      Author: vlad
 */

#ifndef LINKEDHASHSET_H_
#define LINKEDHASHSET_H_

typedef struct linked_hash_set_st linked_hash_set;

linked_hash_set *linked_hash_set_new();
void linked_hash_set_add(linked_hash_set *, char *);
void linked_hash_set_traverse(linked_hash_set *, void *,
		void (*)(void *, void *));
int linked_hash_set_get_size(linked_hash_set *);
void linked_hash_set_destroy(linked_hash_set *);

#endif /* LINKEDHASHSET_H_ */
