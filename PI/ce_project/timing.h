#if !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 199309L
#endif

#ifndef TIMINGH
#define TIMINGH

#include <time.h>

double getTimeStamp();

double getTimeResolution();

double getTimeStamp_();

void dummy(double* a);

#endif