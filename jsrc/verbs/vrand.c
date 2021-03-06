/* Copyright 1990-2011, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Verbs: Random Numbers                                                   */

#include "j.h"

#define NRNG 5 /* # of available RNGs (excluding 0)             */
#define SMI 0  /* sum of all RNGs                               */
#define GBI 1  /* gb_flip, Knuth 1994                           */
#define MTI 2  /* Mersenne Twister, Matsumoto & Nishimura, 2002 */
#define DXI 3  /* DX-1597-4d, Deng, 2005                        */
#define MRI 4  /* MRG32k3a, L'Ecuyer, 1999                      */

#define x31 ((UI)0x80000000)
#define x63 ((UI)0x8000000000000000)
#define X52 2.22044604925031308085e-16
#define X64 5.42101086242752217004e-20

#define NEXT (jt->rngf(jt))

#define INITD \
    { sh = mk = 1; }
#define NEXTD1 ((0.5 + X52 / 2) + X64 * (I)(NEXT & (UI)0xfffffffffffff000))
#define NEXTD0 NEXTD1

/* m<x, greatest multiple of m less than x */
#define GMOF(m, x) (((UI)0xffffffffffffffffLL / (m)) * (m))
#define GMOF2(m, x, z, zq) (zq = (UI)0xffffffffffffffffLL / (m), z = zq * (m))
#define GMOTHRESH (UI)0xf000000000000000

/* ----------------------------------------------------------------------- */
/* linear congruential generator                                           */
/* Lehmer, D.H., Proceedings, 2nd Symposium on Large-Scale Digital         */
/* Calculating Machinery, Harvard University Press, 1951, pp. 141-146.     */

static void
lcg(I n, I* v, I seed) {
    D c = 16807.0, p = 2147483647.0, x = (D)seed;
    DQ(n, x *= c; x -= p * (I)(x / p); *v++ = (I)x;);
}

/* ----------------------------------------------------------------------- */
/* gb_flip routines from D.E. Knuth's "The Stanford GraphBase"             */

#define GBN 56

#define mod_diff(x, y) (((x) - (y)) & 0x7fffffffL) /* difference modulo 2^31 */

static UI
jtgb_flip_cycle(J jt) {
    I* A = (I*)jt->rngv;
    register I *i, *j;
    for (i = &A[1], j = &A[32]; j <= &A[55]; i++, j++) *i = mod_diff(*i, *j);
    for (j = &A[1]; i <= &A[55]; i++, j++) *i = mod_diff(*i, *j);
    jt->rngi = 54;
    return (UI)A[55];
}

static UI
jtgb_next(J jt) {
    UI a, b, c;
    a = jt->rngi ? jt->rngv[jt->rngi--] : jtgb_flip_cycle(jt);
    b = jt->rngi ? jt->rngv[jt->rngi--] : jtgb_flip_cycle(jt);
    c = jt->rngi ? jt->rngv[jt->rngi--] : jtgb_flip_cycle(jt);
    return a + (b << 31) + (c << 33 & 0xc000000000000000UL);
}

static void
jtgb_init(J jt, UI s) {
    I* A;
    register I i, next = 1, prev, seed;
    A    = (I*)jt->rngv;
    next = 1;
    prev = seed = (I)s;
    seed = prev = mod_diff(prev, 0); /* strip off the sign */
    A[0]        = 0;
    A[55]       = prev;
    for (i = 21; i; i = (i + 21) % 55) {
        A[i] = next;
        next = mod_diff(prev, next);
        if (seed & 1)
            seed = 0x40000000 + (seed >> 1);
        else
            seed >>= 1; /* cyclic shift right 1 */
        next = mod_diff(next, seed);
        prev = A[i];
    }
    jtgb_flip_cycle(jt);
    jtgb_flip_cycle(jt);
    jtgb_flip_cycle(jt);
    jtgb_flip_cycle(jt);
    jtgb_flip_cycle(jt);
    jt->rngi = 54;
}

/* ----------------------------------------------------------------------- */
/*
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote
        products derived from this software without specific prior written
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

/* Period parameters */

#define MTN 312
#define MTM 156
#define MATRIX_A 0xB5026F5AA96619E9ULL
#define UM 0xFFFFFFFF80000000ULL /* Most significant 33 bits */
#define LM 0x7fffffffUL          /* least significant r bits  */

/* initializes mt[MTN] with a seed */
static void
jtmt_init(J jt, UI s) {
    I i;
    UI* mt = jt->rngv;
    mt[0]  = s;
    for (i = 1; i < MTN; i++) mt[i] = (6364136223846793005ULL * (mt[i - 1] ^ (mt[i - 1] >> 62)) + i);
    /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
    /* In the previous versions, MSBs of the seed affect   */
    /* only MSBs of the array mt[].                        */
    /* 2002/01/09 modified by Makoto Matsumoto             */
    jt->rngi = MTN;
}

static void
jtmt_init_by_array(J jt, UI init_key[], I key_length) {
    I i, j, k;
    UI* mt = jt->rngv;
    jtmt_init(jt, (UI)19650218);
    i = 1;
    j = 0;
    k = MTN > key_length ? MTN : key_length;
    for (; k; k--) {
        mt[i] = (mt[i] ^ (mt[i - 1] ^ mt[i - 1] >> 62) * 3935559000370003845ULL) + init_key[j] + j; /* non linear */
        i++;
        j++;
        if (i >= MTN) {
            mt[0] = mt[MTN - 1];
            i     = 1;
        }
        if (j >= key_length) j = 0;
    }
    for (k = MTN - 1; k; k--) {
        mt[i] = (mt[i] ^ (mt[i - 1] ^ mt[i - 1] >> 62) * 2862933555777941757ULL) - i; /* non linear */
        i++;
        if (i >= MTN) {
            mt[0] = mt[MTN - 1];
            i     = 1;
        }
    }
    mt[0] = 1ULL << 63; /* MSB is 1; assuring non-zero initial array */
}

