#include "../config.h"
#include "../os/os.h"
#include "loadserver.h"

server::server( char *path )
{
  installed=0;
  lib_handle=NULL;
  lib_handle=import_dynamic_library( path );
  if (!lib_handle)
    {
      return;
    }
  MyInitServer=(void * EXPORT (*)( struct client_protocol **proto, int num_protocol, struct server_reply **rep, int num_reply ))import_dynamic_symbol( lib_handle, "InitServer" );
  if (!MyInitServer) return;
  MyDestroyServer=(void  EXPORT (*)( void *handle ))import_dynamic_symbol( lib_handle, "DestroyServer" );
  if (!MyDestroyServer) return;
  MyStartServer=(int  EXPORT (*)( void *handle ))import_dynamic_symbol( lib_handle, "StartServer" );
  if (!MyStartServer) return;
  MyStopServer=(int  EXPORT (*)( void *handle ))import_dynamic_symbol( lib_handle, "StopServer" );
  if (!MyStopServer) return;
  MyServerTransferedData=(void  EXPORT (*)( void *handle, LONGLONG *readd, LONGLONG *writed ))import_dynamic_symbol( lib_handle, "ServerTransferedData" );
  if (!MyServerTransferedData) return;
  installed=1;
}

server::~server()
{
  if (lib_handle) close_dynamic_library( lib_handle );
  lib_handle=NULL;
}

void *server::InitServer( struct client_protocol **proto, int num_protocol, struct server_reply **rep, int num_reply )
{
  if (!isOk()) return( NULL );
  return( (*MyInitServer)( proto, num_protocol, rep, num_reply ) );
}

void server::DestroyServer( void *handle )
{
  if (!isOk()) return;
  (*MyDestroyServer)( handle );
}

int server::StartServer( void *handle )
{
  if (!isOk()) return( -1 );
  return( (*MyStartServer)( handle ) );
}

int server::StopServer( void *handle )
{
  if (!isOk()) return( -1 );
  return( (*MyStopServer)( handle ) );
}

void server::ServerTransferedData( void *handle, LONGLONG *readd, LONGLONG *writed )
{
  if (!isOk()) return;
  (*MyServerTransferedData)( handle, readd, writed );
}

