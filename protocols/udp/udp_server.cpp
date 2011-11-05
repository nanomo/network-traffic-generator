#include "../../config.h"
#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_SYS_POLL_H
#include <sys/poll.h>
#endif
#include "udp_server.h"
#include "../../os/os.h"

udp_server::udp_server( unsigned port, unsigned num_connections, void *other )
{
  installed=0;
  sock=-1;
  server_ok=0;

  struct protoent *pr;
  int result;

  addr.sin_family=AF_INET;
  addr.sin_port=htons( port );
  result=TranslateInetString( "0.0.0.0", &addr.sin_addr, sizeof(addr.sin_addr) );
  if (result<0) return;
  sock=socket( PF_INET, SOCK_DGRAM,  17 );
  if (sock<0) return;
  if (bind( sock, (const sockaddr *)&addr, sizeof( addr ))<0) return;
  //  if (listen( sock, num_connections )<0) return;
  installed=1;
}

udp_server::~udp_server()
{
  if (sock>=0)
    {
      shutdown( sock, SHUT_RDWR );
      close( sock );
      sock=-1;
    }
}

udp_connection *udp_server::accept( void *other )
{
  udp_connection *tmp;

  //  if (server_ok) return( NULL );
  tmp=new udp_connection( sock, other );
  if (!tmp) return( NULL );
  if (tmp->isOk()) 
    {
      server_ok=1;
      return( tmp );
    }
  delete tmp;
  return( NULL );
}

int udp_server::ready( int why )
{
  if (!isOk()) return( -1 );
  if (server_ok) return( 0 );
  return( 1 );
}

int udp_server::operator==( udp_server &other )
{
  if (other.sock==sock) return( 1 );
  return( 0 );
}
