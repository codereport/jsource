#ifndef BASE64_ENV_H
#define BASE64_ENV_H

// This header file contains macro definitions that describe certain aspects of
// the compile-time environment. Compatibility and portability macros go here.

// Define machine endianness. This is for GCC:
#define BASE64_LITTLE_ENDIAN 1
#define BASE64_LITTLE_ENDIAN 1

// Endian conversion functions:
//   GCC and Clang:
#define BASE64_HTOBE32(x)	__builtin_bswap32(x)
#define BASE64_HTOBE64(x)	__builtin_bswap64(x)

// End-of-file definitions.
// Almost end-of-file when waiting for the last '=' character:
#define BASE64_AEOF 1
// End-of-file when stream end has been reached or invalid input provided:
#define BASE64_EOF 2

#endif	// BASE64_ENV_H
