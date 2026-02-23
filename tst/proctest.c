/*
 * gittest.c
 *
 *  Created on: 16 Jun. 2018
 *      Author: Vlad
 */

#include "proctest.h"

#include <stddef.h>
#include "config.h"
#include "logger.h"
#include "proc.h"

static void check_construction(tester *tst)
{
    logger *logger = logger_create(-1, stdout);
    config *config = config_new();
    proc *git = proc_new(logger, config);
    tester_assert(tst, git, "check_construction");
    proc_destroy(git);
    config_destroy(config);
    logger_destroy(logger);
}

static void check_parse_cmd_line(tester *tst)
{
    logger *logger = logger_create(-1, stdout);
    config *config = config_new();
    proc *git = proc_new(logger, config);

    char *s[] = {"octo", "pull"};
    config_parse_cmd_line(config, 2, s);
    tester_assert(tst, proc_parse_cmd_line(git, 2, s), "check_parse_cmd_line");
    tester_assert(tst, proc_get_action(git) == PULL, "check_parse_cmd_line");

    config_destroy(config);
    proc_destroy(git);
    logger_destroy(logger);
}

static void check_null_logger(tester *tst)
{
    tester_assert(tst, !proc_new(NULL, NULL), "check_null_logger");
}

static void check_is_installed(tester *tst)
{
    tester_assert(tst, proc_is_git_installed(), __func__);
}

void test_proc(tester *tst)
{
    tester_new_group(tst, "test_git");
    check_construction(tst);
    check_parse_cmd_line(tst);
    check_null_logger(tst);
    check_is_installed(tst);
}
