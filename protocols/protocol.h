#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

/*
   This header file is the basic interface of all protocols that is supported.
 */

/*
  This function checks whether this protocol can be supported on this machine
 */
extern "C" int canDoProtocol();

/*
  This function initializes the protocol and specifies whether it is the client or server.
  the client_server parameter is 0 for client and 1 for server.

  It returns a handle to be passed to all subsequent calls.
 */
extern "C" void *InitProtocol( int client_server );

/*
  This function destroys the internal structures generated for the protocol. The handle is passed
  and is thereafter invalid. The waiting is just whether it should exit kindly or not. If it is 0
  it should exit nicely.
 */
extern "C" void DestroyProtocol( void *handle, int waiting );

/*
  This function creates a connection using the protocol tp the desired destination address and port.
  The other pointer can contain data specific to the protocol, but this should be avoided.  

  local_addr and local_port is the local port and address to be used.

  This is only valid for a client instance.

  It returns a handle to the connection.
 */
extern "C" void *CreateConnection( void *handle, char *dest_addr, unsigned dest_port, char *local_addr, unsigned local_port, void *other );

/*
  This destroys a handle to a connection. It is invalid after that. The protocol handle is still valid.

  The waiting variable is non zero if it should destroy it immediately.
 */
extern "C" void DestroyConnection( void *handle, void *connection, int waiting );

/*
  This sends data on an established connection. For protocols that is not connection based,
  it should remember information from the CreateConnection call. It returns the number of bytes sent 
  or -1 on error.
 */
extern "C" int SendConnectionData( void *handle, void *connection, char *data, int len );

/*
  This receives data on an established connection. It will store the remote address of the machine sending the data
  into remote_addr upto remote_len of it. It will receive a maximum of "len" data into the "data" pointer. It will wait
  until data is received

  flags should be 1 to just peek data and not read it, normally it should be 0.
 */
extern "C" int ReceiveConnectionData( void *handle, void *connection, char *data, int len, char *remote_addr, int remote_len, int flags );

/*
  This is only valid in server mode. And will put the server into listening mode for new connections.

  port is the port to listen on, num_connections is the number of pending connections to be held in the queue and other 
  is for protocol specific data. Please try not to use it.
 */
extern "C" void *ListenForConnections( void *handle, unsigned port, unsigned num_connections, void *other );

/*
  This is only valid in server mode. This will remove the listening mode, freeing the port.

  server is the pointer returned by ListenForConnections, and waiting is non-zero if it should not wait for all data.
 */
extern "C" void StopListenForConnections( void *handle, void *server, int waiting );
/*
  This is only valid in server mode. It will accept a waiting connection and create a handle that can be used to send and 
  receive data. The other pointer is for protocol specific data and should preferably not be used.
 */
extern "C" void *AcceptServerConnection( void *handle, void *listen_handle, void *other );

/*
  This will return the number of bytes read and writen on the desired connection. If the connection handle is NULL it will
  return the same numbers for the protocol.
 */
extern "C" void TransferedData( void *handle, void *connect_handle, LONGLONG *read, LONGLONG *write );

/*
  This is to check whether a specific handle has some action or data associated with it.
  The other_handle can be either one created by "CreateConnection" or by "ListenForConnections".

  The "why" can be: 
    1: Ready to read
    2: Ready to write
    4: Connection waiting
 */
extern "C" int ReadyForData( void *handle, void *other_handle, int why );

#endif
