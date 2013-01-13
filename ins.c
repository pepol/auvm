/*
 * ins.c - instruction implementation
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
#include "auvmlib.h"

/* System includes */
#include <string.h>


/* INSTRUCTION IMPLEMENTATION */

int in_nop(vm_t *vm_status, uint8_t opcode, uint8_t arg)
{
	if (opcode != IN_NOP || vm_status == NULL)
		return 1;
	asm("nop");
	return 0;
}

int in_end(vm_t *vm_status, uint8_t opcode, uint8_t arg)
{
	if (opcode == IN_END)
		auvm_exit(vm_status, 0);
	/* will never return */
	return 1;
}

int in_debug(vm_t *vm_status, uint8_t opcode, uint8_t arg)
{
	if (arg)
		vm_status->flags |= FLAGS_DBG;
	else 
		vm_status->flags &= ~FLAGS_DBG;
	return 0;
}

int in_stdcall(vm_t *vm_status, uint8_t opcode, uint8_t arg)
{
	/* Call wrapper-function from table, based on arg -> function's ID
	 *  table[x] == function pointer for stdcall x
	 *
	 * Instruction:
	 *  load arg1_sz arg1
	 *  load arg2_sz arg2
	 *  stdcall function_with_2_args
	 *
	 * Real world example:
	 *  load 13 "Hello world!"0xa		; string
	 *  load 4 0x0000000d			; length of string
	 *  load 4 0x00000001			; output stream: 1 = stdout
	 *  stdcall 0x01			; 1 = print_str(stream, sz, str);
	 */

	func_wrap_t func;
	
	if (opcode != IN_STDCALL)
		return 1;

	func = vm_status->func_table[arg];
	if (func == NULL)
		return 2;

	return (*func)(vm_status);
}

/* Stack */
int in_stack(vm_t *vm_status, uint8_t opcode, uint8_t arg)
{
	uint8_t buf[arg];
	uint32_t get_pos;
	int ret;

	switch (opcode) {
		case IN_DUP :
			/* Duplicate */
			memcpy(buf, ds_pop(&vm_status->ds, arg), arg);
			ret = ds_push(&vm_status->ds, arg, buf);
			ret += ds_push(&vm_status->ds, arg, buf);
			break;
		case IN_GET :
			/* Get @ position */
			get_pos = *(uint32_t *)ds_pop(&vm_status->ds,
					sizeof(uint32_t));
			memcpy(buf, ds_getelem(&vm_status->ds, arg, get_pos),
					arg);
			ret = ds_push(&vm_status->ds, arg, buf);
			break;
		case IN_DROP :
			/* Drop head [sz] elements from stack */
			if (ds_pop(&vm_status->ds, arg) != NULL)
				ret = 0;
			else ret = 1;
			break;
		default :
			ret = 1;
	}

	return ret;
}

