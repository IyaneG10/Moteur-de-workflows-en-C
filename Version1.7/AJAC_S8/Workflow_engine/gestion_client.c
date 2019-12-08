/** fichier gestion_client.c **/

/*****************************************************************/
/** Ce fichier contient des fonctions utilisees pour la gestion des clients qui se connectent au serveur  **/
/**                                                        **/
/*****************************************************************/

/**** Fichiers d'inclusion ****/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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


#include "gestion_client.h"


char * adresseClient(long socket)
{
	char * nomMachine = malloc(SIZE_NOM_MACHINE);
	struct sockaddr_storage adresse;
	struct sockaddr *padresse=(struct sockaddr *)&adresse;
	socklen_t taille=sizeof adresse;
	int statut;
	struct hostent *machine;

	/* Recupere l'adresse de la socket distante */
	statut=getpeername(socket,padresse,&taille);
	if(statut<0){
		perror("adresseClient.getpeername");
		exit(-1);
	}

	/* Recupere le nom de la machine */
	void *ip;
	int taille_ip;
	int taille_nom;
	if(padresse->sa_family==AF_UNSPEC){
		struct sockaddr_in *ipv4=(struct sockaddr_in *)padresse;
		ip=(void *)&ipv4->sin_addr;
		taille_ip=sizeof(ipv4->sin_addr);
		taille_nom=INET_ADDRSTRLEN;
	}

	if(padresse->sa_family==AF_INET6){
		struct sockaddr_in6 *ipv6=(struct sockaddr_in6 *)padresse;
		ip=(void *)&ipv6->sin6_addr;
		taille_ip=sizeof(ipv6->sin6_addr);
		taille_nom=INET6_ADDRSTRLEN;
	}

	machine=gethostbyaddr(ip,taille_ip,padresse->sa_family);

	if(machine==NULL){
		inet_ntop(padresse->sa_family,ip,nomMachine,taille_nom);
		exit(-1);
	}
	else{
		strncpy(nomMachine,machine->h_name,LONG_ID);
	}

	return nomMachine;
}


int parserBDDUsers(FILE *fp_UsersFile, char users[MAX_UTILISATEURS][3][LONG_ID])
{
	char ligne[LONG_ID];
	char lecture='x';
	int nbLectures=0;
	int usersFound = 0;
	char username[LONG_ID],  password[LONG_ID],  utilisateur[LONG_ID];

	while(usersFound < MAX_UTILISATEURS && lecture != EOF)
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
			char * tmp = strtok(ligne, ":");
			if(tmp != NULL){strcpy(username, tmp);}

			tmp = strtok(NULL, ":");
			if(tmp != NULL) {strcpy(password, tmp);}

			tmp = strtok(NULL, "");
			if(tmp != NULL) {strcpy(utilisateur, tmp);}

			strcpy(users[usersFound][0], username);
			strcpy(users[usersFound][1], password);
			strcpy(users[usersFound][2], utilisateur);
			usersFound++;
		}
	}

	return usersFound;

}


struct ConnexionInfos authentificationClient(FILE *file_dialogue,  char users[MAX_UTILISATEURS][3][LONG_ID])
{
	struct ConnexionInfos Infos = {false,""};
	char *pos_log, *pos_pwd; // pour enlever le CR du au fgets
	char identifiantsBDD[LONG_ID*2];
	char identifiantsSaisis[2*LONG_ID];
	char login[LONG_ID];
	char password[LONG_ID];
	fputs("Veuillez saisir votre login\n",file_dialogue);
	fgets(login,LONG_ID,file_dialogue);
	if ((pos_log=strchr(login, '\n')) != NULL)
		*pos_log = '\0';
	else {} // par protection
	fputs("Veuillez saisir votre mot de passe\n",file_dialogue);
	fgets(password,LONG_ID,file_dialogue);
	if ((pos_pwd=strchr(password, '\n')) != NULL)
		*pos_pwd = '\0';
	else {} // par protection

	strcpy(identifiantsSaisis, login);
	strcat(identifiantsSaisis,":");
	strcat(identifiantsSaisis, password); // couple login:password
#ifdef DEBUG
	printf("L'utilisateur a renseigné les infos suivantes [login:pwd] = %s\n", identifiantsSaisis);
#endif

