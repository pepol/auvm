/*
 * intable.c - instruction table implementation
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
#include "ins.h"
#include "intable.h"

/* System includes */
#include <stdlib.h>
#include <stdio.h>


int in_undefined((void)(vm_t *vm_status), uint8_t opcode, (void)(uint8_t arg))
{
	fprintf(stderr, "E: Instruction %u is undefined\n", opcode);
	return 0;
}

in_t *in_table_init(void)
{
	/* Return 256-item array of all instructions - 00 to ff */
	in_t *ret;
	int i;

	ret = (in_t *)malloc(sizeof(in_t) * 256);
	if (ret == NULL)
		return NULL;

	/* Initialize to 'undefined' values */
	for (i = 0; i < 256; i++)
		ret[i] = &in_undefined;

	/* Actual instructions */
	ret[IN_NOP] = &in_nop;
	ret[IN_END] = &in_end;
	ret[IN_DEBUG] = &in_debug;
	ret[IN_STDCALL] = &in_stdcall;

	ret[IN_LOAD] = NULL; /* special case handled by parse function */

	/* one function can behave as more instructions -> opcode parameter */
	ret[IN_DUP] = &in_stack;
	ret[IN_GET] = &in_stack;
	ret[IN_DROP] = &in_stack;

	/* arithmetical and logical */
	ret[IN_ADD_UI] = &in_add;
	ret[IN_ADD_SI] = &in_add;
	ret[IN_ADD_UF] = &in_add;
	ret[IN_ADD_SF] = &in_add;

	ret[IN_SUB_UI] = &in_sub;
	ret[IN_SUB_SI] = &in_sub;
	ret[IN_SUB_UF] = &in_sub;
	ret[IN_SUB_SF] = &in_sub;

	ret[IN_MUL_UI] = &in_mul;
	ret[IN_MUL_SI] = &in_mul;
	ret[IN_MUL_UF] = &in_mul;
	ret[IN_MUL_SF] = &in_mul;

	ret[IN_DIV_UI] = &in_div;
	ret[IN_DIV_SI] = &in_div;
	ret[IN_DIV_UF] = &in_div;
	ret[IN_DIV_SF] = &in_div;

	ret[IN_MOD_UI] = &in_mod;
	ret[IN_MOD_SI] = &in_mod;

	ret[IN_AND] = &in_and;
	ret[IN_AND_L] = &in_and;

	ret[IN_OR] = &in_or;
	ret[IN_OR_L] = &in_or;

	ret[IN_XOR] = &in_xor;
	ret[IN_XOR_L] = &in_xor;

	ret[IN_NOT] = &in_not;
	ret[IN_NOT_L] = &in_not;

	ret[IN_SHL] = &in_shl;
	ret[IN_SHR] = &in_shr;

	ret[IN_ROTL] = &in_shl;
	ret[IN_ROTR] = &in_shr;

	/* branching */
	ret[IN_JMP] = &in_jmp;
	ret[IN_JMP_L] = &in_jmp;
	ret[IN_CALL] = &in_jmp;
	ret[IN_CALL_L] = &in_jmp;
	ret[IN_RET] = &in_ret;

	/* conditionals */
	ret[IN_CMP] = &in_cmp;
	ret[IN_IFEQ] = &in_if;
	ret[IN_IFNEQ] = &in_if;
	ret[IN_IFGT] = &in_if;
	ret[IN_IFGE] = &in_if;
	ret[IN_IFLT] = &in_if;
	ret[IN_IFLE] = &in_if;

	return ret;
}

void in_table_destroy(in_t *in_tbl)
{
	free(in_tbl);
}
