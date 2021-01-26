/* Copyright 1990-2014, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Global Definitions                                                      */

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

// ms vc++ defined _MSC_VER but clang-cl also defined _MSC_VER
// clang-cl doesn't emulate ms vc++ good enough
// and it breaks program logic previously guarded by _MSC_VER
// MMSC_VER means the real ms vc++ excluding clang-cl
// use MMSC_VER instead of _MSC_VER throughout JE source
#if defined(_MSC_VER) && !defined(__clang__)
#undef MMSC_VER
#define MMSC_VER
#else
#undef MMSC_VER
#endif

#ifndef EMU_AVX
#define EMU_AVX 0
#endif
#if defined(MMSC_VER)
#undef EMU_AVX
#define EMU_AVX 0
#endif
#undef EMU_AVX2
#define EMU_AVX2 0

/* msvc does not define __SSE2__ */
#if !defined(__SSE2__)
#if defined(MMSC_VER)
#if (defined(_M_AMD64) || defined(_M_X64))
#define __SSE2__ 1
#elif _M_IX86_FP==2
#define __SSE2__ 1
#endif
#endif
#endif

// for debugging
#define NANTEST0        (fetestexcept(FE_INVALID))  // test but does not clear
#define dump_m128i(a,x) {__m128i _b=x;fprintf(stderr,"%s %li %li %li %li \n", a, ((long*)(&_b))[0], ((long*)(&_b))[1], ((long*)(&_b))[2], ((long*)(&_b))[3]);}
#define dump_m128i64(a,x) {__m128i _b=x;fprintf(stderr,"%s %lli %lli \n", a, ((long long*)(&_b))[0], ((long long*)(&_b))[1]);}
#define dump_m256i(a,x) {__m256i _b=x;fprintf(stderr,"%s %lli %lli %lli %lli \n", a, ((long long*)(&_b))[0], ((long long*)(&_b))[1], ((long long*)(&_b))[2], ((long long*)(&_b))[3]);}
#define dump_m256d(a,x) {__m256d _b=x;fprintf(stderr,"%s %f %f %f %f \n", a, ((double*)(&_b))[0], ((double*)(&_b))[1], ((double*)(&_b))[2], ((double*)(&_b))[3]);}
#define dump_m128d(a,x) {__m128d _b=x;fprintf(stderr,"%s %f %f \n", a, ((double*)(&_b))[0], ((double*)(&_b))[1]);}

#if defined(__i386__) || defined(__x86_64__) || defined(_M_X64) || defined(_M_IX86)
#ifndef C_AVX2
#define C_AVX2 0
#endif

#if (defined(_MSC_VER))
#include <intrin.h>
#endif

#undef EMU_AVX2
#define EMU_AVX2 1
#include <stdint.h>
#include <string.h>
#undef EMU_AVX
#define EMU_AVX 0

#elif defined(__SSE2__)
#if EMU_AVX
#undef EMU_AVX2
#define EMU_AVX2 1   // test avx2 emulation
#include <stdint.h>
#include <string.h>
#include "avxintrin-emu.h"
#else
#include <emmintrin.h>
#endif
#define _CMP_EQ          0
#define _CMP_LT          1
#define _CMP_LE          2
#define _CMP_UNORD       3
#define _CMP_NEQ         4
#define _CMP_NLT         5
#define _CMP_NLE         6
#define _CMP_ORD         7
#undef _CMP_EQ_OQ
#undef _CMP_GE_OQ
#undef _CMP_GT_OQ
#undef _CMP_LE_OQ
#undef _CMP_LT_OQ
#undef _CMP_NEQ_OQ
#define _CMP_EQ_OQ _CMP_EQ
#define _CMP_GE_OQ _CMP_NLT
#define _CMP_GT_OQ _CMP_NLE
#define _CMP_LE_OQ _CMP_LE
#define _CMP_LT_OQ _CMP_LT
#define _CMP_NEQ_OQ _CMP_NEQ
#endif

#if defined(__aarch64__)||defined(_M_ARM64)
#if EMU_AVX
#undef EMU_AVX2
#define EMU_AVX2 1   // test avx2 emulation
#include <stdint.h>
#include <string.h>
#include "sse2neon.h"
#include "sse2neon2.h"
#include "avxintrin-neon.h"
#else
#include <arm_neon.h>
#endif
#endif

#if defined(__arm__)
#if defined(__ARM_NEON)
#include <arm_neon.h>
typedef double float64x2_t __attribute__ ((vector_size (16)));
#else
#include <stdint.h>
typedef int64_t int64x2_t __attribute__ ((vector_size (16)));
typedef double float64x2_t __attribute__ ((vector_size (16)));
#endif
#endif

#undef VOIDARG
#define VOIDARG

#if SLEEF
#include "../sleef/include/sleef.h"
#endif

#if defined(_OPENMP)
#include <omp.h>
#else
typedef int omp_int_t;
static inline omp_int_t omp_get_thread_num() { return 0;}
static inline omp_int_t omp_get_max_threads() { return 1;}
#endif

#ifndef SYS // include js.h only once - dtoa.c
#include "js.h"
#endif

// If you are porting to a new compiler or architecture, see the bottom of this file
// for instructions on defining the CTTZ macros

#if (SYS & SYS_PCWIN)
#define HEAPCHECK  heapcheck()
#else
#define HEAPCHECK
#endif

#if (SYS & SYS_ATARIST)
#define __NO_INLINE__           1
#endif

#if (SYS & SYS_UNIX - SYS_SGI)
#include <memory.h>
#include <sys/types.h>
#endif

// likely/unlikely support
#if defined(__clang__) || defined(__GNUC__)
#ifndef likely
#define likely(x) __builtin_expect((x),1)
#endif
#ifndef unlikely
#define unlikely(x) __builtin_expect((x),0)
#endif
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

#if 1
#include <stdint.h>
#include <float.h>
#include <limits.h>
#define link unused_syscall_link
#define qdiv unused_netbsd_qdiv
#include <stdlib.h>
#undef link
#undef qdiv
#else
#define const /*nothing*/   /* blame rx.h */
#endif

#include <errno.h>
#include <stdio.h>
#include <math.h>
#include <string.h>  




#if defined(__aarch32__)||defined(__arm__)||defined(_M_ARM)||defined(__aarch64__)||defined(_M_ARM64)
#ifndef __ARM_FEATURE_UNALIGNED
#define ALIGNEDMEM
#endif
#endif

#define IMAX            9223372036854775807LL
#define IMIN            (~9223372036854775807LL)   /* ANSI C LONG_MIN is  -LONG_MAX */
#define FLIMAX          9223372036854775296.     // largest FL value that can be converted to I
#define FLIMIN          ((D)IMIN)  // smallest FL value that can be converted to I
#define FMTI            "%lli"
#define FMTI02          "%02lli"
#define FMTI04          "%04lli"
#define FMTI05          "%05lli"

#define strtoI          strtoll

#define NEGATIVE0       0x8000000000000000LL   // IEEE -0 (double precision)

#define C4MAX           0xffffffffUL
#define C4MIN           0L

#if (SYS & SYS_AMIGA)
#define XINF            "\177\377\000\000\000\000\000\000"
#define XNAN            "\177\361\000\000\000\000\000\000"
#endif

#if (SYS & SYS_ARCHIMEDES)
#define XINF            "\000\000\360\177\000\000\000\000"
#define XNAN            "\000\000\370\377\000\000\000\000"
#endif

#if (SYS & SYS_DEC5500)
#define XINF            "\000\000\000\000\000\000\360\177"
#define XNAN            "\000\000\000\000\000\000\370\377"
#endif

#if (SYS & SYS_MACINTOSH)
/* for old versions of ThinkC */
/* #define XINF         "\177\377\000\000\000\000\000\000\000\000\000\000" */
/* #define XNAN         "\377\377\100\000\100\000\000\000\000\000\000\000" */
/* for ThinkC 7.0 or later */
#define XINF            "\177\377\177\377\000\000\000\000\000\000\000\000"
#define XNAN            "\377\377\377\377\100\000\000\000\000\000\000\000"
#endif

#if (SYS & SYS_SUN4+SYS_SUNSOL2)
#define XINF            "\177\360\000\000\000\000\000\000"
#define XNAN            "\177\377\377\377\377\377\377\377"
#endif

#if (SYS & SYS_VAX)
#define XINF            "\377\177\377\377\377\377\377\377"
#define XNAN            "\377\177\377\377\377\377\377\376" /* not right */
#endif

#ifndef XINF
#define XINF            "\000\000\000\000\000\000\360\177"
#define XNAN            "\000\000\000\000\000\000\370\377"
#endif

#ifndef XINF
#define XINF            "\177\360\000\000\000\000\000\000"
#define XNAN            "\177\370\000\000\000\000\000\000"
#endif


#ifndef PI
#define PI              ((D)3.14159265358979323846)
#endif
#define P2              ((D)6.28318530717958647693)
#ifndef OVERFLOW
#define OVERFLOW        ((D)8.988465674311578e307)
#endif
#ifndef UNDERFLOW
#define UNDERFLOW       ((D)4.450147717014403e-308)
#endif
// RESTRICT causes the compiler to generate better code by assuming no overlap of regions pointed to by pointers
// We use RESTRICT for routines that operate in-place on an argument.  This is strictly speaking a violation of the rule,
// but normally something like *z = *x + *y will not cause trouble because there is no reason to refetch an input after
// the result has been written.  On 32-bit machines, registers are so short that sometimes the compilers refetch an input
// after writing to *z, so we don't turn RESTRICT on for 32-bit
#if defined(MMSC_VER)
// RESTRICT is an attribute of a pointer, and indicates that no other pointer points to the same area
#define RESTRICT __restrict
// RESTRICTF is an attribute of a function, and indicates that the object returned by the function is not aliased with any other object
#define RESTRICTF __declspec(restrict)
#define PREFETCH(x) _mm_prefetch((x),_MM_HINT_T0)
#define PREFETCH2(x) _mm_prefetch((x),_MM_HINT_T1)   // prefetch into L2 cache but not L1
#endif
#ifdef __GNUC__
#define RESTRICT __restrict
// No RESTRICTF on GCC
#define PREFETCH(x) __builtin_prefetch(x)
#define PREFETCH2(x) __builtin_prefetch((x),0,2)   // prefetch into L2 cache but not L1
#endif

#ifdef __MINGW32__
#ifndef _SW_INVALID
#define _SW_INVALID    0x00000010 /* invalid */
#endif
#ifndef _EM_ZERODIVIDE
#define _EM_ZERODIVIDE  0x00000008
#endif
#define EM_INVALID    _SW_INVALID
#define EM_ZERODIVIDE _EM_ZERODIVIDE
#if defined(__STRICT_ANSI__)
extern int __cdecl _isnan (double);
extern unsigned int __cdecl _clearfp (void);
#endif
#ifndef _MAX_PATH
#define _MAX_PATH  (260)
#endif
#endif

#ifdef __GNUC__
#define RESTRICTI // __restrict  don't take chances
#endif

