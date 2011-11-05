#ifndef __LOAD_PROTOCOL_H__
#define __LOAD_PROTOCOL_H__

#include "../config.h"

/* No flags */
#define PROTOCOL_NONE 0
/* Peek data on read */
#define PROTOCOL_PEEK 1

class protocol
{
 public:
  protocol( char *path );
  virtual ~protocol();
  int isOk() { return( installed ); };
  void *InitProtocol( int client_server );
  void DestroyProtocol( void *handle, int waiting );
  void *CreateConnection( void *handle, char *dest_addr, unsigned dest_port, char *local_addr, unsigned local_port, void *other );
  void DestroyConnection( void *handle, void *connection, int waiting );
  int SendConnectionData( void *handle, void *connection, char *data, int len );
  int ReceiveConnectionData( void *handle, void *connection, char *data, int len, char *remote_addr, int remote_len, int flags );
  int GetCurrentReadPacketSize( void *handle, void *connection );
  int ReadPacketData( void *handle, void *connection, char *data, int len, char *remote_addr, int remote_len );
  void *ListenForConnections( void *handle, unsigned port, unsigned num_connections, void *other );
  void *AcceptServerConnection( void *handle, void *listen_handle, void *other );
  int ReadyForData( void *handle, void *other_handle, int why );
  void TransferedData( void *handle, void *connect_handle, LONGLONG *read, LONGLONG *write );
 protected:
  void *libhandle;
  void * EXPORT (*ThisInitProtocol)( int client_server );
  void  EXPORT (*ThisDestroyProtocol)( void *handle, int waiting );
  void *  EXPORT (*ThisCreateConnection)( void *handle, char *dest_addr, unsigned dest_port, char *local_addr, unsigned local_port, void *other );
  void  EXPORT (*ThisDestroyConnection)( void *handle, void *connection, int waiting );
  int  EXPORT (*ThisSendConnectionData)( void *handle, void *connection, char *data, int len );
  int  EXPORT (*ThisReceiveConnectionData)( void *handle, void *connection, char *data, int len, char *remote_addr, int remote_len, int flags );
  void * EXPORT (*ThisListenForConnections)( void *handle, unsigned port, unsigned num_connections, void *other );
  void * EXPORT (*ThisAcceptServerConnection)( void *handle, void *listen_handle, void *other );
  int  EXPORT (*ThisReadyForData)( void *handle, void *other_handle, int why );
  void  EXPORT (*ThisTransferedData)( void *handle, void *connect_handle, LONGLONG *read, LONGLONG *write );
 private:
  int installed;
};

/*
 structure to contain an initialized protocol with it's handle
 */
struct client_protocol
{
  protocol *proto;
  void *proto_handle;
  void *connection;
};


#endif
