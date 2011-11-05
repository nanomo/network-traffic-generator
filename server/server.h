#ifndef __SERVER_H__
#define __SERVER_H__

#include "../include/loadprotocol.h"
#include "../include/loadreply.h"

/*
 Template for servers
 */

/*
  This initializes the server with the needed protocol and reply structures. Returns a handle
 */
extern "C" void *InitServer( struct client_protocol **proto, int num_protocol, struct server_reply **rep, int num_reply );

/*
  This stops the server
 */
extern "C" void DestroyServer( void *handle );

/*
  This starts the server listening for clients
 */
extern "C" int StartServer( void *handle );

/*
  This stops the server listening for clients
 */
extern "C" int StopServer( void *handle );

/*
  This get's the total data transfered by this server
 */
extern "C" void ServerTransferedData( void *handle, long long *readd, long long *writed );

#endif
