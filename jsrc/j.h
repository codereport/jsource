/* Copyright 1990-2014, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Global Definitions                                                      */

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

#if defined(__clang_major__) && !defined(__clang__)
#error need workaround by define __clang__ in preprocessor macro
#endif

/* clang-cl */
#if defined(__clang__) && !defined(__GNUC__)
#define __GNUC__ 4
#undef __GNUC_MINOR__
#define __GNUC_MINOR__ 2
#undef __GNUC_PATCHLEVEL__
#define __GNUC_PATCHLEVEL__ 1
#endif

#include <stdint.h>
#include <float.h>
#include <limits.h>
#define link unused_syscall_link
#define qdiv unused_netbsd_qdiv
#include <stdlib.h>
#undef link
#undef qdiv

#include <errno.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define IMAX 9223372036854775807LL
#define IMIN (~9223372036854775807LL) /* ANSI C LONG_MIN is  -LONG_MAX */
#define FLIMAX 9223372036854775296.   // largest FL value that can be converted to I
#define FMTI "%lli"
#define FMTI02 "%02lli"
#define FMTI04 "%04lli"
#define FMTI05 "%05lli"

#define strtoI strtoll

#ifndef XINF
#define XINF "\000\000\000\000\000\000\360\177"
#define XNAN "\000\000\000\000\000\000\370\377"
#endif

#ifndef XINF
#define XINF "\177\360\000\000\000\000\000\000"
#define XNAN "\177\370\000\000\000\000\000\000"
#endif

#ifndef PI
#define PI ((D)3.14159265358979323846)
#endif
#define P2 ((D)6.28318530717958647693)
#ifndef OVERFLOW
#define OVERFLOW ((D)8.988465674311578e307)
#endif
#ifndef UNDERFLOW
#define UNDERFLOW ((D)4.450147717014403e-308)
#endif

#define NALP 256                      /* size of alphabet                */
#define NETX 2000                     /* size of error display buffer    */
#define NPP 20                        /* max value for quad pp           */
#define RMAXX 16                      // number of bits in rank
#define RMAX ((((I)1) << RMAXX) - 1)  // max rank
#define NPATH 1024                    /* max length for path names,      */
                                      /* including trailing 0 byte       */

#define NTSTACK \
    16384L  // number of BYTES in an allocated block of tstack - pointers to allocated blocks - allocation is bigger to
            // leave this many bytes on boundary
#define NTSTACKBLOCK 2048  // boundary for beginning of stack block

// Sizes for the internal stacks.  The goal here is to detect a runaway recursion before it creates a segfault.  This
// cannot be done with precision because we don't know how much C stack we have, or how much is used by a recursion (and
// anyway it depends on what J functions are running). There are two limits: maximum depth of J functions, and maximum
// depth of named functions.  The named-function stack is intelligent and stacks only when there is a locale change or
// deletion; it almost never limits unless locatives are used to an extreme degree. The depth of J function calls will
// probably limit stack use. increase OS stack limit instead of restricting NFDEP/NFCALL
#define NFDEP 2000L

// NEW WAY
// The named-call stack is used only when there is a locative, EXCEPT that after a call to 18!:4 it is used until the
// function calling 18!:4 returns. Since startup calls 18!:4 without a name, we have to allow for the possibility of
// deep recursion in the name stack.  Normally only a little of the stack is used
#define NFCALL (NFDEP / 2)  // call depth for named calls, not important
// Now we are trying to watch the C stack directly
#define CSTACKSIZE (12000000)  // size we allocate in the calling function
#define CSTACKRESERVE 100000   // amount we allow for slop before we sample the stackpointer, and after the last check

// start and length for the stored vector of ascending integers
#define IOTAVECBEGIN (-20)
#define IOTAVECLEN 400

// modes for jtindexofsub(jt, )
#define IIOPMSK 0xf  // operation bits
#define IIDOT 0      // IIDOT and IICO must be 0-1
#define IICO 1
#define INUBSV 2
#define INUB 3
#define ILESS 4
#define INUBI 5
#define IEPS 6
// the I...EPS values below are wired into the function table at the end of vcompsc.c
#define II0EPS 7  // this must come first; other base on it
#define II1EPS 8
#define IJ0EPS 9
#define IJ1EPS 10
#define ISUMEPS 11
#define IANYEPS 12
#define IALLEPS 13
#define IIFBEPS 14
#define IFORKEY 15  // special key support: like i.~, but add # values mapped to the index
#define IIMODPACK \
    0x10  // modifier for type.  (small-range search except i./i:) In IIDOT/IICO, indicates reflexive application.  In
          // others, indicates that the bitmask should be stored as packed bits rather than bytes

#define IIMODREFLEXX 4
#define IIMODREFLEX \
    (((I)1) << IIMODREFLEXX)  // (small-range i. and i:) this is i.~/i:~ (hashing) this is i.~/i:~/~./~:/I.@:~.
#define IIMODFULL \
    0x20  // (small-range search) indicates that the min/max values cover the entire range of possible inputs, so no
          // range checking is required.  Always set for hashing
#define IIMODBASE0 \
    0x40  // set in small-range i./i: (which never use BITS) to indicate that the hashtable starts at index 0 and has m
          // in the place of unused indexes.  Set in hashing always, with same meaning
#define IIMODBITS \
    0x80  // set if the hash field stores bits rather than indexes.  Used only for small-range and not i./i:.  IIMODPACK
          // qualifies this, indicating that the bits are packed
#define IIMODFORCE0X 8
#define IIMODFORCE0 (((I)1) << IIMODFORCE0X)  // set to REQUIRE a (non-bit) allocation to reset to offset 0 and clear
#define IPHCALCX 9
#define IPHCALC (((I)1) << IPHCALCX)  // set when we are calculating a prehashed table
#define IINOTALLOCATED 0x400          // internal flag, set when the block has not been allocated
#define IPHOFFSET 0x2000              /* offset for prehashed versions - set when we are using a prehashed table   */
#define IPHIDOT (IPHOFFSET + IIDOT)
#define IPHICO (IPHOFFSET + IICO)
#define IPHEPS (IPHOFFSET + IEPS)
#define IPHI0EPS (IPHOFFSET + II0EPS)
#define IPHI1EPS (IPHOFFSET + II1EPS)
#define IPHJ0EPS (IPHOFFSET + IJ0EPS)
#define IPHJ1EPS (IPHOFFSET + IJ1EPS)
#define IPHSUMEPS (IPHOFFSET + ISUMEPS)
#define IPHANYEPS (IPHOFFSET + IANYEPS)
#define IPHALLEPS (IPHOFFSET + IALLEPS)
#define IPHIFBEPS (IPHOFFSET + IIFBEPS)

#define jround(x) floor(0.5 + (x))  // for paranoid compatibility with earlier versions

#define BB 8                 // # bits in a byte
#define LGBB 3               // lg(BB)
#define BW 64                // # bits in a word
#define LGSZI 3              // lg(#bytes in an I)
#define LGBW (LGSZI + LGBB)  // lg (# bits in a word)

// nominal cache sizes for current processors
#define L2CACHESIZE (((I)1) << 18)

#define TOOMANYATOMS \
    0xFFFFFFFFFFFFLL  // more atoms than this is considered overflow (64-bit).  i.-family can't handle more than 2G
                      // cells in array.

