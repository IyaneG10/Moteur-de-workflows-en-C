#ifndef __LIBSCK_H__
#define __LIBSCK_H__

/**** Variables globales *****/
struct sockaddr_in adresse;

/** Constantes **/
#define NB_CONNEXIONS 10 

int initialisationServeur(char *port);

void dummy(void *dialogue, void* (*action)(void *));

long boucleServeur(long socket, void(*fonction)(void *, void *(*)(void*)), void* (*gestionClient)(void *));

#endif