#ifndef RESTRICT
#define RESTRICT
#endif
#ifndef RESTRICTF
#define RESTRICTF
#endif
#ifndef RESTRICTI
#define RESTRICTI
#endif
// If PREFETCH is not defined, we won't generate prefetch instrs

// If the user switch C_NOMULTINTRINSIC is defined, suppress using it
#ifdef C_NOMULTINTRINSIC
#define C_USEMULTINTRINSIC 0
#else
#define C_USEMULTINTRINSIC 1
#endif

// disable C_USEMULTINTRINSIC if un-available
#if C_USEMULTINTRINSIC
#if !defined(MMSC_VER)
#if defined(__clang__)
#if !__has_builtin(__builtin_smull_overflow)
#undef C_USEMULTINTRINSIC
#define C_USEMULTINTRINSIC 0
#endif
#elif __GNUC__ < 5
#undef C_USEMULTINTRINSIC
#define C_USEMULTINTRINSIC 0
#endif
#endif
#endif

#define NALP            256             /* size of alphabet                */
#define NETX            2000            /* size of error display buffer    */
#define NPP             20              /* max value for quad pp           */
#define RMAXX           16              // number of bits in rank
#define RMAX            ((((I)1)<<RMAXX)-1)   // max rank
#define NPATH           1024            /* max length for path names,      */
                                        /* including trailing 0 byte       */

#define NTSTACK         16384L          // number of BYTES in an allocated block of tstack - pointers to allocated blocks - allocation is bigger to leave this many bytes on boundary
#define NTSTACKBLOCK    2048            // boundary for beginning of stack block

// Sizes for the internal stacks.  The goal here is to detect a runaway recursion before it creates a segfault.  This cannot
// be done with precision because we don't know how much C stack we have, or how much is used by a recursion (and anyway it depends on
// what J functions are running).
// There are two limits: maximum depth of J functions, and maximum depth of named functions.  The named-function stack is intelligent
// and stacks only when there is a locale change or deletion; it almost never limits unless locatives are used to an extreme degree.
// The depth of J function calls will probably limit stack use.
// #define NFDEP           4000             // fn call depth - for debug builds, must be small (<0xa00) to avoid stack overflow, even smaller for non-AVX
// #define NFCALL          (NFDEP/10)      // call depth for named calls, not important
// increase OS stack limit instead of restricting NFDEP/NFCALL
#define NFDEP           2000L  // 4000             // (obsolete) fn call depth - for debug builds, must be small (<0xa00) to avoid stack overflow, even smaller for non-AVX


// NEW WAY
// The named-call stack is used only when there is a locative, EXCEPT that after a call to 18!:4 it is used until the function calling 18!:4 returns.
// Since startup calls 18!:4 without a name, we have to allow for the possibility of deep recursion in the name stack.  Normally only a little of the stack is used
#define NFCALL          (NFDEP/2)      // call depth for named calls, not important
// Now we are trying to watch the C stack directly
#define CSTACKSIZE      (12000000)  // size we allocate in the calling function
#define CSTACKRESERVE   100000  // amount we allow for slop before we sample the stackpointer, and after the last check
#define USECSTACK       1   // 0 to go back to counting J recursions    

// start and length for the stored vector of ascending integers
#define IOTAVECBEGIN (-20)
#define IOTAVECLEN 400

// modes for indexofsub()
#define IIOPMSK         0xf     // operation bits
#define IIDOT           0        // IIDOT and IICO must be 0-1
#define IICO            1
#define INUBSV          2
#define INUB            3
#define ILESS           4
#define INUBI           5
#define IEPS            6
// the I...EPS values below are wired into the function table at the end of vcompsc.c
#define II0EPS          7  // this must come first; other base on it
#define II1EPS          8
#define IJ0EPS          9
#define IJ1EPS          10
#define ISUMEPS         11
#define IANYEPS         12
#define IALLEPS         13
#define IIFBEPS         14
#define IFORKEY         15  // special key support: like i.~, but add # values mapped to the index
#define IIMODFIELD      0x70  // bits used to indicate processing options
#define IIMODPACK       0x10  // modifier for type.  (small-range search except i./i:) In IIDOT/IICO, indicates reflexive application.  In others, indicates that the
                              // bitmask should be stored as packed bits rather than bytes

#define IIMODREFLEXX    4
#define IIMODREFLEX     (((I)1)<<IIMODREFLEXX)  // (small-range i. and i:) this is i.~/i:~ (hashing) this is i.~/i:~/~./~:/I.@:~.
#define IIMODFULL       0x20  // (small-range search) indicates that the min/max values cover the entire range of possible inputs, so no range checking is required.  Always set for hashing
#define IIMODBASE0      0x40  // set in small-range i./i: (which never use BITS) to indicate that the hashtable starts at index 0 and has m in the place of unused indexes.  Set in hashing always, with same meaning
#define IIMODBITS       0x80  // set if the hash field stores bits rather than indexes.  Used only for small-range and not i./i:.  IIMODPACK qualifies this, indicating that the bits are packed
#define IIMODFORCE0X    8
#define IIMODFORCE0     (((I)1)<<IIMODFORCE0X)  // set to REQUIRE a (non-bit) allocation to reset to offset 0 and clear
#define IPHCALCX        9
#define IPHCALC         (((I)1)<<IPHCALCX)   // set when we are calculating a prehashed table
#define IINOTALLOCATED  0x400  // internal flag, set when the block has not been allocated
#define IIOREPSX        11
#define IIOREPS         (((I)1)<<IIOREPSX)  // internal flag, set if mode is i./i:/e./key, but not if prehashing
#define IREVERSED       0x1000   // set if we have decided to reverse the hash in a small-range situation
#define IPHOFFSET       0x2000              /* offset for prehashed versions - set when we are using a prehashed table   */
#define IPHIDOT         (IPHOFFSET+IIDOT)
#define IPHICO          (IPHOFFSET+IICO)
#define IPHEPS          (IPHOFFSET+IEPS)
#define IPHI0EPS        (IPHOFFSET+II0EPS)
#define IPHI1EPS        (IPHOFFSET+II1EPS)
#define IPHJ0EPS        (IPHOFFSET+IJ0EPS)
#define IPHJ1EPS        (IPHOFFSET+IJ1EPS)
#define IPHSUMEPS       (IPHOFFSET+ISUMEPS)
#define IPHANYEPS       (IPHOFFSET+IANYEPS)
#define IPHALLEPS       (IPHOFFSET+IALLEPS)
#define IPHIFBEPS       (IPHOFFSET+IIFBEPS)

#define jceil(x) ceil(x)
#define jfloor(x) floor(x)
#define jround(x) floor(0.5+(x))  // for paranoid compatibility with earlier versions


#define BB    8            // # bits in a byte 
#define LGBB  3            // lg(BB)
#define BW    64           // # bits in a word 
#define LGSZI 3            // lg(#bytes in an I)
#define LGBW (LGSZI+LGBB)  // lg (# bits in a word)

// nominal cache sizes for current processors
#define L1CACHESIZE (((I)1)<<15)
#define L2CACHESIZE (((I)1)<<18)
#define L3CACHESIZE (((I)1)<<22)

#define TOOMANYATOMS 0xFFFFFFFFFFFFLL  // more atoms than this is considered overflow (64-bit).  i.-family can't handle more than 2G cells in array.

// Tuning options for cip.c
// tuned for linux
#define IGEMM_THRES  (200*200*200)   // when m*n*p less than this use cached; when higher, use BLAS
#define DGEMM_THRES  (200*200*200)   // when m*n*p less than this use cached; when higher, use BLAS   _1 means 'never'
#define ZGEMM_THRES  (60*60*60)      // when m*n*p less than this use cached; when higher, use BLAS  
#define DCACHED_THRES  (64*64*64)    // when m*n*p less than this use blocked; when higher, use cached


// Debugging options

// Use MEMAUDIT to sniff out errant memory alloc/free
#define MEMAUDIT 0x00     // Bitmask for memory audits: 1=check headers 2=full audit of tpush/tpop 4=write garbage to memory before freeing it 8=write garbage to memory after getting it
                     // 16=audit freelist at every alloc/free (starting after you have run 6!:5 (1) to turn it on)
 // 13 (0xD) will verify that there are no blocks being used after they are freed, or freed prematurely.  If you get a wild free, turn on bit 0x2
 // 2 will detect double-frees before they happen, at the time of the erroneous tpush

#define AUDITEXECRESULTS 0  // When set, we go through all execution results to verify recursive and virtual bits are OK
#define FORCEVIRTUALINPUTS 0  // When 1 set, we make all non-inplaceable noun inputs to executions VIRTUAL.  Tests should still run
                           // When 2 set, make all outputs from RETF() virtual.  Tests for inplacing will fail; that's OK if nothing crashes
// set FINDNULLRET to trap when a routine returns 0 without having set an error message
#define FINDNULLRET 0


#if BW==64
#define ALTBYTES 0x00ff00ff00ff00ffLL
// t has totals per byte-lane, result combines them into single total.  t must be an lvalue
#define ADDBYTESINI(t) (t=(t&ALTBYTES)+((t>>8)&ALTBYTES), t = (t>>32) + t, t = (t>>16) + t, t&=0xffff) // sig in 01ff01ff01ff01ff, then xxxxxxxx03ff03ff, then xxxxxxxxxxxx07ff, then 00000000000007ff
#define VALIDBOOLEAN 0x0101010101010101LL   // valid bits in a Boolean
#else
#define ALTBYTES 0x00ff00ffLL
#define ADDBYTESINI(t) (t=(t&ALTBYTES)+((t>>8)&ALTBYTES), t = (t>>16) + t, t&=0xffff) // sig in 01ff01ff, then xxxx03ff, then 000003ff
#define VALIDBOOLEAN 0x01010101   // valid bits in a Boolean
#endif

// macros for bit testing
#define SGNIF(v,bitno) ((I)(v)<<(BW-1-(bitno)))  // Sets sign bit if the numbered bit is set
#define SGNIFNOT(v,bitno) (~SGNIF((v),(bitno)))  // Clears sign bit if the numbered bit is set

#define A0              0   // a nonexistent A-block
#define ABS(a)          (0<=(a)?(a):-(a))
#define ASSERT(b,e)     {if(unlikely(!(b))){jsignal(e); R 0;}}
// version for debugging
// #define ASSERT(b,e)     {if(unlikely(!(b))){fprintf(stderr,"error code: %i : file %s line %d\n",(int)(e),__FILE__,__LINE__); jsignal(e); R 0;}}
#define ASSERTD(b,s)    {if(unlikely(!(b))){jsigd((s)); R 0;}}
#define ASSERTMTV(w)    {ARGCHK1(w); ASSERT(1==AR(w),EVRANK); ASSERT(!AN(w),EVLENGTH);}
#define ASSERTN(b,e,nm) {if(unlikely(!(b))){jt->curname=(nm); jsignal(e); R 0;}}  // set name for display (only if error)
#define ASSERTSYS(b,s)  {if(unlikely(!(b))){fprintf(stderr,"system error: %s : file %s line %d\n",s,__FILE__,__LINE__); jsignal(EVSYSTEM); jtwri(jt,MTYOSYS,"",(I)strlen(s),s); R 0;}}
#define ASSERTW(b,e)    {if(unlikely(!(b))){if((e)<=NEVM)jsignal(e); else jt->jerr=(e); R;}}
#define ASSERTWR(c,e)   {if(unlikely(!(c))){R e;}}
// verify that shapes *x and *y match for l axes using AVX for rank<5, memcmp otherwise

