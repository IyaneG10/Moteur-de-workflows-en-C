#ifndef __LIBSCK_H__
#define __LIBSCK_H__

/* initalise la socket */
/* Param: numero de port */
/* Return: socket de lecture */

/**** Variables globales *****/
struct sockaddr_in adresse;



/** Constantes **/
#define NB_CONNEXIONS 10 

int initialisationServeur(char *port);




//void gestionClientLeger(void* s);

void dummy(void *dialogue, void* (*action)(void *));

//int boucleServeur(int socket, void (*dummy)(void *, void *(*gestionClient)(void*)), void *);
//int boucleServeur(int s,int (*multi_gestionClient)(int[2]),int socket_typ);
long boucleServeur(long socket, void(*fonction)(void *, void *(*)(void*)), void* (*gestionClient)(void *));






#endif
