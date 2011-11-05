#ifndef __OS_H__
#define __OS_H__

/*
  This is all the OS dependant functionality... As far as possible...
 */

/*
  All the stuff needed for loading DLL's or .so's from different spots
 */
#if defined(__WIN32__)
#define DLL_EXT ".dll"
#else
#define DLL_EXT ".so"
#endif

/* This will set the path to search for .dll or .so */
char *set_library_path( int argc, char **argv, int pos, int lib_pos, char *prefix, char *err_msg, int err_len );

/*
 This will import a .so/.dll file dynamically with the path libname.
 */
void *import_dynamic_library( char *libname );

/*
  This will import a function in the .so/.dll file dynamically with the symbol name
 */
void *import_dynamic_symbol( void *handle, char *symbol_name );

/*
  This will stop the dynamic library and free it from memory
 */
int close_dynamic_library( void *handle );

/* 
  Thread class
 */

#include "threads.h"

/*
 Network initialization
 */
void InitNetwork();

void ShutdownNetwork();

int TranslateInetString( char *str, void *dest, int dest_len );

#endif
