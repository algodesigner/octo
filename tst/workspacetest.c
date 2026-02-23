#include <stdio.h>
#include <string.h>
#include "workspace.h"

#include "workspacetest.h"
#include "linkedlist.h"

static const char* NAME = "w1";
static const char* PATH = "/path/to/workspace";
static const char* PROJECT1 = "project1";
static const char* PROJECT2 = "project2";

struct context {
	int project_num;
	HLINKEDLIST names;
	HLINKEDLIST paths;
	HLINKEDLIST projects;
};

static void context_init(struct context *context) {
	context->project_num = 0;
	context->names = linked_list_create();
	context->paths = linked_list_create();
	context->projects = linked_list_create();
}

static void context_deinit(struct context *context) {
	linked_list_destroy(context->names);
	linked_list_destroy(context->paths);
	linked_list_destroy(context->projects);
}

static void check_construction(tester *tst) {
	workspace *workspace = workspace_new(NAME, PATH);
	tester_assert(tst, workspace != NULL, "check_construction");
	workspace_destroy(workspace);
}

static void visit_workspace(void *inst, const char *name, const char *path,
		const char *project) {
	struct context *context = inst;
	context->project_num++;
	linked_list_add(context->names, (void *) name);
	linked_list_add(context->paths, (void *) path);
	linked_list_add(context->projects, (void *) project);
}

static void check_behaviour(tester *tst) {

	struct context context;
	context_init(&context);

	workspace *workspace = workspace_new(NAME, PATH);
	workspace_add_dir(workspace, PROJECT1);
	workspace_add_dir(workspace, PROJECT2);
	/* Deliberately add PROJECT1 the second time */
	workspace_add_dir(workspace, PROJECT1);

	workspace_accept(workspace, &context, visit_workspace);

	tester_assert(tst, context.project_num == 2, "check_behaviour");
	tester_assert(tst, linked_list_get_size(context.names) == 2,
			"check_behaviour");
	tester_assert(tst, linked_list_get_size(context.paths) == 2,
			"check_behaviour");
	tester_assert(tst, linked_list_get_size(context.projects) == 2,
			"check_behaviour");
	tester_assert(tst, strcmp(linked_list_get(context.names, 0), NAME) == 0,
			"check_behaviour");
	tester_assert(tst, strcmp(linked_list_get(context.names, 1), NAME) == 0,
			"check_behaviour");
	tester_assert(tst, strcmp(linked_list_get(context.paths, 0), PATH) == 0,
			"check_behaviour");
	tester_assert(tst, strcmp(linked_list_get(context.paths, 1), PATH) == 0,
			"check_behaviour");
	tester_assert(tst,
			strcmp(linked_list_get(context.projects, 0), PROJECT1) == 0,
			"check_behaviour");
	tester_assert(tst,
			strcmp(linked_list_get(context.projects, 1), PROJECT2) == 0,
			"check_behaviour");

	workspace_destroy(workspace);

	context_deinit(&context);
}

void test_workspace(tester *tst) {
	tester_new_group(tst, "test_workspace");
	check_construction(tst);
	check_behaviour(tst);
}
