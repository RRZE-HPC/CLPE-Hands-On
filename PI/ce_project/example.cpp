#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "timing.cpp"
#include "perfctr.cpp"


int main(int argc, char** argv) {
  double wct_start,wct_end;
  int NITER, N;
  double size;
  double s = 0.0;

  if(argc != 2) {
    printf("Usage: %s <#elements>\n",argv[0]);
    exit(1);
  }
  N = atof(argv[1]);
  double delta_x = 1./N;
  double x;

  struct perf_event_attr attr;
  int perf_fd = init_perf_event(&attr);
  int err;

  NITER = 1;
  long long ncycles = 0;
  // time measurement
  wct_start = getTimeStamp();
  ioctl(perf_fd, PERF_EVENT_IOC_ENABLE, 0);

  // ==================================================
  // benchmark loop
  for(int i=0; i<N; ++i) {
    x   = (i + 0.5) * delta_x;
    s += (4.0 / (1.0 + x * x));
  }
  // end of benchmark loop
  // ==================================================
  if(s<0.) printf("%lf", s);
  ioctl(perf_fd, PERF_EVENT_IOC_DISABLE, 0);
  wct_end = getTimeStamp();
  
  err = read(perf_fd, &ncycles, sizeof(long long));
  if (err < 0) {
    return 1;
  }
  double pi = s * delta_x;
  printf("PI = %.15f\n", pi);
  printf("%d elements\n", N);
  printf("Cycles per high-level iteration: %f\n", (double)(ncycles)/N);
  printf("Total walltime: %f\n", wct_end-wct_start);

  return 0;
}