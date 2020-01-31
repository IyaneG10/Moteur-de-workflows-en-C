/**
 * @file workflow_struct.h
 * @author SECK Malick 
 * @brief 
 * @version 0.1
 * @date 2020-01-30
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef __WORKFLOWSTRUCT_H__
#define __WORKFLOWSTRUCT_H__

#define SIZE 100

/**
 * @brief Structure de la liste chainee pour les activité
 * 
 */
typedef struct Activity {
	struct Activity *prev; // pour pouvoir verifier si l'activite precedente est validee (condition de validation) + etat=RUNNING
	char id [SIZE];
	char name[SIZE];
	char description[SIZE];
	char performer[SIZE];
	char input[SIZE]; //  NULL s'il n'y en a pas
	char output[SIZE]; // NULL s'il n'y en a pas
	char etat [SIZE]; // NOT STARTED, RUNNING, COMPLETED (voir transitions)
	struct Activity *next;

} Activity; 

/**
 * @brief Structure de la liste chainee de processus (incluant une liste d'activités associées)
 * 
 */
typedef struct Process {
	char id [SIZE];
	char description [SIZE];
	char etat [SIZE]; // RUNNING, COMPLETED
	Activity *debutListActivity;
	bool validation;
	struct Process *next;
} Process;

/**
 * @brief Enumération pour connaitre l'état dans lequel on se trouve pour le parsing XML en mode SAX
 * 
 */
enum {
	AUTRE,
	PROCESS,
	DESC_PROCESS,
	ACTIVITY,
	DESC_ACTIVITY,
	PERF_ACTIVITY,
	IN_ACTIVITY,
	OUT_ACTIVITY
};


void ajouterActivite (Process *debut, char *id, char *name, char *description, char *performer,char *input, char *output, char *etat) ;
int instancierProcessus (Process **debut,char* file);
int countProcesses (Process *processCourant) ;
int isEmpty (Process *process);

#endif
