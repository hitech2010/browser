#if defined(_M_IX86)
#include <openssl/opensslconf_win32.h>
#elif defined(_M_X64)
#include <openssl/opensslconf_win64.h>
#elif defined(__APPLE__) && defined(__LP64__)
#include <openssl/opensslconf_mac64.h>
#elif defined(__APPLE__) && !defined(__LP64__)
#include <openssl/opensslconf_mac32.h>
#else
#error "Unknown environment"
#include <openssl/opensslconf_.h>
#endif
