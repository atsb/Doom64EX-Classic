# Makefile for d64bsp

CC=gcc
CFLAGS=-Wall -g

LD=gcc
LDFLAGS=-lm

OBJS=analyze.o blockmap.o display.o glbsp.o level.o main.o node.o reject.o seg.o system.o util.o wad.o

all: d64bsp

d64bsp: $(OBJS)
	$(LD) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -f *.o d64bsp
