#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "aux.h"
#include "umix.h"
#include "mythreads.h"

#ifndef NUM_STEPS
#define NUM_STEPS 200000
#endif

// rand() seems to only return 16 bits on solaris
#define rand() ((rand() << 16) ^ rand())

static int steps = 0;

// from https://en.wikipedia.org/wiki/Fletcher%27s_checksum
uint32_t fletcher32( uint16_t const *data, size_t words ) {
  uint32_t sum1 = 0xffff, sum2 = 0xffff;
  size_t tlen;
 
  while (words) {
    tlen = words >= 359 ? 359 : words;
    words -= tlen;
    do {
      sum2 += sum1 += *data++;
    } while (--tlen);
    sum1 = (sum1 & 0xffff) + (sum1 >> 16);
    sum2 = (sum2 & 0xffff) + (sum2 >> 16);
  }
  /* Second reduction step to reduce sums to 16 bits */
  sum1 = (sum1 & 0xffff) + (sum1 >> 16);
  sum2 = (sum2 & 0xffff) + (sum2 >> 16);
  return sum2 << 16 | sum1;
}

#ifndef CENTIHAVOC_VERBOSE
#define dprintf(...)
#else
#define dprintf printf
#endif

void f(int p) {
  int tid = MyGetThread();
  int iterations = 4 + rand() % 100;
  int xs_count = 30000;
  int checksum_len = xs_count + (sizeof(int)*4)/2;
  checksum_len = xs_count;
  uint16_t xs[xs_count];
  int checksum = fletcher32(xs, checksum_len);
  int i, rv, action;

  if(steps++ >= NUM_STEPS) return;

  printf("%d: step %06d f(%08X): %d\n", tid, steps, p, iterations);
  for(i = 0; i < iterations; i++) {
    p ^= rand();
    action = rand() % 5;
    int target = rand() % MAXTHREADS;
    switch(action) {
    case 0: {      
      int new_tid = MySpawnThread(f, p);
      dprintf("%d: + %d\n", tid, new_tid); break;
    }
    case 1: 
      dprintf("%d: -> %d\n", tid, target);
      int yielder = MyYieldThread(target);
      dprintf("%d: <- %d\n", tid, yielder);
      break;
    case 2:
      dprintf("%d: -> ?\n", tid);
      MySchedThread();
      break;
    case 3:
      if(!(rand() % 2000)) {
	dprintf("%d: exiting after %d iterations\n", tid, iterations);
	MyExitThread();
      }
      break;
    case 4:
      if(!(rand() % 10000)){
	int cur_check = fletcher32(xs, checksum_len);

	if(cur_check != checksum) {
	  printf("%d: checksum was %08X; expected %08X\n", tid, cur_check, checksum);
	  exit(-1);
	}

	for(int j = 0; i < xs_count; i++) {
	  xs[j] ^= rand();
	}
	
	checksum = fletcher32(xs, checksum_len);
	break;
      }
    }
  }
  if(tid != MyGetThread()) {
    printf("%d: TID value %d doesn't match thread ID\n", MyGetThread(), tid);
    exit(-1);
  }
  printf("%d: finished %d\n", tid, iterations);
}
      
void Main() {
  srand(1-800-120-0xDEED);
  MyInitThreads();

  f(0);
  MyExitThread();
}
