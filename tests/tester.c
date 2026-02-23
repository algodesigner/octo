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

#include "octo/tester.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *passed = "PASSED";
const char *failed = "FAILED";

struct tester_s {
    int groups;
    int tests;
    int failures;
    bool only_failures;
    bool reported_results;
};

static void print_hline();

tester *tester_create(bool only_failures)
{
    tester *obj = (tester *)malloc(sizeof(tester));
    if (obj == NULL)
        return NULL;
    memset(obj, 0, sizeof(tester));
    obj->only_failures = only_failures;
    return obj;
}

void tester_new_group(tester *obj, const char *name)
{
    obj->groups++;
    fflush(stdout);
    print_hline();
    puts(name);
    print_hline();
}

void tester_assert(tester *obj, bool exp, const char *name)
{
    const char *result;
    obj->tests++;
    if (exp) {
        result = passed;
    } else {
        result = failed;
        obj->failures++;
    }
    if (result == failed || !obj->only_failures)
        printf("Test '%s' %s\n", name, result);
}

bool tester_result(tester *obj)
{
    print_hline();
    printf("Test Results\n");
    printf("Groups:   %d\n", obj->groups);
    printf("Tests:    %d\n", obj->tests);
    printf("Failures: %d\n", obj->failures);
    obj->reported_results = true;
    return obj->failures > 0;
}

void tester_destroy(tester *obj)
{
    if (!obj->reported_results)
        tester_result(obj);
    free(obj);
}

static void print_hline()
{
    int i;
    for (i = 0; i < 80; i++)
        putchar('-');
    putchar('\n');
}
