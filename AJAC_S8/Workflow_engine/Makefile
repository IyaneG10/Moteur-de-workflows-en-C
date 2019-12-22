#
# Makefile pour generer le moteur de workflows
#


CC=gcc
OBJS = main.o
LIBS = -lpthread
LOCAL_LIBS  =    -L ../Socket  -L ../Threads  -L ../IPC
LOCAL_LIBS +=  -lsck -lthrd -lipc
CFLAGS     +=    -I ../Socket  -I ../Threads   -I ../IPC


#
# Cible generale
#

all: main

#
# La cible de nettoyage
#

debug: 
	$(CC) -DDEBUG -o main main.c optionServer.c gestion_client.c $(LOCAL_LIBS)  $(CFLAGS) $(LIBS) -lresolv -I/usr/include/libxml2/ -lxml2


clean:
	@echo
	@echo Debut du nettoyage Workflow_engine
	rm -f *.o

	@echo Debut du nettoyage Workflow_engine

main: main.o optionServer.o gestion_client.o
	$(CC) -o main main.c optionServer.c gestion_client.c $(LOCAL_LIBS)  $(CFLAGS) $(LIBS) -lresolv -I/usr/include/libxml2/ -lxml2

main.o: main.c optionServer.h gestion_client.h
	$(CC) -Wall $(CFLAGS)  -c  main.c 

optionServer.o : optionServer.c
	$(CC) -c -Wall optionServer.c

gestion_client.o : gestion_client.c 
	$(CC) -c -Wall gestion_client.c  -I/usr/include/libxml2/ -lxml2
