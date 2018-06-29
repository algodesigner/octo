/*
 * utils.h
 *
 * A collection of utility functions.
 *
 *  Created on: 10 Jun. 2018
 *      Author: Vlad
 */

#ifndef UTILS_H_
#define UTILS_H_

#define MAX_PATH 1024

/*
 * Duplicates the specified zero-terminated character sequence. The user is
 * advised to "free" it when it is no longer required to avoid memory leaks.
 */
char *strdup(const char * s);

/*
 * Returns the home directory.
 */
char *get_home();

/*
 * Returns the file path separator character.
 */
char path_separator();

#endif /* UTILS_H_ */
