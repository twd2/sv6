#include "types.h"
#include "kernel.hh"
#include "bits.hh"
#include "cpu.hh"
#include "irq.hh"
#include "kstream.hh"
#include "numa.hh"

#include <algorithm>
#include <iterator>

static console_stream verbose(true);

void
initnuma(void)
{
  verbose.println("numa: assuming single NUMA node");
  numa_nodes.emplace_back(0, 0);
  auto &node = numa_nodes.back();
  node.mems.emplace_back(0, ~0ull);
  for (size_t i = 0; i < ncpu; ++i)
    node.cpuids.push_back(i);
}
