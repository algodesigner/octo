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
#include "linkedlisttest.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

inline static void print_element(void *inst, void *value)
{
    (void)inst;
    printf(" %s", (char *)value);
}

static void print_list(HLINKEDLIST list)
{
    if (!list)
        return;
    printf("[");
    linked_list_traverse(list, NULL, print_element);
    printf(" ]\n");
}

void test_linked_list()
{
    const static char *tokens[] = {"apples", "mangoes", "pears"};
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
