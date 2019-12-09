#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 100
#define ETAT_NS NOT STARTED 
#define ETAT_RS RUNNING
#define ETAT_CP COMPLETED

/* Déclaration structure Activity*/
typedef struct Activity {
	char id [SIZE];
	char name[SIZE];
	char description[SIZE];
	char performer[SIZE];
	//void *input; // faire pointer sur NULL s'il n'y en a pas
	//void *output; // faire pointer sur NULL s'il n'y en a pas
	char etat [SIZE]; // NOT STARTED, RUNNING, COMPLETED
}Activity;



/* Déclaration d'une liste chainee d'activites*/
typedef struct ListActivities {
	struct ListActivities *next;
	struct ListActivities *prev;
	Activity activite;

} ListActivities;



//Déclaration structure processus (je ne considère qu'un seul process possible pour le moment)
typedef struct Process {
	char id [SIZE];
    char description [SIZE];
	char etat [SIZE]; // RUNNING, COMPLETED
	ListActivities listeActivites   ; //Liste des 7 activites 

}Process;


// Déclaration d'une liste chainee de processus
typedef struct ListProcesses {
    struct ListActivities *next;
    struct ListActivities *prev;
    Process process;
    
} ListProcesses;




int trouverFinListeP(ListProcesses *debut_liste, ListProcesses**fin_liste)
{
    int ret=0;
    ListProcesses *process_courant = debut_liste;
    
    if(process_courant)
    {
        while(process_courant->next)
            process_courant=process_courant->next; //pointer sur le processus suivant
    }
    
    	if(fin_liste)
            *fin_liste = process_courant;
        
        return ret;
}




int trouverFinListe(ListActivities *debut_liste, ListActivities**fin_liste)
{
	int ret=0;
	ListActivities *activite_courante = debut_liste;

	if(activite_courante)
	{
		while(activite_courante->next)
			activite_courante=activite_courante->next; //pointer sur l'activité suivante
	}

	if(fin_liste)
		*fin_liste = activite_courante;

	return ret;
}

int ajouterSurListeActivites(ListActivities **tete,char *id,char *name,char *description,char *performer,/*void *input,*//*void *output,*/char *etat) 
{
	int ret=0;
	ListActivities *queue;
	ListActivities *liste_des_activites = NULL;
	ret=trouverFinListe(*tete, &queue);
	 
	liste_des_activites=malloc(sizeof(*liste_des_activites));
	if(NULL == liste_des_activites) {fprintf(stderr, "malloc() failed.\n");}

	// remplir les champs de l'activité      
	sprintf(liste_des_activites->activite.id,"%s", id);
	sprintf(liste_des_activites->activite.name,"%s",  name);
	sprintf(liste_des_activites->activite.description,"%s", description);
	sprintf(liste_des_activites->activite.performer, "%s", performer);
    sprintf(liste_des_activites->activite.etat, "%s", etat);
	
	liste_des_activites->next = NULL;
	if(queue) {queue->next = liste_des_activites;}
	else {*tete = liste_des_activites;}

	return ret;
}




int afficherActivites(
		ListActivities *debut,
		char *login,
		char *id_activity
		)
{
	int ret=0;
	ListActivities *activite_courante = debut;
	int idxActivite=0;

	while(activite_courante)
	{
		++idxActivite;
		if(0 == strcmp(login,"ALL"))
		{
            printf("Activite n°%d\n\t[Id:] %s\n\t[Name:] %s\n\t[Descr:] %s\n\t[Perf:] %s\n\t[Etat:] %s\n\n",
					(idxActivite),
					activite_courante->activite.id,
					activite_courante->activite.name,
					activite_courante->activite.description,
					activite_courante->activite.performer,
                    activite_courante->activite.etat);
			activite_courante=activite_courante->next;
		}
		else
		{
            if ((0 == strcmp(activite_courante->activite.performer,login) )  && (0 == strcmp(id_activity,"ALL"))  )
			{
                printf("Activite assignée n°%d\n\t[Id:] %s\n\t[Name:] %s\n\n",
						(idxActivite),
						activite_courante->activite.id,
						activite_courante->activite.name
				      );

			}
			
           if ((0 == strcmp(activite_courante->activite.performer,login) )  && (0 == strcmp(activite_courante->activite.id,id_activity) ) )
			{
                printf("Activite avec l'Id %s\n\t[Name:] %s\n\t[Descr:] %s\n\t[Perf:] %s\n\t[Etat:] %s\n\n",
                       activite_courante->activite.id,
                       activite_courante->activite.name,
                       activite_courante->activite.description,
                       activite_courante->activite.performer,
                       activite_courante->activite.etat);
                //break;

			}
			activite_courante=activite_courante->next;

		}


	}

	return ret;
}




