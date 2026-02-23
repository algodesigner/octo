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
 * xsystem.c
 */
#define _BSD_SOURCE

#include "octo/xsystem.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#if !defined(pipe) && defined(__MINGW32__)
#define pipe(fds) _pipe(fds, 8192, 0)
#endif

struct char_buffer *char_buffer_new(int length)
{
    char *buffer = malloc(length);
    if (!buffer)
        return NULL;
    struct char_buffer *obj = malloc(sizeof(struct char_buffer));
    obj->buffer = buffer;
    obj->capacity = length;
    char_buffer_reset(obj);
    return obj;
}

int char_buffer_len(struct char_buffer *obj)
{
    return obj->limit - obj->position;
}

void char_buffer_reset(struct char_buffer *obj)
{
    obj->position = 0;
    obj->limit = obj->capacity;
}

void char_buffer_destroy(struct char_buffer *obj)
{
    free(obj->buffer);
    free(obj);
}

void char_buffer_print(struct char_buffer *obj)
{
    int i;
    for (i = obj->position; i < obj->limit; i++) {
        char c = obj->buffer[i];
        putchar(c);
        /* putchar(c >= 0x20 && c < 0x7F ? c : '.'); */
    }
}

int xsystem(const char *cmd, struct char_buffer *dst, bool verbose)
{
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
