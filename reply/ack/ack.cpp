#include "../../config.h"
#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#if defined(__WIN32__)
#include <winsock.h>
#endif

struct ack_packet
{
  char head[4];
  UINT32 size;
};

/*
  Returns a handle to use for all reply function calls
 */
extern "C" void * EXPORT InitReply()
{
  struct ack_packet *ack;
  ack=new struct ack_packet();
  if (!ack) return( NULL );
  memcpy( ack->head, "TRAF", 4 );
  ack->size=htonl( 8 );
  return( ack );
}

/*
  Destroys the handle used for reply's
 */
extern "C" void EXPORT DestroyReply( void *handle )
{
  struct ack_packet *ack;
  if (!handle) return;
  ack=(struct ack_packet *)handle;
  delete ack;
}

/*
  Get's reply packet to send back to client. Uses same header as payload.

  Takes the handle and the received packet as data with it's size.
  Returns a pointer to the reply payload and returns the size of that
  in reply_size
 */
extern "C" char * EXPORT GetReply( void *handle, char *packet, unsigned size, unsigned *reply_size )
{
  if (reply_size) *reply_size=8;
  return( (char *)handle );
}

/*
  Deletes the reply packet as returned by GetReply
 */
extern "C" void  EXPORT DeleteReply( void *handle, char *reply, unsigned reply_size )
{ // do nothing we do the same thing over and over again!
}

