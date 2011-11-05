#ifndef __PAYLOAD_H__
#define __PAYLOAD_H__

/*
  Payload structure. The minimum size for a payload is 8 bytes. The rest of the payload is undefined.

  struct payload_header
  {
    char head[4]="TRAF";
    unsigned int size;
  };

  size is in network byte order and is an 32-bit unsigned integer. This provides the total payload size including the header.
 */

/*
  This initializes the payload. Returns void * to be used as handle.

  min - Minimum size
  max - Maximum size
  increment - If there is an increment base it on this
  time - Time factor for calculations
  other - Implementation specific pointer - please do not use
 */
extern "C" void *InitializePayload( unsigned min, unsigned max, unsigned increment, unsigned time, void *other );

/*
  This destroys the payload
 */
extern "C" void DestroyPayload( void *handle );

/*
  Get payload to be sent

  Returns a "char *" buffer as payload, already filled with junk.
  Returns total size in the "size" pointer.

  Must be freed by calling ReleasePayload.
 */
extern "C" char *GetPayload( void *handle, unsigned *size );

/*
  This releases the payload as created by GetPayload.
 */
extern "C" void ReleasePayload( void *handle, char *payload );

/*
  This resets the payload sequence so that it starts from scratch
 */
extern "C" void ResetPayloadSequence( void *handle );

#endif
