#include "../../config.h"

MThread::MThread(): TThread( true )
{
  ReturnValue=-1;
  installed=0;
  installed=1;
}

_fastcall MThread::~MThread()
{
}

int MThread::Start()
{
  if (Suspended) Resume();
  return( 0 );
}

void *MThread::Stop()
{
  Terminate();
  return( NULL );
}

int MThread::getStatus()
{
 if (Terminated) return( MTHREAD_STOPPED );
 if (Suspended) return( MTHREAD_UNDEFINED );
 return( MTHREAD_RUNNING );
}

void __fastcall MThread::Terminate(void)
{
 stopping=1;
 TThread::Terminate();
}

void __fastcall MThread::Execute(void)
{
 Run();
}

