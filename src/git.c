/*
 * git.c
 * Git controller class.
 *
 *  Created on: 5 Jun. 2018
 *      Author: Vlad
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "git.h"
#include "utils.h"
#include "xsystem.h"
#include "cmdline.h"
#include "logger.h"

#define MAX_PATH 1024
#define CHAR_BUFFER_LEN 8192
#define TMP_BUFFER_SIZE 1024

#define CMD_CURR_BRANCH "git rev-parse --abbrev-ref HEAD"
#define CMD_STATUS "git status --porcelain"

static const char *INVALID_ARGUMENTS = "Invalid argument(s) in command line";
static const char *UNKNOWN_BRANCH = "Branch not specified in checkout command";
static const char *UNKNOWN_REPOSITORY =
		"Repository not specified in the clone command";
static const char *UNKNOWN_COMMAND = "Unknown command";

struct git_st {
	logger *logger;
	config *config;
	enum action action;
	char *branch;
	char *repository;
	bool dry_run;
	const char *error_message;
	void *err_handler_inst;
	void (*handle_err)(void *, int, const char *);
	struct char_buffer *char_buffer;
	char *tmp_buffer;
};

#ifdef DEBUG
static const char *action_to_string(enum action action) {
	switch (action) {
	case PULL:
		return "PULL";
	case PUSH:
		return "PUSH";
	case CHECKOUT:
		return "CHECKOUT";
	case CLONE:
		return "CLONE";
	case STATUS:
		return "STATUS";
	default:
		return "UNKNOWN";
	}
}
#endif

bool git_is_installed() {
	bool result;
	struct char_buffer *char_buffer = char_buffer_new(128);
	result = !xsystem("git version 2>&1", char_buffer, false);
	char_buffer_destroy(char_buffer);
	return result;
}

/*
 * Resets the soft state of this object.
 */
static inline void reset(git *obj) {
	obj->action = UNKNOWN;
	obj->branch = NULL;
	obj->repository = NULL;
	obj->dry_run = false;
	obj->error_message = NULL;
}

static void throw_err(git *obj, int err_code, char *fmt, ...) {
	if (!obj->handle_err)
		return;
	va_list args;
	va_start(args, fmt);
	vsnprintf(obj->tmp_buffer, TMP_BUFFER_SIZE, fmt, args);
	va_end(args);
	obj->handle_err(obj->err_handler_inst, err_code, obj->tmp_buffer);
}

git *git_new(logger *logger, config *config) {
	if (!logger || !config)
		return NULL;
	git *obj = malloc(sizeof(struct git_st));
	obj->logger = logger;
	obj->config = config;
	obj->char_buffer = char_buffer_new(CHAR_BUFFER_LEN);
	obj->tmp_buffer = malloc(TMP_BUFFER_SIZE);
	reset(obj);
	return obj;
}

void git_set_error_handler(git *obj, void *err_handler_inst,
		void (*handle_err)(void *, int, const char *))
{
	obj->err_handler_inst = err_handler_inst;
	obj->handle_err = handle_err;
}

bool git_parse_cmd_line(git *obj, int argc, char *argv[]) {

	reset(obj);
	if (argc <= 1) {
		obj->error_message = INVALID_ARGUMENTS;
		return false;
	}

	if (!strcmp(argv[1], "pull")) {
		obj->action = PULL;
	} else if (!strcmp(argv[1], "checkout")) {
		if (argc < 3 || is_opt(argv[2])) {
			obj->error_message = UNKNOWN_BRANCH;
			return false;
		}
		obj->action = CHECKOUT;
		obj->branch = argv[2];
	} else if (!strcmp(argv[1], "push")) {
		obj->action = PUSH;
	} else if (!strcmp(argv[1], "clone")) {
		if (argc < 3 || is_opt(argv[2])) {
			obj->error_message = UNKNOWN_REPOSITORY;
			return false;
		}
		obj->action = CLONE;
		obj->repository = argv[2];
	} else if (!strcmp(argv[1], "status")) {
		obj->action = STATUS;
	} else {
		obj->error_message = UNKNOWN_COMMAND;
		return false;
	}
	obj->error_message = NULL;
	return true;
}

