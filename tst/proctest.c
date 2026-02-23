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
