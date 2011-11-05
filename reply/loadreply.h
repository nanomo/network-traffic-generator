#ifndef __LOAD_REPLY_H__
#define __LOAD_REPLY_H__

class reply
{
 public:
  reply( char *pathname );
  virtual ~reply();
  int isOk() { return( installed ); };
  void *InitReply();
  void DestroyReply( void *handle );
  char *GetReply( void *handle, char *packet, unsigned size, unsigned *reply_size );
  void DeleteReply( void *handle, char *reply, unsigned reply_size );
 protected:  
  void *libhandle;
  void * EXPORT (*MyInitReply)();
  void  EXPORT (*MyDestroyReply)( void *handle );
  char * EXPORT (*MyGetReply)( void *handle, char *packet, unsigned size, unsigned *reply_size );
  void  EXPORT (*MyDeleteReply)( void *handle, char *reply, unsigned reply_size );
 private:
  int installed;
};

struct server_reply
{
  reply *rep;
};

#endif
