/*
 * workspace.c
 *
 *  Created on: 5 Jun. 2018
 *      Author: Vlad
 */
#include <stdlib.h>
#include <stddef.h>
#include "workspace.h"
#include "linkedhashset.h"

struct workspace_st {
	const char *name;
	const char *path;
	linked_hash_set *projects;
	void (*visit)(void *, const char *, const char *, const char *);
	void *inst;
};

workspace *workspace_new(const char *name, const char *path) {
	if (!name || !path)
		return NULL;
	workspace *obj = malloc(sizeof(struct workspace_st));
	obj->name = name;
	obj->path = path;
	obj->projects = linked_hash_set_new();
	return obj;
}

void workspace_add_dir(workspace *obj, const char *dir) {
	linked_hash_set_add(obj->projects, (char *)dir);
}

static void visit_list_item(void *inst, void *project) {
	workspace *obj = inst;
	obj->visit(obj->inst, obj->name, obj->path, project);
}

void workspace_accept(workspace *obj, void *inst,
		void (*visit)(void *, const char *, const char *, const char *))
{
	obj->inst = inst;
	obj->visit = visit;
	linked_hash_set_traverse(obj->projects, obj, visit_list_item);
}

void workspace_destroy(workspace *obj) {
	linked_hash_set_destroy(obj->projects);
	free(obj);
}
