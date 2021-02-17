#ifndef LIBBASE64_H
#define LIBBASE64_H

#include <stddef.h> /* size_t */

#ifdef __cplusplus
extern "C" {
#endif

struct base64_state {
    int bytes;
    unsigned char carry;
};

/* Wrapper function to encode a plain string of given length. Output is written
 * to *out without trailing zero. Output length in bytes is written to *outlen.
 * The buffer in `out` has been allocated by the caller and is at least 4/3 the
 * size of the input. */
void base64_encode(const char *src, size_t srclen, char *out, size_t *outlen);

/* Wrapper function to decode a plain string of given length. Output is written
 * to *out without trailing zero. Output length in bytes is written to *outlen.
 * The buffer in `out` has been allocated by the caller and is at least 3/4 the
 * size of the input. */
int base64_decode(const char *src, size_t srclen, char *out, size_t *outlen);

#ifdef __cplusplus
}
#endif

#endif /* LIBBASE64_H */
