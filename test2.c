#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#define BUFFER_SIZE 5
#define NP          3
#define NC          3
#define NITERS      4

/* Taille buffer */
/* Nombre de producteurs */
/* Nombre de consommateurs */
/* Nombre d'items produits/consommes */
typedef struct {
    int buffer[BUFFER_SIZE];   /* Buffer partage */
    int in;         
  /*  buffer[in%BUFFER_SIZE] est la premiere case vide  */
    int out;        
 /* buffer[out%BUFFER_SIZE] est la premiere case pleine  */
    sem_t full;     
 /* Represente le nombre de cases pleines */
    sem_t empty;    
 /* Représente le nombre de cases vides */
    sem_t mutex;    
 /*  Pour l'exclusion mutuelle a la variable partagee  */
} sbuf_t;
sbuf_t shared;
void *producteur(void *arg) {
    int i, item, index;
    index = (int)arg;
    for (i=0; i < NITERS; i++) {
        /* Produire un item */
        item = i;
        /* Preparer le depot de l'item dans buffer */
        /* S'il n'y a pas de cases vides, attendre */
        sem_wait(&shared.empty);
        /* Si un autre thread (producteur ou consommateur) utilise buffer,
attendre */
        

        sem_wait(&shared.mutex);
        /* Depot de l'item */
        shared.buffer[shared.in] = item;
        shared.in = (shared.in+1)%BUFFER_SIZE;
        printf("[P%d] Produit %d ...\n", index, item); fflush(stdout);
       
        /* Liberer le buffer */
        
         sem_post(&shared.mutex);
        /* Incrementer le nombre de cases pleines */
        sem_post(&shared.full);
        /* Entrelacer l'execution du producteur et du consommateur */
        if (i % 2 == 1) sleep(rand()%4);
    }
}
void *consommateur(void *arg) {
    int i, item, index;
    index = (int)arg;
    for (i=0; i < NITERS; i++) {
        /* consommer un item */
        item = i;
        /* Preparer le depot de l'item dans buffer */
        /* S'il n'y a pas de cases vides, attendre */
        sem_wait(&shared.full);
        /* Si un autre thread (producteur ou consommateur) utilise buffer,
attendre */
        

         sem_wait(&shared.mutex);
        /* Depot de l'item */
        shared.buffer[shared.in] = item;
        shared.in = (shared.in+1)%BUFFER_SIZE;
        printf("[C%d] consomme %d ...\n", index, item); fflush(stdout);
       
        /* Liberer le buffer */
        
             sem_post(&shared.mutex);
        /* Incrementer le nombre de cases pleines */
        sem_post(&shared.empty);
        /* Entrelacer l'execution du producteur et du consommateur */
       if (i % 2 == 1) sleep(rand()%4);
    }
}

int main()
{
    srand(time(NULL));
    pthread_t idP, idC;
    int index;
    /* Initialisation des semaphores full et empty */
    sem_init(&shared.full,0,0);
    sem_init(&shared.empty, 0,BUFFER_SIZE);
    /* Initialisation de mutex*/
    sem_init(&shared.mutex,0,1);
    for (index = 0; index < NP; index++)
    {  
       /* Creer un nouveau producteur */
       pthread_create(&idP, NULL, producteur, (void*)index);
    }
    /* Inserer ici le code de creation de NC consommateurs */
    for (index = 0; index < NC; index++)
    {  
       /* Creer un nouveau producteur */
       pthread_create(&idC, NULL, consommateur, (void*)index);
    }
    /* Detruire les semaphores full et empty */
        sem_destroy(&shared.full);
        sem_destroy(&shared.empty);
    /* Detruire mutex */ 
   	sem_destroy(&shared.mutex);
    /* Terminer le thread main */
    pthread_exit(0);
}
