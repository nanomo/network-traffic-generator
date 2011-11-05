#ifndef __FIXED_CLIENT_H__
#define __FIXED_CLIENT_H__

#include "fixed_workers.h"

class fixed_client
{
public:
  fixed_client( struct client_protocol *proto, int num_proto, struct client_payload *pld, int num_payload, int num_clients );
  virtual ~fixed_client();
  int isOk() { return( installed ); };
  int Start();
  int Stop();
  void GetClientDataTransfered( LONGLONG *readd, LONGLONG *writed );
  void ResetClientSequence();
protected:
  fixed_workers **workers;
  int num_workers;
  struct client_protocol *my_proto;
  int my_num_proto;
  struct client_payload *my_pld;
  int my_num_pld;
private:
  int installed;
};

#endif
