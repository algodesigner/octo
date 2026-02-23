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

#ifndef PROC_H_
#define PROC_H_

#include "config.h"
#include "logger.h"
#include "stdbool.h"

typedef struct proc_st proc;

enum action { UNKNOWN, PULL, PUSH, CHECKOUT, CLONE, STATUS, LIST, EXEC, PATH };

/*
 * Indicates if Git DCVS is installed on this system.
 */
bool proc_is_git_installed();

/*
 * Constructs this class.
 */
proc *proc_new(logger *, config *);

/*
 * Sets the error handler for this class.
 */
void proc_set_err_handler(proc *, void *, void (*)(void *, int, const char *));

/*
 * Initialises this object off the specified command line.
 */
bool proc_parse_cmd_line(proc *, int, char *[]);

/*
 * Takes action at the specified location.
 */
void proc_action(proc *, const char *, const char *);

/*
 * Takes a single non-repetitive action if one is assigned.
 */
void proc_single_action(
        proc *, void *, int (*)(void *, const char *, char *, int));

/*
 * Returns the currently assigned action.
 */
enum action proc_get_action(proc *);

/*
 * Indicates the if the assigned action is repetitive.
 */
bool proc_is_repetitive(proc *);

/*
 * Returns the last error message or NULL if the last operation was successful.
 */
const char *proc_get_error_message(proc *);

/*
 * Indicates if the application <u>outside of</u> processor is expected
 * top operate in silent mode.
 */
bool proc_is_silent(proc *);

/*
 * Destructs the specified instance of processor.
 */
void proc_destroy(proc *);

#endif /* PROC_H_ */
