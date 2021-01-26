#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "../../../include/libbase64.h"
#include "../../tables/tables.h"
#include "../../codecs.h"
#include "../../env.h"

#include "64/enc_loop.c"

BASE64_ENC_FUNCTION(plain)
{
	#include "enc_head.c"
	enc_loop_generic_64(&s, &slen, &o, &olen);
	#include "enc_tail.c"
}

BASE64_DEC_FUNCTION(plain)
{
	#include "dec_head.c"
	#include "dec_tail.c"
}
