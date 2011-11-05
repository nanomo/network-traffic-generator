#include <windows.h>
#include <winsock.h>

void InitNetwork()
{
 WORD wVersionRequested;
 WSADATA wsaData;

 wVersionRequested = MAKEWORD( 1, 1 );
 WSAStartup( wVersionRequested, &wsaData );
}

void ShutdownNetwork()
{
 WSACleanup();
}

int TranslateInetString( char *str, void *dest, int dest_len )
{
 if (!str || !dest || dest_len<=0) return( -1 );
 memset( dest, 0, dest_len );
 if (!strcmp( str, "0.0.0.0" )) return( 0 );

 struct hostent *tmphe;
 tmphe=gethostbyaddr( str, strlen( str ), AF_INET  );
 if (!tmphe)
  { // It was not a x.y.z.a address...
   tmphe=gethostbyname( str );
   if (!tmphe) return( -1 ); // We can not do anything about this name
  }
 memcpy( dest, tmphe->h_addr_list[0], dest_len );
 return( 0 );
}


