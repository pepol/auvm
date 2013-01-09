/*
 * parse.c - Instruction parser
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
#include "auvm.h"

/* System includes */
#include <stdlib.h>

int parse(vm_t *vm_status)
{
	/* Every opcode is made of 2 parts: OP_NUM (1B) and OP_ARG (1B) */
	uint8_t in_num, in_arg;
	uint32_t objno, addr, tmp;
	in_t func;
	int ret;

	objno = vm_status->nip.obj;
	addr = vm_status->nip.addr;
	in_num = vm_status->ctbl[objno].data[addr];
	in_arg = vm_status->ctbl[objno].data[addr + 1];

	tmp = addr + 2;

	/* Load-specific section */
	if (in_num == IN_LOAD) {
		/* in_arg == number of bytes to push into stack */
		ret = ds_push(&(vm_status->ds), in_arg,
			(const void *)&(vm_status->ctbl[objno].data[tmp]));
		tmp += in_arg;
	}

	/* Update IPs */
	vm_status->cip = vm_status->nip;
	vm_status->nip.addr = tmp;

	if (in_num != IN_LOAD) {
		func = vm_status->in_table[in_num];
		ret = (*func)(vm_status, in_num, in_arg);
	}

#ifdef DEBUG
	ds_show(&vm_status->ds);
#endif

	return ret;
}
