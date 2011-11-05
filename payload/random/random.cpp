#include "../../config.h"
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#if defined(__WIN32__)
#include <winsock.h>
// Windows does not follow random specification
#define random rand
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
  unsigned min_size;
  unsigned max_size;
  unsigned increment;
  unsigned time;
  char *oldpayload;
  int sequence;
};

extern "C" void * EXPORT InitializePayload( unsigned min, unsigned max, unsigned increment, unsigned time, void *other )
{
  struct payload_stuff *payload;
  if (max<8 || min<8 || max>RAND_MAX) return( NULL );
  payload=new payload_stuff();
  if (!payload) return( NULL );
  payload->max_size=max;
  payload->min_size=min;
  payload->increment=increment;
  if (payload->increment<=0) payload->increment=1;
  payload->time=time;
  payload->oldpayload=NULL;
  payload->sequence=0;
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
  if (payload->oldpayload) delete [](payload->oldpayload);
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
  struct payload_header *header;
  struct payload_stuff *payload;
  char *tmpdata;
  unsigned local_size;
  unsigned range;

  if (!handle) return( NULL );
  payload=(struct payload_stuff *)handle;

  if (payload->oldpayload)
    { // There is an old payload that may be re-used
      if (++(payload->sequence)<payload->time)
	{
	  return( payload->oldpayload );
	}
      else
	{
	  delete []payload->oldpayload;
	  payload->oldpayload=NULL;
	}
      }
  range=payload->max_size-payload->min_size;
  if (range<=0) return( NULL );

  local_size=payload->min_size+(unsigned)((((double)random())/((double)RAND_MAX))*range);

  if (size) *size=local_size;
  tmpdata=new char[local_size];
  if (!tmpdata) return( NULL );
  header=(struct payload_header *)tmpdata;
  memcpy( header->head, "TRAF", 4 );
  header->size=htonl( local_size );
  payload->oldpayload=tmpdata;
  payload->sequence=0;
  return( tmpdata );
}

/*
  This releases the payload as created by GetPayload.
 */
extern "C" void  EXPORT ReleasePayload( void *handle, char *mpayload )
{
  // we do this ourselves so ignore
}

/*
  This resets the payload sequence so that it starts from scratch
 */
extern "C" void  EXPORT ResetPayloadSequence( void *handle )
{
  struct payload_stuff *payload;
  if (!handle) return;
  payload=(struct payload_stuff *)handle;

  payload->sequence=0;
  if (payload->oldpayload) delete []payload->oldpayload;
  payload->oldpayload=NULL;
}
