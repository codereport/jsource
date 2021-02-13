/* Copyright 1990-2009, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Aliases for jt (JST* - J Syntax Tree)                                   */
#define BfromD(x, y, z) jtBfromD(jt, (x), (y), (z))
#define DfromZ(x, y, z) jtDfromZ(jt, (x), (y), (z))
#define DXfI(x, y, z) jtDXfI(jt, (x), (y), (z))
#define IfromD(x, y, z) jtIfromD(jt, (x), (y), (z))
#define QfromD(x, y, z) jtQfromD(jt, (x), (y), (z))
#define XfromD(x, y, z) jtXfromD(jt, (x), (y), (z))
#define aaxis(x0, x1, x2, x3, x4, x5, x6, x7) jtaaxis(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7))
#define add2(x, y, z) jtadd2(jt, (x), (y), (z))
#define afrom2(x0, x1, x2, x3) jtafrom2(jt, (x0), (x1), (x2), (x3))
#define aindex(x0, x1, x2, x3) jtaindex(jt, (x0), (x1), (x2), (x3))
#define aindex1(x0, x1, x2, x3) jtaindex1(jt, (x0), (x1), (x2), (x3))
#define am1a(x0, x1, x2, x3) jtam1a(jt, (x0), (x1), (x2), (x3))
#define am1e(x0, x1, x2, x3) jtam1e(jt, (x0), (x1), (x2), (x3))
#define am1sp(x0, x1, x2, x3) jtam1sp(jt, (x0), (x1), (x2), (x3))
#define amendn2(x0, x1, x2, x3) jtamendn2(jt, (x0), (x1), (x2), (x3))
#define amna(x0, x1, x2, x3) jtamna(jt, (x0), (x1), (x2), (x3))
#define amne(x0, x1, x2, x3) jtamne(jt, (x0), (x1), (x2), (x3))
#define amnsp(x0, x1, x2, x3) jtamnsp(jt, (x0), (x1), (x2), (x3))
#define apip(x, y) \
    jtapip((J)((I)jt | JTINPLACEA), (x), (y))  // use apip instead of over when a is an inplaceable context
#define apv(x, y, z) jtapv(jt, (x), (y), (z))
#define apvwr(x, y, z) jtapvwr(jt, (x), (y), (z))
#define assembleresults(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) \
    jtassembleresults(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7), (x8), (x9), (x10))
#define astd1(x, y, z) jtastd1(jt, (x), (y), (z))
#define astdn(x, y, z) jtastdn(jt, (x), (y), (z))
#define atab(x, y, z) jtatab(jt, (x), (y), (z))
#define atcompf(x, y, z) jtatcompf(jt, (x), (y), (z))
#define axbytes1(x0, x1, x2, x3, x4) jtaxbytes1(jt, (x0), (x1), (x2), (x3), (x4))
#define bdot2(x, y, z) jtbdot2(jt, (x), (y), (z))
#define beheadW(x) jtbehead((J)((I)jt | JTINPLACEW), (x))
#define bfi(x, y, z) jtbfi(jt, (x), (y), (z))
#define bitwisecharamp(x0, x1, x2, x3) jtbitwisecharamp(jt, (x0), (x1), (x2), (x3))
#define boxW(x) jtbox((J)((I)jt | JTINPLACEW), (x))
#define box0(x) irs1((x), 0L, 0, jtbox)
#define brep(x, y, z) jtbrep(jt, (x), (y), (z))
#define brephdr(x0, x1, x2, x3) jtbrephdr(jt, (x0), (x1), (x2), (x3))
#define breps(x, y, z) jtbreps(jt, (x), (y), (z))
#define c2j(x, y, z) jtc2j(jt, (x), (y), (z))
#define cants(x, y, z) jtcants(jt, (x), (y), (z))
// if we ensured that setting AFRO always removed inplaceability, we could simplify this test
#define makewritable(x) \
    RZ(x = (AC(x) < (AFLAG(x) << ((BW - 1) - AFROX))) ? x : jtca(jt, x))  // OK if AC is 0x8..1 and AFRO is 0
