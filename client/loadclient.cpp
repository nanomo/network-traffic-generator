#include "../config.h"
#include "../os/os.h"
#include "loadclient.h"

client::client( char *pathname )
{
  installed=0;
  libhandle=NULL;
  libhandle=import_dynamic_library( pathname );
  if (!libhandle)
    {
      return;
    }
  MyInitClient=(void * EXPORT (*)( struct client_protocol *proto, int num_proto, struct client_payload *pld, int num_payload, int min, int max, int incr, int time ))import_dynamic_symbol( libhandle, "InitClient" );
  if (!MyInitClient) return;
  MyDestroyClient=(void  EXPORT (*)( void *handle ))import_dynamic_symbol( libhandle, "DestroyClient" );
  if (!MyDestroyClient) return;
  MyStartClient=(int  EXPORT (*)( void *handle ))import_dynamic_symbol( libhandle, "StartClient" );
  if (!MyStartClient) return;
  MyStopClient=(int  EXPORT (*)( void *handle ))import_dynamic_symbol( libhandle, "StopClient" );
  if (!MyStopClient) return;
  MyGetClientDataTransfered=(void  EXPORT (*)( void *handle, LONGLONG *readd, LONGLONG *writed ))import_dynamic_symbol( libhandle, "GetClientDataTransfered" );
  if (!MyGetClientDataTransfered) return;
  MyResetClientSequence=(void  EXPORT (*)( void *handle ))import_dynamic_symbol( libhandle, "ResetClientSequence" );
  if (!MyResetClientSequence) return;
  installed=1;
}

client::~client()
{
  close_dynamic_library( libhandle );
}

void *client::InitClient( struct client_protocol *proto, int num_proto, struct client_payload *pld, int num_payload, int min, int max, int incr, int time )
{
  if (!isOk()) return( NULL );
  return( (*MyInitClient)( proto, num_proto, pld, num_payload, min, max, incr, time ) );
}

void client::DestroyClient( void *handle )
{
  if (!isOk()) return;
  (*MyDestroyClient)( handle );
}

int client::StartClient( void *handle )
{
  if (!isOk()) return( -1 );
  return( (*MyStartClient)( handle ) );
}

int client::StopClient( void *handle )
{
  if (!isOk()) return( -1 );
  return( (*MyStopClient)( handle ) );
}

void client::GetClientDataTransfered( void *handle, LONGLONG *readd, LONGLONG *writed )
{
  if (!isOk()) return;
  (*MyGetClientDataTransfered)( handle, readd, writed );
}

void client::ResetClientSequence( void *handle )
{
  if (!isOk()) return;
  (*MyResetClientSequence)( handle );
}
