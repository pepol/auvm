#ifndef _INS_H_
#define _INS_H_

/*
 * ins.h - instruction definitions
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


/* INSTRUCTION LIST */

/* General Instructions */
#define IN_NOP		0x00 /* No operation (no arg) */
#define IN_END		0x01 /* End execution (no arg) */
#define IN_DEBUG	0x02 /* Toggle debugging (DBG_FLAG) */
#define IN_STDCALL	0x03 /* Standard Library Function Call (STDCALL_NO) */

/* Stack instructions */
#define IN_LOAD		0x10 /* Load onto stack (LOAD_SZ) */
#define IN_DUP		0x11 /* Duplicate head on stack (DUP_SZ) */
#define IN_GET		0x12 /* Duplicate X element on stack (GET_FLAGS) */
#define IN_DROP		0x13 /* Drop head from stack (DROP_SZ) */

/* Arithmetical and Logical */
#define IN_ADD_UI	0x20 /* Add 2 unsigned integers (ADD_SZ) */
#define IN_ADD_SI	0x21 /* Add 2 signed integers (ADD_SZ) */
#define IN_ADD_UF	0x22 /* Add 2 unsigned floats (ADD_TYPE) */
#define IN_ADD_SF	0x23 /* Add 2 signed floats (ADD_TYPE) */

#define IN_SUB_UI	0x24 /* Subtract 2 unsigned integers (SUB_SZ) */
#define IN_SUB_SI	0x25 /* Subtract 2 signed integers (SUB_SZ) */
#define IN_SUB_UF	0x26 /* Subtract 2 unsigned floats (SUB_TYPE) */
#define IN_SUB_SF	0x27 /* Subtract 2 signed floats (SUB_TYPE) */

#define IN_MUL_UI	0x28 /* Multiply 2 unsigned integers (MUL_SZ) */
#define IN_MUL_SI	0x29 /* Multiply 2 signed integers (MUL_SZ) */
#define IN_MUL_UF	0x2A /* Multiply 2 unsigned floats (MUL_TYPE) */
#define IN_MUL_SF	0x2B /* Multiply 2 signed floats (MUL_TYPE) */

#define IN_DIV_UI	0x2C /* Divide 2 unsigned integers (DIV_SZ) */
#define IN_DIV_SI	0x2D /* Divide 2 signed integers (DIV_SZ) */
#define IN_DIV_UF	0x2E /* Divide 2 unsigned floats (DIV_TYPE) */
#define IN_DIV_SF	0x2F /* Divide 2 signed floats (DIV_TYPE) */

#define IN_MOD_UI	0x30 /* Modulo of 2 unsigned integers (MOD_SZ) */
#define IN_MOD_SI	0x31 /* Modulo of 2 signed integers (MOD_SZ) */

#define IN_AND		0x32 /* Bitwise AND (AND_SZ) */
#define IN_AND_L	0x33 /* Logical AND (AND_SZ) */
#define IN_OR		0x34 /* Bitwise OR (OR_SZ) */
#define IN_OR_L		0x35 /* Logical OR (OR_SZ) */
#define IN_XOR		0x36 /* Bitwise XOR (XOR_SZ) */
#define IN_XOR_L	0x37 /* Logical XOR (XOR_SZ) */
#define IN_NOT		0x38 /* Bitwise NOT (NOT_SZ) */
#define IN_NOT_L	0x39 /* Logical NOT (NOT_SZ) */
#define IN_SHL		0x3A /* Bitwise Shift Left (SHL_SZ); max. 255 */
#define IN_SHR		0x3B /* Bitwise Shift Right (SHR_SZ); max. 255 */
#define IN_ROTL		0x3C /* Bitwise Rotate Left (ROTL_SZ); max. 255 */
#define IN_ROTR		0x3D /* Bitwise Rotate Right (ROTR_SZ); max. 255 */

/* Jumps and branching */
#define IN_JMP		0x40 /* Object-wise jump (JMP_TYPE: REL | ABS) */
#define IN_JMP_L	0x41 /* Long jump (JMP_FLAGS) */
#define IN_CALL		0x42 /* Object-wise returnable jump (CALL_TYPE) */
#define IN_CALL_L	0x43 /* Long returnable jump (CALL_FLAGS) */
#define IN_RET		0x44 /* Return (RET_COUNT) */

/* TODO: Conditionals */