/* generates a random 32-or 64-bit number */
static UI
jtmt_next(J jt) {
    UI *mt = jt->rngv, *u, *v, *w, y;
    if (MTN <= jt->rngi) { /* generate MTN words at one time */
        v = 1 + mt;
        w = MTM + mt;
        u = mt;
        DQ(MTN - MTM, y = (*u & UM) | (*v++ & LM); *u++ = *w++ ^ (y >> 1) ^ MATRIX_A * (y & 0x1UL););
        w = mt;
        DQ(MTM - 1, y = (*u & UM) | (*v++ & LM); *u++ = *w++ ^ (y >> 1) ^ MATRIX_A * (y & 0x1UL););
        v        = mt;
        y        = (*u & UM) | (*v++ & LM);
        *u++     = *w++ ^ (y >> 1) ^ MATRIX_A * (y & 0x1UL);
        jt->rngi = 0;
    }
    y = mt[jt->rngi++];
    y ^= (y >> 29) & 0x5555555555555555ULL;
    y ^= (y << 17) & 0x71D67FFFEDA60000ULL;
    y ^= (y << 37) & 0xFFF7EEE000000000ULL;
    y ^= (y >> 43);
    return y;
}

/* ----------------------------------------------------------------------- */
/* DX-1597-4d                                                              */
/* Deng, L.Y., Efficient and Portable Multiple Recursive Generators of     */
/* Large Order, ACM Transactions on Modelling and Computer Simulations 15, */
/* 2005, pp. 1-13.                                                         */

#define DXM 2147483647UL
#define DXB 1073741362UL
#define DXN 1597

static UI
jtdx_next30(J jt) {
    I j;
    UI *u, *v, *vv, r, x;
    j = jt->rngi;
    v = vv = j + jt->rngv;
    u      = DXN + jt->rngv;
    r      = *v;
    v += 532;
    if (v >= u) v -= DXN;
    r += *v;
    v += 532;
    if (v >= u) v -= DXN;
    r += *v;
    v += 532;
    if (v >= u) v -= DXN;
    r += *v;
    *vv = x = (r * DXB) % DXM;
    ++j;
    jt->rngi = j == DXN ? 0 : j;
    return x;
}

static UI
jtdx_next(J jt) {
    UI a, b, c;
    a = jtdx_next30(jt) & 0x000000003fffffff;
    b = jtdx_next30(jt) & 0x000000003fffffff;
    ;
    c = jtdx_next30(jt) & 0x000000003fffffff;
    ;
    return a | b << 30 | c << 34 & 0xf000000000000000UL;
}

static void
jtdx_init(J jt, UI s) {
    lcg(DXN, jt->rngv, s);
    jt->rngi = 0;
}

/* ----------------------------------------------------------------------- */
/* MRG32k3a                                                                */
/* L'Ecuyer, P., Good parameters and implementation for combined multiple  */
/* recursive random number generators, Operations Research 47, 1999,       */
/* pp. 159-164.                                                            */

#define MRN 6
#define MRM0 4294967087UL /*   _209+2^32 */
#define MRM1 4294944443UL /* _22853+2^32 */

static UI
jtmr_next31(J jt) {
    I d, j, *v = jt->rngv, x, y;
    switch (j = jt->rngi) {
        case 0:
            x        = 1403580 * v[1] - 810728 * v[0];
            y        = 527612 * v[5] - 1370589 * v[3];
            jt->rngi = 1;
            break;
        case 1:
            x        = 1403580 * v[2] - 810728 * v[1];
            y        = 527612 * v[3] - 1370589 * v[4];
            jt->rngi = 2;
            break;
        case 2:
            x        = 1403580 * v[0] - 810728 * v[2];
            y        = 527612 * v[4] - 1370589 * v[5];
            jt->rngi = 0;
    }
    x %= MRM0;
    if (x < 0) x += MRM0;
    v[j] = x;
    y %= MRM1;
    if (y < 0) y += MRM1;
    v[j + 3] = y;
    d        = (x - y) % MRM0;
    if (d < 0) d += MRM0;
    return d;
}

static UI
jtmr_next(J jt) {
    UI a, b, c;
    a = jtmr_next31(jt) & 0x000000007fffffff;
    b = jtmr_next31(jt) & 0x000000007fffffff;
    c = jtmr_next31(jt) & 0x000000007fffffff;
    return a + (b << 31) + (c << 33 & 0xc000000000000000UL);
}

static void
jtmr_init(J jt, UI s) {
    D* v = (D*)jt->rngv;
    I t[MRN];
    lcg(MRN, t, s);
    DO(MRN, *v++ = (D)t[i];);
    jt->rngi = 0;
}

/* ----------------------------------------------------------------------- */
/* sum of all RNGs                                                         */

static UI
jtsm_next(J jt) {
    UI x;
    jt->rngi       = jt->rngI0[GBI];
    jt->rngv       = jt->rngV0[GBI];
    x              = jtgb_next(jt);
    jt->rngI0[GBI] = jt->rngi;
    jt->rngi       = jt->rngI0[MTI];
    jt->rngv       = jt->rngV0[MTI];
    x += jtmt_next(jt);
    jt->rngI0[MTI] = jt->rngi;
    jt->rngi       = jt->rngI0[DXI];
    jt->rngv       = jt->rngV0[DXI];
    x += jtdx_next(jt);
    jt->rngI0[DXI] = jt->rngi;
    jt->rngi       = jt->rngI0[MRI];
    jt->rngv       = jt->rngV0[MRI];
    x += jtmr_next(jt);
    jt->rngI0[MRI] = jt->rngi;
    return x;
}

