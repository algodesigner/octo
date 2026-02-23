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
 */
#include "octo/logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct logger_s {
    int session_id;
    FILE *file;
};

const char *log_levels[] = {"WARNING", "INFO", "DEBUG"};

logger *logger_create(int session_id, FILE *file)
{
    logger *obj = (logger *)malloc(sizeof(logger));
    if (obj == NULL)
        return NULL;
    if (session_id < -1)
        return NULL;
    obj->session_id = session_id;
    obj->file = file;
    return obj;
}

void logger_log(logger *obj, int level, char *fmt, ...)
{
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

void logger_destroy(logger *obj)
{
    free(obj);
}
