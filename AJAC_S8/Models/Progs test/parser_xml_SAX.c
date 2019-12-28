#include <libxml/parser.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>

// compiler avec gcc -Wall parser_xml_SAX.c -I/usr/include/libxml2/ -lxml2
//---> xml2-config --libs
//---> xml2-config --flags

#define SIZE 100


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

char * description_stage;
bool validation;

static xmlSAXHandler handler;
static int           elem_courant;

char process_id [SIZE];
char process_name[SIZE];



const xmlChar *  activity_id;
const xmlChar *  activity_name;
const char *  activity_description;
const xmlChar *  activity_performer;
const xmlChar *  activity_input;
const xmlChar *  activity_output; 
const xmlChar *  activity_etat ;




void caracteres (void *user_data, const xmlChar *text, int length)
{

	if( elem_courant == DESC_ACTIVITY)

	{
		activity_description = strndup((const char *)text, length);
		printf ("\tDescription: %s\n", (char *)activity_description);
	}
	if( elem_courant == PERF_ACTIVITY)

	{
		activity_performer = xmlStrndup(text, length);
		printf ("\tPerformer: %s\n", (char *)activity_performer);
	}
	if( elem_courant == IN_ACTIVITY)

	{
		activity_input = xmlStrndup(text, length);
		printf ("\tInput: %s\n", (char *)activity_input);
	}
	if( elem_courant == OUT_ACTIVITY)

	{
		activity_output = xmlStrndup(text, length);
		printf ("\tOutput: %s\n", (char *)activity_output);
	}

}

void fin_element (void *user_data, const xmlChar *name)
{

	if((strcmp((const char*)name,"tns:activity")==0 )&& elem_courant == ACTIVITY)
	{
		printf ("Fin de l’activité \n");
		printf ("- %s\n", (char *)activity_id);
		printf ("- %s\n", (char *)activity_name);
		printf ("- %s\n", (char *)activity_description);
		printf ("- %s\n", (char *)activity_performer);
		elem_courant=PROCESS;

	}

	else if(strcmp((const char*)name,"tns:description")==0)
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
		printf ("Debut  de l’activité %s\n", activity_id);
		printf("\tid: %s\n",(char*) activity_id);
		printf("\tname: %s\n",(char*) activity_name);

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



int main (int argc, char *argv[])
{
signal (SIGINT, SIG_IGN);	
validation= false;
	elem_courant=AUTRE;
	handler.startElement = debut_element;
	handler.endElement   = fin_element;
	handler.characters   = caracteres;
	if (xmlSAXUserParseFile (&handler, NULL, "process1.xml") < 0) { perror ("oups parser"); }
	return 0;
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
			bool validation=true; // gérer es 2 différents cas plus tard
			fprintf (file_dialogue,"Veuillez saisir l'ID de l'activite concernee\n");
			fgets(actToValid,LONG_ID,file_dialogue);
			fprintf (file_dialogue,"choix: %s\n", actToValid);

			Activity *activiteCourante = processCourant->debutListActivity ;
			//Activity *prev,*next;

			activiteCourante->prev = NULL;

			while (activiteCourante != NULL) {
				if(strncmp(activiteCourante->id,actToValid,2) == 0)
				{
					if(strncmp(activiteCourante->performer,user,sizeof(activiteCourante->performer)) == 0)
					{

						if(activiteCourante->prev !=NULL)
						{
							fprintf (file_dialogue,"activite prec\n");

							Activity *prev;
							prev=activiteCourante->prev;
							if(strncmp(prev->etat,"COMPLETED",9) == 0)
							{
								strcpy (activiteCourante->etat, "COMPLETED");
							}
							else
							{
								fprintf (file_dialogue,"L'activite %s ne peut pas etre validee car la transition n'est pas farnchie\n", activiteCourante->id);
							}

						}
						else // Activite A1
						{
							fprintf (file_dialogue,"PAS activite prec\n");
							strcpy (activiteCourante->etat, "COMPLETED");

							Activity *next;
							next=activiteCourante->next;
							strcpy (next->etat, "RUNNING");
						}

					}
					if(activiteCourante->next !=NULL) 
					{

						// GERER ACTIVITES 2 ET 3 AVEC VARIABLE BOOLEENE VALIDATON
						fprintf (file_dialogue,"activite suiv\n");
						Activity *next;
						next=activiteCourante->next;
						strcpy (next->etat, "RUNNING");
					}
					else // activite A7 (fin du process)
					{
						fprintf (file_dialogue,"PAS activite suiv\n");
						strcpy (processCourant->etat, "COMPLETED");
					}


					fprintf (file_dialogue,"VALIDATION OK\n");


				}
				else
				{
					fprintf (file_dialogue,"L'activite %s n'est pas assignee a %s\n",activiteCourante->id,user);
				}
			}
			//activiteCourante = activiteCourante->next;


			activiteCourante->prev = activiteCourante;
			activiteCourante = activiteCourante->next;


		}
	}    

	processCourant = processCourant->next;
}





