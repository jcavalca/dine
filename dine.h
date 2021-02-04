/* defines possible states for a philosopher*/
#define THINKING 0
#define EATING 1
#define CHANGING 2

/* defines boolen*/
#define FALSE 0
#define TRUE 1

#define ZERO_INDEX 0
#define ONE_INDEX 1
#define DEFAULT_CYCLES 1
#define THREAD_SHARED 1
#define INIT_VALUE 1 /* all forks start on table */
#define SPACING 8 

/*philosopher contents*/
typedef struct Philosopher
{
    char name;
    pthread_t *thread_id;       
    uint8_t state;          
    uint8_t isHungry;
    uint8_t isRightHanded;      /*to allow existence of 
                                left-handed philosopher*/
    int     rightFork;          /*index of forks*/
    int     leftFork;
    uint8_t isHoldingRightFork;
    uint8_t isHoldingLeftFork;

} Philosopher;

