#include "../../config.h"
#include <iostream>
#include "client_config.h"
#include "../../protocols/loadprotocol.h"
#include "../../payload/loadpayload.h"
#include "../../client/loadclient.h"
#include "../../os/os.h"

/*
 Command line parameters

 - protocol
    - name (-dn)
    - destination port (-dp)
    - destination address (-da)
    - source port 
    - source address

 - payload
    - name (-pn)
    - min  (-pmin)
    - max  (-pmax)
    - increment (-pinc)
    - time (-ptim)

 - clients
    - name (-cn)
    - min  (-cmin)
    - max  (-cmax)
    - increment (-cinc)
    - time  (-ctim)

 - location of dynamic libraries (-L)

 - number of clients (-n)

  Assume only one protocol and one payload type.
  Assume number of protocol and payload objects created is the same as the number of clients.
  Assume that the computer assigns source port and source address.
 */


struct client_payload *allocate_payload( config_info *ci )
{
  struct client_payload *tmp;
  payload *mypayload;
  tmp=new struct client_payload[ci->getNumClients()];
  if (!tmp) 
    {
      std::cerr << "Out of memory loading payloads" << std::endl;
      return( NULL );
    }

  mypayload=new payload( ci->getPayloadLibraryPath() );
  if (!mypayload) 
    {
      std::cerr << "Out of memory loading payload library" << std::endl;
      return( NULL );
    }
  if (!mypayload->isOk())
    {
      delete mypayload;
      std::cerr << "Unable to load payload library" << std::endl;
      return( NULL );
    }
  for (int cnt=0;cnt<ci->getNumClients();cnt++)
    {
      void *myhandle;
      
      myhandle=mypayload->InitializePayload( ci->getPayloadMinimum(), ci->getPayloadMaximum(), 
				  ci->getPayloadIncrement(), ci->getPayloadTime(), NULL );
      if (!myhandle)
	{
	  std::cerr << "Unable to initialize payload " << cnt << std::endl;
	  for (int cnt2=0;cnt2<cnt;cnt2++)
	    {
	      mypayload->DestroyPayload( tmp[cnt].handle );
	    }
	  delete tmp[0].pld;
	  return( NULL );
	}
      else
	{
	  tmp[cnt].pld=mypayload;
	  tmp[cnt].handle=myhandle;
	}
    }
  return( tmp );
}

struct client_protocol *allocate_protocol( config_info *ci )
{
  struct client_protocol *tmp;
  protocol *myproto;
  void *my_proto_handle;

  tmp=new struct client_protocol[ci->getNumClients()];
  if (!tmp) 
    {
      std::cerr << "Out of memory loading protocol" << std::endl;
      return( NULL );
    }
  
  myproto=new protocol( ci->getProtocolLibraryPath() );
  if(!myproto) 
    {
      std::cerr << "Out of memory loading protocol library" << std::endl;
      return( NULL );
    }
  if (!myproto->isOk())
    {
      delete myproto;
      std::cerr << "Unable to load protocol library" << std::endl;
      return( NULL );
    }
  my_proto_handle=myproto->InitProtocol( 0 ); // client
  if (!my_proto_handle)
    {
      delete myproto;
      std::cerr << "Unable to initialize protocol" << std::endl;
      return( NULL );
    }
  for (int cnt=0;cnt<ci->getNumClients();cnt++)
    {
      void *myconnection;
      
      myconnection=myproto->CreateConnection( my_proto_handle, ci->getProtocolDestinationAddress(), 
					      ci->getProtocolDestinationPort(), 
					      ci->getProtocolSourceAddress(),
					      ci->getProtocolSourcePort(), 
					      NULL );
      if (myconnection)
	{
	  tmp[cnt].proto=myproto;
	  tmp[cnt].proto_handle=my_proto_handle;
	  tmp[cnt].connection=myconnection;
	}
      else
	{
	  std::cerr << "Unable to create connection " << cnt << std::endl;
      for (int cnt2=0;cnt2<cnt;cnt++)
        {
	  myproto->DestroyConnection( my_proto_handle, tmp[cnt2].connection, 0 );
        }
	  myproto->DestroyProtocol( my_proto_handle, 0 );
	  delete myproto;
      delete []tmp;
      return( NULL );
	}
    }
  return( tmp ); 
}

struct client_client
{
  client *clnt;
  void *handle;
};

