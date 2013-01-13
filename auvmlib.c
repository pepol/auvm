/*
 * auvmlib.c - AUVM Library implementation
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
#include "auvmlib.h"

/* System includes */

func_wrap_t *func_table_init(void)
{
	func_wrap_t *ret;
	int i;

	ret = (func_wrap_t *)malloc(sizeof(func_wrap_t) * 256);
	if (ret == NULL)
		return NULL;

	/* Initialize to 'undefined' values */
	for (i = 0; i < 256; i++)
		ret[i] = NULL;

	AULIB_ADD(ret, print_str, 1);
	AULIB_ADD(ret, print_int, 2);
	AULIB_ADD(ret, print_uint, 3);
	AULIB_ADD(ret, print_float, 4);
	AULIB_ADD(ret, print_double, 5);

	return ret;
}

void func_table_destroy(func_wrap_t *func_tbl)
{
	free(func_tbl);
}