static void
jtsm_init(J jt, UI s) {
    jt->rngv = jt->rngV0[GBI];
    jtgb_init(jt, s);
    jt->rngI0[GBI] = jt->rngi;
    jt->rngv       = jt->rngV0[MTI];
    jtmt_init(jt, s);
    jt->rngI0[MTI] = jt->rngi;
    jt->rngv       = jt->rngV0[DXI];
    jtdx_init(jt, s);
    jt->rngI0[DXI] = jt->rngi;
    jt->rngv       = jt->rngV0[MRI];
    jtmr_init(jt, s);
    jt->rngI0[MRI] = jt->rngi;
}

/* ----------------------------------------------------------------------- */

A
jtrngraw(J jt, A w) {
    A z;
    I n, *v;
    RE(n = jti0(jt, w));
    ASSERT(0 <= n, EVDOMAIN);
    GATV0(z, INT, n, 1);
    v = AV(z);
    DQ(n, *v++ = NEXT;);
    return z;
}

B
jtrnginit(J jt) {
    DO(NRNG, jt->rngV[i] = jt->rngV0[i] = 0;);
    jt->rngF[0]    = jtsm_next;
    jt->rngS[0]    = 16807;
    jt->rngF[1]    = jtgb_next;
    jt->rngS[1]    = 16807;
    jt->rngF[2]    = jtmt_next;
    jt->rngS[2]    = 16807;
    jt->rngF[3]    = jtdx_next;
    jt->rngS[3]    = 16807;
    jt->rngF[4]    = jtmr_next;
    jt->rngS[4]    = 16807;
    jt->rngM[0]    = 0;                     /*   %      2^32 */
    jt->rngM[1]    = 0;                     /*   %      2^31 */
    jt->rngM[2]    = 0;                     /*   %      2^32 */
    jt->rngM[3]    = 0; /*   %   _1+2^31 */ /* fudge; should be _1+2^31 */
    jt->rngM[4]    = 0;                     /*   % _209+2^32 */
    jt->rngI0[GBI] = 54;
    jtrngselects(jt, num(2));
    return 1;
}

A
jtrngselectq(J jt, A w) {
    ASSERTMTV(w);
    return jtsc(jt, jt->rng);
}

static B
jtrngga(J jt, I i, UI** vv) {
    if (vv[i]) {
        jt->rngv = vv[i];
        jt->rngi = jt->rngI[i];
    } else {
        A x;
        I n, t;
        void (*f)();
        switch (i) {
            case GBI:
                t = INT;
                n = GBN;
                f = jtgb_init;
                break;
            case MTI:
                t = INT;
                n = MTN;
                f = jtmt_init;
                break;
            case DXI:
                t = INT;
                n = DXN;
                f = jtdx_init;
                break;
            case MRI:
                t = FL;
                n = MRN;
                f = jtmr_init;
        }
        GA(x, t, n, 1, 0);
        ras(x);
        vv[i] = jt->rngv = AV(x);  // x will never be freed, but that's OK, it's inited only once
        f(jt, jt->rngS[i]);
        jt->rngI[i] = jt->rngi;
    }
    return 1;
}

A
jtrngselects(J jt, A w) {
    I i;
    UI** vv = jt->rngV;
    RE(i = jti0(jt, w));
    ASSERT(BETWEENO(i, 0, NRNG), EVDOMAIN);
    jt->rngI[jt->rng] = jt->rngi;
    switch (jt->rng = i) {
        case SMI:
            vv       = jt->rngV0;
            jt->rngw = 64;
            RZ(jtrngga(jt, GBI, vv));
            RZ(jtrngga(jt, MTI, vv));
            RZ(jtrngga(jt, DXI, vv));
            RZ(jtrngga(jt, MRI, vv));
            break;
        case GBI:
            RZ(jtrngga(jt, i, vv));
            jt->rngw = 64;
            break;
        case MTI:
            RZ(jtrngga(jt, i, vv));
            jt->rngw = 64;
            break;
        case DXI:
            RZ(jtrngga(jt, i, vv));
            jt->rngw = 64;
            break;
        case MRI: RZ(jtrngga(jt, i, vv)); jt->rngw = 64;
    }
    jt->rngf = jt->rngF[jt->rng];
    return mtv;
}

A
jtrngstateq(J jt, A w) {
    A x  = 0, z, *zv;
    D* u = 0;
    I n;
    UI* v;
    ASSERTMTV(w);
    switch (jt->rng) {
        case SMI:
            GAT0(z, BOX, 9, 1);
            zv = AAV(z);
            RZ(*zv++ = jfalse);
            RZ(*zv++ = jtincorp(jt, jtsc(jt, jt->rngI0[GBI])));
            RZ(*zv++ = jtincorp(jt, jtvec(jt, INT, GBN, jt->rngV0[GBI])));
            RZ(*zv++ = jtincorp(jt, jtsc(jt, jt->rngI0[MTI])));
            RZ(*zv++ = jtincorp(jt, jtvec(jt, INT, MTN, jt->rngV0[MTI])));
            RZ(*zv++ = jtincorp(jt, jtsc(jt, jt->rngI0[DXI])));
            RZ(*zv++ = jtincorp(jt, jtvec(jt, INT, DXN, jt->rngV0[DXI])));
            RZ(*zv++ = jtincorp(jt, jtsc(jt, jt->rngI0[MRI])));
            RZ(*zv++ = jtincorp(jt, jtvec(jt, INT, MRN, jt->rngV0[MRI])));
            return z;
        case GBI:
            n = GBN;
            v = jt->rngv;
            break;
        case MTI:
            n = MTN;
            v = jt->rngv;
            break;
        case DXI:
            n = DXN;
            v = jt->rngv;
            break;
        case MRI:
            n = MRN;
            v = jt->rngv;
            break;
    }
    GAT0(z, BOX, 3, 1);
    zv = AAV(z);
    RZ(*zv++ = jtincorp(jt, jtsc(jt, jt->rng)));
    RZ(*zv++ = jtincorp(jt, jtsc(jt, jt->rngi)));
    RZ(*zv++ = jtincorp(jt, jtvec(jt, INT, n, v)));
    return z;
}

