
#ifndef __LIBIPC_H__
#define __LIBIPC_H__

#define CLE_COMMANDE        0x00000001
#define CLE_REPONSE        0x00000002 
#define TAILLE_MSG              1024

typedef struct { 
	int numProcess;
	char contenuMessage[TAILLE_MSG]; 
} messsage_IPC;

void remplacerCar(char * chaine, char ancienneLettre, char nouvelleLettre);

void* gestion_file_message(void* arg);
void printConnectedUsers(messsage_IPC msg,int commandes, int reponses);
void listUsers(messsage_IPC msg,int commandes, int reponses);
void addUser(messsage_IPC msg,int commandes, int reponses);
void modeListen(messsage_IPC msg,int commandes, int reponses);
int verif_format(const char *modele,const char *chaine);







#endif
