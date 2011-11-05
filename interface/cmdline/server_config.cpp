#include "../../config.h"
#include "server_config.h"
#include "../../os/os.h"
#include <deque>

server_config::server_config( int argc, char **argv )
{
  int rn, Ln, sn;
  std::deque <int> param_pos;
  char err_msg[1024];

  installed=0;
  is_version=0;
  reply_library_path=NULL;
  server_library_path=NULL;
  num_proto=0;
  protocol_name=NULL;
  protocol_port=NULL;
  error_string=NULL;
  rn=Ln=sn=-1;
  
  if (argc<6)
    {
      if (argv[1] && strcmp( argv[1], "-v" ))
	{
	  set_error( "Not enough parameters passed" );
	  return;
	}
    }
  for (int cnt=1;cnt<argc;cnt++)
    {
      if (argv[cnt][0]=='-' || argv[cnt][0]=='/')
	{ // Some parameter
	  switch (argv[cnt][1])
	    {
	    case 'r': // -rn Name of reply
	      if (!argv[cnt+1])
		{
		  set_error( "-rn expects a parameter" );
		  return;
		}
	      rn=cnt+1;
	      cnt+=1;
	      break;
	    case 's': // -ss Name of server
	      if (!argv[cnt+1])
		{
		  set_error( "-ss expects a parameter" );
		  return;
		}
	      sn=cnt+1;
	      cnt+=1;
	      break;
	    case 'L': // -L Dynamic library path
	      if (!argv[cnt+1])
		{
		  set_error( "-L expects a parameter" );
		  return;
		}
	      Ln=cnt+1;
	      cnt+=1;
	      break;
	    case 'v': // -v Version stuff
	      char buf[1024];
	      snprintf( buf, 1024, "%s version %s", PRODUCT_NAME, PRODUCT_VERSION );
	      set_error( buf );
	      is_version=1;
	      return;
	    default:
	      set_error( "Unknown parameter passed" );
	      return;
	    }
	}
      else
	{ // possible protocol port combination
	  if (!argv[cnt+1] || argv[cnt+1][0]=='-' || argv[cnt+1][0]=='/' || atol(argv[cnt+1])==0)
	    {
	      printf( "%s %s\n", argv[cnt], argv[cnt+1] );
	      set_error( "Unkown port, protocol parameter" );
	      return;
	    }
	  param_pos.push_back( cnt );
	  num_proto+=1;
	  cnt+=1;
	}
    }
  if (param_pos.empty())
    {
      set_error( "No protocols and ports specified" );
      return;
    }
  protocol_name=new char *[num_proto];
  if (!protocol_name) 
    {
      set_error( "Unable to allocate memory for storing protocols" );
      return;
    }
  protocol_port=new unsigned[num_proto];
  if (!protocol_port) 
    {
      set_error( "Unable to allocate memory for storing protocol ports" );
      return;
    }
  for (int cnt=0;cnt<param_pos.size();cnt++)
    {
      int pos;

      pos=param_pos[cnt];
      protocol_name[cnt]=set_library_path( argc, argv, pos, Ln, "protocol", err_msg, 1024 );
      protocol_port[cnt]=atol( argv[pos+1] );
      if (!protocol_name)
	{
	  set_error( err_msg );
	  return;
	}
    }
  if (sn<=0)
    {
      set_error( "Server name must be specified" );
      return;
    }
  server_library_path=set_library_path( argc, argv, sn, Ln, "server", err_msg, 1024 );
  if (!server_library_path)
    {
      set_error( err_msg );
      return;
    }
  if (rn<=0)
    {
      set_error( "Reply name must be specified" );
      return;
    }
  reply_library_path=set_library_path( argc, argv, rn, Ln, "reply", err_msg, 1024 );
  if (!reply_library_path)
    {
      set_error( err_msg );
      return;
    }
  installed=1;
}

void server_config::set_error( char *str )
{
  if (error_string) delete []error_string;
  error_string=new char[strlen(str)+1];
  if (!error_string) return;
  strcpy( error_string, str );
}

server_config::~server_config()
{
  if (reply_library_path) delete []reply_library_path;
  reply_library_path=NULL;
  if (server_library_path) delete []server_library_path;
  server_library_path=NULL;
  if (protocol_port) delete []protocol_port;
  protocol_port=NULL;
  if (protocol_name) 
    {
      for (int cnt=0;cnt<num_proto;cnt++)
	{
	  if (protocol_name[cnt]) delete []protocol_name[cnt];
	}
      delete []protocol_name;
      protocol_name=NULL;
    }
}

const char *server_config::help()
{
  return( "Usage: trafserver [Command line options] protocol1 port1 protocol2 port2 ... protocoln portn\n\n"
	  "- For the server\n"
	  "- Name (-sn)\n"
	  "\n"
	  "- For the reply\n"
	  "- Name (-rn)\n"
	  "\n"
	  "- location of dynamic libraries (-L)\n"
	  "\n"
	  "- For each protocol the name of the protocol followed by the port. Eg. tcp 10000\n"
	  "\n"
	  "- Assume only one type of reply\n"
	  "- Assume only one server type\n"
	  "\n" );

}

char *server_config::getProtocolLibraryPath( unsigned index )
{
  if (!isOk()) return( NULL );
  if (index>=num_proto) return( NULL );
  return( protocol_name[index] );
}

unsigned server_config::getProtocolLocalPort( unsigned index )
{
  if (!isOk()) return( 0 );
  if (index>=num_proto) return( 0 );
  return( protocol_port[index] );
}
