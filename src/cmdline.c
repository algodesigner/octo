/*
 * cmdline.c
 *
 *  Created on: 30 Jun. 2018
 *      Author: vlad
 */
#include <stdbool.h>

bool is_opt(const char *s) {
	return s && s[0] == '-';
}

/*
 * Indicates if the option contained in the first argument matched the option
 * provided in the second one.
 */
bool equal_opts(char *arg, char *o) {
	if (!is_opt(arg))
		return false;
	/*
	 * The option name contained in the argument can end with '=' in case
	 * of assignment.
	 */
	for (; *arg && o && *arg != '='; arg++, o++)
		if (*arg != *o)
			return false;
	return true;
}

int get_opt(char *opt, int argc, char *argv[]) {
	int i;
	for (i = 1; i < argc; i++) {
		/* A really tricky piece of logic */
		if (equal_opts(argv[i], opt))
			return i;
	}
	return -1;
}
