/* Copyright 1990-2009, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Aliases for jt (JST* - J Syntax Tree)                                   */

#define apip(x, y) \
    jtapip((J)((I)jt | JTINPLACEA), (x), (y))  // use apip instead of over when a is an inplaceable context

#define assembleresults(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) \
    jtassembleresults(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7), (x8), (x9), (x10))

#define beheadW(x) jtbehead((J)((I)jt | JTINPLACEW), (x))
#define boxW(x) jtbox((J)((I)jt | JTINPLACEW), (x))
#define box0(x) jtirs1(jt, (x), 0L, 0, jtbox)

// if we ensured that setting AFRO always removed inplaceability, we could simplify this test
#define makewritable(x) \
    RZ(x = (AC(x) < (AFLAG(x) << ((BW - 1) - AFROX))) ? x : jtca(jt, x))  // OK if AC is 0x8..1 and AFRO is 0

#define ceil1(x) jtatomic1(jt, (x), ds(CCEIL))
#define conjug(x) jtatomic1(jt, (x), ds(CPLUS))

#define df1(r, x, y) (r = ((r = (y)) ? (FAV(r)->valencefns[0])(jt, (x), r) : r))
#define df2(r, x, y, z) (r = ((r = (z)) ? (FAV(r)->valencefns[1])(jt, (x), (y), r) : r))
#define df2ip(r, x, y, z) (r = ((r = (z)) ? (FAV(r)->valencefns[1])(jtinplace, (x), (y), r) : r))
#define divide(x, y) jtatomic2(jt, (x), (y), ds(CDIV))
#define divideAW(x, y) jtatomic2((J)((I)jt | JTINPLACEA | JTINPLACEW), (x), (y), ds(CDIV))
#define divideW(x, y) jtatomic2((J)((I)jt | JTINPLACEW), (x), (y), ds(CDIV))

#define efr(z, ar, r)                                    \
    (z = ((r) > (ar) ? (ar) : (r)) + (REPSGN(r) & (ar)), \
     z = (z < 0) ? 0 : z)  // effective rank: ar is rank of argument, r is rank of verb (may be negative), z becomes
                           // rank of argument cell
#define eq(x, y) jtatomic2(jt, (x), (y), ds(CEQ))
#define equ(x, y) \
    jtequ(jt, (x), (y))  // this macro is used in many other macros. Need to resolve the usage before removal.

#define expn1(x) jtatomic1(jt, (x), ds(CEXP))
#define expn2(x, y) jtatomic2(jt, (x), (y), ds(CEXP))  // bypass sqrt test for internal calls
// Handle top level of fa(), which decrements use count and decides whether recursion is needed.  We recur if the
// contents are traversible and the current block is being decremented to 0 usecount or does not have recursive usecount
// fa() audits the tstack, for use outside the tpop system.  fadecr does just the decrement (for when AC is known > 1)
// Zczero is ~0 if usecount is going negative, 0 otherwise.  Usecount 1->0, 8..1->8..2, 4..0 unchanged, others decrement
#define fadecr(x)  \
    I Zc  = AC(x); \
    AC(x) = Zc = Zc - 1 + ((UI)Zc >> (BW - 2));  // this does the decrement only, checking for PERMANENT
#define faaction(x, nomfaction)                                               \
    {                                                                         \
        fadecr(x) I tt = AT(x);                                               \
        Zc             = REPSGN(Zc - 1);                                      \
        if (((tt &= TRAVERSIBLE) & (Zc | ~AFLAG(x))) != 0) jtfa(jt, (x), tt); \
        if (Zc != 0) { jtmf(jt, x); }                                         \
        nomfaction                                                            \
    }
#define fa(x)                         \
    {                                 \
        if ((x) != 0) faaction((x), ) \
    }
// Within the tpush/tpop when we know the usecount has gone to 0, no need to audit fa, since it was checked on the push
#define fana(x)                  \
    {                            \
        if ((x) != 0) fanano0(x) \
    }
// when x is known to be valid and usecount has gone to 0
#define fanano0(x) faaction((x), )
// Within tpop, no need to check ACISPERM; Zczero is (i. e. usecount has gone to 0) ~0; and we should recur only if flag
// indicates RECURSIBLE.  In that case we can reconstruct the type from the flag
#define fanapop(x, flg)                                               \
    {                                                                 \
        if (((flg)&RECURSIBLE) != 0) jtfa(jt, (x), (flg)&RECURSIBLE); \
        jtmf(jt, x);                                                  \
    }