/* Artihmetical and logical */
int in_add(vm_t *vm_status, uint8_t opcode, uint8_t arg)
{
	int ret;
	uint8_t a1, b1, c1;
	uint16_t a2, b2, c2;
	uint32_t a4, b4, c4;
	int8_t as1, bs1, cs1;
	int16_t as2, bs2, cs2;
	int32_t as4, bs4, cs4;
	float fa, fb, fc;
	double da, db, dc;
	float fas, fbs, fcs;
	double das, dbs, dcs;

	switch (opcode) {
		case IN_ADD_UI :
			switch (arg) {
				case 1 :
					a1 = *(uint8_t *)ds_pop(&vm_status->ds,
							sizeof(uint8_t));
					b1 = *(uint8_t *)ds_pop(&vm_status->ds,
							sizeof(uint8_t));
					c1 = a1 + b1;
					ret = ds_push(&vm_status->ds,
							sizeof(uint8_t), &c1);
					break;
				case 2 :
					a2 = *(uint16_t *)ds_pop(&vm_status->ds,
							sizeof(uint16_t));
					b2 = *(uint16_t *)ds_pop(&vm_status->ds,
							sizeof(uint16_t));
					c2 = a2 + b2;
					ret = ds_push(&vm_status->ds,
							sizeof(uint16_t), &c2);
					break;
				case 4 :
					a4 = *(uint32_t *)ds_pop(&vm_status->ds,
							sizeof(uint32_t));
					b4 = *(uint32_t *)ds_pop(&vm_status->ds,
							sizeof(uint32_t));
					c4 = a4 + b4;
					ret = ds_push(&vm_status->ds,
							sizeof(uint32_t), &c4);
					break;
				default : ret = 1;
			}
			break;
		case IN_ADD_SI :
			switch (arg) {
				case 1 :
					as1 = *(int8_t *)ds_pop(&vm_status->ds,
							sizeof(int8_t));
					bs1 = *(int8_t *)ds_pop(&vm_status->ds,
							sizeof(int8_t));
					cs1 = as1 + bs1;
					ret = ds_push(&vm_status->ds,
							sizeof(int8_t), &cs1);
					break;
				case 2 :
					as2 = *(int16_t *)ds_pop(&vm_status->ds,
							sizeof(int16_t));
					bs2 = *(int16_t *)ds_pop(&vm_status->ds,
							sizeof(int16_t));
					cs2 = as2 + bs2;
					ret = ds_push(&vm_status->ds,
							sizeof(int16_t), &cs2);
					break;
				case 4 :
					as4 = *(int32_t *)ds_pop(&vm_status->ds,
							sizeof(int32_t));
					bs4 = *(int32_t *)ds_pop(&vm_status->ds,
							sizeof(int32_t));
					cs4 = as4 + bs4;
					ret = ds_push(&vm_status->ds,
							sizeof(int32_t), &cs4);
					break;
				default : ret = 1;
			}
			break;
		case IN_ADD_UF :
			switch (arg) {
				case AUVMF_FLOAT :
					fa = *(float *)
						ds_pop(&vm_status->ds,
						sizeof(float));
					fb = *(float *)
						ds_pop(&vm_status->ds,
						sizeof(float));
					fc = fa + fb;
					ret = ds_push(&vm_status->ds,
						sizeof(float), &fc);
					break;
				case AUVMF_DOUBLE :
					da = *(double *)
						ds_pop(&vm_status->ds,
						sizeof(double));
					db = *(double *)
						ds_pop(&vm_status->ds,
						sizeof(double));
					dc = da + db;
					ret = ds_push(&vm_status->ds,
						sizeof(double), &dc);
					break;
				default : ret = 1;
			}
			break;
		case IN_ADD_SF :
			switch (arg) {
				case AUVMF_FLOAT :
					fas = *(float *)
						ds_pop(&vm_status->ds,
						sizeof(float));
					fbs = *(float *)
						ds_pop(&vm_status->ds,
						sizeof(float));
					fcs = fas + fbs;
					ret = ds_push(&vm_status->ds,
						sizeof(float), &fcs);
					break;
				case AUVMF_DOUBLE :
					das = *(double *)
						ds_pop(&vm_status->ds,
						sizeof(double));
					dbs = *(double *)
						ds_pop(&vm_status->ds,
						sizeof(double));
					dcs = das + dbs;
					ret = ds_push(&vm_status->ds,
						sizeof(double), &dcs);
					break;
				default : ret = 1;
			}
			break;
		default : ret = 1;
	}

	return ret;
}

