#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_HOST 20
#define WAITING_LIMIT 5
#define HOSTNUM 1

int waitCount = 0;
int hosting = 0;
int record = 0;

pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;


void *customer(void *t){
	int customerId = (int)t;
	int waiting = 0;	
	int finish = 0;
	printf("Wow, customer %d arrived!\n",customerId);	
	
	while(finish == 0){	
		pthread_mutex_lock(&count_mutex);
		if(waiting == 0){
			record++;
			waitCount++;
		}
		if( hosting < HOSTNUM && waitCount > 0 && waitCount <= WAITING_LIMIT && waiting ==0){
			pthread_cond_signal(&count_threshold_cv);
			printf("Good! GOGOGO,barber is waiting for you! customer %d \n", customerId);
			waitCount--;
			waiting = 1;
			finish = 1;
        	}	
		else if (waitCount < WAITING_LIMIT && waiting == 0 ){
			printf("Sorry, barber is busy, plz wait, customer %d \n",customerId);
                	waiting = 1;
		}	
		else if (waitCount >= WAITING_LIMIT && waiting == 0 ){ 
			printf("Sorry, waiting room is full, see u next time! customer %d \n", customerId);
			finish = 1;
			waitCount--;
		}
	
		pthread_mutex_unlock(&count_mutex);
		sleep(1);
		}	
	// finish this thread
	printf("Customer %d finished!\n", customerId);
	pthread_exit(NULL);
}

void *barber(void *t){
	pthread_mutex_lock(&count_mutex);
	printf("This is barber~\n");
	while(record < HOSTNUM){
		pthread_cond_wait(&count_threshold_cv, &count_mutex);
		hosting++;
		printf("Hair cut.........\n");
		sleep(4);// the time to finish the hair cut
		hosting--;
	}
	pthread_mutex_unlock(&count_mutex);
	pthread_exit(NULL);
}

int main(){
	int i;
	int randNum;
	//createBaber
	pthread_t threads[MAX_HOST];
	pthread_attr_t attr;

	//Initializee mutex and condition variable object
	pthread_mutex_init(&count_mutex, NULL);
	pthread_cond_init(&count_threshold_cv, NULL);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	i = 0;
	pthread_create(&threads[0], &attr, barber, (void *) i);
	sleep(1);
	for (i = 1; i < MAX_HOST; i++){
		randNum = rand()%2; // 0 or 1
		if(randNum == 1){	
			pthread_create(&threads[i], &attr, customer, (void *)i);
		}
		sleep(2);
	}
	
	/* Wait for all threads to complete */
   	for (i=0; i<MAX_HOST; i++) {
     		pthread_join(threads[i], NULL);
	}
   	printf (" Done.\n ");
   	/* Clean up and exit */
   	pthread_attr_destroy(&attr);
   	pthread_mutex_destroy(&count_mutex);
   	pthread_cond_destroy(&count_threshold_cv);
   	pthread_exit(NULL);
}




