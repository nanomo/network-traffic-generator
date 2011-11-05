#include "../config.h"
#include "../os/os.h"
#include "loadpayload.h"

payload::payload( char *pathname )
{
  installed=0;
  libhandle=NULL;

  libhandle=import_dynamic_library( pathname );
  if (!libhandle)
    {
      return;
    }
  MyInitializePayload=(void * EXPORT (*)( unsigned min, unsigned max, unsigned increment, unsigned time, void *other ))import_dynamic_symbol( libhandle, "InitializePayload" );
  if (!MyInitializePayload) return;
  MyDestroyPayload=(void  EXPORT (*)( void *handle ))import_dynamic_symbol( libhandle, "DestroyPayload" );
  if (!MyDestroyPayload) return;
  MyGetPayload=(char * EXPORT (*)( void *handle, unsigned *size ))import_dynamic_symbol( libhandle, "GetPayload" );
  if (!MyGetPayload) return;
  MyReleasePayload=(void  EXPORT (*)( void *handle, char *payload ))import_dynamic_symbol( libhandle, "ReleasePayload" );
  if (!MyReleasePayload) return;
  MyResetPayloadSequence=(void  EXPORT (*)( void *handle ))import_dynamic_symbol( libhandle, "ResetPayloadSequence" );
  if (!MyResetPayloadSequence) return;
  installed=1;
}

payload::~payload()
{
  if (libhandle) close_dynamic_library( libhandle );
  libhandle=NULL;
}

void *payload::InitializePayload( unsigned min, unsigned max, unsigned increment, unsigned time, void *other )
{
  if (!isOk()) return( NULL );
  return( (*MyInitializePayload)( min, max, increment, time, other ) );
}

void payload::DestroyPayload( void *handle )
{
  if (!isOk()) return;
  (*MyDestroyPayload)( handle );
}

char *payload::GetPayload( void *handle, unsigned *size )
{
  if (!isOk()) return( NULL );
  return( (*MyGetPayload)( handle, size ) );
}

void payload::ReleasePayload( void *handle, char *payload )
{
  if (!isOk()) return;
  (*MyReleasePayload)( handle, payload );
}

void payload::ResetPayloadSequence( void *handle )
{
  if (!isOk()) return;
  (*MyResetPayloadSequence)( handle );
}