#define ALTBYTES 0x00ff00ff00ff00ffLL
// t has totals per byte-lane, result combines them into single total.  t must be an lvalue
#define ADDBYTESINI(t)                           \
    (t = (t & ALTBYTES) + ((t >> 8) & ALTBYTES), \
     t = (t >> 32) + t,                          \
     t = (t >> 16) + t,                          \
     t &= 0xffff)  // sig in 01ff01ff01ff01ff, then xxxxxxxx03ff03ff, then xxxxxxxxxxxx07ff, then 00000000000007ff
#define VALIDBOOLEAN 0x0101010101010101LL  // valid bits in a Boolean

// macros for bit testing
#define SGNIF(v, bitno) ((I)(v) << (BW - 1 - (bitno)))  // Sets sign bit if the numbered bit is set
#define SGNIFNOT(v, bitno) (~SGNIF((v), (bitno)))       // Clears sign bit if the numbered bit is set

#define ABS(a) (0 <= (a) ? (a) : -(a))
#define ASSERT(b, e)          \
    {                         \
        if (!(b)) {           \
            jtjsignal(jt, e); \
            return 0;         \
        }                     \
    }
// version for debugging
#define ASSERTD(b, s)         \
    {                         \
        if (!(b)) {           \
            jtjsigd(jt, (s)); \
            return 0;         \
        }                     \
    }
#define ASSERTMTV(w)                \
    {                               \
        ASSERT(1 == AR(w), EVRANK); \
        ASSERT(!AN(w), EVLENGTH);   \
    }
#define ASSERTN(b, e, nm)       \
    {                           \
        if (!(b)) {             \
            jt->curname = (nm); \
            jtjsignal(jt, e);   \
            return 0;           \
        }                       \
    }  // set name for display (only if error)
#define ASSERTSYS(b, s)                                                                     \
    {                                                                                       \
        if (!(b)) {                                                                         \
            fprintf(stderr, "system error: %s : file %s line %d\n", s, __FILE__, __LINE__); \
            jtjsignal(jt, EVSYSTEM);                                                        \
            jtwri(jt, MTYOSYS, "", (I)strlen(s), s);                                        \
            return 0;                                                                       \
        }                                                                                   \
    }
#define ASSERTWR(c, e)          \
    {                           \
        if (!(c)) { return e; } \
    }

#define ASSERTAGREE(x, y, l)                                                    \
    {                                                                           \
        I *aaa = (x), *aab = (y);                                               \
        I aai = (l);                                                            \
        if (aai <= 2) {                                                         \
            aai -= 1;                                                           \
            aaa = (aai < 0) ? (I *)&validitymask[1] : aaa;                      \
            aab = (aai < 0) ? (I *)&validitymask[1] : aab;                      \
            ASSERT(((aaa[0] ^ aab[0]) + (aaa[aai] ^ aab[aai])) == 0, EVLENGTH); \
        } else {                                                                \
            ASSERT(!memcmp(aaa, aab, aai << LGSZI), EVLENGTH)                   \
        }                                                                       \
    }
#define TESTDISAGREE(r, x, y, l)                                    \
    {                                                               \
        I *aaa = (x), *aab = (y);                                   \
        I aai = (l);                                                \
        if (aai <= 2) {                                             \
            aai -= 1;                                               \
            aaa = (aai < 0) ? (I *)&validitymask[1] : aaa;          \
            aab = (aai < 0) ? (I *)&validitymask[1] : aab;          \
            r   = ((aaa[0] ^ aab[0]) + (aaa[aai] ^ aab[aai])) != 0; \
        } else {                                                    \
            r = memcmp(aaa, aab, aai << LGSZI) != 0;                \
        }                                                           \
    }

// BETWEENx requires that lo be <= hi
#define BETWEENC(x, lo, hi) ((UI)((x) - (lo)) <= (UI)((hi) - (lo)))  // x is in [lo,hi]
#define BETWEENO(x, lo, hi) ((UI)((x) - (lo)) < (UI)((hi) - (lo)))   // x is in [lo,hi)
#define BMK(x) (1LL << (x))                                          // bit number x
// test for equality of 2 8-bit values simultaneously
#define BOTHEQ8(x, y, X, Y) (((US)(C)(x) << 8) + (US)(C)(y) == ((US)(C)(X) << 8) + (US)(C)(Y))
#define CALL1(f, w, fs) ((f)(jt, (w), (A)(fs)))
#define CALL2(f, a, w, fs) ((f)(jt, (a), (w), (A)(fs)))
#define CALL1IP(f, w, fs) ((f)(jtinplace, (w), (A)(fs)))
#define CALL2IP(f, a, w, fs) ((f)(jtinplace, (a), (w), (A)(fs)))
#define CALLSTACKRESET                                     \
    {                                                      \
        jt->callstacknext = 0;                             \
        jt->uflags.us.uq.uq_c.pmctrbstk &= ~PMCTRBSTKREQD; \
    }  // establish initial conditions for things that might not get processed off the stack.  The last things stacked
       // may never be popped
#define MODESRESET           \
    {                        \
        jt->xmode = XMEXACT; \
    }  // anything that might get left in a bad state and should be reset on return to immediate mode
// see if a character matches one of many.  Example in ai.c
// create mask for the bit, if any, in word w for value.  Reverse order: 0=MSB
#define CCM(w, value) ((I)(((value) >> LGBW) == (w) ? 1LL << (BW - 1 - ((value)&BW - 1)) : 0))
// user creates #define nm##values(w) CCM(w,value0)+CCM(w,value1)+...
// create 8 mask values, nm##0 to nm##7, for the given values.
#define CCMWD(nm, wd) I nm##wd = nm##values(wd);
#define CCMWDS(nm) \
    CCMWD(nm, 0);  \
    CCMWD(nm, 1);  \
    CCMWD(nm, 2);  \
    CCMWD(nm, 3);  \
    CCMWD(nm, 4);  \
    CCMWD(nm, 5);  \
    CCMWD(nm, 6);  \
    CCMWD(nm, 7);
// Create the comparand for the test value, it the given cand name
#define CCMCAND1(nm, cand, tval, w) cand = nm##w && ((UI)tval >> LGBW) == w ? nm##w : cand;
#define CCMCAND(nm, cand, tval)  \
    I cand = 0;                  \
    CCMCAND1(nm, cand, tval, 0); \
    CCMCAND1(nm, cand, tval, 1); \
    CCMCAND1(nm, cand, tval, 2); \
    CCMCAND1(nm, cand, tval, 3); \
    CCMCAND1(nm, cand, tval, 4); \
    CCMCAND1(nm, cand, tval, 5); \
    CCMCAND1(nm, cand, tval, 6); \
    CCMCAND1(nm, cand, tval, 7)
// set the sign bit to the selected bit of the mask
#define CCMSGN(cand, tval) (cand << (tval & (BW - 1)))           // set sign bit if value found
#define CCMTST(cand, tval) (cand & (1LL << (~tval & (BW - 1))))  // test true is value found
#define DO(n, stm)                  \
    {                               \
        I i = 0, _n = (n);          \
        for (; i < _n; i++) { stm } \
    }  // i runs from 0 to n-1
#define DP(n, stm)                 \
    {                              \
        I i = -(n);                \
        for (; i < 0; ++i) { stm } \
    }  // i runs from -n to -1 (faster than DO)
#define DQ(n, stm)                  \
    {                               \
        I i = (I)(n)-1;             \
        for (; i >= 0; --i) { stm } \
    }  // i runs from n-1 downto 0 (fastest when you don't need i)
#define DQU(n, stm)                  \
    {                                \
        I i = (I)(n)-1;              \
        do { stm } while (--i >= 0); \
    }  // i runs from n-1 downto 0, always at least once
