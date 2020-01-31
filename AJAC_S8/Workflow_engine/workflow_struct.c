/**
 * @file workflow_struct.c
 * @author SECK Malick 
 * @brief 
 * @version 0.1
 * @date 2020-01-30
 * 
 * @copyright Copyright (c) 2020
 * 
 */

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

#include "workflow_struct.h"
#include "libthrd.h"

extern Process *debutListProcess;
extern Process *processus;

static xmlSAXHandler handler;
static int           elem_courant; // pour savoir dans quel état on se trouve
const xmlChar *  process_description;
char * description_stage; // à utiliser plus tard
bool validation;
const xmlChar *  activity_id;
const xmlChar *  activity_name;
const xmlChar *  activity_description;
const xmlChar *  activity_performer;
const xmlChar *  activity_input; 
const xmlChar *  activity_output; 
const xmlChar *  activity_etat ; // Etat initial= NOT STARTED sauf pour le premier qui est en RUNNING





/**
 * @brief Choses à faire selon l'état dans lequel la machine à état se trouve
 * 
 * @param user_data 
 * @param text 
 * @param length 
 */
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

/**
 * @brief Fonction éxécutée à la rencontre d'une balise fermante
 * 
 * @param user_data 
 * @param name 
 */
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

/**
 * @brief Fonction éxécutée à la rencontre d'une balise ouvrante
 * 
 * @param user_data 
 * @param name 
 * @param attrs 
 */
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


int instancierProcessus (Process **debut,char* file) {
	int ret=0;

	int nbProcess=1;
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
		ret = 1;      
	}
	return ret;
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