#define ASSERTAGREE(x,y,l) \
 {I *aaa=(x), *aab=(y); I aai=(l); \
  if(likely(aai<=2)){ \
   aai-=1; aaa=(aai<0)?(I*)&validitymask[1]:aaa; aab=(aai<0)?(I*)&validitymask[1]:aab; \
   ASSERT(((aaa[0]^aab[0])+(aaa[aai]^aab[aai]))==0,EVLENGTH); \
  }else{ASSERT(!memcmp(aaa,aab,aai<<LGSZI),EVLENGTH)} \
 }
#define TESTDISAGREE(r,x,y,l) \
 {I *aaa=(x), *aab=(y); I aai=(l); \
  if(likely(aai<=2)){ \
   aai-=1; aaa=(aai<0)?(I*)&validitymask[1]:aaa; aab=(aai<0)?(I*)&validitymask[1]:aab; \
   r=((aaa[0]^aab[0])+(aaa[aai]^aab[aai]))!=0;  \
  }else{r=memcmp(aaa,aab,aai<<LGSZI)!=0;} \
 }

#define ASSERTAGREESEGFAULT (x,y,l) {I *aaa=(x), *aab=(y), aai=(l)-1; do{aab=aai<0?aaa:aab; if(aaa[aai]!=aab[aai])SEGFAULT; --aai; aab=aai<0?aaa:aab; if(aaa[aai]!=aab[aai])SEGFAULT; --aai;}while(aai>=0); }
// BETWEENx requires that lo be <= hi
#define BETWEENC(x,lo,hi) ((UI)((x)-(lo))<=(UI)((hi)-(lo)))   // x is in [lo,hi]
#define BETWEENO(x,lo,hi) ((UI)((x)-(lo))<(UI)((hi)-(lo)))   // x is in [lo,hi)
#define BMK(x) (1LL<<(x))  // bit number x
// test for equality of 2 8-bit values simultaneously
#define BOTHEQ8(x,y,X,Y) ( ((US)(C)(x)<<8)+(US)(C)(y) == ((US)(C)(X)<<8)+(US)(C)(Y) )
#define CALL1(f,w,fs)   ((f)(jt,    (w),(A)(fs)))
#define CALL2(f,a,w,fs) ((f)(jt,(a),(w),(A)(fs)))
#define CALL1IP(f,w,fs)   ((f)(jtinplace,    (w),(A)(fs)))
#define CALL2IP(f,a,w,fs) ((f)(jtinplace,(a),(w),(A)(fs)))
#define RETARG(z)       (z)   // These places were ca(z) in the original JE
#define CALLSTACKRESET  {jt->callstacknext=0; jt->uflags.us.uq.uq_c.pmctrbstk &= ~PMCTRBSTKREQD;} // establish initial conditions for things that might not get processed off the stack.  The last things stacked may never be popped
#define MODESRESET      {jt->xmode=XMEXACT;}  // anything that might get left in a bad state and should be reset on return to immediate mode
// see if a character matches one of many.  Example in ai.c
// create mask for the bit, if any, in word w for value.  Reverse order: 0=MSB
#define CCM(w,value) ((I)(((value)>>LGBW)==(w)?1LL<<(BW-1-((value)&BW-1)):0))
// user creates #define nm##values(w) CCM(w,value0)+CCM(w,value1)+...
// create 8 mask values, nm##0 to nm##7, for the given values.
#define CCMWD(nm,wd) I nm##wd=nm##values(wd);
#define CCMWDS(nm) CCMWD(nm,0); CCMWD(nm,1); CCMWD(nm,2); CCMWD(nm,3); CCMWD(nm,4); CCMWD(nm,5); CCMWD(nm,6); CCMWD(nm,7);
// Create the comparand for the test value, it the given cand name
#define  CCMCAND1(nm,cand,tval,w) cand=nm##w&&((UI)tval>>LGBW)==w?nm##w:cand;
#define CCMCAND(nm,cand,tval) I cand=0; CCMCAND1(nm,cand,tval,0); CCMCAND1(nm,cand,tval,1); CCMCAND1(nm,cand,tval,2); CCMCAND1(nm,cand,tval,3); CCMCAND1(nm,cand,tval,4); CCMCAND1(nm,cand,tval,5); CCMCAND1(nm,cand,tval,6); CCMCAND1(nm,cand,tval,7)
// set the sign bit to the selected bit of the mask
#define CCMSGN(cand,tval) (cand<<(tval&(BW-1)))   // set sign bit if value found
#define CCMTST(cand,tval) (cand&(1LL<<(~tval&(BW-1))))  // test true is value found
#define DF1(f)          A f(J jt,    A w,A self)
#define DF2(f)          A f(J jt,A a,A w,A self)
#define DO(n,stm)       {I i=0,_n=(n); for(;i<_n;i++){stm}}  // i runs from 0 to n-1
#define DP(n,stm)       {I i=-(n);    for(;i<0;++i){stm}}   // i runs from -n to -1 (faster than DO)
#define DQ(n,stm)       {I i=(I)(n)-1;    for(;i>=0;--i){stm}}  // i runs from n-1 downto 0 (fastest when you don't need i)
#define DOU(n,stm)      {I i=0,_n=(n); do{stm}while(++i<_n);}  // i runs from 0 to n-1, always at least once
#define DPU(n,stm)      {I i=-(n);    do{stm}while(++i<0);}   // i runs from -n to -1 (faster than DO), always at least once
#define DQU(n,stm)      {I i=(I)(n)-1;  do{stm}while(--i>=0);}  // i runs from n-1 downto 0, always at least once
// C suffix indicates that the count is one's complement
#define DOC(n,stm)       {I i=0,_n=~(n); for(;i<_n;i++){stm}}  // i runs from 0 to n-1
#define DPC(n,stm)       {I i=(n)+1;    for(;i<0;++i){stm}}   // i runs from -n to -1 (faster than DO)
#define DQC(n,stm)       {I i=-2-(I)(n);    for(;i>=0;--i){stm}}  // i runs from n-1 downto 0 (fastest when you don't need i)
#define DOUC(n,stm)      {I i=0,_n=~(n); do{stm}while(++i<_n);}  // i runs from 0 to n-1, always at least once
#define DPUC(n,stm)      {I i=(n)+1;    do{stm}while(++i<0);}   // i runs from -n to -1 (faster than DO), always at least once
#define DQUC(n,stm)      {I i=-2-(I)(n);  do{stm}while(--i>=0);}  // i runs from n-1 downto 0, always at least once
#define ds(c)            (A)&primtab[(UC)(c)]
#define UNUSED_VALUE        ds(CRIGHT)  // harmless value to use for self in calls to rank loops
// see if value of x is the atom v.  Do INT/B01/FL here, subroutine for exotic cases
#define EQINTATOM(x,v)  ( (AR(x)==0) && ((AT(x)&(INT+B01)) ? (((*IAV0(x))&(((AT(x)&B01)<<8)-1))==(v)) : (AT(x)&FL) ? *DAV0(x)==(D)(v) : 0!=equ(num(v),x))  )
// define fs block used in every/every2.  It is the self for the f in f&.>, and contains only function pointers, an optional param in AK, and the flag field
#define EVERYFS(name,f0,f1,akparm,flg) PRIM name={{akparm,0,0,0,0,0,0},{.primvb={.valencefns={f0,f1},.flag=flg}}};

