# compiler setup
CC=gcc
CFLAGS= -Wall -Wextra -Wshadow -Wpedantic -std=c99 -O0 -g

# define targets
TARGETS=kNN mk

#define object-files
OBJ=mk.o kNN.o trie.o

build: $(TARGETS)

mk: mk.o trie.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

pack:
	zip -FSr 312CA_RusRazvan_Tema3.zip README.md Makefile *.c *.h

clean:
	rm -f $(TARGETS) $(OBJ)

.PHONY: pack clean