static B
jtrngstates1(J jt, I j, I n, UI** vv, I i, I k, A x, B p) {
    D* u;
    UI* xv;
    RZ(x = jtvi(jt, x));
    xv = AV(x);
    ASSERT(1 == AR(x), EVRANK);
    ASSERT(n == AN(x), EVLENGTH);
    ASSERT(i <= k && k < n + (I)(j == MTI), EVINDEX);
    if (p) DO(n, ASSERT(x31 > xv[i], EVDOMAIN););
    ICPY(vv[j], xv, n);
    jt->rngi = k;
    return 1;
}

A
jtrngstates(J jt, A w) {
    A* wv;
    I k;
    UI** vv = jt->rngV;
    ASSERT(1 == AR(w), EVRANK);
    ASSERT(BOX & AT(w), EVDOMAIN);
    ASSERT(2 <= AN(w), EVLENGTH);
    wv = AAV(w);
    RZ(jtrngselects(jt, wv[0])); /* changes jt->rng */
    ASSERT(AN(w) == (jt->rng ? 3 : 9), EVLENGTH);
    switch (jt->rng) {
        case SMI:
            vv = jt->rngV0;
            RE(k = jti0(jt, wv[1]));
            RZ(jtrngstates1(jt, GBI, GBN, vv, 0, k, wv[2], 1));
            jt->rngI0[GBI] = k;  // We accept 0-55 even though we never produce 55 ourselves
            RE(k = jti0(jt, wv[3]));
            RZ(jtrngstates1(jt, MTI, MTN, vv, 0, k, wv[4], 0));
            jt->rngI0[MTI] = k;
            RE(k = jti0(jt, wv[5]));
            RZ(jtrngstates1(jt, DXI, DXN, vv, 0, k, wv[6], 1));
            jt->rngI0[DXI] = k;
            RE(k = jti0(jt, wv[7]));
            RZ(jtrngstates1(jt, MRI, MRN, vv, 0, k, wv[8], 0));
            jt->rngI0[MRI] = k;
            break;
        case GBI:
            RE(k = jti0(jt, wv[1]));
            RZ(jtrngstates1(jt, GBI, GBN, vv, 0, k, wv[2], 1));
            break;  // We accept 0-55 even though we never produce 55 ourselves
        case MTI:
            RE(k = jti0(jt, wv[1]));
            RZ(jtrngstates1(jt, MTI, MTN, vv, 0, k, wv[2], 0));
            break;
        case DXI:
            RE(k = jti0(jt, wv[1]));
            RZ(jtrngstates1(jt, DXI, DXN, vv, 0, k, wv[2], 1));
            break;
        case MRI: RE(k = jti0(jt, wv[1])); RZ(jtrngstates1(jt, MRI, MRN, vv, 0, k, wv[2], 0));
    }
    return mtv;
}

// Return the seed info.  This is a scalar jt->rngS[jt->rng] unless the generator is Mersenne Twister and
// jt->rngseed is set, in which case jt->rngseed is the vector of seed info
A
jtrngseedq(J jt, A w) {
    ASSERTMTV(w);
    return jt->rngseed && MTI == jt->rng ? jt->rngseed : jtsc(jt, jt->rngS[jt->rng]);
}

// Set the vector of RNG seed info
A
jtrngseeds(J jt, A w) {
    I k, r;
    // Force w to integer; k=first value; r=rank
    RZ(w = jtvi(jt, w));
    k = AV(w)[0];
    r = AR(w);
    if (r) {
        // w is not an atom.  the RNG had better be Mersenne Twister.  Initialize using w, and save the w list
        ASSERT(1 == r && MTI == jt->rng, EVRANK);
        RZ(ras(w));
        fa(jt->rngseed);
        jt->rngseed = w;  // note ra before fa, in case same buffers
        jtmt_init_by_array(jt, AV(w), AN(w));
    } else
        switch (jt->rng) {
            // atomic w.  We can use that for any generator.  Choose the current one.
            case SMI:
                ASSERT(k != 0, EVDOMAIN);
                jtsm_init(jt, k);
                break;
            case GBI: jtgb_init(jt, k); break;
            case MTI: jtmt_init(jt, (UI)k); break;
            case DXI:
                ASSERT(k != 0, EVDOMAIN);
                jtdx_init(jt, k);
                break;
            case MRI: ASSERT(k != 0, EVDOMAIN); jtmr_init(jt, k);
        }
    jt->rngS[jt->rng] = k;  // Save first value, in case k is atomic
    if (!r && MTI == jt->rng && jt->rngseed) {
        fa(jt->rngseed);
        jt->rngseed = 0;
    }  // If k is atomic, discard jt->rngseed if there is one
    // Now jt->rngseed is set iff the w for Mersenne Twister was a list.  jt->rngS[jt->rng] is always set.
    return mtv;
}

