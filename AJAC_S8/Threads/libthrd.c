/**
 * @file libthrd.c
 * @brief 
 * @version 0.1
 * @date 2020-01-30
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "libthrd.h"

/**
 * @brief 3 verrous pour les zones sensibles
 * - pour la table des utilisateurs connectés
 * - pour le fichier de stockage des utilisateurs
 * - pour les listes chainés (processus et activités)
 * 
 */
pthread_mutex_t verrou1; 
pthread_mutex_t verrou2; 
pthread_mutex_t verrou3; 


typedef struct  
{
	void* (*fonction)(void *);
	void *parametre;
} Parametres;

/**
 * @brief Récupération des paramètres
 * 
 * @param arg 
 * @return void* 
 */
void *recup_param_Thread(void *arg)
{
	Parametres *parametres_thread= arg;
	parametres_thread->fonction(parametres_thread->parametre);
	free(parametres_thread);
	return NULL;
}


/**
 * @brief Lancer un thread
 * 
 * @param arg argument de la fonction à executer
 * @param fonction fonction à executer par le thread
 */
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

/**
 * @brief Primitive pour prendre le mutex
 * 
 * @param index selon l'enum
 */
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

/**
 * @brief Primitive pour Vendre le mutex
 * 
 * @param index selon enum
 */
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
