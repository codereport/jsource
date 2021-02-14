#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "../../../include/libbase64.h"
#include "../../tables/tables.h"
#include "../../codecs.h"
#include "../../env.h"

#include "64/enc_loop.c"

void
base64_stream_encode_plain
	( struct base64_state	*state
	, const char		*src
	, size_t		 srclen
	, char			*out
	, size_t		*outlen
	)
{
	#include "enc_head.c"
	enc_loop_generic_64(&s, &slen, &o, &olen);
	#include "enc_tail.c"
}

int
base64_stream_decode_plain
	( struct base64_state	*state
	, const char		*src
	, size_t		 srclen
	, char			*out
	, size_t		*outlen
	)
{
	#include "dec_head.c"
	#include "dec_tail.c"
}
