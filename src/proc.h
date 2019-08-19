/*
 * proc.h
 *
 *  Created on: 5 Jun. 2018
 *      Author: Vlad
 */

#ifndef PROC_H_
#define PROC_H_

#include "stdbool.h"
#include "logger.h"
#include "config.h"

typedef struct proc_st proc;

enum action {
	UNKNOWN, PULL, PUSH, CHECKOUT, CLONE, STATUS, LIST, EXEC, PATH
};

/*
 * Indicates if Git DCVS is installed on this system.
 */
bool proc_is_git_installed();

/*
 * Constructs this class.
 */
proc *proc_new(logger *, config *);

/*
 * Sets the error handler for this class.
 */
void proc_set_err_handler(proc *, void *, void (*)(void *, int, const char *));

/*
 * Initialises this object off the specified command line.
 */
bool proc_parse_cmd_line(proc *, int, char *[]);

/*
 * Takes action at the specified location.
 */
void proc_action(proc *, const char *, const char *);

/*
 * Takes a single non-repetitive action if one is assigned.
 */
void proc_single_action(proc *, void *,
		int (*)(void *, const char *, char *, int));

/*
 * Returns the currently assigned action.
 */
enum action proc_get_action(proc *);

/*
 * Indicates the if the assigned action is repetitive.
 */
bool proc_is_repetitive(proc *);

/*
 * Returns the last error message or NULL if the last operation was successful.
 */
const char *proc_get_error_message(proc *);

/*
 * Indicates if the application <u>outside of</u> processor is expected
 * top operate in silent mode.
 */
bool proc_is_silent(proc *);

/*
 * Destructs the specified instance of processor.
 */
void proc_destroy(proc *);

#endif /* PROC_H_ */
