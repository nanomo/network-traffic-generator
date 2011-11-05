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
#include "tcp_server.h"
#include "../../os/os.h"

tcp_server::tcp_server( unsigned port, unsigned num_connections, void *other )
{
  installed=0;
  sock=-1;
  struct protoent *pr;
  int result;

  pr=getprotobyname( "tcp" );
  if (!pr) return;
  addr.sin_family=AF_INET;
  addr.sin_port=htons( port );
  result=TranslateInetString( "0.0.0.0", &addr.sin_addr, sizeof(addr.sin_addr) );
  if (result<0) return;
  sock=socket( PF_INET, SOCK_STREAM,  pr->p_proto );
  if (sock<0) return;
  if (bind( sock, (const sockaddr *)&addr, sizeof( addr ))<0) return;
  if (listen( sock, num_connections )<0) return;
  installed=1;
}

tcp_server::~tcp_server()
{
  if (sock>=0)
    {
      shutdown( sock, SHUT_RDWR );
      close( sock );
      sock=-1;
    }
}

tcp_connection *tcp_server::accept( void *other )
{
  tcp_connection *tmp;

  if (sock<=0) return( NULL );
  tmp=new tcp_connection( sock, other );
  
  if (!tmp) return( NULL );
  if (tmp->isOk()) return( tmp );
  delete tmp;
  return( NULL );
}

int tcp_server::ready( int why )
{
  if (!isOk()) return( -1 );
#if defined(__WIN32__)
  fd_set setr, setw;
  int result;
  struct timeval delay;

  FD_ZERO( &setr );
  FD_ZERO( &setw );
  FD_SET( sock, &setr );
  FD_SET( sock, &setw );
  delay.tv_sec=0;
  delay.tv_usec=0;

  result=select( 1, &setr, &setw, NULL, &delay );
  switch (result)
    {
     case SOCKET_ERROR: // bad
          return( -1 );
     case 0: // timeout
          return( 0 );
     case 1: // ready
          return( 1 );
    };
#else
  struct pollfd pfd;
  int result;

  pfd.fd=sock;
  pfd.revents=0;
  pfd.events=POLLIN|POLLOUT;
  result=poll( &pfd, 1, 1 ); // 1ms wait
  switch (result)
    {
    case -1: // error
      return( -1 );
    case 0: // nothing to report
      return( 0 );
    case 1: // Something to report
      return( 1 );
    }
#endif
}

int tcp_server::operator==( tcp_server &other )
{
  if (other.sock==sock) return( 1 );
  return( 0 );
}
