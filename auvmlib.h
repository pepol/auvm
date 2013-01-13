#ifndef _AUVMLIB_H_
#define _AUVMLIB_H_

/*
 * auvmlib.h - AUVM standard library definitions
 *
 * Copyright (c) 2013 Peter Polacik <polacik.p@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/* Config file */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* System includes */
#include <stdint.h>
#include <stddef.h>

#include "auvm.h"

/* Function wrapper prototype */
typedef int (*func_wrap_t)(struct _vm *);

/* Macro definitions for easier implementation */
#define AULIB_BEGIN(X) int wrapper_##X##(vm_t *vm_status) \
{
#define AULIB_END }
#define AULIB_ADD(T, F, N) T##[##N##] = &wrapper_##F
#define AULIB_DECL(F) = extern wrapper_##F(vm_t *vm_status)

/* Externs - auvmlib.c */
extern func_wrap_t *func_table_init(void);
extern void func_table_destroy(func_wrap_t *func_tbl);

/* Externs - AUVM Library functions */

/* io.c */
AULIB_DECL(print_str);
AULIB_DECL(print_int);
AULIB_DECL(print_uint);
AULIB_DECL(print_float);
AULIB_DECL(print_double);

#endif /* _AUVMLIB_H_ */
