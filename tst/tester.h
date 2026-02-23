#ifndef TESTER_H
#define TESTER_H

#include <stdbool.h>

typedef struct tester_s tester;

tester *tester_create(bool);
void tester_new_group(tester *, const char *);
void tester_assert(tester *, bool, const char *);
bool tester_result(tester *);
void tester_destroy(tester *);

#endif
