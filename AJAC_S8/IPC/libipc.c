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
#include <regex.h>

#include "libipc.h"


extern char connectedUsers[10][100];
extern char usersFile[100];
extern struct flag flag_connexion ;
extern int flag_connected ;

int verif_format(const char *modele,const char *chaine)
{	
	int ret =1;
	regex_t regex;
	if (regcomp (&regex, modele, REG_NOSUB | REG_EXTENDED)== 0)
	{
		int validite = regexec (&regex, chaine, 0, NULL, 0);

		if (validite == 0)
		{
			ret =0;
            #ifdef DEBUG
			printf ("%s est une chaine valide\n", chaine);
            #endif
		}

		else if (validite == REG_NOMATCH)
		{
            #ifdef DEBUG
			printf ("%s n\'est pas une chaine valide\n", chaine);
            #endif
		}
	}

	regfree (&regex);
	return ret;
}

void remplacerCar(char * chaine, char ancien, char nouveau)
{
	int i = 0;

	while(chaine[i] != '\0')
	{

		if(chaine[i] == ancien)
		{
			chaine[i] = nouveau;
		}

		i=i+1;
	}
}

void listUsers(messsage_IPC msg,int commandes, int reponses)
{
    #ifdef DEBUG
    printf("Entree dans la fonction listUsers \n");
    #endif
	memset(msg.contenuMessage,0,strlen(msg.contenuMessage));
	FILE * fp_UsersFile = fopen(usersFile, "r");
	const char *modele = "^[-_[:alnum:]]+:[-_[:alnum:]]+:[_[:alpha:]]+ [_[:alpha:]]+$";
	char ligne[100];
	char lecture='x';
	int nbLectures=0;
	int usersFound = 0;
	char username[100],  utilisateur[100];

	while(usersFound < 10 && lecture != EOF)
	{
		nbLectures=0;
		lecture = fgetc(fp_UsersFile);
		ligne[0] = '\0';
		while(lecture != '\n' && lecture != EOF)
		{
			nbLectures++;
			ligne[nbLectures-1] = lecture;
			ligne[nbLectures] = '\0'; // enlever le retour chariot
			lecture = fgetc(fp_UsersFile);
		}

		if (ligne[0] != '\0')
		{
			if(verif_format(modele,ligne) == 0)
			{
				char * tmp = strtok(ligne, ":");
				if(tmp != NULL){strcpy(username, tmp);}

				tmp = strtok(NULL, ":");

				tmp = strtok(NULL, "");
				if(tmp != NULL) {strcpy(utilisateur, tmp);}

				sprintf(msg.contenuMessage, "Utilisateur: [%s] Identifiant: [%s]", utilisateur,username);
				if (msgsnd(reponses, & msg, strlen(msg.contenuMessage) + 1, 0) == -1) { perror("msgsnd"); }

			}

		}
	}

	strcpy(msg.contenuMessage, "\0");
	if (msgsnd(reponses, & msg, strlen(msg.contenuMessage) + 1, 0) == -1) { perror("msgsnd"); }

	fclose (fp_UsersFile);


}

void addUser(messsage_IPC msg,int commandes, int reponses)
{
    #ifdef DEBUG
    printf("Entree dans la fonction addUser \n");
    #endif
	bool enableAdd=true;
	const char *modele_file = "^[-_[:alnum:]]+:[-_[:alnum:]]+:[_[:alpha:]]+ [_[:alpha:]]+$";
	const char *modele_addUser = "^[-_[:alnum:]]+:[-_[:alnum:]]+:[_[:alpha:]]+-[_[:alpha:]]+$";
	if(0 == verif_format(modele_addUser,msg.contenuMessage))
	{

		FILE * fp_UsersFile = fopen(usersFile, "a+");
		char ligne[100];
		char lecture='x';
		int nbLectures=0;
		int usersFound = 0;
		char username[100];
		char  userToAdd[100];
		char  loginToAdd[100];
		strcpy(userToAdd,msg.contenuMessage);
		strcpy(loginToAdd,msg.contenuMessage);
		strtok(loginToAdd, ":");
        #ifdef DEBUG
		printf("user to add: %s\n",userToAdd);
		printf("login to add: %s\n",loginToAdd);
        #endif

		while(usersFound < 10 && lecture != EOF)
		{
			nbLectures=0;
			lecture = fgetc(fp_UsersFile);
			ligne[0] = '\0';
			while(lecture != '\n' && lecture != EOF)
			{
				nbLectures++;
				ligne[nbLectures-1] = lecture;
				ligne[nbLectures] = '\0';
				lecture = fgetc(fp_UsersFile);
			}

			if (ligne[0] != '\0')
			{
				if(verif_format(modele_file,ligne) == 0)
				{
					char * tmp = strtok(ligne, ":");
					if(tmp != NULL){strcpy(username, tmp);}

					if (strcmp(username, loginToAdd) == 0)
					{
						strcpy(msg.contenuMessage, "Login deja pris, veuillez en choisir un autre");
						if (msgsnd(reponses, & msg, strlen(msg.contenuMessage) + 1, 0) == -1) { perror("msgsnd"); }
						enableAdd=false;
						break;
					}



				}

			}
		}
		if (enableAdd)
		{
			remplacerCar((char *)userToAdd, '-',' ');

			fprintf(fp_UsersFile,"%s\n",userToAdd);
			strcpy(msg.contenuMessage, "Utilisateur ajoute avec succes");
			if (msgsnd(reponses, & msg, strlen(msg.contenuMessage) + 1, 0) == -1) { perror("msgsnd"); }
		}
		fclose (fp_UsersFile);


	}
	else
	{
		strcpy(msg.contenuMessage, "Impossible d'ajouter l'utilisateur, verifiez que le format est comme suit");
		if (msgsnd(reponses, & msg, strlen(msg.contenuMessage) + 1, 0) == -1) { perror("msgsnd"); }
		strcpy(msg.contenuMessage, "login:password:prenom-nom");
		if (msgsnd(reponses, & msg, strlen(msg.contenuMessage) + 1, 0) == -1) { perror("msgsnd"); }
	}

	strcpy(msg.contenuMessage, "\0");
	if (msgsnd(reponses, & msg, strlen(msg.contenuMessage) + 1, 0) == -1) { perror("msgsnd"); }
	memset(msg.contenuMessage,0,strlen(msg.contenuMessage));

}

