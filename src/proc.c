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
#include "utils.h"
#include "xsystem.h"
#include "cmdline.h"
#include "errpublisher.h"
#include "logger.h"
#include "decorations.h"
#include "proc.h"

#define MAX_PATH 1024
#define CHAR_BUFFER_LEN 8192

#define CMD_CURR_BRANCH "git rev-parse --abbrev-ref HEAD"
#define CMD_STATUS "git status --porcelain"

static const char *INVALID_ARGUMENTS = "Invalid argument(s) in command line";
static const char *UNKNOWN_BRANCH = "Branch not specified in checkout command";
static const char *UNKNOWN_REPOSITORY =
		"Repository not specified in the clone command";
static const char *UNKNOWN_VIRT_PATH = "Virtual path is not specified";
static const char *UNKNOWN_COMMAND = "Unknown command";

struct proc_st {
	logger *logger;
	config *config;
	enum action action;
	bool repetitive;
	char *branch;
	char *repository;
	char *virtual_path;
	bool dry_run;
	const char *error_message;
	struct char_buffer *char_buffer;
	err_publisher *err_publisher;
	bool silent;
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
	case LIST:
		return "LIST";
	case PATH:
		return "PATH";
	default:
		return "UNKNOWN";
	}
}
#endif

bool proc_is_git_installed() {
	bool result;
	struct char_buffer *char_buffer = char_buffer_new(128);
	result = !xsystem("git version 2>&1", char_buffer, false);
	char_buffer_destroy(char_buffer);
	return result;
}

/*
 * Resets the soft state of this object.
 */
static inline void reset(proc *obj) {
	obj->action = UNKNOWN;
	obj->repetitive = true;
	obj->branch = NULL;
	obj->repository = NULL;
	obj->virtual_path = NULL;
	obj->dry_run = false;
	obj->error_message = NULL;
	obj->silent = false;
}

proc *proc_new(logger *logger, config *config) {
	if (!logger || !config)
		return NULL;
	proc *obj = malloc(sizeof(struct proc_st));
	obj->logger = logger;
	obj->config = config;
	obj->char_buffer = char_buffer_new(CHAR_BUFFER_LEN);
	reset(obj);
	return obj;
}

void proc_set_err_handler(proc *obj, void *err_handler_inst,
		void (*handle_err)(void *, int, const char *))
{
	obj->err_publisher = err_publisher_new(err_handler_inst, handle_err);
}

bool proc_parse_cmd_line(proc *obj, int argc, char *argv[]) {
	reset(obj);
	int i = config_get_opt_limit(obj->config);
	if (i >= argc) {
		obj->error_message = INVALID_ARGUMENTS;
		return false;
	}

	if (!strcmp(argv[i], "pull")) {
		obj->action = PULL;
		i++;
	} else if (!strcmp(argv[i], "checkout")) {
		if (++i >= argc || is_opt(argv[i])) {
			obj->error_message = UNKNOWN_BRANCH;
			return false;
		}
		obj->action = CHECKOUT;
		obj->branch = argv[i++];
	} else if (!strcmp(argv[i], "push")) {
		obj->action = PUSH;
		i++;
	} else if (!strcmp(argv[i], "clone")) {
		if (++i >= argc || is_opt(argv[i])) {
			obj->error_message = UNKNOWN_REPOSITORY;
			return false;
		}
		obj->action = CLONE;
		obj->repository = argv[i++];
	} else if (!strcmp(argv[i], "status")) {
		obj->action = STATUS;
		i++;
	} else if (!strcmp(argv[i], "list")) {
		obj->action = LIST;
		obj->silent = true;
		i++;
	} else if (!strcmp(argv[i], "path")) {
		if (++i >= argc || is_opt(argv[i])) {
			obj->error_message = UNKNOWN_VIRT_PATH;
			return false;
		}
		obj->action = PATH;
		obj->repetitive = false;
		obj->virtual_path = argv[i++];
	} else {
		obj->error_message = UNKNOWN_COMMAND;
		return false;
	}
	obj->error_message = NULL;
	if (i < argc) {
		obj->error_message = INVALID_ARGUMENTS;
		return false;
	}
	return true;
}

static int exec(proc *obj, const char *path, const char *project,
		const char *command, void (*run_before)(proc *),
		void (*run_after)(proc *))
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
static void print_branch_name(proc *obj) {
	struct char_buffer *buff = obj->char_buffer;
	char_buffer_reset(buff);
	putchar('{');
	bool colour = config_is_colour(obj->config);
	if (colour)
		printf(ANSI_COLOR_CYAN);
	if (!xsystem(CMD_CURR_BRANCH " 2>&1", buff, false)) {
		/* Trim the LF */
		buff->limit--;
		if (colour) {
			printf(!strncmp("master", buff->buffer + buff->position,
				char_buffer_len(buff)) ? ANSI_COLOR_CYAN : ANSI_COLOR_CYAN_BR);
		}
		char_buffer_print(buff);
		if (colour)
			printf(ANSI_COLOR_RESET);
	} else {
		printf("???");
	}
	putchar('}');
}

