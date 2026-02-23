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

#include "configtest.h"
#include <string.h>
#include "config.h"

static void check_construction(tester *tst)
{
    config *cfg = config_new();
    tester_assert(tst, config_new(), "check_construction");
    tester_assert(tst, !config_get_workspace_name(cfg), "check_construction");
    tester_assert(tst, !config_get_def_file_name(cfg), "check_construction");
    config_destroy(cfg);
}

static void check_parse_cmd_line(tester *tst)
{
    char *argv[] = {"myapp", "--workspace=w3", "token1", "token2", "token3"};
    config *cfg = config_new();
    tester_assert(
            tst, !config_parse_cmd_line(cfg, 5, argv), "check_parse_cmd_line");
    char *wname = config_get_workspace_name(cfg);
    tester_assert(tst, wname && !strcmp("w3", wname), "check_parse_cmd_line");
    tester_assert(tst, config_get_opt_limit(cfg) == 2, "check_parse_cmd_line");
    config_destroy(cfg);
}

static void check_invalid_workspace_option(tester *tst)
{
    char *argv[] = {"myapp", "--workspace=", "token1", "token2"};
    config *config = config_new();
    tester_assert(tst, config_parse_cmd_line(config, 4, argv),
            "check_invalid_workspace_option");
    config_destroy(config);
}

static void check_def_file_name(tester *tst)
{
    char *argv[] = {"myapp", "--def=myspace", "token1", "token2", "token3"};
    config *cfg = config_new();
    tester_assert(
            tst, !config_parse_cmd_line(cfg, 5, argv), "check_def_file_name");
    char *df = config_get_def_file_name(cfg);
    tester_assert(tst, df && !strcmp("myspace", df), "check_def_file_name");
    tester_assert(tst, config_get_opt_limit(cfg) == 2, "check_def_file_name");
    config_destroy(cfg);
}

static void check_invalid_def_file_name(tester *tst)
{
    char *argv[] = {"myapp", "--def", "token1", "token2"};
    config *cfg = config_new();
    tester_assert(tst, config_parse_cmd_line(cfg, 4, argv),
            "check_invalid_def_file_name");
    config_destroy(cfg);
}

void test_config(tester *tst)
{
    tester_new_group(tst, "test_cmdline");
    check_construction(tst);
    check_parse_cmd_line(tst);
    check_invalid_workspace_option(tst);
    check_def_file_name(tst);
    check_invalid_def_file_name(tst);
}
