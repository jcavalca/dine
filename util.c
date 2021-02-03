/* defines number of philosophers */
#ifndef NUM_PHILOSOPHERS 
#define NUM_PHILOSOPHERS 5
#endif

Philosopher phils[NUM_PHILOSOPHERS];
sem_t forks[NUM_PHILOSOPHERS];
sem_t mutex;	/*for changing states and printing atomically*/
pthread_t threads[NUM_PHILOSOPHERS];
int sem_ids[NUM_PHILOSOPHERS];
int numb_cycles;
void *retval;

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

void print_header_struct(){
	int i;
	for (i = 0; i < NUM_PHILOSOPHERS; i++){
		if (i == NUM_PHILOSOPHERS - 1){
			fprintf(stderr, "|=============|\n");
		}else{
			fprintf(stderr, "|=============");
		}
	}
}

void print_beggining(){
	int i;
	print_header_struct();
	for (i = 0; i < NUM_PHILOSOPHERS; i++){
		if (i != NUM_PHILOSOPHERS - 1){
			fprintf(stderr, "|     %c       ", phils[i].name);
		}else{
			fprintf(stderr, "|      %c      |\n", phils[i].name);
		}
	}
	print_header_struct();
}

void print_states(){
	int i;
	for (i = 0; i < NUM_PHILOSOPHERS; i++){
		fprintf(stderr, "|");
		if (phils[i].isHoldingLeftFork)
			fprintf(stderr, "%d", phils[i].leftFork);
		else
			fprintf(stderr, "-");
		if (phils[i].isHoldingRightFork)
			fprintf(stderr, "%d", phils[i].rightFork);
		else
			fprintf(stderr, "-");
		
		if (phils[i].state == EATING)
			fprintf(stderr, "    EAT    ");
		else if (phils[i].state == THINKING)
			fprintf(stderr, "   THINK   ");
		else
			fprintf(stderr, "           ");
		if (i == NUM_PHILOSOPHERS - 1)
			fprintf(stderr, "|\n");
	}
}

void exit_gracefully(char *msg){
	if (msg != NULL){
		perror(msg);}
	exit(EXIT_FAILURE);
}
