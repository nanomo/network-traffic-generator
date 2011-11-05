#include "fixed_client.h"

/*
  This initializes a client with a preconfigured protocol and payload. The parameters passed is also
  the minimum number of connections, the maximum number of connections, the increment and the time factor.

  Multiple payloads and protocols can be passed to the client. Do not try to duplicate them. Use them as fairly as possible, trying not
  to re-use the same protocols and payloads. If however it must happen, so be it.
 */
extern "C" void * EXPORT InitClient( struct client_protocol *proto, int num_proto, struct client_payload *pld, int num_payload, int min, int max, int incr, int time )
{
  fixed_client *tmp;
  tmp=new fixed_client( proto, num_proto, pld, num_payload, max );
  if (!tmp) return( NULL );
  if (tmp->isOk()) return( tmp );
  delete tmp;
  return( NULL );
}

/*
  This destroy's the client. After this the protocol and payload objects can be free'd.
 */
extern "C" void  EXPORT DestroyClient( void *handle )
{
  fixed_client *tmp;

  if (!handle) return;
  tmp=(fixed_client *)handle;
  delete tmp;
}

/*
  The client starts sending data. The client must do this asynchronous.
 */
extern "C" int  EXPORT StartClient( void *handle )
{
  fixed_client *tmp;

  if (!handle) return( -1 );
  tmp=(fixed_client *)handle;

  return( tmp->Start() ); 
}

/*
  The client stops sending data
 */
extern "C" int  EXPORT StopClient( void *handle )
{
  fixed_client *tmp;

  if (!handle) return( -1 );
  tmp=(fixed_client *)handle;

  return( tmp->Stop() );
}

/*
  This queries the protocols for the amount of data transfered
 */
extern "C" void  EXPORT GetClientDataTransfered( void *handle, LONGLONG *readd, LONGLONG *writed )
{
  fixed_client *tmp;

  if (!handle) return;
  tmp=(fixed_client *)handle;

  tmp->GetClientDataTransfered( readd, writed );
}

/*
  This resets any sequence info for the client
 */
extern "C" void  EXPORT ResetClientSequence( void *handle )
{ // Ok maybe we have to reset all the payload sequences...
  fixed_client *tmp;

  if (!handle) return;
  tmp=(fixed_client *)handle;
  
  tmp->ResetClientSequence();
}
