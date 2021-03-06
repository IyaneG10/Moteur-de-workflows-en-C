/**
 * @file libsck.c
 * @brief 
 * @version 0.1
 * @date 2020-01-30
 * 
 * @copyright Copyright (c) 2020
 * 
 */
/* Fichier d'inclusion */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "libsck.h"
#include "libthrd.h"

/**
 * @brief Initialisation de la communication réseau
 * 
 * @param port port passé en argument de la fonction main dans Workflow_engine
 * @return int socket
 */
int initialisationServeur(char *port)
{
	struct addrinfo precisions,*resultat,*origine; // liste chainée pour trouver des adresses
	int statut; 
	int s; // descripteur de fichier pour la socket serveur TCP

	// Construction de la structure adresse 
	memset(&precisions,0,sizeof precisions);
	precisions.ai_family=AF_UNSPEC; // aucune précision, donc prend aussi en compte les clients IPv4 et IPv6
	precisions.ai_socktype=SOCK_STREAM; // Type de socket: TCP (mode flux, connecté) => SOCK_STREAM 
	precisions.ai_flags=AI_PASSIVE; // On veut un serveur (ne fait qu'écouter) => AI_PASSIVE (AI_ACTIVE pour client)
	statut=getaddrinfo(NULL,port,&precisions,&origine);

	if(statut<0){ perror("initialisationServeur.getaddrinfo"); exit(EXIT_FAILURE); }
	struct addrinfo *p;
	for(p=origine,resultat=origine;p!=NULL;p=p->ai_next)
	{
		if(p->ai_family==AF_INET6){ resultat=p; break; } // chercher des adresses jusqu'à IP v6 

	}  
	/**
	 * @brief Création d'une socket
	 * 
	 */
	s=socket(resultat->ai_family,resultat->ai_socktype,resultat->ai_protocol);
	if(s<0){ perror("initialisationServeur.socket"); exit(EXIT_FAILURE); }
	printf("Socket TCP crée avec succes\n"); 

	/**
	 * @brief Options utiles
	 * 
	 */
	int vrai=1;
	/**
	 * @brief socket=s; niveau"reseau (sol_socket)"=; option= adresse reutilisable
	 * 
	 */
	if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&vrai,sizeof(vrai))<0){
		perror("initialisationServeur.setsockopt (REUSEADDR)");
		exit(EXIT_FAILURE);
	}
	/**
	 * @brief Options utiles
	 * socket=s; niveau"couche de transport (mode TCP)"=; option= pas de retard (delay) pour envoyer les messages
	 * 
	 */

	if(setsockopt(s,IPPROTO_TCP,TCP_NODELAY,&vrai,sizeof(vrai))<0){
		perror("initialisationServeur.setsockopt (NODELAY)");
		exit(EXIT_FAILURE);
	}


	/**
	 * @brief Spécification de l'adresse de la socket
	 * 
	 */
	statut=bind(s,resultat->ai_addr,resultat->ai_addrlen);
	if(statut<0) return -1;
	printf("Socket TCP \"bindé\" avec succes\n"); 

	/**
	 * @brief Liberation de la structure d'informations
	 * 
	 */
	//  
	freeaddrinfo(origine); 

	/**
	 * @brief écoute sur la file d'attentes de taille=[connecions]
	 * 
	 */
	statut=listen(s,NB_CONNEXIONS);
	printf("En attente de connexion avec les clients TCP\n");
	if(statut<0) return -1;

	return s;
}


void dummy(void *dialogue, void* (*action)(void *))
{
	action(dialogue);
}





/**
 * @brief boucle d'attente des connexions
 * 
 * @param socket 
 * @param fonction 
 * @param action 
 * @return long 
 */
long boucleServeur(long socket, void(*fonction)(void *, void *(*)(void*)), void* (*action)(void*))
{

	long dialogue; // socket pour le dialogue entre serveur et clients
	while(1)
	{
		socklen_t taille=sizeof(adresse);
		// Attente d'une connexion 
		if((dialogue=accept(socket,(struct sockaddr *)&adresse,&taille))<0) return -1;

		// Passage de la socket de dialogue a la fonction de traitement 
		fonction((void*)dialogue,action);
	}
	return socket;
}
