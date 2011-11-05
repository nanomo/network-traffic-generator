#include "../../config.h"
#include <iostream>
#include <map>
#include "server_config.h"
#include "../../reply/loadreply.h"
#include "../../protocols/loadprotocol.h"
#include "../../server/loadserver.h"
#include "../../os/os.h"
//#include <dlfcn.h>

/*
  Command line parameters

  - Reply
  
     -rn Name of reply

  - Server
    
     -sn Name of server

  - Protocol
  
     [name port] [name port] .....

  -L Path to dynamic libraries

  - Assume only one type of reply
  - Assume only one server type
 */

struct server_reply *allocate_reply( server_config *sc )
{
  struct server_reply *tmp;
  tmp=new struct server_reply();
  if (!tmp)
    {
      std::cerr << "Out of memory" << std::endl;
      return( NULL );
    }
  tmp->rep=new reply( sc->getReplyLibraryPath() );
  if (!tmp->rep)
    {
      std::cerr << "Out of memory" << std::endl;
      return( NULL );
    }
  if (!tmp->rep->isOk())
    {
      std::cerr << "Unable to load reply library" << std::endl;
      delete tmp;
      return( NULL );
    }
  return( tmp );
}

struct cpc_map
{
  bool operator()( const char *s1, const char *s2 )
  {
    return strcmp(s1, s2) < 0;
  }
};

struct client_protocol *allocate_protocol( server_config *sc )
{
  std::map <char *,struct client_protocol *,cpc_map> known_prot;
  struct client_protocol *tmp;
  
  tmp=new struct client_protocol[sc->getNumProtocols()];
  if (!tmp)
    {
      std::cerr << "Unable to allocate server protocol" << std::endl;
      return( NULL );
    }
  for (int cnt=0;cnt<sc->getNumProtocols();cnt++)
    {
      std::map <char *,struct client_protocol *,cpc_map>::iterator cur = known_prot.find( sc->getProtocolLibraryPath( cnt ) );

      tmp[cnt].proto=NULL;
      tmp[cnt].proto_handle=NULL;
      tmp[cnt].connection=NULL;      
      if (cur==known_prot.end())
	{ // first time this protocol come's up...
	  tmp[cnt].proto=new protocol( sc->getProtocolLibraryPath( cnt ) );
	  if (!tmp[cnt].proto)
	    {
	      std::cerr << "Unable to allocate memory for protocol " << cnt << std::endl;
	    }
	  else
	    {
	      if (!tmp[cnt].proto->isOk())
		{
		  std::cerr << "Unable to initialize protocol " << cnt << std::endl;
		  delete tmp[cnt].proto;
		  tmp[cnt].proto=NULL;
		}
	      else
		{
		  tmp[cnt].proto_handle=tmp[cnt].proto->InitProtocol( 1 );
		  if (!tmp[cnt].proto_handle)
		    {
		      std::cerr << "Unable to start server for protocol " << cnt << std::endl;
		    }
		  else
		    {
		      tmp[cnt].connection=tmp[cnt].proto->ListenForConnections( tmp[cnt].proto_handle,
										sc->getProtocolLocalPort( cnt ),
										10, NULL );
		      if (!tmp[cnt].connection)
			{
			  std::cerr << "Unable to listen for protocol " << cnt << " on port " << sc->getProtocolLocalPort( cnt ) << std::endl;
			}
		      else
			{
			  known_prot[sc->getProtocolLibraryPath( cnt )]=&(tmp[cnt]);
			}
		    }
		}
	    }
	}
      else
	{ // we have seen this protocol before - so just a new connection
	  tmp[cnt].proto=(known_prot[sc->getProtocolLibraryPath( cnt )])->proto;
	  tmp[cnt].proto_handle=(known_prot[sc->getProtocolLibraryPath( cnt)])->proto_handle;
	  tmp[cnt].connection=tmp[cnt].proto->ListenForConnections( tmp[cnt].proto_handle,
								    sc->getProtocolLocalPort( cnt ),
								    10, NULL );
	  if (!tmp[cnt].connection)
	    {
	      std::cerr << "Unable to listen for protocol " << cnt << " on port " << sc->getProtocolLocalPort( cnt ) << std::endl;
	    }
	}										
    }
  return( tmp );
}

