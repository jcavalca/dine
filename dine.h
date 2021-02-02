/* defines number of philosophers */
#ifndef NUM_PHILOSOPHERS 
#define NUM_PHILOSOPHERS 5
#endif

/* defines possible states for a philosopher*/
#define THINKING 0
#define EATING 1
#define CHANGING 2

/* defines boolen*/
#define FALSE 0
#define TRUE 1

#define FIRST_INDEX 0
#define DEFAULT_CYCLES 1

typedef struct Philosopher
{
    char name;
    pthread_t *thread_id;       
    uint8_t state;              
    uint8_t isRightHanded;      /*to allow existence of 
                                left-handed philosopher*/
    struct Fork *target1;
    struct Fork *target2;
    uint8_t isHungry;

} Philosopher;

typedef struct Fork
{
    Philosopher *owner;        /*NULL if it's not picked up yet*/
} Fork;
