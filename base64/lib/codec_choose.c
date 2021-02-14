#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdint.h>

#include "../include/libbase64.h"
#include "codecs.h"
#include "env.h"

// Function declarations:
BASE64_ENC_FUNCTION(plain);
BASE64_DEC_FUNCTION(plain);

void
codec_choose (struct codec *codec)
{
	codec->enc = base64_stream_encode_plain;
	codec->dec = base64_stream_decode_plain;
}

