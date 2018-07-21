/*
 * universe.c
 *
 *  Created on: 5 Jun. 2018
 *      Author: Vlad
 */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "universe.h"
#include "workspace.h"
#include "dparser.h"
#include "dconsumer.h"
#include "linkedlist.h"
#include "hashmap.h"
#include "utils.h"
#include "logger.h"
#include "errpublisher.h"

struct universe_st {
	logger *logger;
	HLINKEDLIST workspaces;
	struct dconsumer *dconsumer;
	dparser *parser;
	HLINKEDLIST default_projects;
	HHASHMAP workspace_by_alias;
	HLINKEDLIST alloc_strings;
	void (*visit)(void *, const char *, const char *, const char *);
	void *inst;
	err_publisher *err_publisher;
};

static void parse_file(universe *, const char *);
static void init_dconsumer(universe *, struct dconsumer *);

universe *universe_new(logger *logger, const char *file_name,
		void *err_handler_inst, void (*handle_err)(void *, int, const char *))
{
	if (!logger)
		return NULL;
	universe *obj = malloc(sizeof(struct universe_st));
	struct dconsumer *dconsumer = malloc(sizeof(struct dconsumer));
	obj->logger = logger;
	obj->workspaces = linked_list_create();
	init_dconsumer(obj, dconsumer);
	obj->dconsumer = dconsumer;
	obj->parser = dpaser_new(logger, dconsumer);
	obj->default_projects = linked_list_create();
	obj->workspace_by_alias = hash_map_create();
	obj->alloc_strings = linked_list_create();
	obj->err_publisher = err_publisher_new(err_handler_inst, handle_err);
	parse_file(obj, file_name);
	return obj;
}

static void visit_workspace(void *inst, void *workspace) {
	universe *obj = inst;
	workspace_accept(workspace, obj->inst, obj->visit);
}

void universe_accept(universe *obj, void *inst,
		void (*visit)(void *, const char *, const char *, const char *))
{
	obj->visit = visit;
	obj->inst = inst;
	linked_list_traverse(obj->workspaces, obj, visit_workspace);
}

const char *universe_get_workspace_path(universe *obj, const char *alias) {
	workspace *w = hash_map_get(obj->workspace_by_alias, (char *)alias);
	return w ? workpace_get_path(w) : NULL;
}

/* Frees the memory allocated to the specified character sequence */
static void destroy_string(void *inst, void *s) {
	DEBUG_LOG(((universe * )inst)->logger, "Destroying string '%s'...\n",
			(char * )s);
	free(s);
}

/* Frees the memory allocated too the specified key and value of workspace
 * type.
 */
static void destroy_key_value(void *inst, char *key, void *value) {
	DEBUG_LOG(((universe * )inst)->logger, "Destroying key '%s'...\n",
			(char * )key);
	free(key);
	workspace_destroy(value);
}

void universe_destroy(universe *obj) {
	err_publisher_destroy(obj->err_publisher);
	linked_list_destroy(obj->workspaces);
	dparser_destroy(obj->parser);
	/* Remove the dynamically allocated project strings before destroying
	 * the list that holds them.
	 */
	linked_list_traverse(obj->default_projects, obj, destroy_string);
	linked_list_destroy(obj->default_projects);

	/* Remove the dynamically allocated keys and values */
	hash_map_traverse(obj->workspace_by_alias, obj, destroy_key_value);
	hash_map_destroy(obj->workspace_by_alias);

	/* Remove the dynamically allocated custom project names along with
	 * the list that contains them.
	 */
	linked_list_traverse(obj->alloc_strings, obj, destroy_string);
	linked_list_destroy(obj->alloc_strings);
	free(obj->dconsumer);
	free(obj);
}

/*
 * Parses the specified file containing repository and workspace defintions.
 */
static void parse_file(universe *obj, const char *file_name) {
	int c;
	FILE *infile = fopen(file_name, "r");
	if (!infile) {
		err_publisher_fire(obj->err_publisher, 0, "File not found: %s", file_name);
		return;
	}
	/* Process the file */
	register const char *err_msg = NULL;
	while ((c = fgetc(infile)) != EOF && !err_msg)
		err_msg = dparser_proc_char(obj->parser, c);
	fclose(infile);
	if (err_msg)
		err_publisher_fire(obj->err_publisher, 0, err_msg);
}

/**
 * Adds a project to the specified instance of the workspace.
 */
static void add_to_workspace(void *inst, void *project) {
	workspace_add_dir(inst, project);
}

static void add_project(void *inst, const char *project) {
	universe *obj = inst;
	DEBUG_LOG(obj->logger, "universe: add_project: %s\n", project);
	/* Duplicate the project character sequence as the original is mutable.
	 * The duplicates will be destroyed immediately before the list is.
	 */
	linked_list_add(obj->default_projects, strdup(project));
}

static void add_workspace(void *inst, const char *alias, const char *path) {
	universe *obj = inst;
	DEBUG_LOG(obj->logger, "universe: add_workspace: '%s' -> '%s'\n", alias,
			path);
	workspace *workspace = hash_map_get(obj->workspace_by_alias, (char *)alias);
	if (!workspace) {
		/* The alias copies used as the keys and the workspaces will be
		 * explicitly garbage-collected when before the map is destroyed
		 * in unverse_destroy(universe *)
		 */
		char *alias_copy = strdup(alias);
		char *path_copy = strdup(path);
		/* Register the newly allocated path copy as dynamically allocated
		 * so that it can be garbage-collected before destruction.
		 */
		linked_list_add(obj->alloc_strings, path_copy);
		workspace = workspace_new(alias_copy, path_copy);
		linked_list_add(obj->workspaces, workspace);
		hash_map_put(obj->workspace_by_alias, alias_copy, workspace);
	}
	linked_list_traverse(obj->default_projects, workspace, add_to_workspace);
}

void add_workspace_project(void *inst, const char *alias, const char *project) {
	universe *obj = inst;
	DEBUG_LOG(obj->logger,
			"universe: add_workspace_project: alias=%s, project=%s\n", alias,
			project);
	workspace *workspace = hash_map_get(obj->workspace_by_alias, (char *)alias);
	if (workspace) {
		/* We need to copy the project name as it can mutate later */
		char *project_copy = strdup(project);
		/* Put on the list of custom projects so that we can GC it later */
		linked_list_add(obj->alloc_strings, project_copy);
		workspace_add_dir(workspace, project_copy);
	}
}

/*
 * Initialises the declaration consumer interface.
 */
static void init_dconsumer(universe *obj, struct dconsumer *dconsumer) {
	dconsumer->instance = obj;
	dconsumer->add_project = add_project;
	dconsumer->add_workspace = add_workspace;
	dconsumer->add_workspace_project = add_workspace_project;
}
