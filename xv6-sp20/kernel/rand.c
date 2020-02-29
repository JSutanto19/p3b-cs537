#include "rand.h"

struct xorshift32_state {
   unsigned int a;
};

unsigned int globalSeed = 1;
//unsigned int a = 123456789;
//unsigned int b = 362436069;
//unsigned int c = 521288629;

/* The state word must be initialized to non-zero */
 int xorshift32(void)
 {
  /* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
  unsigned int x = globalSeed;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  globalSeed = x;
  return (unsigned int) (globalSeed % XV6_RAND_MAX);
     
 }

int xv6_rand(){
 int randNum = xorshift32();
  if(randNum >= 0 && randNum <= XV6_RAND_MAX){
    return randNum;
  }
  return -1;
}

void xv6_srand(unsigned int seed){
  globalSeed = seed;
}