#define ccvt(x, y, z) jtccvt(jt, (x), (y), (z))
#define cdload(x, y, z) jtcdload(jt, (x), (y), (z))
#define ceil1(x) jtatomic1(jt, (x), ds(CCEIL))
#define center(x0, x1, x2, x3) jtcenter(jt, (x0), (x1), (x2), (x3))
#define cex(x, y, z) jtcex(jt, (x), (y), (z))
#define charmap(x, y, z) jtcharmap(jt, (x), (y), (z))
#define cirx(x0, x1, x2, x3) jtcirx(jt, (x0), (x1), (x2), (x3))
#define congoto(x, y, z) jtcongoto(jt, (x), (y), (z))
#define congotochk(x, y, z) jtcongotochk(jt, (x), (y), (z))
#define conjug(x) jtatomic1(jt, (x), ds(CPLUS))
#define convert0(x0, x1, x2, x3) jtconvert0(jt, (x0), (x1), (x2), (x3))
#define crelocalsyms(x, y, t, v, f) jtcrelocalsyms(jt, (x), (y), (t), (v), (f))
#define cut02(x, y, z) jtcut02(jt, (x), (y), (z))
#define cut2(x, y, z) jtcut2(jt, (x), (y), (z))
#define cut2bx(x, y, z) jtcut2bx(jt, (x), (y), (z))
#define cut2sx(x, y, z) jtcut2sx(jt, (x), (y), (z))
#define dbunquote(x, y, z, w) jtdbunquote(jt, (x), (y), (z), (w))
#define deba(x0, x1, x2, x3) jtdeba(jt, (x0), (x1), (x2), (x3))
#define deflate(x0, x1, x2, x3) jtdeflate(jt, (x0), (x1), (x2), (x3))
#define deflateq(x0, x1, x2, x3) jtdeflateq(jt, (x0), (x1), (x2), (x3))
#define df1(r, x, y) (r = ((r = (y)) ? (FAV(r)->valencefns[0])(jt, (x), r) : r))
#define df2(r, x, y, z) (r = ((r = (z)) ? (FAV(r)->valencefns[1])(jt, (x), (y), r) : r))
#define df2ip(r, x, y, z) (r = ((r = (z)) ? (FAV(r)->valencefns[1])(jtinplace, (x), (y), r) : r))
#define dfs2(x, y, z) jtdfs2(jt, (x), (y), (z))
#define divide(x, y) jtatomic2(jt, (x), (y), ds(CDIV))
#define divideAW(x, y) jtatomic2((J)((I)jt | JTINPLACEA | JTINPLACEW), (x), (y), ds(CDIV))
#define divideW(x, y) jtatomic2((J)((I)jt | JTINPLACEW), (x), (y), ds(CDIV))
#define dolock(x0, x1, x2, x3) jtdolock(jt, (x0), (x1), (x2), (x3))
#define dotprod(x, y, z) jtdotprod(jt, (x), (y), (z))
#define drow(x, y, z) jtdrow(jt, (x), (y), (z))
#define dspell(x, y, z) jtdspell(jt, (x), (y), (z))
#define eachl(x, y, z) jteachl(jt, (x), (y), (z))
#define ebarprep(x0, x1, x2, x3, x4) jtebarprep(jt, (x0), (x1), (x2), (x3), (x4))
#define ecvt(x0, x1, x2, x3, x4) jtecvt(jt, (x0), (x1), (x2), (x3), (x4))
#define efflev(x, y, z) jtefflev(jt, (x), (y), (z))
#define efr(z, ar, r)                                    \
    (z = ((r) > (ar) ? (ar) : (r)) + (REPSGN(r) & (ar)), \
     z = (z < 0) ? 0 : z)  // effective rank: ar is rank of argument, r is rank of verb (may be negative), z becomes
                           // rank of argument cell
#define enqueue(x, y, z) jtenqueue(jt, (x), (y), (z))
#define eq(x, y) jtatomic2(jt, (x), (y), ds(CEQ))
#define eqa(x0, x1, x2) jteqa(jt, (x0), (x1), (x2))
#define eqd(x, y, z) jteqd(jt, (x), (y), (z))
#define eqq(x, y, z) jteqq(jt, (x), (y), (z))
#define eqx(x, y, z) jteqx(jt, (x), (y), (z))
#define equ(x, y) \
    jtequ(jt, (x), (y))  // this macro is used in many other macros. Need to resolve the usage before removal.
#define eqz(x, y, z) jteqz(jt, (x), (y), (z))
#define ev2(x, y, z) jtev2(jt, (x), (y), (z))
#define evc(x, y, z) jtevc(jt, (x), (y), (z))
#define every2(x0, x1, x2) jtevery2(jt, (x0), (x1), (x2))
#define exec2q(x0, x1, x2, x3, x4) jtexec2q(jt, (x0), (x1), (x2), (x3), (x4))
#define exec2r(x0, x1, x2, x3, x4, x5) jtexec2r(jt, (x0), (x1), (x2), (x3), (x4), (x5))
#define exec2x(x0, x1, x2, x3, x4) jtexec2x(jt, (x0), (x1), (x2), (x3), (x4))
#define exec2z(x0, x1, x2, x3, x4) jtexec2z(jt, (x0), (x1), (x2), (x3), (x4))
#define expn1(x) jtatomic1(jt, (x), ds(CEXP))
#define expn2(x, y) jtatomic2(jt, (x), (y), ds(CEXP))  // bypass sqrt test for internal calls
#define exta(x0, x1, x2, x3) jtexta(jt, (x0), (x1), (x2), (x3))
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
            RZ(z = virtual((w), 0, (r)));                                                                 \
            AFLAG(z) |= AFUNINCORPABLE;                                                                   \
            if ((c) != ACUC1) AC(z) = (c);                                                                \
        }                                                                                                 \
    }
#define fdef(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11) \
    jtfdef(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7), (x8), (x9), (x10), (x11))