int in_sub(vm_t *vm_status, uint8_t opcode, uint8_t arg)
{
	int ret;
	uint8_t a1, b1, c1;
	uint16_t a2, b2, c2;
	uint32_t a4, b4, c4;
	int8_t as1, bs1, cs1;
	int16_t as2, bs2, cs2;
	int32_t as4, bs4, cs4;
	float fa, fb, fc;
	double da, db, dc;
	float fas, fbs, fcs;
	double das, dbs, dcs;

	switch (opcode) {
		case IN_SUB_UI :
			switch (arg) {
				case 1 :
					a1 = *(uint8_t *)ds_pop(&vm_status->ds,
							sizeof(uint8_t));
					b1 = *(uint8_t *)ds_pop(&vm_status->ds,
							sizeof(uint8_t));
					c1 = a1 - b1;
					ret = ds_push(&vm_status->ds,
							sizeof(uint8_t), &c1);
					break;
				case 2 :
					a2 = *(uint16_t *)ds_pop(&vm_status->ds,
							sizeof(uint16_t));
					b2 = *(uint16_t *)ds_pop(&vm_status->ds,
							sizeof(uint16_t));
					c2 = a2 - b2;
					ret = ds_push(&vm_status->ds,
							sizeof(uint16_t), &c2);
					break;
				case 4 :
					a4 = *(uint32_t *)ds_pop(&vm_status->ds,
							sizeof(uint32_t));
					b4 = *(uint32_t *)ds_pop(&vm_status->ds,
							sizeof(uint32_t));
					c4 = a4 - b4;
					ret = ds_push(&vm_status->ds,
							sizeof(uint32_t), &c4);
					break;
				default : ret = 1;
			}
			break;
		case IN_SUB_SI :
			switch (arg) {
				case 1 :
					as1 = *(int8_t *)ds_pop(&vm_status->ds,
							sizeof(int8_t));
					bs1 = *(int8_t *)ds_pop(&vm_status->ds,
							sizeof(int8_t));
					cs1 = as1 - bs1;
					ret = ds_push(&vm_status->ds,
							sizeof(int8_t), &cs1);
					break;
				case 2 :
					as2 = *(int16_t *)ds_pop(&vm_status->ds,
							sizeof(int16_t));
					bs2 = *(int16_t *)ds_pop(&vm_status->ds,
							sizeof(int16_t));
					cs2 = as2 - bs2;
					ret = ds_push(&vm_status->ds,
							sizeof(int16_t), &cs2);
					break;
				case 4 :
					as4 = *(int32_t *)ds_pop(&vm_status->ds,
							sizeof(int32_t));
					bs4 = *(int32_t *)ds_pop(&vm_status->ds,
							sizeof(int32_t));
					cs4 = as4 - bs4;
					ret = ds_push(&vm_status->ds,
							sizeof(int32_t), &cs4);
					break;
				default : ret = 1;
			}
			break;
		case IN_SUB_UF :
			switch (arg) {
				case AUVMF_FLOAT :
					fa = *(float *)
						ds_pop(&vm_status->ds,
						sizeof(float));
					fb = *(float *)
						ds_pop(&vm_status->ds,
						sizeof(float));
					fc = fa - fb;
					ret = ds_push(&vm_status->ds,
						sizeof(float), &fc);
					break;
				case AUVMF_DOUBLE :
					da = *(double *)
						ds_pop(&vm_status->ds,
						sizeof(double));
					db = *(double *)
						ds_pop(&vm_status->ds,
						sizeof(double));
					dc = da - db;
					ret = ds_push(&vm_status->ds,
						sizeof(double), &dc);
					break;
				default : ret = 1;
			}
			break;
		case IN_SUB_SF :
			switch (arg) {
				case AUVMF_FLOAT :
					fas = *(float *)
						ds_pop(&vm_status->ds,
						sizeof(float));
					fbs = *(float *)
						ds_pop(&vm_status->ds,
						sizeof(float));
					fcs = fas - fbs;
					ret = ds_push(&vm_status->ds,
						sizeof(float), &fcs);
					break;
				case AUVMF_DOUBLE :
					das = *(double *)
						ds_pop(&vm_status->ds,
						sizeof(double));
					dbs = *(double *)
						ds_pop(&vm_status->ds,
						sizeof(double));
					dcs = das - dbs;
					ret = ds_push(&vm_status->ds,
						sizeof(double), &dcs);
					break;
				default : ret = 1;
			}
			break;
		default : ret = 1;
	}

	return ret;
}


