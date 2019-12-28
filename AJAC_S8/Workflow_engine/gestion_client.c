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





char * findStrOpt(char str[100])
{

	// La définitions de séparateurs connus.
	//const char * separators = "-";

	// On cherche à récupérer, un à un, tous les mots (token) de la phrase
	// et on commence par le premier.
	char * strToken=strtok (str, "-" );

	strToken = strtok ( NULL, "-" );

	return strToken;
}

char * findStrArg(const char *text)
{   
	const char *p1 = "<";
	const char *p2 = ">";
	char *res = NULL;
	char *start=NULL;
	char *end=NULL;


	if ( (start = strstr( text, p1 )) )
	{
		start += strlen( p1 );
		if ( (end = strstr( start, p2 )) )
		{
			res = ( char * )malloc( end - start + 1 );
			memcpy( res, start, end - start );
			res[end - start] = '\0';
		}
	}


	return res;
}


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

		if ((strncmp((const char*)activity_id,"A1",2)==0 ))
		{
			ajouterActivite (processus, (char *)activity_id,(char *)activity_name,(char *)activity_description, (char *)activity_performer,"NULL","NULL","RUNNING");
		}
		else
		{
			ajouterActivite (processus, (char *)activity_id,(char *)activity_name,(char *)activity_description, (char *)activity_performer,"NULL","NULL","NOT STARTED");
		}



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


void fct_Todo(FILE *file_dialogue, char user[LONG_ID])
{
	char buffer[SIZE_BUFFER];
	fputs("Bienvenue dans le mode To-Do, saisir exit pour en sortir\n",file_dialogue);
	while(fgets(buffer,MAX_LIGNE,file_dialogue)!=NULL)
	{
		if(strncmp(buffer,"exit",4) == 0)
		{
			break;
		}
		if(strncmp(buffer,"ls",2) == 0)
		{
			char* option=findStrOpt(buffer);   
			fct_listActivities(file_dialogue,debutListProcess,option,user);
		}
		if (strncmp(buffer,"valider",7) == 0)
		{
			fct_valider(file_dialogue,debutListProcess, user);
		}

	}
	fputs("Vous êtes sorti du mode To-Do\n",file_dialogue);

}



void fct_listProcesses(FILE *file_dialogue,Process *processCourant, char *option)
{
#ifdef DEBUG
	printf("Process courant: %p\n", processCourant);
#endif
	int nbProcess=0;

	while (processCourant != NULL) {
		nbProcess++;
		if (option == NULL)
		{
			fprintf (file_dialogue,"Process \t[Id:]  %s\t[Etat:] %s\n", processCourant->id, processCourant->etat);   
		}
		else if(strcmp(processCourant->etat,option) == 0)
		{
			fprintf (file_dialogue,"Process \t[Id:]  %s\t[Etat:] %s\n", processCourant->id, processCourant->etat); 
		}
		else 
		{
			fprintf (file_dialogue,"Il n'y a aucun processus avec l'etat:  %s\n", option); 
		}
		fprintf (file_dialogue,"Il y'a au total %d processus:\n", nbProcess);


		processCourant = processCourant->next;
	}
}


void fct_printProcess(FILE *file_dialogue,Process *processCourant,char* arg)
{
#ifdef DEBUG
	printf("Process courant: %p\n", processCourant);
#endif
	int nbProcess=0;

	while (processCourant != NULL) {
		nbProcess++;
		if(strcmp(processCourant->id,arg) == 0)
		{
			fprintf (file_dialogue,"Process \t[Id:] %s\t[Desc:] %s\t[Etat:] %s contient les activites suivantes:\n", processCourant->id, processCourant->description, processCourant->etat);

			Activity *activiteCourante = processCourant->debutListActivity;
			while (activiteCourante != NULL) {
				fprintf (file_dialogue,"\n\t[Id:] %s\n\t[Name:] %s\n\t[Descr:] %s\n\t[Perf:] %s\n\t[Entree:] %s\n\t[Sortie:] %s\n\t[Etat:] %s\n\n", activiteCourante->id,activiteCourante->name,activiteCourante->description,activiteCourante->performer,activiteCourante->input,activiteCourante->output,activiteCourante->etat);
				activiteCourante = activiteCourante->next;
			}
		}
		else 
		{
			fprintf (file_dialogue,"Il n'y a aucun processus avec l'id:  %s\n", arg); 
		}

		processCourant = processCourant->next;
	}

}


