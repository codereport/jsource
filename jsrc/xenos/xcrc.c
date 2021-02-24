/* Copyright 1990-2006, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Xenos: CRC calculation and base64 encode/decode                         */

#include "j.h"
#include "x.h"

#include "../base64/include/libbase64.h"

// Calculate byte-at-a-time CRC table in *crctab, and return the starting value as the result
static UINT
jtcrcvalidate(J jt, A w, UINT *crctab) {
    A *wv;
    B *v;
    I m;
    UINT p, x, z = -1;
    ASSERT(1 >= AR(w), EVRANK);
    m = AN(w);
    if (m && BOX & AT(w)) {
        ASSERT(2 >= m, EVLENGTH);
        wv = AAV(w);
        w  = wv[0];
        if (2 == m) RE(z = (UINT)jti0(jt, wv[1]));
    }
    if (B01 & AT(w)) {
        ASSERT(32 == AN(w), EVLENGTH);
        v = BAV(w);
        p = 0;
        DQ(32, p <<= 1; p |= *v++;);
    } else
        RE(p = (UINT)jti0(jt, w));
    DO(256, x = (UINT)i; DO(8, x = (x >> 1) ^ (p & ((UINT) - (I4)(x & 1)));); crctab[i] = x;);
    return z;
}

A
jtcrc1(J jt, A w) {
    return jtcrc2(jt, jtsc(jt, -306674912), w);
}

A
jtcrc2(J jt, A a, A w) {
    I n;
    UINT z;
    UC *v;
    UINT crctab[256];
    ASSERT(1 >= AR(a) && 1 >= AR(w), EVRANK);
    n = AN(w);
    v = UAV(w);
    // ASSERT(!n||AT(w)&LIT+C2T+C4T,EVDOMAIN);
    ASSERT(!n || AT(w) & LIT, EVDOMAIN);
    RE(z = jtcrcvalidate(jt, a, crctab));
    n = AT(w) & C4T ? (4 * n) : AT(w) & C2T ? n + n : n;
    DQ(n, z = z >> 8 ^ crctab[255 & (z ^ *v++)];);  // do the computation using unsigned z
    return jtsc(jt, (I)(I4)(z ^ -1L));  // sign-extend result if needed to make 64-bit and 32-bit the same numeric value
}

A
jtcrccompile(J jt, A w) {
    A h, *hv;
    UINT z;
    UINT crctab[256];
    GAT0(h, BOX, 2, 1);
    hv = AAV(h);
    RE(z = jtcrcvalidate(jt, w, crctab));
    RZ(hv[0] =
         jtrifvs(jt, jtvec(jt, LIT, sizeof(crctab), crctab)));  // Save the table.  We don't have any other good type to use
    RZ(hv[1] = jtrifvs(jt, jtsc(jt, (I)z)));
    return h;
}

A
jtcrcfixedleft(J jt, A w, A self) {
    A h, *hv;
    I n;
    UINT *t, z;
    UC *v;
    h  = FAV(self)->fgh[2];
    hv = AAV(h);
    t  = (UINT *)AV(hv[0]);
    z  = (UINT)AV(hv[1])[0];
    n  = AN(w);
    v  = UAV(w);
    ASSERT(!n || AT(w) & LIT + C2T + C4T, EVDOMAIN);
    n = AT(w) & C4T ? (4 * n) : AT(w) & C2T ? n + n : n;
    DQ(n, z = z >> 8 ^ t[255 & (z ^ *v++)];);
    return jtsc(jt, (I)(I4)(z ^ -1L));
}

// CRC-based hash.  Bivalent
#ifndef CRC32L
#define CRC32L(acc, in) (0xffffffff & ((acc * 15015) ^ (in)))  // if no hardware CRC (rare), mix the bits a little
#endif
A
jtqhash12(J jt, A a, A w) {
    FPREFIP;
    I hsiz;
    UI crc;
    if (AT(w) & NOUN) {
        RE(hsiz = jti0(jt, jtvib(jt, a)));
    } else {
        w    = a;
        hsiz = 0;
    }  // fetch hashtable size; set w=data to hash
    ASSERT(hsiz >= 0, EVDOMAIN);
    ASSERT(AT(w) & DENSE, EVNONCE);  // not sparse for now
    if (AT(w) & DIRECT) {            // Direct value, calculate CRC of atoms
        crc = hic(AN(w) << bplg(AT(w)),
                  UCAV(w));  // sign-extend result if needed to make 64-bit and 32-bit the same numeric value
    } else {                 // not DIRECT, calculate CRC of component CRCs
        crc    = -1;         // where we accumulate CRC
        I lpct = AN(w) << ((AT(w) >> RATX) & 1);                                    // number of component values
        A *av  = AAV(w);                                                            // pointer to subvalues
        DQ(lpct, crc = CRC32L(crc, jti0(jt, jtqhash12(jt, zeroionei(0), *av++)));)  // recur
    }
    if (hsiz) crc = (crc * (UI)hsiz) >> 32;  // convert hash to user's range
    return jtsc(jt, (I)(I4)crc);             // make the result a valid integer.  Could reuse the a arg inplace
}

// base64 stuff

A
jttobase64(J jt, A w) {
    F1RANK(1, jttobase64, UNUSED_VALUE);  // here we deal only with rank<= 1
    ASSERT(AT(w) & LIT,
           EVDOMAIN);  // we work only on ASCII (which includes UTF-8).  Other types must be converted first
    // Calculate # triples, #extras
    I n3 = AN(w) / 3;
    I ne = AN(w) % 3;
    I zn = n3 + SGNTO0(-ne);  // total # result 4-byte groups
    // Allocate result
    A z;
    GATV0(z, LIT, zn << 2, 1);
    UI4 *zv     = UI4AV(z);  // result block, pointer into it
    C *wv       = CAV(w);    // input pointer
    size_t zlen = AN(z);
    base64_encode(wv, AN(w), CAV(z), &zlen);
    ASSERT((I)zlen == AN(z), EVDOMAIN);  // make sure length agreed
    return z;
}

A
jtfrombase64(J jt, A w) {
    F1RANK(1, jtfrombase64, UNUSED_VALUE);  // here we deal only with rank<= 1
    ASSERT(AT(w) & LIT, EVDOMAIN);          // we work only on ASCII
    // Calculate the number of result bytes.  We take the number of input bytes, and discard trailing '=' on a 2- or
    // 3-boundary
    I wn = AN(w);
    if ((wn & 3) == 0 && wn && CAV(w)[wn - 1] == '=') --wn;  // discard trailing = at boundary
    if ((wn & 3) == 3 && CAV(w)[wn - 1] == '=') --wn;        // discard trailing = at boundary
    ASSERT((wn & 3) != 1, EVDOMAIN);                         // cannot have remnant of 1 char
    // Allocate result
    A z;
    GATV0(z, LIT, (wn >> 2) * 3 + (((wn & 3) + 1) >> 1), 1);  // 3 bytes per full set, plus 0, 1, or 2
    // process the input in full 4-byte groups.  We may overread the input AND overwrite the result, but we will always
    // stay in the padding area, which is OK because we allocated the result here
    size_t zlen = AN(z);
    int rc      = base64_decode(CAV(w), AN(w), CAV(z), &zlen);
    ASSERT(rc == 1 && (I)zlen == AN(z), EVDOMAIN);  // make sure no invalid input bytes
    return z;
}
// the 3 bytes are characters AAAAABB BBBBCCCC CCDDDDDD in bits
//                            7     0 15     8 23    16
