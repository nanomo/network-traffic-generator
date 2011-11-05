#ifndef __SERVER_CONFIG_H__
#define __SERVER_CONFIG_H__

class server_config
{
 public:
  server_config( int argc, char **argv );
  virtual ~server_config();
  int isOk() { return( installed ); };
  int isVersion() { return( is_version ); };
  const char *help();
  char *getErrorString() { return( error_string ); };
  char *getReplyLibraryPath() { return( reply_library_path ); };
  char *getServerLibraryPath() { return( server_library_path ); };
  int getNumProtocols() { return( num_proto ); };
  char *getProtocolLibraryPath( unsigned index );
  unsigned getProtocolLocalPort( unsigned index );
  char *getProtocolLocalAddress( unsigned index ) { return( "0.0.0.0" ); };
  char *getProtocolRemoteAddress( unsigned index ) { return( "0.0.0.0" ); };
  unsigned getProtocolRemotePort( unsigned index ) { return( 0 ); };
 protected:
  char *reply_library_path;
  char *server_library_path;
  int num_proto;
  char **protocol_name;
  unsigned *protocol_port;
  char *error_string;
  int is_version;

  void set_error( char *str );
 private:
  int installed;
};
#endif
