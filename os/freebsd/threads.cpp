#include "../../config.h"

void *MThread_helper( void *arg )
{
  MThread *me;

  me=(MThread *)arg;
  return( me->Run() );
}

MThread::MThread()
{
  stopping=0;
  global_result=-1;
  installed=0;
  status=MTHREAD_UNDEFINED;
  installed=1;
}

MThread::~MThread()
{
  Stop();
}

int MThread::Start()
{
  int result;
  pthread_attr_t attr;
  if (status!=MTHREAD_UNDEFINED) return( -1 );
  pthread_attr_init(&attr);
  pthread_attr_setstacksize(&attr, 1024*1024); // Set 1M stack size... should be enough?
  result=pthread_create( &thread, &attr, MThread_helper, this );
  if (!result) status=MTHREAD_RUNNING;
  pthread_attr_destroy(&attr);
  return( result );
}

void *MThread::Stop()
{
  if (!isOk()) return( NULL );
  if (status!=MTHREAD_RUNNING && status!=MTHREAD_STOPPING) return( NULL );
  status=MTHREAD_STOPPING;
  stopping+=1;
 
  int result;
  void *value;
  result=pthread_join( thread, &value );
  if (result) return( NULL );
  status=MTHREAD_STOPPED;
  return( value );  
}