#define fillv(x, y, z) jtfillv(jt, (x), (y), (z))
// x is locale number, result is address of symbol table, or 0 if nonexistent locale
// only for non-reuse #define findnlz(x,z)                {if((UI)(n)>=jt->numlocsize)z=0; else
// {z=(A)(jt->numloctbl[n]); z=((UI)((I*)z-jt->numloctbl)<jt->numlocsize)?0:z;}}
#define fitct(x, y, n) jtfitct(jt, (x), (y), (n))
#define floor1(x) jtatomic1(jt, (x), ds(CFLOOR))
#define fmfill(x0, x1, x2, x3, x4, x5, x6, x7) jtfmfill(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7))
#define fminit(x0, x1, x2, x3, x4, x5, x6) jtfminit(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6))
#define fmt1(x0, x1, x2, x3, x4, x5) jtfmt1(jt, (x0), (x1), (x2), (x3), (x4), (x5))
#define fmtallcol(x, y, z) jtfmtallcol(jt, (x), (y), (z))
#define fmtcomma(x0, x1, x2, x3) jtfmtcomma(jt, (x0), (x1), (x2), (x3))
#define fmtex(x0, x1, x2, x3, x4, x5, x6, x7) jtfmtex(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7))
#define fmtq(x0, x1, x2, x3, x4, x5) jtfmtq(jt, (x0), (x1), (x2), (x3), (x4), (x5))
#define fmtx(x0, x1, x2, x3, x4, x5) jtfmtx(jt, (x0), (x1), (x2), (x3), (x4), (x5))
#define fmtxi(x0, x1, x2, x3) jtfmtxi(jt, (x0), (x1), (x2), (x3))
#define folk(x, y, z) jtfolk(jt, (x), (y), (z))
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
#define fram(x0, x1, x2, x3, x4) jtfram(jt, (x0), (x1), (x2), (x3), (x4))
#define frombs1(x, y, z) jtfrombs1(jt, (x), (y), (z))
#define frombsn(x, y, z) jtfrombsn(jt, (x), (y), (z))
#define frombu(x, y, z) jtfrombu(jt, (x), (y), (z))
#define fromis1(x0, x1, x2, x3) jtfromis1(jt, (x0), (x1), (x2), (x3))
#define fslashatg(x, y, z) jtfslashatg(jt, (x), (y), (z))
#define fsm0(x, y, z) jtfsm0(jt, (x), (y), (z))
#define fsmdo(x0, x1, x2, x3, x4, x5) jtfsmdo(jt, (x0), (x1), (x2), (x3), (x4), (x5))
#define fx(x) jtfx(jt, (x), 0L)
#define ga(x0, x1, x2, x3) jtga(jt, (x0), (x1), (x2), (x3))
#define gc3(x0, x1, x2, x3) jtgc3(jt, (x0), (x1), (x2), (x3))
#define gcd(x, y) jtatomic2(jt, (x), (y), ds(CPLUSDOT))
#define gconj(x, y, z) jtgconj(jt, (x), (y), (z))
#define ge(x, y) jtatomic2(jt, (x), (y), ds(CGE))
#define grd1spdd(x, y, z) jtgrd1spdd(jt, (x), (y), (z))
#define grd1spds(x, y, z) jtgrd1spds(jt, (x), (y), (z))
#define grd1spsd(x, y, z) jtgrd1spsd(jt, (x), (y), (z))
#define grd1spss(x, y, z) jtgrd1spss(jt, (x), (y), (z))
#define grd1spz(x, y, z) jtgrd1spz(jt, (x), (y), (z))
#define grd2spsd(x, y, z) jtgrd2spsd(jt, (x), (y), (z))
#define grd2spss(x, y, z) jtgrd2spss(jt, (x), (y), (z))
#define gri(x0, x1, x2, x3, x4) jtgri(jt, (x0), (x1), (x2), (x3), (x4))
#define gri1(x0, x1, x2, x3, x4) jtgri1(jt, (x0), (x1), (x2), (x3), (x4))
#define gru1(x0, x1, x2, x3, x4) jtgru1(jt, (x0), (x1), (x2), (x3), (x4))
#define grx(x0, x1, x2, x3, x4) jtgrx(jt, (x0), (x1), (x2), (x3), (x4))
#define gt(x, y) jtatomic2(jt, (x), (y), ds(CGT))
#define hgd(x0, x1, x2, x3, x4) jthgd(jt, (x0), (x1), (x2), (x3), (x4))
#define hgeom2(x, y, z) jthgeom2(jt, (x), (y), (z))
#define hgv(x0, x1, x2, x3) jthgv(jt, (x0), (x1), (x2), (x3))
#define hparm(x, y, z) jthparm(jt, (x), (y), (z))
#define hrep(x, y, z) jthrep(jt, (x), (y), (z))
#define iaddr(x0, x1, x2, x3) jtiaddr(jt, (x0), (x1), (x2), (x3))
#define idenv0(x0, x1, x2, x3, x4) jtidenv0(jt, (x0), (x1), (x2), (x3), (x4))
#define iixBX(x0, x1, x2, x3, x4) jtiixBX(jt, (x0), (x1), (x2), (x3), (x4))
#define iixI(x0, x1, x2, x3, x4) jtiixI(jt, (x0), (x1), (x2), (x3), (x4))
#define indexofprehashed(x, y, z) jtindexofprehashed(jt, (x), (y), (z))
#define indexofss(x, y, z) jtindexofss(jt, (x), (y), (z))
#define indexofsub(x, y, z) jtindexofsub(jt, (x), (y), (z))
#define indexofxx(x, y, z) jtindexofxx(jt, (x), (y), (z))
#define infix(x, y, z) jtinfix(jt, (x), (y), (z))
#define inpl(x, y, z) jtinpl(jt, (x), (y), (z))
#define inv(x) jtinv(jt, (x), 0)
#define invrecur(x) jtinv(jt, (x), 1)  // call inv(), indicating recursive call
#define iocol(x, y, z) jtiocol(jt, (x), (y), (z))
#define iopart(x0, x1, x2, x3, x4, x5, x6) jtiopart(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6))
#define ioresparse(x, y, z) jtioresparse(jt, (x), (y), (z))
#define iovsd(x, y, z) jtiovsd(jt, (x), (y), (z))
#define iovxs(x, y, z) jtiovxs(jt, (x), (y), (z))
#define ipart(x0, x1, x2, x3) jtipart(jt, (x0), (x1), (x2), (x3))
#define ipbx(x0, x1, x2, x3) jtipbx(jt, (x0), (x1), (x2), (x3))
#define ipprep(x0, x1, x2, x3, x4, x5) jtipprep(jt, (x0), (x1), (x2), (x3), (x4), (x5))
#define irs1(x0, x1, x2, x3) jtirs1(jt, (x0), (x1), (x2), (x3))
#define irs2(x0, x1, x2, x3, x4, x5) jtirs2(jt, (x0), (x1), (x2), (x3), (x4), (x5))
#define ixin(x0, x1, x2, x3) jtixin(jt, (x0), (x1), (x2), (x3))
#define jprx(x0, x1, x2, x3, x4) jtjprx(jt, (x0), (x1), (x2), (x3), (x4))
#define jsignal3(x, y, z) jtjsignal3(jt, (x), (y), (z))
#define jsigstr(x, y, z) jtjsigstr(jt, (x), (y), (z))
#define jstd(x, y, z) jtjstd(jt, (x), (y), (z))
#define key(x, y, z) jtkey(jt, (x), (y), (z))
#define keysp(x, y, z) jtkeysp(jt, (x), (y), (z))
#define keytally(x, y, z) jtkeytally(jt, (x), (y), (z))
#define laguerre(x, y, z) jtlaguerre(jt, (x), (y), (z))
#define lbox(x) jtlbox(jtinplace, (x), ltext)
#define lchar(x) jtlchar(jtinplace, (x), ltext)
#define lcm(x, y) jtatomic2(jt, (x), (y), ds(CSTARDOT))
#define lcolon(x) jtlcolon(jtinplace, (x), ltext)
#define le(x, y) jtatomic2(jt, (x), (y), ds(CLE))
#define lev2(x, y, z) jtlev2(jt, (x), (y), (z))
#define levs2(x, y, z) jtlevs2(jt, (x), (y), (z))
#define line(x0, x1, x2, x3) jtline(jt, (x0), (x1), (x2), (x3))
#define linf(x0, x1, x2, x3) jtlinf(jt, (x0), (x1), (x2), (x3))
#define link(x, y) jtlink(jt, (x), (y), UNUSED_VALUE)
#define linsert(x, y) jtlinsert(jtinplace, (x), (y), ltext)
#define lnoun(x) jtlnoun(jtinplace, (x), ltext)
#define lnoun0(x) jtlnoun0(jtinplace, (x), ltext)
#define lnum(x) jtlnum(jtinplace, (x), ltext)
#define lnum1(x) jtlnum1(jtinplace, (x), ltext)
#define locindirect(x, y, z) jtlocindirect(jt, (x), (y), (z))
#define logar1(x) jtatomic1(jt, (x), ds(CLOG))
#define lrv(x) ((UI)((x)->lrr) >> RANKTX)  // left rank of V
#define lr(x) lrv(FAV(x))                  // left rank of A
#define lr2(x, y, z) jtlr2(jt, (x), (y), (z))
#define lrr(x) jtlrr(jtinplace, (x), 0L, ltext)
#define lsh(x) jtlsh(jtinplace, (x), ltext)
#define lshape(x) jtlshape(jtinplace, (x), ltext)
#define lsparse(x) jtlsparse(jtinplace, (x), ltext)
#define lsub(x, y, z) jtlsub(jt, (x), (y), (z))
#define lsymb(x, y) jtlsymb(jtinplace, (x), (y), ltext)
#define lt(x, y) jtatomic2(jt, (x), (y), ds(CLT))
#define mag(x) jtatomic1(jt, (x), ds(CSTILE))
#define matchsub(x0, x1, x2, x3, x4, x5, x6, x7) jtmatchsub(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7))
#define maximum(x, y) jtatomic2(jt, (x), (y), ds(CMAX))
// typepriority is 0, 1, 4, 9, 10, 5, 6, 7, 8, 2, 3
// prioritytype is B01X, LITX, C2TX, C4TX, INTX, BOXX, XNUMX, RATX, SBTX, FLX, CMPXX
//                 00000 00001 10001 10010 00010 00101 00110  00111 10000 00011 00100
// reversed        001 0000 0111 0000 0011 1001 1000 1010 0010 1001 0100 0100 0010 0000
#define maxtype(x, y) (((x) == (y)) ? (x) : jtmaxtype(jt, x, y))
#define maxtypedne(x, y) (jt->typepriority[CTTZ(x)] > jt->typepriority[CTTZ(y)] ? (x) : (y))
#define maxtyped(x, y) (((x) == (y)) ? (x) : maxtypedne(x, y))
// For sparse types, we encode here the corresponding dense type
#define memoget(x, y, z) jtmemoget(jt, (x), (y), (z))
#define memoput(x0, x1, x2, x3) jtmemoput(jt, (x0), (x1), (x2), (x3))
#define merge2(x0, x1, x2, x3, x4) jtmerge2(jt, (x0), (x1), (x2), (x3), (x4))
#define minimum(x, y) jtatomic2(jt, (x), (y), ds(CMIN))
#define minus(x, y) jtatomic2(jt, (x), (y), ds(CMINUS))
#define minusA(x, y) jtatomic2((J)((I)jt | JTINPLACEA), (x), (y), ds(CMINUS))
#define mmharvest(x0, x1, x2, x3, x4, x5, x6) jtmmharvest(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6))
#define mmprep(x0, x1, x2, x3, x4, x5) jtmmprep(jt, (x0), (x1), (x2), (x3), (x4), (x5))
#define move(x, y, z) jtmove(jt, (x), (y), (z))
#define movandor(x0, x1, x2, x3) jtmovandor(jt, (x0), (x1), (x2), (x3))
#define movbwneeq(x0, x1, x2, x3) jtmovbwneeq(jt, (x0), (x1), (x2), (x3))
#define movfslash(x, y, z) jtmovfslash(jt, (x), (y), (z))
#define movminmax(x0, x1, x2, x3) jtmovminmax(jt, (x0), (x1), (x2), (x3))
#define movneeq(x0, x1, x2, x3) jtmovneeq(jt, (x0), (x1), (x2), (x3))
#define movsumavg(x0, x1, x2, x3) jtmovsumavg(jt, (x0), (x1), (x2), (x3))
#define movsumavg1(x0, x1, x2, x3) jtmovsumavg1(jt, (x0), (x1), (x2), (x3))
#define mr(x) ((I)(FAV(x)->mr))
#define msmerge(x, y, z) jtmsmerge(jt, (x), (y), (z))
#define msort(x, y, z) jtmsort(jt, (x), (y), (z))
#define msortitems(w, x, y, z) jtmsortitems(jt, (w), (x), (y), (z))
#define mult(x, y) \
    jtmult(jt, (x), (y))  // there is a second macro in dtoa.c so when we mass remove one it hits both of them.
