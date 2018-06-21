#
# Student makefile for Cache Lab
# Note: requires a 64-bit x86-64 system 
#
CC = gcc
CFLAGS = -g -Wall -Werror -std=c99 -m64

csim: csim.c cachelab.c cachelab.h
	$(CC) $(CFLAGS) -o bin/csim csim.c cachelab.c libcsim.h libcsim.c -lm

test:
	cc -std=c99 -Wall -g test.c minunit.h dbg.h libcsim.h libcsim.c -o bin/test

.PHONY: run-test
run-test: test
	./bin/test

.PHONY: run
run: csim
	./bin/csim

# Clean the src dirctory
#
clean:
	rm -rf *.o
	rm -f *.tar
	rm -f csim
	rm -f .csim_results .marker
