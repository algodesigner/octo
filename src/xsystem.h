/*
 * xsystem.h
 *
 *  Created on: 26 Jun. 2018
 *      Author: vlad
 */

#ifndef XSYSTEM_H_
#define XSYSTEM_H_

#include <stdbool.h>

struct char_buffer {
	char *buffer;
	int position;
	int limit;
	int capacity;
};

struct char_buffer *char_buffer_new(int);
int char_buffer_len(struct char_buffer *);
void char_buffer_reset(struct char_buffer *);
void char_buffer_print(struct char_buffer *);
void char_buffer_destroy(struct char_buffer *);

/*
 * Executes the specified command and stores its output (on stdout) in a quiet
 * or verbose mode.
 */
int xsystem(const char *, struct char_buffer *, bool);

#endif /* XSYSTEM_H_ */
