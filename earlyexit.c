#include "aux.h"
#include "umix.h"
#include "mythreads.h"

void yieldTo(int n) {
  int stackvar = n * 2;
  Printf("%d: [0] yieldTo(%d) -> %d\n", MyGetThread(), n, stackvar);
  Printf("    Yielding: %d -> %d\n", MyGetThread(), n);
  Printf("    Yielded: %d -> %d\n", MyYieldThread(n), MyGetThread());
  MyExitThread ();
  Printf("%d: [1] yieldTo(%d) -> %d\n", MyGetThread(), n, stackvar);
  Printf("    Scheding: %d -> ?\n", MyGetThread());
  MySchedThread();
  Printf("%d: [2] yieldTo(%d) -> %d\n", MyGetThread(), n, stackvar);
}

void Main() {
  int stackvar = -5;
  MyInitThreads();

  MySpawnThread(yieldTo, 1);
  MySpawnThread(yieldTo, 0);
  MySpawnThread(yieldTo, 4);
  MySpawnThread(yieldTo, 2);

  Printf("%d: [0] yieldTo(%d) -> %d\n", MyGetThread(), 3, stackvar);
  Printf("    Yielding: %d -> %d\n", MyGetThread(), 3);
  MyYieldThread(3);
  Printf("%d: [1] yieldTo(%d) -> %d\n", MyGetThread(), 3, stackvar);
  Printf("    Scheding: %d -> ?\n", MyGetThread());
  MySchedThread();
  Printf("%d: [2] yieldTo(%d) -> %d\n", MyGetThread(), 3, stackvar);

  MySpawnThread(yieldTo, 4);
  MySpawnThread(yieldTo, 7);
  MySpawnThread(yieldTo, 8);
  MySpawnThread(yieldTo, 6);

  MyExitThread ();
}
