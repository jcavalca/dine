#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "dawdle.c"

#ifndef NUM_PHILOSOPHERS 
#define NUM_PHILOSOPHERS 5
#endif

/* defining possible states for a philosopher*/
#define THINKING 0
#define EATING 1
#define CHANGING 2

void *philosophing(void *id_ptr){
	printf("Thread %d says hi\n", *(int *)id_ptr);
	return NULL;
}

int main(int argc, char *argv[]){

	int id[NUM_PHILOSOPHERS], i;
	pthread_t threads[NUM_PHILOSOPHERS];

	for (i = 0; i < NUM_PHILOSOPHERS; i++){
		id[i] = i;
	}
	for (i = 0; i < NUM_PHILOSOPHERS; i++){
		int error_check = pthread_create(
						&threads[i], 
						NULL, 
						philosophing, 
						(void*) (&id[i]));
		if (error_check == -1){
			perror("pthread_create\n");
			exit(EXIT_FAILURE);
		}
	}
	for (i = 0; i < NUM_PHILOSOPHERS; i++){
		pthread_join(threads[i], NULL);
	}
	printf("Parent says goodbye\n");
	return 0;
}
