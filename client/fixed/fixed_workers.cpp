#include "../../config.h"
#include "fixed_workers.h"

fixed_workers::fixed_workers( struct client_protocol *prot, struct client_payload *pay )
{
  installed=0;
  if (!MThread::isOk()) return; 
  if (!prot || !pay) return;
  myprot=prot;
  mypay=pay;
  installed=1;
}

#if defined(__WIN32__)
_fastcall fixed_workers::~fixed_workers()
#else
fixed_workers::~fixed_workers()
#endif
{
 Stop();
}

void *fixed_workers::Run()
{
  if (!isOk()) return( NULL );
  while (!stopping)
    {
      char *buf;
      unsigned size;
      
      buf=mypay->pld->GetPayload( mypay->handle, &size );
      if (buf)
	{
	  int result;
	  result=myprot->proto->SendConnectionData( myprot->proto_handle, myprot->connection, buf, size );
	  mypay->pld->ReleasePayload( mypay->handle, buf );
	  if (result>0)
	    {
	      buf=new char[size];
	      if (!buf)
		{ // Ok, not enough memory now... Let's sleep and hope there is available a bit later?
#if defined(__WIN32__)
          Sleep( 1000 );
#else
		  sleep( 1 );
#endif
		  buf=new char[size];
		}
	      if (buf)
		{
		  result=myprot->proto->ReadPacketData( myprot->proto_handle, myprot->connection, buf, size, NULL, 0 );
		  delete []buf;
		  if (result<0)
		    { // ok, could not read reply... We have to close down.. either server died or things very confused
		      return( NULL );
		    }
		}
	      else
		{ // We did not have enough memory to read the reply... We have to close down, we will hang because server will get confused
		  return( NULL );
		}
	    }
	  else
	    { // did not send data... Why? server died? Too much data in the queue? We have to close down, we will hang server?
	      return( NULL );
	    }
	}
      else
	{ // We did not get a payload... A temporary out of memory issue? Let's try again?
	}
    }
  return( NULL );
}
