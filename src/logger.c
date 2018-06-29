/*
 * Copyright (c) 2013 Vlad Shurupov. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are not permitted.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include "logger.h"

struct logger_s {
	int session_id;
	FILE *file;
};

const char *log_levels[] = { "WARNING", "INFO", "DEBUG" };

logger *logger_create(int session_id, FILE *file) {
	logger *obj = (logger *)malloc(sizeof(logger));
	if (obj == NULL)
		return NULL;
	if (session_id < -1)
		return NULL;
	obj->session_id = session_id;
	obj->file = file;
	return obj;
}

void logger_log(logger *obj, int level, char *fmt, ...) {
	if (level > LOGLEVEL_MAX)
		return;

	time_t ctime = time(NULL);
	struct tm *tm = gmtime(&ctime);

	FILE *file = obj->file ? obj->file : stdout;
	if (obj->session_id > -1) {
		fprintf(file, "%02d/%02d/%d %02d:%02d:%02d S%d[%s] ", tm->tm_mday,
				tm->tm_mon + 1, tm->tm_year + 1900, tm->tm_hour, tm->tm_min,
				tm->tm_sec, obj->session_id, log_levels[level]);
	} else {
		/* No session allocated to this logger */
		fprintf(file, "%02d/%02d/%d %02d:%02d:%02d [%s] ", tm->tm_mday,
				tm->tm_mon + 1, tm->tm_year + 1900, tm->tm_hour, tm->tm_min,
				tm->tm_sec, log_levels[level]);
	}
	va_list args;
	va_start(args, fmt);
	vfprintf(file, fmt, args);
	va_end(args);
}

void logger_destroy(logger *obj) {
	free(obj);
}