#if USECSTACK
#define FDEPDEC(d)
#define FDEPINC(d)
#define STACKCHKOFL {D stackpos; ASSERT((uintptr_t)&stackpos>=jt->cstackmin,EVSTACK);}
#else  // old style counting J recursion levels
#define FDEPDEC(d)      jt->fdepi-=(I4)(d)  // can be used in conditional expressions
#define FDEPINC(d)      {ASSERT(jt->fdepn>=jt->fdepi+(I4)(d),EVSTACK); jt->fdepi+=(I4)(d);}
#define STACKVERIFY
#endif
#define FCONS(x)        fdef(0,CFCONS,VERB,jtnum1,jtnum2,0L,0L,(x),VFLAGNONE, RMAX,RMAX,RMAX)
// fuzzy-equal is used for tolerant comparisons not related to jt->cct; for example testing whether x in x { y is an integer
#define FUZZ            0.000000000000056843418860808015   // tolerance
// FFEQ/FFIEQ (fixed fuzz) are used where we know for sure the test should be tolerant
#define FFEQ(u,v)        (ABS((u)-(v))<=FUZZ*MAX(ABS(u),ABS(v)))
#define FFIEQ(u,v)       (ABS((u)-(v))<=FUZZ*ABS(v))  // used when v is known to be exact integer.  It's close enough, maybe ULP too small on the high end
#define F1(f)           A f(J jt,    A w)
#define F2(f)           A f(J jt,A a,A w)
#define FPREF           
#define F1PREF          FPREF
#define F2PREF          FPREF
#define FPREFIP         J jtinplace=jt; jt=(J)(intptr_t)((I)jt&~JTFLAGMSK)  // turn off all flag bits in jt, leave them in jtinplace
#define F1PREFIP        FPREFIP
#define F2PREFIP        FPREFIP
#define F1RANK(m,f,self)    {ARGCHK1(w); if(unlikely(m<AR(w)))if(m==0)R rank1ex0(w,(A)self,f);else R rank1ex(  w,(A)self,(I)m,     f);}  // if there is more than one cell, run rank1ex on them.  m=monad rank, f=function to call for monad cell.  Fall through otherwise
#define F2RANKcommon(l,r,f,self,extra)  {ARGCHK2(a,w); extra if(unlikely((I)((l-AR(a))|(r-AR(w)))<0))if((l|r)==0)R rank2ex0(a,w,(A)self,f);else{I lr=MIN((I)l,AR(a)); I rr=MIN((I)r,AR(w)); R rank2ex(a,w,(A)self,lr,rr,lr,rr,f);}}  // If there is more than one cell, run rank2ex on them.  l,r=dyad ranks, f=function to call for dyad cell
#define F2RANK(l,r,f,self)  F2RANKcommon(l,r,f,self,)
// same, but used when the function may pull an address from w.  In that case, we have to turn pristine off since there may be duplicates in the result
#define F2RANKW(l,r,f,self) F2RANKcommon(l,r,f,self,PRISTCLR(w))
#define F1RANKIP(m,f,self)    {RZ(   w); if(unlikely(m<AR(w)))R jtrank1ex(jtinplpace,  w,(A)self,(I)m,     f);}  // if there is more than one cell, run rank1ex on them.  m=monad rank, f=function to call for monad cell
#define F2RANKIP(l,r,f,self)  {ARGCHK2(a,w); if(unlikely((I)((l-AR(a))|(r-AR(w)))<0)){I lr=MIN((I)l,AR(a)); I rr=MIN((I)r,AR(w)); R jtrank2ex(jtinplace,a,w,(A)self,REX2R(lr,rr,lr,rr),f);}}  // If there is more than one cell, run rank2ex on them.  l,r=dyad ranks, f=function to call for dyad cell
// get # of things of size s, rank r to allocate so as to have an odd number of them at least n, after discarding w items of waste.  Try to fill up a full buffer 
#define FULLHASHSIZE(n,s,r,w,z) {UI4 zzz;  CTLZI((((n)|1)+(w))*(s) + AKXR(r) - 1,zzz); z = ((((I)1<<(zzz+1)) - AKXR(r)) / (s) - 1) | (1&~(w)); }
// Memory-allocation macros
// Size-of-block calculations.  VSZ when size is constant or variable.  Byte counts are (total length including header)-1
// Because the Boolean dyads read beyond the end of the byte area (up to 1 extra word), we add one SZI-1 for islast (which includes B01), rather than adding 1
#define ALLOBYTESVSZ(atoms,rank,size,islast,isname)      ( (((rank)*SZI  + ((islast)? (isname)?(NORMAH*SZI+sizeof(NM)+SZI-1-1):(NORMAH*SZI+SZI-1-1) : (NORMAH*SZI-1)) + (atoms)*(size)))  )  // # bytes to allocate allowing 1 I for string pad - include mem hdr - minus 1
// here when size is constant.  The number of bytes, rounded up with overhead added, must not exceed 2^(PMINL+4)
#define ALLOBYTES(atoms,rank,size,islast,isname)      ((size&(SZI-1))?ALLOBYTESVSZ(atoms,rank,size,islast,isname):(SZI*((rank)+NORMAH+((size)>>LGSZI)*(atoms)+!!(islast))-1))  // # bytes to allocate-1
#define ALLOBLOCK(n) ((n)<2*PMIN?((n)<PMIN?PMINL-1:PMINL) : (n)<8*PMIN?((n)<4*PMIN?PMINL+1:PMINL+2) : (n)<32*PMIN?((n)<16*PMIN?PMINL+3:PMINL+4) : IMIN)   // lg2(#bytes to allocate)-1.  n is #bytes-1
// value to put into name->bucketx for locale names: number if numeric, hash otherwise
#define BUCKETXLOC(len,s) ((*(s)<='9')?strtoI10s((len),(s)):(I)nmhash((len),(s)))
// GA() is used when the type is unknown.  This routine is in m.c and documents the function of these macros.
// NEVER use GA() for NAME types - it doesn't honor it.
// SHAPER is used when shape is given and rank is SDT.  Usually 0/1 use COPYSHAPE0 but they can use this; it always copies from the shaape.  This works only up to rank 2 (but could be extended if needed)
#define GACOPYSHAPER(name,type,atoms,rank,shaape) if((rank)>0)AS(name)[0]=(shaape)[0]; if((rank)>1)AS(name)[1]=(shaape)[1]; if((rank)>2)AS(name)[2]=(shaape)[2];
// SHAPE0 is used when the shape is 0 - write shape only if rank==1
#define GACOPYSHAPE0(name,type,atoms,rank,shaape) if((rank)==1)AS(name)[0]=(atoms);
// General shape copy, branchless when rank<3  AS[0] is always written: #atoms if rank=1, 0 if rank=0.  Used in jtga(), which uses the 0 in AS[0] as a pun for nullptr
#define GACOPYSHAPEG(name,type,atoms,rank,shaape) \
 {I *_d=AS(name); I *_s=(shaape); _s=_s?_s:_d; I cp=*_s; I _r=(rank); cp=_r<1?0:cp; cp=_r==1?(atoms):cp; _s=_r<=1?_d:_s; *_d=cp; ++_d; ++_s; if(likely(_r<3)){*_d=*_s;}else{MC(_d,_s,(_r-1)<<LGSZI);}}
// Use when shape is known to be present but rank is not SDT.  One value is always written to shape
// in this version one value is always written to shape
#define GACOPYSHAPE(name,type,atoms,rank,shaape)  {I *_s=(I*)(shaape); I *_d=AS(name); *_d=*_s; I _r=1-(rank); do{_s+=SGNTO0(_r); _d+=SGNTO0(_r); *_d=*_s;}while(++_r<0);}
#define GACOPY1(name,type,atoms,rank,shaape) {I *_d=AS(name); *_d=1; I _r=1-(rank); do{_d+=SGNTO0(_r); *_d=1;}while(++_r<0);} // copy all 1s to shape
#define GA(v,t,n,r,s)   RZ(v=ga(t,(I)(n),(I)(r),(I*)(s)))
// GAE executes the given expression when there is an error
#define GAE(v,t,n,r,s,erraction)   if(unlikely(!(v=ga(t,(I)(n),(I)(r),(I*)(s)))))erraction;
// When the type and all rank/shape are known at compile time, use GAT.  The compiler precalculates almost everything
// For best results declare name as: AD* RESTRICT name;  The number of bytes, rounded up with overhead added, must not exceed 2^(PMINL+4)
#define GATS(name,type,atoms,rank,shaape,size,shapecopier,erraction) \
{ ASSERT(!((rank)&~RMAX),EVLIMIT); \
 I bytes = ALLOBYTES(atoms,rank,size,(type)&LAST0,(type)&NAME); \
 name = jtgaf(jt, ALLOBLOCK(bytes)); \
 I akx=AKXR(rank);   \
 if(likely(name!=0)){   \
 AK(name)=akx; AT(name)=(type); AN(name)=atoms;   \
 AR(name)=(RANKT)(rank);     \
 if(!((type)&DIRECT)){if(rank==0)AS(name)[0]=0; memset((C*)(AS(name)+1),C0,(bytes-32)&-32);}  \
 shapecopier(name,type,atoms,rank,shaape)   \
    \
 }else{erraction;} \
}
#define GAT(name,type,atoms,rank,shaape)  GATS(name,type,atoms,rank,shaape,type##SIZE,GACOPYSHAPE,R 0)
#define GATR(name,type,atoms,rank,shaape)  GATS(name,type,atoms,rank,shaape,type##SIZE,GACOPYSHAPER,R 0)
#define GAT0(name,type,atoms,rank)  GATS(name,type,atoms,rank,0,type##SIZE,GACOPYSHAPE0,R 0)
#define GAT0E(name,type,atoms,rank,erraction)  GATS(name,type,atoms,rank,0,type##SIZE,GACOPYSHAPE0,erraction)

// Used when type is known and something else is variable.  ##SIZE must be applied before type is substituted, so we have GATVS to use inside other macros.  Normally use GATV
// Note: assigns name before assigning the components of the array, so the components had better not depend on name, i. e. no GATV(z,BOX,AN(z),AR(z),AS(z))
#define GATVS(name,type,atoms,rank,shaape,size,shapecopier,erraction) \
{ I bytes = ALLOBYTES(atoms,rank,size,(type)&LAST0,(type)&NAME); \
 ASSERT(!((((unsigned long long)(atoms))&~TOOMANYATOMS)+((rank)&~RMAX)),EVLIMIT) \
 name = jtgafv(jt, bytes);   \
 I akx=AKXR(rank);   \
 if(likely(name!=0)){   \
  AK(name)=akx; AT(name)=(type); AN(name)=atoms; AR(name)=(RANKT)(rank);     \
  if(!((type)&DIRECT)){AS(name)[0]=0; memset((C*)(AS(name)+1),C0,(bytes-32)&-32);}  \
  shapecopier(name,type,atoms,rank,shaape)   \
     \
 }else{erraction;} \
}

// see warnings above under GATVS
#define GATV(name,type,atoms,rank,shaape) GATVS(name,type,atoms,rank,shaape,type##SIZE,GACOPYSHAPE,R 0)
#define GATVR(name,type,atoms,rank,shaape) GATVS(name,type,atoms,rank,shaape,type##SIZE,GACOPYSHAPER,R 0)
#define GATV1(name,type,atoms,rank) GATVS(name,type,atoms,rank,0,type##SIZE,GACOPY1,R 0)  // this version copies 1 to the entire shape
#define GATV0(name,type,atoms,rank) GATVS(name,type,atoms,rank,0,type##SIZE,GACOPYSHAPE0,R 0)  // shape not written unless rank==1
#define GATV0E(name,type,atoms,rank,erraction) GATVS(name,type,atoms,rank,0,type##SIZE,GACOPYSHAPE0,erraction)  // shape not written unless rank==1
// use this version when you are allocating a sparse matrix.  It handles the AS[0] field correctly.  ALL sparse allocations must come through here so that AC is set sorrectly
#define GASPARSE(n,t,a,r,s) {if((r)==1){GA(n,(t),a,1,0); if(s)AS(n)[0]=(s)[0];}else{GA(n,(t),a,r,s)} AC(n)=ACUC1;}

#define HN              4L  // number of boxes per valence to hold exp-def info (words, control words, original (opt.), symbol table)
// Item count given frame and rank: AS(f) unless r is 0; then 1 
#define SETICFR(w,f,r,targ) (targ=(I)(AS(w)+f), targ=(r)?targ:(I)&I1mem, targ=*(I*)targ)
// Shape item s, but 1 if index is < 0
#define SHAPEN(w,s,targ) (targ=AS(w)[s], targ=(s)<0?1:targ)
// Item count
#define SETIC(w,targ)   (targ=AS(w)[0], targ=AR(w)?targ:1)  //   (AR(w) ? *AS(w) : 1L)
#define ICMP(z,w,n)     memcmpne((z),(w),(n)*SZI)
#define ICPY(z,w,n)     memcpy((z),(w),(n)*SZI)
#define IFCMPNAME(name,string,len,stmt) if(likely((name)->m==(len)))if(likely(!memcmpne((name)->s,string,len)))stmt

