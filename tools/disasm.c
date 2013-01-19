/*
 * tools/disasm.c - Disassembler for AUVM
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
#include "../config.h"
#endif

/* Local includes */
#define _AUVM_H_
#include "../ins.h"

/* System includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>

void usage(const char *progname, int ec, FILE *s)
{
	fprintf(s, 
		"Usage: %s [-h] file1 [file2 .. fileN]\n",
		progname);
	fprintf(s, "\n\t-h\tShow this text.\n");
	exit(ec);
}

int disassemble(char *fname, char **mnem_tbl, char **fname_tbl)
{
	int fd;
	struct stat sbuf;
	uint32_t fsize, i;
	uint8_t opcode, oparg;
	
	fd = open(fname, O_RDONLY);
	if (fd == -1)
		return 1;
	if (fstat(fd, &sbuf) == -1)
		return 2;
	fsize = (uint32_t) sbuf.st_size;

	for (i = 0; i < fsize; i += 2) {
		read(fd, &opcode, 1);
		read(fd, &oparg, 1);
		/* data == opcode */
		if (opcode != IN_STDCALL)
			printf("%s %#u", mnem_tbl[opcode], oparg);
		else if (opcode == IN_STDCALL)
			printf("%s %s", mnem_tbl[opcode], fname_tbl[oparg]);
		if (opcode == IN_LOAD) {
			uint8_t buf;
			int j;

			printf(", 0x");
			for (j = 0; j < oparg; j++) {
				read(fd, &buf, 1);
				printf("%.2x", buf);
			}
			i += oparg;
		}
		printf("\n");

	}

	return 0;
}

int main(int argc, char **argv)
{
	int opt, filecount, i;
	char **filearr;
	char **ins_mnem;
	char **stdcall_fnames;

	ins_mnem = (char **)malloc(sizeof(char *) * 256);
	stdcall_fnames = (char **)malloc(sizeof(char *) * 256);

	if ((ins_mnem == NULL) || (stdcall_fnames == NULL))
		return 1;

	for (i = 0; i < 256; i++) {
		ins_mnem[i] = "ndf";
		stdcall_fnames[i] = "undefined";
	}

	ins_mnem[IN_NOP] = "nop";
	ins_mnem[IN_END] = "end";
	ins_mnem[IN_DEBUG] = "debug";
	ins_mnem[IN_STDCALL] = "stdcall";

	ins_mnem[IN_LOAD] = "load";
	ins_mnem[IN_DUP] = "dup";
	ins_mnem[IN_GET] = "get";
	ins_mnem[IN_DROP] = "drop";

	ins_mnem[IN_ADD_UI] = "add";
	ins_mnem[IN_ADD_SI] = "sadd";
	ins_mnem[IN_ADD_UF] = "addf";
	ins_mnem[IN_ADD_SF] = "saddf";

	ins_mnem[IN_SUB_UI] = "sub";
	ins_mnem[IN_SUB_SI] = "ssub";
	ins_mnem[IN_SUB_UF] = "subf";
	ins_mnem[IN_SUB_SF] = "ssubf";

	ins_mnem[IN_MUL_UI] = "mul";
	ins_mnem[IN_MUL_SI] = "smul";
	ins_mnem[IN_MUL_UF] = "mulf";
	ins_mnem[IN_MUL_SF] = "smulf";

	ins_mnem[IN_DIV_UI] = "div";
	ins_mnem[IN_DIV_SI] = "sdiv";
	ins_mnem[IN_DIV_UF] = "divf";
	ins_mnem[IN_DIV_SF] = "sdivf";

	ins_mnem[IN_MOD_UI] = "mod";
	ins_mnem[IN_MOD_SI] = "smod";

	ins_mnem[IN_AND] = "and";
	ins_mnem[IN_AND_L] = "land";
	ins_mnem[IN_OR] = "or";
	ins_mnem[IN_OR_L] = "lor";
	ins_mnem[IN_XOR] = "xor";
	ins_mnem[IN_XOR_L] = "lxor";
	ins_mnem[IN_NOT] = "not";
	ins_mnem[IN_NOT_L] = "lnot";
	ins_mnem[IN_SHL] = "shl";
	ins_mnem[IN_SHR] = "shr";
	ins_mnem[IN_ROTL] = "rotl";
	ins_mnem[IN_ROTR] = "rotr";

	ins_mnem[IN_JMP] = "jmp";
	ins_mnem[IN_JMP_L] = "ljmp";
	ins_mnem[IN_CALL] = "call";
	ins_mnem[IN_CALL_L] = "lcall";
	ins_mnem[IN_RET] = "ret";

	ins_mnem[IN_CMP] = "cmp";
	ins_mnem[IN_IFEQ] = "ife";
	ins_mnem[IN_IFNEQ] = "ifne";
	ins_mnem[IN_IFGT] = "ifgt";
	ins_mnem[IN_IFGE] = "ifge";
	ins_mnem[IN_IFLT] = "iflt";
	ins_mnem[IN_IFLE] = "ifle";


	stdcall_fnames[1] = "print_string";
	stdcall_fnames[2] = "print_int";
	stdcall_fnames[3] = "print_uint";
	stdcall_fnames[4] = "print_float";
	stdcall_fnames[5] = "print_double";

	while ((opt = getopt(argc, argv, "h")) != -1) {
		switch (opt) {
			case 'h' :
				usage(argv[0], 0, stdout);
				break;
			default :
				usage(argv[0], 2, stderr);
		}
	}

	if (optind >= argc)
		usage(argv[0], 3, stderr);

	filecount = argc - optind;
	filearr = &argv[optind];

	for (i = 0; i < filecount; i++) {
		printf("\n; BEGIN FILE %s\n", filearr[i]);
		disassemble(filearr[i], ins_mnem, stdcall_fnames);
		printf("; END FILE %s\n", filearr[i]);
	}

	free(ins_mnem);
	free(stdcall_fnames);

	return 0;
}