#define mvw(x0, x1, x2, x3, x4, x5, x6) jtmvw(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6))
#define nch1(x0, x1, x2, x3) jtnch1(jt, (x0), (x1), (x2), (x3))
#define ne(x, y) jtatomic2(jt, (x), (y), ds(CNE))
#define negateW(x) jtnegate((J)((I)jt | JTINPLACEW), (x))
#define newt(x0, x1, x2, x3) jtnewt(jt, (x0), (x1), (x2), (x3))
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
#define odom(x, y, z) jtodom(jt, (x), (y), (z))
#define ofxassoc(x, y, z) jtofxassoc(jt, (x), (y), (z))
#define opes(x, y, z) jtopes(jt, (x), (y), (z))
#define opes1(x0, x1, x2, x3, x4, x5) jtopes1(jt, (x0), (x1), (x2), (x3), (x4), (x5))
#define opes2(x0, x1, x2, x3, x4, x5, x6) jtopes2(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6))
#define outfix(x, y, z) jtoutfix(jt, (x), (y), (z))
#define ovgmove(x0, x1, x2, x3, x4, x5, x6) jtovgmove(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6))
#define ovs0(x0, x1, x2, x3) jtovs0(jt, (x0), (x1), (x2), (x3))
#define pad(x, y, z) jtpad(jt, (x), (y), (z))
#define parsex(x0, x1, x2, x3) jtparsex(jt, (x0), (x1), (x2), (x3))
#define pee(a, b, c, d, e) jtpee(jt, (a), (b), (c), (d), (e))
#define pix(x) jtpix(jt, (x), ds(CCIRCLE))
#define plus(x, y) jtatomic2(jt, (x), (y), ds(CPLUS))
#define plusA(x, y) jtatomic2((J)((I)jt | JTINPLACEA), (x), (y), ds(CPLUS))
#define plusW(x, y) jtatomic2((J)((I)jt | JTINPLACEW), (x), (y), ds(CPLUS))
#define pmrecord(x0, x1, x2, x3) jtpmrecord(jt, (x0), (x1), (x2), (x3))
#define polymult(x, y, z) jtpolymult(jt, (x), (y), (z))
#define powop(x, y, z) jtpowop(jt, (x), (y), (z))
#define preparse(x, y, z) jtpreparse(jt, (x), (y), (z))
#define probe(x, y, z, w) jtprobe(jt, (x), (y), (z), (w))
#define probedel(x, y, z, w) jtprobedel(jt, (x), (y), (z), (w))
#define pscangt(x0, x1, x2, x3, x4, x5) jtpscangt(jt, (x0), (x1), (x2), (x3), (x4), (x5))
#define pscanlt(x0, x1, x2, x3, x4, x5) jtpscanlt(jt, (x0), (x1), (x2), (x3), (x4), (x5))
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
#define rank1ex(x0, x1, x2, x3) jtrank1ex(jt, (x0), (x1), (x2), (x3))
#define rank1ex0(x0, x1, x2) jtrank1ex0(jt, (x0), (x1), (x2))
#define REX2R(lr, rr, lcr, rcr) (((I)(lr) << RANKTX) + (I)(rr) + ((((I)(lcr) << RANKTX) + (I)(rcr)) << 2 * RANKTX))
#define rank2ex(x0, x1, x2, x3, x4, x5, x6, x7) jtrank2ex(jt, (x0), (x1), (x2), REX2R((x3), (x4), (x5), (x6)), (x7))
#define rank2ex0(x0, x1, x2, x3) jtrank2ex0(jt, (x0), (x1), (x2), (x3))
#define rankingb(x0, x1, x2, x3, x4, x5) jtrankingb(jt, (x0), (x1), (x2), (x3), (x4), (x5))
// ras does rifv followed by ra
#define ras(x) ((x) = jtras(jt, x))
#define razecut2(x, y, z) jtrazecut2(jt, (x), (y), (z))
#define razeg(x0, x1, x2, x3, x4, x5) jtrazeg(jt, (x0), (x1), (x2), (x3), (x4), (x5))
#define rc(x, y, z, t) jtrc(jt, (x), (y), (z), (t))
#define rd(x, y, z) jtrd(jt, (x), (y), (z))
#define realizeifvirtual(x)                                          \
    {                                                                \
        if ((AFLAG(x) & AFVIRTUAL) != 0) RZ((x) = jtrealize(jt, x)); \
    }
