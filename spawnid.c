#include "aux.h"
#include "umix.h"
#include "mythreads.h"


static int size = 0;

void doNothing(int i) {

}

// Spawn a bunch of threads.  Each thread from 0 to MAXTHREADS-1 will get a
// turn to be the one spawning the threads.
void spawnSomeThreads(int p) {
  int i;
  int returnid;
  Printf("Current Thread: %d\n", MyGetThread());
  for (i = 1; i < MAXTHREADS; i++) {
    returnid = MySpawnThread(doNothing, i);
    Printf("Return id is %d\n", returnid);
    MyYieldThread(returnid);
  }

  if (p+1 < MAXTHREADS) {
    MySpawnThread(spawnSomeThreads, p+1);
  }
  MyExitThread();
}

void Main () {
  int i;
  int j;
  MyInitThreads();

  // Spawn some threads using thread 0 and make sure the id is correct
  for (i = 1; i < MAXTHREADS; i++) {
    int returnid = MySpawnThread(doNothing, i);
    Printf("Return id is %d\n", returnid);
    MyYieldThread(returnid);
  }

  MySpawnThread(spawnSomeThreads, 1);
  MyExitThread ();
}
