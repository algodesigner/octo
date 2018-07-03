#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "universe.h"
#include "git.h"
#include "utils.h"
#include "cmdline.h"

struct app_context {
	logger *logger;
	git *git;
	char *name;
	char *last_name;
	int argc;
	char **argv;
	int result;
	const char *error_message;
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
	if (context->name && strcmp(context->name, name))
		return;

	/* If we have not seen this workspace before, print out its description */
	if (context->last_name != name) {
		printf("Workspace %s (name: %s)\n", path, name);
		context->last_name = (char *)name;
	}
	git_action(context->git, path, project);
}

static void print_usage() {
	printf("Usage: octo <command> [<args>] [<option> [<parameter]]\n\n"
			"Commands:\n"
			"    pull\tPull the repositories\n"
			"    checkout\tCheck out out a branch\n"
			"    push\tPush the repositories\n"
			"    clone\tClone the repositories\n"
			"    status\tPrint out the repositories status\n");
}

bool def_file_name(struct app_context *context, char *dst, int len) {
	int arg_index = get_opt("--def", context->argc, context->argv);
	if (arg_index != -1) {
		char *src = strchr(context->argv[arg_index], '=');
		if (!src || !*++src) {
			context->error_message = "Invalid definition file option";
			context->result = EXIT_FAILURE;
			return false;
		}
		strncpy(dst, src, len);
	} else {
		/* Get the definition file name */
		char *homedir = get_home();
		DEBUG_LOG(context->logger, "Home dir: %s\n", homedir);
		snprintf(dst, len, "%s%c.octo%cworkspaces", homedir, path_separator(),
				path_separator());
		DEBUG_LOG(context->logger, "Definition file: %s\n", dst);
		free(homedir);
	}
	return true;
}

bool get_workspace_name(struct app_context *context) {
	int arg_index = get_opt("--workspace", context->argc, context->argv);
	if (arg_index != -1) {
		char *src = strchr(context->argv[arg_index], '=');
		if (!src || !*++src) {
			context->error_message = "Invalid workspace option";
			context->result = EXIT_FAILURE;
			return false;
		}
		context->name = src;
	}
	return true;
}

int main(int argc, char *argv[]) {

	char declaration_file[MAX_PATH];
	struct app_context context;

	if (!git_is_installed()) {
		puts("Git is not installed!");
		return EXIT_FAILURE;
	}

	if (argc == 1) {
		print_usage();
		return EXIT_SUCCESS;
	}

	/* Initialise the application context */
	context.logger = logger_create(-1, stdout);
	context.git = git_new(context.logger);
	context.name = NULL;
	context.last_name = NULL;
	context.argc = argc;
	context.argv = argv;
	context.result = 0;

	/* Parse the command line parameters */
	if (git_parse_cmd_line(context.git, argc, argv)) {

		/* Get the definition file name */
		if (def_file_name(&context, declaration_file, MAX_PATH)
				&& get_workspace_name(&context))
		{
			/*
			 * Instantiate the workspace "universe" and parse the declaration
			 * file
			 */
			universe *uv = universe_new(context.logger, declaration_file);

			/*
			 * Visit the individual projects and perform the actions requested
			 * in the command line
			 */
			universe_accept(uv, &context, visit);

			/* Release the claimed resources */
			universe_destroy(uv);
			context.result = EXIT_SUCCESS;
		}
	} else {
		context.error_message = git_get_error_message(context.git);
		context.result = EXIT_FAILURE;
	}

	/* Report an error if there are any failures */
	if (context.result != EXIT_SUCCESS)
		fprintf(stderr, "Error: %s\n", context.error_message);

	git_destroy(context.git);
	logger_destroy(context.logger);
	return context.result;
}
