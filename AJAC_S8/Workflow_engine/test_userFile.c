#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>

#define MAX_UTILISATEURS 10
#define LONG_ID 100


int parserUserFile(FILE *, char [MAX_UTILISATEURS][3][LONG_ID]);
bool checkIfValidUser(char *, char [MAX_UTILISATEURS][3][LONG_ID]);

int main(int argc, char *argv[])
{


	char users[MAX_UTILISATEURS][3][LONG_ID];
	FILE * fp = fopen("users", "r");
	if (fp == NULL)
	{
		fprintf(stderr, "Impossible d'ouvrir le fichier\n");
	}


	if ( parserUserFile(fp, users) <= 0)
	{
		fprintf(stderr, "Impossible de parser le fichier\n");
	}



	bool validity_ID = checkIfValidUser("tmv:toto", users);

	if(validity_ID)
	{
		printf("Utilisateur connecté\n");
	}
	else
	{
		printf("Mot de passe ou login incorrect\n");
	}


	return 0;
}


int parserUserFile(FILE *fp, char users[MAX_UTILISATEURS][3][LONG_ID])
{
	char user[LONG_ID];
	int c;
	int charsRead;
	int usersFound = 0;
	char username[LONG_ID],  password[LONG_ID],  utilisateur[LONG_ID];

	do
	{
		charsRead = 0;
		c = fgetc(fp);
		user[0] = '\0';
		while(c != '\n' && c != EOF)
		{
			if (charsRead++ > (LONG_ID*2 + 1))
			{
				fprintf(stderr, "%s : Username or password more than %d characters.", user, LONG_ID);
				break;
			}
			user[charsRead-1] = c;
			user[charsRead] = '\0';
			c = fgetc(fp);
		}
		if (user[0] != '\0')
		{

			char * token = strtok(user, ":");
			if(token != NULL /*&& token != ""*/)
				strcpy(username, token);

			token = strtok(NULL, ":");
			if(token != NULL /*&& token != ""*/)
				strcpy(password, token);

			token = strtok(NULL, "");
			if(token != NULL /*&& token != ""*/)
				strcpy(utilisateur, token);

			strcpy(users[usersFound][0], username);
			strcpy(users[usersFound][1], password);
			strcpy(users[usersFound][2], utilisateur);
			usersFound++;
		}
	}while(usersFound < MAX_UTILISATEURS && c != EOF);



	for(int i=0; i<usersFound; i++)
	{
		printf("User %d: %s Password: %s Identifiant: %s\n",i, users[i][0], users[i][1], users[i][2]);
	}


	return usersFound;
}

bool checkIfValidUser(char *identifiants, char users[MAX_UTILISATEURS][3][LONG_ID])
{
	bool validity=false;
	printf("RENSEIGNE %s\n", identifiants);
	char usernamepassword[LONG_ID*2];
	for(int i=0; i<MAX_UTILISATEURS; i++)
	{
		strcpy(usernamepassword, users[i][0]);
		strcat(usernamepassword,":");
		strcat(usernamepassword, users[i][1]);
		printf("BDD %s\n", usernamepassword);
		if(strcmp(usernamepassword, identifiants) == false)
		{return validity=true;}
	}
	return validity;
}
