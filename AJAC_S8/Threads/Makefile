#
# Makefile pour generer la bibliotheque de threads
#


SOURCES = $(wildcard *.c)
OBJETS = $(SOURCES:.c=.o)
BIBLIOTHEQUE = libthrd.a
AR = ar r
CFLAGS += -g -Wall


all: $(BIBLIOTHEQUE)

$(BIBLIOTHEQUE): $(OBJETS)
	$(AR) $@ $^

$(OBJETS): libthrd.h

clean:
	rm -rf *.o *.a core

