/*
 * configtest.c
 *
 *  Created on: 4 Jul. 2018
 *      Author: vlad
 */

#include <string.h>
#include "configtest.h"
#include "config.h"

static void check_construction(tester *tst) {
	config *cfg = config_new();
	tester_assert(tst, config_new(), "check_construction");
	tester_assert(tst, !config_get_workspace_name(cfg), "check_construction");
	tester_assert(tst, !config_get_def_file_name(cfg), "check_construction");
	config_destroy(cfg);
}

static void check_parse_cmd_line(tester *tst) {
	char *argv[] = { "myapp", "--workspace=w3", "token1", "token2", "token3" };
	config *cfg = config_new();
	tester_assert(tst, !config_parse_cmd_line(cfg, 5, argv),
			"check_parse_cmd_line");
	char *wname = config_get_workspace_name(cfg);
	tester_assert(tst, wname && !strcmp("w3", wname), "check_parse_cmd_line");
	tester_assert(tst, config_get_opt_limit(cfg) == 2, "check_parse_cmd_line");
	config_destroy(cfg);
}

static void check_invalid_workspace_option(tester *tst) {
	char *argv[] = { "myapp", "--workspace=", "token1", "token2" };
	config *config = config_new();
	tester_assert(tst, config_parse_cmd_line(config, 4, argv),
			"check_invalid_workspace_option");
	config_destroy(config);
}

static void check_def_file_name(tester *tst) {
	char *argv[] = { "myapp", "--def=myspace", "token1", "token2", "token3" };
	config *cfg = config_new();
	tester_assert(tst, !config_parse_cmd_line(cfg, 5, argv),
			"check_def_file_name");
	char *df = config_get_def_file_name(cfg);
	tester_assert(tst, df && !strcmp("myspace", df), "check_def_file_name");
	tester_assert(tst, config_get_opt_limit(cfg) == 2, "check_def_file_name");
	config_destroy(cfg);
}

static void check_invalid_def_file_name(tester *tst) {
	char *argv[] = { "myapp", "--def", "token1", "token2" };
	config *cfg = config_new();
	tester_assert(tst, config_parse_cmd_line(cfg, 4, argv),
			"check_invalid_def_file_name");
	config_destroy(cfg);
}

void test_config(tester *tst) {
	tester_new_group(tst, "test_cmdline");
	check_construction(tst);
	check_parse_cmd_line(tst);
	check_invalid_workspace_option(tst);
	check_def_file_name(tst);
	check_invalid_def_file_name(tst);
}
