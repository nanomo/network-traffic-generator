#include "../../config.h"
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_SYS_POLL_H
#include <sys/poll.h>
#endif
#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif
#include "udp_connection.h"
#include "../loadprotocol.h"
#include "../../os/os.h"

udp_connection::udp_connection(char *dest_addr, unsigned dest_port, char *local_addr, unsigned local_port, void *other)
{
  installed=0;
  sock=-1;
  dont_close=0;
  readd=0;
  writed=0;

  memset( &local_address, 0, sizeof(local_address) );

  struct protoent *pr;
  int result;

  pr=getprotobyname( "udp" );
  if (!pr) return;
  addr.sin_family=AF_INET;
  addr.sin_port=htons( dest_port );
  result=TranslateInetString( dest_addr, &addr.sin_addr, sizeof(addr.sin_addr) );
  if (result<0) return;
  sock=socket( PF_INET, SOCK_DGRAM,  pr->p_proto );
  if (sock<0) return;
  if (local_addr || local_port)
    { // we have to bind!
      local_address.sin_family=AF_INET;
      local_address.sin_port=htons( local_port );
      if (local_addr)
        {
         result=TranslateInetString( local_addr, &local_address.sin_addr, sizeof(local_address.sin_addr) );
         if (result<0) return;
        }
      if (bind( sock, (struct sockaddr *)&local_address, sizeof(local_address) )<0) return;
    }
#if !defined(__WIN32__)
  if (connect( sock, (const sockaddr *)&addr, sizeof(addr) )<0) return;
#endif
  installed=1;
}

udp_connection::udp_connection( int listen_sock, void *other )
{
  installed=0;
  sock=-1;
  dont_close=1;
  readd=0;
  writed=0;

  sock=listen_sock;
  if (sock<0) return;
  installed=1;
}

udp_connection::~udp_connection()
{
  if (sock>=0 && !dont_close)
    {
      shutdown( sock, SHUT_RDWR );
      close( sock );
      sock=-1;
    }
}

int udp_connection::write( char *data, int len )
{
  if (!isOk()) return( -1 );
  if (len<=0 || !data) return( 0 );

  int result;
  result=::sendto( sock, data, len, 0, (const struct sockaddr *)&addr, sizeof(addr) );
  if (result>0) writed+=result;
  return( result );
}

int udp_connection::read( char *data, int len, char *remote_addr, int remote_len, int flags )
{
  if (!isOk()) return( -1 );
  if (len<=0 || !data) return( 0 );

  int result, rflag;
  socklen_t slen;

  rflag=PROTOCOL_NONE;
  if (flags&PROTOCOL_PEEK) rflag|=MSG_PEEK;
#if defined(__WIN32__)
  data[0]=0;
#endif
  slen=sizeof( addr );
  result=::recvfrom( sock, data, len, rflag, (struct sockaddr *)&addr, &slen );
#if defined(__WIN32__)
  if (flags&PROTOCOL_PEEK)
    { // Windows can not do peek properly
      if (data[0]!=0)
        result=len;
    }
#endif
  if (result>0) readd+=result;
  if (remote_addr && remote_len>0)
    {
#if defined(__WIN32__)
      strncpy( remote_addr, inet_ntoa( addr.sin_addr ), remote_len );
#else
      inet_ntop( AF_INET, &(addr.sin_addr), remote_addr, remote_len );
#endif
    }
  return( result );
}

void udp_connection::transfered( LONGLONG *readr, LONGLONG *writew )
{
  if (readr) *readr=readd;
  if (writew) *writew=writed;
}

int udp_connection::ready( int why )
{
  if (!isOk()) return( -1 );
#if defined(__WIN32__)
  fd_set setr, setw;
  int result;
  struct timeval delay;

  FD_ZERO( &setr );
  FD_ZERO( &setw );
  switch (why)
    {
    case 1:
      FD_SET( sock, &setr );
      break;
    case 2:
      FD_SET( sock, &setw );
      break;
    default:
      FD_SET( sock, &setr );
      FD_SET( sock, &setw );
      break;
    };
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
  switch (why)
    {
    case 1:
      pfd.events=POLLIN;
      break;
    case 2:
      pfd.events=POLLOUT;
      break;
    default:
      pfd.events=POLLIN|POLLOUT;
    };
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

int udp_connection::operator==( udp_connection &other )
{
  if (sock==other.sock) return( 1 );
  return( 0 );
}

