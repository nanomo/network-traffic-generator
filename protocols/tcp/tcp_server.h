#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include "tcp_connection.h"

class tcp_server
{
public:
  tcp_server( unsigned port, unsigned num_connections, void *other );
  virtual ~tcp_server();
  int isOk() { return( installed ); };
  tcp_connection *accept( void *other );
  int ready( int why );
  int operator==( tcp_server &other );
protected:
  int sock;
  struct sockaddr_in addr;
private:
  int installed;
};

#endif
