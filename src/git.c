/*
 * git.c
 * Git controller class.
 *
 *  Created on: 5 Jun. 2018
 *      Author: Vlad
 */
#undef DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "git.h"
#include "utils.h"
#include "xsystem.h"
#include "logger.h"

#define MAX_PATH 1024
#define CHAR_BUFFER_LEN 8192

#define CMD_CURR_BRANCH "git symbolic-ref --short HEAD"

static const char* INVALID_ARGUMENTS = "Invalid argument(s) in command line";
static const char* UNKNOWN_BRANCH = "Branch not specified in checkout command";

struct git_st {
	logger *logger;
	enum action action;
	char *branch;
	const char *error_message;
	struct char_buffer *char_buffer;
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
		default:
		return "UNKNOWN";
	}
}
#endif

bool git_is_installed() {
	bool result;
	struct char_buffer *char_buffer = char_buffer_new(128);
	result = !_xsystem("git version 2>&1", char_buffer, false);
	char_buffer_destroy(char_buffer);
	return result;
}

/*
 * Resets the soft state of this object.
 */
static inline void reset(git *obj) {
	obj->action = UNKNOWN;
	obj->branch = NULL;
	obj->error_message = NULL;
}

git *git_new(logger *logger) {
	if (!logger)
		return NULL;
	git *obj = malloc(sizeof(struct git_st));
	obj->logger = logger;
	obj->char_buffer = char_buffer_new(CHAR_BUFFER_LEN);
	reset(obj);
	return obj;
}

bool git_parse_cmd_line(git *obj, int argc, char *argv[]) {

	reset(obj);
	if (argc <= 1) {
		obj->error_message = INVALID_ARGUMENTS;
		return false;
	}

	if (!strcmp(argv[1], "pull")) {
		DEBUG_LOG(obj->logger, "pull command\n");
		obj->action = PULL;
	} else if (!strcmp(argv[1], "checkout")) {
		if (argc < 3) {
			obj->error_message = UNKNOWN_BRANCH;
			return false;
		}
		DEBUG_LOG(obj->logger, "checkout command\n");
		obj->action = CHECKOUT;
		obj->branch = argv[2];
	} else if (!strcmp(argv[1], "push")) {
		DEBUG_LOG(obj->logger, "push command\n");
		obj->action = PUSH;
	} else if (!strcmp(argv[1], "clone")) {
		DEBUG_LOG(obj->logger, "clone command\n");
		obj->action = CLONE;
	}
	obj->error_message = NULL;
	return true;
}

static int exec(git *obj, const char *path, const char *project,
		const char *command, void (*run)(git *))
{
	int result;
	result = -1;
	char cwd[MAX_PATH];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		DEBUG_LOG(obj->logger, "exec: current working dir: %s\n", cwd);
		const char *dir_path;
		if (project) {
			char full_path[MAX_PATH];
			snprintf(full_path, MAX_PATH, "%s%c%s", path, path_separator(),
					project);
			dir_path = full_path;
		} else
			dir_path = path;

		DEBUG_LOG(obj->logger, "exec: cd %s\n", dir_path);
		if (!chdir(dir_path)) {
			/* Run the task if provided */
			if (run)
				run(obj);

			/* Execute the command */
			DEBUG_LOG(obj->logger, "exec: %s\n", command);
			char_buffer_reset(obj->char_buffer);
			result = _xsystem(command, obj->char_buffer, false);
			DEBUG_LOG(obj->logger, "exec: result=%d\n", result);
			DEBUG_LOG(obj->logger, "exec: cd %s\n", cwd);
			chdir(cwd);
		}
	}
	return result;
}

void print_branch_name(git *obj) {
	char_buffer_reset(obj->char_buffer);
	if (!_xsystem(CMD_CURR_BRANCH " 2>&1", obj->char_buffer, false)) {
		/* Trim the LF */
		obj->char_buffer->limit--;
		putchar('{');
		char_buffer_print(obj->char_buffer);
		puts("}");
	}
}

static void pull(git *obj, const char *path, const char *project) {
	printf(" o Pulling %s%c%s ", path, path_separator(), project);
	exec(obj, path, project, "git pull -p 2>&1", print_branch_name);
}

static void checkout(git *obj, const char *path, const char *project,
		const char *branch)
{
	printf(" o Checking out %s%c%s {%s}\n", path, path_separator(), project,
			branch);
	char cmd[MAX_PATH];
	snprintf(cmd, MAX_PATH, "git checkout %s 2>&1", branch);
	exec(obj, path, project, cmd, NULL);
}

static void push(git *obj, const char *path, const char *project) {
	printf(" o Pushing %s%c%s ", path, path_separator(), project);
	exec(obj, path, project, "git push 2>&1", print_branch_name);
}

static void clone(git *obj, const char *path, const char *project) {
	printf(" o Cloning %s%c%s\n", path, path_separator(), project);
	char cmd[MAX_PATH];
	/* TODO Add a configurable repository! */
	snprintf(cmd, MAX_PATH, "git clone git@echidna.algoteq.com:%s 2>&1",
			project);
	exec(obj, path, NULL, cmd, NULL);
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
	free(obj);
}
