#include <cstdlib>
#include <cstdio>
#include <iostream>

extern "C" {
#include "header.h"
};

#include "m1cycles.hh"

static const size_t n_funcs = sizeof(funcs)/sizeof(funcs[0]);
static const uint32_t n_iters = 1U<<20;

int main() {
  setup_performance_counters();

  for(size_t i = 0; i < n_funcs; i++) {
    performance_counters c0 = get_counters();
    funcs[i](1, n_iters);
    performance_counters c1 = get_counters();
    c1.missed_branches -= c0.missed_branches;
    c1.branches -= c0.branches;

    //subtract out dummy branches and loop backedge
    c1.branches -= n_iters * (i+2); 

    double r = static_cast<double>(c1.missed_branches) / c1.branches;
    std::cout << i << "," << r << "\n";
    
  }
  
  return 0;
}
