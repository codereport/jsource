/* Copyright 1990-2008, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Verbs: Macros and Defined Constants for Atomic (Scalar) Verbs           */

                                    /*   cv - control vector               */
// bits 0-1 kept open for jtflags
// bits 2-3 should be forced to 1 jtflags;
enum {
    VCVTIP  = 0xc,             // bits 2-3 should always be set, indicating that a converted argument can be inplaced
    VARGX   = 4,               // bit position for arg flags
    VBB     = (B01 << VARGX),  // convert arguments to B
    VII     = (INT << VARGX),  // convert arguments to I
    VDD     = (FL << VARGX),   // convert arguments to D
    VZZ     = (CMPX << VARGX), // convert arguments to Z
    Vxx     = (XNUM << VARGX), // convert arguments to XNUM
    VQQ     = (RAT << VARGX),  // convert arguments to RAT
    VARGMSK = (VBB | VII | VDD | VZZ | Vxx | VQQ),  // mask for argument requested type
    VRESX   = 12,                                   // bit position for result flags
    VB      = (B01 << VRESX),                       // result type B  bit 12
    VI      = (INT << VRESX),                       // result type I  bit 14
    VD      = (FL << VRESX),                        // result type D  bit 15
    VZ      = (CMPX << VRESX),                      // result type Z bit 16
    VX      = (XNUM << VRESX),                      // result type XNUM  bit 18
    VQ      = (RAT << VRESX),                       // result type RAT  bit 19
    VSB     = (SBT << VRESX),                       // result type SBT bit 28
    VRESMSK = (VB | VI | VD | VZ | VX | VQ | VSB),  // mask for result-type
    VRD     = (SLIT << VRESX),                      // convert result to D if possible - unused code point
    VRI     = (SBOX << VRESX),                      // convert result to I if possible - unused code point
    // bits VRESX+ 1 10 11 12 are free
    VIPWFLONGX   = (63),  // (must be >RANKTX) internal use in va2.  We use sign bit where possible
    VIPWFLONG    = ((I)1 << VIPWFLONGX),
    VIPOKWX      = 20,  // This routine can put its result over W
    VIPOKW       = ((I)1 << VIPOKWX),
    VIPOKAX      = 21,  // This routine can put its result over A
    VIPOKA       = ((I)1 << VIPOKAX),
    VCANHALTX    = 25,  // This routine can generate an error after it has started
    VCANHALT     = ((I)1 << VCANHALTX),
    VXCHASVTYPEX = 26,  // set if there is forced conversion to XNUM
    VXCHASVTYPE  = ((I)1 << VXCHASVTYPEX),
    VXCVTYPEX    = 29,                   // bit position for VX conversion type
    VXCVTYPEMSK  = ((I)3 << VXCVTYPEX),  // mask for bit-positions hold XNUM conversion type - leave here where they
                                         // don't overlap noun types
    VIPWCRLONGX = 31,                                               // internal use in va2, must be sign bit
    VIPWCRLONG  = ((I)1 << VIPWCRLONGX),
    // bit 31 must not be used - it may be a sign bit, which has a meaning
    VARGCVTMSKF = (VXCHASVTYPE | VXCVTYPEMSK),               // mask for type to pass into XCVT, includes XNUM override
    VFRCEXMT    = (VXCHASVTYPE | ((I)XMEXMT << VXCVTYPEX)),  // set in arg to jtcvt(jt,) to do rounding for = ~:, if the
                                                             // conversion happens to be to XNUM
    // upper bits for 64-bit va2
    VIPOKRNKWX = 30,  // filled by va2 if the ranks allow inplacing w
    VIPOKRNKW  = ((I)1 << VIPOKRNKWX),
    VIPOKRNKAX = 32,  // filled by va2 if the ranks allow inplacing a
    VIPOKRNKA  = ((I)1 << VIPOKRNKAX),
};

#define VXX  (Vxx | VXCHASVTYPE | ((I)XMEXACT << VXCVTYPEX))  // exact conversion
#define VXEQ (Vxx | VXCHASVTYPE | ((I)XMEXMT << VXCVTYPEX))   // convert to XNUM for = ~:
#define VXCF (Vxx | VXCHASVTYPE | ((I)XMCEIL << VXCVTYPEX))   // convert to XNUM ceiling/floor
#define VXFC (Vxx | VXCHASVTYPE | ((I)XMFLR << VXCVTYPEX))    // convert to XNUM floor/ceiling

// Extract the argument-conversion type from cv coming from the table
#define atype(x) (((x)&VARGMSK)>>VARGX)