int in_mul(vm_t *vm_status, uint8_t opcode, uint8_t arg)
{
	int ret;
	uint8_t a1, b1, c1;
	uint16_t a2, b2, c2;
	uint32_t a4, b4, c4;
	int8_t as1, bs1, cs1;
	int16_t as2, bs2, cs2;
	int32_t as4, bs4, cs4;
	float fa, fb, fc;
	double da, db, dc;
	float fas, fbs, fcs;
	double das, dbs, dcs;

	switch (opcode) {
		case IN_MUL_UI :
			switch (arg) {
				case 1 :
					a1 = *(uint8_t *)ds_pop(&vm_status->ds,
							sizeof(uint8_t));
					b1 = *(uint8_t *)ds_pop(&vm_status->ds,
							sizeof(uint8_t));
					c1 = a1 * b1;
					ret = ds_push(&vm_status->ds,
							sizeof(uint8_t), &c1);
					break;
				case 2 :
					a2 = *(uint16_t *)ds_pop(&vm_status->ds,
							sizeof(uint16_t));
					b2 = *(uint16_t *)ds_pop(&vm_status->ds,
							sizeof(uint16_t));
					c2 = a2 * b2;
					ret = ds_push(&vm_status->ds,
							sizeof(uint16_t), &c2);
					break;
				case 4 :
					a4 = *(uint32_t *)ds_pop(&vm_status->ds,
							sizeof(uint32_t));
					b4 = *(uint32_t *)ds_pop(&vm_status->ds,
							sizeof(uint32_t));
					c4 = a4 * b4;
					ret = ds_push(&vm_status->ds,
							sizeof(uint32_t), &c4);
					break;
				default : ret = 1;
			}
			break;
		case IN_MUL_SI :
			switch (arg) {
				case 1 :
					as1 = *(int8_t *)ds_pop(&vm_status->ds,
							sizeof(int8_t));
					bs1 = *(int8_t *)ds_pop(&vm_status->ds,
							sizeof(int8_t));
					cs1 = as1 * bs1;
					ret = ds_push(&vm_status->ds,
							sizeof(int8_t), &cs1);
					break;
				case 2 :
					as2 = *(int16_t *)ds_pop(&vm_status->ds,
							sizeof(int16_t));
					bs2 = *(int16_t *)ds_pop(&vm_status->ds,
							sizeof(int16_t));
					cs2 = as2 * bs2;
					ret = ds_push(&vm_status->ds,
							sizeof(int16_t), &cs2);
					break;
				case 4 :
					as4 = *(int32_t *)ds_pop(&vm_status->ds,
							sizeof(int32_t));
					bs4 = *(int32_t *)ds_pop(&vm_status->ds,
							sizeof(int32_t));
					cs4 = as4 * bs4;
					ret = ds_push(&vm_status->ds,
							sizeof(int32_t), &cs4);
					break;
				default : ret = 1;
			}
			break;
		case IN_MUL_UF :
			switch (arg) {
				case AUVMF_FLOAT :
					fa = *(float *)
						ds_pop(&vm_status->ds,
						sizeof(float));
					fb = *(float *)
						ds_pop(&vm_status->ds,
						sizeof(float));
					fc = fa * fb;
					ret = ds_push(&vm_status->ds,
						sizeof(float), &fc);
					break;
				case AUVMF_DOUBLE :
					da = *(double *)
						ds_pop(&vm_status->ds,
						sizeof(double));
					db = *(double *)
						ds_pop(&vm_status->ds,
						sizeof(double));
					dc = da * db;
					ret = ds_push(&vm_status->ds,
						sizeof(double), &dc);
					break;
				default : ret = 1;
			}
			break;
		case IN_MUL_SF :
			switch (arg) {
				case AUVMF_FLOAT :
					fas = *(float *)
						ds_pop(&vm_status->ds,
						sizeof(float));
					fbs = *(float *)
						ds_pop(&vm_status->ds,
						sizeof(float));
					fcs = fas * fbs;
					ret = ds_push(&vm_status->ds,
						sizeof(float), &fcs);
					break;
				case AUVMF_DOUBLE :
					das = *(double *)
						ds_pop(&vm_status->ds,
						sizeof(double));
					dbs = *(double *)
						ds_pop(&vm_status->ds,
						sizeof(double));
					dcs = das * dbs;
					ret = ds_push(&vm_status->ds,
						sizeof(double), &dcs);
					break;
				default : ret = 1;
			}
			break;
		default : ret = 1;
	}

	return ret;
}