// C suffix indicates that the count is one's complement
#define DQC(n, stm)                 \
    {                               \
        I i = -2 - (I)(n);          \
        for (; i >= 0; --i) { stm } \
    }  // i runs from n-1 downto 0 (fastest when you don't need i)
#define ds(c) (A) & primtab[(UC)(c)]
#define UNUSED_VALUE ds(CRIGHT)  // harmless value to use for self in calls to rank loops
// define fs block used in every/every2.  It is the self for the f in f&.>, and contains only function pointers, an
// optional param in AK, and the flag field
#define EVERYFS(name, f0, f1, akparm, flg) \
    PRIM name = {{akparm, 0, 0, 0, 0, 0, 0}, {.primvb = {.valencefns = {f0, f1}, .flag = flg}}};

#define FDEPDEC(d)
#define FDEPINC(d)
#define STACKCHKOFL                                             \
    {                                                           \
        D stackpos;                                             \
        ASSERT((uintptr_t)&stackpos >= jt->cstackmin, EVSTACK); \
    }
#define FCONS(x) jtfdef(jt, 0, CFCONS, VERB, jtnum1, jtnum2, 0L, 0L, (x), VFLAGNONE, RMAX, RMAX, RMAX)
// fuzzy-equal is used for tolerant comparisons not related to jt->cct; for example testing whether x in x { y is an
// integer
#define FUZZ 0.000000000000056843418860808015  // tolerance
// FFEQ/FFIEQ (fixed fuzz) are used where we know for sure the test should be tolerant
#define FFEQ(u, v) (ABS((u) - (v)) <= FUZZ * MAX(ABS(u), ABS(v)))
#define FFIEQ(u, v)    \
    (ABS((u) - (v)) <= \
     FUZZ *            \
       ABS(v))  // used when v is known to be exact integer.  It's close enough, maybe ULP too small on the high end
#define FPREFIP       \
    J jtinplace = jt; \
    jt          = (J)(intptr_t)((I)jt & ~JTFLAGMSK)  // turn off all flag bits in jt, leave them in jtinplace
#define F1RANK(m, f, self)                           \
    {                                                \
        if (m < AR(w))                               \
            if (m == 0)                              \
                return jtrank1ex0(jt, w, (A)self, f);      \
            else                                     \
                return jtrank1ex(jt, w, (A)self, (I)m, f); \
    }  // if there is more than one cell, run rank1ex on them.  m=monad rank, f=function to call for monad cell.  Fall
       // through otherwise
#define F2RANKcommon(l, r, f, self, extra)                                                                 \
    {                                                                                                      \
        extra if ((I)((l - AR(a)) | (r - AR(w))) < 0) if ((l | r) == 0) return jtrank2ex0(jt, a, w, (A)self, f); \
        else {                                                                                             \
            I lr = MIN((I)l, AR(a));                                                                       \
            I rr = MIN((I)r, AR(w));                                                                       \
            return rank2ex(a, w, (A)self, lr, rr, lr, rr, f);                                              \
        }                                                                                                  \
    }  // If there is more than one cell, run rank2ex on them.  l,r=dyad ranks, f=function to call for dyad cell
#define F2RANK(l, r, f, self) F2RANKcommon(l, r, f, self, )
// same, but used when the function may pull an address from w.  In that case, we have to turn pristine off since there
// may be duplicates in the result
#define F2RANKW(l, r, f, self) F2RANKcommon(l, r, f, self, PRISTCLR(w))
#define F2RANKIP(l, r, f, self)                                                   \
    {                                                                             \
        if ((I)((l - AR(a)) | (r - AR(w))) < 0) {                                 \
            I lr = MIN((I)l, AR(a));                                              \
            I rr = MIN((I)r, AR(w));                                              \
            return jtrank2ex(jtinplace, a, w, (A)self, REX2R(lr, rr, lr, rr), f); \
        }                                                                         \
    }  // If there is more than one cell, run rank2ex on them.  l,r=dyad ranks, f=function to call for dyad cell
// get # of things of size s, rank r to allocate so as to have an odd number of them at least n, after discarding w
// items of waste.  Try to fill up a full buffer
#define FULLHASHSIZE(n, s, r, w, z)                                 \
    {                                                               \
        UI4 zzz;                                                    \
        CTLZI((((n) | 1) + (w)) * (s) + AKXR(r) - 1, zzz);          \
        z = ((((I)1 << (zzz + 1)) - AKXR(r)) / (s)-1) | (1 & ~(w)); \
    }
// Memory-allocation macros
// Size-of-block calculations.  VSZ when size is constant or variable.  Byte counts are (total length including
// header)-1 Because the Boolean dyads read beyond the end of the byte area (up to 1 extra word), we add one SZI-1 for
// islast (which includes B01), rather than adding 1
#define ALLOBYTESVSZ(atoms, rank, size, islast, isname)                                                \
    ((((rank)*SZI +                                                                                    \
       ((islast) ? (isname) ? (NORMAH * SZI + sizeof(NM) + SZI - 1 - 1) : (NORMAH * SZI + SZI - 1 - 1) \
                 : (NORMAH * SZI - 1)) +                                                               \
       (atoms) * (size))))  // # bytes to allocate allowing 1 I for string pad - include mem hdr - minus 1
// here when size is constant.  The number of bytes, rounded up with overhead added, must not exceed 2^(PMINL+4)
#define ALLOBYTES(atoms, rank, size, islast, isname)     \
    ((size & (SZI - 1))                                  \
       ? ALLOBYTESVSZ(atoms, rank, size, islast, isname) \
       : (SZI * ((rank) + NORMAH + ((size) >> LGSZI) * (atoms) + !!(islast)) - 1))  // # bytes to allocate-1
#define ALLOBLOCK(n)                                               \
    ((n) < 2 * PMIN    ? ((n) < PMIN ? PMINL - 1 : PMINL)          \
     : (n) < 8 * PMIN  ? ((n) < 4 * PMIN ? PMINL + 1 : PMINL + 2)  \
     : (n) < 32 * PMIN ? ((n) < 16 * PMIN ? PMINL + 3 : PMINL + 4) \
                       : IMIN)  // lg2(#bytes to allocate)-1.  n is #bytes-1
// value to put into name->bucketx for locale names: number if numeric, hash otherwise
#define BUCKETXLOC(len, s) ((*(s) <= '9') ? strtoI10s((len), (s)) : (I)nmhash((len), (s)))
// GA() is used when the type is unknown.  This routine is in m.c and documents the function of these macros.
// NEVER use GA() for NAME types - it doesn't honor it.
// SHAPER is used when shape is given and rank is SDT.  Usually 0/1 use COPYSHAPE0 but they can use this; it always
// copies from the shaape.  This works only up to rank 2 (but could be extended if needed)
#define GACOPYSHAPER(name, type, atoms, rank, shaape) \
    if ((rank) > 0) AS(name)[0] = (shaape)[0];        \
    if ((rank) > 1) AS(name)[1] = (shaape)[1];        \
    if ((rank) > 2) AS(name)[2] = (shaape)[2];
// SHAPE0 is used when the shape is 0 - write shape only if rank==1
#define GACOPYSHAPE0(name, type, atoms, rank, shaape) \
    if ((rank) == 1) AS(name)[0] = (atoms);
