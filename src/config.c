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
 *
 * config.c
 */

#include "config.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmdline.h"
#include "utils.h"

struct config_st {
    int opt_limit;
    char *workspace_name;
    char *def_file_name;
    bool verbose;
    bool colour;
};

static void reset(config *obj)
{
    obj->opt_limit = 1;
    obj->workspace_name = obj->def_file_name = NULL;
    obj->verbose = false;
    obj->colour = true;
}

config *config_new()
{
    config *obj = malloc(sizeof(struct config_st));
    reset(obj);
    return obj;
}

static char *parse_workspace_name(config *obj, char *arg)
{
    char *src = strchr(arg, '=');
    if (!src || !*++src)
        return "Invalid workspace option";
    obj->workspace_name = strdup(src);
    return NULL;
}

static char *parse_def_file_name(config *obj, char *arg)
{
    char *src = strchr(arg, '=');
    if (!src || !*++src)
        return "Invalid definition file option";
    obj->def_file_name = strdup(src);
    return NULL;
}

__attribute__((always_inline)) static inline void mark_opt_limit(
        config *obj, int index)
{
    obj->opt_limit = index + 1;
}

char *config_parse_cmd_line(config *obj, int argc, char *argv[])
{
    reset(obj);
    for (int i = 1; i < argc && is_opt(argv[i]); i++) {
        char *err_msg = NULL;
        if (equal_opts(argv[i], "--workspace") || equal_opts(argv[i], "-w")) {
            err_msg = parse_workspace_name(obj, argv[i]);
            mark_opt_limit(obj, i);
        } else if (equal_opts(argv[i], "--def")) {
            err_msg = parse_def_file_name(obj, argv[i]);
            mark_opt_limit(obj, i);
        } else if (!strcmp(argv[i], "--verbose") || !strcmp(argv[i], "-v")) {
            obj->verbose = true;
            mark_opt_limit(obj, i);
        } else if (!strcmp(argv[i], "--no-colour") ||
                !strcmp(argv[i], "--no-color")) {
            obj->colour = false;
            mark_opt_limit(obj, i);
        }
        if (err_msg)
            return err_msg;
    }

    /* Default the definition file name to <user_dir>/.octo/workspace */
    if (!obj->def_file_name) {
        char *homedir = get_home();
        char tmp[MAX_PATH];
        snprintf(tmp, MAX_PATH, "%s%c.octo%cworkspaces", homedir,
                path_separator(), path_separator());
        obj->def_file_name = strdup(tmp);
        free(homedir);
    }

    return NULL;
}

int config_get_opt_limit(config *obj)
{
    return obj->opt_limit;
}

char *config_get_workspace_name(config *obj)
{
    return obj->workspace_name;
}

char *config_get_def_file_name(config *obj)
{
    return obj->def_file_name;
}

bool config_is_verbose(config *obj)
{
    return obj->verbose;
}

bool config_is_colour(config *obj)
{
    return obj->colour;
}

void config_destroy(config *obj)
{
    free(obj->workspace_name);
    free(obj->def_file_name);
    free(obj);
}
