#ifndef __THREADS_H__
#define __THREADS_H__

#if defined(LINUX) || defined(__LINUX__) || defined(__linux__) || defined( __FreeBSD__ )
#include <pthread.h>
#else
#if defined(__WIN32__)
#include <vcl.h>
#else
#error Operating system not supported for threads
#endif
#endif

#define MTHREAD_UNDEFINED -1
#define MTHREAD_RUNNING 0
#define MTHREAD_STOPPING 1
#define MTHREAD_STOPPED 2

#if defined(__WIN32__)
class MThread: public TThread
{
 public:
  MThread();
  _fastcall virtual ~MThread();
  int isOk() { return( installed ); };
  int Start();
  void *Stop();
  int getResult() { return( ReturnValue ); };
  int getStatus();
  void __fastcall Terminate(void);
 protected:
  void __fastcall Execute(void);
  virtual void *Run()=0;

  int stopping;
 private:
  int installed;
};
#else
class MThread
{
 public:
  MThread();
  virtual ~MThread();
  int isOk() { return( installed ); };
  int Start();
  void *Stop();
  int getResult() { return( global_result ); };  
  int getStatus() { return( status ); };
 protected:
  int stopping;
  int global_result;
  int status;
#if defined(LINUX) || defined(__LINUX__) || defined(__linux__) || defined( __FreeBSD__ )
  pthread_t thread;

  friend void *MThread_helper( void *arg );
#endif
  virtual void *Run()=0;
 private:
  int installed;
};
#endif

#endif
