#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "display.h"

pthread_t tid[2];
int counter;
pthread_mutex_t lock;

void* doSomeThing1(void *arg)
{
    int  i = 0;
    for (i=0;i<10;i++)
		{
			pthread_mutex_lock(&lock);
        	display("Hello world\n");
        	pthread_mutex_unlock(&lock);
    	}
    return NULL;
}

void* doSomeThing2(void *arg)
{
    int  j = 0;
    for (j=0;j<10;j++)
		{
			pthread_mutex_lock(&lock);
        	display("Kalimera kosme\n");
        	pthread_mutex_unlock(&lock);
    	}
    return NULL;
}



int main(void)
{
    int i = 0;
    int err;

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    err = pthread_create(&(tid[0]), &attr, doSomeThing2, NULL);
    if (err != 0)
        printf("\ncan't create thread :[%s]", strerror(err));
    printf("11111111");
    err = pthread_create(&(tid[1]), &attr, doSomeThing1, NULL);
    if (err != 0)
        printf("\ncan't create thread :[%s]", strerror(err));
    printf("22222222");

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_mutex_destroy(&lock);

    return 0;
}