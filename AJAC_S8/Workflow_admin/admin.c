/**
 * @file admin.c
 * @author SECK Malick
 * @brief 
 * @version 0.1
 * @date 2020-01-30
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <stdio.h> 
#include <string.h> 
#include <getopt.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 


#define CLE_COMMANDE        0x00000001
#define CLE_REPONSE        0x00000002 
#define TAILLE_MSG              1024
typedef struct { 
	int numProcess;
	char contenuMessage[TAILLE_MSG]; 
} messsage_IPC; 


struct Arg
{
	char arg_U [TAILLE_MSG];
	char arg_C[TAILLE_MSG];
	char arg_L [TAILLE_MSG];
	char arg_A[TAILLE_MSG];
};




struct Arg getCmdAdmin(int argc, char **argv)
{
	int res;
	struct Arg arguments = {"","","","\0"}; // valeurs par défaut

	while (1)
	{
		static struct option long_options[] =
		{
			{"users",  no_argument, 0, 'u'},
			{"connected",  no_argument, 0, 'c'},
			{"listen",  no_argument, 0, 'l'},
			{"add-user",  required_argument, 0, 'a'},
			{0, 0, 0, 0}
		};
		int option_index = 0;
		res = getopt_long (argc, argv, "ucla:",long_options, &option_index);

		if (res == -1)
			break;
		switch (res)
		{
			case 0:
				if (long_options[option_index].flag != 0)
					break;
#ifdef DEBUG
				printf ("GETOPTLONG:option %s", long_options[option_index].name);
#endif
				if (optarg)
				{
#ifdef DEBUG
					printf ("GETOPTLONG:with arg %s", optarg);
#endif
				}
#ifdef DEBUG
				printf ("\n");
#endif
				break;
			case 'u':
				{
					strcpy(arguments.arg_U, "u");
#ifdef DEBUG
					printf ("GETOPTLONG:L'option spécifiée est: %s\n", arguments.arg_U);
#endif
					break;
				}
			case 'c':
				{
					strcpy(arguments.arg_C, "c");
#ifdef DEBUG
					printf ("GETOPTLONG:L'option spécifiée est: %s\n", arguments.arg_C);
#endif
					break;
				}
			case 'a':
				{
					strcpy(arguments.arg_A, optarg);
#ifdef DEBUG
					printf ("GETOPTLONG:L'option spécifiée a pour agrhument : %s\n", optarg);
#endif
					break;
				}
			case 'l':
				{
					strcpy(arguments.arg_L, "l");
#ifdef DEBUG
					printf ("GETOPTLONG:L'option spécifiée est: %s\n", arguments.arg_L);
#endif
					break;
				}
			case '?':
				{
					printf("GETOPTLONG: Option non reconnue\n");
					break;
				}

			default:
				abort ();
		}
	}

	return arguments;
}











int main(int argc, char **argv) { 


    
    struct Arg arg = {"","","","\0"}; // valeurs par défaut
	arg = getCmdAdmin(argc, argv);
	int commandes, reponses;
	messsage_IPC msg;
    memset(msg.contenuMessage,0,strlen(msg.contenuMessage));	
	int res; 
	int pid=getpid();
	printf("Le PID est: %d\n",pid);

	// récupérer l'id de la file de commande (créee par le serveur) 
	commandes = msgget(CLE_COMMANDE, 0); 
	if (commandes == -1) { perror("msgget requete"); return (EXIT_FAILURE); } 
	// créer une file de reponse (les droit R/W pour tous)
	reponses = msgget(CLE_REPONSE, 0666 | IPC_CREAT); 
	if (reponses == -1) { perror("msgget reponse"); return (EXIT_FAILURE); }

	if(strcmp(arg.arg_U, "u") == 0)
	{
		strcpy(msg.contenuMessage, "getUsers");
	}
	else if(strcmp(arg.arg_C, "c") == 0)
	{
		strcpy(msg.contenuMessage, "getConnectedUsers");
	}
	else if(strcmp(arg.arg_L, "l") == 0)
	{
		strcpy(msg.contenuMessage, "listen");
	}
	else if (strcmp(arg.arg_A, "\0") != 0)
	{
		strcpy(msg.contenuMessage, arg.arg_A);
	}



	// envoyer la commande admin avec le numéroo du processus 
	msg.numProcess = pid;
	res = msgsnd(commandes, & msg, strlen(msg.contenuMessage) + 1, 0); 
	if (res == -1) { perror("msgsnd"); return (EXIT_FAILURE); } 

	// Récupérer et afficher la réponse du serveur
	while (1)
	{
		res = msgrcv(reponses, & msg, TAILLE_MSG,0 /*pid*/, 0); 
		if (res == -1) { perror("msgrcv"); return (EXIT_FAILURE); } 
		{
			if (strcmp(msg.contenuMessage, "\0") != 0) 
			{
				printf("%s\n", msg.contenuMessage);
                memset(msg.contenuMessage,0,strlen(msg.contenuMessage));
			}
			else{break;};
		} 
	}
	return 0; 
}