// General shape copy, branchless when rank<3  AS[0] is always written: #atoms if rank=1, 0 if rank=0.  Used in jtga(),
// which uses the 0 in AS[0] as a pun for nullptr
#define GACOPYSHAPEG(name, type, atoms, rank, shaape) \
    {                                                 \
        I *_d = AS(name);                             \
        I *_s = (shaape);                             \
        _s    = _s ? _s : _d;                         \
        I cp  = *_s;                                  \
        I _r  = (rank);                               \
        cp    = _r < 1 ? 0 : cp;                      \
        cp    = _r == 1 ? (atoms) : cp;               \
        _s    = _r <= 1 ? _d : _s;                    \
        *_d   = cp;                                   \
        ++_d;                                         \
        ++_s;                                         \
        if (_r < 3) {                                 \
            *_d = *_s;                                \
        } else {                                      \
            memcpy(_d, _s, (_r - 1) << LGSZI);        \
        }                                             \
    }
// Use when shape is known to be present but rank is not SDT.  One value is always written to shape
// in this version one value is always written to shape
#define GACOPYSHAPE(name, type, atoms, rank, shaape) \
    {                                                \
        I *_s = (I *)(shaape);                       \
        I *_d = AS(name);                            \
        *_d   = *_s;                                 \
        I _r  = 1 - (rank);                          \
        do {                                         \
            _s += SGNTO0(_r);                        \
            _d += SGNTO0(_r);                        \
            *_d = *_s;                               \
        } while (++_r < 0);                          \
    }
#define GACOPY1(name, type, atoms, rank, shaape) \
    {                                            \
        I *_d = AS(name);                        \
        *_d   = 1;                               \
        I _r  = 1 - (rank);                      \
        do {                                     \
            _d += SGNTO0(_r);                    \
            *_d = 1;                             \
        } while (++_r < 0);                      \
    }  // copy all 1s to shape
#define GA(v, t, n, r, s) RZ(v = jtga(jt, t, (I)(n), (I)(r), (I *)(s)))
// GAE executes the given expression when there is an error
#define GAE(v, t, n, r, s, erraction) \
    if (!(v = jtga(jt, t, (I)(n), (I)(r), (I *)(s)))) erraction;

// When the type and all rank/shape are known at compile time, use GAT.  The compiler precalculates
// almost everything. The number of bytes, rounded up with overhead added, must not exceed 2^(PMINL+4)
#define GATS(name, type, atoms, rank, shaape, size, shapecopier, erraction) \
    {                                                                       \
        ASSERT(!((rank) & ~RMAX), EVLIMIT);                                 \
        I bytes = ALLOBYTES(atoms, rank, size, (type)&LAST0, (type)&NAME);  \
        name    = jtgaf(jt, ALLOBLOCK(bytes));                              \
        I akx   = AKXR(rank);                                               \
        if (name != 0) {                                                    \
            AK(name) = akx;                                                 \
            AT(name) = (type);                                              \
            AN(name) = atoms;                                               \
            AR(name) = (RANKT)(rank);                                       \
            if (!((type)&DIRECT)) {                                         \
                if (rank == 0) AS(name)[0] = 0;                             \
                memset((C *)(AS(name) + 1), C0, (bytes - 32) & -32);        \
            }                                                               \
            shapecopier(name, type, atoms, rank, shaape)                    \
                                                                            \
        } else {                                                            \
            erraction;                                                      \
        }                                                                   \
    }

#define GAT(name, type, atoms, rank, shaape) GATS(name, type, atoms, rank, shaape, type##SIZE, GACOPYSHAPE, return 0)
#define GATR(name, type, atoms, rank, shaape) GATS(name, type, atoms, rank, shaape, type##SIZE, GACOPYSHAPER, return 0)
#define GAT0(name, type, atoms, rank) GATS(name, type, atoms, rank, 0, type##SIZE, GACOPYSHAPE0, return 0)
#define GAT0E(name, type, atoms, rank, erraction) GATS(name, type, atoms, rank, 0, type##SIZE, GACOPYSHAPE0, erraction)

// Used when type is known and something else is variable.  ##SIZE must be applied before type is substituted, so we
// have GATVS to use inside other macros.  Normally use GATV Note: assigns name before assigning the components of the
// array, so the components had better not depend on name, i. e. no GATV(z,BOX,AN(z),AR(z),AS(z))
#define GATVS(name, type, atoms, rank, shaape, size, shapecopier, erraction)                   \
    {                                                                                          \
        I bytes = ALLOBYTES(atoms, rank, size, (type)&LAST0, (type)&NAME);                     \
        ASSERT(!((((unsigned long long)(atoms)) & ~TOOMANYATOMS) + ((rank) & ~RMAX)), EVLIMIT) \
        name  = jtgafv(jt, bytes);                                                             \
        I akx = AKXR(rank);                                                                    \
        if (name != 0) {                                                                       \
            AK(name) = akx;                                                                    \
            AT(name) = (type);                                                                 \
            AN(name) = atoms;                                                                  \
            AR(name) = (RANKT)(rank);                                                          \
            if (!((type)&DIRECT)) {                                                            \
                AS(name)[0] = 0;                                                               \
                memset((C *)(AS(name) + 1), C0, (bytes - 32) & -32);                           \
            }                                                                                  \
            shapecopier(name, type, atoms, rank, shaape)                                       \
                                                                                               \
        } else {                                                                               \
            erraction;                                                                         \
        }                                                                                      \
    }

