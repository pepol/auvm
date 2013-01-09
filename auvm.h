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

/* System includes */
#include <stdint.h>
#include <stddef.h>


/* From stack.h */
#include "stack.h"

/* From object.h */
#include "object.h"

#include "ins.h"
#include "intable.h"

/* VM status structure */
typedef struct _vm {
	/* instruction pointers */
	ip_t cip;
	ip_t nip;
	/* stack pointers */
	ds_t ds;
	cs_t cs;
	/* instruction table */
	in_t *in_table;
	/* object table */
	uint8_t obj_count;
	obj_t *ctbl;
	/* FLAGS register */
	uint8_t flags;
} vm_t;

/* FLAGS definition */
#define FLAGS_COMP_LT (1 << 0)
#define FLAGS_COMP_GT (1 << 1)

/* Flags - format */
#define AUVMF_FLOAT 0x01
#define AUVMF_DOUBLE 0x02
#define AUVMF_UINT 0x03
#define AUVMF_SINT 0x04

/* Flags - jumps */
#define JMP_REL 0x00
#define JMP_ABS 0x01

/* Instruction functions */
extern int in_nop(vm_t *, uint8_t, uint8_t);
extern int in_end(vm_t *, uint8_t, uint8_t);
extern int in_debug(vm_t *, uint8_t, uint8_t);
extern int in_stdcall(vm_t *, uint8_t, uint8_t);
extern int in_stack(vm_t *, uint8_t, uint8_t);
extern int in_add(vm_t *, uint8_t, uint8_t);
extern int in_sub(vm_t *, uint8_t, uint8_t);
extern int in_mul(vm_t *, uint8_t, uint8_t);
extern int in_div(vm_t *, uint8_t, uint8_t);
extern int in_mod(vm_t *, uint8_t, uint8_t);
extern int in_and(vm_t *, uint8_t, uint8_t);
extern int in_or(vm_t *, uint8_t, uint8_t);
extern int in_xor(vm_t *, uint8_t, uint8_t);
extern int in_not(vm_t *, uint8_t, uint8_t);
extern int in_shl(vm_t *, uint8_t, uint8_t);
extern int in_shr(vm_t *, uint8_t, uint8_t);
extern int in_jmp(vm_t *, uint8_t, uint8_t);
extern int in_ret(vm_t *, uint8_t, uint8_t);
extern int in_cmp(vm_t *, uint8_t, uint8_t);
extern int in_if(vm_t *, uint8_t, uint8_t);

/* init.c */
extern vm_t *auvm_init(uint32_t, uint32_t, int, char **);

/* parse.c */
extern int parse(vm_t *);

/* util.c */
extern void *revmemcpy(void *, const void *, uint32_t);

#endif /* _SLVM_H_ */
