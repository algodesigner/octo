/*
 * cmdline.c
 *
 *  Created on: 30 Jun. 2018
 *      Author: vlad
 */
#include <stdbool.h>

/*
 * Indicates if the specified string is an option.
 */
bool is_opt(const char *s) {
	return s && s[0] == '-';
}
