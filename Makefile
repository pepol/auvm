# Makefile - Main build makefile for AUVM
#
# Copyright (c) 2013 Peter Polacik <polacik.p@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.

NAME = auvm

VERSION = 0.1dev1

# Directories
PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
DOCDIR ?= $(PREFIX)/share/doc/$(NAME)
MANDIR ?= $(PREFIX)/share/man
LIBDIR ?= $(PREFIX)/lib

## BUILD SETTINGS

CC ?= gcc
CFLAGS += -c -std=gnu99 -W -Wall -Wextra -Wno-unused-value $(CDEBUG)
LDFLAGS += $(LDEBUG)

OUTFILE ?= $(NAME)
OBJS = stack.o util.o parse.o

.PHONY: all debug clean install uninstall

all: $(OUTFILE) $(OBJS)

$(OUTFILE): $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) $<

debug:
	make CDEBUG="-DDEBUG -g" LDEBUG="-g"

clean:
	rm -f *.o
	rm -f $(OUTFILE)
	rm -f *.log *.debug

install: $(OUTFILE)
	install -m 0755 $(OUTFILE) $(BINDIR)
	install -m 0755 doc/$(NAME)/* $(DOCDIR)
	install -m 0755 doc/man/* $(MANDIR)

uninstall: $(OUTFILE)
	rm -f $(BINDIR)/$(OUTFILE)
	rm -rf $(DOCDIR)
