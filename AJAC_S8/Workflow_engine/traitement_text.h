/**
 * @file traitement_text.h
 * @author SECK Malick 
 * @brief 
 * @version 0.1
 * @date 2020-01-30
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef __TRAITEMENTTEXT_H__
#define __TRAITEMENTTEXT_H__

#define MAX_UTILISATEURS 10
#define LONG_ID 100




char * findStrOpt(char str[100]);
char * findStrArg(const char *text);

int check_format(const char *modele,const char *chaine);
int parserBDDUsers(FILE *fp_UsersFile, char users[MAX_UTILISATEURS][3][LONG_ID]);




#endif
