
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "libthrd.h"



typedef struct  
{
	void* (*fonction)(void *);
	void *parametre;
} Parametres;


void *recup_param_Thread(void *arg)
{
	Parametres *parametres_thread= arg;
	parametres_thread->fonction(parametres_thread->parametre);
	free(parametres_thread);
	return NULL;
}



void lanceClientLeger(void *arg, void* (*fonction)(void*))
{

	pthread_t _monthread;

	// allocation de memoire
	Parametres *parametres_thread =  malloc(sizeof(Parametres));

	//  parametrage
	parametres_thread->fonction = fonction;
	parametres_thread->parametre = arg;

	// Creation du thread
	pthread_create(&_monthread, NULL, recup_param_Thread,parametres_thread);


	// détachement du thread
	pthread_detach(_monthread);


}