int in_div(vm_t *vm_status, uint8_t opcode, uint8_t arg)
{
	int ret;
	uint8_t a1, b1, c1;
	uint16_t a2, b2, c2;
	uint32_t a4, b4, c4;
	int8_t as1, bs1, cs1;
	int16_t as2, bs2, cs2;
	int32_t as4, bs4, cs4;
	float fa, fb, fc;
	double da, db, dc;
	float fas, fbs, fcs;
	double das, dbs, dcs;

	switch (opcode) {
		case IN_DIV_UI :
			switch (arg) {
				case 1 :
					a1 = *(uint8_t *)ds_pop(&vm_status->ds,
							sizeof(uint8_t));
					b1 = *(uint8_t *)ds_pop(&vm_status->ds,
							sizeof(uint8_t));
					c1 = a1 / b1;
					ret = ds_push(&vm_status->ds,
							sizeof(uint8_t), &c1);
					break;
				case 2 :
					a2 = *(uint16_t *)ds_pop(&vm_status->ds,
							sizeof(uint16_t));
					b2 = *(uint16_t *)ds_pop(&vm_status->ds,
							sizeof(uint16_t));
					c2 = a2 / b2;
					ret = ds_push(&vm_status->ds,
							sizeof(uint16_t), &c2);
					break;
				case 4 :
					a4 = *(uint32_t *)ds_pop(&vm_status->ds,
							sizeof(uint32_t));
					b4 = *(uint32_t *)ds_pop(&vm_status->ds,
							sizeof(uint32_t));
					c4 = a4 / b4;
					ret = ds_push(&vm_status->ds,
							sizeof(uint32_t), &c4);
					break;
				default : ret = 1;
			}
			break;
		case IN_DIV_SI :
			switch (arg) {
				case 1 :
					as1 = *(int8_t *)ds_pop(&vm_status->ds,
							sizeof(int8_t));
					bs1 = *(int8_t *)ds_pop(&vm_status->ds,
							sizeof(int8_t));
					cs1 = as1 / bs1;
					ret = ds_push(&vm_status->ds,
							sizeof(int8_t), &cs1);
					break;
				case 2 :
					as2 = *(int16_t *)ds_pop(&vm_status->ds,
							sizeof(int16_t));
					bs2 = *(int16_t *)ds_pop(&vm_status->ds,
							sizeof(int16_t));
					cs2 = as2 / bs2;
					ret = ds_push(&vm_status->ds,
							sizeof(int16_t), &cs2);
					break;
				case 4 :
					as4 = *(int32_t *)ds_pop(&vm_status->ds,
							sizeof(int32_t));
					bs4 = *(int32_t *)ds_pop(&vm_status->ds,
							sizeof(int32_t));
					cs4 = as4 / bs4;
					ret = ds_push(&vm_status->ds,
							sizeof(int32_t), &cs4);
					break;
				default : ret = 1;
			}
			break;
		case IN_DIV_UF :
			switch (arg) {
				case AUVMF_FLOAT :
					fa = *(float *)
						ds_pop(&vm_status->ds,
						sizeof(float));
					fb = *(float *)
						ds_pop(&vm_status->ds,
						sizeof(float));
					fc = fa / fb;
					ret = ds_push(&vm_status->ds,
						sizeof(float), &fc);
					break;
				case AUVMF_DOUBLE :
					da = *(double *)
						ds_pop(&vm_status->ds,
						sizeof(double));
					db = *(double *)
						ds_pop(&vm_status->ds,
						sizeof(double));
					dc = da / db;
					ret = ds_push(&vm_status->ds,
						sizeof(double), &dc);
					break;
				default : ret = 1;
			}
			break;
		case IN_DIV_SF :
			switch (arg) {
				case AUVMF_FLOAT :
					fas = *(float *)
						ds_pop(&vm_status->ds,
						sizeof(float));
					fbs = *(float *)
						ds_pop(&vm_status->ds,
						sizeof(float));
					fcs = fas / fbs;
					ret = ds_push(&vm_status->ds,
						sizeof(float), &fcs);
					break;
				case AUVMF_DOUBLE :
					das = *(double *)
						ds_pop(&vm_status->ds,
						sizeof(double));
					dbs = *(double *)
						ds_pop(&vm_status->ds,
						sizeof(double));
					dcs = das / dbs;
					ret = ds_push(&vm_status->ds,
						sizeof(double), &dcs);
					break;
				default : ret = 1;
			}
			break;
		default : ret = 1;
	}

	return ret;
}

