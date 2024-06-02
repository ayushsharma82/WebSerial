#ifndef wslp_h
#define wslp_h

#include <Arduino.h>

#ifndef WEBSERIAL_USE_BROTLI_COMPRESSION
    #define WEBSERIAL_USE_BROTLI_COMPRESSION 0
#endif

#if WEBSERIAL_USE_BROTLI_COMPRESSION == 1
  extern const uint8_t WEBSERIAL_HTML[18909];
#else
  extern const uint8_t WEBSERIAL_HTML[20891];
#endif

#endif
