#ifndef H_LIBTHRD
#define H_LIBTHRD


void *recup_param_Thread(void *arg); 
void lanceClientLeger(void *param, void* (*fonction) (void*)); 


void P(int index);
void V(int index);


#endif


