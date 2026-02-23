/*
 * Copyright (c) 2018-2026, Vlad Shurupov
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include "octo/classes.h"

#define HASHSIZE 101
#define LOADFACTOR (0.75f)

NEWHANDLE(HHASHMAP);

/* Constructs a new instance of this hash map class */
HHASHMAP hash_map_create(void);

/* Constructs a new instance of this hash map class */
HHASHMAP hash_map_create_ex(int, float);

/* Returns a value from this map associated with a key */
void *hash_map_get(HHASHMAP, char *);

/* Puts a value into this map associated with a key */
void *hash_map_put(HHASHMAP, char *, void *);

/* Removes a value from this map associated with a key */
void *hash_map_remove(HHASHMAP, char *);

/*
 * Returns the keys stored in this map. The last char pointer is guaranteed
 * to be NULL
 */
char **hash_map_get_keys(HHASHMAP);

/* Returns the size of this hash map */
int hash_map_get_size(HHASHMAP);

/* Traverses the map entries */
void hash_map_traverse(HHASHMAP, void *, void (*)(void *, char *, void *));

/* Removes all the key value pairs (mappings) from this hash map */
void hash_map_clear(HHASHMAP);

/* Destructs this hash map */
void hash_map_destroy(HHASHMAP);

#endif /* _HASHMAP_H_ */
