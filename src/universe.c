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
};

static void parse_file(universe *, const char *);
static void init_dconsumer(universe *, struct dconsumer *);

universe *universe_new(logger *logger, const char *file_name) {
	if (!logger)
		return NULL;
	universe *obj = malloc(sizeof(struct universe_st));
	struct dconsumer *dconsumer = malloc(sizeof(struct dconsumer));
	obj->logger = logger;
	obj->workspaces = LinkedListCreate();
	init_dconsumer(obj, dconsumer);
	obj->dconsumer = dconsumer;
	obj->parser = dpaser_new(logger, dconsumer);
	obj->default_projects = LinkedListCreate();
	obj->workspace_by_alias = HashMapCreate();
	obj->alloc_strings = LinkedListCreate();
	parse_file(obj, file_name);
	return obj;
}

static void visit_workspace(void *inst, void *workspace) {
	universe *obj = inst;
	workspace_accept(workspace, obj->inst, obj->visit);
}

void universe_accept(universe *obj, void *inst,
		void (*visit)(void *, const char *, const char *, const char *)) {
	obj->visit = visit;
	obj->inst = inst;
	LinkedListTraverse(obj->workspaces, obj, visit_workspace);
}

/* Frees the memory allocated to the specified character sequence */
static void destroy_string(void *inst, void *s) {
	DEBUG_LOG(((universe *)inst)->logger, "Destroying string '%s'...\n",
			(char *)s);
	free(s);
}

/* Frees the memory allocated too the specified key and value of workspace
 * type.
 */
static void destroy_key_value(void *inst, char *key, void *value) {
	DEBUG_LOG(((universe *)inst)->logger, "Destroying key '%s'...\n",
			(char *)key);
	free(key);
	workspace_destroy(value);
}

void universe_destroy(universe *obj) {
	LinkedListDestroy(obj->workspaces);
	dparser_destroy(obj->parser);
	/* Remove the dynamically allocated project strings before destroying
	 * the list that holds them.
	 */
	LinkedListTraverse(obj->default_projects, obj, destroy_string);
	LinkedListDestroy(obj->default_projects);

	/* Remove the dynamically allocated keys and values */
	HashMapTraverse(obj->workspace_by_alias, obj, destroy_key_value);
	HashMapDestroy(obj->workspace_by_alias);

	/* Remove the dynamically allocated custom project names along with
	 * the list that contains them.
	 */
	LinkedListTraverse(obj->alloc_strings, obj, destroy_string);
	LinkedListDestroy(obj->alloc_strings);
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
		printf("File not found: %s\n", file_name);
		return;
	}
	/* Process the file */
	while ((c = fgetc(infile)) != EOF)
		dparser_proc_char(obj->parser, c);
	fclose(infile);
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
	LinkedListAdd(obj->default_projects, strdup(project));
}

static void add_workspace(void *inst, const char *alias, const char *path) {
	universe *obj = inst;
	DEBUG_LOG(obj->logger, "universe: add_workspace: '%s' -> '%s'\n", alias,
			path);
	workspace *workspace = HashMapGet(obj->workspace_by_alias, (char *)alias);
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
		LinkedListAdd(obj->alloc_strings, path_copy);
		workspace = workspace_new(alias_copy, path_copy);
		LinkedListAdd(obj->workspaces, workspace);
		HashMapPut(obj->workspace_by_alias, alias_copy, workspace);
	}
	LinkedListTraverse(obj->default_projects, workspace, add_to_workspace);
}

void add_workspace_project(void *inst, const char *alias, const char *project) {
	universe *obj = inst;
	DEBUG_LOG(obj->logger, "universe: add_workspace_project: alias=%s, project=%s\n",
			alias, project);
	workspace *workspace = HashMapGet(obj->workspace_by_alias, (char *)alias);
	if (workspace) {
		/* We need to copy the project name as it can mutate later */
		char *project_copy = strdup(project);
		/* Put on the list of custom projects so that we can GC it later */
		LinkedListAdd(obj->alloc_strings, project_copy);
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
