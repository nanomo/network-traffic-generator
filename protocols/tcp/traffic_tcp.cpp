#include "../../config.h"
#include <deque>
#include "tcp_connection.h"
#include "tcp_server.h"
#include "tcp_proto.h"

/*
  This function checks whether this protocol can be supported on this machine
 */
extern "C" int EXPORT canDoProtocol()
{ // TCP should always be supported
  return( 1 );
}

/*
  This function initializes the protocol and specifies whether it is the client or server.
  the client_server parameter is 0 for client and 1 for server.

  It returns a handle to be passed to all subsequent calls.
 */
extern "C" void * EXPORT InitProtocol( int client_server )
{
  tcp_proto *tmp;
  tmp=new tcp_proto( client_server );
  if (tmp && tmp->isOk()) return( tmp );
  if (tmp) delete tmp;
  return( NULL );
}
/*
  This function destroys the internal structures generated for the protocol. The handle is passed
  and is thereafter invalid. The waiting is just whether it should exit kindly or not. If it is 0
  it should exit nicely.
 */
extern "C" void  EXPORT DestroyProtocol( void *handle, int waiting )
{
  tcp_proto *tmp;
  tmp=(tcp_proto *)handle;
  if (!tmp) return;
  delete tmp;
}

/*
  This function creates a connection using the protocol tp the desired destination address and port.
  The other pointer can contain data specific to the protocol, but this should be avoided.  

  This is only valid for a client instance.

  It returns a handle to the connection.
 */
extern "C" void * EXPORT CreateConnection( void *handle, char *dest_addr, unsigned dest_port, char *local_addr, unsigned local_port, void *other )
{
  tcp_proto *tmp;

  tmp=(tcp_proto *)handle;
  if (!tmp) return( NULL );
  return( tmp->create_connection( dest_addr, dest_port, local_addr, local_port, other ) );
}

/*
  This destroys a handle to a connection. It is invalid after that. The protocol handle is still valid.

  The waiting variable is non zero if it should destroy it immediately.
 */
extern "C" void  EXPORT DestroyConnection( void *handle, void *connection, int waiting )
{
  tcp_proto *tmp;

  tmp=(tcp_proto *)handle;
  if (!tmp) return;
  tmp->destroy_connection( (tcp_connection *)connection, waiting );
}
/*
  This sends data on an established connection. For protocols that is not connection based,
  it should remember information from the CreateConnection call. It returns the number of bytes sent 
  or -1 on error.
 */
extern "C" int  EXPORT SendConnectionData( void *handle, void *connection, char *data, int len )
{
  tcp_proto *tmp;

  tmp=(tcp_proto *)handle;
  if (!tmp) return( -1 );
  return( tmp->write( (tcp_connection *)connection, data, len ) );
}

/*
  This receives data on an established connection. It will store the remote address of the machine sending the data
  into remote_addr upto remote_len of it. It will receive a maximum of "len" data into the "data" pointer. It will wait
  until data is received
 */
extern "C" int  EXPORT ReceiveConnectionData( void *handle, void *connection, char *data, int len, char *remote_addr, int remote_len, int flags )
{
  tcp_proto *tmp;

  tmp=(tcp_proto *)handle;
  if (!tmp) return( -1 );
  return( tmp->read( (tcp_connection *)connection, data, len, remote_addr, remote_len, flags ) );
}

/*
  This is only valid in server mode. And will put the server into listening mode for new connections.

  port is the port to listen on, num_connections is the number of pending connections to be held in the queue and other 
  is for protocol specific data. Please try not to use it.
 */
extern "C" void * EXPORT ListenForConnections( void *handle, unsigned port, unsigned num_connections, void *other )
{
  tcp_proto *tmp;

  tmp=(tcp_proto *)handle;
  if (!tmp) return( NULL );
  return( tmp->listen( port, num_connections, other ) );
}

/*
  This is only valid in server mode. This will remove the listening mode, freeing the port.

  server is the pointer returned by ListenForConnections, and waiting is non-zero if it should not wait for all data.
 */
extern "C" void  EXPORT StopListenForConnections( void *handle, void *server, int waiting )
{
  tcp_proto *tmp;

  tmp=(tcp_proto *)handle;
  if (!tmp) return;
  tmp->destroy_server( (tcp_server *)server, waiting );
}

/*
  This is only valid in server mode. It will accept a waiting connection and create a handle that can be used to send and 
  receive data. The other pointer is for protocol specific data and should preferably not be used.
 */
extern "C" void * EXPORT AcceptServerConnection( void *handle, void *listen_handle, void *other )
{
  tcp_proto *tmp;

  tmp=(tcp_proto *)handle;
  if (!tmp) return( NULL );
  return( tmp->accept( (tcp_server *)listen_handle, other ) );
}
/*
  This will return the number of bytes read and writen on the desired connection. If the connection handle is NULL it will
  return the same numbers for the protocol.
 */
extern "C" void  EXPORT TransferedData( void *handle, void *connect_handle, LONGLONG *read, LONGLONG *write )
{
  tcp_proto *tmp;

  tmp=(tcp_proto *)handle;
  if (!tmp) return;
  tmp->transfered( (tcp_connection *)connect_handle, read, write );
}
/*
  This is to check whether a specific handle has some action or data associated with it.
  The other_handle can be either one created by "CreateConnection" or by "ListenForConnections".

  The "why" can be: 
    1: Ready to read
    2: Ready to write
    4: Connection waiting
 */
extern "C" int  EXPORT ReadyForData( void *handle, void *other_handle, int why )
{
  tcp_proto *tmp;

  tmp=(tcp_proto *)handle;
  if (!tmp) return( -1 );
  return( tmp->ready( other_handle, why ) );
}
