#include <stdlib.h>
#include <stdio.h>
#include "os.h"

class MyThread: public MThread
{
public:
  MyThread( int val );
protected:
  int val;

  void *Run();
};

MyThread::MyThread( int v ) : MThread()
{
  val=v;
}

void *MyThread::Run()
{
  while (!stopping)
    {
      printf( "Thread %d running.\n", val );
    };
  return( NULL );
}

int main()
{
  MyThread M( 1 );
  MyThread N( 2 );
  M.Start();
  N.Start();
  getchar();
  M.Stop();
  N.Stop();
  return( 0 );
}
