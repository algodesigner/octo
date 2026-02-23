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

#include "octo/cmdlinetest.h"
#include "octo/cmdline.h"

static void check_is_opt(tester *tst)
{
    tester_assert(tst, is_opt("-o"), "check_is_opt");
    tester_assert(tst, is_opt("--version"), "check_is_opt");
    tester_assert(tst, is_opt("-"), "check_is_opt");
    tester_assert(tst, !is_opt("option"), "check_is_opt");
    tester_assert(tst, !is_opt(""), "check_is_opt");
}

static void check_get_opt(tester *tst)
{
    char *argv[] = {"myapp", "token1", "token2", "--my-option", "token3"};
    tester_assert(tst, get_opt("--my-option", 5, argv) == 3, "check_get_opt");
    tester_assert(
            tst, get_opt("--other-option", 5, argv) == -1, "check_get_opt");
    tester_assert(tst, get_opt("token1", 5, argv) == -1, "check_get_opt");
}

static void check_equal_opts(tester *tst)
{
    tester_assert(tst, equal_opts("--workspace=w5", "--workspace"),
            "check_equal_opts");
    tester_assert(
            tst, !equal_opts("--max=15", "--workspace"), "check_equal_opts");
    tester_assert(
            tst, !equal_opts("--w=w5", "--workspace"), "check_equal_opts");
}

void test_cmdline(tester *tst)
{
    tester_new_group(tst, "test_cmdline");
    check_is_opt(tst);
    check_get_opt(tst);
    check_equal_opts(tst);
}
