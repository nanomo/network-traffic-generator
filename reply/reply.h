#ifndef __REPLY_H__
#define __REPLY_H__

/* 
  This is the template for reply types. This is loaded by the server to reply to packets sent by the client
 */

/*
  Returns a handle to use for all reply function calls
 */
extern "C" void *InitReply();

/*
  Destroys the handle used for reply's
 */
void DestroyReply( void *handle );

/*
  Get's reply packet to send back to client. Uses same header as payload.

  Takes the handle and the received packet as data with it's size.
  Returns a pointer to the reply payload and returns the size of that
  in reply_size
 */
char *GetReply( void *handle, char *packet, unsigned size, unsigned *reply_size );

/*
  Deletes the reply packet as returned by GetReply
 */
void DeleteReply( void *handle, char *reply, unsigned reply_size );

#endif
