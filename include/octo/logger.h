/*
 * Copyright (c) 2018-2026, Vlad Shurupov
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @file
 * Logger interface definition.
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
#define DEBUG_LOG(obj, fmt, ...) \
    logger_log(obj, LOGLEVEL_DEBUG, fmt, ##__VA_ARGS__)
#else
#define DEBUG_LOG(obj, fmt, ...)
#endif

#define logger_warn(obj, fmt, ...) \
    logger_log(obj, LOGLEVEL_WARN, fmt, ##__VA_ARGS__)
#define logger_info(obj, fmt, ...) \
    logger_log(obj, LOGLEVEL_INFO, fmt, ##__VA_ARGS__)
#define logger_debug(obj, fmt, ...) \
    logger_log(obj, LOGLEVEL_DEBUG, fmt, ##__VA_ARGS__)

/** The main logger class */
typedef struct logger_s logger;

logger *logger_create(int, FILE *);
void logger_log(logger *, int, char *fmt, ...);
void logger_destroy(logger *);

#endif
