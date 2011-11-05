#ifndef __SERVER_CLIENT_H__
#define __SERVER_CLIENT_H__

#include "../../os/os.h"
#include "../../reply/loadreply.h"
#include "../../protocols/loadprotocol.h"

#define MAX_BASIC_SERVER_PACKET_SIZE 102400

class server_client: public MThread
{
 public:
  server_client( struct client_protocol *prot, struct server_reply *rep );
#if defined(__WIN32__)
  _fastcall virtual ~server_client();
#else
  virtual ~server_client();
#endif
  int isOk() { return( installed ); };
  void TransferedData( LONGLONG *readd, LONGLONG *writed );
 protected:
  struct server_reply *sreply;
  struct client_protocol *proto;
  void *reply_handle;

  virtual void *Run();
 private:
  int installed;
};
#endif
