#ifndef _STACK_H_
#define _STACK_H_

/*
 * stack.h - stack definitions
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

/* Local includes */
#include "object.h"

/* System includes */
#include <stdint.h>
#include <stddef.h>

/* Data stack */
struct _ds {
	uint32_t st_count;
	uint32_t st_max;
	uint8_t *st_data;
};
typedef struct _ds ds_t;

/* Call stack */
struct _cs {
	uint32_t st_count;
	uint32_t st_max;
	ip_t *st_data;
};
typedef struct _cs cs_t;

/* Manipulation functions */
extern int ds_init(ds_t *, uint32_t);
extern int ds_destroy(ds_t *);
extern int ds_push(ds_t *, uint32_t, const void *);
extern void *ds_pop(ds_t *, uint32_t);
extern void *ds_getelem(ds_t *, uint32_t, uint32_t);
extern uint32_t ds_size(ds_t *);
extern uint32_t ds_limit(ds_t *);
extern void ds_show(ds_t *);

extern int cs_init(cs_t *, uint32_t);
extern int cs_destroy(cs_t *);
extern int cs_push(cs_t *, const ip_t *);
extern ip_t *cs_pop(cs_t *);
extern ip_t *cs_getelem(cs_t *, uint32_t);
extern uint32_t cs_size(cs_t *);
extern uint32_t cs_limit(cs_t *);

#endif /* _STACK_H_ */