// Mark a block as incorporated by removing its inplaceability.  The blocks that are tested for incorporation are ones that are allocated by partitioning, and they will always start out as inplaceable
// If a block is virtual, it must be realized before it can be incorporated.  realized blocks always start off inplaceable and non-pristine
// z is an lvalue
// Use INCORPNA if you need to tell the caller that the block e sent you has been incorporated.  If you created the block being incorporated,
// even by calling a function that returns it, you can be OK just using rifv() or rifvs().  This may leave an incorporated block marked inplaceable,
// but that's OK as long as you don't pass it to some place where it can become an argument to another function
// When a block is incorporated it becomes not pristine, because extractions from the parent may compromise it and we don't want to have to go through recursively to find them
#define INCORP(z) {I af=AFLAG(z); if(unlikely((af&AFVIRTUAL)!=0)){RZ((z)=realize(z))} else{AFLAG(z)=af&~AFPRISTINE;} ACIPNO(z); }
#define INCORPNV(z) {I af=AFLAG(z); AFLAG(z)=af&~AFPRISTINE; ACIPNO(z);}  // use when z is known nonvirtul
// same, but for nonassignable argument.  Must remember to check the result for 0
#define INCORPNA(z) incorp(z)
// use to incorporate into a known-recursive box.  We raise the usecount of z
#define INCORPRA(z) {I af=AFLAG(z); if(unlikely((af&AFVIRTUAL)!=0)){RZ((z)=realize(z))} else{AFLAG(z)=af&~AFPRISTINE;} ra(z); }
// Tests for whether a result incorporates its argument.  The originator, who is going to check this, always marks the argument inplaceable,
// and we signal incorporation either by returning the argument itself or by marking it non-inplaceable (if we box it)
#define WASINCORP1(z,w)    ((z)==(w)||0<=AC(w))
#define WASINCORP2(z,a,w)  ((z)==(w)||(z)==(a)||0<=(AC(a)|AC(w)))
#define INF(x)          ((x)==inf||(x)==infm)
// true if a has recursive usecount
#define UCISRECUR(a)    (AFLAG(a)&RECURSIBLE)
// Install new value z into xv[k], where xv is AAV(x).  If x has recursive usecount, we must increment the usecount of z.
// This also guarantees that z has recursive usecount whenever x does, and that z is realized
#define INSTALLBOX(x,xv,k,z) rifv(z); if(likely((UCISRECUR(x))!=0)){A zzZ=xv[k]; ra(z); fa(zzZ);} xv[k]=z
#define INSTALLBOXNF(x,xv,k,z) rifv(z); if(likely((UCISRECUR(x))!=0)){ra(z);} xv[k]=z   // Don't do the free - if we are installing into known 0 or known nonrecursive
#define INSTALLBOXRECUR(xv,k,z) rifv(z); {I zzK=(k); {A zzZ=xv[zzK]; ra(z); fa(zzZ);} xv[zzK]=z;}  // Don't test - we know we are installing into a recursive block
// Same thing for RAT type.  z is a Q, xv[k] is a Q
#define INSTALLRAT(x,xv,k,z) if(likely((UCISRECUR(x))!=0)){Q zzZ=xv[k]; ra(z.n); ra(z.d); fa(zzZ.n); fa(zzZ.d);} xv[k]=z
#define INSTALLRATNF(x,xv,k,z) if(likely((UCISRECUR(x))!=0)){ra(z.n); ra(z.d);} xv[k]=z   // Don't do the free - if we are installing into known 0
#define INSTALLRATRECUR(xv,k,z) rifv(z.n); rifv(z.d); {I zzK=(k); {Q zzZ=xv[k]; ra(z.n); ra(z.d); fa(zzZ.n); fa(zzZ.d);} xv[zzK]=z;}  // Don't test - we know we are installing into a recursive block
// Use IRS[12] to call a verb that supports IRS.  Rank is nonnegative; result is assigned to z.  z mustn't be any other arg - it is also used as a temp
#define IRS1COMMON(j,w,fs,r,f1,z) (z=(A)(r),z=(I)AR(w)>(I)(r)?z:(A)~0,jt->ranks=(RANK2T)(I)z,z=((AF)(f1))(j,(w),(A)(fs)),jt->ranks=(RANK2T)~0,z)  // nonneg rank
#define IRS1(w,fs,r,f1,z) IRS1COMMON(jt,w,fs,r,f1,z)  // nonneg rank
#define IRSIP1(w,fs,r,f1,z) IRS1COMMON(jtinplace,w,fs,r,f1,z)  // nonneg rank
#define IRS2COMMON(j,a,w,fs,l,r,f2,z) (jt->ranks=(RANK2T)(((((I)AR(a)-(l)>0)?(l):RMAX)<<RANKTX)+(((I)AR(w)-(r)>0)?(r):RMAX)),z=((AF)(f2))(j,(a),(w),(A)(fs)),jt->ranks=(RANK2T)~0,z) // nonneg rank
#define IRS2(a,w,fs,l,r,f2,z) IRS2COMMON(jt,a,w,fs,l,r,f2,z)
#define IRSIP2(a,w,fs,l,r,f2,z) IRS2COMMON(jtinplace,a,w,fs,l,r,f2,z)
#define IRS2AGREE(a,w,fs,l,r,f2,z) {I fl=(I)AR(a)-(l); fl=fl<0?0:fl; I fr=(I)AR(w)-(r); fr=fr<0?0:fr; fl=fr<fl?fr:fl; ASSERTAGREE(AS(a),AS(w),fl) IRS2COMMON(jt,(a),(w),fs,(l),(r),(f2),z); } // nonneg rank; check agreement first
// call to atomic2(), similar to IRS2.  fs is a local block to use to hold the rank (declared as D fs[16]), cxx is the Cxx value of the function to be called
#define ATOMIC2(jt,a,w,fs,l,r,cxx) (FAV((A)(fs))->fgh[0]=ds(cxx), FAV((A)(fs))->id=CQQ, FAV((A)(fs))->lrr=(RANK2T)((l)<<RANKTX)+(r), jtatomic2(jt,(a),(w),(A)fs))

// memory copy, for J blocks.  Like memory copy, but knows it can fetch outside the arg boundaries for LIT-type args
// if bytelen is 1, the arg may be of any length; if 0, must be a multiple of Is and the low bits of length are ignored

#define JMC(d,s,l,lbl,bytelen) MC(d,s,bytelen?(l):(l)&-SZI);
#define JMCR(d,s,l,lbl,bytelen,maskname) MC(d,s,bytelen?(l):(l)&-SZI);
#define JMCDECL(mskname)
#define JMCSETMASK(mskname,l,bytelen)

#define IX(n)           apv((n),0L,1L)
#define JATTN           {if(unlikely(*jt->adbreakr!=0)){jsignal(EVATTN); R 0;}}
#define JBREAK0         {if(unlikely(2<=*jt->adbreakr)){jsignal(EVBREAK); R 0;}}
#define JTIPA           ((J)((I)jt|JTINPLACEA))
#define JTIPAW          ((J)((I)jt|JTINPLACEA+JTINPLACEW))
#define JTIPW           ((J)((I)jt|JTINPLACEW))
#define JTIPAtoW        (J)((I)jt+(((I)jtinplace>>JTINPLACEAX)&JTINPLACEW))          // jtinplace, with a' inplaceability transferred to w
#define JTIPWonly       (J)((I)jtinplace&~(JTINPLACEA+JTWILLBEOPENED+JTCOUNTITEMS))  // dyad jt converted to monad for w
#define JTIPEX1(name,arg) jt##name(JTIPW,arg)                                        // like name(arg) but inplace
#define JTIPEX1S(name,arg,self) jt##name(JTIPW,arg,self)                             // like name(arg,self) but inplace
#define JTIPAEX2(name,arga,argw) jt##name(JTIPA,arga,argw)                           // like name(arga,argw) but inplace on a
#define JTIPAEX2S(name,arga,argw,self) jt##name(JTIPA,arga,argw,self)                // like name(arga,argw,self) but inplace on a
// given a unique num, define loop begin and end labels
#define LOOPBEGIN(num) lp##num
#define LOOPEND(num) lp##num##e
#define MAX(a,b)        ((a)>(b)?(a):(b))
#define MC              memcpy
#define MCL(dest,src,n) memcpy(dest,src,n)  // use when copy is expected to be long
#define MCI(dest,src,n) memcpy(dest,src,(n)*sizeof(*src))   // copy items of source
#define MCIL(dest,src,n) memcpy(dest,src,(n)*sizeof(*src))   // use when copy expected to be long
#define MCIS(dest,src,n) {I * RESTRICT _d=(dest); I * RESTRICT _s=(src); I _n=~(n); while((_n-=REPSGN(_n))<0)*_d++=*_s++;}  // use for short copies.  the tricky stuff is to confound the compiler so it doesn't produce memcpy
#define MCISd(dest,src,n) {I * RESTRICT _s=(src); I _n=~(n); while((_n-=REPSGN(_n))<0)*dest++=*_s++;}  // ... this version when d increments through the loop
#define MCISs(dest,src,n) {I * RESTRICT _d=(dest); I _n=~(n); while((_n-=REPSGN(_n))<0)*_d++=*src++;}  // ... this when s increments through the loop
#define MCISds(dest,src,n) {I _n=~(n); while((_n-=REPSGN(_n))<0)*dest++=*src++;}  // ...this when both
// Copy shapes.  Optimized for length <5, subroutine for others
#define MCISH(dest,src,n) \
 {I *_d=(dest), *_s=(src); I _n=(I)(n); \
  if(likely(_n<=2)){ \
   _n-=1; _d=(_n<0)?jt->shapesink+1:_d; _s=(_n<0)?jt->shapesink+1:_s; \
   _d[0]=_s[0]; _d[_n]=_s[_n];  \
  }else{MC(_d,_s,_n<<LGSZI);} \
 }
#define MCISHd(dest,src,n) {MCISH(dest,src,n) dest+=(n);}  // ... this version when d increments through the loop
#define MCISHs(dest,src,n) {MCISH(dest,src,n) src+=(n);}
#define MCISHds(dest,src,n) {MCISH(dest,src,n) dest+=(n); src+=(n);}
// not used #define MCISU(dest,src,n) {I * RESTRICT _d=(I*)(dest); I * RESTRICT _s=(I*)(src); I _n=-(n); do{*_d++=*_s++;}while((_n-=(_n>>(BW-1)))<0);}  // always runs once
// not used #define MCISUds(dest,src,n) {I _n=-(n); do{*dest++=*src++;}while((_n-=(_n>>(BW-1)))<0);}  // always runs once

#define MIN(a,b)        ((a)<(b)?(a):(b))
#define MLEN            (63)
// change the type of the inplaceable block z to t.  We know or assume that the type is being changed.  If the block is UNINCORPABLE (& therefore virtual), replace it with a clone first.  z is an lvalue
#define MODBLOCKTYPE(z,t)  {if(unlikely((AFLAG(z)&AFUNINCORPABLE)!=0)){RZ(z=clonevirtual(z));} AT(z)=(t);}
#define MODIFIABLE(x)   (x)   // indicates that we modify the result, and it had better not be read-only
// define multiply-add
#define NAN0            (_clearfp())
#define NAN1            {if(unlikely(_SW_INVALID&_clearfp())){jsignal(EVNAN); R 0;}}
#define NAN1V           {if(unlikely(_SW_INVALID&_clearfp())){jsignal(EVNAN); R  ;}}
#define NANTEST         (_SW_INVALID&_clearfp())

#if defined(__GNUC__)   // vector extension

#if !(defined(__aarch64__)||defined(__arm__))
typedef int64_t int64x2_t __attribute__ ((vector_size (16),aligned(16)));
typedef double float64x2_t __attribute__ ((vector_size (16),aligned(16)));
#endif
#define dump_int64x2(a,x) {int64x2_t _b=x;fprintf(stderr,"%s %lli %lli \n", a, ((long long*)(&_b))[0], ((long long*)(&_b))[1]);}
#define dump_float64x2(a,x) {float64x2_t _b=x;fprintf(stderr,"%s %f %f \n", a, ((double*)(&_b))[0], ((double*)(&_b))[1]);}

