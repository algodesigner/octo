/*
 * git.h
 *
 *  Created on: 5 Jun. 2018
 *      Author: Vlad
 */

#ifndef GIT_H_
#define GIT_H_

#include "stdbool.h"
#include "logger.h"
#include "config.h"

typedef struct git_st git;

enum action {
	UNKNOWN, PULL, PUSH, CHECKOUT, CLONE, STATUS
};

/*
 * Indicates if Git DCVS is installed on this system.
 */
bool git_is_installed();

/*
 * Constructs this class.
 */
git *git_new(logger *, config *);

/*
 * Sets the error handler for this class.
 */
void git_set_error_handler(git *, void *, void (*)(void *, int, const char *));

/*
 * Initialises this object off the specified command line.
 */
bool git_parse_cmd_line(git *, int, char *[]);

/*
 * Runs the git process at the specified location.
 */
void git_action(git *, const char *, const char *);

/*
 * Returns the currently assigned action.
 */
enum action git_get_action(git *);

/*
 * Returns the last error message or NULL if the last operation was successful.
 */
const char *git_get_error_message(git *);

/*
 * Destructs the specified instance of git controller.
 */
void git_destroy(git *);

#endif /* GIT_H_ */