struct server_server
{
  server *serv;
  void *handle;
};

struct server_server *allocate_servers( server_config *sc, struct server_reply *sr, struct client_protocol *sp )
{
  struct server_server *tmp;
  tmp=new struct server_server();
  if (!tmp)
    {
      std::cerr << "Unable to allocate server" << std::endl;
      return( NULL );
    }
  tmp->serv=new server( sc->getServerLibraryPath() );
  if (!tmp->serv)
    {
      std::cerr << "Unable to allocate server library" << std::endl;
      delete tmp;
      return( NULL );
    }
  if (!tmp->serv->isOk())
    {
      std::cerr << "Unable to initialize server library" << std::endl;
      delete tmp->serv;
      delete tmp;
      return( NULL );
    }
  tmp->handle=tmp->serv->InitServer( &sp, sc->getNumProtocols(), &sr, 1 );
  if (!tmp->handle)
    {
      delete tmp->serv;
      delete tmp;
      std::cerr << "Unable to initialize server" << std::endl;
      return( NULL );
    }
  return( tmp );
}

void cleanup( server_config *sc, struct server_server *ss, 
	      struct server_reply *sr, struct client_protocol *sp )
{
  if (ss)
    {
      if (ss->handle && ss->serv) ss->serv->DestroyServer( ss->handle );
      if (ss->serv) delete ss->serv;
      delete ss;
      ss=NULL;
    }
  if (sr)
    {
      /*      if (sr->handle && sr->rep)
	{
	  sr->rep->DestroyReply( sr->handle );
	  } */
      if (sr->rep)
	{
	  delete sr->rep;
	}
      delete sr;
      sr=NULL;
    }
  if (sp)
    {
      for (int cnt=0;cnt<sc->getNumProtocols();cnt++)
	{
	  struct client_protocol *tmp;
	  
	  tmp=&(sp[cnt]);
	  if (tmp->proto && tmp->proto_handle && tmp->connection)
	    {
	      for (int cnt2=cnt+1;cnt2<sc->getNumProtocols();cnt2++)
		{
		  if (tmp->proto_handle==sp[cnt2].proto_handle && sp[cnt2].proto_handle && sp[cnt2].connection)
		    {
		      sp[cnt2].proto->DestroyConnection( sp[cnt2].proto_handle, sp[cnt2].connection, 0 );
		      sp[cnt2].proto_handle=NULL;
		      sp[cnt2].connection=NULL;
		    }
		}
	      tmp->proto->DestroyProtocol( tmp->proto_handle, 0 );
	      delete tmp->proto;
	      tmp->proto=NULL;
	      tmp->proto_handle=NULL;
	    }
	}
      delete []sp;
      sp=NULL;
    }
  ShutdownNetwork();
}

int main( int argc, char **argv )
{
  server_config sc( argc, argv );
  struct server_server *ss;
  struct server_reply *sr;
  struct client_protocol *sp;

  if (!sc.isOk())
    {
      std::cerr << sc.getErrorString() << std::endl << std::endl;
      if (!sc.isVersion()) std::cerr << sc.help() << std::endl;
      return( 1 );
    }

  std::cout << argv[0] << " Starting" << std::endl;

  std::cout << "Initializing Reply" << std::endl;
  sr=allocate_reply( &sc );
  if (!sr) return( 2 );

  std::cout << "Initializing Protocols" << std::endl;
  InitNetwork();
  sp=allocate_protocol( &sc );
  if (!sp)
    {
      cleanup( &sc, NULL, sr, sp );
      return( 3 );
    }
  std::cout << "Initializing Servers" << std::endl;
  ss=allocate_servers( &sc, sr, sp );
  if (!ss)
    {
      cleanup( &sc, ss, sr, sp );
      return( 4 );
    }
  std::cout << "Starting servers. Press ENTER to end." << std::endl;
  ss->serv->StartServer( ss->handle );
  getchar();
  ss->serv->StopServer( ss->handle );
  std::cout << "Stopped servers." << std::endl;
  cleanup( &sc, ss, sr, sp );
  return( 0 );
}
