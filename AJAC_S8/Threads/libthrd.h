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


// pour les différents verrous (car les fonctions P et V prennent des entiers en arguments)
// Il y'a 3 verrous pour le code 
enum {
    LOCK_TAB_CONN_USERS,
    LOCK_USERS_FILE,
    LOCK_LIST_PROCESSES
};


#endif


