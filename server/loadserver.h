#ifndef __LOAD_SERVER_H__
#define __LOAD_SERVER_H__

class server
{
 public:
  server( char *path );
  virtual ~server();
  int isOk() { return( installed ); };
  void *InitServer( struct client_protocol **proto, int num_protocol, struct server_reply **rep, int num_reply );
  void DestroyServer( void *handle );
  int StartServer( void *handle );
  int StopServer( void *handle );
  void ServerTransferedData( void *handle, LONGLONG *readd, LONGLONG *writed );
 protected:
  void *lib_handle;

  void * EXPORT (*MyInitServer)( struct client_protocol **proto, int num_protocol, struct server_reply **rep, int num_reply );
  void  EXPORT (*MyDestroyServer)( void *handle );
  int  EXPORT (*MyStartServer)( void *handle );
  int  EXPORT (*MyStopServer)( void *handle );
  void  EXPORT (*MyServerTransferedData)( void *handle, LONGLONG *readd, LONGLONG *writed );
 private:
  int installed;
};
#endif
