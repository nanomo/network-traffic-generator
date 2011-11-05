#include "../../config.h"
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#if defined(__WIN32__)
#include <winsock.h>
#endif

struct payload_header
{
  char head[4];
  UINT32 size;
};

/*
  This initializes the payload. Returns void * to be used as handle.

  min - Minimum size
  max - Maximum size
  increment - If there is an increment base it on this
  time - Time factor for calculations
  other - Implementation specific pointer - please do not use
 */

struct payload_stuff
{
 union
 {
   char *data;
   struct payload_header *header;
 };
  unsigned size;
};

extern "C" void * EXPORT InitializePayload( unsigned min, unsigned max, unsigned increment, unsigned time, void *other )
{
  struct payload_stuff *payload;
  if (max<8) return( NULL );
  payload=new payload_stuff();
  if (!payload) return( NULL );
  payload->size=max;
  payload->data=NULL;
  return( payload );
}


/*
  This destroys the payload
 */
extern "C" void  EXPORT DestroyPayload( void *handle )
{
  struct payload_stuff *payload;
  if (!handle) return;
  payload=(struct payload_stuff *)handle;

  if (payload->data) delete [](payload->data);
  delete payload;
}

/*
  Get payload to be sent

  Returns a "char *" buffer as payload, already filled with junk.
  Returns total size in the "size" pointer.

  Must be freed by calling ReleasePayload.
 */
extern "C" char * EXPORT GetPayload( void *handle, unsigned *size )
{
  struct payload_stuff *payload;
  if (!handle) return( NULL );
  payload=(struct payload_stuff *)handle;

  if (size) *size=payload->size;
  if (payload->data) return( payload->data );
  payload->data=new char[payload->size];
  memcpy( payload->header->head, "TRAF", 4 );
  payload->header->size=htonl( payload->size );
  return( payload->data );
}

/*
  This releases the payload as created by GetPayload.
 */
extern "C" void  EXPORT ReleasePayload( void *handle, char *mpayload )
{
  // We re-use the same payload so it just wastes cycles to allocate and free it all the time  
}

/*
  This resets the payload sequence so that it starts from scratch
 */
extern "C" void  EXPORT ResetPayloadSequence( void *handle )
{
  // No sequence so do nothing
}
