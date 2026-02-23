/*
 * cmdlinetest.c
 *
 *  Created on: 30 Jun. 2018
 *      Author: vlad
 */

#include "cmdlinetest.h"
#include "cmdline.h"

static void check_is_opt(tester *tst) {
	tester_assert(tst, is_opt("-o"), "check_is_opt");
	tester_assert(tst, is_opt("--version"), "check_is_opt");
	tester_assert(tst, is_opt("-"), "check_is_opt");
	tester_assert(tst, !is_opt("option"), "check_is_opt");
	tester_assert(tst, !is_opt(""), "check_is_opt");
}

static void check_get_opt(tester *tst) {
	char *argv[] = { "myapp", "token1", "token2", "--my-option", "token3" };
	tester_assert(tst, get_opt("--my-option", 5, argv) == 3, "check_get_opt");
	tester_assert(tst, get_opt("--other-option", 5, argv) == -1,
			"check_get_opt");
	tester_assert(tst, get_opt("token1", 5, argv) == -1, "check_get_opt");
}

static void check_equal_opts(tester *tst) {
	tester_assert(tst, equal_opts("--workspace=w5", "--workspace"),
			"check_equal_opts");
	tester_assert(tst, !equal_opts("--max=15", "--workspace"),
			"check_equal_opts");
	tester_assert(tst, !equal_opts("--w=w5", "--workspace"),
			"check_equal_opts");
}

void test_cmdline(tester *tst) {
	tester_new_group(tst, "test_cmdline");
	check_is_opt(tst);
	check_get_opt(tst);
	check_equal_opts(tst);
}
