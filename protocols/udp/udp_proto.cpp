#include "../../config.h"
#include "udp_proto.h"

udp_proto::udp_proto( int client_server )
{
  installed=0;
  dead_read=0;
  dead_write=0;
  installed=1;
}

udp_proto::~udp_proto()
{
  udp_connection *ctmp;
  udp_server *stmp;

  if (connections.size()!=0)
   {
  ctmp=connections.front();
  while (ctmp)
    {
      delete ctmp;
      connections.pop_front();
        ctmp=NULL;
        if (connections.size()!=0)
         {
      ctmp=connections.front();
    }
      }
   }
  if (servers.size()!=0)
   {
  stmp=servers.front();
  while (stmp)
    {
      delete stmp;
      servers.pop_front();
       stmp=NULL;
       if (servers.size())
        {
      stmp=servers.front();
    }
     }
   }
}

udp_connection *udp_proto::create_connection( char *dest_addr, unsigned dest_port, char *local_addr, unsigned local_port, void *other )
{
  udp_connection *tmp;
  tmp=new udp_connection( dest_addr, dest_port, local_addr, local_port, other );
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

void udp_proto::destroy_connection( udp_connection *conn, int waiting )
{
  int cnt;
  udp_connection *tmp;
  std::deque<udp_connection *>::iterator x;
  
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

void udp_proto::destroy_server( udp_server *serv, int waiting )
{
  int cnt;
  udp_server *tmp;
  std::deque<udp_server *>::iterator x;
  
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


int udp_proto::write( udp_connection *conn, char *data, int len )
{
  return( conn->write( data, len ) );
}

int udp_proto::read( udp_connection *conn, char *data, int len, char *remote_addr, int remote_len, int flags )
{
  return( conn->read( data, len, remote_addr, remote_len, flags ) );
}

udp_server *udp_proto::listen( unsigned port, unsigned num_connections, void *other )
{
  udp_server *tmp;
  tmp=new udp_server( port, num_connections, other );
  if (!tmp) return( NULL );
  if (tmp->isOk())
    {
      servers.push_back( tmp );
      return( tmp );
    }
  delete tmp;
  return( NULL );
}

udp_connection *udp_proto::accept( udp_server *serv, void *other )
{
  return( serv->accept( other ) );
}

void udp_proto::transfered( udp_connection *conn, LONGLONG *read, LONGLONG *write )
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

int udp_proto::ready( void *handle, int why )
{
  udp_server *tmps;
  udp_connection *tmpc;

  switch (why)
    {
    case 1:
    case 2:
    case 3:
      tmpc=(udp_connection *)handle;
      return( tmpc->ready( why ) );
    case 4:
      tmps=(udp_server *)handle;
      return( tmps->ready( why ) );
    default:
      return( -1 );
    }
}