#define fact(x) jtatomic1(jt, (x), ds(CBANG))
#define fauxblock(z) \
    I z[NORMAH + 4]  // define a block that can be passed in to fauxvirtual.  The 4 is the max rank, and must match
                     // fauxvirtual and fauxplain
#define fauxblockINT(z, n, r) \
    I z[(AKXR(r) >> LGSZI) + (n)]  // define a block, big enough to hold n atoms at rank r, for use in fauxINT
// Allocate an INT block. z is the zvalue to hold the result; v is the fauxblock to use if n INTs will fit in the
// fauxblock, which has rank r shape is not filled in, except when rank is 1; therefore shape will be 0 in any call to
// GATV scaf We should mark the blocks as NJA for good form?
#define fauxINT(z, v, n, r)                      \
    {                                            \
        if (AKXR(r) + (n)*SZI <= (I)sizeof(v)) { \
            z        = (A)(v);                   \
            AK(z)    = AKXR(r);                  \
            AFLAG(z) = 0 /*AFNJA*/;              \
            AT(z)    = INT;                      \
            AC(z)    = ACUC1;                    \
            AN(z)    = (n);                      \
            AR(z)    = (RANKT)(r);               \
            if (r == 1) AS(z)[0] = (n);          \
        } else {                                 \
            GATV0(z, INT, (n), (r));             \
        }                                        \
    }
// v is a block declared by fauxblock, w is the source data, r is the rank.  offset is assumed 0.  c is the initial
// value for AC.  If the rank is small enough, we use the fauxblock, otherwise we allocate a block.  We assume that the
// caller will fill in AN, AS.  Block must be marked UNINCORPABLE so it will not free its backer if freed, and so it
// will not be in-place virtualed, and must be marked recursive if it is of such a type so that if we fa() the block we
// will not try to recur PRISTINE is inherited from the backer (this is not done in virtual(), perhaps it should),
// because we know the block will never be inplaced unless it was inplaceable at the time this fauxblock was created,
// which means it is not extant anywhere it could be assigned or extracted from.
#define fauxvirtual(z, v, w, r, c)                                                                        \
    {                                                                                                     \
        if ((r) <= 4) {                                                                                   \
            z             = ABACK(w);                                                                     \
            AK((A)(v))    = (CAV(w) - (C *)(v));                                                          \
            AT((A)(v))    = AT(w);                                                                        \
            AR((A)(v))    = (RANKT)(r);                                                                   \
            z             = AFLAG(w) & AFVIRTUAL ? z : (w);                                               \
            AFLAG((A)(v)) = AFVIRTUAL | AFUNINCORPABLE | (AFLAG(z) & AFPRISTINE) | (AT(w) & TRAVERSIBLE); \
            ABACK((A)(v)) = z;                                                                            \
            z             = (A)(v);                                                                       \
            AC(z)         = (c);                                                                          \
        } else {                                                                                          \
            RZ(z = jtvirtual(jt, (w), 0, (r)));                                                                 \
            AFLAG(z) |= AFUNINCORPABLE;                                                                   \
            if ((c) != ACUC1) AC(z) = (c);                                                                \
        }                                                                                                 \
    }

#define fdef(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11) \
    jtfdef(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7), (x8), (x9), (x10), (x11))

#define floor1(x) jtatomic1(jt, (x), ds(CFLOOR))

#define fr(x)                    \
    {                            \
        if ((x) != 0) {          \
            I Zs = AC(x);        \
            if (!ACISPERM(Zs)) { \
                if (--Zs <= 0)   \
                    jtmf(jt, x); \
                else             \
                    AC(x) = Zs;  \
            }                    \
        }                        \
    }
#define fx(x) jtfx(jt, (x), 0L)
#define gcd(x, y) jtatomic2(jt, (x), (y), ds(CPLUSDOT))
#define ge(x, y) jtatomic2(jt, (x), (y), ds(CGE))

#define grd1spdd(x, y, z) jtgrd1spdd(jt, (x), (y), (z))
#define grd1spds(x, y, z) jtgrd1spds(jt, (x), (y), (z))
#define grd1spsd(x, y, z) jtgrd1spsd(jt, (x), (y), (z))
#define grd1spss(x, y, z) jtgrd1spss(jt, (x), (y), (z))
#define grd1spz(x, y, z) jtgrd1spz(jt, (x), (y), (z))
#define grd2spsd(x, y, z) jtgrd2spsd(jt, (x), (y), (z))
#define grd2spss(x, y, z) jtgrd2spss(jt, (x), (y), (z))

