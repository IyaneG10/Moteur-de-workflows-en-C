#ifndef __GESTIONCLIENT_H__
#define __GESTIONCLIENT_H__

#define MAX_LIGNE 2048
#define SIZE_BUFFER 1024
#define MAX_UTILISATEURS 10
#define LONG_ID 100
#define SIZE_NOM_MACHINE 400


#define SIZE 100
#define ETAT_NS NOT STARTED 
#define ETAT_RS RUNNING
#define ETAT_CP COMPLETED

extern char usersFile[100]; // déclaré dans main
extern char connectedUsers[MAX_UTILISATEURS][LONG_ID]; // déclaré dans main


typedef struct Activity {
    char id [SIZE];
    char name[SIZE];
    char description[SIZE];
    char performer[SIZE];
    char input[SIZE]; //  NULL s'il n'y en a pas
    char output[SIZE]; // NULL s'il n'y en a pas
    char etat [SIZE]; // NOT STARTED, RUNNING, COMPLETED (voir transitions)
     struct Activity *next;
    struct Activity *prev; // pour pouvoir verifier si l'activite precedente est validee (condition de validation) + etat=RUNNING
} Activity; 


typedef struct Process {
    char id [SIZE];
    char description [SIZE];
    char etat [SIZE]; // RUNNING, COMPLETED
    Activity *debutListActivity;
    bool validation;
    struct Process *next;
} Process;



struct ConnexionInfos
{
	bool validity; // true: connexion réussie
	char connectedUser [LONG_ID]; // Nom et Prénom de l'utilisateur connecté
};

void fct_aide(FILE *file_dialogue);
void fct_Todo(FILE *file_dialogue);
void fct_connectedUsers(FILE *file_dialogue);

void supprConnList(char connectedUsers[MAX_UTILISATEURS][LONG_ID],struct ConnexionInfos Connexion); 

void afficherConnList(char connectedUsers[MAX_UTILISATEURS][LONG_ID]);

void ajouterConnList(char connectedUsers[MAX_UTILISATEURS][LONG_ID],struct ConnexionInfos Connexion);

char * adresseClient(long socket);

void* gestionClient(void *s);

int parserBDDUsers(FILE *fp, char usersBDD[MAX_UTILISATEURS][3][LONG_ID]);

struct ConnexionInfos authentificationClient(FILE *file_dialogue, char usersBDD[MAX_UTILISATEURS][3][LONG_ID]);



void ajouterActivite (Process *debut, char *id, char *name, char *description, char *performer,char *input, char *output, char *etat) ;

void instancierProcessus (Process **debut, char *id, char *description, char *etat);

void afficherInfos (Process *processCourant) ;



#endif
