/** fichier libipc.c **/

/* Fichier d'inclusion */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <resolv.h> 
#include <pthread.h>
#include <stdbool.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 

#include "libipc.h"



void* gestion_file_message(void* arg)
{
	int requetes, reponses;
	messsage_IPC msg;
	

	/* Se connecter aux IPC de requête et de réponse */
	requetes = msgget(CLE_REQUETE, 0700 | IPC_CREAT); 
	if (requetes == -1) { perror("msgget");} 

	reponses = msgget(CLE_REPONSE, 0700 | IPC_CREAT); 
	if (reponses == -1) { perror("msgget"); } 

	while (1) {
		printf("En attente des requetes admin\n");
		if (msgrcv(requetes, & msg, TAILLE_MSG, 0, 0) == -1) { perror("msgrcv"); } 
		// afficher la requete IPC 
		printf("La requete admin du processus n° %ld est : %s \n", msg.numProcess,msg.contenuMessage); 
		// réponse du serveur (renvoyer même message)
		if (msgsnd(reponses, & msg, strlen(msg.contenuMessage) + 1, 0) == -1) { perror("msgsnd"); }
	}

	return NULL;

}
