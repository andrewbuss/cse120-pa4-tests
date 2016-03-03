#include <stdlib.h>
#include "aux.h"
#include "umix.h"
#include "mythreads.h"

#define MAX_SPAWN 100000
#define NUM_ACTIONS 4
#define MIN_ACTIONS 10000
#define MAX_ACTIONS 50000

static int spawned = 0;

void nRandomActions();

void mhDoPrint(int n) {
  int stackvar = rand() % 50 + n * 50;
  Printf("%d: mhDoPrint(%d) ~ %d\n", MyGetThread(), n, stackvar);
  Printf("%d\n", n);
}

void mhDoSpawn(int n) {
  int i;
  int stackvar = rand() % 50 + n * 50;
  Printf("%d: mhDoSpawn(%d) ~ %d\n", MyGetThread(), n, stackvar);
  for (i = 0; i < n; i++) {
    if (spawned++ < MAX_SPAWN) {
      MySpawnThread(nRandomActions,
          rand() % (MAX_ACTIONS - MIN_ACTIONS) + MIN_ACTIONS);
    }
  }
}

void mhDoYield(int n) {
  int stackvar = rand() % 50 + n * 50;
  Printf("%d: mhDoYield(%d) ~ %d\n", MyGetThread(), n, stackvar);
  MyYieldThread(n);
}

void mhDoSched(int n) {
  int stackvar = rand() % 50 + n * 50;
  Printf("%d: mhDoSched(%d) ~ %d\n", MyGetThread(), n, stackvar);
  MySchedThread();
}

void mhDoExit(int n) {
  int stackvar = rand() % 50 + n * 50;
  Printf("%d: mhDoExit(%d) ~ %d\n", MyGetThread(), n, stackvar);
  MyExitThread();
}

// You can try exiting as well by uncommenting and changing NUM_ACTIONS
// Without exit, you get a lot more actions
static void (*actions[NUM_ACTIONS])(int) = {
  mhDoPrint, mhDoSpawn, mhDoYield, mhDoSched//, mhDoExit
};

void nRandomActions(int n) {
  int i;
  int stackvar = rand() % 50 + n * 50;
  Printf("%d: nRandomActions(%d) ~ %d\n", MyGetThread(), n, stackvar);
  for (i = 0; i < n; i++)
    actions[rand() % NUM_ACTIONS](rand() % (MAXTHREADS - 1) + 1);
}

void Main() {
  int stackvar = rand() % 50;
  int i;
  srand(0xdeadbeef);
  MyInitThreads();

  for (i = 1; i < MAXTHREADS; i++) {
    Printf("%d: Spawning thread %d ~ %d\n", MyGetThread(), i, stackvar);
    if (spawned++ < MAX_SPAWN) {
      MySpawnThread(nRandomActions,
          rand() % (MAX_ACTIONS - MIN_ACTIONS) + MIN_ACTIONS);
    }
  }

  MyExitThread ();
}
