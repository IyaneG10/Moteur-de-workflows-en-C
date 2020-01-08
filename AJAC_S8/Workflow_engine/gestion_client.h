#ifndef __GESTIONCLIENT_H__
#define __GESTIONCLIENT_H__

#include "workflow_struct.h"
#include "traitement_text.h"

#define MAX_LIGNE 2048
#define SIZE_BUFFER 1024
#define SIZE_NOM_MACHINE 400



extern char usersFile[100]; // declare dans main
extern char connectedUsers[MAX_UTILISATEURS][LONG_ID]; // declare dans main


struct ConnexionInfos
{
	bool validity; // true: connexion reussie
	char connectedUser [LONG_ID]; // Nom et Prenom de l'utilisateur connecte
	char login [LONG_ID];
};





void fct_aide(FILE *file_dialogue);
void fct_Todo(FILE *file_dialogue,char user[LONG_ID]);
void fct_listProcesses(FILE *file_dialogue,Process *processCourant,char *option);
void fct_printProcess(FILE *file_dialogue,Process *processCourant,char* arg);
void fct_listActivities(FILE *file_dialogue,Process *processCourant,char* arg,char user[LONG_ID]);
void fct_valider(FILE *file_dialogue,Process *processCourant, char user[LONG_ID]);

void supprConnList(char connectedUsers[MAX_UTILISATEURS][LONG_ID],struct ConnexionInfos Connexion); 
void afficherConnList(char connectedUsers[MAX_UTILISATEURS][LONG_ID],FILE *file_dialogue);
void ajouterConnList(char connectedUsers[MAX_UTILISATEURS][LONG_ID],struct ConnexionInfos Connexion);


char * getAdresseClient(long socket);

void* gestionClient(void *s);
struct ConnexionInfos authentificationClient(FILE *file_dialogue, char usersBDD[MAX_UTILISATEURS][3][LONG_ID]);



#endif