#define rifv(x) realizeifvirtual(x)
// We have used rifvs liberally through the code to guarantee that all functions can deal with virtual blocks returned.
// In some cases, the call is to an internal routine that we know will not return a virtual block normally, and is in an
// important performance path.  We use rifvsdebug for these places.  rifvs is called only during debugging.  Review them
// from time to time.
#define redcatsp(x, y, z) jtredcatsp(jt, (x), (y), (z))
#define redg(x, y) jtredg(jtinplace, (x), (y))
#define redsp1(x0, x1, x2, x3, x4, x5, x6, x7) jtredsp1(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7))
#define redsp1a(x0, x1, x2, x3, x4, x5) jtredsp1a(jt, (x0), (x1), (x2), (x3), (x4), (x5))
#define redspd(x0, x1, x2, x3, x4, x5, x6, x7) jtredspd(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7))
#define redsps(x0, x1, x2, x3, x4, x5, x6, x7) jtredsps(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7))
#define redspse(x0, x1, x2, x3, x4, x5, x6, x7) jtredspse(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7))
#define redspsprep(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12) \
    jtredspsprep(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7), (x8), (x9), (x10), (x11), (x12))
#define rep1d(x0, x1, x2, x3) jtrep1d(jt, (x0), (x1), (x2), (x3))
#define rep1sa(x, y, z) jtrep1sa(jt, (x), (y), (z))
#define repidx(x0, x1, x2, x3) jtrepidx(jt, (x0), (x1), (x2), (x3))
#define repzdx(x0, x1, x2, x3) jtrepzdx(jt, (x0), (x1), (x2), (x3))
#define reshapeW(x, y) jtreshape((J)((I)jt | JTINPLACEW), (x), (y))
#define reshapesp(x0, x1, x2, x3) jtreshapesp(jt, (x0), (x1), (x2), (x3))
#define reshapesp0(x0, x1, x2, x3) jtreshapesp0(jt, (x0), (x1), (x2), (x3))
#define residue(x, y) jtresidue(jt, (x), (y), ds(CSTILE))
#define rfcq(x0, x1, x2, x3) jtrfcq(jt, (x0), (x1), (x2), (x3))
#define rngstates1(x0, x1, x2, x3, x4, x5, x6) jtrngstates1(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6))
#define rollk(x, y, z) jtrollk(jt, (x), (y), (z))
#define rot(x0, x1, x2, x3, x4, x5, x6, x7) jtrot(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7))
#define rrv(x) ((UI)((x)->lrr) & RANKTMSK)  // rr of V
#define rr(x) rrv(FAV(x))                   // rr of A
#define sbcheck1(x0, x1, x2, x3, x4, x5, x6, x7) jtsbcheck1(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7))
#define sbextend(x0, x1, x2, x3) jtsbextend(jt, (x0), (x1), (x2), (x3))
#define sbinsert(x0, x1, x2, x3, x4, x5) jtsbinsert(jt, (x0), (x1), (x2), (x3), (x4), (x5))
#define sbprobe(x0, x1, x2, x3) jtsbprobe(jt, (x0), (x1), (x2), (x3))
#define scansp(x, y, z) jtscansp(jt, (x), (y), (z))
#define scuba(x, y, z) jtscuba(jt, (x), (y), (z))
#define scubc(x, y, z) jtscubc(jt, (x), (y), (z))
#define scube(x, y, z) jtscube(jt, (x), (y), (z))
#define selx(x, y, z) jtselx(jt, (x), (y), (z))
#define sely(x0, x1, x2, x3) jtsely(jt, (x0), (x1), (x2), (x3))
#define signum(x) jtatomic1(jt, (x), ds(CSTAR))
#define sortb2(x0, x1, x2) jtsortb2(jt, (x0), (x1), (x2))
#define sortb4(x0, x1, x2) jtsortb4(jt, (x0), (x1), (x2))
#define sortc2(x0, x1, x2) jtsortc2(jt, (x0), (x1), (x2))
#define sortu1(x0, x1, x2) jtsortu1(jt, (x0), (x1), (x2))
#define sorti1(x0, x1, x2) jtsorti1(jt, (x0), (x1), (x2))
#define sparse1a(x0, x1, x2, x3, x4) jtsparse1a(jt, (x0), (x1), (x2), (x3), (x4))
#define sparseit(x, y, z) jtsparseit(jt, (x), (y), (z))
#define spdscell(x0, x1, x2, x3, x4) jtspdscell(jt, (x0), (x1), (x2), (x3), (x4))
#define spmult(x0, x1, x2, x3, x4, x5, x6, x7) jtspmult(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7))
#define spradv(x0, x1, x2, x3, x4, x5, x6) jtspradv(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6))
#define sprank1(x0, x1, x2, x3) jtsprank1(jt, (x0), (x1), (x2), (x3))
#define sprank2(x0, x1, x2, x3, x4, x5) jtsprank2(jt, (x0), (x1), (x2), (x3), (x4), (x5))
#define sprank2_0w(x0, x1, x2, x3, x4, x5) jtsprank2_0w(jt, (x0), (x1), (x2), (x3), (x4), (x5))
#define sprank2_a0(x0, x1, x2, x3, x4, x5) jtsprank2_a0(jt, (x0), (x1), (x2), (x3), (x4), (x5))
#define spredge(x, y, z) jtspredge(jt, (x), (y), (z))
#define sprinit(x0, x1, x2, x3, x4) jtsprinit(jt, (x0), (x1), (x2), (x3), (x4))
#define sprintfeD(x0, x1, x2, x3, x4) jtsprintfeD(jt, (x0), (x1), (x2), (x3), (x4))
#define sprintfnD(x0, x1, x2, x3, x4) jtsprintfnD(jt, (x0), (x1), (x2), (x3), (x4))
#define sprintfI(x0, x1, x2, x3, x4) jtsprintfI(jt, (x0), (x1), (x2), (x3), (x4))
#define sprz(x0, x1, x2, x3, x4) jtsprz(jt, (x0), (x1), (x2), (x3), (x4))
#define spspd(x0, x1, x2, x3) jtspspd(jt, (x0), (x1), (x2), (x3))
#define spspx(x0, x1, x2, x3) jtspspx(jt, (x0), (x1), (x2), (x3))
#define spsscell(x0, x1, x2, x3, x4) jtspsscell(jt, (x0), (x1), (x2), (x3), (x4))
#define sqroot(x) jtatomic1(jt, (x), ds(CROOT))
#define ssdo(x, y, z) jtssdo(jt, (x), (y), (z))
#define stcreate(x0, x1, x2, x3) jtstcreate(jt, (x0), (x1), (x2), (x3))
#define stfind(x, y, z) jtstfind(jt, (x), (y), (z))
#define stfindcre(x, y, z) jtstfindcre(jt, (x), (y), (z))
#define sumatgbool(x, y, z) jtsumatgbool(jt, (x), (y), (z))
#define symbis(x, y, z) jtsymbis(jt, (x), (y), (z))
#define symbisdel(x, y, z) jtsymbisdel(jt, (x), (y), (z))
#define syrd1(x, y, z, w) jtsyrd1(jt, (x), (y), (z), (w))
#define syrd1forlocale(x, y, z, w) jtsyrd1forlocale(jt, (x), (y), (z), (w))
#define tesos(x, y, z, w) jttesos(jt, (x), (y), (z), (w))
#define tess2(x, y, z) jttess2(jt, (x), (y), (z))
#define th2a(x0, x1, x2, x3, x4, x5, x6, x7, x8) jtth2a(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7), (x8))
#define th2c(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9) \
    jtth2c(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7), (x8), (x9))
