#ifndef _AUVM_H_
#define _AUVM_H_
/*
 * stack.h - stack header file
 *
 * Copyright (c) 2012 Peter Polacik <polacik.p@gmail.com>
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

#ifndef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stack.h>

#include <stdint.h>

/* stack.c */
extern int ds_init(ds_t *s, uint32_t sz);
extern int ds_destroy(ds_t *s);
extern int ds_push(ds_t *s, uint32_t sz, const void *ptr);
extern int ds_pop(ds_t *s, uint32_t sz, void *ptr);
extern int ds_getelem(ds_t *s, uint32_t sz, uint32_t pos, void *ptr);
extern uint32_t ds_size(ds_t *s);
extern uint32_t ds_limit(ds_t *s);

#endif /* _AUVM_ */
