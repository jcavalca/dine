/* defines number of philosophers */
#ifndef NUM_PHILOSOPHERS 
#define NUM_PHILOSOPHERS 5
#endif

/* only used when there is only one philosopher
   in this case we have: 
	#forks != #philosophers
*/
#define NUMB_FORKS NUM_PHILOSOPHERS!=1?NUM_PHILOSOPHERS:NUM_PHILOSOPHERS + 1

Philosopher phils[NUM_PHILOSOPHERS];
sem_t mutex;	/*for changing states and printing atomically*/
pthread_t threads[NUM_PHILOSOPHERS];
int sem_ids[NUM_PHILOSOPHERS];
int numb_cycles;
void *retval;
sem_t forks[NUMB_FORKS];

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
	int i, i2;
	for (i = 0; i < NUM_PHILOSOPHERS; i++){
		if (i == NUM_PHILOSOPHERS - 1){
			fprintf(stderr, "|");
			for (i2 = 0; i2 < NUM_PHILOSOPHERS + SPACING; i2++){
				fprintf(stderr, "=");
			}
			if (NUM_PHILOSOPHERS == 1){
				fprintf(stderr, "=");
			}
			fprintf(stderr, "|\n");

		}else{
			fprintf(stderr, "|");
			for (i2 = 0; i2 < NUM_PHILOSOPHERS + SPACING; i2++){
				fprintf(stderr, "=");
			}
		}
	}
}

void print_beggining(){
	int i, i2;
	print_header_struct();
	for (i = 0; i < NUM_PHILOSOPHERS; i++){
		if (i != NUM_PHILOSOPHERS - 1){
			int mid = (int)(NUM_PHILOSOPHERS + SPACING) / 2;
			fprintf(stderr, "|");
			for (i2 = 0; i2 < NUM_PHILOSOPHERS + SPACING; i2++){
				if (i2 == mid)
					fprintf(stderr, "%c", phils[i].name);
				else
					fprintf(stderr, " ");
			}
		}else{
			int mid = (int)(NUM_PHILOSOPHERS + SPACING) / 2;
			fprintf(stderr, "|");
			for (i2 = 0; i2 < NUM_PHILOSOPHERS + SPACING; i2++){
				if (i2 == mid)
					fprintf(stderr, "%c", phils[i].name);
				else
					fprintf(stderr, " ");
			}
			if (NUM_PHILOSOPHERS == 1){
				fprintf(stderr, " ");
			}
			fprintf(stderr, "|\n");
		}
	}
	print_header_struct();
}

void print_states(){
	int i, i2, lim = (int) NUMB_FORKS;
	for (i = 0; i < NUM_PHILOSOPHERS; i++){
		fprintf(stderr, "|");
		fprintf(stderr, " ");
		for (i2 = 0; i2 < lim; i2++){
			if(phils[i].rightFork == i2){
				if(phils[i].isHoldingRightFork)
					fprintf(stderr, "%d", i2);
				else
					fprintf(stderr, "-");
			}else if(phils[i].leftFork == i2){
				if(phils[i].isHoldingLeftFork)
					fprintf(stderr, "%d", i2);
				else
					fprintf(stderr, "-");
			}
			else{
				fprintf(stderr, "-");
			}
		}
		if (phils[i].state == EATING)
			fprintf(stderr, "   EAT");
		else if (phils[i].state == THINKING)
			fprintf(stderr, " THINK");
		else
			fprintf(stderr, "      ");
		fprintf(stderr, " ");
		if (i == NUM_PHILOSOPHERS - 1)
			fprintf(stderr, "|\n");
	}
}

void exit_gracefully(char *msg){
	if (msg != NULL){
		perror(msg);}
	exit(EXIT_FAILURE);
}