#define th2ctrl(x0, x1, x2, x3, x4, x5) jtth2ctrl(jt, (x0), (x1), (x2), (x3), (x4), (x5))
#define thv(x, y, z) jtthv(jt, (x), (y), (z))
#define tine(x) jttine(jt, (x), tmonad)
#define tk0(x, y, z) jttk0(jt, (x), (y), (z))
// if jttg(jt,) fails, tpush leaves nextpushx unchanged
// Handle top level of tpush().  push the current block, and recur if it is traversible and does not have recursive
// usecount We can have an inplaceable but recursible block, if it was gc'd.  We never push a PERMANENT block, so that
// we won't try to free it NOTE that PERMANENT blocks are always marked traversible if they are of traversible type, so
// we will not recur on them internally
#define tpushcommon(x, suffix)                                                                 \
    {                                                                                          \
        if (!ACISPERM(AC(x))) {                                                                \
            I tt     = AT(x);                                                                  \
            A *pushp = jt->tnextpushp;                                                         \
            *pushp++ = (x);                                                                    \
            if (!((I)pushp & (NTSTACKBLOCK - 1))) { RZ(pushp = jttg(jt, pushp)); }             \
            if (((tt ^ AFLAG(x)) & TRAVERSIBLE) != 0) RZ(pushp = jttpush(jt, (x), tt, pushp)); \
            jt->tnextpushp = pushp;                                                            \
            suffix                                                                             \
        }                                                                                      \
    }
