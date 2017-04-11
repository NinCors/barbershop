#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_HOST 20
#define WAITING_LIMIT 5
#define HOSTNUM 1

int waitCount = 0;
int hosting = 0;
int record = 0;
pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;
srand(time(NULL));

void *customer(void *t){
	int customerId = t;
	bool finish = false;
	printf("Wow, customer %d arrived!\n",customerId);
	
	pthread_mutex_lock(&count_mutex);
	record++;	
	
	while(finish){
	
	if( hosting < HOSTNUM && waitCount > 0 && waitCount <= WAITING_LIMIT){
		pthread_cond_signal(&count_threshold_cv);
		printf("Good! GOGOGO,barber is waiting for you! customer %d \n", customerId);
		finish = true;
        }
	else if (waitCount < WAITING_LIMIT ){
		waitCount++;
		printf("Sorry, barber is busy, plz wait, customer %d \n",customerId);
		finish = true;
	}
	else if (waitCount < WAITING_LIMIT){ 
		printf("Sorry, waiting room is fool, see u next time! customer %d \n", customerId);
		finish = true;
	}
	sleep(1);
	
	}	
	pthread_mutex_unlock(&count_mutex);
	// finish this thread
	pthread_exit(NULL);
}

void *barber(void *t){
	pthread_mutex_lock(&count_mutex);
	while(record < HOSTNUM){
		pthread_cond_wait(&cont_threshold_cv, &count_mutex);
		hosting++;
		printf("Hair cut.........\n");
		sleep(2);// the time to finish the hair cut
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
	pthread_create(&threads[0], &attr, barber, (void *) i)
	for (i = 1; i < MAX_HOST; i++){
		randNum = rand()%2; // 0 or 1
		if(ranNum == 1){	
			pthread_create(&threads[i], &attr, customer, (void *)i)
		}
	}
	
	/* Wait for all threads to complete */
   	for (i=0; i<NUM_THREADS; i++) {
     		pthread_join(threads[i], NULL);
	}
   	printf (" Done.\n ");
   	/* Clean up and exit */
   	pthread_attr_destroy(&attr);
   	pthread_mutex_destroy(&count_mutex);
   	pthread_cond_destroy(&count_threshold_cv);
   	pthread_exit(NULL);
}




