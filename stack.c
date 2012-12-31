/*
 * stack.c - stack implementation
 *
 * Copyright (c) 2012 Peter Polacik <polacik.p@gmail.com>
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

/* Config */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* Local */
#include <auvm.h>
#include <stack.h>

/* System */
#include <stdlib.h>
#include <stdint.h>

/*
 * DS is implemented as classical array of 8-bit unsigned integers, status
 * is kept using 2 pointers, base pointer (the element 0 of "array") and
 * stack point (pointer to first free element of stack)
 *
 * stack size -> SP - BP
 * push 1B -> stack[SP++] = data
 * pop 1B -> stack[--SP]
 * drop 1B -> SP--
 */

int ds_init(ds_t *s, uint32_t sz)
{
	s->sz = sz;
	s->bp = (uint8_t *)malloc(sizeof(uint8_t) * sz);
	if (s->bp == NULL)
		return 1;
	else {
		s->sp = s->bp;
		return 0;
	}
}

int ds_destroy(ds_t *s)
{
	int ret;

	ret = (uint32_t) s->sp - (uint32_t) s->bp;
	s->sz = 0;
	s->sp = NULL;
	free(s->bp);
	return ret;
}

int ds_push(ds_t *s, uint32_t sz, const void *ptr)
{
	const uint8_t *src = (const uint8_t *)ptr;
	uint32_t count = 0;
	while (sz-- && (ds_size(s) < s->sz)) {
		*(s->sp++ - 1) = *src++;
}

int ds_pop(ds_t *s, uint32_t sz, void *ptr)
{
	uint8_t *dst = (uint8_t *)ptr;
	uint32_t count = 0;
	while (sz-- && ds_size(s)) {
		*dst++ = *(s->sp-- - 1);
		count++;
	}

	return count;
}

int ds_getelem(ds_t *s, uint32_t sz, uint32_t pos, void *ptr)
{
	uint8_t *dst = (uint8_t *)ptr;
	uint8_t *src = (uint8_t *)((uint32_t) s->bp + pos);
	uint32_t count = 0;

	if (pos >= ds_size(s))
		count = ds_pop(s, sz, ptr);
	else {
		while (sz-- && ds_size(s)) {
			*dst++ = *src--;
			count++;
		}
	}
	
	return count;
}

uint32_t ds_size(ds_t *s)
{
	return (uint32_t) s->sp - (uint32_t) s->bp;
}

uint32_t ds_limit(ds_t *s)
{
	return s->sz;
}