void fct_listActivities(FILE *file_dialogue,Process *processCourant,char* option, char user[LONG_ID])
{
#ifdef DEBUG
	printf("Process courant: %p\n", processCourant);
#endif
	int nbProcess=0;

	while (processCourant != NULL) {
		nbProcess++;


		if (option == NULL)
		{


			Activity *activiteCourante = processCourant->debutListActivity;
			while (activiteCourante != NULL) {
				if(strncmp(activiteCourante->performer,user,sizeof(activiteCourante->performer)) == 0)
				{
					fprintf (file_dialogue,"Process \t[Id:] %s\n", processCourant->id);
					fprintf (file_dialogue,"Activitee assignee\n\t[Id:] %s\n\t[Name:] %s\n", activiteCourante->id,activiteCourante->name);
				}
				activiteCourante = activiteCourante->next;

			}

		}
		else
		{
			char opt[LONG_ID];
			strncpy(opt,(const char *) option,2);
			Activity *activiteCourante = processCourant->debutListActivity;
			while (activiteCourante != NULL) {

				if(strncmp(activiteCourante->performer,user,sizeof(activiteCourante->performer)) == 0)
				{
					if(strncmp(activiteCourante->id,opt,2) == 0) 
					{
						fprintf (file_dialogue,"Process \t[Id:] %s \n", processCourant->id);
						fprintf (file_dialogue,"Activitee associee [Id:] %s details: \n\t[Name:] %s\n\t[Descr:] %s\n\t[Perf:] %s\n\t[Entree:] %s\n\t[Sortie:] %s\n\t[Etat:] %s\n\n", activiteCourante->id,activiteCourante->name,activiteCourante->description,activiteCourante->performer,activiteCourante->input,activiteCourante->output,activiteCourante->etat);
					}
				}

				activiteCourante = activiteCourante->next;
			} 
		}



		processCourant = processCourant->next;
	}

}


