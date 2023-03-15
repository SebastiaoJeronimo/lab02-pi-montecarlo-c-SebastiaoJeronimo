#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <assert.h>
#include <pthread.h>

int silent = 0;
int debug = 0;

//seed = rand()
//x,seed = pseudorandom.gen(seed)
#define DEBUG(statement) if(debug) statement

/**
 * FROM: https://gist.github.com/diabloneo/9619917
 * @fn timespec_diff(struct timespec *, struct timespec *, struct timespec *)
 * @brief Compute the diff of two timespecs, that is a - b = result.
 * @param a the minuend
 * @param b the subtrahend
 * @param result a - b
 */
static inline void timespec_diff(struct timespec *a, struct timespec *b,
    struct timespec *result) {
    result->tv_sec  = a->tv_sec  - b->tv_sec;
    result->tv_nsec = a->tv_nsec - b->tv_nsec;
    if (result->tv_nsec < 0) {
        --result->tv_sec;
        result->tv_nsec += 1e9L;
    }
}

double timespec_to_milisecs (struct timespec *t) {
  double tf = (double)t->tv_sec + t->tv_nsec / 1e9L;
  return tf;
}

void usage () {
  printf ("Usage: approxPi [-s] [-d] nPoints nThreads\n\
                  -d: Enable debugging… more info is printed on the screen.\n\
                  -s: Silent mode… just print numbers with no explanation\n\
                      of their meaning.\n\
             nPoints: The total number of points to be used in the simulation\n\
                      (to be divided by the working threads).\n\
            nThreads: The number of working threads.");
  exit (1);
}

/* unsigned long myrand_r (unsigned long *seed) {
  *seed = (*seed >> 1) | ((*seed & 1)<<(sizeof(*seed)-1));
  return *seed;
}  00000001} */

void *worker (void *arg) {
  unsigned long nP = *(unsigned long*)arg;
  unsigned long *in = malloc(sizeof(*in));
  *in = 0;
  DEBUG(printf ("  Worker: %ld\n", nP));
  // set seed 
  // main loop
  // make a pseudo random generator
  int x = rand();
  int y = rand();
  //same formula as the java one
  DEBUG(printf ("     Win: %ld\n", *in));
  return (void *)in;
}

double calculate_pi (long nPoints, int nThreads) {
  assert (nPoints > 0);
  assert (nThreads > 0);

  long in = 0;
  // launch threads  
  // wait for threads / get results
  int i = 0;
  pthread_t thread_ids[nThreads]; //instanciate an array for the threads ids
  /**
   * //have to compute how many points are each thread going to take
   * 1 -> do the rest 
   *      ex:  int rest = Points%nThreads 
   * 2 -> divide the points per threads 
   *      ex : int pointsPerThread = nPoints/nThreads
   * 3 -> the last thread when created will have the points per thread plus the rest
  */
  while (i < nThreads)
  {
    // id of thread , atributes NULL in most cases , name of function to execute , 1 corresponds to argument
    pthread_create(&thread_ids[i], NULL, worker, 1);
    i++;
  }
  i = 0;
  int *aux_ptr;
  while (i < nThreads)
  {
    *aux_ptr = 0;
    //pthread_join sends value to aux_ptr
    pthread_join(thread_ids[i], &aux_ptr); 
    in += *aux_ptr;
    i++;
  }
  return (double)in * 4 / nPoints;
}


int main (int argc, char *argv[]) {
  
  // process command line flags  
  int ch;
  while ((ch = getopt(argc, argv, "sd")) != -1) {
    switch (ch) {
      case 's':
                silent = 1;
                break;
      case 'd':
                debug = 1;
                break;
      case '?':
      default:
                usage();
    }
  }
  argc -= optind;
  argv += optind;
  
  // get arguments
  if (argc != 2)
    usage ();
  long nPoints = atol (argv[0]);
  int nThreads = atoi (argv[1]);
  DEBUG(printf (" nPoints: %ld\n", nPoints));
  DEBUG(printf ("nThreads: %d\n", nThreads));
  
  // start timers for both read and CPU time
  struct timespec beginReal, beginCPU;
  clock_gettime (CLOCK_REALTIME, &beginReal);
  clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &beginCPU);
  
  double myPI = calculate_pi (nPoints, nThreads);
  
  // stop timers
  struct timespec endReal, endCPU;
  clock_gettime (CLOCK_REALTIME, &endReal);
  clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &endCPU);
  
  // calculate difference
  struct timespec timeReal, timeCPU;
  timespec_diff (&endReal, &beginReal, &timeReal);
  timespec_diff (&endCPU, &beginCPU, &timeCPU);

  // print times
  if (silent) {
    DEBUG(printf ("nPnt\tnThr\tReal\tCPU\tπ\n"));
    printf ("%ld\t%d\t%0.6lf\t%0.6lf\t%0.9lf\n", nPoints, nThreads, 
            timespec_to_milisecs(&timeReal), timespec_to_milisecs(&timeCPU),myPI);
  } else {
    printf ("\nπ: %0.9lf\n\n", myPI);
    printf ("Real Time:\t%0.6lf\n", timespec_to_milisecs(&timeReal));
    printf (" CPU Time:\t%0.6lf\n", timespec_to_milisecs(&timeReal));
  }
}