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


void fct_aide(FILE *file_dialogue)
{

    fputs("Vos possibilités sont:\n",file_dialogue);
    fputs("\tToDo : pour passer en mode gestion de la To-Do List\n",file_dialogue);
    fputs("\tconnected users : pour de connaître tous les utilisateurs connectés\n",file_dialogue);
    fputs("\tls processes [etat]: pour de lister  les processus\n",file_dialogue);
    fputs("\tls process <id-process> : pour récupérer toutes les informations associées à un processus particulier\n",file_dialogue);
    fputs("\tcreate process <file> : pour créer une nouvelle instance de processus\n",file_dialogue);
    fputs("\texit : pour quitter l’application et se déconnecter\n",file_dialogue);
}


void fct_Todo(FILE *file_dialogue)
{
    char buffer[SIZE_BUFFER];
    fputs("Bienvenue dans le mode To-Do, saisir exit pour en sortir\n",file_dialogue);
    while(fgets(buffer,MAX_LIGNE,file_dialogue)!=NULL)
    {
        if(strncmp(buffer,"exit",4) == 0)
        {
            break;
        }

    }
    fputs("Vous êtes sorti du mode To-Do\n",file_dialogue);

}


void fct_connectedUsers(FILE *file_dialogue)
{
    fputs("Les utilisateurs connectés sont: \n",file_dialogue);
    for (int i=0; i < MAX_UTILISATEURS; i++)
    {
        if (strcmp(connectedUsers[i], "") != 0)
        {
            fprintf(file_dialogue,"- %d°) %s\n",(i+1),connectedUsers[i]);
        }
        
    }

}

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
        
        //Process *debutListProcess = NULL;
        //instancierProcessus (&debutListProcess, "1", "Demande de stage Malick", "RUNNING");
        //instancierProcessus (&debutListProcess, "2", "Demande de stage Iyane", "RUNNING");
        //afficherInfos (debutListProcess);

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
            break;
		}
        if(strncmp(buffer,"aide",4) == 0)
		{
			fct_aide(file_dialogue);
		}
        if(strncmp(buffer,"ToDo",4) == 0)
		{
            fct_Todo(file_dialogue);
		}
        if(strncmp(buffer,"connected users",15) == 0)
		{
            fct_connectedUsers(file_dialogue);
		}
	}

	fputs("Vous vous êtes déconnecté\n",file_dialogue);
	printf("L'utilisateur %s est déconnecté\n", Connexion.connectedUser);
	supprConnList( connectedUsers,Connexion);
    fclose(file_dialogue);
    free(nomMachine);
	fclose(fp_fichierUtilisateurs);
	return 0;

}



void ajouterActivite (Process *debut, char *id, char *name, char *description, char *performer,char *input, char *output, char *etat) {
    
    Activity *activite = malloc (sizeof (*activite));
    strcpy (activite->id, id);
    strcpy (activite->name, name);
    strcpy (activite->description, description);
    strcpy (activite->performer, performer);
    strcpy (activite->input, input);
    strcpy (activite->output, output);
    strcpy (activite->etat, etat);
    activite->next = debut->debutListActivity;  //  On ajoute au début (plus simple)
        debut->debutListActivity = activite;
}


void instancierProcessus (Process **debut, char *id, char *description, char *etat) {
    
    
    Process *processus = malloc (sizeof (*processus));
    processus->validation=false;
    strcpy (processus->id, id);
    strcpy (processus->description, description);
    strcpy (processus->etat, etat);
    processus->next = *debut;     //  On ajoute au début (plus simple)
        *debut = processus;
        // on utilisera le parsing du fichier xml plus tard
        ajouterActivite (processus, "A7","Signature","Signature de la convention de stage par l'ecole","nd","NULL","NULL", "NOT STARTED" );
        ajouterActivite (processus,"A6","Signature","Signature de la convention de stage par l'entreprise","HAL","NULL","NULL", "NOT STARTED");
        ajouterActivite (processus, "A5","Signature","Signature de la convention de stage par l'etudiant","tmv","NULL","NULL", "NOT STARTED");
        ajouterActivite (processus, "A4","Redaction","Redaction de la convention de stage","nd","NULL","NULL", "NOT STARTED" );
        ajouterActivite (processus, "A3","Refus","Demande de stage refusee","tmv","NULL","NULL", "NOT STARTED");
        ajouterActivite (processus, "A2","Etude_demande","Etude de la demande","rex","NULL","NULL", "NOT STARTED");
        ajouterActivite (processus, "A1","Demande","Remplir la demande de stage","tmv","NULL","NULL","NOT STARTED");
        
        
}




void afficherInfos (Process *processCourant) {
    
    
    while (processCourant != NULL) {
        printf ("Process \t[Id:] %s\t[Desc:] %s\t[Etat:] %s contient les activités suivantes:\n", processCourant->id, processCourant->description, processCourant->etat);
        
        
        Activity *activiteCourante = processCourant->debutListActivity;
        
        while (activiteCourante != NULL) {
            printf ("\n\t[Id:] %s\n\t[Name:] %s\n\t[Descr:] %s\n\t[Perf:] %s\n\t[Entree:] %s\n\t[Sortie:] %s\n\t[Etat:] %s\n\n", activiteCourante->id,activiteCourante->name,activiteCourante->description,activiteCourante->performer,activiteCourante->input,activiteCourante->output,activiteCourante->etat);
            activiteCourante = activiteCourante->next;
        }
                        processCourant = processCourant->next;
    }
}