int in_mod(vm_t *vm_status, uint8_t opcode, uint8_t arg)
{
	int ret;
	uint8_t a1, b1, c1;
	uint16_t a2, b2, c2;
	uint32_t a4, b4, c4;
	int8_t as1, bs1, cs1;
	int16_t as2, bs2, cs2;
	int32_t as4, bs4, cs4;

	switch (opcode) {
		case IN_ADD_UI :
			switch (arg) {
				case 1 :
					a1 = *(uint8_t *)ds_pop(&vm_status->ds,
							sizeof(uint8_t));
					b1 = *(uint8_t *)ds_pop(&vm_status->ds,
							sizeof(uint8_t));
					c1 = a1 % b1;
					ret = ds_push(&vm_status->ds,
							sizeof(uint8_t), &c1);
					break;
				case 2 :
					a2 = *(uint16_t *)ds_pop(&vm_status->ds,
							sizeof(uint16_t));
					b2 = *(uint16_t *)ds_pop(&vm_status->ds,
							sizeof(uint16_t));
					c2 = a2 % b2;
					ret = ds_push(&vm_status->ds,
							sizeof(uint16_t), &c2);
					break;
				case 4 :
					a4 = *(uint32_t *)ds_pop(&vm_status->ds,
							sizeof(uint32_t));
					b4 = *(uint32_t *)ds_pop(&vm_status->ds,
							sizeof(uint32_t));
					c4 = a4 % b4;
					ret = ds_push(&vm_status->ds,
							sizeof(uint32_t), &c4);
					break;
				default : ret = 1;
			}
			break;
		case IN_ADD_SI :
			switch (arg) {
				case 1 :
					as1 = *(int8_t *)ds_pop(&vm_status->ds,
							sizeof(int8_t));
					bs1 = *(int8_t *)ds_pop(&vm_status->ds,
							sizeof(int8_t));
					cs1 = as1 % bs1;
					ret = ds_push(&vm_status->ds,
							sizeof(int8_t), &cs1);
					break;
				case 2 :
					as2 = *(int16_t *)ds_pop(&vm_status->ds,
							sizeof(int16_t));
					bs2 = *(int16_t *)ds_pop(&vm_status->ds,
							sizeof(int16_t));
					cs2 = as2 % bs2;
					ret = ds_push(&vm_status->ds,
							sizeof(int16_t), &cs2);
					break;
				case 4 :
					as4 = *(int32_t *)ds_pop(&vm_status->ds,
							sizeof(int32_t));
					bs4 = *(int32_t *)ds_pop(&vm_status->ds,
							sizeof(int32_t));
					cs4 = as4 % bs4;
					ret = ds_push(&vm_status->ds,
							sizeof(int32_t), &cs4);
					break;
				default : ret = 1;
			}
			break;
		default : ret = 1;
	}

	return ret;
}

int in_and(vm_t *vm_status, uint8_t opcode, uint8_t arg)
{
	int ret;
	uint8_t a, b, c;

	switch (opcode) {
		case IN_AND :
			a = *(uint8_t *)ds_pop(&vm_status->ds,
						sizeof(uint8_t));
			b = *(uint8_t *)ds_pop(&vm_status->ds,
						sizeof(uint8_t));
			c = a & b;
			ret = ds_push(&vm_status->ds, sizeof(uint8_t), &c);
			break;
		case IN_AND_L :
			a = *(uint8_t *)ds_pop(&vm_status->ds,
						sizeof(uint8_t));
			b = *(uint8_t *)ds_pop(&vm_status->ds,
						sizeof(uint8_t));
			c = a && b;
			ret = ds_push(&vm_status->ds, sizeof(uint8_t), &c);
			break;
		default : ret = 1;
	}

	return ret;
}

int in_or(vm_t *vm_status, uint8_t opcode, uint8_t arg)
{
	int ret;
	uint8_t a, b, c;

	switch (opcode) {
		case IN_OR :
			a = *(uint8_t *)ds_pop(&vm_status->ds,
						sizeof(uint8_t));
			b = *(uint8_t *)ds_pop(&vm_status->ds,
						sizeof(uint8_t));
			c = a | b;
			ret = ds_push(&vm_status->ds, sizeof(uint8_t), &c);
			break;
		case IN_OR_L :
			a = *(uint8_t *)ds_pop(&vm_status->ds,
						sizeof(uint8_t));
			b = *(uint8_t *)ds_pop(&vm_status->ds,
						sizeof(uint8_t));
			c = a || b;
			ret = ds_push(&vm_status->ds, sizeof(uint8_t), &c);
			break;
		default : ret = 1;
	}

	return ret;
}

