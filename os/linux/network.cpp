#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

void InitNetwork()
{
}

void ShutdownNetwork()
{
}

int TranslateInetString( char *str, void *dest, int dest_len )
{
 return( inet_pton( AF_INET, str, dest ) );
}
