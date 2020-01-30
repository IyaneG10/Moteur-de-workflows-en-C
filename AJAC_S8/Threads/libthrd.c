/**
 * @file libthrd.c
 * @author SECK Malick
 * @brief 
 * @version 0.1
 * @date 2020-01-30
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "libthrd.h"

pthread_mutex_t verrou1; 
pthread_mutex_t verrou2; 
pthread_mutex_t verrou3; 

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


void P(int index) {
    switch(index)
    {
        case 0: 
            pthread_mutex_lock(&(verrou1)); 
              break;
        case 1: 
            pthread_mutex_lock(&(verrou2));
            break;
        case 2: 
            pthread_mutex_lock(&(verrou3));
            break;
    }
}

void V(int index) {
    switch(index)
    {
        case 0: 
            pthread_mutex_unlock(&(verrou1)); 
            break;
        case 1: 
            pthread_mutex_unlock(&(verrou2));
            break;
        case 2: 
            pthread_mutex_unlock(&(verrou3));
            break;
    }
}
