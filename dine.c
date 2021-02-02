#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include "dawdle.c"
#include "dine.h"

int read_command_line(int argc, char *argv[]){
	int i;
	if (argc < 2){
		return DEFAULT_CYCLES;
	}
	else{
		int len = strlen(argv[1]);
		for(i = 0; i < len; i++){
			if (isdigit((int)argv[1][i]) == 0)
				return DEFAULT_CYCLES;
		}
		return atoi(argv[1]);
	}
}

void *philosophing(void *id_ptr){
	printf("Thread %d says hi\n", *(int *)id_ptr);
	return NULL;
}

void getForks(Fork forks[NUM_PHILOSOPHERS]){
	int i;
	for (i = 0; i < NUM_PHILOSOPHERS; i++){
		forks[i].owner = NULL;
	}
}

void getPhilsophers(Philosopher phils[NUM_PHILOSOPHERS],
				   Fork forks[NUM_PHILOSOPHERS], 
				   pthread_t threads[NUM_PHILOSOPHERS]){

	int i;
	for (i = 0; i < NUM_PHILOSOPHERS; i++){
		Philosopher confucious; /*threads are confusing*/
		confucious.name = 'a' + i;
		confucious.thread_id = &threads[i];	
		confucious.state = CHANGING;
		if (i == 0){
			confucious.isRightHanded = FALSE;
		}else{
			confucious.isRightHanded = TRUE;
		}
		confucious.target1 = &forks[i];
		if (i != NUM_PHILOSOPHERS - 1){
			confucious.target2 = &forks[FIRST_INDEX];
		}else{
			confucious.target2 = &forks[i+1];
		}
		confucious.isHungry = TRUE;
		phils[i] = confucious;
	}
}



int main(int argc, char *argv[]){

	Philosopher phils[NUM_PHILOSOPHERS];
	Fork forks[NUM_PHILOSOPHERS];
	pthread_t threads[NUM_PHILOSOPHERS];
	int id[NUM_PHILOSOPHERS];
	int i, numb_cycles;

	numb_cycles = read_command_line(argc, argv);
	printf("numb cycles is:%d\n", numb_cycles);

	for (i = 0; i < NUM_PHILOSOPHERS; i++){
		id[i] = i;
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
