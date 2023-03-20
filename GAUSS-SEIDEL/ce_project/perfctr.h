#ifndef PERFCTRH
#define PERFCTRH

#include <linux/perf_event.h>

int perfevent_paranoid_value();
int init_perf_event(struct perf_event_attr *attr);

#endif