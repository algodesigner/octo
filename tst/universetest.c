/*
 * universetest.c
 *
 *  Created on: 16 Jun. 2018
 *      Author: Vlad
 */
#include "universetest.h"

static void check_construction(tester *tst) {

}

void test_universe(tester *tst) {
	tester_new_group(tst, "test_universe");
	check_construction(tst);
}
