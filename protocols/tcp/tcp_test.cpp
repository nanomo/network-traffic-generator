#include "../../config.h"
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif
#include "../protocol.h"

int test_echo()
{
  void *proto;
  void *conn;
  int result;
  char buf[1024], addr[1024];

  proto=InitProtocol( 0 );
  if (!proto)
    {
      fprintf( stderr, "%s: Unable to create protocol.\n" );
      return( 1 );
    }
  conn=CreateConnection( proto, "127.0.0.1", 7, NULL, 10003, NULL );
  if (!conn)
    { 
      DestroyProtocol( proto, 0 );
      printf( "Unable to create connection.\n" );
      return( 1 );
    }
  // Test echo
  printf( "Connection created.\n" );
  printf( "Press ENTER to continue..." );
  getchar();
  result=SendConnectionData( proto, conn, "Hello!", 7 );
  printf( "%d bytes sent.\n", result );
  result=ReceiveConnectionData( proto, conn, buf, 1024, addr, 1024, 0 );
  printf( "Receive %d bytes from %s: [%s]\n", result, addr, buf );
  DestroyConnection( proto, conn, 0 );
  DestroyProtocol( proto, 0 );
  return( 0 );
}

int test_server_server()
{
  void *proto, *conn, *serv;
  int result;
  char buf[1024], addr[1024];

  proto=InitProtocol( 1 );
  if (!proto)
    {
      fprintf( stderr, "%s: Server unable to create protocol.\n" );
      return( 1 );
    }
  serv=ListenForConnections( proto, 10002, 10, NULL );
  if (!serv)
    {
      printf( "Unable to create server on port 10002.\n" );
      DestroyProtocol( proto, 0 );
      return( 1 );
    }
  conn=AcceptServerConnection( proto, serv, NULL );
  if (!conn)
    {
      printf( "Unable to create server connection.\n" );
      DestroyProtocol( proto, 0 );
      return( 1 );
    }  
  // read, write
  result=ReceiveConnectionData( proto, conn, buf, 1024, addr, 1024, 0 );
  printf( "Server receive %d bytes from %s: [%s]\n", result, addr, buf );
  result=SendConnectionData( proto, conn, buf, result );
  printf( "server %d bytes echo.\n", result );
  DestroyConnection( proto, conn, 0 );
  DestroyProtocol( proto, 0 ); 
  printf( "Server fini.\n" );
  return( 0 );
}

int test_server_client()
{
  void *proto;
  void *conn;
  int result;
  char buf[1024], addr[1024];

  proto=InitProtocol( 0 );
  if (!proto)
    {
      fprintf( stderr, "%s: Client unable to create protocol.\n" );
      return( 1 );
    }
  conn=CreateConnection( proto, "127.0.0.1", 10002, NULL, 0, NULL );
  if (!conn)
    { 
      DestroyProtocol( proto, 0 );
      printf( "Unable to create client connection.\n" );
      return( 1 );
    }
  // Test echo
  printf( "Client connection created.\n" );
  result=SendConnectionData( proto, conn, "Hello!", 7 );
  printf( "client %d bytes sent.\n", result );
  result=ReceiveConnectionData( proto, conn, buf, 1024, addr, 1024, 0 );
  printf( "Client receive %d bytes from %s: [%s]\n", result, addr, buf );
  DestroyConnection( proto, conn, 0 );
  DestroyProtocol( proto, 0 );
  printf( "Client fini.\n" );
  return( 0 );
}

int test_server()
{
  int pid;
  pid=fork();
  if (pid)
    { // parent
      int result;
      result=test_server_server();
      waitpid( pid, NULL, 0 ); // kill the Zombies!
      return( result );
    }
  else
    { // child
      return( test_server_client() );
    }
}
int main( int argc, char **argv )
{
  int result;

  result=test_echo();
  if (result) 
    {
      return( result );
    }
  result=test_server();
  return( result );
}
