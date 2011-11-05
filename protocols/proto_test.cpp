#include "../config.h"
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif
#include "loadprotocol.h"

int echo_test( protocol *test )
{
  void *proto;
  void *conn;
  int result;
  char data[1024];
  char remote_addr[1024];

  proto=test->InitProtocol( 0 );
  if (!proto)
    {
      fprintf( stderr, "Unable to initialize protocol.\n" );
      return( 4 );
    }
  conn=test->CreateConnection( proto, "127.0.0.1", 7, NULL, 0, NULL );
  if (!conn)
    {
      fprintf( stderr, "Unable to create connection to 127.0.0.1:7\n" );
      test->DestroyProtocol( proto, 0 );
      return( 5 );
    }
  result=test->SendConnectionData( proto, conn, "Hello!", 7 );
  printf( "Send %d bytes\n", result );
  result=test->ReceiveConnectionData( proto, conn, data, 1024, remote_addr, 1024, 0 );
  printf( "Received %d bytes from %s: [%s]\n", result, remote_addr, data );
  test->DestroyConnection( proto, conn, 0 );
  test->DestroyProtocol( proto, 0 );
  return( 0 );
}
int test_server_server( char *path )
{
  protocol *test;
  void *proto, *conn, *serv;
  int result;
  char buf[1024], addr[1024];

  test=new protocol( path );
  if (!test)
    {
      fprintf( stderr, "Client: No memory.\n" );
      return( 2 );
    }
  if (!test->isOk())
    {
      fprintf( stderr, "Client: Unable to load %s.\n", path );
      delete test;
      return( 3 );
    }
  proto=test->InitProtocol( 1 );
  if (!proto)
    {
      delete test;
      fprintf( stderr, "Server unable to create protocol.\n" );
      return( 1 );
    }
  serv=test->ListenForConnections( proto, 10002, 10, NULL );
  if (!serv)
    {
      printf( "Unable to create server on port 10002.\n" );
      test->DestroyProtocol( proto, 0 );
      delete test;
      return( 1 );
    }
  conn=test->AcceptServerConnection( proto, serv, NULL );
  if (!conn)
    {
      printf( "Unable to create server connection.\n" );
      test->DestroyProtocol( proto, 0 );
      delete test;
      return( 1 );
    }  
  // read, write
  result=test->ReceiveConnectionData( proto, conn, buf, 1024, addr, 1024, 0 );
  printf( "Server receive %d bytes from %s: [%s]\n", result, addr, buf );
  result=test->SendConnectionData( proto, conn, buf, result );
  printf( "server %d bytes echo.\n", result );
  test->DestroyConnection( proto, conn, 0 );
  test->DestroyProtocol( proto, 0 ); 
  delete test;
  printf( "Server fini.\n" );
  return( 0 );
}

int test_server_client( char *path )
{
  protocol *test;
  void *proto;
  void *conn;
  int result;
  char buf[1024], addr[1024];
  
  test=new protocol( path );
  if (!test)
    {
      fprintf( stderr, "Client: No memory.\n" );
      return( 2 );
    }
  if (!test->isOk())
    {
      fprintf( stderr, "Client: Unable to load %s.\n", path );
      delete test;
      return( 3 );
    }
  proto=test->InitProtocol( 0 );
  if (!proto)
    {
      fprintf( stderr, "Client unable to create protocol.\n" );
      delete test;
      return( 1 );
    }
  conn=test->CreateConnection( proto, "127.0.0.1", 10002, NULL, 0, NULL );
  if (!conn)
    { 
      test->DestroyProtocol( proto, 0 );
      printf( "Unable to create client connection.\n" );
      delete test;
      return( 1 );
    }
  // Test echo
  printf( "Client connection created.\n" );
  result=test->SendConnectionData( proto, conn, "Hello!", 7 );
  printf( "client %d bytes sent.\n", result );
  result=test->ReceiveConnectionData( proto, conn, buf, 1024, addr, 1024, 0 );
  printf( "Client receive %d bytes from %s: [%s]\n", result, addr, buf );
  test->DestroyConnection( proto, conn, 0 );
  test->DestroyProtocol( proto, 0 );
  delete test;
  printf( "Client fini.\n" );
  return( 0 );
}

int server_client_test( char *path )
{
  int pid, result;
  pid=fork();
  if (pid)
    {
      result=test_server_server( path );
      waitpid( pid, NULL, 0 ); // kill the Zombies!
      return( result );
    }
  else
    {
      return( test_server_client( path ) );
    }
}

int main( int argc, char **argv )
{
  if (argc<2)
    {
      fprintf( stderr, "Usage: %s protocol.so\n", argv[0] );
      return( 1 );
    }
  protocol *test;
  
  test=new protocol( argv[1] );
  if (!test)
    {
      fprintf( stderr, "%s: No memory.\n", argv[0] );
      return( 2 );
    }
  if (!test->isOk())
    {
      fprintf( stderr, "%s: Unable to load %s.\n", argv[0], argv[1] );
      delete test;
      return( 3 );
    }
  printf( "%s loaded %s\n", argv[0], argv[1] );
  echo_test( test );
  delete test;
  server_client_test( argv[1] );
  // TODO Tests
  return( 0 );
}