static int exec(git *obj, const char *path, const char *project,
		const char *command, void (*run_before)(git *),
		void (*run_after)(git *))
{
	int result = -1;
	char cwd[MAX_PATH];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		const char *dir_path;
		if (project) {
			char full_path[MAX_PATH];
			snprintf(full_path, MAX_PATH, "%s%c%s", path, path_separator(),
					project);
			dir_path = full_path;
		} else
			dir_path = path;

		if (!chdir(dir_path)) {
			/* Run the "pre" task if provided */
			if (run_before)
				run_before(obj);

			/* Execute the command unless we are in the dry run or verbose
			 * mode
			 */
			DEBUG_LOG(obj->logger, "exec: %s\n", command);
			bool verbose = config_is_verbose(obj->config);
			char_buffer_reset(obj->char_buffer);
			if (!obj->dry_run || verbose)
				result = xsystem(command, obj->char_buffer, verbose);
			else
				result = 0;
			DEBUG_LOG(obj->logger, "exec: result=%d\n", result);

			/* Run the "post" task if provided */
			if (run_after)
				run_after(obj);
			chdir(cwd);
		}
	}
	return result;
}

/*
 * Prints out the currently checked out git branch.
 */
static void print_branch_name(git *obj) {
	struct char_buffer *buff = obj->char_buffer;
	char_buffer_reset(buff);
	putchar('{');
	if (!xsystem(CMD_CURR_BRANCH " 2>&1", buff, false)) {
		/* Trim the LF */
		buff->limit--;
		char_buffer_print(buff);
	} else {
		printf("???");
	}
	putchar('}');
}

/*
 * Prints the currently checked out branch name and report changes to
 * the branch if any.
 */
static void print_branch_name_chg(git *obj) {
	print_branch_name(obj);
	struct char_buffer *buff = obj->char_buffer;
	char_buffer_reset(buff);
	int result = xsystem(CMD_STATUS " 2>&1", buff, false);
	if (!result && buff->limit - buff->position > 0)
		printf(" Found changes!");
	if (config_is_verbose(obj->config))
		putchar('\n');
}

static void print_action(git *obj, const char *action, const char *project) {
	printf(" o %s %s ", action, project);
}

static void pull(git *obj, const char *path, const char *project) {
	print_action(obj, "Pulling", project);
	exec(obj, path, project, "git pull -p 2>&1", print_branch_name_chg, NULL);
	putchar('\n');
}

static void checkout(git *obj, const char *path, const char *project,
		const char *branch)
{
	print_action(obj, "Checking out", project);
	char cmd[MAX_PATH];
	snprintf(cmd, MAX_PATH, "git checkout %s 2>&1", branch);
	exec(obj, path, project, cmd, NULL, print_branch_name_chg);
	putchar('\n');
}

static void push(git *obj, const char *path, const char *project) {
	print_action(obj, "Pushing", project);
	exec(obj, path, project, "git push 2>&1", print_branch_name_chg, NULL);
	putchar('\n');
}

static void clone(git *obj, const char *path, const char *project) {
	print_action(obj, "Cloning", project);
	if (config_is_verbose(obj->config))
		putchar('\n');

	char cmd[MAX_PATH];
	snprintf(cmd, MAX_PATH, "git clone %s%s 2>&1", obj->repository, project);
	int result = exec(obj, path, NULL, cmd, NULL, NULL);
	if (result)
		throw_err(obj, result, "Failed to clone '%s'", project);
}

static void status(git *obj, const char *path, const char *project) {
	print_action(obj, "Found", project);
	bool prev_dry_run = obj->dry_run;
	obj->dry_run = true;
	int result = exec(obj, path, project, "git status 2>&1",
			print_branch_name_chg, NULL);
	putchar('\n');
	obj->dry_run = prev_dry_run;
	if (result)
		throw_err(obj, result, "Failed to retrieve status of '%s'", project);
}

void git_action(git *obj, const char *path, const char *project) {

	DEBUG_LOG(obj->logger, "git_action: action='%s', path='%s', project='%s'\n",
			action_to_string(obj->action), path, project);

	switch (obj->action) {
	case PULL:
		pull(obj, path, project);
		break;
	case CHECKOUT:
		checkout(obj, path, project, obj->branch);
		break;
	case PUSH:
		push(obj, path, project);
		break;
	case CLONE:
		clone(obj, path, project);
		break;
	case STATUS:
		status(obj, path, project);
		break;
	default:
		break;
	}
}

enum action git_get_action(git *obj) {
	return obj->action;
}

const char *git_get_error_message(git *obj) {
	return obj->error_message;
}

void git_destroy(git *obj) {
	char_buffer_destroy(obj->char_buffer);
	free(obj->tmp_buffer);
	free(obj);
}