static A
jtrollksub(J jt, A a, A w) {
    A z;
    I an, *av, k, m1, n, p, q, r, sh;
    UI m, mk, s, t, *u, x = jt->rngM[jt->rng];
    if (!(a && w)) return 0;
    an = AN(a);
    RE(m1 = jti0(jt, w));
    ASSERT(0 <= m1, EVDOMAIN);
    m = m1;
    RZ(a = jtvip(jt, a));
    av = AV(a);
    PRODX(n, an, av, 1);
    GA(z, 0 == m ? FL : 2 == m ? B01 : INT, n, an, av);
    u = (UI*)AV(z);
    if (!m) {
        D* v = DAV(z);
        INITD;
        if (sh) DQ(n, *v++ = NEXTD1;) else DQ(n, *v++ = NEXTD0;);
    }  // floating-point output
    else if (2 == m) {
        I nslice;
        I j;
        // binary output
        p = (BW / 8) * (nslice = (8 - (BW - jt->rngw)));  // #bits/slice, times number of slices
        // See how many p-size blocks we can have, and how many single leftovers
        q  = n / p;
        r  = n % p;  // q=# p-size blocks, r=#single-bit leftovers
        mk = (UI)0x0101010101010101;
        // Loop to output all the p-size blocks
        for (j = 0; j < q; ++j) {
            t = NEXT;
            DQ(nslice, *u++ = mk & t; t >>= 1;);
        }
        // Get a random # for finishing slices, & out them
        t = NEXT;  // Get random # for slices
        DQ(r >> LGSZI, *u++ = mk & t; t >>= 1;);
        // Output the rest, one bit at a time
        t    = NEXT;  // Get random # for bits
        B* c = (B*)u;
        DQ(r & (SZI - 1), *c++ = 1 & t; t >>= 1;);
    } else {
        // integer output
        r = n;
        s = GMOF(m, x);
        if (s == x) s = 0;
        if (m > 1 && !(m & (m - 1))) {
            if (s == 0)
                s = 0 - m;  // since we reject t>=s, we must make s less than IMAX.  This is the max possible multiple
                            // of s.  We don't check for s=0 in this path.  s==0 possible only in 32-bit
            // here if w is a power of 2, >2; take bits from each value.  s cannot be 0
            k  = CTTZI(m);  // lg(m)
            p  = jt->rngw / k;
            mk = m - 1;  // p=#results per random number; r is number of values left after bit processing
            r -= p;
            while (r >= 0) {
                do { t = NEXT; } while (s <= t);
                DQU(p, *u++ = mk & t; t >>= k;) r -= p;
            }        // deal p at a time till we are as close to n as we can get
            r += p;  // rebias to get # values still needed
        }
        if (m < (1LL << 50)) {
            // If we can do the calculation in the floating-point unit, do
            D md = m * X64;
            DQ(r,
               *u++ =
                 (I)(md * ((D)(I)NEXT + (D)x63));)  // avoid unsigned conversion, which requires conditional correction
        } else {
            if (r && s) DQ(r, while (s <= (t = NEXT)); *u++ = t % m;) else DQ(r, *u++ = NEXT % m;);
        }
    }
    return z;
}

A
jtrollk(J jt, A a, A w, A self) {
    A g, z;
    V* sv;
    sv = FAV(self);
    g  = sv->fgh[2] ? sv->fgh[2] : sv->fgh[1];
    if (AT(w) & XNUM + RAT || !(!AR(w) && 1 >= AR(a) && (g == ds(CDOLLAR) || 1 == AN(a))))
        return jtroll(jt, df2(z, a, w, g));
    return jtrollksub(jt, a, jtvi(jt, w));
} /* ?@$ or ?@# or [:?$ or [:?# */

static A
jtrollbool(J jt, A w) {
    A z;
    B* v;
    D* u;
    I n, sh;
    UINT mk;
    n = AN(w);
    v = BAV(w);
    INITD;
    GATV(z, FL, n, AR(w), AS(w));
    u = DAV(z);
    if (sh)
        DQ(n, *u++ = *v++ ? 0.0 : NEXTD1;)
    else
        DQ(n, *u++ = *v++ ? 0.0 : NEXTD0;)
    return z;
} /* ?n$x where x is boolean */

// If w is all 2, deal Booleans, with each each bit of a random number providing a single Boolean
// Result is Boolean array, or mark if w is not all 2
// *b=0 if w contained non-2, 1 otherwise (i. e. result is valid if *b=1)
static A
jtroll2(J jt, A w, B* b) {
    A z;
    I j, n, nslice, p, q, r, *v;
    UI mk, t, *zv;
    *b = 0;
    n  = AN(w);
    v  = AV(w);  // init failure return; n=#atoms of w, v->first atom
    // If w contains non-2, return with error
    DO(n, if (v[i] != 2) return mark;);  // return fast if not all-Boolean result
    // See how many RNG values to use.  jt->rngw gives the number of bits in a generated random #
    // We will shift these out 4 or 8 bits at a time; the number of slices we can get out of
    // a random number is 8 - the number of non-random bits at the top of a word.  p will be the number
    // of bits we can get per random number
    p = (BW / 8) * (nslice = (8 - (BW - jt->rngw)));  // #bits/slice, times number of slices
    // See how many p-size blocks we can have, and how many single leftovers
    q  = n / p;
    r  = n % p;  // q=# p-size blocks, r=#single-bit leftovers
    mk = (UI)0x0101010101010101;
    GATV(z, B01, n, AR(w), AS(w));
    zv = (UI*)AV(z);  // Allocate result area
    // Loop to output all the p-size blocks
    for (j = 0; j < q; ++j) {
        t = NEXT;
        DQ(nslice, *zv++ = mk & t; t >>= 1;);
    }
    // Get a random # for finishing slices, & out them
    t = NEXT;  // Get random # for slices
    DQ(r >> LGSZI, *zv++ = mk & t; t >>= 1;);
    // Output the rest, one bit at a time
    t    = NEXT;  // Get random # for bits
    B* c = (B*)zv;
    DQ(r & (SZI - 1), *c++ = 1 & t; t >>= 1;);
    *b = 1;
    return z;
} /* ?n$x where x is 2, maybe */

