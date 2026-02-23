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
#include "errpublisher.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define TMP_BUFFER_SIZE 1024

struct err_publisher_st {
    void *err_handler_inst;
    void (*handle_err)(void *, int, const char *);
    char *tmp_buffer;
};

err_publisher *err_publisher_new(
        void *err_handler_inst, void (*handle_err)(void *, int, const char *))
{
    if (!handle_err)
        return NULL;
    err_publisher *obj = malloc(sizeof(struct err_publisher_st));
    obj->err_handler_inst = err_handler_inst;
    obj->handle_err = handle_err;
    obj->tmp_buffer = malloc(TMP_BUFFER_SIZE);
    return obj;
}

void err_publisher_fire(err_publisher *obj, int err_code, const char *fmt, ...)
{
    if (!obj->handle_err)
        return;
    va_list args;
    va_start(args, fmt);
    vsnprintf(obj->tmp_buffer, TMP_BUFFER_SIZE, fmt, args);
    va_end(args);
    obj->handle_err(obj->err_handler_inst, err_code, obj->tmp_buffer);
}

void err_publisher_destroy(err_publisher *obj)
{
    free(obj->tmp_buffer);
    free(obj);
}
