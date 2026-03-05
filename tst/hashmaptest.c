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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#include "tester.h"

static void check_construction(tester *tst)
{
    HHASHMAP map = hash_map_create();
    tester_assert(tst, map != NULL, "check_construction");
    tester_assert(
            tst, hash_map_get_size(map) == 0, "check_construction - empty map");
    hash_map_destroy(map);
}

static void check_basic_operations(tester *tst)
{
    HHASHMAP map = hash_map_create();

    /* Test put and get */
    hash_map_put(map, "elegant", "Java");
    hash_map_put(map, "complex", "C++");
    tester_assert(tst, hash_map_get_size(map) == 2,
            "check_basic_operations - size after puts");

    tester_assert(tst, strcmp(hash_map_get(map, "elegant"), "Java") == 0,
            "check_basic_operations - get elegant");
    tester_assert(tst, strcmp(hash_map_get(map, "complex"), "C++") == 0,
            "check_basic_operations - get complex");

    /* Test remove */
    void *removed = hash_map_remove(map, "elegant");
    tester_assert(tst, removed != NULL && strcmp(removed, "Java") == 0,
            "check_basic_operations - remove returns correct value");
    tester_assert(tst, hash_map_get_size(map) == 1,
            "check_basic_operations - size after remove");
    tester_assert(tst, hash_map_get(map, "elegant") == NULL,
            "check_basic_operations - get after remove returns NULL");
    tester_assert(tst, strcmp(hash_map_get(map, "complex"), "C++") == 0,
            "check_basic_operations - other key still present");

    /* Test get_keys */
    char **keys = hash_map_get_keys(map);
    tester_assert(
            tst, keys != NULL, "check_basic_operations - get_keys not NULL");
    tester_assert(tst, keys[0] != NULL && strcmp(keys[0], "complex") == 0,
            "check_basic_operations - get_keys first key");
    tester_assert(tst, keys[1] == NULL,
            "check_basic_operations - get_keys NULL terminator");
    free(keys);

    hash_map_destroy(map);
}

static void check_put_return_value(tester *tst)
{
    HHASHMAP map = hash_map_create();

    /* First insert returns NULL */
    void *old_value = hash_map_put(map, "key1", "value1");
    tester_assert(tst, old_value == NULL,
            "check_put_return_value - first insert returns NULL");

    /* Update returns old value */
    old_value = hash_map_put(map, "key1", "value2");
    tester_assert(tst, old_value != NULL && strcmp(old_value, "value1") == 0,
            "check_put_return_value - update returns old value");

    /* New key returns NULL */
    old_value = hash_map_put(map, "key2", "value3");
    tester_assert(tst, old_value == NULL,
            "check_put_return_value - new key returns NULL");

    /* Another update returns old value */
    old_value = hash_map_put(map, "key2", "value4");
    tester_assert(tst, old_value != NULL && strcmp(old_value, "value3") == 0,
            "check_put_return_value - second update returns old value");

    hash_map_destroy(map);
}

static void check_remove_return_value(tester *tst)
{
    HHASHMAP map = hash_map_create();

    hash_map_put(map, "key1", "value1");
    hash_map_put(map, "key2", "value2");

    /* Remove existing key returns value */
    void *removed_value = hash_map_remove(map, "key1");
    tester_assert(tst,
            removed_value != NULL && strcmp(removed_value, "value1") == 0,
            "check_remove_return_value - remove returns correct value");
    tester_assert(tst, hash_map_get(map, "key1") == NULL,
            "check_remove_return_value - get after remove returns NULL");

    /* Remove non-existent key returns NULL */
    removed_value = hash_map_remove(map, "nonexistent");
    tester_assert(tst, removed_value == NULL,
            "check_remove_return_value - remove non-existent returns NULL");

    hash_map_destroy(map);
}

