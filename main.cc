#include <cstdlib>
#include <cstdio>
#include <iostream>

extern "C" {
#include "header.h"
};

#include "perf.hh"

static const size_t n_funcs = sizeof(funcs)/sizeof(funcs[0]);
static const uint32_t n_iters = 1U<<20;

int main() {
  //setup_performance_counters();
  perf_counter pc;
  pc.enable_counter();
  for(size_t i = 0; i < n_funcs; i++) {
    uint64_t c = pc.read_counter();
    funcs[i](1, n_iters);
    c = pc.read_counter() - c;
    std::cout << (i+1) << "," << c << "\n";
  }
  
  return 0;
}
