#include <linux/perf_event.h>    /* Definition of PERF_* constants */
#include <linux/hw_breakpoint.h> /* Definition of HW_* constants */
#include <sys/syscall.h>         /* Definition of SYS_* constants */
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "perfctr.h"

static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid, int cpu, int group_fd, unsigned long flags) {
    int ret;

    ret = syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);

    return ret;
}

int init_perf_event(struct perf_event_attr *attr)
{
    memset(attr, 0, sizeof(struct perf_event_attr));
    attr->size = sizeof(struct perf_event_attr);
    attr->disabled = 1;
    attr->exclude_kernel = 1;
    attr->type = PERF_TYPE_HARDWARE;
    attr->config = PERF_COUNT_HW_CPU_CYCLES;

    int perf_fd = perf_event_open(attr, 0, -1, -1, 0);
    if (perf_fd < 0) { // error;
        fprintf(stderr, "Can't open perf_fd (%d)\n", perf_fd);
        exit(1);
    }
    return perf_fd;
}

int perfevent_paranoid_value()
{
    FILE* fd;
    int paranoid = 3;
    char buff[100];
    fd = fopen("/proc/sys/kernel/perf_event_paranoid", "r");
    if (fd == NULL)
    {
        errno = EPERM;
        fprintf(stderr, "Linux kernel has no perf_event support. Cannot access /proc/sys/kernel/perf_event_paranoid\nERRNO: %d", errno);
        return paranoid;
    }
    size_t read = fread(buff, sizeof(char), 100, fd);
    if (read > 0)
    {
        paranoid = atoi(buff);
    }
    fclose(fd);
    return paranoid;
}