static void check_hash_collisions(tester *tst)
{
    HHASHMAP map = hash_map_create();

    /* Add multiple keys (they might hash to same bucket) */
    hash_map_put(map, "a", "value_a");
    hash_map_put(map, "b", "value_b");
    hash_map_put(map, "c", "value_c");
    hash_map_put(map, "d", "value_d");
    hash_map_put(map, "e", "value_e");

    tester_assert(tst, hash_map_get_size(map) == 5,
            "check_hash_collisions - initial size");

    /* Remove middle element */
    void *removed = hash_map_remove(map, "c");
    tester_assert(tst, removed != NULL && strcmp(removed, "value_c") == 0,
            "check_hash_collisions - remove middle returns correct value");
    tester_assert(tst, hash_map_get_size(map) == 4,
            "check_hash_collisions - size after removing middle");
    tester_assert(tst, hash_map_get(map, "c") == NULL,
            "check_hash_collisions - get removed middle returns NULL");

    /* Verify other elements still present */
    tester_assert(tst, hash_map_get(map, "a") != NULL,
            "check_hash_collisions - a still present");
    tester_assert(tst, hash_map_get(map, "b") != NULL,
            "check_hash_collisions - b still present");
    tester_assert(tst, hash_map_get(map, "d") != NULL,
            "check_hash_collisions - d still present");
    tester_assert(tst, hash_map_get(map, "e") != NULL,
            "check_hash_collisions - e still present");

    /* Remove last element */
    removed = hash_map_remove(map, "e");
    tester_assert(tst, removed != NULL && strcmp(removed, "value_e") == 0,
            "check_hash_collisions - remove last returns correct value");
    tester_assert(tst, hash_map_get_size(map) == 3,
            "check_hash_collisions - size after removing last");

    /* Remove first element */
    removed = hash_map_remove(map, "a");
    tester_assert(tst, removed != NULL && strcmp(removed, "value_a") == 0,
            "check_hash_collisions - remove first returns correct value");
    tester_assert(tst, hash_map_get_size(map) == 2,
            "check_hash_collisions - size after removing first");

    /* Verify remaining elements */
    tester_assert(tst, hash_map_get(map, "b") != NULL,
            "check_hash_collisions - b remains");
    tester_assert(tst, hash_map_get(map, "d") != NULL,
            "check_hash_collisions - d remains");

    hash_map_destroy(map);
}

static void traverse_callback(void *context, char *key, void *value)
{
    (void)key;
    (void)value;
    (*(int *)context)++;
}

static void check_clear_and_traverse(tester *tst)
{
    HHASHMAP map = hash_map_create();

    hash_map_put(map, "key1", "value1");
    hash_map_put(map, "key2", "value2");
    hash_map_put(map, "key3", "value3");

    tester_assert(tst, hash_map_get_size(map) == 3,
            "check_clear_and_traverse - size before clear");

    /* Test traverse */
    int traverse_count = 0;
    hash_map_traverse(map, &traverse_count, traverse_callback);
    tester_assert(tst, traverse_count == 3,
            "check_clear_and_traverse - traverse visits all elements");

    /* Test clear */
    hash_map_clear(map);
    tester_assert(tst, hash_map_get_size(map) == 0,
            "check_clear_and_traverse - size after clear");

    /* Verify all keys are gone */
    tester_assert(tst, hash_map_get(map, "key1") == NULL,
            "check_clear_and_traverse - key1 gone after clear");
    tester_assert(tst, hash_map_get(map, "key2") == NULL,
            "check_clear_and_traverse - key2 gone after clear");
    tester_assert(tst, hash_map_get(map, "key3") == NULL,
            "check_clear_and_traverse - key3 gone after clear");

    hash_map_destroy(map);
}

void test_hash_map(tester *tst)
{
    tester_new_group(tst, "test_hash_map");
    check_construction(tst);
    check_basic_operations(tst);
    check_put_return_value(tst);
    check_remove_return_value(tst);
    check_hash_collisions(tst);
    check_clear_and_traverse(tst);
}