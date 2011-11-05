#include "../config.h"
#include "../os/os.h"
#include "loadreply.h"

reply::reply( char *pathname )
{
  installed=0;
  libhandle=NULL;

  libhandle=import_dynamic_library( pathname );
  if (!libhandle)
    {
      return;
    }
  MyInitReply=(void * EXPORT (*)())import_dynamic_symbol( libhandle, "InitReply" );
  if (!MyInitReply) return;
  MyDestroyReply=(void  EXPORT (*)( void *handle ))import_dynamic_symbol( libhandle, "DestroyReply" );
  if (!MyDestroyReply) return;
  MyGetReply=(char * EXPORT (*)( void *handle, char *packet, unsigned size, unsigned *reply_size ))import_dynamic_symbol( libhandle, "GetReply" );
  if (!MyGetReply) return;
  MyDeleteReply=(void  EXPORT (*)( void *handle, char *reply, unsigned reply_size ))import_dynamic_symbol( libhandle, "DeleteReply" );
  if (!MyDeleteReply) return;
  installed=1;
}

reply::~reply()
{
  if (libhandle) close_dynamic_library( libhandle );
  libhandle=NULL;
}

void *reply::InitReply()
{
  if (!isOk()) return( NULL );
  return( (*MyInitReply)() );
}

void reply::DestroyReply( void *handle )
{
  if (!isOk()) return;
  (*MyDestroyReply)( handle );
}

char *reply::GetReply( void *handle, char *packet, unsigned size, unsigned *reply_size )
{
  if (!isOk()) return( NULL );
  return( (*MyGetReply)( handle, packet, size, reply_size ) );
}

void reply::DeleteReply( void *handle, char *reply, unsigned reply_size )
{
  if (!isOk()) return;
  (*MyDeleteReply)( handle, reply, reply_size );
}
