/* Programme principal du moteur de workflow*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <fcntl.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <resolv.h> 
#include <pthread.h>
#include <stdbool.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 

#include "optionServer.h"
#include "gestion_client.h"
#include "libsck.h"
#include "libthrd.h"
#include "libipc.h"




Process *debutListProcess;
//int test;

char usersFile[100]; // en variable globale car partagée par tous les processus (sans modification)
char connectedUsers[MAX_UTILISATEURS][LONG_ID];

#define CLE_COMMANDE        0x00000001
#define CLE_REPONSE        0x00000002 
#define TAILLE_MSG              1024

typedef struct { 
	int numProcess;
	char contenuMessage[TAILLE_MSG]; 
} messsage_IPC; 




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

		/*while (debutListProcess != NULL) {
		printf ("Process \t[Id:] %s\t[Desc:] %s\t[Etat:] %s contient les activités suivantes:\n", debutListProcess->id, debutListProcess->description, debutListProcess->etat);
		debutListProcess = debutListProcess->next;
		}*/


		// réponse du serveur (renvoyer même message)
		if (msgsnd(reponses, & msg, strlen(msg.contenuMessage) + 1, 0) == -1) { perror("msgsnd"); }
		//msgctl(commandes, IPC_RMID, NULL);

	}


	return NULL;

}


int main(int argc, char *argv[])
{
#ifdef DEBUG
	printf("*** LE DEBOGGAGE FONCTIONNE ***\n");
#endif
	struct OptionArg arg= check_arguments(argc,argv);
	printf("Le port utilisé est: %s\n", arg.portNum);
	strcpy(usersFile, arg.File);
	printf("Le userfile utilisé est: %s\n", usersFile);

	//test=18;
	//debutListProcess = NULL;
	//instancierProcessus (&debutListProcess, "1", "Demande de stage Malick", "RUNNING");
	//instancierProcessus (&debutListProcess, "100", "Demande de stage Malick", "RUNNING");
	//afficherInfos (debutListProcess);

	//Thread de gestion de la file de message pour les requetes admin
	lanceClientLeger(NULL, gestion_file_message); 

	/* Initialisation du serveur */	    
	long socket_ecoute=initialisationServeur(arg.portNum);

	/* Lancement de la boucle d'ecoute */
	boucleServeur(socket_ecoute,lanceClientLeger,gestionClient);
	printf("Au revoir !!!\n");
    
 
    
	return 0;
}







