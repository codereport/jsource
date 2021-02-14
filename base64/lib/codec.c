#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "../include/libbase64.h"
#include "tables/tables.h"
#include "codec.h"

static inline void
enc_loop_generic_64_inner (const uint8_t **s, uint8_t **o)
{
	uint64_t src;

	// Load input:
	memcpy(&src, *s, sizeof (src));

	// Reorder to 64-bit big-endian, if not already in that format. The
	// workset must be in big-endian, otherwise the shifted bits do not
	// carry over properly among adjacent bytes:
	src = __builtin_bswap64(src);

	// Four indices for the 12-bit lookup table:
	const size_t index0 = (src >> 52) & 0xFFFU;
	const size_t index1 = (src >> 40) & 0xFFFU;
	const size_t index2 = (src >> 28) & 0xFFFU;
	const size_t index3 = (src >> 16) & 0xFFFU;

	// Table lookup and store:
	memcpy(*o + 0, base64_table_enc_12bit + index0, 2);
	memcpy(*o + 2, base64_table_enc_12bit + index1, 2);
	memcpy(*o + 4, base64_table_enc_12bit + index2, 2);
	memcpy(*o + 6, base64_table_enc_12bit + index3, 2);

	*s += 6;
	*o += 8;
}

static inline void
enc_loop_generic_64 (const uint8_t **s, size_t *slen, uint8_t **o, size_t *olen)
{
	if (*slen < 8) {
		return;
	}

	// Process blocks of 6 bytes at a time. Because blocks are loaded 8
	// bytes at a time, ensure that there will be at least 2 remaining
	// bytes after the last round, so that the final read will not pass
	// beyond the bounds of the input buffer:
	size_t rounds = (*slen - 2) / 6;

	*slen -= rounds * 6;	// 6 bytes consumed per round
	*olen += rounds * 8;	// 8 bytes produced per round

	do {
		if (rounds >= 8) {
			enc_loop_generic_64_inner(s, o);
			enc_loop_generic_64_inner(s, o);
			enc_loop_generic_64_inner(s, o);
			enc_loop_generic_64_inner(s, o);
			enc_loop_generic_64_inner(s, o);
			enc_loop_generic_64_inner(s, o);
			enc_loop_generic_64_inner(s, o);
			enc_loop_generic_64_inner(s, o);
			rounds -= 8;
			continue;
		}
		if (rounds >= 4) {
			enc_loop_generic_64_inner(s, o);
			enc_loop_generic_64_inner(s, o);
			enc_loop_generic_64_inner(s, o);
			enc_loop_generic_64_inner(s, o);
			rounds -= 4;
			continue;
		}
		if (rounds >= 2) {
			enc_loop_generic_64_inner(s, o);
			enc_loop_generic_64_inner(s, o);
			rounds -= 2;
			continue;
		}
		enc_loop_generic_64_inner(s, o);
		break;

	} while (rounds > 0);
}

void
base64_stream_encode
	( struct base64_state	*state
	, const char		*src
	, size_t		 srclen
	, char			*out
	, size_t		*outlen
	)
{
	// Assume that *out is large enough to contain the output.
	// Theoretically it should be 4/3 the length of src.
	const uint8_t *s = (const uint8_t *) src;
	uint8_t *o = (uint8_t *) out;

	// Use local temporaries to avoid cache thrashing:
	size_t olen = 0;
	size_t slen = srclen;
	struct base64_state st;
	st.bytes = state->bytes;
	st.carry = state->carry;

	// Turn three bytes into four 6-bit numbers:
	// in[0] = 00111111
	// in[1] = 00112222
	// in[2] = 00222233
	// in[3] = 00333333

	// Duff's device, a for() loop inside a switch() statement. Legal!
	switch (st.bytes)
	{
		for (;;)
		{
		case 0:
			enc_loop_generic_64(&s, &slen, &o, &olen);
			if (slen-- == 0) {
				break;
			}
			*o++ = base64_table_enc_6bit[*s >> 2];
			st.carry = (*s++ << 4) & 0x30;
			st.bytes++;
			olen += 1;

			// Deliberate fallthrough:

		case 1:	if (slen-- == 0) {
				break;
			}
			*o++ = base64_table_enc_6bit[st.carry | (*s >> 4)];
			st.carry = (*s++ << 2) & 0x3C;
			st.bytes++;
			olen += 1;

			// Deliberate fallthrough:

		case 2:	if (slen-- == 0) {
				break;
			}
			*o++ = base64_table_enc_6bit[st.carry | (*s >> 6)];
			*o++ = base64_table_enc_6bit[*s++ & 0x3F];
			st.bytes = 0;
			olen += 2;
		}
	}
	state->bytes = st.bytes;
	state->carry = st.carry;
	*outlen = olen;
}

int
base64_stream_decode
	( struct base64_state	*state
	, const char		*src
	, size_t		 srclen
	, char			*out
	, size_t		*outlen
	)
{
	int ret = 0;
	const uint8_t *s = (const uint8_t *) src;
	uint8_t *o = (uint8_t *) out;
	uint8_t q;

	// Use local temporaries to avoid cache thrashing:
	size_t olen = 0;
	size_t slen = srclen;
	struct base64_state st;
	st.bytes = state->bytes;
	st.carry = state->carry;

	// Turn four 6-bit numbers into three bytes:
	// out[0] = 11111122
	// out[1] = 22223333
	// out[2] = 33444444

	// Duff's device again:
	switch (st.bytes)
	{
		for (;;)
		{
		case 0:
			if (slen-- == 0) {
				ret = 1;
				break;
			}
			if ((q = base64_table_dec_8bit[*s++]) >= 254) {
				// Treat character '=' as invalid for byte 0:
				break;
			}
			st.carry = q << 2;
			st.bytes++;

			// Deliberate fallthrough:

		case 1:	if (slen-- == 0) {
				ret = 1;
				break;
			}
			if ((q = base64_table_dec_8bit[*s++]) >= 254) {
				// Treat character '=' as invalid for byte 1:
				break;
			}
			*o++ = st.carry | (q >> 4);
			st.carry = q << 4;
			st.bytes++;
			olen++;

			// Deliberate fallthrough:

		case 2:	if (slen-- == 0) {
				ret = 1;
				break;
			}
			if ((q = base64_table_dec_8bit[*s++]) >= 254) {
				st.bytes++;
				// When q == 254, the input char is '='.
				// Check if next byte is also '=':
				if (q == 254) {
					if (slen-- != 0) {
						st.bytes = 0;
						// EOF:
						q = base64_table_dec_8bit[*s++];
						ret = ((q == 254) && (slen == 0)) ? 1 : 0;
						break;
					}
					else {
						// Almost EOF
						ret = 1;
						break;
					}
				}
				// If we get here, there was an error:
				break;
			}
			*o++ = st.carry | (q >> 2);
			st.carry = q << 6;
			st.bytes++;
			olen++;

			// Deliberate fallthrough:

		case 3:	if (slen-- == 0) {
				ret = 1;
				break;
			}
			if ((q = base64_table_dec_8bit[*s++]) >= 254) {
				st.bytes = 0;
				// When q == 254, the input char is '='. Return 1 and EOF.
				// When q == 255, the input char is invalid. Return 0 and EOF.
				ret = ((q == 254) && (slen == 0)) ? 1 : 0;
				break;
			}
			*o++ = st.carry | q;
			st.carry = 0;
			st.bytes = 0;
			olen++;
		}
	}

	state->bytes = st.bytes;
	state->carry = st.carry;
	*outlen = olen;
	return ret;
}