// Extract the result type from cv coming from the table
#define rtype(x) (((x)&VRESMSK)>>VRESX)

#define NOT(v)          ((v)^0x0101010101010101)

#define SNOT(v)         ((v)^0x0101)
#define INOT(v)         ((v)^0x01010101)

#define AND(u,v)        ( u &  v)
#define GT(u,v)         ( u & ~v)
#define LT(u,v)         (~u &  v)
#define OR(u,v)         ( u |  v)
#define NE(u,v)         ( u ^  v)
#define GE(u,v)         NOT( LT(u,v))
#define LE(u,v)         NOT( GT(u,v))
#define EQ(u,v)         NOT( NE(u,v))
#define NOR(u,v)        NOT( OR(u,v))
#define NAND(u,v)       NOT(AND(u,v))

#define IAND            AND
#define IGT             GT
#define ILT             LT
#define IOR             OR
#define INE             NE
#define IGE(u,v)        INOT(ILT(u,v))
#define ILE(u,v)        INOT(IGT(u,v))
#define IEQ(u,v)        INOT(INE(u,v))
#define INOR(u,v)       INOT(IOR(u,v))
#define INAND(u,v)      INOT(IAND(u,v))

#define SAND            AND
#define SGT             GT
#define SLT             LT
#define SOR             OR
#define SNE             NE
#define SGE(u,v)        SNOT(ILT(u,v))
#define SLE(u,v)        SNOT(IGT(u,v))
#define SEQ(u,v)        SNOT(INE(u,v))
#define SNOR(u,v)       SNOT(IOR(u,v))
#define SNAND(u,v)      SNOT(IAND(u,v))

// comparisons one value at a time
#define BAND(u,v)       (u && v)
#define BGT(u,v)        (u >  v)
#define BLT(u,v)        (u <  v)
#define BOR(u,v)        (u || v)
#define BNE(u,v)        (u != v)
#define BGE(u,v)        (u >= v)
#define BLE(u,v)        (u <= v)
#define BEQ(u,v)        (u == v)
#define BNOR(u,v)       (!(u||v))
#define BNAND(u,v)      (!(u&&v))

// comparisons between LIT types, one word at a time producing bits in v.  work is destroyed
#define CMPEQCC(u,v)    (work=(u)^(v), ZBYTESTOZBITS(work), work=~work, work&=VALIDBOOLEAN)
#define CMPNECC(u,v)    (work=(u)^(v), ZBYTESTOZBITS(work), work&=VALIDBOOLEAN)

#define PLUS(u,v)       ((u)+   (v))
#define PLUSO(u,v)      ((u)+(D)(v))
#define MINUS(u,v)      ((u)-   (v))
#define MINUSO(u,v)     ((u)-(D)(v))
#define TYMES(u,v)      ((u)&&(v)?dmul2(u,v):0)
#define TYMESO(u,v)     ((u)&&(v)?dmul2(u,(D)v):0)
#define DIV(u,v)        ((u)||(v)?ddiv2(u,v):0)

#define SBORDER(v)      (SBUV(v)->order)

#define SBNE(u,v)       (SBORDER(u)!=SBORDER(v))
#define SBLT(u,v)       (SBORDER(u)< SBORDER(v))
#define SBLE(u,v)       (SBORDER(u)<=SBORDER(v))
#define SBGT(u,v)       (SBORDER(u)> SBORDER(v))
#define SBGE(u,v)       (SBORDER(u)>=SBORDER(v))

#define SBMIN(u,v)      (SBORDER(u)<=SBORDER(v)?(u):(v))
#define SBMAX(u,v)      (SBORDER(u)>=SBORDER(v)?(u):(v))

#define BOV(exp)        if(exp){er=EWOV; break;}

#define BW0000(x,y)     (0)
#define BW0001(x,y)     (   (x)& (y) )
#define BW0010(x,y)     (   (x)&~(y) )
#define BW0011(x,y)     (x)

#define BW0100(x,y)     (  ~(x)& (y) )
#define BW0101(x,y)     (y)
#define BW0110(x,y)     (   (x)^ (y) )
#define BW0111(x,y)     (   (x)| (y) )

#define BW1000(x,y)     (~( (x)| (y)))
#define BW1001(x,y)     (~( (x)^ (y)))
#define BW1010(x,y)     (       ~(y) )
#define BW1011(x,y)     (   (x)|~(y) )

#define BW1100(x,y)     (  ~(x)      )
#define BW1101(x,y)     (  ~(x)| (y) )
#define BW1110(x,y)     (~( (x)& (y)))
#define BW1111(x,y)     (-1)

