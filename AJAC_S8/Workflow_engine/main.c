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

char usersFile[100]; // en variable globale car partagée par tous les processus (sans modification)
char connectedUsers[MAX_UTILISATEURS][LONG_ID];



int main(int argc, char *argv[])
{
    struct OptionArg arg= check_arguments(argc,argv);
	printf("Le port utilisé est: %s\n", arg.portNum);
	strcpy(usersFile, arg.File);
	printf("Le userfile utilisé est: %s\n", usersFile);

	debutListProcess = NULL;

	//Thread de gestion de la file de message pour les requetes admin
	lanceClientLeger(NULL, gestion_file_message); 

	/* Initialisation du serveur */	    
	long socket_ecoute=initialisationServeur(arg.portNum);

	/* Lancement de la boucle d'ecoute */
	boucleServeur(socket_ecoute,lanceClientLeger,gestionClient);
	printf("Au revoir !!!\n");
    
 
    
	return 0;
}







