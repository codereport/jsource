/* Copyright 1990-2006, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Xenos: SHA calculation                                                  */

#ifdef defined(__MACH__)
#include <machine/endian.h>
#else
#include <sys/types.h>
#endif

#include "j.h"
#include "x.h"
#undef num
#include "cpuinfo.h"
// static UC hwsha1=0,hwsha2=0,hwssse3=0,hwsse41=0,hwavx=0,hwavx2=0;

#ifndef BYTE_ORDER
#if defined(MMSC_VER)
#define LITTLE_ENDIAN 1234
#define BIG_ENDIAN    4321
#define BYTE_ORDER LITTLE_ENDIAN
#endif
#endif

static const char *hex_digits = "0123456789abcdef";

/* not null terminated */
static UC* tohex(UC* dest, UC* src, I len)
{
  I i;
  UC* d = src;
  UC* buffer = dest;
  for (i = 0; i < len; i++) {
    *buffer++ = hex_digits[(*d & 0xf0) >> 4];
    *buffer++ = hex_digits[*d & 0x0f];
    d++;
  }
  R dest;
}
