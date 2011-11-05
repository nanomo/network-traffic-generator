#include "../../config.h"
#include "server_listener.h"

server_listener::server_listener( struct client_protocol *prot, struct server_reply *rep ): MThread()
{
  installed=0;
  if (!prot || !rep) return;
  if (!prot->proto || !prot->proto->isOk() || !prot->proto_handle || !prot->connection) return;
  if (!rep->rep || !rep->rep->isOk()/* || !rep->handle*/) return;
  proto=prot;
  sreply=rep;
  installed=1;
}

#if defined(__WIN32__)
_fastcall server_listener::~server_listener()
#else
server_listener::~server_listener()
#endif
{
  server_client *tmp;

  if (clients.size()!=0)
    {
  tmp=clients.front();
  while (tmp)
    {
      delete tmp;
      clients.pop_front();
       if (clients.size()!=0)
        {
      tmp=clients.front();
    }
       else
        {
         tmp=NULL;
        }
     }
   }  
}

void *server_listener::Run()
{

  while (!stopping)
    {
      int result;

      result=proto->proto->ReadyForData( proto->proto_handle, proto->connection, 4 );
      if (result<0)
	{ // Some error
	  return( NULL );
	}
      if (result)
	{ // some connectionavailable
	  
	  struct client_protocol *new_prot;
	  server_client *tmp_client;

	  new_prot=new struct client_protocol();
	  if (!new_prot)
	    {
	      return( NULL );
	    }
	  new_prot->proto=proto->proto;
	  new_prot->proto_handle=proto->proto_handle;


	  new_prot->connection=new_prot->proto->AcceptServerConnection( proto->proto_handle, proto->connection, NULL );
	  if (!new_prot->connection)
	    {
	      delete new_prot;
	      return( NULL );
	    }


	  tmp_client=new server_client( new_prot, sreply );
	  if (!tmp_client || !tmp_client->isOk())
	    {
	      delete new_prot;
	      if (tmp_client) delete tmp_client;
	      return( NULL );
	    }
	  clients.push_back( tmp_client );
	  tmp_client->Start();
	}
    }
  return( NULL );
}

void server_listener::TransferedData( LONGLONG *readd, LONGLONG *writed )
{
  int cnt;
  LONGLONG rr, ww, rt, wt;
 
  wt=rt=0;
  for (cnt=0;cnt<clients.size();cnt++)
    {
      (clients[cnt])->TransferedData( &rr, &ww );
      wt+=ww;
      rt+=rr;
    }
  if (readd) *readd=rt;
  if (writed) *writed=wt;
}
