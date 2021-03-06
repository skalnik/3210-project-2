#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "buzz.h"

#define THREADCOUNT 100
#define GOLD 30
#define BLACK THREADCOUNT-GOLD
#define THREADPOOL 5

#define TIMEOUT 10  // in us
#define ACTIVITY 5 // in us
buzz_t GTLOCK;

void thread(){
  struct timeval t1,t2;
  double elapsedTime;
  int ID = omp_get_thread_num();

  // SET COLOR OF LOCK HERE Depending on condiion
  if(ID<=GOLD) buzz_color(BZZ_GOLD,GTLOCK);
  else         buzz_color(BZZ_BLACK,GTLOCK);

  gettimeofday(&t1,NULL);
  bzz_lock(GTLOCK);
  usleep(ACTIVITY); // ACTIVITY
  bzz_release(GTLOCK);
  gettimeofday(&t2,NULL);

  elapsedTime = (t2.tv_sec - t1.tv_sec);
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000000.0;

  if(ID<=GOLD) printf("GOLD = %f\n",elapsedTime);
  else         printf("BLACK = %f\n",elapsedTime);
}


int main(){
  omp_set_num_threads(THREADCOUNT);
  init_bzz(GTLOCK,THREADPOOL,TIMEOUT);
  #pragma omp parallel
  {
    thread();
  }
  bzz_kill(GTLOCK);
}
