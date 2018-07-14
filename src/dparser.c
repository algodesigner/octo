/*
 * dparser.c
 *
 * Workspace declaration stream parser. It passes the generic project, workspace
 * and workspace project declarations to the nominated consumer.
 *
 *  Created on: 7 Jun. 2018
 *      Author: Vlad
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dparser.h"
#include "dconsumer.h"
#include "logger.h"

#define BUFFER_SIZE 1024

/*
 * TODO Implement error detection and handling.
 */

/*
 * Specifier values.
 */
static const char *SPEC_PROJECTS = "projects";
static const char *SPEC_WORKSPACE = "workspace";
static const char *OP_POINTER = "->";

/*
 * Parsing state enumeration.
 */
enum parsing_state {
	IDLE, COMMENT, TOKEN
};

/*
 * Grammatic mode enumeration.
 */
enum mode {
	CONTROL, PROJ, PROJ_TUPLE, WORKSPACE, W_ALIAS, W_POINTER, W_PATH, W_TUPLE
};

/*
 * Grammatic token structure.
 */
struct token {
	char buffer[BUFFER_SIZE];
	int pos;
	int capacity;
};

/*
 * Parser state.
 */
struct dparser_st {
	logger *logger;
	enum parsing_state parsing_state;
	enum mode mode;
	struct token token;
	struct token token2;
	const struct dconsumer *dconsumer;
	const char *err_msg;
};

dparser *dpaser_new(logger *logger, const struct dconsumer *dconsumer) {
	dparser *obj = (dparser *)malloc(sizeof(struct dparser_st));
	if (!logger)
		return NULL;
	obj->logger = logger;
	obj->parsing_state = IDLE;
	obj->mode = CONTROL;
	obj->dconsumer = dconsumer;
	obj->err_msg = NULL;
	return obj;
}

/*
 * Puts the zero character terminator at the end of the token buffer for
 * convenience.
 */
static void put_terminator(struct token *t) {
	int index = t->pos >= BUFFER_SIZE ? BUFFER_SIZE - 1 : t->pos;
	t->buffer[index] = 0;
}

static void finalise_token(dparser *obj) {
	/*
	 * Zero-terminate the token buffer so that we can pass into various
	 * call back functions.
	 */
	put_terminator(&obj->token);

	switch (obj->mode) {
	case CONTROL:
		/*
		 * If we are in the CONTROL mode and we have just seen the projects
		 * specifier, switch to the projects tuple mode.
		 */
		if (!strcmp(obj->token.buffer, SPEC_PROJECTS)) {
			obj->mode = PROJ;
			DEBUG_LOG(obj->logger, "proj_token: %s\n", obj->token.buffer);
		} else if (!strcmp(obj->token.buffer, SPEC_WORKSPACE)) {
			obj->mode = WORKSPACE;
			DEBUG_LOG(obj->logger, "w_token: %s\n", obj->token.buffer);
		}
		break;
	case PROJ_TUPLE:
		/* Notify the consumer of this generic project declaration */
		obj->dconsumer->add_project(obj->dconsumer->instance,
				obj->token.buffer);
		break;
	case WORKSPACE:
		obj->mode = W_ALIAS;
		/* Copy this token to the secondary slot which we will need when
		 * we deliver the workspace notification to the consumer interface.
		 */
		strcpy(obj->token2.buffer, obj->token.buffer);
		obj->token2.pos = obj->token.pos;
		DEBUG_LOG(obj->logger, "w_alias: %s\n", obj->token.buffer);
		break;
	case W_ALIAS:
		if (!strcmp(obj->token.buffer, OP_POINTER)) {
			obj->mode = W_POINTER;
			DEBUG_LOG(obj->logger, "Found a workspace pointer\n");
		} else {
			/* TODO We have a problem... */
		}
		break;
	case W_POINTER:
		obj->mode = W_PATH;
		/* Notify the consumer of this workspace declaration. */
		obj->dconsumer->add_workspace(obj->dconsumer->instance,
				obj->token2.buffer, obj->token.buffer);
		DEBUG_LOG(obj->logger, "w_path: %s\n", obj->token.buffer);
		break;
	case W_TUPLE:
		/* Notify the consumer of this workspace project declaration */
		obj->dconsumer->add_workspace_project(obj->dconsumer->instance,
				obj->token2.buffer, obj->token.buffer);
		break;

	default:
		break;
	}

	/* Print out the token for debugging purposes */
	DEBUG_LOG(obj->logger, "Token: %s\n", obj->token.buffer);
}

const char *dparser_proc_char(dparser *obj, int c) {
	switch (c) {
	case ' ':
	case '\t':
		if (obj->parsing_state == TOKEN) {
			obj->parsing_state = IDLE;
			finalise_token(obj);
		}
		break;
	case '#':
		/* If we are on the new line and we see the comment character,
		 * enter the COMMENT state.
		 */
		if (obj->parsing_state == IDLE) {
			obj->parsing_state = COMMENT;
			DEBUG_LOG(obj->logger, "Found a new comment\n");
		}
		break;
	case '\n':
		if (obj->parsing_state == TOKEN)
			finalise_token(obj);
		obj->parsing_state = IDLE;
		break;
	case '{':
		if (obj->parsing_state == TOKEN)
			finalise_token(obj);
		switch (obj->mode) {
		case PROJ:
			obj->mode = PROJ_TUPLE;
			DEBUG_LOG(obj->logger, "Entered PROJ_TUPLE mode...\n");
			break;
		case W_PATH:
			obj->mode = W_TUPLE;
			DEBUG_LOG(obj->logger, "Entered W_TUPLE mode...\n");
			break;
		default:
			break;
		}
		break;
	case '}':
		if (obj->parsing_state == TOKEN)
			finalise_token(obj);
		/* We are definitely back to CONTROL mode */
		obj->mode = CONTROL;
		break;
	default:
		if (c >= 0x20 && c <= 0x7E) {
			if (obj->parsing_state == IDLE) {
				obj->parsing_state = TOKEN;
				obj->token.pos = 0;
				DEBUG_LOG(obj->logger, "Found a new token\n");
			}
			if (obj->parsing_state == TOKEN)
				obj->token.buffer[obj->token.pos++] = (char)c;
		}
		break;
	}
	return NULL;
}

void dparser_destroy(dparser *obj) {
	free(obj);
}
