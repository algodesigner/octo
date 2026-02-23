/*
 * octo-test.c
 *
 *  Created on: 8 Jun. 2018
 *      Author: vlad
 */

#include <stdio.h>
#include <stdlib.h>
#include "tester.h"
#include "workspacetest.h"
#include "dparsertest.h"
#include "universetest.h"
#include "linkedlisttest.h"
#include "hashmaptest.h"
#include "linkedhashsettest.h"
#include "cmdlinetest.h"
#include "configtest.h"
#include "proctest.h"

int main(int argn, char *args[]) {

	tester *tst = tester_create(true);
	test_workspace(tst);
	test_dparser(tst);
	test_proc(tst);
	test_universe(tst);
	test_linked_list();
	test_hash_map();
	test_linked_hash_set(tst);
	test_cmdline(tst);
	test_config(tst);
	tester_destroy(tst);
}