#define gt(x, y) jtatomic2(jt, (x), (y), ds(CGT))

#define inv(x) jtinv(jt, (x), 0)
#define invrecur(x) jtinv(jt, (x), 1)  // call inv(), indicating recursive call

#define lbox(x) jtlbox(jtinplace, (x), ltext)
#define lchar(x) jtlchar(jtinplace, (x), ltext)
#define lcm(x, y) jtatomic2(jt, (x), (y), ds(CSTARDOT))
#define lcolon(x) jtlcolon(jtinplace, (x), ltext)
#define le(x, y) jtatomic2(jt, (x), (y), ds(CLE))

#define link(x, y) jtlink(jt, (x), (y), UNUSED_VALUE)
#define linsert(x, y) jtlinsert(jtinplace, (x), (y), ltext)
#define lnoun(x) jtlnoun(jtinplace, (x), ltext)
#define lnoun0(x) jtlnoun0(jtinplace, (x), ltext)
#define lnum(x) jtlnum(jtinplace, (x), ltext)
#define lnum1(x) jtlnum1(jtinplace, (x), ltext)
#define logar1(x) jtatomic1(jt, (x), ds(CLOG))
#define lrv(x) ((UI)((x)->lrr) >> RANKTX)  // left rank of V
#define lr(x) lrv(FAV(x))                  // left rank of A
#define lrr(x) jtlrr(jtinplace, (x), 0L, ltext)
#define lsh(x) jtlsh(jtinplace, (x), ltext)
#define lshape(x) jtlshape(jtinplace, (x), ltext)
#define lsparse(x) jtlsparse(jtinplace, (x), ltext)
#define lsymb(x, y) jtlsymb(jtinplace, (x), (y), ltext)
#define lt(x, y) jtatomic2(jt, (x), (y), ds(CLT))
#define mag(x) jtatomic1(jt, (x), ds(CSTILE))
#define maximum(x, y) jtatomic2(jt, (x), (y), ds(CMAX))
// typepriority is 0, 1, 4, 9, 10, 5, 6, 7, 8, 2, 3
// prioritytype is B01X, LITX, C2TX, C4TX, INTX, BOXX, XNUMX, RATX, SBTX, FLX, CMPXX
//                 00000 00001 10001 10010 00010 00101 00110  00111 10000 00011 00100
// reversed        001 0000 0111 0000 0011 1001 1000 1010 0010 1001 0100 0100 0010 0000
#define maxtype(x, y) (((x) == (y)) ? (x) : jtmaxtype(jt, x, y))
#define maxtypedne(x, y) (jt->typepriority[CTTZ(x)] > jt->typepriority[CTTZ(y)] ? (x) : (y))
#define maxtyped(x, y) (((x) == (y)) ? (x) : maxtypedne(x, y))

#define minimum(x, y) jtatomic2(jt, (x), (y), ds(CMIN))
#define minus(x, y) jtatomic2(jt, (x), (y), ds(CMINUS))
#define minusA(x, y) jtatomic2((J)((I)jt | JTINPLACEA), (x), (y), ds(CMINUS))

#define mr(x) ((I)(FAV(x)->mr))
#define msmerge(x, y, z) jtmsmerge(jt, (x), (y), (z))
#define msort(x, y, z) jtmsort(jt, (x), (y), (z))
#define msortitems(w, x, y, z) jtmsortitems(jt, (w), (x), (y), (z))
#define mult(x, y) \
    jtmult(jt, (x), (y))  // there is a second macro in dtoa.c so when we mass remove one it hits both of them.

#define ne(x, y) jtatomic2(jt, (x), (y), ds(CNE))
#define negateW(x) jtnegate((J)((I)jt | JTINPLACEW), (x))
#define nmhash(x, y) hic((x), (y))

#define nodupgrade(x0, x1, x2, x3, x4, x5, x6, x7, x8) \
    jtnodupgrade(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7), (x8))

#define nor(x, y) jtatomic2(jt, (x), (y), ds(CPLUSCO))
#define numb(x0, x1, x2, x3) jtnumb(jt, (x0), (x1), (x2), (x3))
#define numbpx(x, y, z) jtnumbpx(jt, (x), (y), (z))
#define numd(x, y, z) jtnumd(jt, (x), (y), (z))
#define numi(x, y, z) jtnumi(jt, (x), (y), (z))
#define numj(x, y, z) jtnumj(jt, (x), (y), (z))
#define numq(x, y, z) jtnumq(jt, (x), (y), (z))
#define numr(x, y, z) jtnumr(jt, (x), (y), (z))
#define numx(x, y, z) jtnumx(jt, (x), (y), (z))

