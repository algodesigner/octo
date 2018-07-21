/*
 * workspace.h
 *
 *  Created on: 5 Jun. 2018
 *      Author: Vlad
 */

#ifndef WORKSPACE_H_
#define WORKSPACE_H_

typedef struct workspace_st workspace;

/*
 * Creates a new instance of the workpace with the specified name and path.
 */
workspace *workspace_new(const char *, const char *);

/**
 * Adds the specified directory entry to this workspace.
 */
void workspace_add_dir(workspace *, const char *);

/*
 * Accepts the specified visitor.
 */
void workspace_accept(workspace *, void *,
		void (*)(void *, const char *, const char *, const char *));

/*
 * Returns the path to this workspace.
 */
const char *workpace_get_path(workspace *);

/**
 * Releases the resources claimed by the workspace and destroys it.
 */
void workspace_destroy(workspace *);

#endif /* WORKSPACE_H_ */
