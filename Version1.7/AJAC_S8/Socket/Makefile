#
# Makefile pour generer la bibliotheque de fonctions reseau
#




############################################################################################################################################################

SOURCES = $(wildcard *.c)
OBJETS = $(SOURCES:.c=.o)
BIBLIOTHEQUE = libsck.a
AR = ar r
LOCAL_LIBS  =  -L  ../Threads 
LOCAL_LIBS +=  -lthrd 
CFLAGS     +=  -g -Wall    -I ../Threads  


all: $(BIBLIOTHEQUE)

$(BIBLIOTHEQUE): $(OBJETS)
	$(AR) $@ $^

$(OBJETS): libsck.h 

clean:
	rm -rf *.o *.a core