static A
jtrollnot0(J jt, A w, B* b) {
    A z;
    I j, m1, n, *u, *v;
    UI m, s, t, x = jt->rngM[jt->rng];
    *b = 0;
    n  = AN(w);
    if (n) {
        v  = AV(w);
        m1 = *v++;
        j  = 1;
        DQ(
          n - 1, if (m1 != *v++) {
              j = 0;
              break;
          });
    }
    if (n && j)
        RZ(z = jtrollksub(jt, shape(jt, w), jtsc(jt, m1)))
    else {
        GATV(z, INT, n, AR(w), AS(w));
        v = AV(w);
        u = AV(z);
        for (j = 0; j < n; ++j) {
            m1 = *v++;
            if (!m1) return mark;
            ASSERT(0 <= m1, EVDOMAIN);
            m = m1;
            s = GMOF(m, x);
            t = NEXT;
            if (s)
                while (s <= t) t = NEXT;
            *u++ = t % m;
        }
    }
    *b = 1;
    return z;
} /* ?n$x where x is not 0, maybe */

static A
jtrollany(J jt, A w, B* b) {
    A z;
    D* u;
    I j, m1, n, sh, *v;
    UI m, mk, s, t, x = jt->rngM[jt->rng];
    *b = 0;
    n  = AN(w);
    v  = AV(w);
    INITD;
    GATV(z, FL, n, AR(w), AS(w));
    u = DAV(z);
    for (j = 0; j < n; ++j) {
        m1 = *v++;
        ASSERT(0 <= m1, EVDOMAIN);
        m = m1;
        if (0 == m)
            *u++ = sh ? NEXTD1 : NEXTD0;
        else {
            s = GMOF(m, x);
            t = NEXT;
            if (s)
                while (s <= t) t = NEXT;
            *u++ = (D)(t % m);
        }
    }
    *b = 1;
    return z;
} /* ?s$x where x can be anything and 1<#x */

A
jtroll(J jt, A w) {
    A z;
    B b = 0;
    I m, wt;
    if (!w) return 0;
    wt = AT(w);
    ASSERT(wt & DENSE, EVDOMAIN);
    ASSERT(!(wt & XNUM + RAT), EVDOMAIN);

    if (!AN(w)) {
        GATV(z, B01, 0, AR(w), AS(w));
        return z;
    }
    if (wt & B01) return jtrollbool(jt, w);

    RZ(w = jtvi(jt, w));
    m = AV(w)[0];
    if (2 == m) RZ(z = jtroll2(jt, w, &b));
    if (!b && 0 != m) RZ(z = jtrollnot0(jt, w, &b));
    if (!b) RZ(z = jtrollany(jt, w, &b));
    return z;
}

A
jtdeal(J jt, A a, A w) {
    A z;
    I at, j, k, m, n, wt, *zv;
    UI c, s, t, x = jt->rngM[jt->rng];
    UI sq;
    at = AT(a);
    wt = AT(w);
    ASSERT(at & DENSE & at && wt & DENSE, EVDOMAIN);
    F2RANK(0, 0, jtdeal, UNUSED_VALUE);
    RE(m = jti0(jt, a));
    RE(c = n = jti0(jt, w));             // c starts as max#+1
    ASSERT(0 <= m && m <= n, EVDOMAIN);  // m and n must both be positive
    if (0 == m)
        z = mtv;
    else if (m * 3.0 < n || (x && x <= (UI)n)) {  // TUNE for about m=100000; the cutoff would be higher for smaller n
        // calculate the number of values to deal: m, plus a factor times the expected number of collisions, plus 2 for
        // good measure.  Will never exceed n.  Repeats a little less than 1% of the time for n between 30 and 300
        A h = jtsc(jt, m + 4 + (I)((n < 1000 ? 2.4 : 2.2) * ((D)m + (D)n * (pow((((D)(n - 1)) / (D)n), (D)m) - 1))));
        do { RZ(z = jtnub(jt, jtrollksub(jt, h, w))); } while (AN(z) < m);
        RZ(z = jttake(JTIPW, a, z));
    } else {
        RZ(z = jtapvwr(jt, n, 0L, 1L));
        zv = AV(z);
        if (n < (1LL << 50)) {
            // If we can do the calculation in the floating-point unit, do
            D cd = c * X64;
            DO(m, j = i + (I)(cd * ((D)(I)NEXT + (D)x63)); cd -= X64; k = zv[i]; zv[i] = zv[j];
               zv[j] = k;)  // avoid unsigned conversion, which requires conditional correction
        } else {
            GMOF2(c, x, s, sq);
            DO(m, if (s < GMOTHRESH) GMOF2(c, x, s, sq); t = NEXT; if (s) while (s <= t) t = NEXT; s -= sq;
               j     = i + t % c--;
               k     = zv[i];
               zv[i] = zv[j];
               zv[j] = k;);
        }

        AN(z) = AS(z)[0] = m;  // lots of wasted space!
    }
    return at & XNUM + RAT || wt & XNUM + RAT ? jtxco1(jt, z) : z;
}

// support for ?.
// To the extent possible, ?. is frozen.  Changed modules need to be copied here to preserve compatibility
#undef GMOF
/* m<x, greatest multiple of m less than x */
#define GMOF(m, x) (x63 + (x63 - (2 * (x63 % m)) % m))

