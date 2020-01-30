/**
 * @file gestion_client.h
 * @author SECK Malick 
 * @brief 
 * @version 0.1
 * @date 2020-01-30
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef __GESTIONCLIENT_H__
#define __GESTIONCLIENT_H__

#include "workflow_struct.h"
#include "traitement_text.h"

#define MAX_LIGNE 2048
#define SIZE_BUFFER 1024
#define SIZE_NOM_MACHINE 400



extern char usersFile[100]; // declare dans main
extern char connectedUsers[MAX_UTILISATEURS][LONG_ID]; // declare dans main


struct ConnexionInfos
{


void* gestionClient(void *s);
struct ConnexionInfos authentificationClient(FILE *file_dialogue, char usersBDD[MAX_UTILISATEURS][3][LONG_ID]);



#endif
