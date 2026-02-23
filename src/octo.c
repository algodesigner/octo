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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmdline.h"
#include "config.h"
#include "proc.h"
#include "universe.h"
#include "utils.h"

#define APP_VERSION "0.1.3b"

struct app_context {
    config *config;
    logger *logger;
    proc *proc;
    universe *universe;
    char *last_name;
};

/*
 * Visits the specified file.
 */
static void visit(
        void *inst, const char *name, const char *path, const char *project)
{
    struct app_context *context = inst;
    DEBUG_LOG(context->logger, "Visiting workspace: %s, %s: %s\n", name, path,
            project);

    /* Return if the workspace name is set but not matched */
    char *wname = config_get_workspace_name(context->config);
    if (wname && strcmp(wname, name))
        return;

    /* If we have not seen this workspace before, print out its description */
    if (!proc_is_silent(context->proc) && context->last_name != name) {
        printf("Workspace %s (name: %s)\n", path, name);
        context->last_name = (char *)name;
    }
    proc_action(context->proc, path, project);
}

static void print_usage()
{
    printf("Usage: octo [--def=<filename>] [--workspace=<name>] [--verbose]\n"
           "            command [<args>]\n"
           "Commands:\n"
           "    pull\tPull the repositories\n"
           "    checkout\tCheck out out a branch\n"
           "    push\tPush the repositories\n"
           "    clone\tClone the repositories\n"
           "    status\tPrint out the repositories status\n"
           "    list\tList the repository paths\n"
           "    path\tPrint the full path to repository\n"
           "    exec\tExecute a command\n");
}

/*
 * Frees up the application resources referenced by the context.
 */
static void destroy(struct app_context *context)
{
    proc_destroy(context->proc);
    logger_destroy(context->logger);
    config_destroy(context->config);
}

/*
 * Handles the specified fatal application error.
 */
static void handle_error(void *inst, int err_code, const char *err_msg)
{
    fflush(stdout);
    fprintf(stderr, "Error: %s\n", err_msg);
    destroy((struct app_context *)inst);
    exit(EXIT_FAILURE);
}

static int resolve_path(void *inst, const char *virt_path, char *dst, int len)
{
    struct app_context *context = inst;
    const char *sep = "/";
    char *alias = strtok((char *)virt_path, sep);
    if (!alias)
        return 0;
    char *project = strtok(NULL, sep);
    if (!project || strtok(NULL, sep))
        return 0;
    const char *path = universe_get_workspace_path(context->universe, alias);
    if (!path)
        return 0;
    return snprintf(dst, len, "%s%c%s", path, path_separator(), project);
}

int main(int argc, char *argv[])
{
    struct app_context context;

    if (!proc_is_git_installed()) {
        puts("Git is not installed!");
        return EXIT_FAILURE;
    }

    if (argc == 1) {
        print_usage();
        return EXIT_SUCCESS;
    }

    if (argc == 2 && !strcmp("version", argv[1])) {
        puts("octo version " APP_VERSION);
        return EXIT_SUCCESS;
    }

    /* Initialise the application context */
    context.config = config_new();
    context.logger = logger_create(-1, stdout);
    context.proc = proc_new(context.logger, context.config);
    context.last_name = NULL;

    /* Assign the error handler function */
    proc_set_err_handler(context.proc, &context, handle_error);

    const char *err_msg = config_parse_cmd_line(context.config, argc, argv);

    /* Parse the command line parameters */
    if (!err_msg && proc_parse_cmd_line(context.proc, argc, argv)) {
        /*
         * Instantiate the workspace "universe" and parse the declaration
         * file
         */
        context.universe = universe_new(context.logger,
                config_get_def_file_name(context.config), &context,
                handle_error);
        /*
         * Perform a single or repetitive task (by visiting each and every
         * entry contained by universe)
         */
        if (proc_is_repetitive(context.proc))
            universe_accept(context.universe, &context, visit);
        else
            proc_single_action(context.proc, &context, resolve_path);

        /* Release the claimed resources */
        universe_destroy(context.universe);
        context.universe = NULL;

    } else {
        err_msg = proc_get_error_message(context.proc);
    }

    /* If there is an error, pass it to the error handler */
    if (err_msg) {
        /* The error handler is expected to release the claimed resources */
        handle_error(&context, EXIT_FAILURE, err_msg);
        return EXIT_FAILURE;
    }

    destroy(&context);
    return EXIT_SUCCESS;
}
