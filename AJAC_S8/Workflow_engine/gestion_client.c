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
#include <libxml/parser.h>



#include "gestion_client.h"

extern Process *debutListProcess;
Process *processus;


char * description_stage; // à utiliser plus tard
bool validation;// à utiliser plus tard

static xmlSAXHandler handler;
static int           elem_courant; // pour savoir dans quel état on se trouve


const xmlChar *  process_description;

const xmlChar *  activity_id;
const xmlChar *  activity_name;
const xmlChar *  activity_description;
const xmlChar *  activity_performer;
const xmlChar *  activity_input; // NULL pour l'instant
const xmlChar *  activity_output; // NULL pour l'instant 
const xmlChar *  activity_etat ; // Etat initial= NOT_STARTED


void caracteres (void *user_data, const xmlChar *text, int length)
{
	if( elem_courant == DESC_PROCESS)
	{
		process_description = xmlStrndup(text, length);
        #ifdef DEBUG
		printf ("\tDescription du process: %s\n", (char *)process_description);
        #endif
	}
	
	if( elem_courant == DESC_ACTIVITY)
	{
		activity_description = xmlStrndup(text, length);
        #ifdef DEBUG
		printf ("\tDescription: %s\n", (char *)activity_description);
        #endif
	}
	
	if( elem_courant == PERF_ACTIVITY)
	{
		activity_performer = xmlStrndup(text, length);
        #ifdef DEBUG
		printf ("\tPerformer: %s\n", (char *)activity_performer);
        #endif
	}
	
	if( elem_courant == IN_ACTIVITY)
	{
		activity_input = xmlStrndup(text, length);
        #ifdef DEBUG
        printf ("\tInput: %s\n", (char *)activity_input);
        #endif
	}

	if( elem_courant == OUT_ACTIVITY)
	{
		activity_output = xmlStrndup(text, length);
        #ifdef DEBUG
		printf ("\tOutput: %s\n", (char *)activity_output);
        #endif
	}

}

void fin_element (void *user_data, const xmlChar *name)
{

	if((strcmp((const char*)name,"tns:activity")==0 )&& elem_courant == ACTIVITY)
	{
        #ifdef DEBUG
        printf ("Fin de l’activité \n");
		printf ("- %s\n", (char *)activity_id);
		printf ("- %s\n", (char *)activity_name);
		printf ("- %s\n", (char *)activity_description);
		printf ("- %s\n", (char *)activity_performer);
        #endif
		elem_courant=PROCESS;

		ajouterActivite (processus, (char *)activity_id,(char *)activity_name,(char *)activity_description, (char *)activity_performer,"NULL","NULL","NOT STARTED");


	}

	else if((strcmp((const char*)name,"tns:description")==0) && elem_courant==DESC_PROCESS)
	{
		elem_courant=PROCESS;
	}

	else if((strcmp((const char*)name,"tns:description")==0) && elem_courant==DESC_ACTIVITY)
	{
		elem_courant=ACTIVITY;
	}

	else if(strcmp((const char*)name,"tns:performer")==0)
	{
		elem_courant=ACTIVITY;
	}
	else if(strcmp((const char*)name,"tns:input")==0)
	{
		elem_courant=ACTIVITY;
	}

	else if(strcmp((const char*)name,"tns:output")==0)
	{
		elem_courant=ACTIVITY;
	}
}

void debut_element (void *user_data, const xmlChar *name, const xmlChar **attrs)
{


	if(strcmp((const char*)name,"tns:activity")==0 )

	{
		elem_courant=ACTIVITY;

		activity_id = xmlStrndup(attrs[1], sizeof(attrs[1])+1);
		activity_name = xmlStrndup(attrs[3], sizeof(attrs[3])+1);
        #ifdef DEBUG
		printf ("Debut  de l’activité %s\n", activity_id);
		printf("\tid: %s\n",(char*) activity_id);
		printf("\tname: %s\n",(char*) activity_name);
        #endif

	}
	else if(strcmp((const char*)name,"tns:description")==0 && elem_courant != ACTIVITY)
	{
		elem_courant=DESC_PROCESS;
	}
	else if(strcmp((const char*)name,"tns:description")==0 && elem_courant == ACTIVITY)
	{
		elem_courant=DESC_ACTIVITY;
	}
	else if(strcmp((const char*)name,"tns:performer")==0 && elem_courant == ACTIVITY)
	{
		elem_courant=PERF_ACTIVITY;
	}
	else if(strcmp((const char*)name,"tns:input")==0 && elem_courant == ACTIVITY)
	{
		elem_courant=IN_ACTIVITY;
	}
	else if(strcmp((const char*)name,"tns:ouput")==0 && elem_courant == ACTIVITY)
	{
		elem_courant=OUT_ACTIVITY;
	}





}



void fct_aide(FILE *file_dialogue)
{

	fputs("Vos possibilites sont:\n",file_dialogue);
	fputs("\tToDo : pour passer en mode gestion de la To-Do List\n",file_dialogue);
	fputs("\tconnected users : pour de connaître tous les utilisateurs connectes\n",file_dialogue);
	fputs("\tls processes [etat]: pour de lister  les processus\n",file_dialogue);
	fputs("\tls process <id-process> : pour recuperer toutes les informations associees à un processus particulier\n",file_dialogue);
	fputs("\tcreate process <file> : pour creer une nouvelle instance de processus\n",file_dialogue);
	fputs("\texit : pour quitter l’application et se deconnecter\n",file_dialogue);
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
	fputs("Les utilisateurs connectes sont: \n",file_dialogue);
	for (int i=0; i < MAX_UTILISATEURS; i++)
	{
		if (strcmp(connectedUsers[i], "") != 0)
		{
			fprintf(file_dialogue,"- %d°) %s\n",(i+1),connectedUsers[i]);
		}

	}

}

