/*
 * config.c
 *
 *  Created on: 4 Jul. 2018
 *      Author: vlad
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "config.h"
#include "cmdline.h"
#include "utils.h"

struct config_st {
	int opt_limit;
	char *workspace_name;
	char *def_file_name;
	bool verbose;
};

static void reset(config *obj) {
	obj->opt_limit = 0;
	obj->workspace_name = obj->def_file_name = NULL;
	obj->verbose = true;
}

config *config_new() {
	config *obj = malloc(sizeof(struct config_st));
	reset(obj);
	return obj;
}

static char *parse_workspace_name(config *obj, char *arg) {
	char *src = strchr(arg, '=');
	if (!src || !*++src)
		return "Invalid workspace option";
	obj->workspace_name = strdup(src);
	return NULL;
}

static char *parse_def_file_name(config *obj, char *arg) {
	char *src = strchr(arg, '=');
	if (!src || !*++src)
		return "Invalid definition file option";
	obj->def_file_name = strdup(src);
	return NULL;
}

__attribute__((always_inline))
static inline void mark_opt_limit(config *obj, int index) {
	if (!obj->opt_limit)
		obj->opt_limit = index;
}

char *config_parse_cmd_line(config *obj, int argc, char *argv[]) {
	reset(obj);
	for (int i = 0; i < argc; i++) {
		char *err_msg = NULL;
		if (equal_opts(argv[i], "--workspace")) {
			err_msg = parse_workspace_name(obj, argv[i]);
			mark_opt_limit(obj, i);
		} else if (equal_opts(argv[i], "--def")) {
			err_msg = parse_def_file_name(obj, argv[i]);
			mark_opt_limit(obj, i);
		} else if (!strcmp(argv[i], "--verbose")) {
			obj->verbose = true;
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

int config_get_opt_limit(config *obj) {
	return obj->opt_limit;
}

char *config_get_workspace_name(config *obj) {
	return obj->workspace_name;
}

char *config_get_def_file_name(config *obj) {
	return obj->def_file_name;
}

bool config_is_verbose(config *obj) {
	return obj->verbose;
}

void config_destroy(config *obj) {
	free(obj->workspace_name);
	free(obj->def_file_name);
	free(obj);
}
