#ifndef __UDP_SERVER_H__
#define __UDP_SERVER_H__

#include "udp_connection.h"

class udp_server
{
public:
  udp_server( unsigned port, unsigned num_connections, void *other );
  virtual ~udp_server();
  int isOk() { return( installed ); };
  udp_connection *accept( void *other );
  int ready( int why );
  int operator==( udp_server &other );
protected:
  int sock;
  struct sockaddr_in addr;

  int server_ok;
private:
  int installed;
};

#endif
