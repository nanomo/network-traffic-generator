#include "../../config.h"
#include "server_client.h"

server_client::server_client( struct client_protocol *prot, struct server_reply *rep ) : MThread()
{
  installed=0;
  reply_handle=NULL;
  if (!MThread::isOk()) return;
  if (!prot || !rep) return;
  if (!prot->proto || !prot->proto->isOk() || !prot->proto_handle || !prot->connection) return;
  if (!rep->rep || !rep->rep->isOk()/* || !rep->handle*/) return;
  sreply=rep;
  proto=prot;
  reply_handle=sreply->rep->InitReply();
  if (!reply_handle) return;
  installed=1;
}

#if defined(__WIN32__)
_fastcall server_client::~server_client()
#else
server_client::~server_client()
#endif
{
  Stop();
  if (reply_handle && sreply)
    {
      sreply->rep->DestroyReply( reply_handle );
      reply_handle=NULL;
    }
  if (proto) delete proto;
  proto=NULL;
}

void *server_client::Run()
{
  char workbuf[MAX_BASIC_SERVER_PACKET_SIZE];
  char *repbuf;
  int result;
  unsigned reply_size;

  while (!stopping)
    {
      result=proto->proto->ReadyForData( proto->proto_handle, proto->connection, 1 );
      if (result<0)
	{ // Some error, maybe client had enough?
	  return( NULL );
	}
      if (result)
	{ // some data available
	  result=proto->proto->ReadPacketData( proto->proto_handle, proto->connection, 
					       workbuf, MAX_BASIC_SERVER_PACKET_SIZE,
					       NULL, 0 );
	  if (result<=0)
	    { // error reading. Too much data, badly formatted data, or some other error
	      return( NULL );
	    }
	  repbuf=sreply->rep->GetReply( reply_handle, workbuf, result, &reply_size );
	  if (!repbuf || reply_size<=0)
	    {
	      return( NULL );
	    }
	  result=proto->proto->SendConnectionData( proto->proto_handle, proto->connection,
						   repbuf, reply_size );
	  sreply->rep->DeleteReply( reply_handle, repbuf, reply_size );
	  if (result!=reply_size)
	    { // could not write everything?
	      return( NULL );
	    }
	}
    }
  return( NULL );
}

void server_client::TransferedData( LONGLONG *readd, LONGLONG *writed )
{
  if (!isOk()) return;
  proto->proto->TransferedData( proto->proto_handle, proto->connection, readd, writed );
}
