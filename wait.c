#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_HOST 20
#define WAITING_LIMIT 5
#define HOSTNUM 1

int waitCount = 0;
int hosting = 0;
int busy = 0;//0 not busy, 1 busy
int record = 0;

pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;


void printData(){
	printf("Total wait number is %d \n",waitCount);
	printf("Total record is %d \n",record);
	printf("Total hosting is %d \n", hosting);
}


void *customer(void *t){
	
	int id = (int)t;
	int finish = -1;
	int wait = 0;
	
	while(finish == -1){
//	printf("Customer %d print data \n",id);
//	printData();
	pthread_mutex_lock(&count_mutex);
	if(waitCount < WAITING_LIMIT && hosting == 1 && wait == 0){
		waitCount++;
		printf("Dear Customer %d please sit in waiting room \n", id);
		wait = 1;
	}
	else if( hosting == 0 ){
		waitCount++;
		printf("GoGoGo, customer %d, barber is waiting for you \n", id);
		pthread_cond_wait(&count_threshold_cv,&count_mutex);
		finish = 1;
	}
	else if(waitCount > WAITING_LIMIT){
		printf("Waiting room is full, Customer %d left !\n",id);
		finish = 1;
	}

	pthread_mutex_unlock(&count_mutex);
	sleep(1);

	}
	pthread_mutex_lock(&count_mutex);	
	record++;
	pthread_mutex_unlock(&count_mutex);
	printf("Customer %d finished ! \n",id);
	pthread_exit(NULL);
}

void *barber(void *t){
	printf("This is barber! \n");
	while(record <= MAX_HOST){
		if(waitCount > 0 && hosting == 0){
			pthread_mutex_lock(&count_mutex);
			hosting++;
			pthread_mutex_unlock(&count_mutex);
			
			printf("Doing hair cut for customer.........\n");
			sleep(3);
			pthread_mutex_lock(&count_mutex);
			waitCount--;
			hosting--;
			pthread_cond_signal(&count_threshold_cv);
			pthread_mutex_unlock(&count_mutex);
		}
		sleep(1);
	}
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
		randNum = 0;
		while(randNum == 0){
			randNum = rand()%2; // 0 or 1
			if(randNum == 1){	
				pthread_create(&threads[i], &attr, customer, (void *)i);
			}	
			sleep(2);
		}
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


