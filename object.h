#ifndef _OBJECT_H_
#define _OBJECT_H_

/*
 * object.h - object definitions
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
#include <stddef.h>
#include <stdint.h>

/* Address */
struct _ip {
	uint32_t addr;
	uint32_t obj;
};
typedef struct _ip ip_t;

/* Object structure */
struct _obj {
	char *filename;
	uint8_t type;
	uint32_t sz;
	uint8_t *data;
};

typedef struct _obj obj_t;

/* Functions */
extern uint8_t obj_type(int fd);
extern int obj_load(obj_t *o, char *fname);
extern void obj_unload(obj_t *o);

/* Object types */
#define OBJ_UNKNOWN 0
#define OBJ_BIN_RAW 1
#define OBJ_BIN_UEX 2

#endif /* _OBJECT_H_ */
