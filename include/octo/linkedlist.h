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

#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include "octo/classes.h"

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
