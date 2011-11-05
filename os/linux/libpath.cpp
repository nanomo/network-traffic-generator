#include "../os.h"

#define NUM_LIB_PATH   7

char *lib_path[NUM_LIB_PATH]=
  {
    "/lib/traffic",
    "/usr/lib/traffic",
    "/usr/local/traffic/lib",
    "/usr/local/lib/traffic",
    "../../lib_dynamic",
    "../lib_dynamic",
    "./lib_dynamic"
  };

char *set_library_path( int argc, char **argv, int pos, int lib_pos, char *prefix, char *err_msg, int err_len )
{
  char buf[1024];
  char *tmp;
  if (argc<=0 || !argv || pos<=0 || lib_pos>=argc || pos>=argc || !prefix) return( NULL );

  if (lib_pos>0)
    {
      snprintf( buf, 1024, "%s/%s_%s%s", argv[lib_pos], prefix, argv[pos], DLL_EXT );
    }
  else
    {
      snprintf( buf, 1024, "%s_%s%s", prefix, argv[pos], DLL_EXT );
    }
  if (access( buf, R_OK ))
    { // Ok the nice way did not work, iterate through possible spots before giving up
      for (int cnt=0;cnt<NUM_LIB_PATH;cnt++)
	{
	  snprintf( buf, 1024, "%s/%s_%s%s", lib_path[cnt], prefix, argv[pos], DLL_EXT );
	  if (!access( buf, R_OK ))
	    { // Found it!
	      tmp=new char[strlen( buf ) + 1 ];
	      if (!tmp)
		{
		  if (err_msg) snprintf( err_msg, err_len, "Out of memory" );
		  return( NULL );
		}
	      strcpy( tmp, buf );
	      return( tmp );
	    }
	}
      if (err_msg) snprintf( err_msg, err_len, "Unable to find %s plugin", prefix );
      return( NULL );
    }
  tmp=new char[strlen( buf ) + 1 ];
  if (!tmp)
    {
      if (err_msg) snprintf( err_msg, err_len, "Out of memory" );
      return( NULL );
    }
  strcpy( tmp, buf );
  return( tmp );
}