static inline __attribute__((inline)) int vec_any_si128(int64x2_t mask)
{
   return (mask[0] & 0x8000000000000000)||(mask[1] & 0x8000000000000000);
}

static inline __attribute__((inline)) float64x2_t vec_maskload_pd(double const* mem_addr, int64x2_t mask)
{
   float64x2_t ret={0.0,0.0};
   int i;

    for (i=0; i<2; i++){
      if (mask[i] & 0x8000000000000000){
        ret[i] = *(mem_addr + i);
      }
    }
    return ret;
}

static inline __attribute__((inline)) void vec_maskstore_pd(double * mem_addr, int64x2_t  mask, float64x2_t a)
{
   int i;
    for (i=0; i<2; i++){
      if (mask[i] & 0x8000000000000000)
        *(mem_addr + i) = a[i];
    }
}

static inline __attribute__((inline)) int64x2_t vec_loadu_si128(int64x2_t const * a)
{
   int64x2_t ret;
   memcpy(&ret, (int64_t *)a, 2*sizeof(int64_t));  // must cast pointer otherwise segment 
   return ret;
}

static inline __attribute__((inline)) float64x2_t vec_loadu_pd(double const * a)
{
   float64x2_t ret;
   memcpy(&ret, a, 2*sizeof(double));
   return ret;
}

static inline __attribute__((inline)) void vec_storeu_pd(double * mem_addr, float64x2_t a)
{
   memcpy(mem_addr, &a , 2*sizeof(double));
}

static inline __attribute__((inline)) float64x2_t vec_and_pd(float64x2_t a, float64x2_t b)
{
   int64x2_t a1,b1;
   float64x2_t ret;
   memcpy(&a1, &a, 2*sizeof(double));
   memcpy(&b1, &b, 2*sizeof(double));
   a1 &= b1;
   memcpy(&ret, &a1, 2*sizeof(double));
   return ret;
}

#define NPAR ((I)(sizeof(float64x2_t)/sizeof(D))) // number of Ds processed in parallel
#define LGNPAR 1  // 128-bit no good automatic way to do this
// loop for atomic parallel ops.  // fixed: n is #atoms (never 0), x->input, z->result, u=input atom4 and result
//                                                                                  __SSE2__    atom2
#define AVXATOMLOOP(preloop,loopbody,postloop) \
 int64x2_t endmask;  float64x2_t u; \
 endmask = vec_loadu_si128((int64x2_t*)(validitymask+((-n)&(NPAR-1))));  /* mask for 0 1 2 3 4 5 is xx 01 11 01 11 01 */ \
 preloop \
 I i=(n-1)>>LGNPAR;  /* # loops for 0 1 2 3 4 5 is x 0 0 0 0 1 */ \
            /* __SSE2__ # loops for 0 1 2 3 4 5 is x 1 0 1 0 1 */ \
 while(--i>=0){ u=vec_loadu_pd(x); \
  loopbody \
  vec_storeu_pd(z, u); x+=NPAR; z+=NPAR; \
 } \
 u=vec_maskload_pd(x,endmask); \
 loopbody \
 vec_maskstore_pd(z, endmask, u); \
 x+=((n-1)&(NPAR-1))+1; z+=((n-1)&(NPAR-1))+1; \
 postloop

// version that pipelines one read ahead.  Input to loopbody2 is zu; result of loopbody1 is in zt
#define AVXATOMLOOPPIPE(preloop,loopbody1,loopbody2,postloop) \
 int64x2_t endmask;  float64x2_t u, zt, zu; \
 endmask = vec_loadu_si128((int64x2_t*)(validitymask+((-n)&(NPAR-1))));  /* mask for 0 1 2 3 4 5 is xx 01 11 01 11 01 */ \
 preloop \
 I i=(n-1)>>LGNPAR;  /* # loops for 0 1 2 3 4 5 is x 0 0 0 0 1 */ \
            /* __SSE2__ # loops for 0 1 2 3 4 5 is x 1 0 1 0 1 */ \
 if(i>0){u=*(float64x2_t *)(x); x+=NPAR; loopbody1 \
 while(--i>=0){ u=vec_loadu_pd(x); x+=NPAR; \
  zu=zt; loopbody1 loopbody2 \
  vec_storeu_pd(z, u); z+=NPAR; \
 } zu=zt; loopbody2 vec_storeu_pd(z, u); z+=NPAR;} \
 u=vec_maskload_pd(x,endmask); \
 loopbody1 zu=zt; loopbody2 \
 vec_maskstore_pd(z, endmask, u); \
 x+=((n-1)&(NPAR-1))+1; z+=((n-1)&(NPAR-1))+1; \
 postloop

// Dyadic version.  v is right argument, u is still result
#define AVXATOMLOOP2(preloop,loopbody,postloop) \
 int64x2_t endmask;  float64x2_t u,v; \
 endmask = vec_loadu_si128((int64x2_t*)(validitymask+((-n)&(NPAR-1))));  /* mask for 0 1 2 3 4 5 is xx 01 11 01 11 01 */ \
 preloop \
 I i=(n-1)>>LGNPAR;  /* # loops for 0 1 2 3 4 5 is x 0 0 0 0 1 */ \
            /* __SSE2__ # loops for 0 1 2 3 4 5 is x 1 0 1 0 1 */ \
 while(--i>=0){ u=vec_loadu_pd(x); v=vec_loadu_pd(y); \
  loopbody \
  vec_storeu_pd(z, u); x+=NPAR; y+=NPAR; z+=NPAR; \
 } \
 u=_mm_maskload_pd(x,endmask); v=_mm_maskload_pd(y,endmask); \
 loopbody \
 _mm_maskstore_pd(z, endmask, u); \
 postloop

#endif

#define NUMMAX          9    // largest number represented in num[]
#define NUMMIN          (~NUMMAX)    // smallest number represented in num[]
// Given SZI B01s read into p, pack the bits into the MSBs of p and clear the lower bits of p
#if BW==64
// this is what it should be #define PACKBITS(p) {p|=p>>7LL;p|=p>>14LL;p|=p>>28LL;p<<=56LL;}
#define PACKBITS(p) {p|=p>>7LL;p|=p>>14LL;p|=p<<28LL;p&=0xff0000000; p<<=28LL;}  // this generates one extra instruction, rather than the 3 for the correct version
#define PACKBITSINTO(p,out) {p|=p>>7LL;p|=p>>14LL;out=((p|(p>>28LL))<<56)|(out>>SZI);}  // pack and shift into out
#endif
#define PRISTCOMSET(w,flg) awback=(w); if(unlikely((flg&AFVIRTUAL)!=0)){awback=ABACK(awback); flg=AFLAG(awback);} AFLAG(awback)=flg&~AFPRISTINE;
#define PRISTCOMSETF(w,flg) if(unlikely((flg&AFVIRTUAL)!=0)){w=ABACK(w); flg=AFLAG(w);} AFLAG(w)=flg&~AFPRISTINE;   // used only at end, when w can be destroyed
#define PRISTCOMMON(w,exe) awflg=AFLAG(w); exe PRISTCOMSET(w,awflg)
// same but destroy w
#define PRISTCOMMONF(w,exe) I awflg=AFLAG(w); exe PRISTCOMSETF(w,awflg)
#define PRISTCLRNODCL(w) PRISTCOMMON(w,)
// normal entry points.  clear PRISTINE flag in w (or its backer, if virtual) because we have removed something from it
#define PRISTCLR(w) I awflg; A awback; PRISTCLRNODCL(w)
// same, but destroy w in the process
#define PRISTCLRF(w) PRISTCOMMONF(w,)
// transfer pristinity of w to z
#define PRISTXFER(z,w) PRISTCOMMON(w,AFLAG(z)|=awflg&((SGNTO0(AC(w))&((I)jtinplace>>JTINPLACEWX))<<AFPRISTINEX);)
// transfer pristinity of w to z, destroying w
#define PRISTXFERF(z,w) PRISTCOMMONF(w,AFLAG(z)|=awflg&((SGNTO0(AC(w))&((I)jtinplace>>JTINPLACEWX))<<AFPRISTINEX);)  // use w bit of jtinplace
#define PRISTXFERAF(z,a) PRISTCOMMONF(a,AFLAG(z)|=awflg&((SGNTO0(AC(a))&((I)jtinplace>>JTINPLACEAX))<<AFPRISTINEX);)  // use a bit of jtinplace
// same, but with an added condition (in bit 0)
#define PRISTXFERFIF(z,w,cond) PRISTCOMMONF(w,AFLAG(z)|=awflg&(((cond)&SGNTO0(AC(w))&((I)jtinplace>>JTINPLACEWX))<<AFPRISTINEX);)
// transfer pristinity from a AND w to z (not if a==w)
#define PRISTXFERF2(z,a,w) I aflg=AFLAG(a), wflg=AFLAG(w); AFLAG(z)|=aflg&wflg&(((a!=w)&SGNTO0(AC(a)&AC(w))&((I)jtinplace>>JTINPLACEAX)&((I)jtinplace>>JTINPLACEWX))<<AFPRISTINEX); \
                           PRISTCOMSETF(a,aflg) PRISTCOMSETF(w,wflg)
// PROD multiplies a list of numbers, where the product is known not to overflow a signed int (for example, it might be part of the shape of a nonempty dense array)
// assign length first so we can sneak some computation into ain in va2
#define PROD(z,length,ain) {I _i=(length); I * RESTRICT _zzt=(ain)-2; \
if(likely(_i<3)){_zzt+=_i; z=(I)&oneone; _zzt=_i>=1?_zzt:(I*)z; z=_i>1?(I)_zzt:z; z=((I*)z)[0]; z*=_zzt[1];}else{z=prod(_i,_zzt+2);} }
// This version ignores bits of length above the low RANKTX bits
#define PRODRNK(result,length,ain) {I _i=(length); I * RESTRICT _zzt=(ain); \
  if(likely((US)_i<3)){_zzt=_i&3?_zzt:iotavec-IOTAVECBEGIN+1; result=*_zzt; ++_zzt; _zzt=_i&2?_zzt:iotavec-IOTAVECBEGIN+1; result*=*_zzt;}else{result=prod((US)_i,_zzt);} }
// the 3REG version is perfect - too perfect, because the compiler saves a reg and decides to spill acr rather than aaa/aab which are used only in a predictable test
#define PRODRNK3REG(z,length,ain) {I _i=(length); I * RESTRICT _zzt=(ain)-2; z=(US)_i; \
if(likely(z<3)){_zzt+=z; z=(I)&oneone; _zzt=_i&3?_zzt:(I*)z; z=_i&2?(I)_zzt:z; z=((I*)z)[0]; z*=_zzt[1];}else{z=prod(z,_zzt+2);} }

