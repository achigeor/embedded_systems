#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

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
    timestamps[samples-iteration+1]=timestamp.tv_sec*1000+timestamp.tv_usec/1000; // store timestamps in milliseconds

    iteration--;
    signal (sig, catch_alarm);
    /* ualarm can only hold values up to 1.000.000, i.e. 1 sec*/
    if (sampling_period<1){
        ualarm(sampling_period*1000000,0);
    }
    else{
        alarm(sampling_period);
    }}


int main (int argc,char *argv[])
{
    /* Establish a handler for SIGALRM signals. */
    signal (SIGALRM, catch_alarm);
    iteration= atoi(argv[1]);
    samples = atoi(argv[1]); // pollaplasia tou 5 vgazoun koulo error??
    timestamps= (double *) malloc((samples)*sizeof(double));
    sampling_period=atof(argv[2]);

    printf("to samples einai %d kai to sampling_period einai %f \n",samples, sampling_period);
    gettimeofday(&timestamp,NULL);
    timestamps[0]=timestamp.tv_sec*1000+timestamp.tv_usec/1000; // store timestamps in milliseconds

    /* Set an alarm to go off in <sampling_period> seconds. */
    /* ualarm can only hold values up to 1.000.000, i.e. 1 sec*/
    if (sampling_period<1){
        ualarm(sampling_period*1000000,0);
    }
    else{
        alarm(sampling_period);
    }

    /* Check the flag once in <sampling_period> seconds to see when to quit. */
    while (iteration){
        usleep(sampling_period*1000000); // we sleep while counting to save resources!
    }

    FILE *f = fopen("time_deltas.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    int i;
    for(i=1; i<=samples; i++){
        fprintf(f, "Sample: %d, delta: %f\n", i, timestamps[i]-timestamps[i-1]);
    }

    free(timestamps);
    return 0;

}