void fct_listProcesses(FILE *file_dialogue,Process *processCourant)
{
    #ifdef DEBUG
    printf("Process courant: %p\n", processCourant);
    #endif
	int nbProcess=0;

	while (processCourant != NULL) {
		nbProcess++;
		fprintf (file_dialogue,"Process \t[Id:] %s\t[Desc:] %s\t[Etat:] %s contient les activites suivantes:\n", processCourant->id, processCourant->description, processCourant->etat);

		Activity *activiteCourante = processCourant->debutListActivity;
		while (activiteCourante != NULL) {
			fprintf (file_dialogue,"\n\t[Id:] %s\n\t[Name:] %s\n\t[Descr:] %s\n\t[Perf:] %s\n\t[Entree:] %s\n\t[Sortie:] %s\n\t[Etat:] %s\n\n", activiteCourante->id,activiteCourante->name,activiteCourante->description,activiteCourante->performer,activiteCourante->input,activiteCourante->output,activiteCourante->etat);
			activiteCourante = activiteCourante->next;
		}

		processCourant = processCourant->next;
	}
	fprintf (file_dialogue,"Il y'a au total %d processus:\n", nbProcess);

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
	printf("L'utilisateur a renseigne les infos suivantes [login:pwd] = %s\n", identifiantsSaisis);
#endif

	for(int i=0; i<MAX_UTILISATEURS; i++)
	{
		strcpy(identifiantsBDD, users[i][0]);
		strcat(identifiantsBDD,":");
		strcat(identifiantsBDD, users[i][1]); // couple login:password
#ifdef DEBUG
		printf("Selon la base de donnees,  [login:pwd] = %s\n", identifiantsSaisis);
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
	printf("Les utilisateurs connectes sont: \n");
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
	char usersBDD[MAX_UTILISATEURS][3][LONG_ID]; // Base De Donnees matrice pour stocker: [0]=login, [1]=pwd, [2]=prenom, [3]=nom
	FILE * fp_fichierUtilisateurs = fopen(usersFile, "r");
	if (fp_fichierUtilisateurs == NULL)
	{
		fprintf(stderr, "Impossible d'ouvrir le fichier\n");
	}
	if ( parserBDDUsers(fp_fichierUtilisateurs, usersBDD) <= 0)
	{
		fprintf(stderr, "Impossible de parser le fichier de base de donnees\n");
	}

	struct ConnexionInfos Connexion = {false,""};
	Connexion = authentificationClient(file_dialogue, usersBDD);

	if(Connexion.validity)
	{
		nomMachine = adresseClient((long)dialogue); // recuperation de l'adresse de la machine connectee
		printf("L'utilisateur %s est connecte avec la machine: %s\n", Connexion.connectedUser, nomMachine);
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
		if(strncmp(buffer,"ls processes",12) == 0)
		{
			fct_listProcesses(file_dialogue,debutListProcess);
		}
		if(strncmp(buffer,"create process",13) == 0)
		{
			instancierProcessus (&debutListProcess, "1", "Demande de stage Malick", "RUNNING");
			fputs("Instanciation de processus effectuee avec succes\n",file_dialogue);
		}

	}

	fputs("Vous vous êtes deconnecte\n",file_dialogue);
	printf("L'utilisateur %s est deconnecte\n", Connexion.connectedUser);
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
	activite->next = debut->debutListActivity;  //  On ajoute au debut (plus simple)
	debut->debutListActivity = activite;

}


void instancierProcessus (Process **debut, char *id, char *description, char *etat) {

	int nbProcess=1;
    #ifdef DEBUG
	printf("debut : %p\n",debut);
	printf("debutListProcess: %p\n",debutListProcess);
	printf("processus: %p\n",processus);
    #endif
	processus = malloc (sizeof (*processus));

	nbProcess=  countProcesses (debutListProcess);
	nbProcess++;

	elem_courant=AUTRE;
	handler.startElement = debut_element;
	handler.endElement   = fin_element;
	handler.characters   = caracteres;
	if (xmlSAXUserParseFile (&handler, NULL, "../Models/process1.xml") < 0) { perror ("oups parser"); }


	processus->validation=false;
	sprintf (processus->id,"%d",nbProcess); 
	strcpy (processus->description, (char *)process_description);
	strcpy (processus->etat, etat);
	processus->next = *debut;     //  On ajoute au debut (plus simple)
	*debut = processus;

	printf("Le nombre de processus est: %d\n", nbProcess);

}




void afficherInfos (Process *processCourant) {

	while (processCourant != NULL) {

		printf ("Process \t[Id:] %s\t[Desc:] %s\t[Etat:] %s contient les activites suivantes:\n", processCourant->id, processCourant->description, processCourant->etat);


		Activity *activiteCourante = processCourant->debutListActivity;

		while (activiteCourante != NULL) {
			printf ("\n\t[Id:] %s\n\t[Name:] %s\n\t[Descr:] %s\n\t[Perf:] %s\n\t[Entree:] %s\n\t[Sortie:] %s\n\t[Etat:] %s\n\n", activiteCourante->id,activiteCourante->name,activiteCourante->description,activiteCourante->performer,activiteCourante->input,activiteCourante->output,activiteCourante->etat);
			activiteCourante = activiteCourante->next;
		}
		processCourant = processCourant->next;
	}
}


int countProcesses (Process *processCourant) {
	int nbProcess=0;
	while (processCourant != NULL) {

		nbProcess++;
		processCourant = processCourant->next;
	}

	return nbProcess;
}

