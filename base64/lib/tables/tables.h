#ifndef BASE64_TABLES_H
#define BASE64_TABLES_H

#include <stdint.h>

// These tables are used by all codecs for fallback plain encoding/decoding:
extern const uint8_t base64_table_enc_6bit[];
extern const uint8_t base64_table_dec_8bit[];

// This table is used by the 32 and 64-bit generic encoders:
extern const uint16_t base64_table_enc_12bit[];

#endif	// BASE64_TABLES_H
