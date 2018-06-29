/*
 * utils.c
 *
 *  Created on: 10 Jun. 2018
 *      Author: Vlad
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

char *strdup(const char * s) {
	size_t len = strlen(s) + 1;
	char *p = malloc(len);
	return p ? memcpy(p, s, len) : NULL;
}

char *get_home() {
	char homedir[MAX_PATH];
#ifdef _WIN32
	snprintf(homedir, MAX_PATH, "%s%s", getenv("HOMEDRIVE"),
			getenv("HOMEPATH"));
#elif defined(__CYGWIN__)
	snprintf(homedir, MAX_PATH, "%s", getenv("USERPROFILE"));
#else
	snprintf(homedir, MAX_PATH, "%s", getenv("HOME"));
#endif
	return strdup(homedir);
}

char path_separator() {
#ifdef _WIN32
	return '\\';
#else
	return '/';
#endif
}