int in_xor(vm_t *vm_status, uint8_t opcode, uint8_t arg)
{
	int ret;
	uint8_t a, b, c;

	switch (opcode) {
		case IN_XOR :
			a = *(uint8_t *)ds_pop(&vm_status->ds,
						sizeof(uint8_t));
			b = *(uint8_t *)ds_pop(&vm_status->ds,
						sizeof(uint8_t));
			c = a ^ b;
			ret = ds_push(&vm_status->ds, sizeof(uint8_t), &c);
			break;
		case IN_XOR_L :
			a = *(uint8_t *)ds_pop(&vm_status->ds,
						sizeof(uint8_t));
			b = *(uint8_t *)ds_pop(&vm_status->ds,
						sizeof(uint8_t));
			c = !!a ^ !!b;
			ret = ds_push(&vm_status->ds, sizeof(uint8_t), &c);
			break;
		default : ret = 1;
	}

	return ret;
}

int in_not(vm_t *vm_status, uint8_t opcode, uint8_t arg)
{
	int ret;
	uint8_t a, b;

	switch (opcode) {
		case IN_NOT :
			a = *(uint8_t *)ds_pop(&vm_status->ds,
						sizeof(uint8_t));
			b = ~a;
			ret = ds_push(&vm_status->ds, sizeof(uint8_t), &b);
			break;
		case IN_NOT_L :
			a = *(uint8_t *)ds_pop(&vm_status->ds,
						sizeof(uint8_t));
			b = !a;
			ret = ds_push(&vm_status->ds, sizeof(uint8_t), &b);
			break;
		default : ret = 1;
	}

	return ret;
}

int in_shl(vm_t *vm_status, uint8_t opcode, uint8_t arg)
{
	int ret;
	uint8_t a, b;

	switch (opcode) {
		case IN_SHL :
			a = *(uint8_t *)ds_pop(&vm_status->ds,
						sizeof(uint8_t));
			b = a << arg;
			ret = ds_push(&vm_status->ds, sizeof(uint8_t), &b);
			break;
		case IN_ROTL :
			a = *(uint8_t *)ds_pop(&vm_status->ds,
						sizeof(uint8_t));
			b = (a << arg) | (a >> sizeof(uint8_t) * 8 - arg);
			ret = ds_push(&vm_status->ds, sizeof(uint8_t), &b);
			break;
		default : ret = 1;
	}

	return ret;
}

int in_shr(vm_t *vm_status, uint8_t opcode, uint8_t arg)
{
	int ret;
	uint8_t a, b;

	switch (opcode) {
		case IN_SHR :
			a = *(uint8_t *)ds_pop(&vm_status->ds,
						sizeof(uint8_t));
			b = a >> arg;
			ret = ds_push(&vm_status->ds, sizeof(uint8_t), &b);
			break;
		case IN_ROTR :
			a = *(uint8_t *)ds_pop(&vm_status->ds,
						sizeof(uint8_t));
			b = (a >> arg) | (a << sizeof(uint8_t) * 8 - arg);
			ret = ds_push(&vm_status->ds, sizeof(uint8_t), &b);
			break;
		default : ret = 1;
	}

	return ret;
}

/* Branching */

int in_jmp(vm_t *vm_status, uint8_t opcode, uint8_t arg)
{
	int ret = 0;
	int32_t offset;
	uint32_t addr, obj;

	if ((opcode == IN_CALL) || (opcode == IN_CALL_L))
		/* save NIP */
		ret += cs_push(&vm_status->cs, &vm_status->nip);

	switch (opcode) {
		/* Object-wise jumps / calls */
		case IN_JMP :
		case IN_CALL :
			switch (arg) {
				case JMP_REL :
					/* Relative jump */
					offset = *(int32_t *)
						 ds_pop(&vm_status->ds,
							sizeof(int32_t));
					vm_status->nip.addr += offset;
					break;
				case JMP_ABS :
					/* Absolute jump */
					addr = *(uint32_t *)
						 ds_pop(&vm_status->ds,
							sizeof(uint32_t));
					vm_status->nip.addr = addr;
					break;
				default : ret++;
			}
			break;
		/* Long jumps / calls (changing object) */
		case IN_JMP_L :
		case IN_CALL_L :
			addr = *(uint32_t *)ds_pop(&vm_status->ds,
						sizeof(uint32_t));
			obj = *(uint32_t *)ds_pop(&vm_status->ds,
						sizeof(uint32_t));
			if (vm_status->obj_count <= obj)
				/* Illegal object */
				ret++;
			else {
				/* Update NIP */
				vm_status->nip.addr = addr;
				vm_status->nip.obj = obj;
			}
			break;
		default : ret++;
	}

	return ret;
}

