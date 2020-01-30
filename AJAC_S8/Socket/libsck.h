/**
 * @file libsck.h
 * @author SECK Malick 
 * @brief 
 * @version 0.1
 * @date 2020-01-30
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef __LIBSCK_H__
#define __LIBSCK_H__

/**** Variables globales *****/
struct sockaddr_in adresse;

/** Constantes **/
#define NB_CONNEXIONS 10 

int initialisationServeur(char *port);

void dummy(void *dialogue, void* (*action)(void *));

long boucleServeur(long socket, void(*fonction)(void *, void *(*)(void*)), void* (*gestionClient)(void *));

#endif
