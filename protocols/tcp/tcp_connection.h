#ifndef __TCP_CONNECTION_H__
#define __TCP_CONNECTION_H__
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#if defined(__WIN32__)
#include <winsock.h>
#endif

class tcp_connection
{
public:
  tcp_connection(char *dest_addr, unsigned dest_port, char *local_addr, unsigned local_port, void *other);
  tcp_connection( int listen_sock, void *other );
  virtual ~tcp_connection();
  int isOk() { return( installed ); }; 
  int write( char *data, int len );
  int read( char *data, int len, char *remote_addr, int remote_len, int flags );
  void transfered( LONGLONG *readr, LONGLONG *writew );
  int ready( int why );
  int operator==( tcp_connection &other );
protected:
  LONGLONG readd;
  LONGLONG writed;
  int sock;
  struct sockaddr_in addr;
  struct sockaddr_in local_address;
private:
  int installed;
};

#endif
