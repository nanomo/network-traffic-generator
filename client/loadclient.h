#ifndef __LOAD_CLIENTS_H__
#define __LOAD_CLIENTS_H__

#include "../protocols/loadprotocol.h"
#include "../payload/loadpayload.h"

class client
{
 public:
  client( char *pathname );
  virtual ~client();
  int isOk() { return( installed ); };
  void *InitClient( struct client_protocol *proto, int num_proto, struct client_payload *pld, int num_payload, int min, int max, int incr, int time );
  void DestroyClient( void *handle );
  int StartClient( void *handle );
  int StopClient( void *handle );
  void GetClientDataTransfered( void *handle, LONGLONG *readd, LONGLONG *writed );
  void ResetClientSequence( void *handle );
 protected:
  void *libhandle;
  void * EXPORT (*MyInitClient)( struct client_protocol *proto, int num_proto, struct client_payload *pld, int num_payload, int min, int max, int incr, int time );
  void  EXPORT (*MyDestroyClient)( void *handle );
  int  EXPORT (*MyStartClient)( void *handle );
  int  EXPORT (*MyStopClient)( void *handle );
  void  EXPORT (*MyGetClientDataTransfered)( void *handle, LONGLONG *readd, LONGLONG *writed );
  void  EXPORT (*MyResetClientSequence)( void *handle );
 private:
  int installed;
};

#endif
