/*
 * errpublisher.c
 *
 *  Created on: 14 Jul. 2018
 *      Author: Vlad
 */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include "errpublisher.h"

#define TMP_BUFFER_SIZE 1024

struct err_publisher_st {
	void *err_handler_inst;
	void (*handle_err)(void *, int, const char *);
	char *tmp_buffer;
};

err_publisher *err_publisher_new(void *err_handler_inst,
		void (*handle_err)(void *, int, const char *))
{
	if (!handle_err)
		return NULL;
	err_publisher *obj = malloc(sizeof(struct err_publisher_st));
	obj->err_handler_inst = err_handler_inst;
	obj->handle_err = handle_err;
	obj->tmp_buffer = malloc(TMP_BUFFER_SIZE);
	return obj;
}

void throw_err(err_publisher *obj, int err_code, const char *fmt, ...) {
	if (!obj->handle_err)
		return;
	va_list args;
	va_start(args, fmt);
	vsnprintf(obj->tmp_buffer, TMP_BUFFER_SIZE, fmt, args);
	va_end(args);
	obj->handle_err(obj->err_handler_inst, err_code, obj->tmp_buffer);
}

void err_publisher_destroy(err_publisher *obj) {
	free(obj->tmp_buffer);
	free(obj);
}
