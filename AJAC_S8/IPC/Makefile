#
# Makefile pour generer la bibliotheque de fonctions IPC
#


SOURCES = $(wildcard *.c)
OBJETS = $(SOURCES:.c=.o)
BIBLIOTHEQUE = libipc.a
AR = ar r

all: $(BIBLIOTHEQUE)

$(BIBLIOTHEQUE): $(OBJETS)
	$(AR) $@ $^

$(OBJETS): libipc.h 

clean:
	rm -rf *.o *.a core












