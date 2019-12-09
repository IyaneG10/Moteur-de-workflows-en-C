#ifndef __GESTIONCLIENT_H__
#define __GESTIONCLIENT_H__

#define MAX_LIGNE 2048
#define SIZE_BUFFER 1024
#define MAX_UTILISATEURS 10
#define LONG_ID 100
#define SIZE_NOM_MACHINE 400

extern char usersFile[100]; // déclaré dans main
extern char connectedUsers[MAX_UTILISATEURS][LONG_ID]; // déclaré dans main

struct ConnexionInfos
{
	bool validity; // true: connexion réussie
	char connectedUser [LONG_ID]; // Nom et Prénom de l'utilisateur connecté
};

void supprConnList(char connectedUsers[MAX_UTILISATEURS][LONG_ID],struct ConnexionInfos Connexion); 

void afficherConnList(char connectedUsers[MAX_UTILISATEURS][LONG_ID]);

void ajouterConnList(char connectedUsers[MAX_UTILISATEURS][LONG_ID],struct ConnexionInfos Connexion);

char * adresseClient(long socket);

void* gestionClient(void *s);

int parserBDDUsers(FILE *fp, char usersBDD[MAX_UTILISATEURS][3][LONG_ID]);

struct ConnexionInfos authentificationClient(FILE *file_dialogue, char usersBDD[MAX_UTILISATEURS][3][LONG_ID]);


#endif