typedef I AHDR1FN(JST * RESTRICT jt,I n,void* z,void* x);
typedef I AHDR2FN(I n,I m,void* RESTRICTI x,void* RESTRICTI y,void* RESTRICTI z,J jt);
typedef I AHDRPFN(I d,I n,I m,void* RESTRICTI x,void* RESTRICTI z,J jt);  // these 3 must be the same for now, for VARPS
typedef I AHDRRFN(I d,I n,I m,void* RESTRICTI x,void* RESTRICTI z,J jt);
typedef I AHDRSFN(I d,I n,I m,void* RESTRICTI x,void* RESTRICTI z,J jt);

#define AMON(f,Tz,Tx,stmt) I f(JST * RESTRICT jt,I n,Tz* z,Tx* x){DQ(n, {stmt} ++z; ++x;); return EVOK;}
#define AMONPS(f,Tz,Tx,prefix,stmt,suffix) I f(JST * RESTRICT jt,I n,Tz* z,Tx* x){prefix DQ(n, {stmt} ++z; ++x;) suffix}
#define HDR1JERR I rc=jt->jerr; jt->jerr=0; return rc?rc:EVOK;   // translate no error to no-error value
#define HDR1JERRNAN I rc=jt->jerr; rc=NANTEST?EVNAN:rc; jt->jerr=0; return rc?rc:EVOK;   // translate no error to no-error value

enum {
    // value in vaptr[]
    VA2CBW0000  = 1,
    VA2CBW0001  = 2,
    VA2CBW0010  = 3,
    VA2CBW0011  = 4,
    VA2CBW0100  = 5,
    VA2CBW0101  = 6,
    VA2CBW0110  = 7,
    VA2CBW0111  = 8,
    VA2CBW1000  = 9,
    VA2CBW1001  = 10,
    VA2CBW1010  = 11,
    VA2CBW1011  = 12,
    VA2CBW1100  = 13,
    VA2CBW1101  = 14,
    VA2CBW1110  = 15,
    VA2CBW1111  = 16,
    VA2CNE      = 17,  // 35
    VA2CDIV     = 18,
    VA2CPLUSCO  = 19,  // 32
    VA2CPLUSDOT = 20,  // 31
    VA2CMINUS   = 21,
    VA2CLT      = 22,
    VA2CEQ      = 23,
    VA2CGT      = 24,
    VA2CSTARDOT = 25,  // 33
    VA2CSTARCO  = 26,  // 34
    VA2CGE      = 27,  // 30
    VA2CLE      = 28,
    // the following are in the same order in va1
    VA2CMIN    = 29,  // 27
    VA2CMAX    = 30,  // 29
    VA2CPLUS   = 31,  // 20
    VA2CSTAR   = 32,  // 19
    VA2CEXP    = 33,  // 25
    VA2CSTILE  = 34,  // 26
    VA2CBANG   = 35,  // 17
    VA2CCIRCLE = 36,
    VA1CMIN    = 29,  // 27
    VA1CMAX    = 30,  // 29
    VA1CPLUS   = 31,  // 20
    VA1CSTAR   = 32,  // 19
    VA1CEXP    = 33,  // 25
    VA1CSTILE  = 34,  // 26
    VA1CBANG   = 35,  // 17
    VA1CCIRCLE = 36,
    VA1CROOT   = 37,
    VA1CLOG    = 38,
};

/*
 b    1 iff cell rank of a <= cell rank of w
 m    # atoms of in the cell with the smaller rank
 n    excess # of cell atoms
 z    pointer to result atoms
 x    pointer to a      atoms
 y    pointer to w      atoms
*/

#define AIFX(f, Tz, Tx, Ty, symb)                                            \
    I f(I n, I m, Tx* RESTRICTI x, Ty* RESTRICTI y, Tz* RESTRICTI z, J jt) { \
        Tx u;                                                                \
        Ty v;                                                                \
        if (n - 1 == 0)                                                      \
            DQ(m, *z++ = *x++ symb * y++;)                                   \
        else if (n - 1 < 0)                                                  \
            DQ(m, u = *x++; DQC(n, *z++ = u symb * y++;))                    \
        else                                                                 \
            DQ(m, v = *y++; DQ(n, *z++ = *x++ symb v;));                     \
        return EVOK;                                                         \
    }

