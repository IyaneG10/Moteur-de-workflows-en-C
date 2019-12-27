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
//#include "gestion_client.h"





void* gestion_file_message(void* arg)
{
	int commandes, reponses;
	messsage_IPC msg;


	// créer une file de message pour les commandes admin (les droit R/W pour tous)
	commandes = msgget(CLE_COMMANDE, 0666 | IPC_CREAT); 
	if (commandes == -1) { perror("msgget commande");} 
	// récupérer l'id de la file de réponse (crée par l'admin)
	reponses = msgget(CLE_REPONSE, 0); 
	if (reponses == -1) { perror("msgget reponse"); } 

	while (1) {
		printf("En attente des requetes admin\n");
		if (msgrcv(commandes, & msg, TAILLE_MSG, 0, 0) == -1) { perror("msgrcv"); } 

		printf("La requete admin du processus n° %i est : %s \n", msg.numProcess,msg.contenuMessage); 

		// réponse du serveur (renvoyer même message)
		if (msgsnd(reponses, & msg, strlen(msg.contenuMessage) + 1, 0) == -1) { perror("msgsnd"); }
		//msgctl(commandes, IPC_RMID, NULL);

	}


	return NULL;

}

