#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "universe.h"
#include "git.h"
#include "utils.h"
#include "cmdline.h"
#include "config.h"

#define APP_VERSION "0.1.2a"

struct app_context {
	config *config;
	logger *logger;
	git *git;
	char *last_name;
};

/*
 * Visits the specified file.
 */
static void visit(void *inst, const char *name, const char *path,
		const char *project)
{
	struct app_context *context = inst;
	DEBUG_LOG(context->logger, "Visiting workspace: %s, %s: %s\n", name, path,
			project);

	/* Return if the workspace name is set but not matched */
	char *wname = config_get_workspace_name(context->config);
	if (wname && strcmp(wname, name))
		return;

	/* If we have not seen this workspace before, print out its description */
	if (context->last_name != name) {
		printf("Workspace %s (name: %s)\n", path, name);
		context->last_name = (char *)name;
	}
	git_action(context->git, path, project);
}

static void print_usage() {
	printf("Usage: octo [--def=<filename>] [--workspace=<name>] [--verbose]\n"
			"            command [<args>]\n"
			"Commands:\n"
			"    pull\tPull the repositories\n"
			"    checkout\tCheck out out a branch\n"
			"    push\tPush the repositories\n"
			"    clone\tClone the repositories\n"
			"    status\tPrint out the repositories status\n");
}

/*
 * Frees up the application resources referenced by the context.
 */
static void destroy(struct app_context *context) {
	git_destroy(context->git);
	logger_destroy(context->logger);
	config_destroy(context->config);
}

/*
 * Handles the specified fatal application error.
 */
static void handle_error(void *inst, int err_code, const char *err_msg) {
	fflush(stdout);
	fprintf(stderr, "Error: %s\n", err_msg);
	destroy((struct app_context *)inst);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {

	struct app_context context;

	if (!git_is_installed()) {
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
	context.git = git_new(context.logger, context.config);
	context.last_name = NULL;

	/* Assign the error handler function */
	git_set_error_handler(context.git, &context, handle_error);

	const char *err_msg = config_parse_cmd_line(context.config, argc, argv);

	/* Parse the command line parameters */
	if (!err_msg && git_parse_cmd_line(context.git, argc, argv)) {
		/*
		 * Instantiate the workspace "universe" and parse the declaration
		 * file
		 */
		universe *uv = universe_new(context.logger,
				config_get_def_file_name(context.config), &context,
				handle_error);
		/*
		 * Visit the individual projects and perform the actions requested
		 * in the command line
		 */
		universe_accept(uv, &context, visit);

		/* Release the claimed resources */
		universe_destroy(uv);

	} else {
		err_msg = git_get_error_message(context.git);
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
