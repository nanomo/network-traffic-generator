#include "../../config.h"
#include "server_listener.h"

struct basic_server
{
  server_listener **list;
  int num;
};

/*
  This initializes the server with the needed protocol and reply structures. Returns a handle
 */
extern "C" void * EXPORT InitServer( struct client_protocol **proto, int num_protocol, struct server_reply **rep, int num_reply )
{
  struct basic_server *tmp;
  int cnt;

  tmp=new struct basic_server();
  if (!tmp) return( NULL );
  tmp->list=new server_listener *[num_protocol];
  if (!tmp->list) 
    {
      delete tmp;
      return( NULL );
    }
  tmp->num=num_protocol;
  for (cnt=0;cnt<num_protocol;cnt++)
    {
      tmp->list[cnt]=new server_listener( &((*proto)[cnt]), &((*rep)[cnt%num_reply]) );
      if (!tmp->list[cnt] || !tmp->list[cnt]->isOk())
	{
	  if (tmp->list[cnt]) delete tmp->list[cnt];
	  for (int cnt2=0;cnt2<cnt;cnt2++)
	    delete tmp->list[cnt2];
	  delete []tmp->list;
	  delete tmp;
	  return( NULL );
	}
    }
  return( tmp );
}

/*
  This stops the server
 */
extern "C" void  EXPORT DestroyServer( void *handle )
{
  struct basic_server *tmp;
  int cnt;

  tmp=(struct basic_server *)handle;
  if (!tmp) return;

  if (tmp->list)
    {
      for (cnt=0;cnt<tmp->num;cnt++)
	{
	  if (tmp->list[cnt]) delete tmp->list[cnt];
	}
      delete []tmp->list;
    }
  delete tmp;
}

/*
  This starts the server listening for clients
 */
extern "C" int  EXPORT StartServer( void *handle )
{
  struct basic_server *tmp;
  int cnt;

  tmp=(struct basic_server *)handle;
  if (!tmp) return( -1 );

  if (tmp->list)
    {
      for (cnt=0;cnt<tmp->num;cnt++)
	{
	  if (tmp->list[cnt])
	    tmp->list[cnt]->Start();
	}
    }
  return( 0 );
}

/*
  This stops the server listening for clients
 */
extern "C" int  EXPORT StopServer( void *handle )
{
 struct basic_server *tmp;
 int cnt;
 
 tmp=(struct basic_server *)handle;
 if (!tmp) return( -1 );
 
 if (tmp->list)
   {
     for (cnt=0;cnt<tmp->num;cnt++)
       {
	 if (tmp->list[cnt])
	   tmp->list[cnt]->Stop();
       }
   }
 return( 0 );
}

/*
  This get's the total data transfered by this server
 */
extern "C" void  EXPORT ServerTransferedData( void *handle, LONGLONG *readd, LONGLONG *writed )
{
 struct basic_server *tmp;
 int cnt;
 LONGLONG rt, rr, wt, ww;

 tmp=(struct basic_server *)handle;
 if (!tmp) return;
  
 if (tmp->list)
   {
     rt=wt=0;
     for (cnt=0;cnt<tmp->num;cnt++)
       {
	 if (tmp->list[cnt])
	   {
	     tmp->list[cnt]->TransferedData( &rr, &ww );
	     rt+=rr;
	     wt+=ww;
	   }
       }
     if (readd) *readd=rt;
     if (writed) *writed=wt;
   }
}

