#ifndef __perfhh__
#define __perfhh__

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif



#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/time.h>
#include <cassert>
#include <cstdint>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/unistd.h>
#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>
#include <sys/ioctl.h>

class perf_counter {
private:
  int fd;
  struct perf_event_attr pe;
public:
  perf_counter() : fd(-1) {
    memset(&pe, 0, sizeof(pe));
    pe.type = PERF_TYPE_HARDWARE;
    pe.size = sizeof(pe);
    pe.config = PERF_COUNT_HW_BRANCH_MISSES;
    pe.disabled = 1;
    //pe.exclude_kernel = 1;
    //pe.exclude_hv = 1;
    int cpu = sched_getcpu();
    fd = syscall(__NR_perf_event_open, &pe, 0, cpu, -1, 0);
    if(fd < 0) perror("WTF");
    assert(fd > -1);
  }
  ~perf_counter() {
    close(fd);
  }
  void reset_counter() {
    ioctl(fd, PERF_EVENT_IOC_RESET, 0);
  }
  void enable_counter() {
    ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);
  }
  void disable_counter() {
    ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
  }
  uint64_t read_counter() {
    uint64_t count = 0;
    ssize_t rc = read(fd, &count, sizeof(count));
    assert(rc != -1);
    return count;
  }

};

#endif