#define tpush(x) tpushcommon(x, )
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
#define tridiag(x, y, z) jttridiag(jt, (x), (y), (z))
#define tryinit(x, y, z) jttryinit(jt, (x), (y), (z))
#define tymes(x, y) jtatomic2(jt, (x), (y), ds(CSTAR))
#define tymesA(x, y) jtatomic2((J)((I)jt | JTINPLACEA), (x), (y), ds(CSTAR))
#define tymesW(x, y) jtatomic2((J)((I)jt | JTINPLACEW), (x), (y), ds(CSTAR))
#define unbinr(x0, x1, x2, x3, x4) jtunbinr(jt, (x0), (x1), (x2), (x3), (x4))
#define unparse1(x0, x1, x2, x3) jtunparse1(jt, (x0), (x1), (x2), (x3))
#define unparse1a(x, y, z) jtunparse1a(jt, (x), (y), (z))
#define unquote(x, y, z) jtunquote(jt, (x), (y), (z))
#define unwordil(x, y, z) jtunwordil(jt, (x), (y), (z))
#define upon2(x, y, z) jtupon2(jt, (x), (y), (z))
#define usebs(x, y, z) jtusebs(jt, (x), (y), (z))
#define va1s(x0, x1, x2, x3) jtva1s(jt, (x0), (x1), (x2), (x3))
#define var(x0, x1, x2) jtvar(jt, (x0), (x1), (x2))
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
#define vasp(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12) \
    jtvasp(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7), (x8), (x9), (x10), (x11), (x12))
