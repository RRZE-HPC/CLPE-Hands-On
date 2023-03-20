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
  double *a, *b, *c;

  if(argc != 2) {
    printf("Usage: %s <size in KiB>\n",argv[0]);
    exit(1);
  }
  size  = atof(argv[1]);
  N     = size * 1024 / 8 / 3;
  size  = (double)N / 1024 * 8 * 3;

  struct perf_event_attr attr;
  int perf_fd = init_perf_event(&attr);
  int err;

  a = (double*)aligned_alloc(64, N*sizeof(double));
  b = (double*)aligned_alloc(64, N*sizeof(double));

  for(int k=0; k<N; ++k) {
    a[k] = 0.00321;
    b[k] = 0.01231;
  }

  NITER = 1;
  long long ncycles_tmp = 0;
  long long ncycles = 0;
  do {
    ncycles_tmp = ncycles;
    // time measurement
    wct_start = getTimeStamp();
    ioctl(perf_fd, PERF_EVENT_IOC_ENABLE, 0);

    // repeat measurement often enough
    for(int k=0; k<NITER; ++k) {
      if (s == 1.20000) dummy(&s);
      // ==================================================
      // benchmark loop
      for(int i=0; i<N; ++i) {
        s = s + a[i] * b[i];
      }
      // end of benchmark loop
      // ==================================================
      if(s<0.) printf("%lf",s);
    }
    ioctl(perf_fd, PERF_EVENT_IOC_DISABLE, 0);
    wct_end = getTimeStamp();
    err = read(perf_fd, &ncycles, sizeof(long long));
    NITER = NITER*2;
  } while (wct_end-wct_start<1.0); // at least 1000 ms

  NITER = NITER/2;

  err = read(perf_fd, &ncycles, sizeof(long long));
  if (err < 0) {
    return 1;
  }
  printf("Size: %.2f kB, %d elements\n", size, N);
  printf("Cycles per high-level iteration: %f\n", (double)(ncycles-ncycles_tmp)/NITER/N);
  printf("Total walltime: %f, NITER: %d\n",wct_end-wct_start,NITER);

  free(a);
  free(b);
  return 0;
}