	for(int i=0; i<MAX_UTILISATEURS; i++)
	{
		strcpy(identifiantsBDD, users[i][0]);
		strcat(identifiantsBDD,":");
		strcat(identifiantsBDD, users[i][1]); // couple login:password
#ifdef DEBUG
		printf("Selon la base de données,  [login:pwd] = %s\n", identifiantsSaisis);
#endif
		if(strcmp(identifiantsBDD, identifiantsSaisis) == false) // correspondance entre login et password
		{   Infos.validity=true;
			strcpy(Infos.connectedUser, users[i][2]);
			return Infos;
		}
	}

	return Infos;
}


void supprConnList(char connectedUsers[MAX_UTILISATEURS][LONG_ID],struct ConnexionInfos Connexion)
{
	int index_connected_user=0;
	for (index_connected_user=0; index_connected_user < MAX_UTILISATEURS; index_connected_user++)
	{
		if (strcmp(connectedUsers[index_connected_user],Connexion.connectedUser ) == 0)
		{
			strcpy(connectedUsers[index_connected_user], "");
			break;			
		}

	}
}

void ajouterConnList(char connectedUsers[MAX_UTILISATEURS][LONG_ID],struct ConnexionInfos Connexion)
{
	int index_connected_user=0;
	bool ajouter=true;
	for (index_connected_user=0; index_connected_user < MAX_UTILISATEURS; index_connected_user++)
	{
		if (strcmp(connectedUsers[index_connected_user],Connexion.connectedUser ) == 0 )
		{
			ajouter=false;
			break;		
		}

	}
	if(ajouter)
	{
		for (index_connected_user=0; index_connected_user < MAX_UTILISATEURS; index_connected_user++)
		{
			if(strcmp(connectedUsers[index_connected_user], "") == 0)
			{
				strcpy(connectedUsers[index_connected_user], Connexion.connectedUser);
				break;
			}	

		}
	}

}

void afficherConnList(char connectedUsers[MAX_UTILISATEURS][LONG_ID])
{
	printf("Les utilisateurs connectés sont: \n");
	for (int i=0; i < MAX_UTILISATEURS; i++)
	{
		if (strcmp(connectedUsers[i], "") != 0)
		{
			printf("- %d°) %s\n",(i+1),connectedUsers[i]);
		}

	}
}


void* gestionClient(void *dialogue)
{
	FILE *file_dialogue=fdopen((long)dialogue,"a+"); 
	if(file_dialogue==NULL){ perror("gestionClient.fdopen"); exit(EXIT_FAILURE); }
	char * nomMachine = malloc(SIZE_NOM_MACHINE);
	char buffer[SIZE_BUFFER];
	char usersBDD[MAX_UTILISATEURS][3][LONG_ID]; // Base De Données matrice pour stocker: [0]=login, [1]=pwd, [2]=prenom, [3]=nom
	FILE * fp_fichierUtilisateurs = fopen(usersFile, "r");
	if (fp_fichierUtilisateurs == NULL)
	{
		fprintf(stderr, "Impossible d'ouvrir le fichier\n");
	}
	if ( parserBDDUsers(fp_fichierUtilisateurs, usersBDD) <= 0)
	{
		fprintf(stderr, "Impossible de parser le fichier de base de données\n");
	}

	struct ConnexionInfos Connexion = {false,""};
	Connexion = authentificationClient(file_dialogue, usersBDD);

	if(Connexion.validity)
	{
		nomMachine = adresseClient((long)dialogue); // récupération de l'adresse de la machine connectée
		printf("L'utilisateur %s est connecté avec la machine: %s\n", Connexion.connectedUser, nomMachine);
		fputs("Connexion reussie\n",file_dialogue);

		ajouterConnList( connectedUsers,Connexion);
		afficherConnList(connectedUsers);


	}
	else
	{
		fputs("ERREUR: Mot de passe ou login incorrect!\n",file_dialogue);
		fputs("ERREUR: Retentez la connexion\n",file_dialogue);
		printf("Tentative de connexion refusee\n");
		fclose(file_dialogue);
		return 0;
	}


	while(fgets(buffer,MAX_LIGNE,file_dialogue)!=NULL)
	{
		printf("Message de l'utilisateur %s: %s",Connexion.connectedUser, buffer);
		if(strncmp(buffer,"exit",4) == 0)
		{
			fclose(file_dialogue);
			free(nomMachine);
			break;
		}
	}


	printf("L'utilisateur %s est déconnecté\n", Connexion.connectedUser);
	supprConnList( connectedUsers,Connexion);
	fclose(fp_fichierUtilisateurs);
	return 0;

}
