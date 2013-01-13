/*
 * init.c - Initialization functions of AUVM
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
#include "stack.h"
#include "object.h"
#include "ins.h"

/* System includes */
#include <stdlib.h>

vm_t *auvm_init(uint32_t ds_sz, uint32_t cs_sz,	int argc, char **argv)
{
	int i;
	vm_t *ret;

	/* Allocate VM status struct */
	ret = (vm_t *)malloc(sizeof(vm_t));
	if (ret == NULL)
		return NULL;

	/* Initialize instruction pointers */
	ret->cip.addr = 0;
	ret->cip.obj = 0;
	ret->nip.addr = 0;
	ret->nip.obj = 0;
	
	/* Initialize stacks */
	if (ds_init(&(ret->ds), ds_sz) != 0) {
		free(ret);
		return NULL;
	}

	if (cs_init(&(ret->cs), cs_sz) != 0) {
		ds_destroy(&ret->ds);
		free(ret);
		return NULL;
	}

	/* Load instruction table */
	ret->in_table = in_table_init();
	if (ret->in_table == NULL) {
		ds_destroy(&ret->ds);
		cs_destroy(&ret->cs);
		free(ret);
		return NULL;
	}

	/* Load AUVM Library */
	ret->func_table = func_table_init();
	if (ret->func_table == NULL) {
		ds_destroy(&ret->ds);
		cs_destroy(&ret->cs);
		in_table_destroy(ret->in_table);
		free(ret);
	}

	/* Load object table:
	 *  argc contains count of (char *) elements in argv
	 *  argv is array of strings containing objects
	 *
	 *  1. Allocate memory for object table
	 *  2. Iterate through objects and initialize them
	 */

	ret->obj_count = 0;
	ret->ctbl = (obj_t *)malloc(sizeof(obj_t) * argc);
	if (ret->ctbl == NULL) {
		ds_destroy(&ret->ds);
		cs_destroy(&ret->cs);
		in_table_destroy(ret->in_table);
		func_table_destroy(ret->func_table);
		free(ret);
		return NULL;
	}

	for (i = 0; i < argc; i++) {
		/* 
		 * This function opens file, reads its contents, parses them
		 * and set obj_t structure to correct values
		 */
		if (obj_load(&(ret->ctbl[i]), argv[i]) != 0) {
			ds_destroy(&ret->ds);
			cs_destroy(&ret->cs);
			in_table_destroy(ret->in_table);
			func_table_destroy(ret->func_table);
			for (int j = 0; j < i; j++)
				obj_unload(&(ret->ctbl[j]));
			free(ret->ctbl);
			free(ret);
			return NULL;
		}
		ret->obj_count++;
	}

	ret->flags = 0;
#ifdef DEBUG
	/* Set flags to debug */
	ret->flags |= FLAGS_DBG;
#endif

	return ret;
}
