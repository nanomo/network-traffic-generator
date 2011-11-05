#ifndef __UDP_CONNECTION_H__
#define __UDP_CONNECTION_H__

#include "../../config.h"
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

class udp_connection
{
public:
  udp_connection(char *dest_addr, unsigned dest_port, char *local_addr, unsigned local_port, void *other);
  udp_connection( int listen_sock, void *other );
  virtual ~udp_connection();
  int isOk() { return( installed ); }; 
  int write( char *data, int len );
  int read( char *data, int len, char *remote_addr, int remote_len, int flags );
  void transfered( LONGLONG *readr, LONGLONG *writew );
  int ready( int why );
  int operator==( udp_connection &other );
protected:
  LONGLONG readd;
  LONGLONG writed;
  int sock;
  struct sockaddr_in addr;
  int dont_close;
  struct sockaddr_in local_address;
private:
  int installed;
};

#endif
