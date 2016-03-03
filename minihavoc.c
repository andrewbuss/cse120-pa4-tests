#include <stdlib.h>
#include "aux.h"
#include "umix.h"
#include "mythreads.h"

#define MAX_SPAWN 1000
#define NUM_ACTIONS 5
#define MIN_ACTIONS 100
#define MAX_ACTIONS 500

static int spawned = 0;

void nRandomActions();

void mhDoPrint(int n) {
  Printf("%d: mhDoPrint(%d)\n", MyGetThread(), n);
  Printf("%d\n", n);
}

void mhDoSpawn(int n) {
  int i;
  Printf("%d: mhDoSpawn(%d)\n", MyGetThread(), n);
  for (i = 0; i < n; i++) {
    if (spawned++ < MAX_SPAWN) {
      MySpawnThread(nRandomActions,
          rand() % (MAX_ACTIONS - MIN_ACTIONS) + MIN_ACTIONS);
    }
  }
}

void mhDoYield(int n) {
  Printf("%d: mhDoYield(%d)\n", MyGetThread(), n);
  MyYieldThread(n);
}

void mhDoSched(int n) {
  Printf("%d: mhDoSched(%d)\n", MyGetThread(), n);
  MySchedThread();
}

void mhDoExit(int n) {
  Printf("%d: mhDoExit(%d)\n", MyGetThread(), n);
  MyExitThread();
}

static void (*actions[NUM_ACTIONS])(int) = {
  mhDoPrint, mhDoSpawn, mhDoYield, mhDoSched, mhDoExit
};

void nRandomActions(int n) {
  int i;
  Printf("%d: nRandomActions(%d)\n", MyGetThread(), n);
  for (i = 0; i < n; i++)
    actions[rand() % NUM_ACTIONS](rand() % (MAXTHREADS - 1) + 1);
}

void Main() {
  int stackvar = -5;
  int i;
  MyInitThreads();

  for (i = 1; i < MAXTHREADS; i++) {
    if (spawned++ < MAX_SPAWN) {
      MySpawnThread(nRandomActions,
          rand() % (MAX_ACTIONS - MIN_ACTIONS) + MIN_ACTIONS);
    }
  }

  MyExitThread ();
}
