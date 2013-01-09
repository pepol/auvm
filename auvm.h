#ifndef _AUVM_H_
#define _AUVM_H_

/*
 * auvm.h - main include file
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
#include "stack.h"
#include "intable.h"
#include "ins.h"

/* System includes */
#include <stdint.h>
#include <stddef.h>

typedef struct _ip {
	uint32_t addr;
	uint32_t obj;
} ip_t;

/* From stack.h */
typedef struct _ds ds_t;
typedef struct _cs cs_t;

typedef struct _obj {
	char *filename;
	uint8_t type;
	uint8_t *data;
} obj_t;

/* VM status structure */
typedef struct _vm {
	/* instruction pointers */
	ip_t *cip;
	ip_t *nip;
	/* stack pointers */
	ds_t *ds;
	cs_t *cs;
	/* instruction table */
	in_t *in_table;
	/* object table */
	obj_t *ctbl;
	/* FLAGS register */
	uint8_t flags;
} vm_t;

/* FLAGS definition */
#define FLAGS_COMP_LT (1 << 0)
#define FLAGS_COMP_GT (1 << 1)

/* Instruction function type:
 *  first argument is vm status pointer
 *  second is opcode
 *  third is an argument
 */
typedef int (*in_t)(vm_t *, uint8_t, uint8_t);

#endif /* _SLVM_H_ */
