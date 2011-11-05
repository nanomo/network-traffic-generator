#include "../config.h"
#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#include "../os/os.h"
#include "loadprotocol.h"

protocol::protocol( char *path )
{
  installed=0;
  libhandle=NULL;
  
  libhandle=import_dynamic_library( path );
  if (!libhandle) 
    {
      return;
    }
  int (*canDoProtocol)();

  canDoProtocol=(int (*)())import_dynamic_symbol( libhandle, "canDoProtocol" );
  if (!canDoProtocol) return;
  if ((*canDoProtocol)()==0) return; // Check that protocol is supported
  ThisInitProtocol=(void * EXPORT (*)( int client_server ))import_dynamic_symbol( libhandle, "InitProtocol" );
  if (!ThisInitProtocol) return;
  ThisDestroyProtocol=(void  EXPORT (*)( void *handle, int waiting ))import_dynamic_symbol( libhandle, "DestroyProtocol" );
  if (!ThisDestroyProtocol) return;
  ThisCreateConnection=(void * EXPORT (*)( void *handle, char *dest_addr, unsigned dest_port, char *local_addr, unsigned local_port, void *other ))import_dynamic_symbol( libhandle, "CreateConnection" );
  if (!ThisCreateConnection) return;
  ThisDestroyConnection=(void  EXPORT (*)( void *handle, void *connection, int waiting ))import_dynamic_symbol( libhandle, "DestroyConnection" );
  if (!ThisDestroyConnection) return;
  ThisSendConnectionData=(int  EXPORT (*)( void *handle, void *connection, char *data, int len ))import_dynamic_symbol( libhandle, "SendConnectionData" );
  if (!ThisSendConnectionData) return;
  ThisReceiveConnectionData=(int  EXPORT (*)( void *handle, void *connection, char *data, int len, char *remote_addr, int remote_len, int flags ))import_dynamic_symbol( libhandle, "ReceiveConnectionData" );
  if (!ThisReceiveConnectionData) return;
  ThisListenForConnections=(void * EXPORT (*)( void *handle, unsigned port, unsigned num_connections, void *other ))import_dynamic_symbol( libhandle, "ListenForConnections" );
  if (!ThisListenForConnections) return;
  ThisAcceptServerConnection=(void * EXPORT (*)( void *handle, void *listen_handle, void *other ))import_dynamic_symbol( libhandle, "AcceptServerConnection" );
  if (!ThisAcceptServerConnection) return;
  ThisReadyForData=(int  EXPORT (*)( void *handle, void *other_handle, int why ))import_dynamic_symbol( libhandle, "ReadyForData" );
  if (!ThisReadyForData) return;
  ThisTransferedData=(void  EXPORT (*)( void *handle, void *connect_handle, LONGLONG *read, LONGLONG *write ))import_dynamic_symbol( libhandle, "TransferedData" );
  if (!ThisTransferedData) return; 
  installed=1;
}

protocol::~protocol()
{
  if (libhandle) close_dynamic_library( libhandle );
  libhandle=NULL;
}

void *protocol::InitProtocol( int client_server )
{
  if (!isOk()) return( NULL );
  return( (*ThisInitProtocol)( client_server ) );
}

void protocol::DestroyProtocol( void *handle, int waiting )
{
  if (!isOk()) return;
  (*ThisDestroyProtocol)( handle, waiting );
}

void *protocol::CreateConnection( void *handle, char *dest_addr, unsigned dest_port, char *local_addr, unsigned local_port, void *other )
{
  if (!isOk()) return( NULL );
  return( (*ThisCreateConnection)( handle, dest_addr, dest_port, local_addr, local_port, other ) );
}

void protocol::DestroyConnection( void *handle, void *connection, int waiting )
{
  if (!isOk()) return;
  (*ThisDestroyConnection)( handle, connection, waiting );
}

int protocol::SendConnectionData( void *handle, void *connection, char *data, int len )
{
  if (!isOk()) return( -1 );
  return( (*ThisSendConnectionData)( handle, connection, data, len ) );
}

int protocol::GetCurrentReadPacketSize( void *handle, void *connection )
{
  char tmp[8];
  if (!isOk()) return( -1 );
  int result;
  unsigned size;
  unsigned total;
  result=ReceiveConnectionData( handle, connection, tmp, 8, NULL, 0, PROTOCOL_PEEK );
  if (result<0) return( result );
  if (memcmp( tmp, "TRAF", 4 )) return( -1 );
  size=*((UINT32 *)&(tmp[4]));
  return( ntohl( size ) );
} 

int protocol::ReadPacketData( void *handle, void *connection, char *data, int len, char *remote_addr, int remote_len )
{
  unsigned size;
  int result;

  if (!data || len<8) return( -1 );
  size=GetCurrentReadPacketSize( handle, connection );
  if (size<0) return( -1 );
  if (size>len) return( -1 );
  unsigned total;
  total=0;
  while (total<size)
    {
      result=ReceiveConnectionData( handle, connection, &data[total], len-total, remote_addr, remote_len, PROTOCOL_NONE );
      if (result<0) return( result );
      total+=result;
    }
  return( total );
}

int protocol::ReceiveConnectionData( void *handle, void *connection, char *data, int len, char *remote_addr, int remote_len, int flags )
{
  if (!isOk()) return( -1 );
  return( (*ThisReceiveConnectionData)( handle, connection, data, len, remote_addr, remote_len, flags ) );
}

void *protocol::ListenForConnections( void *handle, unsigned port, unsigned num_connections, void *other )
{
  if (!isOk()) return( NULL );
  return( (*ThisListenForConnections)( handle, port, num_connections, other ) );
}

void *protocol::AcceptServerConnection( void *handle, void *listen_handle, void *other )
{
  if (!isOk()) return( NULL );
  return( (*ThisAcceptServerConnection)( handle, listen_handle, other ) );
}

int protocol::ReadyForData( void *handle, void *other_handle, int why )
{

  return( (*ThisReadyForData)( handle, other_handle, why ) );
}

void protocol::TransferedData( void *handle, void *connect_handle, LONGLONG *read, LONGLONG *write )
{
  if (!isOk()) return;
  (*ThisTransferedData)( handle, connect_handle, read, write );
}
