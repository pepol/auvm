/*
 * object.c - Object manipulation functions
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
#include "auvm.h"
#include "object.h"

/* System includes */
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* Get object type 
 * FIXME: Determines *every* file as raw binary
 */
uint8_t obj_type((void)(int fd))
{
	return OBJ_BIN_RAW;
}

/* Load object */
int obj_load(obj_t *o, char *fname)
{
	int fd;
	struct stat sbuf;
	uint32_t fsize;
	uint8_t ftype;
	ssize_t ret;

	/* Operations:
	 *  1. open file
	 *  2. get file type
	 *  3. parse it based on type
	 */
	fd = open(fname, O_RDONLY);
	if (fd == -1)
		return 1;
	ftype = obj_type(fd);
	if (fstat(fd, &sbuf) == -1)
		return 2;
	/* get file size */
	fsize = (uint32_t) sbuf.st_size;

	switch (ftype) {
		case OBJ_BIN_RAW :
			/* Object is raw binary, only supported format now:
			 *  1. Allocate memory
			 *  2. Load contents into memory
			 *  3. Return
			 */
			o->data = (uint8_t *)malloc(fsize);
			if (o->data == NULL)
				return 3;
			ret = read(fd, o->data, fsize);
			o->sz = (uint32_t) ret;
			break;
		case OBJ_BIN_UEX :
			/* Native executable format */
			/* FIXME: implement
			 * return parse_uex(fd, o);*/
			break;
		default :
			return 4;
	}

	o->type = ftype;
	o->filename = fname;
	close(fd);

	return 0;
}

/* Unload object */
void obj_unload(obj_t *o)
{
	free(o->data);
	o->type = 0;
	o->sz = 0;
	o->filename = NULL;
}