int in_ret(vm_t *vm_status, uint8_t opcode, uint8_t arg)
{
	ip_t *tmp;

	/* Argument contains number of levels to return from */
	while (arg--) {
		tmp = cs_pop(&vm_status->cs);
	}
	if (tmp == NULL)
		return 1;

	vm_status->nip.addr = tmp->addr;
	vm_status->nip.obj = tmp->obj;

	return 0;
}

/* Conditionals */

int in_cmp(vm_t *vm_status, uint8_t opcode, uint8_t arg)
{
	int ret = 0;
	uint8_t a, b;
	int8_t as, bs;
	float fa, fb;
	double da, db;
	
	/* discard previous comparison results */
	vm_status->flags = (vm_status->flags >> 2) << 2;

	switch (arg) {
		case AUVMF_UINT :
			a = *(uint8_t *)ds_pop(&vm_status->ds,
						sizeof(uint8_t));
			b = *(uint8_t *)ds_pop(&vm_status->ds,
						sizeof(uint8_t));
			vm_status->flags += (a < b) ? FLAGS_COMP_LT : 
					((a > b) ? FLAGS_COMP_GT : 0);
			break;
		case AUVMF_SINT :
			as = *(int8_t *)ds_pop(&vm_status->ds,
						sizeof(int8_t));
			bs = *(int8_t *)ds_pop(&vm_status->ds,
						sizeof(int8_t));
			vm_status->flags += (as < bs) ? FLAGS_COMP_LT : 
					((as > bs) ? FLAGS_COMP_GT : 0);
			break;
		case AUVMF_FLOAT :
			fa = *(float *)ds_pop(&vm_status->ds,
						sizeof(float));
			fb = *(float *)ds_pop(&vm_status->ds,
						sizeof(float));
			vm_status->flags += (fa < fb) ? FLAGS_COMP_LT : 
					((fa > fb) ? FLAGS_COMP_GT : 0);
			break;
		case AUVMF_DOUBLE :
			da = *(double *)ds_pop(&vm_status->ds,
						sizeof(double));
			db = *(double *)ds_pop(&vm_status->ds,
						sizeof(double));
			vm_status->flags += (da < db) ? FLAGS_COMP_LT : 
					((da > db) ? FLAGS_COMP_GT : 0);
			break;
		default : ret = 1;
	}
	return ret;
}

int in_if(vm_t *vm_status, uint8_t opcode, uint8_t arg)
{
	uint8_t skip = 1;
	
	switch (opcode) {
		case IN_IFEQ :
			if (!((vm_status->flags & FLAGS_COMP_GT)
				|| (vm_status->flags & FLAGS_COMP_LT)))
				skip = 0;
			break;
		case IN_IFNEQ :
			if ((vm_status->flags & FLAGS_COMP_GT)
				|| (vm_status->flags & FLAGS_COMP_LT))
				skip = 0;
			break;
		case IN_IFGT :
			if (vm_status->flags & FLAGS_COMP_GT)
				skip = 0;
			break;
		case IN_IFGE :
			if (vm_status->flags & FLAGS_COMP_GT)
				skip = 0;
			else if (!(vm_status->flags & FLAGS_COMP_LT))
				skip = 0;
			break;
		case IN_IFLT :
			if (vm_status->flags & FLAGS_COMP_LT)
				skip = 0;
			break;
		case IN_IFLE :
			if (vm_status->flags & FLAGS_COMP_LT)
				skip = 0;
			else if (!(vm_status->flags & FLAGS_COMP_GT))
				skip = 0;
			break;
		default : return 1;
	}

	/* If skip != 0, move nip to new position (+2) */
	if (skip)
		vm_status->nip.addr += 2;

	return 0;
}
