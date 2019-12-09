#ifndef __LIBIPC_H__
#define __LIBIPC_H__


#define CLE_REQUETE         0x00000001
#define CLE_REPONSE        0x00000002 
#define TAILLE_MSG              1024

typedef struct { 
	long numProcess;
	char contenuMessage[ TAILLE_MSG ]; 
} messsage_IPC; 

void* gestion_file_message(void* arg);







#endif
