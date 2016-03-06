#include <stdlib.h>
#include "aux.h"
#include "umix.h"
#include "mythreads.h"

#define NUM_STEPS 10000

static int steps = 0;

void f(int p) {
  if(steps++ >= NUM_STEPS) return;
  int iterations = 4 + rand() % 10;
  Printf("%d: step %06d f(%08X): %d\n", MyGetThread(), steps, p, iterations);
  for(int i = 0; i < iterations; i++) {
    p ^= rand();
    int rv = 0;
    int action = rand() % 4;
    Printf("%d: %d..\n", MyGetThread(), action);
    switch(action) {
    case 0: rv = MySpawnThread(f, p); break;
    case 1: rv = MyYieldThread(rand() % MAXTHREADS); break;
    case 2: MySchedThread(); break;
    case 3: if(!(rand() % 40)) MyExitThread(); break;
    }
    Printf("%d: ..%d\n", MyGetThread(), rv);
  }
}
      
void Main() {
  srand(0x123d09e);
  MyInitThreads();

  f(0);
  MyExitThread();
}
