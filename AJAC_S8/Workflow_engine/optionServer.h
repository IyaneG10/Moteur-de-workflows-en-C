/**
 * @file optionServer.h
 * @author SECK Malick 
 * @brief 
 * @version 0.1
 * @date 2020-01-30
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef __OPTIONSERVER_H__
#define __OPTIONSERVER_H__

extern char usersFile[100];
/**
 * @brief Paramètre à passer en argument au programme principal pour définir le numéro du port et/ou le fichier de stockage des utilisateurs
 * 
 */
struct OptionArg
{
	char portNum [100];
	char File[100];
};

struct OptionArg check_arguments(int argc, char **argv);

#endif
