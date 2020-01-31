/**
 * @file traitement_text.c
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
#include <unistd.h>
#include <stddef.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include<regex.h>

#include "traitement_text.h"
#include "libthrd.h"

/**
 * @brief Pour détecter l'option (aprés le tiret)
 * 
 * @param str 
 * @return char* 
 */
char * findStrOpt(char str[100])
{
	char * strToken=strtok (str, "-" );
	strToken = strtok ( NULL, "-" );
	return strToken;
}

/**
 * @brief Pour détecter un argument compris entre "<" et ">"
 * 
 * @param text 
 * @return char* 
 */
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

/**
 * @brief Vérification du format avec des expression régulièreq
 * 
 * @param modele regex 
 * @param chaine 
 * @return int 
 */
int check_format(const char *modele,const char *chaine)
{	
	int ret =1;
	regex_t regex;
	if (regcomp (&regex, modele, REG_NOSUB | REG_EXTENDED)== 0)
	{
		int validite = regexec (&regex, chaine, 0, NULL, 0);

		if (validite == 0)
		{
			ret =0;
#ifdef DEBUG
			printf ("%s est une chaine valide\n", chaine);
#endif
		}

		else if (validite == REG_NOMATCH)
		{
#ifdef DEBUG
			printf ("%s n\'est pas une chaine valide\n", chaine);
#endif
		}
	}

	regfree (&regex);
	return ret;
}
/**
 * @brief Parser le fichier d'utilisateurs et mettre les éléments dans une matrice
 * 
 * @param fp_UsersFile 
 * @param users 
 * @return int 
 */
int parserBDDUsers(FILE *fp_UsersFile, char users[MAX_UTILISATEURS][3][LONG_ID])
{
	P(LOCK_USERS_FILE);
	const char *modele = "^[-_[:alnum:]]+:[-_[:alnum:]]+:[_[:alpha:]]+ [_[:alpha:]]+$";
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

			if(check_format(modele,ligne) == 0)
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

	}

	V(LOCK_USERS_FILE);
	return usersFound;

}
