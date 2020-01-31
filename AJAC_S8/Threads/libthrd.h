/**
 * @file libthrd.h
 * @author SECK Malick 
 * @brief 
 * @version 0.1
 * @date 2020-01-30
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef H_LIBTHRD
#define H_LIBTHRD


void *recup_param_Thread(void *arg); 
void lanceClientLeger(void *param, void* (*fonction) (void*)); 


void P(int index);
void V(int index);


/**
 * @brief 3 verrous pour les zones sensibles
 * - pour la table des utilisateurs connectés
 * - pour le fichier de stockage des utilisateurs
 * - pour les listes chainés (processus et activités)
 * 
 */

enum {
	LOCK_TAB_CONN_USERS,
	LOCK_USERS_FILE,
	LOCK_LIST_PROCESSES
};


#endif


