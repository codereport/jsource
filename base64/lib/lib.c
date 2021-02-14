#include <stdint.h>
#include <stddef.h>

#include "../include/libbase64.h"
#include "tables/tables.h"
#include "codec.h"

/* Call this before calling base64_stream_encode() to init the state. See above
 */
static void
base64_stream_encode_init (struct base64_state *state)
{
	state->eof = 0;
	state->bytes = 0;
	state->carry = 0;
}

/* Encodes the block of data of given length at `src`, into the buffer at
 * `out`. Caller is responsible for allocating a large enough out-buffer; it
 * must be at least 4/3 the size of the in-buffer, but take some margin. Places
 * the number of new bytes written into `outlen` (which is set to zero when the
 * function starts). Does not zero-terminate or finalize the output. */
static void
base64_stream_encode
	( struct base64_state	*state
	, const char		*src
	, size_t		 srclen
	, char			*out
	, size_t		*outlen
	)
{
	base64_stream_encode_plain(state, src, srclen, out, outlen);
}

/* Finalizes the output begun by previous calls to `base64_stream_encode()`.
 * Adds the required end-of-stream markers if appropriate. `outlen` is modified
 * and will contain the number of new bytes written at `out` (which will quite
 * often be zero). */
static void
base64_stream_encode_final
	( struct base64_state	*state
	, char			*out
	, size_t		*outlen
	)
{
	uint8_t *o = (uint8_t *)out;

	if (state->bytes == 1) {
		*o++ = base64_table_enc_6bit[state->carry];
		*o++ = '=';
		*o++ = '=';
		*outlen = 3;
		return;
	}
	if (state->bytes == 2) {
		*o++ = base64_table_enc_6bit[state->carry];
		*o++ = '=';
		*outlen = 2;
		return;
	}
	*outlen = 0;
}

/* Call this before calling base64_stream_decode() to init the state. See above
 */
static void
base64_stream_decode_init (struct base64_state *state)
{
	state->eof = 0;
	state->bytes = 0;
	state->carry = 0;
}

/* Decodes the block of data of given length at `src`, into the buffer at
 * `out`. Caller is responsible for allocating a large enough out-buffer; it
 * must be at least 3/4 the size of the in-buffer, but take some margin. Places
 * the number of new bytes written into `outlen` (which is set to zero when the
 * function starts). Does not zero-terminate the output. Returns 1 if all is
 * well, and 0 if a decoding error was found, such as an invalid character.
 * Returns -1 if the chosen codec is not included in the current build. Used by
 * the test harness to check whether a codec is available for testing. */
static int
base64_stream_decode
	( struct base64_state	*state
	, const char		*src
	, size_t		 srclen
	, char			*out
	, size_t		*outlen
	)
{
	return base64_stream_decode_plain(state, src, srclen, out, outlen);
}

void
base64_encode
	( const char	*src
	, size_t	 srclen
	, char		*out
	, size_t	*outlen
	)
{
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
base64_decode
	( const char	*src
	, size_t	 srclen
	, char		*out
	, size_t	*outlen
	)
{
	int ret;
	struct base64_state state;

	// Init the stream reader:
	base64_stream_decode_init(&state);

	// Feed the whole string to the stream reader:
	ret = base64_stream_decode(&state, src, srclen, out, outlen);

	// If when decoding a whole block, we're still waiting for input then fail:
	if (ret && (state.bytes == 0)) {
		return ret;
	}
	return 0;
}
