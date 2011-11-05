#include "../../config.h"
#include "../../os/os.h"
#include "client_config.h"

config_info::config_info( int argc, char **argv )
{
  int dn=-1, cn=-1, pn=-1, lp=-1;

  is_version=0;
  num_clients=1;
  protocol_lib_path=NULL;
  client_lib_path=NULL;
  payload_lib_path=NULL;
  client_min=client_max=client_inc=client_time=0;
  payload_min=payload_max=payload_inc=payload_time=0;
  protocol_dest_port=0;
  protocol_dest_address=NULL;
  installed=0;
  error_string=NULL;

  if (argc<3)
    {
      if (argv[1] && strcmp( argv[1], "-v" ))
	{
	  set_error( "Not enough parameters specified" );
	  return;
	}
    }
  for (int cnt=1;cnt<argc;cnt++)
    {
      int result;
      if (argv[cnt][0]!='-' && argv[cnt][0]!='/')
	{
	  set_error( "Parameter specified that does not start with - or /" );
	  return;
	}
      switch (argv[cnt][1])
	{
	case 'd':
	  result=parse_protocol_data( cnt, argc, argv, &dn );
	  if (result) return;
	  cnt+=1;
	  continue;
	case 'c':
	  result=parse_client_data( cnt, argc, argv, &cn );
	  if (result) return;
	  cnt+=1;
	  continue;
	case 'p':
	  result=parse_payload_data( cnt, argc, argv, &pn );
	  if (result) return;
	  cnt+=1;
	  continue;
	case 'n': // num clients
	  if (!argv[cnt+1])
	    {
	      set_error( "Parameter specified without value" );
	      return;
	    }
	  num_clients=atol( argv[cnt+1] );
	  cnt+=1;
	  continue;
	case 'L': // lib path
	  if (!argv[cnt+1])
	    {
	      set_error( "Parameter specified without value" );
	      return;
	    }
	  lp=cnt+1;
	  cnt+=1;
	  continue;
	case 'v': // version
	  char buf[1024];
	  snprintf( buf, 1024, "%s version %s", PRODUCT_NAME, PRODUCT_VERSION );
	  set_error( buf );
	  is_version=1;
	  return;
	default:
	  set_error( "Unknown parameter specified" );
	  return;
	}
    }
  // Logic checking
  if (dn==-1 || cn==-1 || pn==-1)
    {
      set_error( "You must specify -cn and -dn and -pn" );
      return;
    }
  if (num_clients<1)
    {
      set_error( "You must specify at least one client" );
      return;
    }
  char err_msg[1024];
  
  protocol_lib_path=set_library_path( argc, argv, dn, lp, "protocol", err_msg, 1024 );
  if (!protocol_lib_path) 
    {
      set_error( err_msg );
      return;
    }
  client_lib_path=set_library_path( argc, argv, cn, lp, "client", err_msg, 1024 );
  if (!client_lib_path) 
    {
      set_error( err_msg );
      return;
    }
  payload_lib_path=set_library_path( argc, argv, pn, lp, "payload", err_msg, 1024 );
  if (!payload_lib_path) 
    {
      set_error( err_msg );
      return;
    }
  if (protocol_dest_port<=0)
    {
      set_error( "Protocol destination port must be defined" );
      return;
    }
  if (!protocol_dest_address)
    {
      set_error( "Protocol destination address must be defined" );
      return;
    }
  installed=1;
}

int config_info::parse_client_data( int cnt, int argc, char **argv, int *cn )
{
  if (!argv[cnt+1])
    {
      set_error( "Parameter specified without value" );
      return( -1 );
    }
  switch (argv[cnt][2])
    {
    case 'n': // -cn name
      *cn=cnt+1;
      return( 0 );
    case 'i': // -cinc increment
      client_inc=atol( argv[cnt+1] );
      return( 0 );
    case 'm': // -cmin or -cmax
      switch (argv[cnt][3])
	{
	case 'i': // -cmin
	  client_min=atol( argv[cnt+1] );
	  return( 0 );
	case 'a': // -cmax
	  client_max=atol( argv[cnt+1] );
	  return( 0 );
	default:
	  set_error( "Unknown parameter specified" );
	  return( -1 );
	}
    case 't': // -ctime time
      client_time=atol( argv[cnt+1] );
      return( 0 );
    default:
      set_error( "Unknown parameter specified" );
      return( -1 );
    }
}