static A
jtrollksubdot(J jt, A a, A w) {
    A z;
    I an, *av, k, m1, n, p, q, r, sh;
    UI j, m, mk, s, t, *u, x = jt->rngM[jt->rng];
    if (!(a && w)) return 0;
    an = AN(a);
    RE(m1 = jti0(jt, w));
    ASSERT(0 <= m1, EVDOMAIN);
    m = m1;
    RZ(a = jtvip(jt, a));
    av = AV(a);
    PRODX(n, an, av, 1);
    GA(z, 0 == m ? FL : 2 == m ? B01 : INT, n, an, av);
    u = (UI*)AV(z);
    if (!m) {
        D* v = DAV(z);
        INITD;
        if (sh) DQ(n, *v++ = NEXTD1;) else DQ(n, *v++ = NEXTD0;);
    } else if (2 == m) {
        I nslice;
        I j;
        p = (BW / 8) * (nslice = (8 - (BW - jt->rngw)));  // #bits/slice, times number of slices
        // See how many p-size blocks we can have, and how many single leftovers
        q  = n / p;
        r  = n % p;  // q=# p-size blocks, r=#single-bit leftovers
        mk = (UI)0x0101010101010101;
        // Loop to output all the p-size blocks
        for (j = 0; j < q; ++j) {
            t = NEXT;
            DQ(nslice, *u++ = mk & t; t >>= 1;);
        }
        // Get a random # for finishing slices, & out them
        t = NEXT;  // Get random # for slices
        DQ(r >> LGSZI, *u++ = mk & t; t >>= 1;);
        // Output the rest, one bit at a time
        t    = NEXT;  // Get random # for bits
        B* c = (B*)u;
        DQ(r & (SZI - 1), *c++ = 1 & t; t >>= 1;);
    } else {
        r = n;
        s = GMOF(m, x);
        if (s == x) s = 0;
        k = 0;
        j = 1;
        while (m > j) {
            ++k;
            j <<= 1;
        }
        if (k && j == m) { /* m=2^k but is not 1 or 2 */
            p  = jt->rngw / k;
            q  = n / p;
            r  = n % p;
            mk = m - 1;
            switch ((s ? 2 : 0) + (1 < p)) {
                case 0: DQ(q, t = NEXT; *u++ = mk & t;); break;
                case 1: DQ(q, t = NEXT; DQ(p, *u++ = mk & t; t >>= k;);); break;
                case 2: DQ(q, while (s <= (t = NEXT)); *u++ = mk & t;); break;
                case 3: DQ(q, while (s <= (t = NEXT)); DQ(p, *u++ = mk & t; t >>= k;););
            }
        }
        if (r && s) DQ(r, while (s <= (t = NEXT)); *u++ = t % m;) else DQ(r, *u++ = NEXT % m;);
    }
    return z;
}

A
jtrollkdot(J jt, A a, A w, A self) {
    A g, z;
    V* sv;
    sv = FAV(self);
    g  = sv->fgh[2] ? sv->fgh[2] : sv->fgh[1];
    if (AT(w) & XNUM + RAT || !(!AR(w) && 1 >= AR(a) && (g == ds(CDOLLAR) || 1 == AN(a))))
        return jtroll(jt, df2(z, a, w, g));
    return jtrollksubdot(jt, a, jtvi(jt, w));
} /* ?@$ or ?@# or [:?$ or [:?# */

static A
jtrollbooldot(J jt, A w) {
    A z;
    B* v;
    D* u;
    I n, sh;
    UINT mk;
    n = AN(w);
    v = BAV(w);
    INITD;
    GATV(z, FL, n, AR(w), AS(w));
    u = DAV(z);
    if (sh)
        DQ(n, *u++ = *v++ ? 0.0 : NEXTD1;)
    else
        DQ(n, *u++ = *v++ ? 0.0 : NEXTD0;)
    return z;
} /* ?n$x where x is boolean */

// If w is all 2, deal Booleans, with each each bit of a random number providing a single Boolean
// Result is Boolean array, or mark if w is not all 2
// *b=0 if w contained non-2, 1 otherwise (i. e. result is valid if *b=1)

static A
jtroll2dot(J jt, A w, B* b) {
    A z;
    I j, n, nslice, p, q, r, *v;
    UI mk, t, *zv;
    *b = 0;
    n  = AN(w);
    v  = AV(w);  // init failure return; n=#atoms of w, v->first atom
    // If w contains non-2, return with error
    DO(n, if (v[i] != 2) return mark;);  // return fast if not all-Boolean result
    // See how many RNG values to use.  jt->rngw gives the number of bits in a generated random #
    // We will shift these out 4 or 8 bits at a time; the number of slices we can get out of
    // a random number is 8 - the number of non-random bits at the top of a word.  p will be the number
    // of bits we can get per random number
    p = (BW / 8) * (nslice = (8 - (BW - jt->rngw)));  // #bits/slice, times number of slices
    // See how many p-size blocks we can have, and how many single leftovers
    q  = n / p;
    r  = n % p;  // q=# p-size blocks, r=#single-bit leftovers
    mk = (UI)0x0101010101010101;
    GATV(z, B01, n, AR(w), AS(w));
    zv = (UI*)AV(z);  // Allocate result area
    // Loop to output all the p-size blocks
    for (j = 0; j < q; ++j) {
        t = NEXT;
        DQ(nslice, *zv++ = mk & t; t >>= 1;);
    }
    // Get a random # for finishing slices, & out them
    t = NEXT;  // Get random # for slices
    DQ(r >> LGSZI, *zv++ = mk & t; t >>= 1;);
    // Output the rest, one bit at a time
    t    = NEXT;  // Get random # for bits
    B* c = (B*)zv;
    DQ(r & (SZI - 1), *c++ = 1 & t; t >>= 1;);
    *b = 1;
    return z;
} /* ?n$x where x is 2, maybe */

static A
jtrollnot0dot(J jt, A w, B* b) {
    A z;
    I j, m1, n, *u, *v;
    UI m, s, t, x = jt->rngM[jt->rng];
    *b = 0;
    n  = AN(w);
    if (n) {
        v  = AV(w);
        m1 = *v++;
        j  = 1;
        DQ(
          n - 1, if (m1 != *v++) {
              j = 0;
              break;
          });
    }
    if (n && j)
        RZ(z = jtrollksubdot(jt, shape(jt, w), jtsc(jt, m1)))
    else {
        GATV(z, INT, n, AR(w), AS(w));
        v = AV(w);
        u = AV(z);
        for (j = 0; j < n; ++j) {
            m1 = *v++;
            if (!m1) return mark;
            ASSERT(0 <= m1, EVDOMAIN);
            m = m1;
            s = GMOF(m, x);
            t = NEXT;
            if (s)
                while (s <= t) t = NEXT;
            *u++ = t % m;
        }
    }
    *b = 1;
    return z;
} /* ?n$x where x is not 0, maybe */

