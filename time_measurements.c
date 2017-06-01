#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

/*
 * Measuring time difference given
 * a number of samples and the sampling period
 */

/* This flag controls termination of the main loop. */
volatile sig_atomic_t iteration;
struct timeval timestamp;
double *timestamps;
int samples;
double sampling_period;

/* The signal handler just clears the flag and re-enables itself. */
void catch_alarm (int sig)
{
    gettimeofday(&timestamp,NULL);
    timestamps[samples-iteration]=timestamp.tv_sec*1000+timestamp.tv_usec/1000; // store timestamps in milliseconds

    iteration--;
    signal (sig, catch_alarm);
    /* ualarm can only hold values up to 1.000.000, i.e. 1 sec*/
    if (sampling_period <1){
        ualarm(sampling_period*1000000,0);
    }
    else{
        alarm(sampling_period);
    }}


int main (int argc,char *argv[])
{
    if (argc != 4) { // Check if the command line arguments are correct
        printf("Usage: %s S SP FN\n"
                       "where\n"
                       "  S     : number of samples\n"
                       "  SP    : sampling period\n"
                       "  FN    : txt filename to save output\n",argv[0]);
        return (1);
    }

    /* Establish a handler for SIGALRM signals. */
    signal (SIGALRM, catch_alarm);
    iteration= atoi(argv[1]);
    samples = atoi(argv[1]); // pollaplasia tou 5 vgazoun koulo error??
    timestamps= (double *) malloc((samples)*sizeof(double));
    sampling_period=atof(argv[2]);
    char* filename = argv[3];

    printf("You chose %d samples and %f seconds as sampling_period \n",samples, sampling_period);

    /* Set an alarm to go off in <sampling_period> seconds. */
    /* ualarm can only hold values up to 1.000.000, i.e. 1 sec*/
    if (sampling_period <1){ // todo make it work with doubles more than 1 sec
        ualarm(sampling_period*1000000,0);
    }
    else{
        alarm(sampling_period);
    }

    /* Check the flag once in <sampling_period> seconds to see when to quit. */
    while (iteration && sampling_period<1){
        usleep(sampling_period*1000000); // we sleep while counting to save resources!
    }
    while (iteration && sampling_period>=1){
        sleep(sampling_period); // we sleep while counting to save resources!
    }

    FILE *f = fopen("time_deltas.txt", "w");
    FILE *f2 = fopen(filename, "w");
    if (f == NULL || f2 ==NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    int i;
    for(i=1; i<=samples-1; i++){
        fprintf(f, "Time difference between sample %d and %d, delta: %f\n", i,i-1, timestamps[i]-timestamps[i-1]);
        fprintf(f2,"%f\n", timestamps[i]-timestamps[i-1]);
    }

    free(timestamps);
    return 0;

}