#define PROD1(result,length,ain) PROD(result,length,ain)  // scaf
// PRODX replaces CPROD.  It is PROD with a test for overflow included.  To save calls to mult, PRODX takes an initial value
// PRODX takes the product of init and v[0..n-1], generating error if overflow, but waiting till the end so no error if there is a 0 in the product
// overflow sets z to the error value of 0; if we see a multiplicand of 0 we stop right away so we can skip the error
// This is written to be branchless for rank < 3
// I have been unable to make clang produce a simple loop that doesn't end with a backward branch.  So I am going to handle ranks 0-2 here and call a subroutine for the rest
#define PRODX(z,n,v,init) \
 {I nn=(n); \
  if(likely(nn<3)){I temp=(init);  /* must use temp because init may depend on z */ \
   I *_zzt=(v); _zzt+=nn-2; z=(I)&oneone; _zzt=nn>=1?_zzt:(I*)z; z=nn>1?(I)_zzt:z;   /* set up pointers to args, and init value */ \
   z=((I*)z)[0]; if(likely(z!=0)){DPMULDZ(z,_zzt[1],z); if(likely(_zzt[1]!=0)){DPMULDZ(z,temp,z);if(likely(temp!=0)){ASSERT(z!=0,EVLIMIT)}}}  /* no error if any nonzero */ \
  }else{DPMULDE(init,prod(nn,v),z) RE(0)} /* error if error inside prod */ \
 }
// CPROD is to be used to create a test testing #atoms.  Because empty arrays can have cells that have too many atoms, we can't use PROD if
// we don't know that the array isn't empty or will be checked later
#define CPROD(t,z,x,a) PRODX(z,x,a,1)
#define CPROD1(t,z,x,a) CPROD(t,z,x,a)
// PROLOG/EPILOG are the main means of memory allocation/free.  jt->tstack contains a pointer to every block that is allocated by GATV(i. e. all blocks).
// GA causes a pointer to the block to be pushed onto tstack.  PROLOG saves a copy of the stack pointer in _ttop, a local variable in its function.  Later, tpop(_ttop)
// can be executed to free every block that the function allocated, without requiring bookkeeping in the function.  This may be done from time to time in
// long-running definitions, to free memory [for this application it is normal to do some allocating of working memory, then save the tstack pointer in a local name
// other than _ttop, then periodically do tpop(other local name); such a sequence will free up all memory that was allocated after the working memory; the working
// memory itself will be freed by the eventual tpop(_ttop)].
// EPILOG performs the tpop(_ttop), but it has another important function: that of preserving the result of a function.  Of all the blocks that were allocated by a function,
// one (possibly including its descendants) is the result of the function.  It must not be freed, so that it can carry the result back to the caller of this function.
// So, it is preserved by incrementing its usecount before the tpop(_ttop); then after the tpop, it is pushed back onto the tstack, indicating that it will be freed
// by the next-higher-level function.  Thus, when X calls Y inside PROLOG/EPILOG, the result of Y (which is an A block), has the same viability as any other GA executed in X
// (unless its usecount is > 1 because it was assigned elsewhere)
#define PROLOG(x)       A *_ttop=jt->tnextpushp
#define PROLOGNOTREQ(x)   // if nothing is allocated, we don't really need PROLOG
#define EPILOGNORET(z) (gc(z,_ttop))   // protect z and return its address
#define EPILOG(z)       RETF(EPILOGNORET(z))   // z is the result block
#define EPILOGNOVIRT(z)       R rifvsdebug((gc(z,_ttop)))   // use this when the repercussions of allowing virtual result are too severe
#define EPILOGZOMB(z)       if(unlikely(!gc3(&(z),0L,0L,_ttop)))R0; RETF(z);   // z is the result block.  Use this if z may contain inplaceable contents that would free prematurely
// Routines that do little except call a function that does PROLOG/EPILOG have EPILOGNULL as a placeholder
#define EPILOGNULL(z)   R z
#define EPILOGNOTREQ(z)   R z  // used if originak JE had needless EPILOG
// Routines that do not return A
#define EPILOG0         tpop(_ttop)
// Routines that modify the comparison tolerance must stack it
#define PUSHCCT(value) D cctstack = jt->cct; jt->cct=(value);   // declare the stacked value where it can be popped
#define PUSHCCTIF(value,cond) D cctstack = jt->cct; if(cond)jt->cct=(value);   // declare the stacked value where it can be popped
#define POPCCT jt->cct=cctstack;
// When we push, we are about to execute verbs before the last one, and an inplacement there would lead to the name's being assigned with invalid
// data.  So, we clear the inplace variables if we don't want to allow that: if the user set zomblevel=0, or if there is no local symbol table
// (which means the user is fooling around at the keyboard & performance is not as important as transparency)
#define CLEARZOMBIE     {jt->assignsym=0;}  // Used when we know there shouldn't be an assignsym, just in case
#define PUSHZOMB L*savassignsym = jt->assignsym; if(savassignsym){if(unlikely(((jt->asgzomblevel-1)|((AN(jt->locsyms)-2)))<0)){CLEARZOMBIE}}  // test is (jt->asgzomblevel==0||AN(jt->locsyms)<2)
#define POPZOMB {jt->assignsym=savassignsym;}
#define R               return
#if FINDNULLRET   // When we return 0, we should always have an error code set.  trap if not
#define R0 {if(jt->jerr)R A0;else SEGFAULT;}
#else
#define R0 R 0;
#endif
#define REPSGN(x) ((x)>>(BW-1))  // replicate sign bit of x to entire word (assuming x is signed type - if unsigned, just move sign to bit 0)
#define SGNTO0(x) ((UI)(x)>>(BW-1))  // move sign bit to bit 0, clear other bits
// In the original JE many verbs returned a clone of the input, i. e. R ca(w).  We have changed these to avoid the clone, but we preserve the memory in case we need to go back
#define RCA(w)          R w
#define RE(exp)         {if(unlikely(((exp),jt->jerr!=0)))R 0;}
#define RESETERR        {jt->etxn=jt->jerr=0;}
#define RESETERRANDMSG  {jt->etxn1=jt->etxn=jt->jerr=0;}
#define RESETRANK       (jt->ranks=(RANK2T)~0)
#define RNE(exp)        {R jt->jerr?0:(exp);}
#define RZ(exp)         {if(unlikely(!(exp)))R0}
#if MEMAUDIT&0xc
#define DEADARG(x)      (x?(AFLAG(x)&CONW?SEGFAULT:0):0); if(MEMAUDIT&0x10)auditmemchains(); if(MEMAUDIT&0x2)audittstack(jt); 
#define ARGCHK1D(x)     ARGCHK1(x)  // these not needed normally, but useful for debugging
#define ARGCHK2D(x,y)   ARGCHK2(x,y)
#else
#define DEADARG(x)      0
#define ARGCHK1D(x)
#define ARGCHK2D(x,y)
#endif
#define ARGCHK1(x)      RZ(x) DEADARG(x);   // bit set in deadbeef
#define ARGCHK2(x,y)    ARGCHK1(x) ARGCHK1(y)
#define ARGCHK3(x,y,z)  ARGCHK1(x) ARGCHK1(y) ARGCHK1(z)


// RETF is the normal function return.  For debugging we hook into it
#if AUDITEXECRESULTS && (FORCEVIRTUALINPUTS==2)
#define RETF(exp)       A ZZZz = (exp); auditblock(ZZZz,1,1); ZZZz = virtifnonip(jt,0,ZZZz); R ZZZz
#else
#if MEMAUDIT&0xc
#define RETF(exp)       A ZZZz = (exp); DEADARG(ZZZz); R ZZZz
#else
#define RETF(exp)       R exp
#endif
// Input is a byte.  It is replicated to all lanes of a UI
#endif
#if BW==64
#define REPLBYTETOW(in,out) (out=(UC)(in),out|=out<<8,out|=out<<16,out|=out<<32)
#else
#define REPLBYTETOW(in,out) (out=(UC)(in),out|=out<<8,out|=out<<16)
#endif
// Output is pointer, Input is I/UI, count is # bytes to NOT store to output pointer (0-7).
#define STOREBYTES(out,in,n) {*(UI*)(out) = (*(UI*)(out)&~((UI)~(I)0 >> ((n)<<3))) | ((in)&((UI)~(I)0 >> ((n)<<3)));}
// Input is the name of word of bytes.  Result is modified name, 1 bit per input byte, spaced like B01s, with the bit 0 iff the corresponding input byte was all 0.  Non-boolean bits of result are garbage.
#define ZBYTESTOZBITS(b) (b=b|((b|(~b+VALIDBOOLEAN))>>7))  // for each byte: zero if b0 off, b7 off, and b7 turns on when you subtract 1 or 2
// to verify gah conversion #define RETF(exp)       { A retfff=(exp);  if ((retfff) && ((AT(retfff)&SPARSE && AN(retfff)!=1) || (AT(retfff)&DENSE && AN(retfff)!=prod(AR(retfff),AS(retfff)))))SEGFAULT;; R retfff; } // scaf
#define SBSV(x)         (jt->sbsv+(I)(x))
#define SBUV(x)         (jt->sbuv+(I)(x))
#define SEGFAULT        (*(volatile I*)0 = 0)
#define SGN(a)          ((I )(0<(a))-(I )(0>(a)))
#define SMAX            65535
#define SMIN            (-65536)
#define SYMHASH(h,n)    (((UI)(h)*(UI)(n)>>32)+SYMLINFOSIZE)   // h is hash value for symbol; n is number of symbol chains (not including LINFO entries)
// symbol tables.   jt->locsyms is locals and jt->global is globals.  AN(table) gives #hashchains+1; if it's 1 we have an empty table, used to indicate that there are no locals
// At all times we keep the k field of locsyms as a copy of jt->global so that if we need it for u./v. we know what the symbol tables were.  We could remove jt->global but that would cost
// some load instructions sometimes.  AM(local table) points to the previous local table in the stack, looping to self at end
#define SYMSETGLOBAL(l,g) (jt->global=(g), AKGST(l)=(g))  // l is jt->locsyms, g is new global value
#define SYMRESTOREFROMLOCAL(l) (jt->locsyms=(l), jt->global=AKGST(l))  // go to stacked local l
#define SYMSWAPTOLOCAL(l,lsave) (lsave=jt->locsyms, SYMRESTOREFROMLOCAL(l))  // go to stacked local l, save current in lsave
#define SYMSETLOCAL(l) (AKGST(l)=jt->global, jt->locsyms=(l))  // change the locals to l
#define SYMPUSHLOCAL(l) (AM(l)=(I)jt->locsyms, SYMSETLOCAL(l))  // push l onto locals stack
// fa() the value when a symbol is deleted/reassigned.  If the symbol was ABANDONED, don't fa() because there was no ra() - but do revert 1 to 8..1 
#define SYMVALFA(l) if(unlikely(((l).flag&LWASABANDONED)!=0)){(l).flag&=~LWASABANDONED; AC((l).val)|=ACINPLACE&(AC((l).val)-2);}else{fa((l).val);}
#define SZA             ((I)sizeof(A))
#define LGSZA    LGSZI  // we always require A and I to have same size
#define SZD             ((I)sizeof(D))
#define SZI             ((I)sizeof(I))
#define LGSZD    3  // lg(#bytes in a D)
#define SZI4            ((I)sizeof(I4))
#define LGSZI4   2  // lg (bytes in an I4)
#define SZUI4            ((I)sizeof(UI4))
#define LGSZUI4  2  // lg(#bytes in a UI4)
#define SZUS            ((I)sizeof(US))
#define LGSZUS   1  // lg(bytes in a US)
#define SZS            ((I)sizeof(S))
#define LGSZS   1  // lg (bytes in an S)