// suff must return the correct result
#define APFX(f, Tz, Tx, Ty, pfx, pref, suff)                                                                     \
    I f(I n, I m, Tx* RESTRICTI x, Ty* RESTRICTI y, Tz* RESTRICTI z, J jt) {                                     \
        Tx u;                                                                                                    \
        Ty v;                                                                                                    \
        pref if (n - 1 == 0) DQ(m, *z++ = pfx(*x, *y); x++; y++;) else if (n - 1 < 0)                            \
          DQ(m, u = *x++; DQC(n, *z++ = pfx(u, *y); y++;)) else DQ(m, v = *y++; DQ(n, *z++ = pfx(*x, v); x++;)); \
        suff                                                                                                     \
    }

// support intolerant comparisons explicitly
#define ACMP0(f, Tz, Tx, Ty, pfx, pfx0)                                       \
    I f(I n, I m, Tx* RESTRICTI x, Ty* RESTRICTI y, B* RESTRICTI z, J jt) {   \
        D u, v;                                                               \
        if (jt->cct != 1.0) {                                                 \
            if (n - 1 == 0)                                                   \
                DQ(m, u = (D)*x++; v = (D)*y++; *z = pfx(u, v); z++;)         \
            else if (n - 1 < 0)                                               \
                DQ(m, u = (D)*x++; DQC(n, v = (D)*y++; *z = pfx(u, v); z++;)) \
            else                                                              \
                DQ(m, v = (D)*y++; DQ(n, u = (D)*x++; *z = pfx(u, v); z++;)); \
        } else {                                                              \
            if (n - 1 == 0)                                                   \
                DQ(m, u = (D)*x++; v = (D)*y++; *z = u pfx0 v; z++;)          \
            else if (n - 1 < 0)                                               \
                DQ(m, u = (D)*x++; DQC(n, v = (D)*y++; *z = u pfx0 v; z++;))  \
            else                                                              \
                DQ(m, v = (D)*y++; DQ(n, u = (D)*x++; *z = u pfx0 v; z++;));  \
        }                                                                     \
        return EVOK;                                                          \
    }

// n and m are never 0.

#define BPFXNOAVX(f, pfx, bpfx, pfyx, bpfyx, fuv, decls, decls256)                                                      \
    I f(I n, I m, void* RESTRICTI x, void* RESTRICTI y, void* RESTRICTI z, J jt) {                                      \
        I u, v;                                                                                                         \
        decls if (n - 1 == 0) {                                                                                         \
            DQ(((m - 1) >> LGSZI), u = *(I*)x; v = *(I*)y; *(I*)z = pfx(u, v); x = (C*)x + SZI; y = (C*)y + SZI;        \
               z = (C*)z + SZI;);                                                                                       \
            u = *(I*)x;                                                                                                 \
            v = *(I*)y;                                                                                                 \
            u = pfx(u, v);                                                                                              \
            STOREBYTES(z, u, (-m) & (SZI - 1));                                                                         \
        }                                                                                                               \
        else if (n - 1 < 0) {                                                                                           \
            n = ~n;                                                                                                     \
            DQ(m, REPLBYTETOW(*(C*)x, u); x = (C*)x + 1;                                                                \
               DQ(((n - 1) >> LGSZI), v = *(I*)y; *(I*)z = pfx(u, v); y = (C*)y + SZI; z = (C*)z + SZI;) v = *(I*)y;    \
               u                                                                                           = pfx(u, v); \
               STOREBYTES(z, u, (-n) & (SZI - 1));                                                                      \
               y = (I*)((UC*)y + (((n - 1) & (SZI - 1)) + 1));                                                          \
               z = (I*)((UC*)z + (((n - 1) & (SZI - 1)) + 1));)                                                         \
        }                                                                                                               \
        else {                                                                                                          \
            DQ(m, REPLBYTETOW(*(C*)y, v); y = (C*)y + 1;                                                                \
               DQ(((n - 1) >> LGSZI), u = *(I*)x; *(I*)z = pfx(u, v); x = (C*)x + SZI; z = (C*)z + SZI;) u = *(I*)x;    \
               u                                                                                           = pfx(u, v); \
               STOREBYTES(z, u, (-n) & (SZI - 1));                                                                      \
               x = (I*)((UC*)x + (((n - 1) & (SZI - 1)) + 1));                                                          \
               z = (I*)((UC*)z + (((n - 1) & (SZI - 1)) + 1));)                                                         \
        }                                                                                                               \
        return EVOK;                                                                                                    \
    }

#define BPFXAVX2(f,pfx,bpfx,pfyx,bpfyx,fuv,decls,decls256) BPFXNOAVX(f,pfx,bpfx,pfyx,bpfyx,fuv,decls,decls256)
#define BPFX(f,pfx,bpfx,pfyx,bpfyx,fuv,decls,decls256) BPFXNOAVX(f,pfx,bpfx,pfyx,bpfyx,fuv,decls,decls256)
