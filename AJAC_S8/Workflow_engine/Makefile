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
	$(CC) -DDEBUG -o main main.c optionServer.c gestion_client.c $(LOCAL_LIBS)  $(CFLAGS) $(LIBS) -lresolv


clean:
	@echo
	@echo Debut du nettoyage Workflow_engine
	rm -f *.o
	#rm -f core *.o 
	@echo Debut du nettoyage Workflow_engine


#
# Les cibles pour l'executable
#
#gcc -Wall -o nom_du_fichier_objet  -c nom_du_fichier_source_C
  #gcc -o nom_du_programme   -lnom_de_la_librairie   nom_du_fichier_objet 
main: main.o optionServer.o gestion_client.o
	$(CC) -o main main.c optionServer.c gestion_client.c $(LOCAL_LIBS)  $(CFLAGS) $(LIBS) -lresolv 
	#$(CC) main.o optionServer.o -o main
#gcc -L/usr/local/lib/tableaunoir  essai.c -o essai.exe -lTableauNoir 
#gcc [options] [source files] [object files] [-Ldir] -llibname [-o outfile]
main.o: main.c optionServer.h gestion_client.h
	$(CC) -Wall $(CFLAGS)  -c  main.c 

optionServer.o : optionServer.c
	$(CC) -c -Wall optionServer.c

gestion_client.o : gestion_client.c 
	$(CC) -c -Wall gestion_client.c







#main: $(OBJS) optionServer.o
#	$(CC) $(OBJS) optionServer.o $(CFLAGS) -g -o main  $(LOCAL_LIBS) $(LIBS) -lresolv

#main.o: main.c optionServer.h
#	$(CC) -c -Wall  main.c

#optionServer.o : optionServer.c
#	$(CC) -c -Wall optionServer.c

#DEBUG: $(OBJS)
#	$(CC) $(CFLAGS) -g -o main $(OBJS) $(LOCAL_LIBS) $(LIBS) -DDEBUG

#main : main.o optionServer.o
#gcc main.o optionServer.o -o main
#main.o : main.c optionServer.h
#gcc -c -Wall main.c
#optionServer.o : optionServer.c
#gcc -c -Wall optionServer.c
