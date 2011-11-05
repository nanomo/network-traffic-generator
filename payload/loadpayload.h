#ifndef __LOAD_PAYLOAD_H__
#define __LOAD_PAYLOAD_H__

class payload
{
 public:
  payload( char *pathname );
  virtual ~payload();
  int isOk() { return( installed ); };
  void *InitializePayload( unsigned min, unsigned max, unsigned increment, unsigned time, void *other );
  void DestroyPayload( void *handle );
  char *GetPayload( void *handle, unsigned *size );
  void ReleasePayload( void *handle, char *payload );
  void ResetPayloadSequence( void *handle );
 protected:
  void *libhandle;
  void * EXPORT (*MyInitializePayload)( unsigned min, unsigned max, unsigned increment, unsigned time, void *other );
  void  EXPORT (*MyDestroyPayload)( void *handle );
  char * EXPORT (*MyGetPayload)( void *handle, unsigned *size );
  void  EXPORT (*MyReleasePayload)( void *handle, char *payload );
  void  EXPORT (*MyResetPayloadSequence)( void *handle );
 private:
  int installed;
};

/*
 structure to contain an initialized payload with it's handle
 */
struct client_payload
{
  payload *pld;
  void *handle;
};


#endif
