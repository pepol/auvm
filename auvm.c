/*
 * auvm.c - main file (starting point) of AUVM
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

/* System includes */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

/* 1kB data stack default size */
#define DS_SIZE_DEFAULT 1024
/* default call-depth (size of CS): 256 */
#define CS_SIZE_DEFAULT 256

void usage(const char *progname, int ec, FILE *s)
{
	fprintf(s, 
		"Usage: %s [-h] [-d SIZE] [-c SIZE] file1 [file2 .. fileN]\n",
		progname);
	fprintf(s, "\n\t-h\tShow this text.");
	fprintf(s, "\n\t-d SIZE\tSet data stack size to SIZE.");
	fprintf(s, "\n\t-c SIZE\tSet code stack size to SIZE.\n");
	exit(ec);
}

void auvm_exit(vm_t *vm_status, int ec)
{
	int i;
	if (vm_status->flags & FLAGS_DBG)
		ds_show(&vm_status->ds);
	ds_destroy(&vm_status->ds);
	cs_destroy(&vm_status->cs);
	in_table_destroy(vm_status->in_table);
	for (i = 0; i < vm_status->obj_count; i++)
		obj_unload(&(vm_status->ctbl[i]));
	free(vm_status->ctbl);
	free(vm_status);

	exit(ec);
}

int main(int argc, char **argv)
{
	int opt, filecount;
	char **filearr;
	uint32_t cs_size, ds_size;
	vm_t *vmst;

	cs_size = CS_SIZE_DEFAULT;
	ds_size = DS_SIZE_DEFAULT;

	while ((opt = getopt(argc, argv, "hd:c:")) != -1) {
		switch (opt) {
			case 'h' :
				usage(argv[0], 0, stdout);
				break;
			case 'd' :
				sscanf(optarg, "%lu", &ds_size);
#ifdef DEBUG
				fprintf(stderr, "[DEBUG] ds_size = %lu\n",
						ds_size);
#endif
				break;
			case 'c' :
				sscanf(optarg, "%lu", &cs_size);
#ifdef DEBUG
				fprintf(stderr, "[DEBUG] cs_size = %lu\n",
						cs_size);
#endif
				break;
			default :
				usage(argv[0], 1, stderr);
		}
	}

	if (optind >= argc)
		usage(argv[0], 2, stderr);

	filecount = argc - optind;
	filearr = &argv[optind];

	vmst = auvm_init(ds_size, cs_size, filecount, filearr);
	if (vmst == NULL)
		exit(3);

	while (!parse(vmst));

	auvm_exit(vmst, 4);
	return 5; /* This shouldn't happen, so there must be an error */
}
