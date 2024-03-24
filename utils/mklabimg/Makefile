CC?=cc
CFLAGS+=-Wall -pedantic -std=gnu99 $(shell pkg-config --cflags pangocairo)
LDFLAGS+=$(shell pkg-config --libs pangocairo)

all: mklabimg

mklabimg: mklabimg.o
	$(CC) -o $@ $^ $(LDFLAGS)

mklabimg.o: mklabimg.c
	$(CC) -o $@ $(CFLAGS) -c $^

install:
	install -s -m 0755 mklabimg $(DESTDIR)/usr/local/bin/

clean:
	-rm -f *.o mklabimg
