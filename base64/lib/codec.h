#include <stdint.h>
#include <stddef.h>

#include "../include/libbase64.h"

void
base64_stream_encode_plain
	( struct base64_state	*state
	, const char		*src
	, size_t		 srclen
	, char			*out
	, size_t		*outlen
	);

int
base64_stream_decode_plain
	( struct base64_state	*state
	, const char		*src
	, size_t		 srclen
	, char			*out
	, size_t		*outlen
	);
