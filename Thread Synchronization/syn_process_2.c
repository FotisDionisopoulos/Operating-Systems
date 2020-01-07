#include <sys/ipc.h>

#include <sys/shm.h>

#include <sys/sem.h>

#include <sys/wait.h>

#include <stdio.h>

#include "display.h"

int main() {



	struct sembuf up1   = {0,  1, 0};

	struct sembuf down1 = {0, -1, 0};



	struct sembuf up2   = {1,  1, 0};

	struct sembuf down2 = {1, -1, 0};

	

	int my_sem, status, val, i;



	my_sem=  semget(IPC_PRIVATE, 2, 0600); /* create semaphore */

	semop(my_sem, &up2, 1); 



	if ( fork()==0) 

	{

		 

		for (i=0;i<10;i++)

		{

			semop(my_sem, &down1, 1); 

			display("cd\n");

			semop(my_sem, &up2, 1);

		}

		  

	}

	else 

	{

		

		for (i=0;i<10;i++)

		{

			semop(my_sem, &down2, 1); 

			display("ab");

			semop(my_sem, &up1, 1); 

		}

		 



		wait(NULL);

		

	}



	semctl(my_sem, 0, IPC_RMID);

	return 0;

}


