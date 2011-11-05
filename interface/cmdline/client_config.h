#ifndef __CLIENT_CONFIG_H__
#define __CLIENT_CONFIG_H__

class config_info
{
 public:
  config_info( int argc, char **argv );
  virtual ~config_info();
  int isOk() { return( installed ); };
  int isVersion() { return( is_version ); };
  const char *help();
  int getNumClients() { return( num_clients ); };
  char *getProtocolLibraryPath() { return( protocol_lib_path ); };
  char *getClientLibraryPath() { return( client_lib_path ); };
  char *getPayloadLibraryPath() { return( payload_lib_path ); };
  int getClientMinimum() { return( client_min ); };
  int getClientMaximum() { return( client_max ); };
  int getClientIncrement() { return( client_inc ); };
  int getClientTime() { return( client_time ); };
  int getPayloadMinimum() { return( payload_min ); };
  int getPayloadMaximum() { return( payload_max ); };
  int getPayloadIncrement() { return( payload_inc ); };
  int getPayloadTime() { return( payload_time ); };
  int getProtocolDestinationPort() { return( protocol_dest_port ); };
  char *getProtocolDestinationAddress() { return( protocol_dest_address ); };
  int getProtocolSourcePort() { return( 0 ); };
  char *getProtocolSourceAddress() { return( "0.0.0.0" ); };
  char *getErrorString() { return( error_string ); };
 protected:
  int num_clients;
  char *protocol_lib_path;
  char *client_lib_path;
  char *payload_lib_path;
  int client_min, client_max, client_inc, client_time;
  int payload_min, payload_max, payload_inc, payload_time;
  int protocol_dest_port;
  char *protocol_dest_address;
  char *error_string;
  int is_version;

  void set_error( char *str );
  int parse_client_data( int cnt, int argc, char **argv, int *cn );
  int parse_protocol_data( int cnt, int argc, char **argv, int *dn );
  int parse_payload_data( int cnt, int argc, char **argv, int *pn );
 private:
  int installed;
};

#endif
