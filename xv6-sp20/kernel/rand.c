#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
#include "rand.h"

struct xorshift32_state {
   unsigned int a;
};

struct xorshift32_state* rand;
rand ->a = 1;

/* The state word must be initialized to non-zero */
 int xorshift32(struct xorshift32_state *state)
 {
  /* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
  unsigned int x = state->a;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  return state->a = x;
     
 }

int xv6_rand(){
 int randNum = xorshift32(rand);
  if(randNum >= 0 && randNum <= xv6_RAND_MAX){
    return randNum;
  }
}

void xv6_srand(unsigned int seed){
  rand -> a = seed;
}