static A
jtrollanydot(J jt, A w, B* b) {
    A z;
    D* u;
    I j, m1, n, sh, *v;
    UI m, mk, s, t, x = jt->rngM[jt->rng];
    *b = 0;
    n  = AN(w);
    v  = AV(w);
    INITD;
    GATV(z, FL, n, AR(w), AS(w));
    u = DAV(z);
    for (j = 0; j < n; ++j) {
        m1 = *v++;
        ASSERT(0 <= m1, EVDOMAIN);
        m = m1;
        if (0 == m)
            *u++ = sh ? NEXTD1 : NEXTD0;
        else {
            s = GMOF(m, x);
            t = NEXT;
            if (s)
                while (s <= t) t = NEXT;
            *u++ = (D)(t % m);
        }
    }
    *b = 1;
    return z;
} /* ?s$x where x can be anything and 1<#x */

static A
jtrolldot(J jt, A w) {
    A z;
    B b = 0;
    I m, wt;
    wt = AT(w);
    ASSERT(wt & DENSE, EVDOMAIN);
    ASSERT(!(wt & XNUM + RAT), EVDOMAIN);

    if (!AN(w)) {
        GATV(z, B01, 0, AR(w), AS(w));
        return z;
    }

    if (wt & B01) return jtrollbooldot(jt, w);

    RZ(w = jtvi(jt, w));
    m = AV(w)[0];
    if (2 == m) RZ(z = jtroll2dot(jt, w, &b));
    if (!b && 0 != m) RZ(z = jtrollnot0dot(jt, w, &b));
    if (!b) RZ(z = jtrollanydot(jt, w, &b));
    return z;
}

static A
jtdealdot(J jt, A a, A w) {
    A h, y, z;
    I at, d, *hv, i, i1, j, k, m, n, p, q, *v, wt, *yv, *zv;
    UI c, s, t, x = jt->rngM[jt->rng];
    at = AT(a);
    wt = AT(w);
    ASSERT(at & DENSE & at && wt & DENSE, EVDOMAIN);
    F2RANK(0, 0, jtdealdot, UNUSED_VALUE);
    RE(m = jti0(jt, a));
    RE(c = n = jti0(jt, w));
    ASSERT(0 <= m && m <= n, EVDOMAIN);  // m and n must both be positive
    if (0 == m)
        z = mtv;
    else if (m < n / 5.0 || x <= (UI)n) {
        FULLHASHSIZE(2 * m, INTSIZE, 1, 0, p);
        GATV0(h, INT, p, 1);
        hv = AV(h);
        DO(p, hv[i] = 0;);
        GATV0(y, INT, 2 + 2 * m, 1);
        yv = AV(y);
        d  = 2;
        GATV0(z, INT, m, 1);
        zv = AV(z);
        for (i = 0; i < m; ++i) {
            s = GMOF(c, x);
            t = NEXT;
            if (s)
                while (s <= t) t = NEXT;
            j = i + t % c--;
            q = i % p;
            while (hv[q] && (v = yv + hv[q], i != *v)) q = (1 + q) % p;
            i1 = hv[q] ? v[1] : i;
            q  = j % p;
            while (hv[q] && (v = yv + hv[q], j != *v)) q = (1 + q) % p;
            if (hv[q]) {
                ++v;
                *zv++ = *v;
                *v    = i1;
            } else {
                v     = yv + d;
                *zv++ = *v++ = j;
                *v           = i1;
                hv[q]        = d;
                d += 2;
            }
        }
    } else {
        RZ(z = jtapvwr(jt, n, 0L, 1L));
        zv = AV(z);
        DO(m, s = GMOF(c, x); t = NEXT; if (s) while (s <= t) t = NEXT; j = i + t % c--; k = zv[i]; zv[i] = zv[j];
           zv[j] = k;);
        AN(z) = AS(z)[0] = m;
    }
    return at & XNUM + RAT || wt & XNUM + RAT ? jtxco1(jt, z) : z;
}

#define FXSDECL       \
    A z;              \
    I i, j = jt->rng; \
    UI* v = jt->rngV[GBI];
#define FXSDO                                    \
    {                                            \
        i = j == GBI ? jt->rngi : jt->rngI[GBI]; \
        if (!jt->rngfxsv) {                      \
            GAT0(z, INT, GBN, 1);                \
            ras(z);                              \
            jt->rngfxsv = AV(z);                 \
        }                                        \
        jt->rngV[GBI] = jt->rngfxsv;             \
        jtrngselects(jt, jtsc(jt, GBI));         \
        jtgb_init(jt, 16807);                    \
    }
#define FXSOD                          \
    {                                  \
        jt->rngV[GBI] = v;             \
        jt->rngI[GBI] = jt->rngi = i;  \
        jtrngselects(jt, jtsc(jt, j)); \
    }

A
jtrollx(J jt, A w) {
    FXSDECL;
    FXSDO;
    z = jtrolldot(jt, w);
    FXSOD;
    return z;
}
A
jtdealx(J jt, A a, A w) {
    FXSDECL;
    F2RANK(0, 0, jtdealx, UNUSED_VALUE);
    FXSDO;
    z = jtdealdot(jt, a, w);
    FXSOD;
    return z;
}
A
jtrollkx(J jt, A a, A w, A self) {
    FXSDECL;
    FXSDO;
    z = jtrollkdot(jt, a, w, self);
    FXSOD;
    return z;
}
