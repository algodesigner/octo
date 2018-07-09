/*
 * xsystem.c
 *
 *  Created on: 26 Jun. 2018
 *      Author: vlad
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include "xsystem.h"

#if  !defined(pipe) && defined(__MINGW32__)
#define pipe(fds) _pipe(fds, 8192, 0)
#endif

struct char_buffer *char_buffer_new(int length) {
	char *buffer = malloc(length);
	if (!buffer)
		return NULL;
	struct char_buffer *obj = malloc(sizeof(struct char_buffer));
	obj->buffer = buffer;
	obj->capacity = length;
	char_buffer_reset(obj);
	return obj;
}

void char_buffer_reset(struct char_buffer *obj) {
	obj->position = 0;
	obj->limit = obj->capacity;
}

void char_buffer_destroy(struct char_buffer *obj) {
	free(obj->buffer);
	free(obj);
}

void char_buffer_print(struct char_buffer *obj) {
	int i;
	for (i = obj->position; i < obj->limit; i++) {
		char c = obj->buffer[i];
		putchar(c);
		/* putchar(c >= 0x20 && c < 0x7F ? c : '.'); */
	}
}

int xsystem(const char *cmd, struct char_buffer *dst, bool verbose) {

	/*
	 * Flush stdout as the command might take a while to execute leaving
	 * the output split.
	 */
	fflush(stdout);

	if (!dst)
		return system(cmd);

	FILE *fp = popen(cmd, "r");
	if (!fp) {
		fprintf(stderr, "Error popen with %s\n", cmd);
		dst->position = dst->limit = 0;
		return 1;
	}

	int ch;
	int prev_position = dst->position;

	// Read from the process and print
	while ((ch = fgetc(fp)) != EOF) {
		if (verbose)
			putchar(ch);
		if (dst->position < dst->limit)
			dst->buffer[dst->position++] = ch;
	}

	// Flip char_buffer
	dst->limit = dst->position;
	dst->position = prev_position;

	return pclose(fp);
}
