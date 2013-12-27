#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUMTHRDS 1
#define VECLEN 100000000

/*
The following structure contains the necessary information
to allow the function "dotprod" to access its input data and
place its output into the structure.  This structure is
unchanged from the sequential version.
*/

typedef struct
 {
   float      *a;
   float      maxval;
   float      minval;
   long       nval[NUMTHRDS];
   long       veclen;
 } THRESHDATA;

/* Define globally accessible variables and a mutex */


THRESHDATA threshData;
pthread_t analysisThread[NUMTHRDS];
pthread_mutex_t mutexThresh;

/*
The function dotprod is activated when the thread is created.
As before, all input to this routine is obtained from a structure
of type DOTDATA and all output from this function is written into
this structure. The benefit of this approach is apparent for the
multi-threaded program: when a thread is created we pass a single
argument to the activated function - typically this argument
is a thread number. All  the other information required by the
function is accessed from the globally accessible structure.
*/

void *threshold(void *arg)
{

/* Define and use local variables for convenience */

   long offset, i, start, end, len, nval;
   float *x, minv, maxv, val;
   offset = (long)arg;

   len = threshData.veclen;
   start = offset*len;
   end   = start + len;
   x = threshData.a;
   minv = threshData.minval;
   maxv = threshData.maxval;
   long* result = (long*) malloc( len*sizeof(long) );

/*
Perform the dot product and assign result
to the appropriate variable in the structure.
*/
   nval = 0;
   for (i=start; i<end ; i++) {
	  val = x[i];
	  if( ( val <= maxv ) && (val >= minv ) ) {
		  result[ nval ] = i;
		  nval++;
	  }
   }
   threshData.nval[ offset ] = nval;

/*
Lock a mutex prior to updating the value in the shared
structure, and unlock it upon updating.
*/
//   pthread_mutex_lock (&mutexThresh);
//   threshData.sum += mysum;
//   printf("Thread %ld did %d to %d:  mysum=%f global sum=%f\n",offset,start,end,mysum,threshData.sum);
//   pthread_mutex_unlock (&mutexThresh);

   printf ("nval = %ld / %ld ( %.2f )\n", nval, len, nval/(float)len );

   free ( result );
   pthread_exit((void*) 0);
}

/*
The main program creates threads which do all the work and then
print out result upon completion. Before creating the threads,
The input data is created. Since all threads update a shared structure, we
need a mutex for mutual exclusion. The main thread needs to wait for
all threads to complete, it waits for each one of the threads. We specify
a thread attribute value that allow the main thread to join with the
threads it creates. Note also that we free up handles  when they are
no longer needed.
*/

int thresholdFunction() {

	long i;
	float *a;
	void *status;
//	pthread_attr_t attr;

	/* Assign storage and initialize values */

	int local_veclen = VECLEN / NUMTHRDS;

	a = (float*) malloc (VECLEN*sizeof(float));

	for (i=0; i<VECLEN; i++) {
	  a[i] = rand()/(float)RAND_MAX;;
	}

	threshData.veclen = local_veclen;
	threshData.a = a;
	threshData.minval = 0.4;
	threshData.maxval = 0.6;

	clock_t t;
	t = clock();

//	pthread_mutex_init(&mutexThresh, NULL);

	/* Create threads to perform the dotproduct  */
//	pthread_attr_init(&attr);
//	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for(i=0;i<NUMTHRDS;i++) {
	  /* Each thread works on a different set of data.
	   * The offset is specified by 'i'. The size of
	   * the data for each thread is indicated by VECLEN.
	   */
//	   pthread_create(&analysisThread[i], &attr, threshold, (void *)i);
	   pthread_create(&analysisThread[i], NULL, threshold, (void *)i);
	 }

//	pthread_attr_destroy(&attr);
	/* Wait on the other threads */

	for(i=0;i<NUMTHRDS;i++) {
	  pthread_join(analysisThread[i], &status);
	}
	/* After joining, print out the results and cleanup */

	t = clock() - t;
	printf ("The global operation took %lu clicks (%f seconds) with %d threads\n", t, ((float)t)/CLOCKS_PER_SEC, NUMTHRDS );

	free (a);
//	pthread_mutex_destroy(&mutexThresh);
	pthread_exit(NULL);
}