#define VAL1            '\001'
#define VAL2            '\002'
#define WITHDEBUGOFF(stmt) {UC d=jt->uflags.us.cx.cx_c.db; jt->uflags.us.cx.cx_c.db=0; stmt jt->uflags.us.cx.cx_c.db=d;}  // execute stmt with debug turned off

#if BW==64
#define IHALF0  0x00000000ffffffffLL
#else
#define IHALF0  0x0000ffff
#endif
#define B0000   0x00000000
#define B0001   0x01000000
#define B0010   0x00010000
#define B0011   0x01010000
#define B0100   0x00000100
#define B0101   0x01000100
#define B0110   0x00010100
#define B0111   0x01010100
#define B1000   0x00000001
#define B1001   0x01000001
#define B1010   0x00010001
#define B1011   0x01010001
#define B1100   0x00000101
#define B1101   0x01000101
#define B1110   0x00010101
#define B1111   0x01010101
#define BS00    0x0000
#define BS01    0x0100
#define BS10    0x0001
#define BS11    0x0101

#define CACHELINESIZE 64  // size of processor cache line, in case we align to it

// flags in call to cachedmmult and blockedmmult
#define FLGCMPX 0
#define FLGCMP ((I)1<<FLGCMPX)  // arguments are complex
#define FLGAUTRIX 1
#define FLGAUTRI ((I)1<<FLGAUTRIX)  // left arg is upper-triangular
#define FLGWUTRIX 2
#define FLGWUTRI ((I)1<<FLGWUTRIX)  // left arg is upper-triangular
#define FLGINTX 3
#define FLGINT ((I)1<<FLGINTX)  // args are INT
#define FLGZFIRSTX 4
#define FLGZFIRST ((I)1<<FLGZFIRSTX)  // first pass of the Z values, use 0
#define FLGZLASTX 5
#define FLGZLAST ((I)1<<FLGZLASTX)  // last pass of the Z values, write to output
#define FLGWMINUSZX 6
#define FLGWMINUSZ ((I)1<<FLGWMINUSZX)  // calculate z-x*y rather than x*y.  Used by %.


#include "ja.h" 
#include "jc.h" 
#include "jtype.h" 
#include "m.h"
#include "jt.h" 
#include "jlib.h"
#include "je.h" 
#include "jerr.h" 
#include "va.h" 
#include "vq.h" 
#include "vx.h" 
#include "vz.h"
#include "a.h"
#include "s.h"



// CTTZ(w) counts trailing zeros in low 32 bits of w.  Result is undefined if w is 0.
// CTTZZ(w) does the same, but returns 32 if w is 0
// CTTZI(w) counts trailing zeros in an argument of type I (32 or 64 bits depending on architecture)

// CTTZ uses the single-instruction count-trailing-zeros instruction to convert
// a 1-bit mask to a bit number.  If this instruction is available on your architecture/compiler,
// you should use the compiler intrinsic to create this instruction, and define the CTTZ and CTTZZ
// macros to use the instruction inline.  It is used enough in the J engine to make a difference.

// If you set AUDITCOMPILER to 1, i.c will include code to test CTTZ (and signed shift) on startup and crash if it
// does not perform properly, as a debugging aid.

// If CTTZ is not defined, the default routine defined in u.c will be used.  You can look there
// for the complete spec for CTTZ and CTTZZ.



#ifdef MMSC_VER
#define NOINLINE __declspec(noinline)
#else
#define NOINLINE __attribute__((noinline))
#ifndef __forceinline
#define __forceinline inline __attribute__((inline))
#endif
#endif
#ifdef __MINGW32__
// original definition
// #define __forceinline extern __inline__ __attribute__((inline,__gnu_inline__))
#ifdef __forceinline
#undef __forceinline
#endif
#define __forceinline __inline__ __attribute__((inline,__gnu_inline__))
#endif

#ifdef __GNUC__
#define CTTZ(w) __builtin_ctzl((UINT)(w))
#define CTTZI(w) __builtin_ctzll((UI)(w))
#define CTLZI(w,out) (out=(63-__builtin_clzll((UI)(w))))
#define CTTZZ(w) ((w)==0 ? 32 : CTTZ(w))
#endif

// For older processors, TZCNT is executed as BSF, which differs from TZCNT in that it does not
// set the Z flag if the result is 0.  The optimizer sometimes turns a switch into tests rather than a branch
// table, and it expects TZCNT to set the Z flag properly.  We use CTTZNOFLAG to set it right
#define CTTZNOFLAG(w) (CTTZ(w)&31)

#ifdef __GNUC__
#ifndef offsetof
#define offsetof(TYPE, MEMBER)  __builtin_offsetof (TYPE, MEMBER)
#endif
#endif

// Insert other compilers/architectures here

// Insert CTLZ here if CTTZ is not available

// If your machine supports count-leading-zeros but not count-trailing-zeros, you can define the macros
// CTLZ/CTLZI, which returns the number of high-order zeros in the low 32 bits of its argument, and the following
// CTTZ will be defined:
#if defined(CTLZ) && !defined(CTTZ)
#define CTTZ(w) (31-CTLZ((w)&-(w)))
#define CTTZI(w) (63-CTLZI((w)&-(w)))
#define CTTZZ(w) (0xffffffff&(w) ? CTTZ(w) : 32)
#endif

// If CTTZ is not defined, the following code will use the default from u.c:
#if !defined(CTTZ)
extern I CTTZ(I);
extern I CTTZI(I);
extern I CTTZZ(I);
#endif
#if !defined(CTLZI)
extern I CTLZI_(UI,UI4*);
#define CTLZI(in,out) CTLZI_(in,&(out))
#endif

// Set these switches for testing
#define AUDITBP 0  // Verify that bp() returns expected data
#define AUDITCOMPILER 0  // Verify compiler features CTTZ, signed >>


// JPFX("here we are\n")
// JPF("size is %i\n", v)
// JPF("size and extra: %i %i\n", (v,x))
#define JPFX(s)  {char b[1000]; sprintf(b, s);    jsto(gjt,MTYOFM,b);}
#define JPF(s,v) {char b[1000]; sprintf(b, s, v); jsto(gjt,MTYOFM,b);}
extern J gjt; // global for JPF (procs without jt)

#if (defined(__arm__)||defined(__aarch64__)||defined(_M_ARM64)) && !defined(__MACH__)
// option -fsigned-char in android and raspberry
#ifdef strchr
#undef strchr
#endif
#define strchr(a,b)     (C*)strchr((unsigned char*)(a), (unsigned char)(b))
#endif

/* workaround clang branch prediction side effect */
#if defined(__clang__) && ( (__clang_major__ > 3) || ((__clang_major__ == 3) && (__clang_minor__ > 3)))
#define dmul2(u,v) ({__asm__("" ::: "cc");(u)*(v);})
#define ddiv2(u,v) ({__asm__("" ::: "cc");(u)/(v);})
#else
#define dmul2(u,v) ((u)*(v))
#define ddiv2(u,v) ((u)/(v))
#endif

/* (hopefully) turn off some re-scheduling optimization  */
#ifdef __GNUC__
#define CCBLOCK __asm__("" ::: "cc")
#else
#define CCBLOCK
#endif

#if SYS & SYS_UNIX
#include <fenv.h>
// bug clang isnan(x) set NaN flag if x is NaN

#define _isnan       isnan

#define _SW_INVALID  FE_INVALID

static inline UINT _clearfp(void){
  int r = fetestexcept(FE_ALL_EXCEPT);
  feclearexcept(FE_ALL_EXCEPT); 
  return r;
}
#endif

// Define integer multiply, *z=x*y but do something else if integer overflow.
// Depending on the compiler, the overflowed result may or may not have been stored

#if C_USEMULTINTRINSIC

#define DPMULDECLS
#define DPMUL(x,y,z,s) if(unlikely(__builtin_smulll_overflow(x,y,z)))s
#define DPMULDDECLS
#define DPMULD(x,y,z,s) if(unlikely(__builtin_smulll_overflow(x,y,&z)))s
#define DPMULDZ(x,y,z) z=__builtin_smulll_overflow(x,y,&z)?0:z;
#define DPMULDE(x,y,z) ASSERT(!__builtin_smulll_overflow(x,y,&z),EVLIMIT)
#define DPUMUL(x,y,z,h) {__int128 _t; _t=(__int128)(x)*(__int128)(y); z=(I)_t; h=(I)(_t>>64);}  // product in z and h
#else // C_USEMULTINTRINSIC 0 - use standard-C version (64-bit)
#define DPMULDECLS
#define DPMUL(x,y,z,s) {I _l, _x=(x), _y=(y); D _d; _l=_x*_y; _d=(D)_x*(D)_y-(D)_l; *z=_l; _d=ABS(_d); if(_d>1e8)s}  // *z may be the same as x or y
#define DPMULDDECLS
#define DPMULD(x,y,z,s) {I _l, _x=(x), _y=(y); D _d; _l=_x*_y; _d=(D)_x*(D)_y-(D)_l; z=_l; _d=ABS(_d); if(_d>1e8)s}
#define DPMULDZ(x,y,z) DPMULD(x,y,z,z=0;)
#define DPMULDE(x,y,z)  DPMULD(x,y,z,ASSERT(0,EVLIMIT))
#endif

// end of multiply builtins

// define single+double-precision integer add

#ifndef SPDPADD   // default version for systems without addcarry
#define SPDPADD(addend, sumlo, sumhi) sumlo += addend; sumhi += (addend > sumlo);
#endif
// end of addition builtins

// aligned memory allocation, assume align is power of 2
static __forceinline void* aligned_malloc(size_t size, size_t align) {
 void *result;
 align = (align>=sizeof(void*))?align:sizeof(void*);
#if ( !defined(ANDROID) || defined(__LP64__) )
 if(posix_memalign(&result, align, size)) result = 0;
#else
 void *mem = malloc(size+(align-1)+sizeof(void*));
 if(mem){
  result = (void*)((uintptr_t)(mem+(align-1)+sizeof(void*)) & ~(align-1));
  ((void**)result)[-1] = mem;
 } else result = 0;
#endif
 return result;
}

static __forceinline void aligned_free(void *ptr) {
#if ( !defined(ANDROID) || defined(__LP64__) )
 free(ptr);
#else
 free(((void**)ptr)[-1]);
#endif
}

// Create (x&y) where x and y are signed, so we can test for overflow.

#define XANDY(x,y) ((I)((UI)(x)&(UI)(y)))
