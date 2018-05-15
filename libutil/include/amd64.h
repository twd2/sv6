#pragma once
// Routines to let C code use special x86 instructions.

#include <stddef.h>
#include <stdint.h>

static inline void
nop_pause(void)
{
  // doing nothing is okay, but in x86, `pause` is recommended to save energy
  // but RISC-V seems do not have the corresponding instruction.
  // -- twd2
}

// TODO: move out

// Atomically set bit nr of *a.  nr must be <= 64.
static inline void
locked_set_bit(int nr, volatile uint64_t *a)
{
  uint64_t mask = 1UL << nr;
  __atomic_fetch_or(a, mask, __ATOMIC_ACQ_REL);
}

// Atomically clear bit nr of *a.  nr must be <= 64.
static inline void
locked_clear_bit(int nr, volatile uint64_t *a)
{
  uint64_t mask = ~(1UL << nr);
  __atomic_fetch_and(a, mask, __ATOMIC_ACQ_REL);
}

// Atomically set bit nr of *a and return its old value
static inline int
locked_test_and_set_bit(int nr, volatile uint64_t *a)
{
  uint64_t mask = 1UL << nr;
  uint64_t old = __atomic_fetch_or(a, mask, __ATOMIC_ACQ_REL) & mask;
  return old != 0;
}

// Clear bit nr of *a.  On the x86, this can be used to release a lock
// as long as nothing else can concurrently modify the same byte.
static inline void
clear_bit(int nr, volatile uint64_t *a)
{
  uint64_t mask = ~(1UL << nr);
  *a &= mask;
}

enum {
  FXSAVE_BYTES = 512
};


