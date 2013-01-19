/*
 * lib/io.c - AUVM Library I/O functions
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

#include "../auvmlib.h"
#include "../auvm.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int wrapper_print_str(vm_t *vm_status)
{
	int32_t fd;
	uint32_t str_sz;
	char *str;

	fd = *(int32_t *)ds_pop(&vm_status->ds, sizeof(int32_t));
	str_sz = *(uint32_t *)ds_pop(&vm_status->ds, sizeof(uint32_t));

	str = (char *)malloc(sizeof(char) * (str_sz + 1));
	if (str == NULL)
		return 1;
	revmemcpy(str + str_sz, (const char *)ds_pop(&vm_status->ds,
				sizeof(char) * (str_sz)), str_sz);

	str[str_sz] = '\0';
	write((int) fd, str, str_sz);
	return 0;
}

int wrapper_print_int(vm_t *vm_status)
{
	int32_t fd;
	int32_t num;
	char buf[256] = "";

	fd = *(int32_t *)ds_pop(&vm_status->ds, sizeof(int32_t));
	num = *(int32_t *)ds_pop(&vm_status->ds, sizeof(int32_t));

	snprintf(buf, 255, "%d", num);
	write((int) fd, buf, strlen(buf));
	return 0;
}

int wrapper_print_uint(vm_t *vm_status)
{
	int32_t fd;
	uint32_t num;
	char buf[256] = "";

	fd = *(int32_t *)ds_pop(&vm_status->ds, sizeof(int32_t));
	num = *(uint32_t *)ds_pop(&vm_status->ds, sizeof(uint32_t));

	snprintf(buf, 255, "%u", num);
	write((int) fd, buf, strlen(buf));
	return 0;
}

int wrapper_print_float(vm_t *vm_status)
{
	int32_t fd;
	int8_t prec;
	float num;
	char buf[256] = "";

	fd = *(int32_t *)ds_pop(&vm_status->ds, sizeof(int32_t));
	prec = *(int8_t *)ds_pop(&vm_status->ds, sizeof(int8_t));
	num = *(float *)ds_pop(&vm_status->ds, sizeof(float));

	snprintf(buf, 255, "%.*g", (int) prec, num);
	write((int) fd, buf, strlen(buf));
	return 0;
}

int wrapper_print_double(vm_t *vm_status)
{
	int32_t fd;
	int8_t prec;
	double num;
	char buf[256] = "";

	fd = *(int32_t *)ds_pop(&vm_status->ds, sizeof(int32_t));
	prec = *(int8_t *)ds_pop(&vm_status->ds, sizeof(int8_t));
	num = *(double *)ds_pop(&vm_status->ds, sizeof(double));

	snprintf(buf, 255, "%.*g", (int) prec, num);
	write((int) fd, buf, strlen(buf));
	return 0;
}
