#include "aux.h"
#include "umix.h"
#include "mythreads.h"

void printName(int n) {
  int stackvar = n * 2;
  Printf("%d: printName(%d) -> %d\n", MyGetThread(), n, stackvar);
}

void spawnPrint(int n) {
  int stackvar = n * 3;
  Printf("%d: spawnPrint(%d) -> %d\n", MyGetThread(), n, stackvar);
  MySpawnThread(printName, n + 1);
}

void Main() {
  int count;
  MyInitThreads();

  // We need one more slot for threads to spawn threads
  for(count = 1; count < MAXTHREADS - 1; count++) {
    Printf("Spawning thread %d\n", count);
    MySpawnThread(spawnPrint, count);
  }

  MyExitThread ();
}
