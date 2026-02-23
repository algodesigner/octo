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
#include "linkedhashsettest.h"
#include <string.h>
#include "linkedhashset.h"
#include "linkedlist.h"

static void check_construction(tester *tst)
{
    linked_hash_set *set = linked_hash_set_new();
    tester_assert(tst, set, "check_construction");
    linked_hash_set_destroy(set);
}

static void check_add(tester *tst)
{
    linked_hash_set *set = linked_hash_set_new();
    tester_assert(tst, linked_hash_set_get_size(set) == 0, "check_add");
    linked_hash_set_add(set, "apple");
    tester_assert(tst, linked_hash_set_get_size(set) == 1, "check_add");
    linked_hash_set_add(set, "apple");
    tester_assert(tst, linked_hash_set_get_size(set) == 1, "check_add");
    linked_hash_set_destroy(set);
}

static void visit(void *state, void *value)
{
    linked_list_add((HLINKEDLIST)state, value);
}

static void check_traverse(tester *tst)
{
    linked_hash_set *set = linked_hash_set_new();
    HLINKEDLIST list = linked_list_create();

    linked_hash_set_add(set, "apple");
    linked_hash_set_add(set, "banana");
    linked_hash_set_add(set, "pear");
    linked_hash_set_add(set, "apple");
    tester_assert(tst, linked_hash_set_get_size(set) == 3, "check_traverse");
    linked_hash_set_traverse(set, list, visit);
    tester_assert(tst, linked_list_get_size(list) == 3, "check_traverse");
    tester_assert(
            tst, !strcmp("apple", linked_list_get(list, 0)), "check_traverse");
    tester_assert(
            tst, !strcmp("banana", linked_list_get(list, 1)), "check_traverse");
    tester_assert(
            tst, !strcmp("pear", linked_list_get(list, 2)), "check_traverse");
    linked_list_destroy(list);
    linked_hash_set_destroy(set);
}

void test_linked_hash_set(tester *tst)
{
    tester_new_group(tst, "test_git");
    check_construction(tst);
    check_traverse(tst);
    check_add(tst);
}
