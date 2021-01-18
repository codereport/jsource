#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdint.h>

#include "../include/libbase64.h"
#include "codecs.h"
#include "env.h"

// Function declarations:
#define BASE64_CODEC_FUNCS(arch)	\
	BASE64_ENC_FUNCTION(arch);	\
	BASE64_DEC_FUNCTION(arch);	\

BASE64_CODEC_FUNCS(avx2)
// BASE64_CODEC_FUNCS(neon32)
BASE64_CODEC_FUNCS(neon64)
BASE64_CODEC_FUNCS(plain)
BASE64_CODEC_FUNCS(ssse3)
BASE64_CODEC_FUNCS(sse41)
BASE64_CODEC_FUNCS(sse42)
BASE64_CODEC_FUNCS(avx)

void
codec_choose (struct codec *codec, int flags)
{
	// TODO assert BASE64_FORCE_PLAIN
	if (flags & BASE64_FORCE_PLAIN) {
		codec->enc = base64_stream_encode_plain;
		codec->dec = base64_stream_decode_plain;
		return;
	}
	return;
}