struct client_client *allocate_clients( config_info *ci, struct client_protocol *proto, int num_proto, struct client_payload *pld, int num_payload )
{
  struct client_client *tmp;
  client *myclient;

  if (num_proto>ci->getClientMaximum())
    {
      std::cerr << "Warning: Not enough clients specified to use all the connections requested." << std::endl;
    }
  tmp=new client_client();
  if (!tmp)
    {
      std::cerr << "Out of memory loading client" << std::endl;
      return( NULL );
    }
  myclient=new client( ci->getClientLibraryPath() );
  if (!myclient)
    {
      std::cerr << "Out of memory loading client instance" << std::endl;
      return( NULL );
    }
  if (!myclient->isOk())
    {
      std::cerr << "Unable to initialize client instance" << std::endl;
      delete myclient;
      return( NULL );
    }
  tmp->clnt=myclient;
  tmp->handle=myclient->InitClient( proto, num_proto, pld, num_payload, 
				    ci->getClientMinimum(), ci->getClientMaximum(), 
				    ci->getClientIncrement(), ci->getClientTime() );
  if (!tmp->handle)
    {
      std::cerr << "Unable to initialize client" << std::endl;
      delete myclient;
      return( NULL );
    }
  return( tmp );
}

void cleanup( struct client_payload *payload, struct client_protocol *protocol, struct client_client *client, config_info *ci )
{
  if (payload)
    {
      for (int cnt=0;cnt<ci->getNumClients();cnt++)
	{
	  if (payload[cnt].pld && payload[cnt].handle)
	    {
	      payload[cnt].pld->DestroyPayload( payload[cnt].handle );
	      payload[cnt].handle=NULL;
	    }
	}
      if (payload[0].pld)
	{
	  delete payload[0].pld;
	  payload[0].pld=NULL;
	}
      delete []payload;
      payload=NULL;
    }
  if (protocol)
    {
      for (int cnt=0;cnt<ci->getNumClients();cnt++)
	{
	  if (protocol[cnt].connection && protocol[cnt].proto_handle && protocol[cnt].proto)
	    {
	      protocol[cnt].proto->DestroyConnection( protocol[cnt].proto_handle, protocol[cnt].connection ,0 );
	      protocol[cnt].connection=NULL;
	    }
	}
      if (protocol[0].proto_handle && protocol[0].proto)
	{
	  protocol[0].proto->DestroyProtocol( protocol[0].proto_handle, 0 );
	  protocol[0].proto_handle=NULL;
	}
      if (protocol[0].proto)
	{
	  delete protocol[0].proto;
	}
      delete []protocol;
      protocol=NULL;
    }
  if (client)
    {
      if (client->clnt && client->handle)
	{
	  client->clnt->DestroyClient( client->handle );
	  client->handle=NULL;
	}
      if (client->clnt)
	{
	  delete client->clnt;
	  client->clnt=NULL;
	}
    }
  ShutdownNetwork();
}

int main( int argc, char **argv )
{
  struct client_payload *payload;
  struct client_protocol *protocol;
  struct client_client *client;
  config_info ci( argc, argv );

  if (!ci.isOk())
    {
      if (ci.isVersion())
	{
	  std::cout << ci.getErrorString() << std::endl;
	  return( 0 );
	}
      std::cerr << argv[0] << ":Error " << ci.getErrorString() << std::endl;
      std::cerr << std::endl << ci.help() << std::endl;
      return( 1 );
    }
  std::cout << argv[0] << " Starting" << std::endl;
  std::cout << "Initializing Payloads" << std::endl;
  payload=allocate_payload( &ci );
  if (!payload) return( 2 );
  std::cout << "Initializing Protocols" << std::endl;
  InitNetwork();
  protocol=allocate_protocol( &ci );
  if (!protocol)
    {
      cleanup( payload, NULL, NULL, &ci );
      return( 3 );
    }
  client=allocate_clients( &ci, protocol, ci.getNumClients(), payload, ci.getNumClients() );
  if (!client)
    {
      cleanup( payload, protocol, NULL, &ci );
      return( 4 );
    }
  std::cout << "Starting clients. Press ENTER to end." << std::endl;
  client->clnt->StartClient( client->handle );
  getchar();
  client->clnt->StopClient( client->handle );
  cleanup( payload, protocol, client, &ci );
  std::cout << "Client stopped." << std::endl;
  return( 0 );
}