#define pix(x) jtpix(jt, (x), ds(CCIRCLE))
#define plus(x, y) jtatomic2(jt, (x), (y), ds(CPLUS))
#define plusA(x, y) jtatomic2((J)((I)jt | JTINPLACEA), (x), (y), ds(CPLUS))
#define plusW(x, y) jtatomic2((J)((I)jt | JTINPLACEW), (x), (y), ds(CPLUS))

#define qplus(x, y) jtqplus(jt, (x), (y))    // used by 2 macros in 2 different files.
#define qtymes(x, y) jtqtymes(jt, (x), (y))  // used by 3 macros in 3 different files.
// Handle top level of ra().  Increment usecount.  Set usecount recursive usecount if recursible type; recur on contents
// if original usecount is not recursive We can have an inplaceable but recursible block, if it was gc'd or created that
// way ra() DOES NOT realize a virtual block, so that it can be used in places where virtual blocks are not possible.
// ras() does include rifv
#define ra(x)                                    \
    {                                            \
        I c = AC(x);                             \
        c &= ~ACINPLACE;                         \
        AC(x)     = c += (c >> (BW - 2)) ^ 1;    \
        I tt      = AT(x);                       \
        FLAGT flg = AFLAG(x);                    \
        if (((tt ^ flg) & TRAVERSIBLE) != 0) {   \
            AFLAG(x) = flg |= (tt & RECURSIBLE); \
            jtra((x), tt);                       \
        };                                       \
    }
// If this is a recursible type, make it recursive if it isn't already, by traversing the descendants.  This is like
// raising the usecount by 0.  Since we aren't liable to assign the block, we don't have to realize a virtual block
// unless it is a recursible type.  NOTE that PERMANENT and VIRTUAL blocks are always marked recursible if they are of
// recursible type
#define ra0(x)                                   \
    {                                            \
        I tt      = AT(x);                       \
        FLAGT flg = AFLAG(x);                    \
        if (((tt ^ flg) & RECURSIBLE) != 0) {    \
            if ((flg & AFVIRTUAL) != 0) {        \
                RZ((x) = jtrealize(jt, x));      \
                flg = AFLAG(x);                  \
            }                                    \
            AFLAG(x) = flg |= (tt & RECURSIBLE); \
            jtra((x), tt);                       \
        }                                        \
    }
// make this block recursive, used when x has just been allocated & thus is known to be nonrecursive & nonvirtual.  We
// may know the type t, too (otherwise use AT(x))
#define ra00(x, tt)                      \
    {                                    \
        if (((tt)&RECURSIBLE) != 0) {    \
            AFLAG(x) |= (tt)&RECURSIBLE; \
            jtra((x), (tt));             \
        }                                \
    }
#define REX2R(lr, rr, lcr, rcr) (((I)(lr) << RANKTX) + (I)(rr) + ((((I)(lcr) << RANKTX) + (I)(rcr)) << 2 * RANKTX))
#define rank2ex(x0, x1, x2, x3, x4, x5, x6, x7) jtrank2ex(jt, (x0), (x1), (x2), REX2R((x3), (x4), (x5), (x6)), (x7))
// ras does rifv followed by ra
#define ras(x) ((x) = jtras(jt, x))
#define realizeifvirtual(x)                                          \
    {                                                                \
        if ((AFLAG(x) & AFVIRTUAL) != 0) RZ((x) = jtrealize(jt, x)); \
    }
// We have used rifvs liberally through the code to guarantee that all functions can deal with virtual blocks returned.
// In some cases, the call is to an internal routine that we know will not return a virtual block normally, and is in an
// important performance path.  We use rifvsdebug for these places.  rifvs is called only during debugging.  Review them
// from time to time.
#define redg(x, y) jtredg(jtinplace, (x), (y))

#define redspsprep(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12) \
    jtredspsprep(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7), (x8), (x9), (x10), (x11), (x12))

#define reshapeW(x, y) jtreshape((J)((I)jt | JTINPLACEW), (x), (y))
#define residue(x, y) jtresidue(jt, (x), (y), ds(CSTILE))

#define rrv(x) ((UI)((x)->lrr) & RANKTMSK)  // rr of V
#define rr(x) rrv(FAV(x))                   // rr of A

