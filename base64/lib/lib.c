#include <stdint.h>
#include <stddef.h>

#include "../include/libbase64.h"
#include "tables/tables.h"
#include "codec.h"

/* Call this before calling base64_stream_encode() to init the state. See above
 */
static void
base64_stream_encode_init(struct base64_state *state) {
    state->bytes = 0;
    state->carry = 0;
}

/* Finalizes the output begun by previous calls to `base64_stream_encode()`.
 * Adds the required end-of-stream markers if appropriate. `outlen` is modified
 * and will contain the number of new bytes written at `out` (which will quite
 * often be zero). */
static void
base64_stream_encode_final(struct base64_state *state, char *out, size_t *outlen) {
    uint8_t *o = (uint8_t *)out;

    if (state->bytes == 1) {
        *o++    = base64_table_enc_6bit[state->carry];
        *o++    = '=';
        *o++    = '=';
        *outlen = 3;
        return;
    }
    if (state->bytes == 2) {
        *o++    = base64_table_enc_6bit[state->carry];
        *o++    = '=';
        *outlen = 2;
        return;
    }
    *outlen = 0;
}

/* Call this before calling base64_stream_decode() to init the state. See above
 */
static void
base64_stream_decode_init(struct base64_state *state) {
    state->bytes = 0;
    state->carry = 0;
}

void
base64_encode(const char *src, size_t srclen, char *out, size_t *outlen) {
    size_t s;
    size_t t;
    struct base64_state state;

    // Init the stream reader:
    base64_stream_encode_init(&state);

    // Feed the whole string to the stream reader:
    base64_stream_encode(&state, src, srclen, out, &s);

    // Finalize the stream by writing trailer if any:
    base64_stream_encode_final(&state, out + s, &t);

    // Final output length is stream length plus tail:
    *outlen = s + t;
}

int
base64_decode(const char *src, size_t srclen, char *out, size_t *outlen) {
    int ret;
    struct base64_state state;

    // Init the stream reader:
    base64_stream_decode_init(&state);

    // Feed the whole string to the stream reader:
    ret = base64_stream_decode(&state, src, srclen, out, outlen);

    // If when decoding a whole block, we're still waiting for input then fail:
    if (ret && (state.bytes == 0)) { return ret; }
    return 0;
}
