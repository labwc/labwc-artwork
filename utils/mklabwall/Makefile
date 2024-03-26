CC?=cc
CFLAGS+=-Wall -pedantic -std=gnu99 $(shell pkg-config --cflags pangocairo)
LDFLAGS+=$(shell pkg-config --libs pangocairo)

all: mklabwall

mklabwall: mklabwall.o
	$(CC) -o $@ $^ $(LDFLAGS)

mklabwall.o: mklabwall.c
	$(CC) -o $@ $(CFLAGS) -c $^

install:
	install -s -m 0755 mklabwall $(DESTDIR)/usr/local/bin/

clean:
	-rm -f *.o mklabwall
