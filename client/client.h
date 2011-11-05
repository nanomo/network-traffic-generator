#ifndef __CLIENT_H__
#define __CLIENT_H__

/* 
  This is the template for clients
 */
#include "../include/loadprotocol.h"
#include "../include/loadpayload.h"

/*
  This initializes a client with a preconfigured protocol and payload. The parameters passed is also
  the minimum number of connections, the maximum number of connections, the increment and the time factor.

  Multiple payloads and protocols can be passed to the client. Do not try to duplicate them. Use them as fairly as possible, trying not
  to re-use the same protocols and payloads. If however it must happen, so be it.
 */
extern "C" void *InitClient( struct client_protocol *proto, int num_proto, struct client_payload *pld, int num_payload, int min, int max, int incr, int time );

/*
  This destroy's the client. After this the protocol and payload objects can be free'd.
 */
extern "C" void DestroyClient( void *handle );

/*
  The client starts sending data. The client must do this asynchronous.
 */
extern "C" int StartClient( void *handle );

/*
  The client stops sending data
 */
extern "C" int StopClient( void *handle );

/*
  This queries the protocols for the amount of data transfered
 */
extern "C" void GetClientDataTransfered( void *handle, long long *readd, long long *writed );

/*
  This resets any sequence info for the client
 */
extern "C" void ResetClientSequence( void *handle );

#endif