void fct_valider(FILE *file_dialogue,Process *processCourant, char user[LONG_ID])
{
	char procToValid[LONG_ID];
	fprintf (file_dialogue,"Veuillez saisir  l'ID du process concerne\n");
	fgets(procToValid,LONG_ID,file_dialogue);
	fprintf (file_dialogue,"choix: %s\n", procToValid);

	int nbProcess=0;

	while (processCourant != NULL) {
		nbProcess++;
		if(strncmp(processCourant->id,procToValid,1) == 0)
		{
			char actToValid[LONG_ID];
			//bool validation=true; // gérer es 2 différents cas plus tard
			fprintf (file_dialogue,"Veuillez saisir l'ID de l'activite concernee\n");
			fgets(actToValid,LONG_ID,file_dialogue);
			fprintf (file_dialogue,"choix: %s\n", actToValid);

			Activity *activiteCourante = processCourant->debutListActivity ;

			while (activiteCourante != NULL) {
				if(strncmp(activiteCourante->id,actToValid,2) == 0)
				{
					if(strncmp(activiteCourante->performer,user,sizeof(activiteCourante->performer)) == 0)
					{

                            if(strncmp(activiteCourante->etat,"RUNNING",7) == 0)
							{
								strcpy (activiteCourante->etat, "COMPLETED");
                                fprintf (file_dialogue,"Activite validee avec succes\n");
						if(activiteCourante->next !=NULL) 
						{

							// GERER ACTIVITES 2 ET 3 AVEC VARIABLE BOOLEENE VALIDATON
							Activity *next;
							next=activiteCourante->next;
							strcpy (next->etat, "RUNNING");
						}
						else // activite A7 (fin du process)
						{
							strcpy (processCourant->etat, "COMPLETED");
						}
							}
							else
							{
								fprintf (file_dialogue,"L'activite %s ne peut pas etre validee car la transition n'est pas franchie\n", activiteCourante->id);
							}

					}
					else
					{
						fprintf (file_dialogue,"L'activite %s n'est pas assignee a %s\n",activiteCourante->id,user);
					}

				}

				activiteCourante->prev = activiteCourante;
				activiteCourante = activiteCourante->next;
			}
	
		}
		processCourant = processCourant->next;
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
	struct ConnexionInfos Infos = {false,"",""};
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

	strcpy(Infos.login, login);
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

void afficherConnList(char connectedUsers[MAX_UTILISATEURS][LONG_ID],FILE *file_dialogue)
{
	fprintf(file_dialogue,"Les utilisateurs connectes sont: \n");
	for (int i=0; i < MAX_UTILISATEURS; i++)
	{
		if (strcmp(connectedUsers[i], "") != 0)
		{
			fprintf(file_dialogue,"- %d°) %s\n",(i+1),connectedUsers[i]);
		}

	}
}


void* gestionClient(void *dialogue)
{

	bool enable_instancProc=true; // sécurité (faut se deco puis reco pour instancier plusieurs processus)
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
		strtok(buffer, "\n");

		printf("Message de l'utilisateur %s: %s\n",Connexion.connectedUser, buffer);
		if(strncmp(buffer,"exit",4) == 0)
		{
			break;
		}
		if(strcmp(buffer,"aide") == 0)
		{
			fct_aide(file_dialogue);
		}
		if(strcmp(buffer,"ToDo") == 0)
		{
			fct_Todo(file_dialogue,Connexion.login);
		}
		if(strcmp(buffer,"connected users") == 0)
		{
			afficherConnList(connectedUsers,file_dialogue);
		}
		if(strncmp(buffer,"ls process <",12) == 0)
		{
			char *acc_ouvr=strchr(buffer,'<');
			char *acc_ferm=strchr(buffer,'>');
#ifdef DEBUG
			printf("ouvr: %s\n", acc_ouvr);
			printf("ferm: %s\n", acc_ferm);
			printf("diff: %ld\n", acc_ferm-acc_ouvr);
#endif 
			if((acc_ouvr!=NULL)&&(acc_ferm!=NULL)&&(acc_ferm>acc_ouvr))
			{
				char* arg= findStrArg((const char *)buffer);
				fct_printProcess(file_dialogue,debutListProcess,arg);
			}
		}
		if(strncmp(buffer,"ls processes",12) == 0)
		{
			char* option=findStrOpt(buffer);           ;
			fct_listProcesses(file_dialogue,debutListProcess,option);
		}
		if(strncmp(buffer,"create process ",15) == 0)
		{
			if(enable_instancProc)
			{
				char *acc_ouvr=strchr(buffer,'<');
				char *acc_ferm=strchr(buffer,'>');
#ifdef DEBUG
				printf("ouvr: %s\n", acc_ouvr);
				printf("ferm: %s\n", acc_ferm);
				printf("diff: %ld\n", acc_ferm-acc_ouvr);
#endif 
				if((acc_ouvr!=NULL)&&(acc_ferm!=NULL)&&(acc_ferm>acc_ouvr))
				{
					char* file=findStrArg((const char *)buffer);
#ifdef DEBUG
					printf("%s\n",file);
#endif
					instancierProcessus (&debutListProcess,file);
					fputs("Instanciation de processus effectuee avec succes\n",file_dialogue);
					enable_instancProc=false; // sécurité (faut se deco puis reco pour instancier plusieurs processus)
				}
			}
			else
			{
				fputs("Veuillez vous reconnecter pour instancier à nouveau un processus\n",file_dialogue);
			}
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



void ajouterActivite (Process *process, char *id, char *name, char *description, char *performer,char *input, char *output, char *etat) {

	Activity *activite = (Activity *)malloc (sizeof (Activity));



	if(isEmpty(process))
	{
		activite->next = NULL;
		process->debutListActivity= activite;
		strcpy (activite->id, id);
		strcpy (activite->name, name);
		strcpy (activite->description, description);
		strcpy (activite->performer, performer);
		strcpy (activite->input, input);
		strcpy (activite->output, output);
		strcpy (activite->etat, etat);
	}
	else
	{
		Activity *ptr=process->debutListActivity;//recent
		while (ptr->next != NULL)
		{
			ptr = ptr->next;
		}
		strcpy (activite->id, id);
		strcpy (activite->name, name);
		strcpy (activite->description, description);
		strcpy (activite->performer, performer);
		strcpy (activite->input, input);
		strcpy (activite->output, output);
		strcpy (activite->etat, etat);

		activite->next = NULL;  
		ptr->next = activite;
	}







}


void instancierProcessus (Process **debut,char* file) {

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
	char fichier [SIZE]= "../Models/";
	strcat(fichier,file);

	if( access( fichier, F_OK ) != -1 ) // pour vérifier si le fichier existe
	{

		if (xmlSAXUserParseFile (&handler, NULL,  (const char *)fichier) < 0) { perror ("oups parser"); }


		processus->validation=false;
		sprintf (processus->id,"%d",nbProcess); 
		strcpy (processus->description, (char *)process_description);
		strcpy (processus->etat, "RUNNING");
		processus->next = *debut;     //  On ajoute au debut (plus simple)
		*debut = processus;

		printf("Le nombre de processus est: %d\n", nbProcess);
	}
	else 
	{
		printf("Le fichier %s n'existe pas \n", fichier);        
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

int isEmpty (Process *process)
{
	return (process->debutListActivity==NULL);
}
