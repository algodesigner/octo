/*
 * ls.c
 *
 *  Created on: 5 Jun. 2018
 *      Author: Vlad
 */

/* Includes _BSD_SOURCE for DT_UNKNOWN etc. */
#define _GNU_SOURCE
#include <dirent.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Indicates if the specified directory entry is of directory type.
 */
static _Bool is_dir_type(struct dirent *de) {
	_Bool dir_type;
#ifdef _DIRENT_HAVE_D_TYPE
	if (de->d_type != DT_UNKNOWN && de->d_type != DT_LNK) {
		// don't have to stat if we have d_type info, unless it's a symlink
		// (since we stat, not lstat)
		dir_type = (de->d_type == DT_DIR);
	} else
#endif
	{
		/*
		 * The only method if d_type isn't available, otherwise this is a
		 * fall-back for FSes where the kernel leaves it DT_UNKNOWN.
		 */
		struct stat stbuf;
		/* stat follows symlinks, lstat doesn't. */
		stat(de->d_name, &stbuf);
		/* TODO: error check */
		dir_type = S_ISDIR(stbuf.st_mode);
	}
	return dir_type;
}

void ls(const char *path, void (*visit)(struct dirent *)) {
	/* POSIX doesn't require this to be a plain file descriptor. */
	/* Linux uses open(".", O_DIRECTORY); to implement this */
	DIR *dir = opendir(path);
	if (dir == NULL) {
		printf("Could not open current directory");
		return;
	}

	struct dirent *de;
	while ((de = readdir(dir)) != NULL) { // NULL means end of directory
		if (is_dir_type(de))
			visit(de);
	}
}
