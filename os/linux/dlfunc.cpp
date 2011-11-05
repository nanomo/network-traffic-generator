#include "../../config.h"
#include <dlfcn.h>

/*
 This will import a .so/.dll file dynamically with the path libname.
 */
void *import_dynamic_library( char *libname )
{
  return( dlopen( libname, RTLD_GLOBAL|RTLD_NOW ) );
}

/*
  This will import a function in the .so/.dll file dynamically with the symbol name
 */
void *import_dynamic_symbol( void *handle, char *symbol_name )
{
  return( dlsym( handle, symbol_name ) );
}

/*
  This will stop the dynamic library and free it from memory
 */
int close_dynamic_library( void *handle )
{
  return( dlclose( handle ) );
}