// see warnings above under GATVS
#define GATV(name, type, atoms, rank, shaape) GATVS(name, type, atoms, rank, shaape, type##SIZE, GACOPYSHAPE, return 0)
#define GATVR(name, type, atoms, rank, shaape) \
    GATVS(name, type, atoms, rank, shaape, type##SIZE, GACOPYSHAPER, return 0)
#define GATV1(name, type, atoms, rank) \
    GATVS(name, type, atoms, rank, 0, type##SIZE, GACOPY1, return 0)  // this version copies 1 to the entire shape
#define GATV0(name, type, atoms, rank) \
    GATVS(name, type, atoms, rank, 0, type##SIZE, GACOPYSHAPE0, return 0)  // shape not written unless rank==1
// use this version when you are allocating a sparse matrix.  It handles the AS[0] field correctly.  ALL sparse
// allocations must come through here so that AC is set sorrectly
#define GASPARSE(n, t, a, r, s)       \
    {                                 \
        if ((r) == 1) {               \
            GA(n, (t), a, 1, 0);      \
            if (s) AS(n)[0] = (s)[0]; \
        } else {                      \
            GA(n, (t), a, r, s)       \
        }                             \
        AC(n) = ACUC1;                \
    }

#define HN 4L  // number of boxes per valence to hold exp-def info (words, control words, original (opt.), symbol table)
// Item count given frame and rank: AS(f) unless r is 0; then 1
#define SETICFR(w, f, r, targ) (targ = (I)(AS(w) + f), targ = (r) ? targ : (I)&I1mem, targ = *(I *)targ)
// Shape item s, but 1 if index is < 0
#define SHAPEN(w, s, targ) (targ = AS(w)[s], targ = (s) < 0 ? 1 : targ)

// Item count
#define SETIC(w, targ) (targ = AS(w)[0], targ = AR(w) ? targ : 1)  //   (AR(w) ? *AS(w) : 1L)

#define ICMP(z, w, n) memcmpne((z), (w), (n)*SZI)
#define ICPY(z, w, n) memcpy((z), (w), (n)*SZI)
#define IFCMPNAME(name, string, len, stmt) \
    if ((name)->m == (len))                \
        if (!memcmpne((name)->s, string, len)) stmt

// Mark a block as incorporated by removing its inplaceability.  The blocks that are tested for incorporation are ones
// that are allocated by partitioning, and they will always start out as inplaceable If a block is virtual, it must be
// realized before it can be incorporated.  realized blocks always start off inplaceable and non-pristine z is an lvalue
// Use incorp if you need to tell the caller that the block e sent you has been incorporated.  If you created the block
// being incorporated, even by calling a function that returns it, you can be OK just using rifv() or jtrifvs(jt,). This
// may leave an incorporated block marked inplaceable, but that's OK as long as you don't pass it to some place where it
// can become an argument to another function When a block is incorporated it becomes not pristine, because extractions
// from the parent may compromise it and we don't want to have to go through recursively to find them
#define INCORP(z)                        \
    {                                    \
        I af = AFLAG(z);                 \
        if ((af & AFVIRTUAL) != 0) {     \
            RZ((z) = jtrealize(jt, z))   \
        } else {                         \
            AFLAG(z) = af & ~AFPRISTINE; \
        }                                \
        ACIPNO(z);                       \
    }
#define INCORPNV(z)                  \
    {                                \
        I af     = AFLAG(z);         \
        AFLAG(z) = af & ~AFPRISTINE; \
        ACIPNO(z);                   \
    }  // use when z is known nonvirtul
// use to incorporate into a known-recursive box.  We raise the usecount of z
#define INCORPRA(z)                      \
    {                                    \
        I af = AFLAG(z);                 \
        if ((af & AFVIRTUAL) != 0) {     \
            RZ((z) = jtrealize(jt, z))   \
        } else {                         \
            AFLAG(z) = af & ~AFPRISTINE; \
        }                                \
        ra(z);                           \
    }
// Tests for whether a result incorporates its argument.  The originator, who is going to check this, always marks the
// argument inplaceable, and we signal incorporation either by returning the argument itself or by marking it
// non-inplaceable (if we box it)
#define WASINCORP1(z, w) ((z) == (w) || 0 <= AC(w))
#define INF(x) ((x) == inf || (x) == infm)
// true if a has recursive usecount
#define UCISRECUR(a) (AFLAG(a) & RECURSIBLE)
// Install new value z into xv[k], where xv is AAV(x).  If x has recursive usecount, we must increment the usecount of
// z. This also guarantees that z has recursive usecount whenever x does, and that z is realized
#define INSTALLBOX(x, xv, k, z) \
    realizeifvirtual(z);        \
    if ((UCISRECUR(x)) != 0) {  \
        A zzZ = xv[k];          \
        ra(z);                  \
        fa(zzZ);                \
    }                           \
    xv[k] = z
#define INSTALLBOXNF(x, xv, k, z)       \
    realizeifvirtual(z);                \
    if ((UCISRECUR(x)) != 0) { ra(z); } \
    xv[k] = z  // Don't do the free - if we are installing into known 0 or known nonrecursive
#define INSTALLBOXRECUR(xv, k, z) \
    realizeifvirtual(z);          \
    {                             \
        I zzK = (k);              \
        {                         \
            A zzZ = xv[zzK];      \
            ra(z);                \
            fa(zzZ);              \
        }                         \
        xv[zzK] = z;              \
    }  // Don't test - we know we are installing into a recursive block
// Same thing for RAT type.  z is a Q, xv[k] is a Q
#define INSTALLRAT(x, xv, k, z) \
    if ((UCISRECUR(x)) != 0) {  \
        Q zzZ = xv[k];          \
        ra(z.n);                \
        ra(z.d);                \
        fa(zzZ.n);              \
        fa(zzZ.d);              \
    }                           \
    xv[k] = z
// Use IRS[12] to call a verb that supports IRS.  Rank is nonnegative; result is assigned to z.  z mustn't be any other
// arg - it is also used as a temp
#define IRS1COMMON(j, w, fs, r, f1, z)          \
    (z         = (A)(r),                        \
     z         = (I)AR(w) > (I)(r) ? z : (A)~0, \
     jt->ranks = (RANK2T)(I)z,                  \
     z         = ((AF)(f1))(j, (w), (A)(fs)),   \
     jt->ranks = (RANK2T)~0,                    \
     z)                                                                 // nonneg rank
#define IRS1(w, fs, r, f1, z) IRS1COMMON(jt, w, fs, r, f1, z)           // nonneg rank
#define IRSIP1(w, fs, r, f1, z) IRS1COMMON(jtinplace, w, fs, r, f1, z)  // nonneg rank
#define IRS2COMMON(j, a, w, fs, l, r, f2, z)                                                                      \
    (jt->ranks = (RANK2T)(((((I)AR(a) - (l) > 0) ? (l) : RMAX) << RANKTX) + (((I)AR(w) - (r) > 0) ? (r) : RMAX)), \
     z         = ((AF)(f2))(j, (a), (w), (A)(fs)),                                                                \
     jt->ranks = (RANK2T)~0,                                                                                      \
     z)  // nonneg rank
#define IRS2(a, w, fs, l, r, f2, z) IRS2COMMON(jt, a, w, fs, l, r, f2, z)
#define IRSIP2(a, w, fs, l, r, f2, z) IRS2COMMON(jtinplace, a, w, fs, l, r, f2, z)
// call to atomic2(), similar to IRS2.  fs is a local block to use to hold the rank (declared as D fs[16]), cxx is the
// Cxx value of the function to be called
#define ATOMIC2(jt, a, w, fs, l, r, cxx)                   \
    (FAV((A)(fs))->fgh[0] = ds(cxx),                       \
     FAV((A)(fs))->id     = CQQ,                           \
     FAV((A)(fs))->lrr    = (RANK2T)((l) << RANKTX) + (r), \
     jtatomic2(jt, (a), (w), (A)fs))

// memory copy, for J blocks.  Like memory copy, but knows it can fetch outside the arg boundaries for LIT-type args
// if bytelen is 1, the arg may be of any length; if 0, must be a multiple of Is and the low bits of length are ignored

#define Jmemcpy(d, s, l, lbl, bytelen) memcpy(d, s, bytelen ? (l) : (l) & -SZI);
#define JMCR(d, s, l, lbl, bytelen, maskname) memcpy(d, s, bytelen ? (l) : (l) & -SZI);

#define IX(n) jtapv(jt, (n), 0L, 1L)
#define JATTN                      \
    {                              \
        if (*jt->adbreakr != 0) {  \
            jtjsignal(jt, EVATTN); \
            return 0;              \
        }                          \
    }
#define JBREAK0                     \
    {                               \
        if (2 <= *jt->adbreakr) {   \
            jtjsignal(jt, EVBREAK); \
            return 0;               \
        }                           \
    }
#define JTIPA ((J)((I)jt | JTINPLACEA))
#define JTIPAW ((J)((I)jt | JTINPLACEA + JTINPLACEW))
#define JTIPW ((J)((I)jt | JTINPLACEW))
#define JTIPAtoW \
    (J)((I)jt + (((I)jtinplace >> JTINPLACEAX) & JTINPLACEW))  // jtinplace, with a' inplaceability transferred to w
#define JTIPWonly (J)((I)jtinplace & ~(JTINPLACEA + JTWILLBEOPENED + JTCOUNTITEMS))  // dyad jt converted to monad for w
// given a unique num, define loop begin and end labels
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MCIL(dest, src, n) memcpy(dest, src, (n) * sizeof(*src))  // use when copy expected to be long
// Copy shapes.  Optimized for length <5, subroutine for others
#define MCISH(dest, src, n)                             \
    {                                                   \
        I *_d = (dest), *_s = (src);                    \
        I _n = (I)(n);                                  \
        if (_n <= 2) {                                  \
            _n -= 1;                                    \
            _d     = (_n < 0) ? jt->shapesink + 1 : _d; \
            _s     = (_n < 0) ? jt->shapesink + 1 : _s; \
            _d[0]  = _s[0];                             \
            _d[_n] = _s[_n];                            \
        } else {                                        \
            memcpy(_d, _s, _n << LGSZI);                \
        }                                               \
    }
#define MCISHd(dest, src, n) \
    { MCISH(dest, src, n) dest += (n); }  // ... this version when d increments through the loop

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MLEN (63)
// change the type of the inplaceable block z to t.  We know or assume that the type is being changed.  If the block is
// UNINCORPABLE (& therefore virtual), replace it with a clone first.  z is an lvalue
#define MODBLOCKTYPE(z, t)                                                       \
    {                                                                            \
        if ((AFLAG(z) & AFUNINCORPABLE) != 0) { RZ(z = jtclonevirtual(jt, z)); } \
        AT(z) = (t);                                                             \
    }
#define MODIFIABLE(x) (x)  // indicates that we modify the result, and it had better not be read-only
// define multiply-add
#define NAN0 (_clearfp())
#define NAN1                            \
    {                                   \
        if (_SW_INVALID & _clearfp()) { \
            jtjsignal(jt, EVNAN);       \
            return 0;                   \
        }                               \
    }
#define NANTEST (_SW_INVALID & _clearfp())

#define NUMMAX 9          // largest number represented in num[]
#define NUMMIN (~NUMMAX)  // smallest number represented in num[]
// Given SZI B01s read into p, pack the bits into the MSBs of p and clear the lower bits of p
// this is what it should be #define PACKBITS(p) {p|=p>>7LL;p|=p>>14LL;p|=p>>28LL;p<<=56LL;}
#define PACKBITS(p)       \
    {                     \
        p |= p >> 7LL;    \
        p |= p >> 14LL;   \
        p |= p << 28LL;   \
        p &= 0xff0000000; \
        p <<= 28LL;       \
    }  // this generates one extra instruction, rather than the 3 for the correct version
#define PACKBITSINTO(p, out)                            \
    {                                                   \
        p |= p >> 7LL;                                  \
        p |= p >> 14LL;                                 \
        out = ((p | (p >> 28LL)) << 56) | (out >> SZI); \
    }  // pack and shift into out
#define PRISTCOMSET(w, flg)       \
    awback = (w);                 \
    if ((flg & AFVIRTUAL) != 0) { \
        awback = ABACK(awback);   \
        flg    = AFLAG(awback);   \
    }                             \
    AFLAG(awback) = flg & ~AFPRISTINE;
#define PRISTCOMSETF(w, flg)      \
    if ((flg & AFVIRTUAL) != 0) { \
        w   = ABACK(w);           \
        flg = AFLAG(w);           \
    }                             \
    AFLAG(w) = flg & ~AFPRISTINE;  // used only at end, when w can be destroyed
#define PRISTCOMMON(w, exe) \
    awflg = AFLAG(w);       \
    exe PRISTCOMSET(w, awflg)
// same but destroy w
#define PRISTCOMMONF(w, exe) \
    I awflg = AFLAG(w);      \
    exe PRISTCOMSETF(w, awflg)
#define PRISTCLRNODCL(w) PRISTCOMMON(w, )
// normal entry points.  clear PRISTINE flag in w (or its backer, if virtual) because we have removed something from it
#define PRISTCLR(w) \
    I awflg;        \
    A awback;       \
    PRISTCLRNODCL(w)
// same, but destroy w in the process
#define PRISTCLRF(w) PRISTCOMMONF(w, )
// transfer pristinity of w to z, destroying w
#define PRISTXFERF(z, w)     \
    PRISTCOMMONF(w,          \
                 AFLAG(z) |= \
                 awflg & ((SGNTO0(AC(w)) & ((I)jtinplace >> JTINPLACEWX)) << AFPRISTINEX);)  // use w bit of jtinplace
#define PRISTXFERAF(z, a)    \
    PRISTCOMMONF(a,          \
                 AFLAG(z) |= \
                 awflg & ((SGNTO0(AC(a)) & ((I)jtinplace >> JTINPLACEAX)) << AFPRISTINEX);)  // use a bit of jtinplace
// same, but with an added condition (in bit 0)
#define PRISTXFERFIF(z, w, cond) \
    PRISTCOMMONF(w, AFLAG(z) |= awflg & (((cond)&SGNTO0(AC(w)) & ((I)jtinplace >> JTINPLACEWX)) << AFPRISTINEX);)
// PROD multiplies a list of numbers, where the product is known not to overflow a signed int (for example, it might be
// part of the shape of a nonempty dense array) assign length first so we can sneak some computation into ain in va2
#define PROD(z, length, ain)                \
    {                                       \
        I _i             = (length);        \
        I * _zzt = (ain)-2;         \
        if (_i < 3) {                       \
            _zzt += _i;                     \
            z    = (I)&oneone;              \
            _zzt = _i >= 1 ? _zzt : (I *)z; \
            z    = _i > 1 ? (I)_zzt : z;    \
            z    = ((I *)z)[0];             \
            z *= _zzt[1];                   \
        } else {                            \
            z = jtprod(jt, _i, _zzt + 2);   \
        }                                   \
    }
// This version ignores bits of length above the low RANKTX bits
#define PRODRNK(result, length, ain)                             \
    {                                                            \
        I _i             = (length);                             \
        I * _zzt = (ain);                                \
        if ((US)_i < 3) {                                        \
            _zzt   = _i & 3 ? _zzt : iotavec - IOTAVECBEGIN + 1; \
            result = *_zzt;                                      \
            ++_zzt;                                              \
            _zzt = _i & 2 ? _zzt : iotavec - IOTAVECBEGIN + 1;   \
            result *= *_zzt;                                     \
        } else {                                                 \
            result = jtprod(jt, (US)_i, _zzt);                   \
        }                                                        \
    }

// PRODX replaces CPROD.  It is PROD with a test for overflow included.  To save calls to mult, PRODX takes an initial
// value PRODX takes the product of init and v[0..n-1], generating error if overflow, but waiting till the end so no
// error if there is a 0 in the product overflow sets z to the error value of 0; if we see a multiplicand of 0 we stop
// right away so we can skip the error This is written to be branchless for rank < 3 I have been unable to make clang
// produce a simple loop that doesn't end with a backward branch.  So I am going to handle ranks 0-2 here and call a
// subroutine for the rest
#define PRODX(z, n, v, init)                                                           \
    {                                                                                  \
        I nn = (n);                                                                    \
        if (nn < 3) {                                                                  \
            I temp  = (init); /* must use temp because init may depend on z */         \
            I *_zzt = (v);                                                             \
            _zzt += nn - 2;                                                            \
            z    = (I)&oneone;                                                         \
            _zzt = nn >= 1 ? _zzt : (I *)z;                                            \
            z    = nn > 1 ? (I)_zzt : z; /* set up pointers to args, and init value */ \
            z    = ((I *)z)[0];                                                        \
            if (z != 0) {                                                              \
                DPMULDZ(z, _zzt[1], z);                                                \
                if (_zzt[1] != 0) {                                                    \
                    DPMULDZ(z, temp, z);                                               \
                    if (temp != 0) { ASSERT(z != 0, EVLIMIT) }                         \
                }                                                                      \
            } /* no error if any nonzero */                                            \
        } else {                                                                       \
            DPMULDE(init, jtprod(jt, nn, v), z) RE(0)                                  \
        } /* error if error inside prod */                                             \
    }
// CPROD is to be used to create a test testing #atoms.  Because empty arrays can have cells that have too many atoms,
// we can't use PROD if we don't know that the array isn't empty or will be checked later
#define CPROD(z, x, a) PRODX(z, x, a, 1)
// PROLOG/EPILOG are the main means of memory allocation/free.  jt->tstack contains a pointer to every block that is
// allocated by GATV(i. e. all blocks). GA causes a pointer to the block to be pushed onto tstack.  PROLOG saves a copy
// of the stack pointer in _ttop, a local variable in its function.  Later, jttpop(jt,_ttop) can be executed to free
// every block that the function allocated, without requiring bookkeeping in the function.  This may be done from time
// to time in long-running definitions, to free memory [for this application it is normal to do some allocating of
// working memory, then save the tstack pointer in a local name other than _ttop, then periodically do jttpop(jt,other
// local name); such a sequence will free up all memory that was allocated after the working memory; the working memory
// itself will be freed by the eventual jttpop(jt,_ttop)]. EPILOG performs the jttpop(jt,_ttop), but it has another
// important function: that of preserving the result of a function.  Of all the blocks that were allocated by a
// function, one (possibly including its descendants) is the result of the function.  It must not be freed, so that it
// can carry the result back to the caller of this function. So, it is preserved by incrementing its usecount before the
// jttpop(jt,_ttop); then after the tpop, it is pushed back onto the tstack, indicating that it will be freed by the
// next-higher-level function.  Thus, when X calls Y inside PROLOG/EPILOG, the result of Y (which is an A block), has
// the same viability as any other GA executed in X (unless its usecount is > 1 because it was assigned elsewhere)
#define PROLOG(x) A *_ttop = jt->tnextpushp
#define EPILOGNORET(z) (jtgc(jt, z, _ttop))  // protect z and return its address
#define EPILOG(z) return EPILOGNORET(z)      // z is the result block
#define EPILOGNOVIRT(z) \
    return (jtgc(jt, z, _ttop))  // use this when the repercussions of allowing virtual result are too severe
#define EPILOGZOMB(z)                        \
    if (!jtgc3(jt, &(z), 0L, 0L, _ttop)) return 0; \
    return z;  // z is the result block.  Use this if z may contain inplaceable contents that would free prematurely
// Routines that do little except call a function that does PROLOG/EPILOG have EPILOGNULL as a placeholder
// Routines that do not return A
#define EPILOG0 jttpop(jt, _ttop)
// Routines that modify the comparison tolerance must stack it
#define PUSHCCT(value)    \
    D cctstack = jt->cct; \
    jt->cct    = (value);  // declare the stacked value where it can be popped
#define PUSHCCTIF(value, cond) \
    D cctstack = jt->cct;      \
    if (cond) jt->cct = (value);  // declare the stacked value where it can be popped
#define POPCCT jt->cct = cctstack;
// When we push, we are about to execute verbs before the last one, and an inplacement there would lead to the name's
// being assigned with invalid data.  So, we clear the inplace variables if we don't want to allow that: if the user set
// zomblevel=0, or if there is no local symbol table (which means the user is fooling around at the keyboard &
// performance is not as important as transparency)
#define CLEARZOMBIE \
    { jt->assignsym = 0; }  // Used when we know there shouldn't be an assignsym, just in case
#define PUSHZOMB                                                                    \
    L *savassignsym = jt->assignsym;                                                \
    if (savassignsym) {                                                             \
        if (((jt->asgzomblevel - 1) | ((AN(jt->locsyms) - 2))) < 0) { CLEARZOMBIE } \
    }  // test is (jt->asgzomblevel==0||AN(jt->locsyms)<2)
#define POPZOMB \
    { jt->assignsym = savassignsym; }
#define REPSGN(x) \
    ((x) >> (BW - 1))  // replicate sign bit of x to entire word (assuming x is signed type - if unsigned, just move
                       // sign to bit 0)
#define SGNTO0(x) ((UI)(x) >> (BW - 1))  // move sign bit to bit 0, clear other bits
// In the original JE many verbs returned a clone of the input, i. e. return jtca(jt,w).  We have changed these to avoid
// the clone, but we preserve the memory in case we need to go back
#define RE(exp)                               \
    {                                         \
        if (((exp), jt->jerr != 0)) return 0; \
    }
#define RESETERR \
    { jt->etxn = jt->jerr = 0; }
#define RESETERRANDMSG \
    { jt->etxn1 = jt->etxn = jt->jerr = 0; }
#define RESETRANK (jt->ranks = (RANK2T)~0)
#define RNE(exp) \
    { return jt->jerr ? 0 : (exp); }
#define RZ(exp)               \
    {                         \
        if (!(exp)) return 0; \
    }  // Return Zero

// Input is a byte.  It is replicated to all lanes of a UI
#define REPLBYTETOW(in, out) (out = (UC)(in), out |= out << 8, out |= out << 16, out |= out << 32)
// Output is pointer, Input is I/UI, count is # bytes to NOT store to output pointer (0-7).
#define STOREBYTES(out, in, n) \
    { *(UI *)(out) = (*(UI *)(out) & ~((UI) ~(I)0 >> ((n) << 3))) | ((in) & ((UI) ~(I)0 >> ((n) << 3))); }
// Input is the name of word of bytes.  Result is modified name, 1 bit per input byte, spaced like B01s, with the bit 0
// iff the corresponding input byte was all 0.  Non-boolean bits of result are garbage.
#define ZBYTESTOZBITS(b)                   \
    (b = b | ((b | (~b + VALIDBOOLEAN)) >> \
              7))  // for each byte: zero if b0 off, b7 off, and b7 turns on when you subtract 1 or 2
// to verify gah conversion #define return exp       { A retfff=(exp);  if ((retfff) && ((AT(retfff)&SPARSE &&
// AN(retfff)!=1) || (AT(retfff)&DENSE && AN(retfff)!=jtprod(jt,AR(retfff),AS(retfff)))))SEGFAULT;; return retfff; } //
// scaf
#define SBSV(x) (jt->sbsv + (I)(x))
#define SBUV(x) (jt->sbuv + (I)(x))
#define SEGFAULT (*(volatile I *)0 = 0)
#define SGN(a) ((I)(0 < (a)) - (I)(0 > (a)))
#define SMAX 65535
#define SMIN (-65536)
#define SYMHASH(h, n)            \
    (((UI)(h) * (UI)(n) >> 32) + \
     SYMLINFOSIZE)  // h is hash value for symbol; n is number of symbol chains (not including LINFO entries)
// symbol tables.   jt->locsyms is locals and jt->global is globals.  AN(table) gives #hashchains+1; if it's 1 we have
// an empty table, used to indicate that there are no locals At all times we keep the k field of locsyms as a copy of
// jt->global so that if we need it for u./v. we know what the symbol tables were.  We could remove jt->global but that
// would cost some load instructions sometimes.  AM(local table) points to the previous local table in the stack,
// looping to self at end
#define SYMSETGLOBAL(l, g) (jt->global = (g), AKGST(l) = (g))              // l is jt->locsyms, g is new global value
#define SYMRESTOREFROMLOCAL(l) (jt->locsyms = (l), jt->global = AKGST(l))  // go to stacked local l
#define SYMSETLOCAL(l) (AKGST(l) = jt->global, jt->locsyms = (l))          // change the locals to l
#define SYMPUSHLOCAL(l) (AM(l) = (I)jt->locsyms, SYMSETLOCAL(l))           // push l onto locals stack
// fa() the value when a symbol is deleted/reassigned.  If the symbol was ABANDONED, don't fa() because there was no
// ra() - but do revert 1 to 8..1
#define SYMVALFA(l)                                   \
    if (((l).flag & LWASABANDONED) != 0) {            \
        (l).flag &= ~LWASABANDONED;                   \
        AC((l).val) |= ACINPLACE & (AC((l).val) - 2); \
    } else {                                          \
        fa((l).val);                                  \
    }
#define SZA ((I)sizeof(A))
#define SZD ((I)sizeof(D))
#define SZI ((I)sizeof(I))
#define LGSZD 3  // lg(#bytes in a D)
#define SZI4 ((I)sizeof(I4))
#define SZUI4 ((I)sizeof(UI4))
#define LGSZUI4 2  // lg(#bytes in a UI4)
#define LGSZUS 1   // lg(bytes in a US)
#define SZS ((I)sizeof(S))
#define LGSZS 1  // lg (bytes in an S)

#define VAL1 '\001'
#define VAL2 '\002'
#define WITHDEBUGOFF(stmt)                                        \
    {                                                             \
        UC d                          = jt->uflags.us.cx.cx_c.db; \
        jt->uflags.us.cx.cx_c.db      = 0;                        \
        stmt jt->uflags.us.cx.cx_c.db = d;                        \
    }  // execute stmt with debug turned off

#define B0000 0x00000000
#define B0001 0x01000000
#define B0010 0x00010000
#define B0011 0x01010000
#define B0100 0x00000100
#define B0101 0x01000100
#define B0110 0x00010100
#define B0111 0x01010100
#define B1000 0x00000001
#define B1001 0x01000001
#define B1010 0x00010001
#define B1011 0x01010001
#define B1100 0x00000101
#define B1101 0x01000101
#define B1110 0x00010101
#define B1111 0x01010101
#define BS00 0x0000
#define BS01 0x0100
#define BS10 0x0001
#define BS11 0x0101

#define CACHELINESIZE 64  // size of processor cache line, in case we align to it

// flags in call to cachedmmult and blockedmmult
#define FLGCMPX 0
#define FLGCMP ((I)1 << FLGCMPX)  // arguments are complex
#define FLGAUTRIX 1
#define FLGAUTRI ((I)1 << FLGAUTRIX)  // left arg is upper-triangular
#define FLGWUTRIX 2
#define FLGWUTRI ((I)1 << FLGWUTRIX)  // left arg is upper-triangular
#define FLGINTX 3
#define FLGINT ((I)1 << FLGINTX)  // args are INT

#include "ja.h"
#include "jc.h"
#include "jtype.h"
#include "m.h"
#include "jt.h"
#include "jlib.h"
#include "je.h"
#include "jerr.h"
#include "verbs/va.h"
#include "vq.h"
#include "vx.h"
#include "vz.h"
#include "adverbs/a.h"
#include "s.h"

// CTTZ(w) counts trailing zeros in low 32 bits of w.  Result is undefined if w is 0.
// CTTZZ(w) does the same, but returns 32 if w is 0
// CTTZI(w) counts trailing zeros in an argument of type I (32 or 64 bits depending on architecture)

// CTTZ uses the single-instruction count-trailing-zeros instruction to convert
// a 1-bit mask to a bit number.  If this instruction is available on your architecture/compiler,
// you should use the compiler intrinsic to create this instruction, and define the CTTZ and CTTZZ
// macros to use the instruction inline.  It is used enough in the J engine to make a difference.

// If CTTZ is not defined, the default routine defined in u.c will be used.  You can look there
// for the complete spec for CTTZ and CTTZZ.

#ifndef __forceinline
#define __forceinline inline __attribute__((always_inline))
#endif

#ifdef __GNUC__
#define CTTZ(w) __builtin_ctzl((UINT)(w))
#define CTTZI(w) __builtin_ctzll((UI)(w))
#define CTLZI(w, out) (out = (63 - __builtin_clzll((UI)(w))))
#define CTTZZ(w) ((w) == 0 ? 32 : CTTZ(w))
#endif

// For older processors, TZCNT is executed as BSF, which differs from TZCNT in that it does not
// set the Z flag if the result is 0.  The optimizer sometimes turns a switch into tests rather than a branch
// table, and it expects TZCNT to set the Z flag properly.  We use CTTZNOFLAG to set it right
#define CTTZNOFLAG(w) (CTTZ(w) & 31)

#ifdef __GNUC__
#ifndef offsetof
#define offsetof(TYPE, MEMBER) __builtin_offsetof(TYPE, MEMBER)
#endif
#endif

// Insert other compilers/architectures here

// Insert CTLZ here if CTTZ is not available

// If your machine supports count-leading-zeros but not count-trailing-zeros, you can define the macros
// CTLZ/CTLZI, which returns the number of high-order zeros in the low 32 bits of its argument, and the following
// CTTZ will be defined:
#if defined(CTLZ) && !defined(CTTZ)
#define CTTZ(w) (31 - CTLZ((w) & -(w)))
#define CTTZI(w) (63 - CTLZI((w) & -(w)))
#define CTTZZ(w) (0xffffffff & (w) ? CTTZ(w) : 32)
#endif

// If CTTZ is not defined, the following code will use the default from u.c:
#if !defined(CTTZ)
extern I CTTZ(I);
extern I CTTZI(I);
extern I CTTZZ(I);
#endif
#if !defined(CTLZI)
extern I CTLZI_(UI, UI4 *);
#define CTLZI(in, out) CTLZI_(in, &(out))
#endif

#include <fenv.h>
// bug clang isnan(x) set NaN flag if x is NaN

#define _isnan isnan

#define _SW_INVALID FE_INVALID

static inline UINT
_clearfp(void) {
    int r = fetestexcept(FE_ALL_EXCEPT);
    feclearexcept(FE_ALL_EXCEPT);
    return r;
}

#define DPMUL(x, y, z, s)           \
    {                               \
        I _l, _x = (x), _y = (y);   \
        D _d;                       \
        _l = _x * _y;               \
        _d = (D)_x * (D)_y - (D)_l; \
        *z = _l;                    \
        _d = ABS(_d);               \
        if (_d > 1e8) s             \
    }  // *z may be the same as x or y
#define DPMULD(x, y, z, s)          \
    {                               \
        I _l, _x = (x), _y = (y);   \
        D _d;                       \
        _l = _x * _y;               \
        _d = (D)_x * (D)_y - (D)_l; \
        z  = _l;                    \
        _d = ABS(_d);               \
        if (_d > 1e8) s             \
    }
#define DPMULDZ(x, y, z) DPMULD(x, y, z, z = 0;)
#define DPMULDE(x, y, z) DPMULD(x, y, z, ASSERT(0, EVLIMIT))

// define single+double-precision integer add

#ifndef SPDPADD  // default version for systems without addcarry
#define SPDPADD(addend, sumlo, sumhi) \
    sumlo += addend;                  \
    sumhi += (addend > sumlo);
#endif
// end of addition builtins

// Create (x&y) where x and y are signed, so we can test for overflow.
#define XANDY(x, y) ((I)((UI)(x) & (UI)(y)))

#ifdef __cplusplus
}
#endif
