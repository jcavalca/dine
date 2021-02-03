#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "dawdle.c"
#include "dine.h"
#include "util.c"
/*
sem_wait()  decrements  (locks)  the semaphore pointed to by sem
sem_post()  increments  (unlocks)  the  semaphore  pointed  to  by sem
*/

void think(Philosopher *p){
	dawdle();
	if (-1 == sem_wait(&mutex) ){
		exit_gracefully("sem_wait");
	}
	p -> state = CHANGING;				/*changing to eating*/
	print_states();

	if (-1 == sem_post(&mutex) ){
		exit_gracefully("sem_wait");
	}
}

void take_forks(Philosopher *p, sem_t *first, sem_t *second){

	/* getting first fork*/
	if (-1 == sem_wait(&mutex) ){
		exit_gracefully("sem_wait");
	}

	if (-1 == sem_wait(first) ){
		exit_gracefully("sem_wait");
	}
	p -> isHoldingRightFork = TRUE;		/*critical region*/

	if (-1 == sem_post(&mutex) ){
		exit_gracefully("sem_post");
	}

	/* getting second fork*/
	if (-1 == sem_wait(&mutex) ){
		exit_gracefully("sem_wait");
	}

	if (-1 == sem_wait(second) ){
		exit_gracefully("sem_wait");
	}
    p -> isHoldingLeftFork = TRUE;		  /*critical region*/
	p -> state = EATING;				  /*now ready to eat*/
	print_states();
	if (-1 == sem_post(&mutex) ){
		exit_gracefully("sem_post");
	}
}

void eat(Philosopher *p){
	dawdle();
	if (-1 == sem_wait(&mutex) ){
		exit_gracefully("sem_wait");
	}
	p -> state = CHANGING;				/*changing to thinking*/
	print_states();
	if (-1 == sem_post(&mutex) ){
		exit_gracefully("sem_wait");
	}
}

void put_forks(Philosopher *p, sem_t *first, sem_t *second){

	
	/* letting go first fork*/
	if (-1 == sem_wait(&mutex) ){
		exit_gracefully("sem_wait");
	}

	if (-1 == sem_post(first) ){
		exit_gracefully("sem_wait");
	}
	p -> isHoldingRightFork = FALSE;		/*critical region*/

	if (-1 == sem_post(&mutex) ){
		exit_gracefully("sem_post");
	}

	/* getting second fork*/
	if (-1 == sem_wait(&mutex) ){
		exit_gracefully("sem_wait");
	}

	if (-1 == sem_post(second) ){
		exit_gracefully("sem_wait");
	}
    p -> isHoldingLeftFork = FALSE;		  /*critical region*/
	p -> state = THINKING;				  /*now thinking*/
	print_states();
	if (-1 == sem_post(&mutex) ){
		exit_gracefully("sem_post");
	}

}

void *philosophing(void *ptr){
	int i;
	Philosopher *p = (Philosopher *)ptr;
	sem_t *first;
	sem_t *second;
	if (p -> isRightHanded == TRUE){
		first  =  &forks[p -> rightFork];
		second =  &forks[p -> leftFork];
	}else{
		first  =  &forks[p -> leftFork];
		second =  &forks[p -> rightFork];
	}

	for (i = 0; i < numb_cycles; i++){
		think(p);
	    
		take_forks(p, first, second);
        
		eat(p);
        
		put_forks(p, first, second);

	}
	pthread_exit(retval);
}

void getForks(){
	int i;
	for (i = 0; i < NUM_PHILOSOPHERS; i++){
		sem_t fork;		/*All forks start in the table*/
		if (sem_init(&fork, THREAD_SHARED, INIT_VALUE) == -1){
               exit_gracefully("sem_init");
		}
		forks[i] = fork;
	}
	if (sem_init(&mutex, THREAD_SHARED, INIT_VALUE) == -1){
               exit_gracefully("sem_init");
		}
}

void getPhilosophers(){

	int i;
	for (i = 0; i < NUM_PHILOSOPHERS; i++){
		Philosopher confucious; /*threads are confusing*/
		confucious.name = (char) ('A' + i);
		confucious.thread_id = &threads[i];	
		confucious.state = THINKING;
		if (i == 0){
			confucious.isRightHanded = FALSE;
		}else{
			confucious.isRightHanded = TRUE;
		}
		confucious.leftFork = i;
		if (i == NUM_PHILOSOPHERS - 1){
			confucious.rightFork = ZERO_INDEX;
		}else{
			confucious.rightFork = i+1;
		}
		confucious.isHungry = TRUE;
		confucious.isHoldingRightFork = FALSE;
		confucious.isHoldingLeftFork = FALSE;
		phils[i] = confucious;
	}
}

int main(int argc, char *argv[]){

	int i;
	numb_cycles = read_command_line(argc, argv);
	getForks();
	getPhilosophers();
	print_beggining();
	print_states();
	for (i = 0; i < NUM_PHILOSOPHERS; i++){
		sem_ids[i] = i;
		int error_check = pthread_create(
						&threads[i], 
						NULL, 
						philosophing, 
						(void*) (&phils[i]));
		if (error_check == -1){
			exit_gracefully("pthread_create");
		}
	}
	for (i = 0; i < NUM_PHILOSOPHERS; i++){
		pthread_join(threads[i], NULL);
	}

	return 0;
}
