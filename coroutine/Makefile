#
# @file  Makefile
# @author chenxueyou
# @version 0.1
# @brief   :A asymmetric coroutine library for C++
# History
#      1. Date: 2014-12-12 
#          Author: chenxueyou
#          Modification: this file was created 
#

CFLAGS = -std=gnu99 
CC = gcc

all:
	$(CC) $(CFLAGS)  coroutine.c  -g -c
	$(CC) $(CFLAGS)  main.c -g -o test coroutine.o
clean:
	rm -f coroutine.o test 
