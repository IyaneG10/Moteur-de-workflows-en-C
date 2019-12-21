#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100
#define ETAT_NS NOT STARTED 
#define ETAT_RS RUNNING
#define ETAT_CP COMPLETED

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
    struct Process *next;
} Process;



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


int main (void) {
    
    Process *debutListProcess = NULL;
    
    instancierProcessus (&debutListProcess, "1", "Demande de stage Malick", "RUNNING");
    instancierProcessus (&debutListProcess, "2", "Demande de stage Iyane", "RUNNING");
   
    afficherInfos (debutListProcess);
    
    return 0;
}
