#include "aux.h"
#include "umix.h"
#include "mythreads.h"

void printName(int n)
{
  int count = 0;
  for (; count < 3; ++count)
    {
      Printf("I am thread: %d, my argument is: %d\n", MyGetThread(), n);
      MyYieldThread(MyGetThread());
    }
}

void Main ()
{
  int count = 1;
  MyInitThreads();

  for(; count < 3; ++count)
    {
      Printf("Spawning thread %d\n", count);
      MySpawnThread(printName, count);
    }

  MyExitThread ();
}
