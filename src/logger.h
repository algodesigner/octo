/**
 * @file
 * Logger interface definition.
 */

/*
 * Copyright (c) 2013 Vlad Shurupov. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are not permitted.
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

#define LOGLEVEL_WARN 0
#define LOGLEVEL_INFO 1
#define LOGLEVEL_DEBUG 2
#define LOGLEVEL_MAX LOGLEVEL_DEBUG

/**
 * The debug entries using this macro will not be included in
 * the releases.
 */
#ifdef DEBUG
#define DEBUG_LOG(obj, fmt, ...) logger_log(obj, LOGLEVEL_DEBUG, fmt, ##__VA_ARGS__)
#else
#define DEBUG_LOG(obj, fmt, ...)
#endif

#define logger_warn(obj, fmt, ...) logger_log(obj, LOGLEVEL_WARN, fmt, ##__VA_ARGS__)
#define logger_info(obj, fmt, ...) logger_log(obj, LOGLEVEL_INFO, fmt, ##__VA_ARGS__)
#define logger_debug(obj, fmt, ...) logger_log(obj, LOGLEVEL_DEBUG, fmt, ##__VA_ARGS__)

/** The main logger class */
typedef struct logger_s logger;

logger *logger_create(int, FILE *);
void logger_log(logger *, int, char *fmt, ...);
void logger_destroy(logger *);

#endif
