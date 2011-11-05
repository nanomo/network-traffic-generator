#ifndef __FIXED_WORKERS_H__
#define __FIXED_WORKERS_H__

#include "../../protocols/loadprotocol.h"
#include "../../payload/loadpayload.h"
#include "../../os/os.h"

class fixed_workers: public MThread
{
 public:
  fixed_workers( struct client_protocol *prot, struct client_payload *pay );
#if defined(__WIN32__)
  _fastcall virtual ~fixed_workers();
#else
  virtual ~fixed_workers();
#endif
  int isOk() { return( installed ); };
 protected:
  struct client_protocol *myprot;
  struct client_payload *mypay;

  void *Run();
 private:
  int installed;
};

#endif
