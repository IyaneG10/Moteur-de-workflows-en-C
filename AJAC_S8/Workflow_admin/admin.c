#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <getopt.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 

#define CLE_REQUETE         0x00000001
#define CLE_REPONSE        0x00000002 
#define TAILLE_MSG              1024

typedef struct { 
    int numProcess;
    char contenuMessage[ TAILLE_MSG ]; 
} messsage_IPC; 





char getCmdAdmin(int argc, char **argv)
{
	int res;
	char arguments [10];


	while (1)
	{
		static struct option long_options[] =
		{
			{"users",  no_argument, 0, 'u'},
			{"connected",  no_argument, 0, 'c'},
			{"add-user",  required_argument, 0, 'a'},
			{"listen",  no_argument, 0, 'l'},
			{0, 0, 0, 0}
		};
		int option_index = 0;
		res = getopt_long (argc, argv, "ucal",long_options, &option_index);

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
					strcpy(arguments, "u");
					printf ("spécifié en argument est: %s\n", arguments);
					#ifdef DEBUG
					printf ("GETOPTLONG:Le port spécifié en argument est: %s\n", optarg);
					#endif
					break;
				}
			case 'c':
				{
					strcpy(arguments, "c");
					#ifdef DEBUG
					printf ("GETOPTLONG:Le fichier utilisateur spécifié en argument est: %s\n", optarg);
					#endif
					break;
				}
			case 'a':
				{
					strcpy(arguments, optarg);
					#ifdef DEBUG
					printf ("GETOPTLONG:Le fichier utilisateur spécifié en argument est: %s\n", optarg);
					#endif
					break;
				}
			case 'l':
				{
					strcpy(arguments, "l");
					#ifdef DEBUG
					printf ("GETOPTLONG:Le fichier utilisateur spécifié en argument est: %s\n", optarg);
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

	char arg [10];
	strcpy(arg,getCmdAdmin(argc, argv));
	int requetes, reponses;
	messsage_IPC msg;
	int res; 

	/* se connecter aux IPC de requête et de réponse */
	requetes = msgget(CLE_REQUETE, 0); 
	if (requetes == -1) { perror("msgget"); return (EXIT_FAILURE); } 

	reponses = msgget(CLE_REPONSE, 0700 | IPC_CREAT); 
	if (reponses == -1) { perror("msgget"); return (EXIT_FAILURE); }

	

	printf("L'argument est: %s\n",arg);

	/* demander un messsage_IPC à l'utilisateur */
	printf("Saisir la commande à envoyer : ");
	fgets(msg.contenuMessage, TAILLE_MSG, stdin);

	/* envoyer la requêtes signée avec le numéro de processus */
	msg.numProcess = getpid();
	res = msgsnd(requetes, & msg, strlen(msg.contenuMessage) + 1, 0); 
	if (res == -1) { perror("msgsnd"); return (EXIT_FAILURE); } 

	/* Ne récupérer que les réponses dédiées à notre processus */
	res = msgrcv(reponses, & msg, TAILLE_MSG, getpid(), 0); 
	if (res == -1) { perror("msgrcv"); return (EXIT_FAILURE); } 

	printf("Le serveur a envoyé : %s\n", msg.contenuMessage); 

	return 0; 
}