int instancierProcess(ListProcesses **listdebutProcesses,char *id, char *description,char *etat, char *login,char *id_activity) 
{
    
    ListActivities *listdebutActivities = NULL;
    /* Instanciation des activites pour le moment sans passer par le parsing du fichier xml*/
    
    ListProcesses *listfinProcesses;
    ListProcesses *liste_des_processus = NULL;
    trouverFinListeP(*listdebutProcesses, &listfinProcesses);
    // Allocate memory for new node (with its payload). 
    liste_des_processus=malloc(sizeof(*liste_des_processus));
    if(NULL == liste_des_processus) {fprintf(stderr, "malloc() failed.\n");}
    
    	// remplir les champs de processus    
    		sprintf(liste_des_processus->process.id,"%s", id);
            sprintf(liste_des_processus->process.description,"%s", description);
            sprintf(liste_des_processus->process.etat, "%s", etat);
            /* Link this node into the list as the new tail node. */
            liste_des_processus->next = NULL;
            if(listfinProcesses) {listfinProcesses->next = liste_des_processus;}
            else {*listdebutProcesses = liste_des_processus;}
            
                            	
           
           
    ajouterSurListeActivites(&listdebutActivities, "A1","Demande","Remplir la demande de stage","tmv"/*,NULL,NULL*/,"NOT STARTED");
    ajouterSurListeActivites(&listdebutActivities, "A2","Etude_demande","Etude de la demande","rex"/*,NULL,NULL*/, "NOT STARTED" );  
    ajouterSurListeActivites(&listdebutActivities, "A3","Refus","Demande de stage refusee","tmv"/*,NULL,NULL*/, "NOT STARTED" );
    ajouterSurListeActivites(&listdebutActivities, "A4","Redaction","Redaction de la convention de stage","nd"/*,NULL,NULL*/, "NOT STARTED" );
    ajouterSurListeActivites(&listdebutActivities, "A5","Signature","Signature de la convention de stage par l'etudiant","tmv"/*,NULL,NULL*/, "NOT STARTED" );
    ajouterSurListeActivites(&listdebutActivities, "A6","Signature","Signature de la convention de stage par l'entreprise","HAL"/*,NULL,NULL*/, "NOT STARTED" );
    ajouterSurListeActivites(&listdebutActivities, "A7","Signature","Signature de la convention de stage par l'ecole","nd"/*,NULL,NULL*/, "NOT STARTED" );
  
    
    
    afficherActivites(listdebutActivities,login,id_activity);
    
    return 0;
    
}








int afficherProcesses(
		ListProcesses *debut
		)
{
	int ret=0;
    ListProcesses *process_courant = debut;
	int idxProcess=0;

    while(process_courant)
	{
		++idxProcess;

            printf("Processus n°%d\n\t[Id:] %s\n\t[Descr:] %s\n\t[Etat:] %s\n\n",
					(idxProcess),
                   process_courant->process.id,
                   process_courant->process.description,
                   process_courant->process.etat);
            process_courant=process_courant->next;



	}

	return ret;
}



int main(int argc, char *argv[]) 
// soit pas d'arument
// soit 1 argument: login 
// soit 2 arguments:  respectivement login et passward 


{
    //Process *myProcess;
    //myProcess = (struct Process*)malloc(sizeof(struct Process));
    char* login; // pour affichage des taches associees à l'utilisateur
    char* id_activity;
    if(argc==2) {login=argv[1];id_activity="ALL";}
    else if(argc==3) {login=argv[1];id_activity=argv[2];}
    else {login="ALL";id_activity="ALL";}

    
    ListProcesses *listdebutProcesses = NULL;    
    instancierProcess(&listdebutProcesses,"1","Process 1","RUNNING", login, id_activity);  // Processus de base
    //instancierProcess(&listdebutProcesses,"2","Process 2","RUNNING", login, id_activity); 
    afficherProcesses(listdebutProcesses);



	return 0;
}

