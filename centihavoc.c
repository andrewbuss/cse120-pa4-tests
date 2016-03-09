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

int checksum(int *data, size_t count ) {
  int sum = 0;
  for(int i = 0; i < count; i++){
    sum ^= data[i];
  }
  return sum;
}

#ifndef CENTIHAVOC_VERBOSE
#define dprintf(...)
#else
#define dprintf printf
#endif

int xs_count = 15000;

void f(int p) {
  // guard the frame of f with a buffer on one end and the checksum on the other
  int xs[xs_count];
  int tid = MyGetThread();
  int iterations = 4 + rand() % 100;
  int checksum_len = xs_count + 2;
  int i, rv, action, new_tid;
  int sum = checksum(xs, checksum_len);

  if(steps++ >= NUM_STEPS) return;

  dprintf("%d: step %06d f(%08X): %d\n", tid, steps, p, iterations);
  for(i = 0; i < iterations; i++) {
    p ^= rand();
    action = rand() % 5;
    int target = rand() % MAXTHREADS;
    switch(action) {
    case 0: 
      new_tid = MySpawnThread(f, p);
      dprintf("%d: + %d\n", tid, new_tid); break;
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
      if(!(rand() % 20000)) {
	dprintf("%d: exiting after %d iterations\n", tid, iterations);
	MyExitThread();
      }
      break;
    case 4:
      if(!(rand() % 100)){

	// check that the buffer has the same checksum as before
	int cur_check = checksum(xs, checksum_len);

	if(cur_check != sum) {
	  printf("%d: checksum was %08X; expected %08X\n", tid, cur_check, sum);
	  exit(-1);
	}

	// scramble the buffer
	for(int j = 0; i < xs_count; i++) {
	  xs[j] ^= rand();
	}
	
	sum = checksum(xs, checksum_len);
	break;
      }
    }
  }
  dprintf("%d: finished %d\n", tid, iterations);
}
      
void Main() {
  srand(1-800-120-0xDEED);
  MyInitThreads();

  f(0);
  MyExitThread();
}