#define vasp0(x0, x1, x2, x3, x4, x5) jtvasp0(jt, (x0), (x1), (x2), (x3), (x4), (x5))
#define vaspc(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12) \
    jtvaspc(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7), (x8), (x9), (x10), (x11), (x12))
#define vaspeq(x0, x1, x2, x3, x4, x5, x6, x7, x8) jtvaspeq(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7), (x8))
#define vaspeqprep(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11) \
    jtvaspeqprep(jt, (x0), (x1), (x2), (x3), (x4), (x5), (x6), (x7), (x8), (x9), (x10), (x11))
#define vec(x, y, z) jtvec(jt, (x), (y), (z))
#define vecb01(x, y, z) jtvecb01(jt, (x), (y), (z))
#define virtual(x, y, z) jtvirtual(jt, (x), (y), (z))
#define virtualip(x, y, z) jtvirtual(jtinplace, (x), (y), (z))
#define wa(x, y, z) jtwa(jt, (x), (y), (z))
#define widthdp(x, y, z) jtwidthdp(jt, (x), (y), (z))
#define words(x) jtwords(jt, (x), ds(CWORDS))
#define xdefn(x, y, z) jtxdefn(jt, (x), (y), (z))
#define xdiv(x, y, z) jtxdiv(jt, (x), (y), (z))
#define xdivrem(x0, x1, x2, x3) jtxdivrem(jt, (x0), (x1), (x2), (x3))
#define xev2(x, y, z) jtxev2(jt, (x), (y), (z))
#define xcompare(x, y) jtxcompare(jt, (x), (y))  // used in multiple macros in multiple files.
#define xop2(x, y, z) jtxop2(jt, (x), (y), (z))
#define xplus(x, y) jtxplus(jt, (x), (y))    // 5 files use this in macros.
#define xtymes(x, y) jtxtymes(jt, (x), (y))  // 4 files use this in macros
#define zdiv(x, y) jtzdiv(jt, (x), (y))      // 3 files' use this in macros
#define zeq(x, y) jtzeq(jt, (x), (y))        // 3 files' use this in macros
#define zhorner(x, y, z) jtzhorner(jt, (x), (y), (z))
#define zminus(x, y) jtzminus(jt, (x), (y))  // 3 files' use this in macros
#define zpad1(x, y, z) jtzpad1(jt, (x), (y), (z))
#define zpadn(x, y, z) jtzpadn(jt, (x), (y), (z))
#define zplus(x, y) jtzplus(jt, (x), (y))  // 4 files' use this in macros
#define zpow(x, y) jtzpow(jt, (x), (y))    // 2 files' use this in macros
#define ztridiag(x, y, z) jtztridiag(jt, (x), (y), (z))
#define ztymes(x, y) jtztymes(jt, (x), (y))  // 4 files' use this in macros
