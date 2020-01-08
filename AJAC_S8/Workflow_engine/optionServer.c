/** fichier optionServer.c **/

/*****************************************************************/
/** Ce fichier contient des fonctions utilisees par le traitement des argument de la fonction principale                             **/
/*****************************************************************/

/**** Fichiers d'inclusion ****/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "optionServer.h"




struct OptionArg check_arguments(int argc, char **argv)
{
	int res;
	struct OptionArg arguments = {"4000","users"}; // valeurs par défaut

	while (1)
	{
		static struct option long_options[] =
		{
			{"port",  required_argument, 0, 'p'},
			{"users file",  required_argument, 0, 'u'},
			{0, 0, 0, 0}
		};
		int option_index = 0;
		res = getopt_long (argc, argv, "p:u:",long_options, &option_index);

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
			case 'p':
				{
					strcpy(arguments.portNum, optarg);
					#ifdef DEBUG
					printf ("GETOPTLONG:Le port spécifié en argument est: %s\n", optarg);
					#endif
					break;
				}
			case 'u':
				{
					strcpy(arguments.File, optarg);
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