int config_info::parse_protocol_data( int cnt, int argc, char **argv, int *dn )
{
  if (!argv[cnt+1])
    {
      set_error( "Parameter specified without value" );
      return( -1 );
    }
  switch (argv[cnt][2])
    {
    case 'n': // -dn protocol name
      *dn=cnt+1;
      return( 0 );
    case 'p': // -dp protocol destination port
      protocol_dest_port=atol( argv[cnt+1] );
      return( 0 );
    case 'a': // -da protocol destination address
      protocol_dest_address=new char[strlen(argv[cnt+1])+1];
      if (!protocol_dest_address)
	{
	  set_error( "Out of memory" );
	  return( -1 );
	}
      strcpy( protocol_dest_address, argv[cnt+1] );
      return( 0 );
    default:
      set_error( "Unknown parameter specified" );
      return( -1 );
    }
}

int config_info::parse_payload_data( int cnt, int argc, char **argv, int *pn )
{
  if (!argv[cnt+1])
    {
      set_error( "Parameter specified without value" );
      return( -1 );
    }
  switch (argv[cnt][2])
    {
    case 'n': // -pn name
      *pn=cnt+1;
      return( 0 );
    case 'i': // -pinc increment
      payload_inc=atol( argv[cnt+1] );
      return( 0 );
    case 'm': // -pmin or -pmax
      switch (argv[cnt][3])
	{
	case 'i': // -pmin
	  payload_min=atol( argv[cnt+1] );
	  return( 0 );
	case 'a': // -pmax
	  payload_max=atol( argv[cnt+1] );
	  return( 0 );
	default:
	  set_error( "Unknown parameter specified" );
	  return( -1 );
	}
    case 't': // -ptime time
      payload_time=atol( argv[cnt+1] );
      return( 0 );
    default:
      set_error( "Unknown parameter specified" );
      return( -1 );
    }
}

void config_info::set_error( char *str )
{
  if (error_string) delete []error_string;
  error_string=new char[strlen(str)+1];
  if (!error_string) return;
  strcpy( error_string, str );
}

config_info::~config_info()
{
  if (protocol_lib_path) delete []protocol_lib_path;
  protocol_lib_path=NULL;
  if (client_lib_path) delete []client_lib_path;
  client_lib_path=NULL;
  if (payload_lib_path) delete []payload_lib_path;
  payload_lib_path=NULL;
  if (protocol_dest_address) delete []protocol_dest_address;
  protocol_dest_address=NULL;
  installed=0;
  if (error_string) delete []error_string;
  error_string=NULL;
}

const char *config_info::help()
{
  return( "Usage: trafclient [Command line parameters]\n\n"
          "- For the protocol\n"
	  "- name (-dn)\n"
	  "- destination port (-dp)\n"
	  "- destination address (-da)\n"
	  "\n"
	  "- For the payload\n"
	  "- name (-pn)\n"
	  "- min  (-pmin)\n"
	  "- max  (-pmax)\n"
	  "- increment (-pinc)\n"
	  "- time (-ptim)\n"
	  "\n"
	  "- For the clients\n"
	  "- name (-cn)\n"
	  "- min  (-cmin)\n"
	  "- max  (-cmax)\n"
	  "- increment (-cinc)\n"
	  "- time  (-ctim)\n"
	  "\n"
	  "- location of dynamic libraries (-L)\n"
	  "\n"
	  "- number of clients (-n)\n"
	  "\n"
	  "Assume only one protocol and one payload type.\n"
	  "Assume number of protocol and payload objects created is the same as the number of clients.\n"
	  "Assume that the computer assigns source port and source address.\n"
	  "\n" );
}