/*
 * Prints the currently checked out branch name and report changes to
 * the branch if any.
 */
static void print_branch_name_chg(proc *obj) {
	print_branch_name(obj);
	struct char_buffer *buff = obj->char_buffer;
	char_buffer_reset(buff);
	int result = xsystem(CMD_STATUS " 2>&1", buff, false);
	if (!result && buff->limit - buff->position > 0) {
		bool colour = config_is_colour(obj->config);
		if (colour)
			printf(ANSI_COLOR_RED);
		printf(" Changed!");
		if (colour)
			printf(ANSI_COLOR_RESET);
	}
	if (config_is_verbose(obj->config))
		putchar('\n');
}

static void print_action(proc *obj, const char *action, const char *project) {
	printf(" · %s ", action);
	bool color = config_is_colour(obj->config);
	if (color)
		printf(ANSI_COLOR_YELLOW);
	printf("%s ", project);
	if (color)
		printf(ANSI_COLOR_RESET);
}

static void pull(proc *obj, const char *path, const char *project) {
	print_action(obj, "Pulling", project);
	exec(obj, path, project, "git pull -p 2>&1", print_branch_name_chg, NULL);
	putchar('\n');
}

static void checkout(proc *obj, const char *path, const char *project,
		const char *branch)
{
	print_action(obj, "Checking out", project);
	char cmd[MAX_PATH];
	snprintf(cmd, MAX_PATH, "git checkout %s 2>&1", branch);
	exec(obj, path, project, cmd, NULL, print_branch_name_chg);
	putchar('\n');
}

static void push(proc *obj, const char *path, const char *project) {
	print_action(obj, "Pushing", project);
	exec(obj, path, project, "git push 2>&1", print_branch_name_chg, NULL);
	putchar('\n');
}

static void clone(proc *obj, const char *path, const char *project) {
	print_action(obj, "Cloning", project);
	putchar('\n');

	char cmd[MAX_PATH];
	snprintf(cmd, MAX_PATH, "git clone %s%s 2>&1", obj->repository, project);
	int result = exec(obj, path, NULL, cmd, NULL, NULL);
	if (result && obj->err_publisher) {
		err_publisher_fire(obj->err_publisher, result, "Failed to clone '%s'",
				project);
	}
}

static void status(proc *obj, const char *path, const char *project) {
	print_action(obj, "Found", project);
	bool prev_dry_run = obj->dry_run;
	obj->dry_run = true;
	int result = exec(obj, path, project, "git status 2>&1",
			print_branch_name_chg, NULL);
	putchar('\n');
	obj->dry_run = prev_dry_run;
	if (result && obj->err_publisher) {
		err_publisher_fire(obj->err_publisher, result,
				"Failed to retrieve status of '%s'", project);
	}
}

static void list(proc *obj, const char *path, const char *project) {
	printf("%s%c%s\n", path, path_separator(), project);
}

static void print_path(proc *obj, const char *path) {
	puts(path);
}

void proc_action(proc *obj, const char *path, const char *project) {

	if (!proc_is_repetitive(obj))
		return;

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
	case LIST:
		list(obj, path, project);
		break;
	default:
		break;
	}
}

void proc_single_action(proc *obj, void *inst,
		int (*resolve_path)(void *, const char *, char *, int))
{
	if (proc_is_repetitive(obj))
		return;
	if (!obj->virtual_path) {
		err_publisher_fire(obj->err_publisher, 1, UNKNOWN_VIRT_PATH);
		return;
	}
	if (!resolve_path)
		return;
	char path[MAX_PATH];
	int len = resolve_path(inst, obj->virtual_path, path, MAX_PATH);
	if (!len) {
		err_publisher_fire(obj->err_publisher, 1, "Invalid virtual path '%s'",
				obj->virtual_path);
		return;
	}
	switch (obj->action) {
	case PATH:
		print_path(obj, path);
		break;
	default:
		break;
	}
}

enum action proc_get_action(proc *obj) {
	return obj->action;
}

bool proc_is_repetitive(proc *obj) {
	return obj->repetitive;
}

const char *proc_get_error_message(proc *obj) {
	return obj->error_message;
}

bool proc_is_silent(proc *obj) {
	return obj->silent;
}

void proc_destroy(proc *obj) {
	char_buffer_destroy(obj->char_buffer);
	free(obj);
}
