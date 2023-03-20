#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <linux/perf_event.h>    /* Definition of PERF_* constants */
#include <linux/hw_breakpoint.h> /* Definition of HW_* constants */
#include <sys/syscall.h>         /* Definition of SYS_* constants */
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "timing.cpp"
#include "perfctr.cpp"


int main(int argc, char** argv) {

  double wct_start,wct_end;
  double eps = 1.0e-12;
  int NITER, NI, NK;

  if(argc != 3) {
    printf("Usage: %s <NI> <NK>\n",argv[0]);
    exit(1);
  }
  
  NI = atoi(argv[1]);
  NK = atoi(argv[2]);
  printf("NI = %d, NK = %d\n", NI, NK);
  double phi [NI][NK];
  
  double size = (double)NI*NK/1024*sizeof(double);

  int pl = perfevent_paranoid_value();
  //printf("Paranoia level: %d\n", pl);
  // manual perf_event handling
  struct perf_event_attr attr;
  int perf_fd;
  memset(&attr, 0, sizeof(struct perf_event_attr));
  attr.size = sizeof(struct perf_event_attr);
  attr.disabled = 1;
  attr.exclude_kernel = 1;
  attr.type = PERF_TYPE_HARDWARE;
  attr.config = PERF_COUNT_HW_CPU_CYCLES;
  perf_fd = perf_event_open(&attr, 0, -1, -1, 0);
  if (perf_fd < 0) { // error;
    fprintf(stderr, "Can't open perf_fd (%d)\n", perf_fd);
    return 1;
  }
  for (int i=0; i<NI; ++i) {
    for (int k=0; k<NK; ++k) {
      phi[i][k] = 0.0;
    }
  }
  for (int i=0; i<NI; ++i) {
        phi[i][NK-1] = 1.0;
        phi[i][0] = 1.0;
  }
  for (int k=0; k<NK; ++k) {
        phi[0][k] = (NI+1)/NI;
        phi[NI-1][k] = (NI+1)/NI;
  }

  NITER=1;
  long long diff = 0;
  long long result = 0;

  do {
    diff = result;
    // time measurement
    wct_start = getTimeStamp();
    ioctl(perf_fd, PERF_EVENT_IOC_ENABLE, 0);

    // repeat measurement often enough
    for(int it=0; it<NITER; ++it) {
      // benchmark loop
      for (int i=1; i<NI-1; ++i) {
          for (int k=1; k<NK-1; ++k) {
              phi[i][k] = 0.25 * (
                  phi[i][k-1] + phi[i+1][k]
                + phi[i][k+1] + phi[i-1][k]
              );
          }
      }
      // end of benchmark loop
    }
    ioctl(perf_fd, PERF_EVENT_IOC_DISABLE, 0);
    wct_end = getTimeStamp();
    int err = read(perf_fd, &result, sizeof(long long));
    NITER = NITER*2;
  } while (wct_end-wct_start<1.0); // at least 1000 ms

  NITER = NITER/2;
  int err = read(perf_fd, &result, sizeof(long long));
  if (err < 0) {
    return 1;
  }
  printf("Size: %.2f kB, %d elements\n", size, NI*NK);
  printf("Cycles per high-level iteration: %f\n", (double)(result-diff)/NITER/NI/NK);
  printf("Total walltime: %f, NITER: %d\n",wct_end-wct_start,NITER);

  return 0;
}