/*
 * cmdline.h
 *
 *  Command line utility functions.
 *
 *  Created on: 30 Jun. 2018
 *      Author: vlad
 */

#ifndef CMDLINE_H_
#define CMDLINE_H_

#include <stdbool.h>

/*
 * Indicates if the specified string is an option.
 */
bool is_opt(const char *);

/*
 * Checks if the specified argument contains a given option.
 */
bool equal_opts(char *arg, char *o);

/*
 * Retrieves the index of the specified command line option or -1 if not found.
 */
int get_opt(char *, int, char **);

#endif /* CMDLINE_H_ */
