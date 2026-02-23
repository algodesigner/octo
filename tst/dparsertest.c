/*
 * dparsertest.c
 *
 *  Created on: 14 Jun. 2018
 *      Author: Vlad
 */

#include "dparsertest.h"
#include "dconsumer.h"
#include "dparser.h"
#include "logger.h"

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