void modeListen(messsage_IPC msg,int commandes, int reponses)     // Cette fonction ne marche pas correctement
{
    #ifdef DEBUG
	strcpy(msg.contenuMessage, "Bienvenue dans le mode d'ecoute");
    #endif
	if (msgsnd(reponses, & msg, strlen(msg.contenuMessage) + 1, 0) == -1) { perror("msgsnd"); }
	memset(msg.contenuMessage,0,strlen(msg.contenuMessage));

	while (1)
	{
		if (flag_connected==1)
		{
			strcpy(msg.contenuMessage, "FLAG: Un nouvel utilisateur connecte");
			if (msgsnd(reponses, & msg, strlen(msg.contenuMessage) + 1, 0) == -1) { perror("msgsnd"); }
			memset(msg.contenuMessage,0,strlen(msg.contenuMessage));
			flag_connected=0;
		}
	}

}

void printConnectedUsers(messsage_IPC msg,int commandes, int reponses)
{
    #ifdef DEBUG
    printf("Entree dans la fonction printConnectedUsers \n");
    #endif
	memset(msg.contenuMessage,0,strlen(msg.contenuMessage));
	int len= (sizeof(connectedUsers) / sizeof(connectedUsers[0]));
	int nbconnected=0;
	for (int i=0; i < len; i++)
	{
		if (strcmp(connectedUsers[i], "\0") != 0)
		{
			nbconnected++;
			strcpy(msg.contenuMessage, connectedUsers[i]);	
			if (msgsnd(reponses, & msg, strlen(msg.contenuMessage) + 1, 0) == -1) { perror("msgsnd"); }
		}  
	}
	if(nbconnected==0)
	{
		strcpy(msg.contenuMessage, "Aucun utilisateur connecte");
		if (msgsnd(reponses, & msg, strlen(msg.contenuMessage) + 1, 0) == -1) { perror("msgsnd"); }
	}
	strcpy(msg.contenuMessage, "\0");
	if (msgsnd(reponses, & msg, strlen(msg.contenuMessage) + 1, 0) == -1) { perror("msgsnd"); }

}


void* gestion_file_message(void* arg)
{
	int commandes, reponses;
	messsage_IPC msg;


	// créer une file de message pour les commandes admin (les droit R/W pour tous)
	commandes = msgget(CLE_COMMANDE, 0666 | IPC_CREAT); 
	if (commandes == -1) { perror("msgget commande");} 
	strcpy(msg.contenuMessage, "");
	printf("Contenu du message : %s \n",msg.contenuMessage); 
	// récupérer l'id de la file de réponse (crée par l'admin)
	reponses = msgget(CLE_REPONSE, 0); 
	if (reponses == -1) { perror("msgget reponse"); } 

	printf("En attente des requetes admin\n");

	while (1) {

		if (msgrcv(commandes, & msg, TAILLE_MSG, 0, 0) == -1) { perror("msgrcv"); } 

		printf("La requete admin du processus n° %i est : %s \n", msg.numProcess,msg.contenuMessage); 

		if (strncmp(msg.contenuMessage, "getConnectedUsers",17) == 0)
		{
			printConnectedUsers(msg,commandes,reponses);
			memset(msg.contenuMessage,0,strlen(msg.contenuMessage));
		}
		if (strncmp(msg.contenuMessage, "getUsers",8) == 0)
		{
			listUsers(msg,commandes,reponses);
			memset(msg.contenuMessage,0,strlen(msg.contenuMessage));
		}
		if (strncmp(msg.contenuMessage, "listen",6) == 0)
		{
			modeListen(msg,commandes,reponses);
		}
		if (strcmp(msg.contenuMessage, "\0") != 0)
		{
			addUser(msg,commandes,reponses);
		}

	}


	return NULL;

}
