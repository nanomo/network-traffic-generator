#ifndef __SERVER_LISTENER_H__
#define __SERVER_LISTENER_H__

#include "../../os/os.h"
#include "../../protocols/loadprotocol.h"
#include "../../reply/loadreply.h"
#include "server_client.h"
#include <deque>

class server_listener: public MThread
{
 public:
  server_listener( struct client_protocol *prot, struct server_reply *rep );
#if defined(__WIN32__)
  _fastcall virtual ~server_listener();
#else
  virtual ~server_listener();
#endif
  int isOk() { return( installed ); };
  void TransferedData( LONGLONG *readd, LONGLONG *writed );
 protected:
  struct client_protocol *proto; 
  struct server_reply *sreply;
  std::deque<server_client *> clients;

  virtual void *Run();
 private:
  int installed;
};

#endif
