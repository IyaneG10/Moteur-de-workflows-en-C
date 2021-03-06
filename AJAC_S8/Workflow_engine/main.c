
/**
 * @file main.c
 * @author SECK Malick 
 * @brief 
 * Programme du serveur
 * @version 0.1
 * @date 2020-01-25
 * 
 * @copyright Copyright (c) 2020
 * 
 */
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
#include "workflow_struct.h"
#include "libsck.h"
#include "libthrd.h"
#include "libipc.h"

/**
 * @brief pour la liste des processus (partagé par plusieurs fichiers)
 * 
 */
Process *debutListProcess = NULL;
Process *processus = NULL;

char usersFile[100]; // en variable globale car partagée par tous les processus (sans modification)
char connectedUsers[MAX_UTILISATEURS][LONG_ID];
int flag_connected;

/**
 * @brief 
 * Programme principal
 * 
 * 
 * @param argc 
 * @param argv : -p <numPort> -u <fichier>
 * @return int : return 0 si tout se passe bien
 */
int main(int argc, char *argv[])
{
	struct OptionArg arg = check_arguments(argc, argv);
	printf("Le port utilisé est: %s\n", arg.portNum);
	strcpy(usersFile, arg.File);
	printf("Le userfile utilisé est: %s\n", usersFile);

	flag_connected = 0;

	debutListProcess = NULL;


	/**
	 * @fn lanceClientLeger(NULL, gestion_file_message) 
	 * @brief Thread de gestion de la file de message pour les requetes admin
	 * 
	 */
	lanceClientLeger(NULL, gestion_file_message);


	/**
	 * @fn long socket_ecoute = initialisationServeur(arg.portNum)
	 * 
	 * @brief Fonction d'initialisation du serveur TCP
	 * 
	 */
	long socket_ecoute = initialisationServeur(arg.portNum);



	/**
	 * @brief Fonction boucle serveur qui lance un thread de gestionpour chaque client
	 * 
	 */


	boucleServeur(socket_ecoute, lanceClientLeger, gestionClient);

	printf("Au revoir !!!\n");
	return 0;
}
