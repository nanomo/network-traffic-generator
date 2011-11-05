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
struct echo_header
{
  char head[4];
  UINT32 size;
};

struct echo_reply
{
  char *buf;
  unsigned last_size;
  unsigned total_size;
};
/*
  Returns a handle to use for all reply function calls
 */
extern "C" void * EXPORT InitReply()
{
  struct echo_reply *tmp;
  tmp=new struct echo_reply();
  if (!tmp) return( NULL );
  tmp->buf=NULL;
  tmp->last_size=0;
  tmp->total_size=0;
  return( tmp );
}

/*
  Destroys the handle used for reply's
 */
extern "C" void  EXPORT DestroyReply( void *handle )
{
  struct echo_reply *tmp;
  if (!handle) return;
  tmp=(struct echo_reply *)handle;
  if (tmp->buf) delete [](tmp->buf);
  delete tmp;
}

/*
  Get's reply packet to send back to client. Uses same header as payload.

  Takes the handle and the received packet as data with it's size.
  Returns a pointer to the reply payload and returns the size of that
  in reply_size
 */
extern "C" char * EXPORT GetReply( void *handle, char *packet, unsigned size, unsigned *reply_size )
{
  struct echo_reply *tmp;
  if (!handle) return( NULL );
  tmp=(struct echo_reply *)handle;
  if (size>tmp->total_size || !tmp->buf)
    {
      if (tmp->buf) delete [](tmp->buf);
      tmp->total_size=size*2+10240;
      tmp->buf=new char[tmp->total_size];
      if (!tmp->buf) return( NULL );
    }
  tmp->last_size=size;
  if (reply_size) *reply_size=size;
  memcpy( tmp->buf, packet, size );
  return( tmp->buf );
}

/*
  Deletes the reply packet as returned by GetReply
 */
extern "C" void  EXPORT DeleteReply( void *handle, char *reply, unsigned reply_size )
{
  // do nothing, it will be destroyed in the DestroyReply
}