#define signum(x) jtatomic1(jt, (x), ds(CSTAR))
#define sqroot(x) jtatomic1(jt, (x), ds(CROOT))
#define tine(x) jttine(jt, (x), tmonad)
// if jttg(jt,) fails, tpush leaves nextpushx unchanged
// Handle top level of tpush().  push the current block, and recur if it is traversible and does not have recursive
// usecount We can have an inplaceable but recursible block, if it was gc'd.  We never push a PERMANENT block, so that
// we won't try to free it NOTE that PERMANENT blocks are always marked traversible if they are of traversible type, so
// we will not recur on them internally
#define tpush(x)                                                                               \
    {                                                                                          \
        if (!ACISPERM(AC(x))) {                                                                \
            I tt     = AT(x);                                                                  \
            A *pushp = jt->tnextpushp;                                                         \
            *pushp++ = (x);                                                                    \
            if (!((I)pushp & (NTSTACKBLOCK - 1))) { RZ(pushp = jttg(jt, pushp)); }             \
            if (((tt ^ AFLAG(x)) & TRAVERSIBLE) != 0) RZ(pushp = jttpush(jt, (x), tt, pushp)); \
            jt->tnextpushp = pushp;                                                            \
        }                                                                                      \
    }
// Internal version, used when the local name pushp is known to hold jt->tnextpushp
#define tpushi(x)                                                                              \
    {                                                                                          \
        if (!ACISPERM(AC(x))) {                                                                \
            I tt     = AT(x);                                                                  \
            *pushp++ = (x);                                                                    \
            if (!((I)pushp & (NTSTACKBLOCK - 1))) { RZ(pushp = jttg(jt, pushp)); }             \
            if (((tt ^ AFLAG(x)) & TRAVERSIBLE != 0)) RZ(pushp = jttpush(jt, (x), tt, pushp)); \
        }                                                                                      \
    }
// tpush1 is like tpush, but it does not recur to lower levels.  Used only for virtual block (which cannot be PERMANENT)
#define tpush1(x)                                                              \
    {                                                                          \
        A *pushp = jt->tnextpushp;                                             \
        *pushp++ = (x);                                                        \
        if (!((I)pushp & (NTSTACKBLOCK - 1))) { RZ(pushp = jttg(jt, pushp)); } \
        jt->tnextpushp = pushp;                                                \
    }

#define tymes(x, y) jtatomic2(jt, (x), (y), ds(CSTAR))
#define tymesA(x, y) jtatomic2((J)((I)jt | JTINPLACEA), (x), (y), ds(CSTAR))
#define tymesW(x, y) jtatomic2((J)((I)jt | JTINPLACEW), (x), (y), ds(CSTAR))

// fetch adocv for an rps function (i. e. f/ f/\ f/\.) in self.  rps is 0-2 for / /\ /\.   t is the type of the input.
// Assign result to z, which is a VARPS
#define varps(z, self, t, rps)                                 \
    {                                                          \
        VARPSA *rpsa = FAV(self)->localuse.lvp[1];             \
        I tmax       = rpsa->nprec;                            \
        if (jt->jerr < EWOV) {                                 \
            I zc      = CTTZI(t);                              \
            VARPS *az = &rpsa->actrtns[tmax * (rps) + zc];     \
            az        = zc >= tmax ? &rpsnull.actrtns[0] : az; \
            z         = *az;                                   \
        } else {                                               \
            RESETERR;                                          \
            z = rpsa->actrtns[3 * tmax + (rps)];               \
        }                                                      \
    }
#define vec(x, y, z) jtvec(jt, (x), (y), (z))

#define words(x) jtwords(jt, (x), ds(CWORDS))
#define xcompare(x, y) jtxcompare(jt, (x), (y))  // used in multiple macros in multiple files.
#define xplus(x, y) jtxplus(jt, (x), (y))    // 5 files use this in macros.
#define xtymes(x, y) jtxtymes(jt, (x), (y))  // 4 files use this in macros
#define zdiv(x, y) jtzdiv(jt, (x), (y))      // 3 files' use this in macros
#define zeq(x, y) jtzeq(jt, (x), (y))        // 3 files' use this in macros
#define zminus(x, y) jtzminus(jt, (x), (y))  // 3 files' use this in macros
#define zplus(x, y) jtzplus(jt, (x), (y))  // 4 files' use this in macros
#define zpow(x, y) jtzpow(jt, (x), (y))    // 2 files' use this in macros
#define ztymes(x, y) jtztymes(jt, (x), (y))  // 4 files' use this in macros
