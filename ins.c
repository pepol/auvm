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

/* System includes */


/* INSTRUCTION IMPLEMENTATION */

int in_nop((void)(vm_t *vm_status), (void)(uint8_t opcode), (void)(uint8_t arg))
{
	asm("nop");
	return 0;
}

int in_end(vm_t *vm_status, (void)(uint8_t opcode), (void)(uint8_t arg))
{
	auvm_exit(vm_status, 0);
	/* will never return */
	return 0;
}

int in_debug(vm_t *vm_status, (void)(uint8_t opcode), uint8_t arg)
{
	/* TODO: Implement */
	return 0;
}

int in_stdcall(vm_t *vm_status, (void)(uint8_t opcode), uint8_t arg)
{
	/* TODO: Implement */
	return 0;
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
			memcpy(buf, ds_getelem(&vm_status->ds, arg, get_pos));
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

	switch (opcode) {
		case IN_ADD_UI :
			switch (arg) {
				case 1 :
					uint8_t a, b, c;
					a = *(uint8_t *)ds_pop(&vm_status->ds,
							sizeof(uint8_t));
					b = *(uint8_t *)ds_pop(&vm_status->ds,
							sizeof(uint8_t));
					c = a + b;
					ret = ds_push(&vm_status->ds,
							sizeof(uint8_t), &c);
					break;
				case 2 :
					uint16_t a, b, c;
					a = *(uint16_t *)ds_pop(&vm_status->ds,
							sizeof(uint16_t));
					b = *(uint16_t *)ds_pop(&vm_status->ds,
							sizeof(uint16_t));
					c = a + b;
					ret = ds_push(&vm_status->ds,
							sizeof(uint16_t), &c);
					break;
				case 4 :
					uint32_t a, b, c;
					a = *(uint32_t *)ds_pop(&vm_status->ds,
							sizeof(uint32_t));
					b = *(uint32_t *)ds_pop(&vm_status->ds,
							sizeof(uint32_t));
					c = a + b;
					ret = ds_push(&vm_status->ds,
							sizeof(uint32_t), &c);
					break;
				default : ret = 1;
			}
			break;
		case IN_ADD_SI :
			switch (arg) {
				case 1 :
					int8_t a, b, c;
					a = *(int8_t *)ds_pop(&vm_status->ds,
							sizeof(int8_t));
					b = *(int8_t *)ds_pop(&vm_status->ds,
							sizeof(int8_t));
					c = a + b;
					ret = ds_push(&vm_status->ds,
							sizeof(int8_t), &c);
					break;
				case 2 :
					int16_t a, b, c;
					a = *(int16_t *)ds_pop(&vm_status->ds,
							sizeof(int16_t));
					b = *(int16_t *)ds_pop(&vm_status->ds,
							sizeof(int16_t));
					c = a + b;
					ret = ds_push(&vm_status->ds,
							sizeof(int16_t), &c);
					break;
				case 4 :
					int32_t a, b, c;
					a = *(int32_t *)ds_pop(&vm_status->ds,
							sizeof(int32_t));
					b = *(int32_t *)ds_pop(&vm_status->ds,
							sizeof(int32_t));
					c = a + b;
					ret = ds_push(&vm_status->ds,
							sizeof(int32_t), &c);
					break;
				default : ret = 1;
			}
			break;
		case IN_ADD_UF :
			switch (arg) {
				case AUVMF_FLOAT :
					unsigned float a, b, c;
					a = *(unsigned float *)
						ds_pop(&vm_status->ds,
						sizeof(unsigned float));
					b = *(unsigned float *)
						ds_pop(&vm_status->ds,
						sizeof(unsigned float));
					c = a + b;
					ret = ds_push(&vm_status->ds,
						sizeof(unsigned float), &c);
					break;
				case AUVMF_DOUBLE :
					unsigned double a, b, c;
					a = *(unsigned double *)
						ds_pop(&vm_status->ds,
						sizeof(unsigned double));
					b = *(unsigned double *)
						ds_pop(&vm_status->ds,
						sizeof(unsigned double));
					c = a + b;
					ret = ds_push(&vm_status->ds,
						sizeof(unsigned double), &c);
					break;
				default : ret = 1;
			}
			break;
		case IN_ADD_SF :
			switch (arg) {
				case AUVMF_FLOAT :
					signed float a, b, c;
					a = *(signed float *)
						ds_pop(&vm_status->ds,
						sizeof(signed float));
					b = *(signed float *)
						ds_pop(&vm_status->ds,
						sizeof(signed float));
					c = a + b;
					ret = ds_push(&vm_status->ds,
						sizeof(signed float), &c);
					break;
				case AUVMF_DOUBLE :
					signed double a, b, c;
					a = *(signed double *)
						ds_pop(&vm_status->ds,
						sizeof(signed double));
					b = *(signed double *)
						ds_pop(&vm_status->ds,
						sizeof(signed double));
					c = a + b;
					ret = ds_push(&vm_status->ds,
						sizeof(signed double), &c);
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

	switch (opcode) {
		case IN_SUB_UI :
			switch (arg) {
				case 1 :
					uint8_t a, b, c;
					a = *(uint8_t *)ds_pop(&vm_status->ds,
							sizeof(uint8_t));
					b = *(uint8_t *)ds_pop(&vm_status->ds,
							sizeof(uint8_t));
					c = a - b;
					ret = ds_push(&vm_status->ds,
							sizeof(uint8_t), &c);
					break;
				case 2 :
					uint16_t a, b, c;
					a = *(uint16_t *)ds_pop(&vm_status->ds,
							sizeof(uint16_t));
					b = *(uint16_t *)ds_pop(&vm_status->ds,
							sizeof(uint16_t));
					c = a - b;
					ret = ds_push(&vm_status->ds,
							sizeof(uint16_t), &c);
					break;
				case 4 :
					uint32_t a, b, c;
					a = *(uint32_t *)ds_pop(&vm_status->ds,
							sizeof(uint32_t));
					b = *(uint32_t *)ds_pop(&vm_status->ds,
							sizeof(uint32_t));
					c = a - b;
					ret = ds_push(&vm_status->ds,
							sizeof(uint32_t), &c);
					break;
				default : ret = 1;
			}
			break;
		case IN_SUB_SI :
			switch (arg) {
				case 1 :
					int8_t a, b, c;
					a = *(int8_t *)ds_pop(&vm_status->ds,
							sizeof(int8_t));
					b = *(int8_t *)ds_pop(&vm_status->ds,
							sizeof(int8_t));
					c = a - b;
					ret = ds_push(&vm_status->ds,
							sizeof(int8_t), &c);
					break;
				case 2 :
					int16_t a, b, c;
					a = *(int16_t *)ds_pop(&vm_status->ds,
							sizeof(int16_t));
					b = *(int16_t *)ds_pop(&vm_status->ds,
							sizeof(int16_t));
					c = a - b;
					ret = ds_push(&vm_status->ds,
							sizeof(int16_t), &c);
					break;
				case 4 :
					int32_t a, b, c;
					a = *(int32_t *)ds_pop(&vm_status->ds,
							sizeof(int32_t));
					b = *(int32_t *)ds_pop(&vm_status->ds,
							sizeof(int32_t));
					c = a - b;
					ret = ds_push(&vm_status->ds,
							sizeof(int32_t), &c);
					break;
				default : ret = 1;
			}
			break;
		case IN_SUB_UF :
			switch (arg) {
				case AUVMF_FLOAT :
					unsigned float a, b, c;
					a = *(unsigned float *)
						ds_pop(&vm_status->ds,
						sizeof(unsigned float));
					b = *(unsigned float *)
						ds_pop(&vm_status->ds,
						sizeof(unsigned float));
					c = a - b;
					ret = ds_push(&vm_status->ds,
						sizeof(unsigned float), &c);
					break;
				case AUVMF_DOUBLE :
					unsigned double a, b, c;
					a = *(unsigned double *)
						ds_pop(&vm_status->ds,
						sizeof(unsigned double));
					b = *(unsigned double *)
						ds_pop(&vm_status->ds,
						sizeof(unsigned double));
					c = a - b;
					ret = ds_push(&vm_status->ds,
						sizeof(unsigned double), &c);
					break;
				default : ret = 1;
			}
			break;
		case IN_SUB_SF :
			switch (arg) {
				case AUVMF_FLOAT :
					signed float a, b, c;
					a = *(signed float *)
						ds_pop(&vm_status->ds,
						sizeof(signed float));
					b = *(signed float *)
						ds_pop(&vm_status->ds,
						sizeof(signed float));
					c = a - b;
					ret = ds_push(&vm_status->ds,
						sizeof(signed float), &c);
					break;
				case AUVMF_DOUBLE :
					signed double a, b, c;
					a = *(signed double *)
						ds_pop(&vm_status->ds,
						sizeof(signed double));
					b = *(signed double *)
						ds_pop(&vm_status->ds,
						sizeof(signed double));
					c = a - b;
					ret = ds_push(&vm_status->ds,
						sizeof(signed double), &c);
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

	switch (opcode) {
		case IN_MUL_UI :
			switch (arg) {
				case 1 :
					uint8_t a, b, c;
					a = *(uint8_t *)ds_pop(&vm_status->ds,
							sizeof(uint8_t));
					b = *(uint8_t *)ds_pop(&vm_status->ds,
							sizeof(uint8_t));
					c = a * b;
					ret = ds_push(&vm_status->ds,
							sizeof(uint8_t), &c);
					break;
				case 2 :
					uint16_t a, b, c;
					a = *(uint16_t *)ds_pop(&vm_status->ds,
							sizeof(uint16_t));
					b = *(uint16_t *)ds_pop(&vm_status->ds,
							sizeof(uint16_t));
					c = a * b;
					ret = ds_push(&vm_status->ds,
							sizeof(uint16_t), &c);
					break;
				case 4 :
					uint32_t a, b, c;
					a = *(uint32_t *)ds_pop(&vm_status->ds,
							sizeof(uint32_t));
					b = *(uint32_t *)ds_pop(&vm_status->ds,
							sizeof(uint32_t));
					c = a * b;
					ret = ds_push(&vm_status->ds,
							sizeof(uint32_t), &c);
					break;
				default : ret = 1;
			}
			break;
		case IN_MUL_SI :
			switch (arg) {
				case 1 :
					int8_t a, b, c;
					a = *(int8_t *)ds_pop(&vm_status->ds,
							sizeof(int8_t));
					b = *(int8_t *)ds_pop(&vm_status->ds,
							sizeof(int8_t));
					c = a * b;
					ret = ds_push(&vm_status->ds,
							sizeof(int8_t), &c);
					break;
				case 2 :
					int16_t a, b, c;
					a = *(int16_t *)ds_pop(&vm_status->ds,
							sizeof(int16_t));
					b = *(int16_t *)ds_pop(&vm_status->ds,
							sizeof(int16_t));
					c = a * b;
					ret = ds_push(&vm_status->ds,
							sizeof(int16_t), &c);
					break;
				case 4 :
					int32_t a, b, c;
					a = *(int32_t *)ds_pop(&vm_status->ds,
							sizeof(int32_t));
					b = *(int32_t *)ds_pop(&vm_status->ds,
							sizeof(int32_t));
					c = a * b;
					ret = ds_push(&vm_status->ds,
							sizeof(int32_t), &c);
					break;
				default : ret = 1;
			}
			break;
		case IN_MUL_UF :
			switch (arg) {
				case AUVMF_FLOAT :
					unsigned float a, b, c;
					a = *(unsigned float *)
						ds_pop(&vm_status->ds,
						sizeof(unsigned float));
					b = *(unsigned float *)
						ds_pop(&vm_status->ds,
						sizeof(unsigned float));
					c = a * b;
					ret = ds_push(&vm_status->ds,
						sizeof(unsigned float), &c);
					break;
				case AUVMF_DOUBLE :
					unsigned double a, b, c;
					a = *(unsigned double *)
						ds_pop(&vm_status->ds,
						sizeof(unsigned double));
					b = *(unsigned double *)
						ds_pop(&vm_status->ds,
						sizeof(unsigned double));
					c = a * b;
					ret = ds_push(&vm_status->ds,
						sizeof(unsigned double), &c);
					break;
				default : ret = 1;
			}
			break;
		case IN_MUL_SF :
			switch (arg) {
				case AUVMF_FLOAT :
					signed float a, b, c;
					a = *(signed float *)
						ds_pop(&vm_status->ds,
						sizeof(signed float));
					b = *(signed float *)
						ds_pop(&vm_status->ds,
						sizeof(signed float));
					c = a * b;
					ret = ds_push(&vm_status->ds,
						sizeof(signed float), &c);
					break;
				case AUVMF_DOUBLE :
					signed double a, b, c;
					a = *(signed double *)
						ds_pop(&vm_status->ds,
						sizeof(signed double));
					b = *(signed double *)
						ds_pop(&vm_status->ds,
						sizeof(signed double));
					c = a * b;
					ret = ds_push(&vm_status->ds,
						sizeof(signed double), &c);
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

	switch (opcode) {
		case IN_DIV_UI :
			switch (arg) {
				case 1 :
					uint8_t a, b, c;
					a = *(uint8_t *)ds_pop(&vm_status->ds,
							sizeof(uint8_t));
					b = *(uint8_t *)ds_pop(&vm_status->ds,
							sizeof(uint8_t));
					c = a / b;
					ret = ds_push(&vm_status->ds,
							sizeof(uint8_t), &c);
					break;
				case 2 :
					uint16_t a, b, c;
					a = *(uint16_t *)ds_pop(&vm_status->ds,
							sizeof(uint16_t));
					b = *(uint16_t *)ds_pop(&vm_status->ds,
							sizeof(uint16_t));
					c = a / b;
					ret = ds_push(&vm_status->ds,
							sizeof(uint16_t), &c);
					break;
				case 4 :
					uint32_t a, b, c;
					a = *(uint32_t *)ds_pop(&vm_status->ds,
							sizeof(uint32_t));
					b = *(uint32_t *)ds_pop(&vm_status->ds,
							sizeof(uint32_t));
					c = a / b;
					ret = ds_push(&vm_status->ds,
							sizeof(uint32_t), &c);
					break;
				default : ret = 1;
			}
			break;
		case IN_DIV_SI :
			switch (arg) {
				case 1 :
					int8_t a, b, c;
					a = *(int8_t *)ds_pop(&vm_status->ds,
							sizeof(int8_t));
					b = *(int8_t *)ds_pop(&vm_status->ds,
							sizeof(int8_t));
					c = a / b;
					ret = ds_push(&vm_status->ds,
							sizeof(int8_t), &c);
					break;
				case 2 :
					int16_t a, b, c;
					a = *(int16_t *)ds_pop(&vm_status->ds,
							sizeof(int16_t));
					b = *(int16_t *)ds_pop(&vm_status->ds,
							sizeof(int16_t));
					c = a / b;
					ret = ds_push(&vm_status->ds,
							sizeof(int16_t), &c);
					break;
				case 4 :
					int32_t a, b, c;
					a = *(int32_t *)ds_pop(&vm_status->ds,
							sizeof(int32_t));
					b = *(int32_t *)ds_pop(&vm_status->ds,
							sizeof(int32_t));
					c = a / b;
					ret = ds_push(&vm_status->ds,
							sizeof(int32_t), &c);
					break;
				default : ret = 1;
			}
			break;
		case IN_DIV_UF :
			switch (arg) {
				case AUVMF_FLOAT :
					unsigned float a, b, c;
					a = *(unsigned float *)
						ds_pop(&vm_status->ds,
						sizeof(unsigned float));
					b = *(unsigned float *)
						ds_pop(&vm_status->ds,
						sizeof(unsigned float));
					c = a / b;
					ret = ds_push(&vm_status->ds,
						sizeof(unsigned float), &c);
					break;
				case AUVMF_DOUBLE :
					unsigned double a, b, c;
					a = *(unsigned double *)
						ds_pop(&vm_status->ds,
						sizeof(unsigned double));
					b = *(unsigned double *)
						ds_pop(&vm_status->ds,
						sizeof(unsigned double));
					c = a / b;
					ret = ds_push(&vm_status->ds,
						sizeof(unsigned double), &c);
					break;
				default : ret = 1;
			}
			break;
		case IN_DIV_SF :
			switch (arg) {
				case AUVMF_FLOAT :
					signed float a, b, c;
					a = *(signed float *)
						ds_pop(&vm_status->ds,
						sizeof(signed float));
					b = *(signed float *)
						ds_pop(&vm_status->ds,
						sizeof(signed float));
					c = a / b;
					ret = ds_push(&vm_status->ds,
						sizeof(signed float), &c);
					break;
				case AUVMF_DOUBLE :
					signed double a, b, c;
					a = *(signed double *)
						ds_pop(&vm_status->ds,
						sizeof(signed double));
					b = *(signed double *)
						ds_pop(&vm_status->ds,
						sizeof(signed double));
					c = a / b;
					ret = ds_push(&vm_status->ds,
						sizeof(signed double), &c);
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

	switch (opcode) {
		case IN_MOD_UI :
			switch (arg) {
				case 1 :
					uint8_t a, b, c;
					a = *(uint8_t *)ds_pop(&vm_status->ds,
							sizeof(uint8_t));
					b = *(uint8_t *)ds_pop(&vm_status->ds,
							sizeof(uint8_t));
					c = a % b;
					ret = ds_push(&vm_status->ds,
							sizeof(uint8_t), &c);
					break;
				case 2 :
					uint16_t a, b, c;
					a = *(uint16_t *)ds_pop(&vm_status->ds,
							sizeof(uint16_t));
					b = *(uint16_t *)ds_pop(&vm_status->ds,
							sizeof(uint16_t));
					c = a % b;
					ret = ds_push(&vm_status->ds,
							sizeof(uint16_t), &c);
					break;
				case 4 :
					uint32_t a, b, c;
					a = *(uint32_t *)ds_pop(&vm_status->ds,
							sizeof(uint32_t));
					b = *(uint32_t *)ds_pop(&vm_status->ds,
							sizeof(uint32_t));
					c = a % b;
					ret = ds_push(&vm_status->ds,
							sizeof(uint32_t), &c);
					break;
				default : ret = 1;
			}
			break;
		case IN_MOD_SI :
			switch (arg) {
				case 1 :
					int8_t a, b, c;
					a = *(int8_t *)ds_pop(&vm_status->ds,
							sizeof(int8_t));
					b = *(int8_t *)ds_pop(&vm_status->ds,
							sizeof(int8_t));
					c = a % b;
					ret = ds_push(&vm_status->ds,
							sizeof(int8_t), &c);
					break;
				case 2 :
					int16_t a, b, c;
					a = *(int16_t *)ds_pop(&vm_status->ds,
							sizeof(int16_t));
					b = *(int16_t *)ds_pop(&vm_status->ds,
							sizeof(int16_t));
					c = a % b;
					ret = ds_push(&vm_status->ds,
							sizeof(int16_t), &c);
					break;
				case 4 :
					int32_t a, b, c;
					a = *(int32_t *)ds_pop(&vm_status->ds,
							sizeof(int32_t));
					b = *(int32_t *)ds_pop(&vm_status->ds,
							sizeof(int32_t));
					c = a % b;
					ret = ds_push(&vm_status->ds,
							sizeof(int32_t), &c);
					break;
				default : ret = 1;
			}
			break;
		default : ret = 1;
	}

	return ret;
} 

int in_and(vm_t *vm_status, uint8_t opcode, (void)(uint8_t arg))
{
	int ret;
	uint8_t a, b, c;

	switch (opcode) {
		case IN_ADD :
			a = *(uint8_t *)ds_pop(&vm_status->ds,
						sizeof(uint8_t));
			b = *(uint8_t *)ds_pop(&vm_status->ds,
						sizeof(uint8_t));
			c = a & b;
			ret = ds_push(&vm_status->ds, sizeof(uint8_t), &c);
			break;
		case IN_ADD_L :
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

int in_or(vm_t *vm_status, uint8_t opcode, (void)(uint8_t arg))
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

int in_xor(vm_t *vm_status, uint8_t opcode, (void)(uint8_t arg))
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

int in_not(vm_t *vm_status, uint8_t opcode, (void)(uint8_t arg))
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

int in_jmp(vm_t *vm_status, uint8_t opcode, uint8_t arg)
{
	/* TODO: Implement */
	return 0;
}

int in_ret(vm_t *vm_status, uint8_t, opcode, uint8_t arg)
{
	/* TODO: Implement */
	return 0;
}
