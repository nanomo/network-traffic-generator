#include "../../config.h"
#include "tcp_proto.h"

tcp_proto::tcp_proto( int client_server )
{
  installed=0;
  dead_read=0;
  dead_write=0;
  installed=1;
}

tcp_proto::~tcp_proto()
{
  tcp_connection *ctmp;
  tcp_server *stmp;

  if (connections.size()!=0)
   {
  ctmp=connections.front();
  while (ctmp)
    {
      delete ctmp;
      connections.pop_front();
      ctmp=connections.front();
    }
   }
  if (servers.size()!=0)
   {
  stmp=servers.front();
  while (stmp)
    {
      delete stmp;
      servers.pop_front();
      if (servers.size()!=0)
        {
      stmp=servers.front();
    }
       else
        {
         stmp=NULL;
        }
     }
   }
}

tcp_connection *tcp_proto::create_connection( char *dest_addr, unsigned dest_port, char *local_addr, unsigned local_port, void *other )
{
  tcp_connection *tmp;
  tmp=new tcp_connection( dest_addr, dest_port, local_addr, local_port, other );
  if (!tmp) return( NULL );
  if (tmp->isOk())
    {
      connections.push_back( tmp );
      return( tmp );
    }
  else
    {
      delete tmp;
      return( NULL );
    }
}

void tcp_proto::destroy_connection( tcp_connection *conn, int waiting )
{
  int cnt;
  tcp_connection *tmp;
  std::deque<tcp_connection *>::iterator x;
  
  x=connections.begin();
  for (cnt=0;cnt<connections.size();cnt++)
    {
      if ((x[cnt])==(conn))
	{
	  connections.erase( x );
	  return;
	}
    }
}

void tcp_proto::destroy_server( tcp_server *serv, int waiting )
{
  int cnt;
  tcp_server *tmp;
  std::deque<tcp_server *>::iterator x;
  
  x=servers.begin();
  for (cnt=0;cnt<servers.size();cnt++)
    {
      if ((x[cnt])==(serv))
	{
	  servers.erase( x );
	  return;
	}
    }
}


int tcp_proto::write( tcp_connection *conn, char *data, int len )
{
  if (!conn) return( -1 );
  return( conn->write( data, len ) );
}

int tcp_proto::read( tcp_connection *conn, char *data, int len, char *remote_addr, int remote_len, int flags )
{
  if (!conn) return( -1 );
  return( conn->read( data, len, remote_addr, remote_len, flags ) );
}

tcp_server *tcp_proto::listen( unsigned port, unsigned num_connections, void *other )
{
  tcp_server *tmp;
  tmp=new tcp_server( port, num_connections, other );
  if (!tmp) return( NULL );
  if (tmp->isOk())
    {
      servers.push_back( tmp );
      return( tmp );
    }
  delete tmp;
  return( NULL );
}

tcp_connection *tcp_proto::accept( tcp_server *serv, void *other )
{
  if (!serv) return( NULL );
  return( serv->accept( other ) );
}

void tcp_proto::transfered( tcp_connection *conn, LONGLONG *read, LONGLONG *write )
{
  if (conn)
    {
     conn->transfered( read, write );
     return;
    }

  int cnt;
  LONGLONG tread, twrite, ttread, ttwrite;

  ttread=0;
  ttwrite=0;
  for (cnt=0;cnt<connections.size();cnt++)
    {
      (connections[cnt])->transfered( &tread, &twrite );
      ttread+=tread;
      ttwrite+=twrite;
    }
  *read=ttread;
  *write=ttwrite;
}

int tcp_proto::ready( void *handle, int why )
{
  tcp_server *tmps;
  tcp_connection *tmpc;
  
  if (!handle) return( -1 );

  switch (why)
    {
    case 1:
    case 2:
    case 3:
      tmpc=(tcp_connection *)handle;
      return( tmpc->ready( why ) );
    case 4:
      tmps=(tcp_server *)handle;
      return( tmps->ready( why ) );
    default:
      return( -1 );
    }
}
