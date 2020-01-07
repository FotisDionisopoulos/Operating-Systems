#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "display.h"

#define N 10         

pthread_mutex_t mutex;
pthread_cond_t condc, condp;
int buf = 0;

void* pro(void *ptr) {
  int i;
  for (i = 1; i <= N; i++) {
   pthread_mutex_lock(&mutex); 

    while (buf != 0)         

                    

    pthread_cond_wait(&condp, &mutex);
    buf = i;
    display("ab");
    pthread_cond_signal(&condc);    /* wake up call for con */
    pthread_mutex_unlock(&mutex);   /* release buf */
  }
  pthread_exit(0);

}



void* con(void *ptr) {
  int i;

  for (i = 1; i <= N; i++) {
    pthread_mutex_lock(&mutex); 
    while (buf == 0)         

      pthread_cond_wait(&condc, &mutex);

    buf = 0;
    display("cd\n");
    pthread_cond_signal(&condp);    /* wake up call for con */
    pthread_mutex_unlock(&mutex);   /* release buf */
  }
  pthread_exit(0);

}



int main(int argc, char **argv) {
  pthread_t t_pro, t_con;
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  pthread_mutex_init(&mutex, NULL); 
  pthread_cond_init(&condc, NULL);      
  pthread_cond_init(&condp, NULL);     

  pthread_create(&t_con, &attr, con, NULL);
  pthread_create(&t_pro, &attr, pro, NULL);

  pthread_join(t_con, NULL);
  pthread_join(t_pro, NULL);

  pthread_mutex_destroy(&mutex);    
  pthread_cond_destroy(&condc);    
  pthread_cond_destroy(&condp);    

}

