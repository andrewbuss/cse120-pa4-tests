#include "aux.h"
#include "umix.h"
#include "mythreads.h"

void printName(int n)
{
  Printf("I am thread: %d, my argument is: %d\n", MyGetThread(), n);
}

void Main ()
{
  int count = 1;
  MyInitThreads();

  for(; count < MAXTHREADS; ++count)
    {
      Printf("Spawning thread %d\n", count);
      MySpawnThread(printName, count);
    }

  MyExitThread ();
}
