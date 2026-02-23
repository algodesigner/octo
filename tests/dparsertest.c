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

#include "octo/dparsertest.h"
#include "octo/dconsumer.h"
#include "octo/dparser.h"
#include "octo/logger.h"

/* A basic declaration stream */
const char *D1 =
        "projects {\n"
        "  strawberry\n"
        "  reader\n"
        "  game\n"
        "}\n"
        "\n"
        "workpace w1 -> /path/to/workspace {}\n";

struct context {
};

static void add_project(void *inst, const char *project)
{
}

static void add_workspace(void *inst, const char *alias, const char *path)
{
}

static void add_workspace_project(
        void *inst, const char *alias, const char *project)
{
}

static void dcosumer_init(void *inst, struct dconsumer *dconsumer)
{
    dconsumer->instance = inst;
    dconsumer->add_project = add_project;
    dconsumer->add_workspace = add_workspace;
    dconsumer->add_workspace_project = add_workspace_project;
}

static void check_parse_basic_def(tester *tst)
{
    logger *logger = logger_create(-1, stdout);
    struct dconsumer dconsumer;
    struct context context;
    dcosumer_init(&context, &dconsumer);

    dparser *dparser = dpaser_new(logger, &dconsumer);

    char *s;
    for (s = (char *)D1; *s; s++)
        dparser_proc_char(dparser, *s);

    dparser_destroy(dparser);
    logger_destroy(logger);
}

static void check_construction(tester *tst)
{
    logger *logger = logger_create(-1, stdout);
    struct dconsumer dconsumer;
    struct context context;
    dcosumer_init(&context, &dconsumer);

    dparser *dparser = dpaser_new(logger, &dconsumer);
    tester_assert(tst, dparser, "check_construction");
    dparser_destroy(dparser);
    logger_destroy(logger);
}

void test_dparser(tester *tst)
{
    tester_new_group(tst, "test_dparser");
    check_construction(tst);
    check_parse_basic_def(tst);
}
