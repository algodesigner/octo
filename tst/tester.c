#include "tester.h"
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
