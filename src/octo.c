#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "universe.h"
#include "git.h"
#include "utils.h"

struct app_context {
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
	/* If we have not seen this workspace before, print out its description */
	if (context->last_name != name) {
		printf("Workspace %s%c%s (name: %s)\n", path, path_separator(), project,
				name);
		context->last_name = (char *)name;
	}
	git_action(context->git, path, project);
}

static void print_usage() {
	printf("Usage: octo <command> [<args>] [<option> [<parameter]]\n\n"
			"Commands:\n"
			"\tpull\t\tPull the repositories\n"
			"\tcheckout\tCheck out out a branch\n"
			"\tpush\t\tPush the repositories\n"
			"\tclone\t\tClone the repositories\n"
			"\tstatus\t\tPrint out the repositories status\n");
}

int main(int argc, char *argv[]) {

	int result;
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

	/* Redirect stderr to stdout */
	dup2(1, 2);

	/* Initialise the application context */
	context.logger = logger_create(-1, stdout);
	context.git = git_new(context.logger);
	context.last_name = NULL;

	/* Parse the command line parameters */
	if (git_parse_cmd_line(context.git, argc, argv)) {

		/* Prepare the declaration file name */
		char *homedir = get_home();
		DEBUG_LOG(context.logger, "Home dir: %s\n", homedir);
		snprintf(declaration_file, MAX_PATH, "%s%c.octo%cworkspaces", homedir,
				path_separator(), path_separator());
		DEBUG_LOG(context.logger, "Declaration file: %s\n", declaration_file);

		/* Instantiate the workspace "unverse" and parse the declaration file */
		universe *uv = universe_new(context.logger, declaration_file);

		/*
		 * Visit the individual projects and perform the actions requested
		 * in the command line
		 */
		universe_accept(uv, &context, visit);

		/* Release the claimed resources */
		universe_destroy(uv);
		free(homedir);
		result = EXIT_SUCCESS;
	} else {
		printf("Error: %s\n", git_get_error_message(context.git));
		result = EXIT_FAILURE;
	}

	git_destroy(context.git);
	logger_destroy(context.logger);
	return result;
}
