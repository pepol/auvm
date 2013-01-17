/*
 * stack.c - implementation of stacks
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

/* System includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

/* DS */

int ds_init(ds_t *s, uint32_t max)
{
	s->st_max = max;
	s->st_count = 0;
	s->st_data = (uint8_t *)malloc(sizeof(uint8_t) * max);
	if (s->st_data == NULL)
		return 1;
	else
		return 0;
}

int ds_destroy(ds_t *s)
{
	int ret;

	ret = s->st_count;
	s->st_max = 0;
	s->st_count = 0;
	free(s->st_data);
	return ret;
}

int ds_push(ds_t *s, uint32_t sz, const void *ptr)
{
	if ((s->st_count + sz) < s->st_max) {
		s->st_count += sz;
		revmemcpy(&(s->st_data[s->st_count - 1]), ptr, sz);
		return 0;
	} else return 1;
}

void *ds_pop(ds_t *s, uint32_t sz)
{
	void *ret;
	long comp = s->st_count - sz;
	if (comp >= 0) {
		ret = &(s->st_data[s->st_count - sz]);
		s->st_count -= sz;
		return ret;
	} else return NULL;
}

void *ds_getelem(ds_t *s, uint32_t sz, uint32_t pos)
{
	void *ret;
	long comp = s->st_count - sz;
	long poscomp = pos - sz;
	if ((comp >= 0)	&& (pos < s->st_count) && (poscomp >= 0)) {
		ret = &(s->st_data[pos]);
		return ret;
	} else return NULL;
}

uint32_t ds_size(ds_t *s)
{
	return s->st_count;
}

uint32_t ds_limit(ds_t *s)
{
	return s->st_max;
}

void ds_show(ds_t *s)
{
	unsigned int i;
	printf("\n\nBEGIN DATA STACK DUMP\n");
	for (i = 0; i < s->st_count; i++) {
		if (i % 8 == 0)
			printf("\n");
		printf(" %.2x", s->st_data[i]);
	}
	printf("\nEND DATA STACK DUMP\n");
}


/* CS */

int cs_init(cs_t *s, uint32_t max)
{
	s->st_max = max;
	s->st_count = 0;
	s->st_data = (ip_t *)malloc(sizeof(ip_t) * max);
	if (s->st_data == NULL)
		return 1;
	else
		return 0;
}

int cs_destroy(cs_t *s)
{
	int ret;

	ret = s->st_count;
	s->st_max = 0;
	s->st_count = 0;
	free(s->st_data);
	return ret;
}

int cs_push(cs_t *s, const ip_t *ptr)
{
	if ((s->st_count + 1) < s->st_max) {
		s->st_count++;
		memcpy(&(s->st_data[s->st_count - 1]), ptr, sizeof(ip_t));
		return 0;
	} else return 1;
}

ip_t *cs_pop(cs_t *s)
{
	ip_t *ret;
	long comp = s->st_count - 1;
	if (comp >= 0) {
		ret = &(s->st_data[s->st_count - 1]);
		s->st_count--;
		return ret;
	} else return NULL;
}

ip_t *cs_getelem(cs_t *s, uint32_t pos)
{
	ip_t *ret;
	long comp = s->st_count - 1;
	long poscomp = pos - 1;
	if ((comp >= 0) && (pos < s->st_count) && (poscomp >= 0)) {
		ret = &(s->st_data[pos]);
		return ret;
	} else return NULL;
}

uint32_t cs_size(cs_t *s)
{
	return s->st_count;
}

uint32_t cs_limit(cs_t *s)
{
	return s->st_max;
}
