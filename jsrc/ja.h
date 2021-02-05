/* Copyright 1990-2009, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Aliases for jt (JST* - J Syntax Tree)                                   */
#define BfromD(x,y,z)               jtBfromD(jt,(x),(y),(z))
#define DfromZ(x,y,z)               jtDfromZ(jt,(x),(y),(z))
#define DXfI(x,y,z)                 jtDXfI(jt,(x),(y),(z))
#define IfromD(x,y,z)               jtIfromD(jt,(x),(y),(z))

#define QfromD(x,y,z)               jtQfromD(jt,(x),(y),(z))


#define XfromD(x,y,z)               jtXfromD(jt,(x),(y),(z))



#define aaxis(x0,x1,x2,x3,x4,x5,x6,x7)              jtaaxis(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6),(x7))

#define ac2(x)                      jtac2(jt,(x))
#define add2(x,y,z)                 jtadd2(jt,(x),(y),(z))


#define afrom2(x0,x1,x2,x3)         jtafrom2(jt,(x0),(x1),(x2),(x3))

#define aii(x)                      jtaii(jt,(x))
#define aindex(x0,x1,x2,x3)         jtaindex(jt,(x0),(x1),(x2),(x3))
#define aindex1(x0,x1,x2,x3)        jtaindex1(jt,(x0),(x1),(x2),(x3))
#define am1a(x0,x1,x2,x3)           jtam1a(jt,(x0),(x1),(x2),(x3))
#define am1e(x0,x1,x2,x3)           jtam1e(jt,(x0),(x1),(x2),(x3))
#define am1sp(x0,x1,x2,x3)          jtam1sp(jt,(x0),(x1),(x2),(x3))

#define amendn2(x0,x1,x2,x3)        jtamendn2(jt,(x0),(x1),(x2),(x3))
#define amna(x0,x1,x2,x3)           jtamna(jt,(x0),(x1),(x2),(x3))
#define amne(x0,x1,x2,x3)           jtamne(jt,(x0),(x1),(x2),(x3))
#define amnsp(x0,x1,x2,x3)          jtamnsp(jt,(x0),(x1),(x2),(x3))


#define apip(x,y)                   jtapip((J)((I)jt|JTINPLACEA),(x),(y))  // use apip instead of over when a is an inplaceable context
#define apv(x,y,z)                  jtapv(jt,(x),(y),(z))
#define apvwr(x,y,z)                jtapvwr(jt,(x),(y),(z))
#define arep(x)                     jtarep(jt,(x))
#define aro(x)                      jtaro(jt,(x))



#define assembleresults(x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10)   jtassembleresults(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6),(x7),(x8),(x9),(x10))
#define astd1(x,y,z)                jtastd1(jt,(x),(y),(z))
#define astdn(x,y,z)                jtastdn(jt,(x),(y),(z))
#define atab(x,y,z)                 jtatab(jt,(x),(y),(z))

#define atcompf(x,y,z)              jtatcompf(jt,(x),(y),(z))



#define axbytes1(x0,x1,x2,x3,x4)    jtaxbytes1(jt,(x0),(x1),(x2),(x3),(x4))

#define b0(x)                       jtb0(jt,(x))


#define bdot2(x,y,z)                jtbdot2(jt,(x),(y),(z))
#define behead(x)                   jtbehead(jt,(x))
#define beheadW(x)                  jtbehead((J)((I)jt|JTINPLACEW),(x))
#define bfi(x,y,z)                  jtbfi(jt,(x),(y),(z))

#define bindd(x,y)                  jtbindd(jt,(x),(y))
#define binrep1(x)                  jtbinrep1(jt,(x))
#define binzz(x,y)                  jtbinzz(jt,(x),(y))

#define bitwisecharamp(x0,x1,x2,x3) jtbitwisecharamp(jt,(x0),(x1),(x2),(x3))
#define box(x)                      jtbox(jt,(x))
#define boxW(x)                     jtbox((J)((I)jt|JTINPLACEW),(x))
#define box0(x)                     irs1((x),0L,0,jtbox)
#define boxatop(x)                  jtboxatop(jt,(x))
#define boxopen(x)                  jtboxopen(jt,(x))
#define brep(x,y,z)                 jtbrep(jt,(x),(y),(z))
#define brephdr(x0,x1,x2,x3)        jtbrephdr(jt,(x0),(x1),(x2),(x3))
#define breps(x,y,z)                jtbreps(jt,(x),(y),(z))
#define bsdot(x)                    jtbsdot(jt,(x))
#define bslash(x)                   jtbslash(jt,(x))
#define c2fi(x)                     jtc2fi(jt,(x))
#define c2j(x,y,z)                  jtc2j(jt,(x),(y),(z))
#define ca(x)                       jtca(jt,(x))

#define cant1(x)                    jtcant1(jt,(x))


#define cants(x,y,z)                jtcants(jt,(x),(y),(z))
#define cap(x)                      jtcap(jt,(x))
#define car(x)                      jtcar(jt,(x))
#define caro(x)                     jtcaro(jt,(x))
// if we ensured that setting AFRO always removed inplaceability, we could simplify this test
#define makewritable(x)             RZ(x=(AC(x)<(AFLAG(x)<<((BW-1)-AFROX)))?x:ca(x))  // OK if AC is 0x8..1 and AFRO is 0
#define mkwris(x)                   jtmkwris(jt,x)
#define casev(x)                    jtcasev(jt,(x))
#define catalog(x)                  jtcatalog(jt,(x))
#define ccvt(x,y,z)                 jtccvt(jt,(x),(y),(z))
#define cdgahash(x)                 jtcdgahash(jt,(x))
#define cdinit()                    jtcdinit(jt)

#define cdload(x,y,z)               jtcdload(jt,(x),(y),(z))
#define cdlookup(x)                 jtcdlookup(jt,(x))
#define cdlookupl(x)                jtcdlookupl(jt,(x))
#define cdot1(x)                    jtcdot1(jt,(x))


#define ceil1(x)                    jtatomic1(jt,(x),ds(CCEIL))

#define center(x0,x1,x2,x3)         jtcenter(jt,(x0),(x1),(x2),(x3))
#define cex(x,y,z)                  jtcex(jt,(x),(y),(z))
#define cfd(x)                      jtcfd(jt,(x))
#define cfn(x)                      jtcfn(jt,(x))
#define cfr(x)                      jtcfr(jt,(x))
#define charmap(x,y,z)              jtcharmap(jt,(x),(y),(z))
#define cirx(x0,x1,x2,x3)           jtcirx(jt,(x0),(x1),(x2),(x3))
#define clonelocalsyms(x)           jtclonelocalsyms(jt,(x))
#define clonevirtual(z)             jtclonevirtual(jt,(z))
#define coeff(x)                    jtcoeff(jt,(x))

#define colon0(x)                   jtcolon0(jt,(x))


#define congoto(x,y,z)              jtcongoto(jt,(x),(y),(z))

#define congotochk(x,y,z)           jtcongotochk(jt,(x),(y),(z))
#define conjug(x)                   jtatomic1(jt,(x),ds(CPLUS))

#define consinit()                  jtconsinit(jt)

#define convert0(x0,x1,x2,x3)       jtconvert0(jt,(x0),(x1),(x2),(x3))

#define cps(x)                      jtcps(jt,(x))

#define crccompile(x)               jtcrccompile(jt,(x))


#define crelocalsyms(x,y,t,v,f)     jtcrelocalsyms(jt,(x),(y),(t),(v),(f))

#define cstr(x)                     jtcstr(jt,(x))
#define curtail(x)                  jtcurtail(jt,(x))

#define cut02(x,y,z)                jtcut02(jt,(x),(y),(z))
#define cut2(x,y,z)                 jtcut2(jt,(x),(y),(z))
#define cut2bx(x,y,z)               jtcut2bx(jt,(x),(y),(z))
#define cut2sx(x,y,z)               jtcut2sx(jt,(x),(y),(z))

#define cvt0(x)                     jtcvt0(jt,(x))




#define dbjump(x)                   jtdbjump(jt,(x))

#define dbstack(x)                  jtdbstack(jt,(x))


#define dbunquote(x,y,z,w)          jtdbunquote(jt,(x),(y),(z),(w))




#define deba(x0,x1,x2,x3)           jtdeba(jt,(x0),(x1),(x2),(x3))
#define debdisp(x)                  jtdebdisp(jt,(x))
#define debsi1(x)                   jtdebsi1(jt,(x))
#define debug()                     jtdebug(jt)

#define debz()                      jtdebz(jt)
#define deflate(x0,x1,x2,x3)        jtdeflate(jt,(x0),(x1),(x2),(x3))
#define deflateq(x0,x1,x2,x3)       jtdeflateq(jt,(x0),(x1),(x2),(x3))
#define denseit(x)                  jtdenseit(jt,(x))

#define detd(x)                     jtdetd(jt,(x))
#define detmr(x)                    jtdetmr(jt,(x))
#define detr(x)                     jtdetr(jt,(x))

#define detz(x)                     jtdetz(jt,(x))
#define df1(r,x,y)                  (r=((r=(y))?(FAV(r)->valencefns[0])(jt,(x),r):r))
#define df2(r,x,y,z)                (r=((r=(z))?(FAV(r)->valencefns[1])(jt,(x),(y),r):r))
#define df2ip(r,x,y,z)                (r=((r=(z))?(FAV(r)->valencefns[1])(jtinplace,(x),(y),r):r))

#define dfr(x)                      jtdfr(jt,(x))
#define dfrep(x)                    jtdfrep(jt,(x))

#define dfs2(x,y,z)                 jtdfs2(jt,(x),(y),(z))
#define dgamma(x)                   jtdgamma(jt,(x))
#define dgcd(x,y)                   jtdgcd(jt,(x),(y))
#define dgrade1(x)                  jtdgrade1(jt,(x))



#define diff(x)                     jtdiff(jt,(x))

#define divide(x,y)                 jtatomic2(jt,(x),(y),ds(CDIV))
#define divideAW(x,y)               jtatomic2((J)((I)jt|JTINPLACEA|JTINPLACEW),(x),(y),ds(CDIV))
#define divideW(x,y)                jtatomic2((J)((I)jt|JTINPLACEW),(x),(y),ds(CDIV))
#define dlcm(x,y)                   jtdlcm(jt,(x),(y))

#define dloc(x)                     jtdloc(jt,(x))
#define dolock(x0,x1,x2,x3)         jtdolock(jt,(x0),(x1),(x2),(x3))

#define dotprod(x,y,z)              jtdotprod(jt,(x),(y),(z))

#define drep(x)                     jtdrep(jt,(x))


#define drow(x,y,z)                 jtdrow(jt,(x),(y),(z))
#define drr(x)                      jtdrr(jt,(x))
#define dspell(x,y,z)               jtdspell(jt,(x),(y),(z))
#define duble(x)                    jtduble(jt,(x))
#define dx_init(x)                  jtdx_init(jt,(x))
#define dx_next()                   jtdx_next(jt)
#define dx_next30()                 jtdx_next30(jt)
#define eachl(x,y,z)                jteachl(jt,(x),(y),(z))


#define ebarprep(x0,x1,x2,x3,x4)    jtebarprep(jt,(x0),(x1),(x2),(x3),(x4))

#define eca(x0,x1,x2,x3,x4,x5)      jteca(jt,(x0),(x1),(x2),(x3),(x4),(x5))
#define ecd(x0,x1,x2,x3,x4)         jtecd(jt,(x0),(x1),(x2),(x3),(x4))
#define ecm(x0,x1,x2,x3,x4,x5)      jtecm(jt,(x0),(x1),(x2),(x3),(x4),(x5))
#define ecm_s1(x0,x1,x2,x3,x4,x5)   jtecm_s1(jt,(x0),(x1),(x2),(x3),(x4),(x5))
#define ecm_s2(x0,x1,x2,x3,x4,x5,x6)                jtecm_s2(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6))
#define ecvt(x0,x1,x2,x3,x4)        jtecvt(jt,(x0),(x1),(x2),(x3),(x4))
#define efflev(x,y,z)               jtefflev(jt,(x),(y),(z))

#define efr(z,ar,r)                 (z = ((r)>(ar)?(ar):(r))+(REPSGN(r)&(ar)), z=(z<0)?0:z)  // effective rank: ar is rank of argument, r is rank of verb (may be negative), z becomes rank of argument cell
#define enframe(x)                  jtenframe(jt,(x))
#define enqueue(x,y,z)              jtenqueue(jt,(x),(y),(z))


#define eputc(x)                    jteputc(jt,(x))
#define eputl(x)                    jteputl(jt,(x))

#define eputs(x)                    jteputs(jt,(x))
#define eputv(x)                    jteputv(jt,(x))
#define eq(x,y)                     jtatomic2(jt,(x),(y),ds(CEQ))
#define eqa(x0,x1,x2)               jteqa(jt,(x0),(x1),(x2))
#define eqd(x,y,z)                  jteqd(jt,(x),(y),(z))

#define eqq(x,y,z)                  jteqq(jt,(x),(y),(z))
#define eqx(x,y,z)                  jteqx(jt,(x),(y),(z))
#define equ(x,y)                    jtequ(jt,(x),(y))
#define eqz(x,y,z)                  jteqz(jt,(x),(y),(z))
#define etc(x)                      jtetc(jt,(x))
#define ev2(x,y,z)                  jtev2(jt,(x),(y),(z))

#define eval(x)                     jteval(jt,(x))
#define evc(x,y,z)                  jtevc(jt,(x),(y),(z))


#define every2(x0,x1,x2)            jtevery2(jt,(x0),(x1),(x2))


#define evinit()                    jtevinit(jt)
#define ex(x)                       jtex(jt,(x))
#define exec1(x)                    jtexec1(jt,(x))
#define exec2q(x0,x1,x2,x3,x4)      jtexec2q(jt,(x0),(x1),(x2),(x3),(x4))
#define exec2r(x0,x1,x2,x3,x4,x5)   jtexec2r(jt,(x0),(x1),(x2),(x3),(x4),(x5))
#define exec2x(x0,x1,x2,x3,x4)      jtexec2x(jt,(x0),(x1),(x2),(x3),(x4))
#define exec2z(x0,x1,x2,x3,x4)      jtexec2z(jt,(x0),(x1),(x2),(x3),(x4))
#define exg(x)                      jtexg(jt,(x))

#define expn1(x)                    jtatomic1(jt,(x),ds(CEXP))
#define expn2(x,y)                  jtatomic2(jt,(x),(y),ds(CEXP))   // bypass sqrt test for internal calls


#define exta(x0,x1,x2,x3)           jtexta(jt,(x0),(x1),(x2),(x3))
#define extnvr()                    jtextnvr(jt)
// Handle top level of fa(), which decrements use count and decides whether recursion is needed.  We recur if the contents are traversible and
// the current block is being decremented to 0 usecount or does not have recursive usecount
// fa() audits the tstack, for use outside the tpop system.  fadecr does just the decrement (for when AC is known > 1)
// Zczero is ~0 if usecount is going negative, 0 otherwise.  Usecount 1->0, 8..1->8..2, 4..0 unchanged, others decrement
#define fadecr(x) I Zc=AC(x); AC(x)=Zc=Zc-1+((UI)Zc>>(BW-2));  // this does the decrement only, checking for PERMANENT
#define faaction(x, nomfaction) {fadecr(x) I tt=AT(x); Zc=REPSGN(Zc-1); if(((tt&=TRAVERSIBLE)&(Zc|~AFLAG(x)))!=0)jtfa(jt,(x),tt); if(Zc!=0){jtmf(jt,x);} nomfaction}
#define fa(x)                       {if((x)!=0)faaction((x),)}
// Within the tpush/tpop when we know the usecount has gone to 0, no need to audit fa, since it was checked on the push
#define fana(x)                     {if((x)!=0)fanano0(x)}
// when x is known to be valid and usecount has gone to 0
#define fanano0(x)                  faaction((x),)
// Within tpop, no need to check ACISPERM; Zczero is (i. e. usecount has gone to 0) ~0; and we should recur only if flag indicates RECURSIBLE.  In that case we can reconstruct the type from the flag
#define fanapop(x,flg)              {if(((flg)&RECURSIBLE)!=0)jtfa(jt,(x),(flg)&RECURSIBLE); jtmf(jt,x);}
#define fac_ecm(x)                  jtfac_ecm(jt,(x))
#define fact(x)                     jtatomic1(jt,(x),ds(CBANG))
#define factor(x)                   jtfactor(jt,(x))
#define fauxblock(z) I z[NORMAH+4]  // define a block that can be passed in to fauxvirtual.  The 4 is the max rank, and must match fauxvirtual and fauxplain
#define fauxblockINT(z,n,r) I z[(AKXR(r)>>LGSZI)+(n)]   // define a block, big enough to hold n atoms at rank r, for use in fauxINT
// Allocate an INT block. z is the zvalue to hold the result; v is the fauxblock to use if n INTs will fit in the fauxblock, which has rank r
// shape is not filled in, except when rank is 1; therefore shape will be 0 in any call to GATV
// scaf We should mark the blocks as NJA for good form?
#define fauxINT(z,v,n,r) {if(AKXR(r)+(n)*SZI<=(I)sizeof(v)){z=(A)(v); AK(z)=AKXR(r); AFLAG(z)=0/*AFNJA*/; AT(z)=INT; AC(z)=ACUC1; AN(z)=(n); AR(z)=(RANKT)(r); if(r==1)AS(z)[0]=(n);}else{GATV0(z,INT,(n),(r));}}
// v is a block declared by fauxblock, w is the source data, r is the rank.  offset is assumed 0.  c is the initial value for AC.  If the rank is small enough, we use the fauxblock, otherwise
// we allocate a block.  We assume that the caller will fill in AN, AS.  Block must be marked UNINCORPABLE so it will not free its backer if freed, and so it will not be in-place virtualed,
// and must be marked recursive if it is of such a type so that if we fa() the block we will not try to recur
// PRISTINE is inherited from the backer (this is not done in virtual(), perhaps it should), because we know the block will never be inplaced unless it was inplaceable at the time this fauxblock was
// created, which means it is not extant anywhere it could be assigned or extracted from.
#define fauxvirtual(z,v,w,r,c) {if((r)<=4){z=ABACK(w); AK((A)(v))=(CAV(w)-(C*)(v)); AT((A)(v))=AT(w); AR((A)(v))=(RANKT)(r); z=AFLAG(w)&AFVIRTUAL?z:(w); AFLAG((A)(v))=AFVIRTUAL|AFUNINCORPABLE|(AFLAG(z)&AFPRISTINE)|(AT(w)&TRAVERSIBLE); ABACK((A)(v))=z; z=(A)(v); AC(z)=(c);} \
                              else{RZ(z=virtual((w),0,(r))); AFLAG(z)|=AFUNINCORPABLE; if((c)!=ACUC1)AC(z)=(c);} }
#define fdef(x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11)     jtfdef(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6),(x7),(x8),(x9),(x10),(x11))
#if !USECSTACK
#define fdep(x)                     jtfdep(jt,(x))
#else
#define fdep(x)                     0
#endif
#define fdepger(x)                  jtfdepger(jt,(x))
#define fh(x)                       jtfh(jt,(x))
#define filler(x)                   jtfiller(jt,(x))
#define fillv(x,y,z)                jtfillv(jt,(x),(y),(z))
#define findnl(x)                   jtfindnl(jt, (x))
// x is locale number, result is address of symbol table, or 0 if nonexistent locale
// only for non-reuse #define findnlz(x,z)                {if((UI)(n)>=jt->numlocsize)z=0; else {z=(A)(jt->numloctbl[n]); z=((UI)((I*)z-jt->numloctbl)<jt->numlocsize)?0:z;}}

#define fitct(x,y,n)                jtfitct(jt,(x),(y),(n))



#define floor1(x)                   jtatomic1(jt,(x),ds(CFLOOR))
#define fmfill(x0,x1,x2,x3,x4,x5,x6,x7) jtfmfill(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6),(x7))
#define fminit(x0,x1,x2,x3,x4,x5,x6)   jtfminit(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6))

#define fmt1(x0,x1,x2,x3,x4,x5)     jtfmt1(jt,(x0),(x1),(x2),(x3),(x4),(x5))


#define fmtallcol(x,y,z)            jtfmtallcol(jt,(x),(y),(z))
#define fmtbfc(x)                   jtfmtbfc(jt,(x))
#define fmtcomma(x0,x1,x2,x3)       jtfmtcomma(jt,(x0),(x1),(x2),(x3))

#define fmtex(x0,x1,x2,x3,x4,x5,x6,x7)              jtfmtex(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6),(x7))

#define fmtq(x0,x1,x2,x3,x4,x5)     jtfmtq(jt,(x0),(x1),(x2),(x3),(x4),(x5))
#define fmtx(x0,x1,x2,x3,x4,x5)     jtfmtx(jt,(x0),(x1),(x2),(x3),(x4),(x5))
#define fmtxi(x0,x1,x2,x3)          jtfmtxi(jt,(x0),(x1),(x2),(x3))
#define fname(x)                    jtfname(jt,(x))
#define fnegate(x)                  jtfnegate(jt,(x))
#define fnum(x)                     jtfnum(jt,(x))
#define folk(x,y,z)                 jtfolk(jt,(x),(y),(z))



#define fplus(x,y)                  jtfplus(jt,(x),(y))
#define fr(x)                       {if((x)!=0){I Zs = AC(x); if(!ACISPERM(Zs)){if(--Zs<=0)mf(x);else AC(x)=Zs;}}}
#define fram(x0,x1,x2,x3,x4)        jtfram(jt,(x0),(x1),(x2),(x3),(x4))


#define frombs1(x,y,z)              jtfrombs1(jt,(x),(y),(z))
#define frombsn(x,y,z)              jtfrombsn(jt,(x),(y),(z))
#define frombu(x,y,z)               jtfrombu(jt,(x),(y),(z))

#define fromis1(x0,x1,x2,x3)        jtfromis1(jt,(x0),(x1),(x2),(x3))



#define fslashatg(x,y,z)            jtfslashatg(jt,(x),(y),(z))
#define fsm0(x,y,z)                 jtfsm0(jt,(x),(y),(z))
#define fsmdo(x0,x1,x2,x3,x4,x5)    jtfsmdo(jt,(x0),(x1),(x2),(x3),(x4),(x5))
#define fsmvfya(x)                  jtfsmvfya(jt,(x))
#define ftymes(x,y)                 jtftymes(jt,(x),(y))
#define fullname(x)                 jtfullname(jt,(x))
#define fx(x)                       jtfx(jt,(x),0L)



#define ga(x0,x1,x2,x3)             jtga(jt,(x0),(x1),(x2),(x3))
#define gafv(x0)                    jtgafv(jt,(x0))


#define gaussdet(x)                 jtgaussdet(jt,(x))
#define gausselm(x)                 jtgausselm(jt,(x))
#define gb_flip_cycle()             jtgb_flip_cycle(jt)
#define gb_init(x)                  jtgb_init(jt,(x))
#define gb_next()                   jtgb_next(jt)
#define gb_unif_rand(x)             jtgb_unif_rand(jt,(x))

#define gc3(x0,x1,x2,x3)            jtgc3(jt,(x0),(x1),(x2),(x3))
#define gcd(x,y)                    jtatomic2(jt,(x),(y),ds(CPLUSDOT))
#define gconj(x,y,z)                jtgconj(jt,(x),(y),(z))
#define ge(x,y)                     jtatomic2(jt,(x),(y),ds(CGE))
#define gerexact(x)                 jtgerexact(jt,(x))

#define getsen(x)                   jtgetsen(jt,(x))
#define gr1(x)                      jtgr1(jt,(x))
#define grade1(x)                   jtgrade1(jt,(x))


#define graft(x)                    jtgraft(jt,(x))
#define grd1sp(x)                   jtgrd1sp(jt,(x))
#define grd1spdd(x,y,z)             jtgrd1spdd(jt,(x),(y),(z))
#define grd1spds(x,y,z)             jtgrd1spds(jt,(x),(y),(z))
#define grd1spsd(x,y,z)             jtgrd1spsd(jt,(x),(y),(z))
#define grd1spss(x,y,z)             jtgrd1spss(jt,(x),(y),(z))
#define grd1spz(x,y,z)              jtgrd1spz(jt,(x),(y),(z))

#define grd2spsd(x,y,z)             jtgrd2spsd(jt,(x),(y),(z))
#define grd2spss(x,y,z)             jtgrd2spss(jt,(x),(y),(z))
#define gri(x0,x1,x2,x3,x4)         jtgri(jt,(x0),(x1),(x2),(x3),(x4))
#define gri1(x0,x1,x2,x3,x4)        jtgri1(jt,(x0),(x1),(x2),(x3),(x4))
#define gru1(x0,x1,x2,x3,x4)        jtgru1(jt,(x0),(x1),(x2),(x3),(x4))
#define grx(x0,x1,x2,x3,x4)         jtgrx(jt,(x0),(x1),(x2),(x3),(x4))
#define gt(x,y)                     jtatomic2(jt,(x),(y),ds(CGT))
#define head(x)                     jthead(jt,(x))
#define hgd(x0,x1,x2,x3,x4)         jthgd(jt,(x0),(x1),(x2),(x3),(x4))
#define hgeom2(x,y,z)               jthgeom2(jt,(x),(y),(z))
#define hgv(x0,x1,x2,x3)            jthgv(jt,(x0),(x1),(x2),(x3))

#define hiau(x)                     jthiau(jt,(x))
#define hid(x)                      jthid(jt,(x))


#define hostio(x)                   jthostio(jt,(x))
#define hparm(x,y,z)                jthparm(jt,(x),(y),(z))
#define hrep(x,y,z)                 jthrep(jt,(x),(y),(z))
#define i0(x)                       jti0(jt,(x))
#define iaddr(x0,x1,x2,x3)          jtiaddr(jt,(x0),(x1),(x2),(x3))
#define icap(x)                     jticap(jt,(x))
#define icor(x)                   jticor(jt,(x))
#define icvt(x)                     jticvt(jt,(x))
#define iden(x)                     jtiden(jt,(x))
#define idensb(x)                   jtidensb(jt,(x))
#define idenv0(x0,x1,x2,x3,x4)      jtidenv0(jt,(x0),(x1),(x2),(x3),(x4))

#define ifc2(x)                     jtifc2(jt,(x))
#define ifdz(x)                     jtifdz(jt,(x))


#define igcd(x,y)                   jtigcd(jt,(x),(y))

#define iixBX(x0,x1,x2,x3,x4)       jtiixBX(jt,(x0),(x1),(x2),(x3),(x4))
#define iixI(x0,x1,x2,x3,x4)        jtiixI(jt,(x0),(x1),(x2),(x3),(x4))
#define ilcm(x,y)                   jtilcm(jt,(x),(y))
#define immea(x)                    jtimmea(jt,(x))
#define immex(x)                    jtimmex(jt,(x))
#define incorp(x)                   jtincorp(jt,(x))
#define increm(x)                   jtincrem(jt,(x))

#define indexofprehashed(x,y,z)     jtindexofprehashed(jt,(x),(y),(z))
#define indexofss(x,y,z)            jtindexofss(jt,(x),(y),(z))
#define indexofsub(x,y,z)           jtindexofsub(jt,(x),(y),(z))
#define indexofxx(x,y,z)            jtindexofxx(jt,(x),(y),(z))
#define infix(x,y,z)                jtinfix(jt,(x),(y),(z))
#define inpl(x,y,z)                 jtinpl(jt,(x),(y),(z))
#define int0(x)                     jtint0(jt,(x))


#define intpow(x,y)                 jtintpow(jt,(x),(y))
#define inv(x)                      jtinv(jt,(x),0)
#define invrecur(x)                 jtinv(jt,(x),1)  // call inv(), indicating recursive call
#define invamp(x)                   jtinvamp(jt,(x))
#define invfork(x)                  jtinvfork(jt,(x))
#define iocol(x,y,z)                jtiocol(jt,(x),(y),(z))

#define iopart(x0,x1,x2,x3,x4,x5,x6)                jtiopart(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6))
#define ioresparse(x,y,z)           jtioresparse(jt,(x),(y),(z))

#define iovsd(x,y,z)                jtiovsd(jt,(x),(y),(z))
#define iovxs(x,y,z)                jtiovxs(jt,(x),(y),(z))
#define iota(x)                     jtiota(jt,(x))
#define ipart(x0,x1,x2,x3)          jtipart(jt,(x0),(x1),(x2),(x3))
#define ipbx(x0,x1,x2,x3)           jtipbx(jt,(x0),(x1),(x2),(x3))
#define ipprep(x0,x1,x2,x3,x4,x5)   jtipprep(jt,(x0),(x1),(x2),(x3),(x4),(x5))
#define iprimetest(x)               jtiprimetest(jt,(x))
#define irs1(x0,x1,x2,x3)           jtirs1(jt,(x0),(x1),(x2),(x3))
#define irs2(x0,x1,x2,x3,x4,x5)     jtirs2(jt,(x0),(x1),(x2),(x3),(x4),(x5))
#define isnanq(x)                   jtisnanq(jt,(x))

#define ixf(x)                      jtixf(jt,(x))
#define ixin(x0,x1,x2,x3)           jtixin(jt,(x0),(x1),(x2),(x3))
#define jclose(x)                   jtjclose(jt,(x))
#define jdot1(x)                    jtjdot1(jt,(x))

#define jerrno()                    jtjerrno(jt)
#define jfread(x)                   jtjfread(jt,(x))

#define jgets(x)                    jtjgets(jt,(x))

#define jinit3()                    jtjinit3(jt)
#define joff(x)                     jtjoff(jt,(x))

#define jpr(x)                      jtjpr(jt,(x))
#define jpr1(x)                     jtjpr1(jt,(x))
#define jprx(x0,x1,x2,x3,x4)        jtjprx(jt,(x0),(x1),(x2),(x3),(x4))


#define jsigd(x)                    jtjsigd(jt,(x))
#define jsignal(x)                  jtjsignal(jt,(x))
#define jsignal3(x,y,z)             jtjsignal3(jt,(x),(y),(z))
#define jsigstr(x,y,z)              jtjsigstr(jt,(x),(y),(z))
#define jstd(x,y,z)                   jtjstd(jt,(x),(y),(z))
#define key(x,y,z)                  jtkey(jt,(x),(y),(z))
#define keysp(x,y,z)                jtkeysp(jt,(x),(y),(z))
#define keytally(x,y,z)             jtkeytally(jt,(x),(y),(z))
#define keytallysp(x)               jtkeytallysp(jt,(x))
#define laguerre(x,y,z)             jtlaguerre(jt,(x),(y),(z))
#define lamin1(x)                   jtlamin1(jt,(x))

#define lbox(x)                     jtlbox(jtinplace,(x),ltext)
#define lchar(x)                    jtlchar(jtinplace,(x),ltext)
#define lcm(x,y)                    jtatomic2(jt,(x),(y),ds(CSTARDOT))
#define lcolon(x)                   jtlcolon(jtinplace,(x),ltext)
#define lcpx(x)                     jtlcpx(jt,(x))
#define le(x,y)                     jtatomic2(jt,(x),(y),ds(CLE))


#define lev2(x,y,z)                 jtlev2(jt,(x),(y),(z))

#define levs2(x,y,z)                jtlevs2(jt,(x),(y),(z))
#define line(x0,x1,x2,x3)           jtline(jt,(x0),(x1),(x2),(x3))
#define linf(x0,x1,x2,x3)           jtlinf(jt,(x0),(x1),(x2),(x3))
#define link(x,y)                   jtlink(jt,(x),(y),UNUSED_VALUE)
#define linsert(x,y)                jtlinsert(jtinplace,(x),(y),ltext)
#define lnoun(x)                    jtlnoun(jtinplace,(x),ltext)
#define lnoun0(x)                   jtlnoun0(jtinplace,(x),ltext)
#define lnum(x)                     jtlnum(jtinplace,(x),ltext)
#define lnum1(x)                    jtlnum1(jtinplace,(x),ltext)

#define loccrenum(x)                jtloccrenum(jt,(x))
#define locdestroy(x)               jtlocdestroy(jt,(x))
#define locindirect(x,y,z)          jtlocindirect(jt,(x),(y),(z))
#define locmap1(x)                  jtlocmap1(jt,(x))
#define locname(x)                  jtlocname(jt,(x))
#define locnlx(x)                   jtlocnlx(jt,(x))
#define logar1(x)                   jtatomic1(jt,(x),ds(CLOG))
#define lp(x)                       jtlp(jt,(x))
#define lrv(x)                      ((UI)((x)->lrr)>>RANKTX)  // left rank of V
#define lr(x)                       lrv(FAV(x))  // left rank of A
#define lr2(x,y,z)                  jtlr2(jt,(x),(y),(z))
#define lrep(x)                     jtlrep(jt,(x))
#define lrr(x)                      jtlrr(jtinplace,(x),0L,ltext)
#define lsh(x)                      jtlsh(jtinplace,(x),ltext)
#define lshape(x)                   jtlshape(jtinplace,(x),ltext)
#define lsparse(x)                  jtlsparse(jtinplace,(x),ltext)
#define lsub(x,y,z)                 jtlsub(jt,(x),(y),(z))
#define lsymb(x,y)                  jtlsymb(jtinplace,(x),(y),ltext)
#define lt(x,y)                     jtatomic2(jt,(x),(y),ds(CLT))
#define ltie(x)                     jtltie(jt,(x))
#define ma(x)                       jtma(jt,(x))
#define mag(x)                      jtatomic1(jt,(x),ds(CSTILE))
#define makename(x)                 jtmakename(jt,(x))
#define map(x)                      jtmap(jt,(x))

#define mat(x)                      jtmat(jt,(x))


#define matchsub(x0,x1,x2,x3,x4,x5,x6,x7)  jtmatchsub(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6),(x7))


#define maximum(x,y)                jtatomic2(jt,(x),(y),ds(CMAX))
// typepriority is 0, 1, 4, 9, 10, 5, 6, 7, 8, 2, 3
// prioritytype is B01X, LITX, C2TX, C4TX, INTX, BOXX, XNUMX, RATX, SBTX, FLX, CMPXX
//                 00000 00001 10001 10010 00010 00101 00110  00111 10000 00011 00100
// reversed        001 0000 0111 0000 0011 1001 1000 1010 0010 1001 0100 0100 0010 0000

#define maxtype(x,y)                (((x)==(y))?(x):jtmaxtype(jt,x,y))
#define maxtypedne(x,y) (jt->typepriority[CTTZ(x)]>jt->typepriority[CTTZ(y)]?(x):(y))
#define maxtyped(x,y)               (((x)==(y))?(x):maxtypedne(x,y))
// For sparse types, we encode here the corresponding dense type

#define meminit()                   jtmeminit(jt)
#define memoget(x,y,z)              jtmemoget(jt,(x),(y),(z))
#define memoput(x0,x1,x2,x3)        jtmemoput(jt,(x0),(x1),(x2),(x3))

#define merge2(x0,x1,x2,x3,x4)         jtmerge2(jt,(x0),(x1),(x2),(x3),(x4))
#define mf(x)                       jtmf(jt,(x))
#define minimum(x,y)                jtatomic2(jt,(x),(y),ds(CMIN))
#define minors(x)                   jtminors(jt,(x))
#define minus(x,y)                  jtatomic2(jt,(x),(y),ds(CMINUS))
#define minusA(x,y)                 jtatomic2((J)((I)jt|JTINPLACEA),(x),(y),ds(CMINUS))
#define minv(x)                     jtminv(jt,(x))
#define mmharvest(x0,x1,x2,x3,x4,x5,x6) jtmmharvest(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6))
#define mmprep(x0,x1,x2,x3,x4,x5)   jtmmprep(jt,(x0),(x1),(x2),(x3),(x4),(x5))

#define move(x,y,z)                 jtmove(jt,(x),(y),(z))
#define movandor(x0,x1,x2,x3)       jtmovandor(jt,(x0),(x1),(x2),(x3))
#define movbwneeq(x0,x1,x2,x3)      jtmovbwneeq(jt,(x0),(x1),(x2),(x3))
#define movfslash(x,y,z)            jtmovfslash(jt,(x),(y),(z))
#define movminmax(x0,x1,x2,x3)      jtmovminmax(jt,(x0),(x1),(x2),(x3))
#define movneeq(x0,x1,x2,x3)        jtmovneeq(jt,(x0),(x1),(x2),(x3))
#define movsumavg(x0,x1,x2,x3)      jtmovsumavg(jt,(x0),(x1),(x2),(x3))
#define movsumavg1(x0,x1,x2,x3)     jtmovsumavg1(jt,(x0),(x1),(x2),(x3))
#define mr(x)                       ((I)(FAV(x)->mr))
#define mr_init(x)                  jtmr_init(jt,(x))
#define mr_next()                   jtmr_next(jt)
#define mr_next31()                 jtmr_next31(jt)
#define msmerge(x,y,z)              jtmsmerge(jt,(x),(y),(z))
#define msort(x,y,z)                jtmsort(jt,(x),(y),(z))
#define msortitems(w,x,y,z)         jtmsortitems(jt,(w),(x),(y),(z))

#define mt_init(x)                  jtmt_init(jt,(x))
#define mt_next()                   jtmt_next(jt)
#define mult(x,y)                   jtmult(jt,(x),(y))

#define mvw(x0,x1,x2,x3,x4,x5,x6)   jtmvw(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6))



#define nch1(x0,x1,x2,x3)           jtnch1(jt,(x0),(x1),(x2),(x3))
#define ne(x,y)                     jtatomic2(jt,(x),(y),ds(CNE))
#define negate(x)                   jtnegate(jt,(x))
#define negateW(x)                  jtnegate((J)((I)jt|JTINPLACEW),(x))
#define neutral(x)                  jtneutral(jt,(x))
#define newt(x0,x1,x2,x3)           jtnewt(jt,(x0),(x1),(x2),(x3))
#define nextprime(x)                jtnextprime(jt,(x))
#define nfb(x)                      jtnfb(jt,(x))

#define nlsym(x)                    jtnlsym(jt,(x))
#define nlx(x)                      jtnlx(jt,(x))
#define nlxxx(x)                    jtnlxxx(jt,(x))
#define nmhash(x,y)                 hic((x),(y))
#define nodupgrade(x0,x1,x2,x3,x4,x5,x6,x7,x8)      jtnodupgrade(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6),(x7),(x8))
#define nor(x,y)                    jtatomic2(jt,(x),(y),ds(CPLUSCO))
#define norm(x)                     jtnorm(jt,(x))
#define __not(x)                    jtnot(jt,(x))
#define notonupperstack(x)          jtnotonupperstack(jt,(x))
#define nub(x)                      jtnub(jt,(x))
#define nubsieve(x)                 jtnubsieve(jt,(x))
#define nubsievesp(x)               jtnubsievesp(jt,(x))
#define numb(x0,x1,x2,x3)           jtnumb(jt,(x0),(x1),(x2),(x3))
#define numbpx(x,y,z)               jtnumbpx(jt,(x),(y),(z))

#define numd(x,y,z)                 jtnumd(jt,(x),(y),(z))
#define nume(x,y,z)                 jtnume(jt,(x),(y),(z))
#define numi(x,y,z)                 jtnumi(jt,(x),(y),(z))
#define numj(x,y,z)                 jtnumj(jt,(x),(y),(z))
#define numq(x,y,z)                 jtnumq(jt,(x),(y),(z))
#define numr(x,y,z)                 jtnumr(jt,(x),(y),(z))
#define numx(x,y,z)                 jtnumx(jt,(x),(y),(z))

#define obqfslash(x,y)              jtobqfslash(jt,(x),(y))
#define obverse(x,y)                jtobverse(jt,(x),(y))
#define odom(x,y,z)                 jtodom(jt,(x),(y),(z))
#define ofxassoc(x,y,z)             jtofxassoc(jt,(x),(y),(z))
#define omask(x,y)                  jtomask(jt,(x),(y))
#define onm(x)                      jtonm(jt,(x))
#define ope(x)                      jtope(jt,(x))
#define opes(x,y,z)                 jtopes(jt,(x),(y),(z))
#define opes1(x0,x1,x2,x3,x4,x5)    jtopes1(jt,(x0),(x1),(x2),(x3),(x4),(x5))
#define opes2(x0,x1,x2,x3,x4,x5,x6) jtopes2(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6))
#define ord(x)                      jtord(jt,(x))
#define ordstat(x,y)                jtordstat(jt,(x),(y))
#define outfix(x,y,z)               jtoutfix(jt,(x),(y),(z))
#define over(x,y)                   jtover(jt,(x),(y))
#define ovgmove(x0,x1,x2,x3,x4,x5,x6)               jtovgmove(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6))
#define ovs(x,y)                    jtovs(jt,(x),(y))
#define ovs0(x0,x1,x2,x3)           jtovs0(jt,(x0),(x1),(x2),(x3))
#define pad(x,y,z)                  jtpad(jt,(x),(y),(z))
#define parse(x)                    jtparse(jt,(x))
#define parsea(x,y)                 jtparsea(jt,(x),(y))
#define parseinit()                 jtparseinit(jt)
#define parsex(x0,x1,x2,x3)         jtparsex(jt,(x0),(x1),(x2),(x3))
#define paxis(x,y)                  jtpaxis(jt,(x),(y))
#define pcvt(x,y)                   jtpcvt(jt,(x),(y))
#define pdt(x,y)                    jtpdt(jt,(x),(y))
#define pdtby(x,y)                  jtpdtby(jt,(x),(y))
#define pdtsp(x,y)                  jtpdtsp(jt,(x),(y))
#define pdtspmv(x,y)                jtpdtspmv(jt,(x),(y))
#define pdtspmm(x,y)                jtpdtspmm(jt,(x),(y))
#define pdtspvm(x,y)                jtpdtspvm(jt,(x),(y))
#define pdtspvv(x,y)                jtpdtspvv(jt,(x),(y))
#define pee(a,b,c,d,e    )          jtpee(jt,(a),(b),(c),(d),(e))
#define pfill(x,y)                  jtpfill(jt,(x),(y))
#define piev(x,y)                   jtpiev(jt,(x),(y))
#define pind(x,y)                   jtpind(jt,(x),(y))
#define pinit()                     jtpinit(jt)
#define pinv(x)                     jtpinv(jt,(x))
#define pix(x)                      jtpix(jt,(x),ds(CCIRCLE))
#define plt(x)                      jtplt(jt,(x))
#define plus(x,y)                   jtatomic2(jt,(x),(y),ds(CPLUS))
#define plusA(x,y)                  jtatomic2((J)((I)jt|JTINPLACEA),(x),(y),ds(CPLUS))
#define plusW(x,y)                  jtatomic2((J)((I)jt|JTINPLACEW),(x),(y),ds(CPLUS))
#define pmarea2(x,y)                jtpmarea2(jt,(x),(y))
#define pmfree(x)                   jtpmfree(jt,(x))
#define pmrecord(x0,x1,x2,x3)       jtpmrecord(jt,(x0),(x1),(x2),(x3))
#define pollard_p_1(x)              jtpollard_p_1(jt,(x))
#define pollard_rho(x)              jtpollard_rho(jt,(x))
#define poly1(x)                    jtpoly1(jt,(x))
#define poly2a(x,y)                 jtpoly2a(jt,(x),(y))
#define polymult(x,y,z)             jtpolymult(jt,(x),(y),(z))
#define pospow(x,y)                 jtpospow(jt,(x),(y))
#define powop(x,y,z)                jtpowop(jt,(x),(y),(z))
#define powseqlim(x,y)              jtpowseqlim(jt,(x),(y))
#define prefix(x,y)                 jtprefix(jt,(x),(y))
#define prep(x)                     jtprep(jt,(x))
#define preparse(x,y,z)             jtpreparse(jt,(x),(y),(z))
#define prevprime(x)                jtprevprime(jt,(x))
#define prime(x)                    jtprime(jt,(x))
#define prime1(x)                   jtprime1(jt,(x))
#define prime1d(x)                  jtprime1d(jt,(x))
#define primetest(x)                jtprimetest(jt,(x))
#define primitive(x)                jtprimitive(jt,(x))
#define probe(x,y,z,w)              jtprobe(jt,(x),(y),(z),(w))
#define probedel(x,y,z,w)           jtprobedel(jt,(x),(y),(z),(w))
#define probeis(x,y)                jtprobeis(jt,(x),(y))
#define probeislocal(x)             jtprobeislocal(jt,(x))
#define probelocal(x,y)             jtprobelocal(jt,(x),(y))
#define prod(x,y)                   jtprod(jt,(x),(y))
#define pscan(x,y)                  jtpscan(jt,(x),(y))
#define pscangt(x0,x1,x2,x3,x4,x5)         jtpscangt(jt,(x0),(x1),(x2),(x3),(x4),(x5))
#define pscanlt(x0,x1,x2,x3,x4,x5)  jtpscanlt(jt,(x0),(x1),(x2),(x3),(x4),(x5))
#define qbin(x,y)                   jtqbin(jt,(x),(y))
#define qco2(x,y)                   jtqco2(jt,(x),(y))
#define qco2x(x,y)                  jtqco2x(jt,(x),(y))
#define qcompare(x,y)               jtqcompare(jt,(x),(y))
#define qdiv(x,y)                   jtqdiv(jt,(x),(y))
#define qgcd(x,y)                   jtqgcd(jt,(x),(y))
#define qlcm(x,y)                   jtqlcm(jt,(x),(y))
#define qlogd1(x)                   jtqlogd1(jt,(x))
#define qlogz1(x)                   jtqlogz1(jt,(x))
#define qminus(x,y)                 jtqminus(jt,(x),(y))
#define qplus(x,y)                  jtqplus(jt,(x),(y))
#define qpow(x,y)                   jtqpow(jt,(x),(y))
#define qq(x,y)                     jtqq(jt,(x),(y))
#define qr(x)                       jtqr(jt,(x))
#define qrem(x,y)                   jtqrem(jt,(x),(y))
#define qrr(x)                      jtqrr(jt,(x))
#define qstd(x)                     jtqstd(jt,(x))
#define qtymes(x,y)                 jtqtymes(jt,(x),(y))
// Handle top level of ra().  Increment usecount.  Set usecount recursive usecount if recursible type; recur on contents if original usecount is not recursive
// We can have an inplaceable but recursible block, if it was gc'd or created that way
// ra() DOES NOT realize a virtual block, so that it can be used in places where virtual blocks are not possible.  ras() does include rifv
#define ra(x)                       {I c=AC(x); c&=~ACINPLACE; AC(x)=c+=(c>>(BW-2))^1; I tt=AT(x); FLAGT flg=AFLAG(x); if(((tt^flg)&TRAVERSIBLE)!=0){AFLAG(x)=flg|=(tt&RECURSIBLE); jtra((x),tt);};}
// If this is a recursible type, make it recursive if it isn't already, by traversing the descendants.  This is like raising the usecount by 0.  Since we aren't liable to assign the block, we don't have to realize a
// virtual block unless it is a recursible type.  NOTE that PERMANENT and VIRTUAL blocks are always marked recursible if they are of recursible type
#define ra0(x)                      {I tt=AT(x); FLAGT flg=AFLAG(x); if(((tt^flg)&RECURSIBLE)!=0){if((flg&AFVIRTUAL)!=0){RZ((x)=realize(x)); flg=AFLAG(x);} AFLAG(x)=flg|=(tt&RECURSIBLE); jtra((x),tt);}}
// make this block recursive, used when x has just been allocated & thus is known to be nonrecursive & nonvirtual.  We may know the type t, too (otherwise use AT(x))
#define ra00(x,tt)                   {if(((tt)&RECURSIBLE)!=0){AFLAG(x)|=(tt)&RECURSIBLE; jtra((x),(tt));}}
#define ranec(x0,x1,x2,x3,x4,x5)    jtranec(jt,(x0),(x1),(x2),(x3),(x4),(x5))
#define rank1ex(x0,x1,x2,x3)        jtrank1ex(jt,(x0),(x1),(x2),(x3))
#define rank1ex0(x0,x1,x2)          jtrank1ex0(jt,(x0),(x1),(x2))
#define REX2R(lr,rr,lcr,rcr)        (((I)(lr)<<RANKTX)+(I)(rr)+((((I)(lcr)<<RANKTX)+(I)(rcr))<<2*RANKTX))
#define rank2ex(x0,x1,x2,x3,x4,x5,x6,x7)  jtrank2ex(jt,(x0),(x1),(x2),REX2R((x3),(x4),(x5),(x6)),(x7))
#define rank2ex0(x0,x1,x2,x3)       jtrank2ex0(jt,(x0),(x1),(x2),(x3))
#define rankingb(x0,x1,x2,x3,x4,x5) jtrankingb(jt,(x0),(x1),(x2),(x3),(x4),(x5))
// ras does rifv followed by ra
#define ras(x)                      ((x) = jtras(jt,x))
#define rat(x)                      jtrat(jt,(x))
#define ravel(x)                    jtravel(jt,(x))
#define raze(x)                     jtraze(jt,(x))
#define razecut2(x,y,z)             jtrazecut2(jt,(x),(y),(z))
#define razeg(x0,x1,x2,x3,x4,x5)    jtrazeg(jt,(x0),(x1),(x2),(x3),(x4),(x5))
#define razeh(x)                    jtrazeh(jt,(x))
#define rc(x,y,z,t)                 jtrc(jt,(x),(y),(z),(t))
#define rd(x,y,z)                   jtrd(jt,(x),(y),(z))
#define rdns(x)                     jtrdns(jt,(x))
#define rdot1(x)                    jtrdot1(jt,(x))
#define realize(x)                  jtrealize(jt,(x))
#define realizeifvirtual(x)         {if((AFLAG(x)&AFVIRTUAL)!=0)RZ((x)=realize(x));}
#define rifv(x)                     realizeifvirtual(x)
#define rifvs(x)                    jtrifvs(jt,(x))
// We have used rifvs liberally through the code to guarantee that all functions can deal with virtual blocks returned.
// In some cases, the call is to an internal routine that we know will not return a virtual block normally, and is in an
// important performance path.  We use rifvsdebug for these places.  rifvs is called only during debugging.  Review them from time to time.
#if FORCEVIRTUALINPUTS==2
#define rifvsdebug(x)               rifvs(x)
#else
#define rifvsdebug(x)               (x)
#endif
#define reaxis(x,y)                 jtreaxis(jt,(x),(y))
#define recip(x)                    jtrecip(jt,(x))
#define rect(x)                     jtrect(jt,(x))
#define red0(x,y)                   jtred0(jt,(x),(y))
#define redcat(x,y)                 jtredcat(jt,(x),(y))
#define redcatsp(x,y,z)             jtredcatsp(jt,(x),(y),(z))
#define redef(x,y)                  jtredef(jt,(x),(y))
#define redefg(x)                   jtredefg(jt,(x))
#define redg(x,y)                   jtredg(jtinplace,(x),(y))
#define redsp1(x0,x1,x2,x3,x4,x5,x6,x7)             jtredsp1(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6),(x7))
#define redsp1a(x0,x1,x2,x3,x4,x5)                  jtredsp1a(jt,(x0),(x1),(x2),(x3),(x4),(x5))
#define redspd(x0,x1,x2,x3,x4,x5,x6,x7)             jtredspd(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6),(x7))
#define redsps(x0,x1,x2,x3,x4,x5,x6,x7)             jtredsps(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6),(x7))
#define redspse(x0,x1,x2,x3,x4,x5,x6,x7)            jtredspse(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6),(x7))
#define redspsprep(x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12)  jtredspsprep(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6),(x7),(x8),(x9),(x10),(x11),(x12))
#define reduce(x,y)                 jtreduce(jt,(x),(y))
#define reducesp(x,y)               jtreducesp(jt,(x),(y))
#define reitem(x,y)                 jtreitem(jt,(x),(y))
#define remdd(x,y)                  jtremdd(jt,(x),(y))
#define remid(x,y)                  jtremid(jt,(x),(y))
#define rep1d(x0,x1,x2,x3)          jtrep1d(jt,(x0),(x1),(x2),(x3))
#define rep1sa(x,y,z)               jtrep1sa(jt,(x),(y),(z))
#define repeat(x,y)                 jtrepeat(jt,(x),(y))
#define repeatr(x,y)                jtrepeatr(jt,(x),(y))
#define repidx(x0,x1,x2,x3)         jtrepidx(jt,(x0),(x1),(x2),(x3))
#define repzdx(x0,x1,x2,x3)         jtrepzdx(jt,(x0),(x1),(x2),(x3))
#define reshape(x,y)                jtreshape(jt,(x),(y))
#define reshapeW(x,y)               jtreshape((J)((I)jt|JTINPLACEW),(x),(y))
#define reshapesp(x0,x1,x2,x3)      jtreshapesp(jt,(x0),(x1),(x2),(x3))
#define reshapesp0(x0,x1,x2,x3)     jtreshapesp0(jt,(x0),(x1),(x2),(x3))
#define residue(x,y)                jtresidue(jt,(x),(y),ds(CSTILE))
#define reverse(x)                  jtreverse(jt,(x))
#define revsp(x)                    jtrevsp(jt,(x))
#define rezero(x,y)                 jtrezero(jt,(x),(y))
#define rfc(x)                      jtrfc(jt,(x))
#define rfcq(x0,x1,x2,x3)           jtrfcq(jt,(x0),(x1),(x2),(x3))
#define rfcz(x,y)                   jtrfcz(jt,(x),(y))
#define rfd(x)                      jtrfd(jt,(x))
#define rngga(x,y)                  jtrngga(jt,(x),(y))
#define rnginit()                   jtrnginit(jt)
#define rngseeds(x)                 jtrngseeds(jt,(x))
#define rngselects(x)               jtrngselects(jt,(x))
#define rngstates1(x0,x1,x2,x3,x4,x5,x6)         jtrngstates1(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6))
#define roll(x)                     jtroll(jt,(x))
#define roll2(x,y)                  jtroll2(jt,(x),(y))
#define rollany(x,y)                jtrollany(jt,(x),(y))
#define rollbool(x)                 jtrollbool(jt,(x))
#define rollk(x,y,z)                jtrollk(jt,(x),(y),(z))
#define rollksub(x,y)               jtrollksub(jt,(x),(y))
#define rollnot0(x,y)               jtrollnot0(jt,(x),(y))
#define rollxnum(x)                 jtrollxnum(jt,(x))
#define root(x,y)                   jtroot(jt,(x),(y))
#define rot(x0,x1,x2,x3,x4,x5,x6,x7)                jtrot(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6),(x7))
#define rotate(x,y)                 jtrotate(jt,(x),(y))
#define rotsp(x,y)                  jtrotsp(jt,(x),(y))
#define roundID(x,y)                jtroundID(jt,(x),(y))
#define rrv(x)                      ((UI)((x)->lrr)&RANKTMSK)  // rr of V
#define rr(x)                       rrv(FAV(x))  // rr of A
#define rsh0(x)                     jtrsh0(jt,(x))
#define rsort(x)                    jtrsort(jt,(x))
#define sb2(x,y)                    jtsb2(jt,(x),(y))
#define sbbox(x)                    jtsbbox(jt,(x))
#define sbcheck(x)                  jtsbcheck(jt,(x))
#define sbcheck1(x0,x1,x2,x3,x4,x5,x6,x7)           jtsbcheck1(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6),(x7))
#define sbextend(x0,x1,x2,x3)       jtsbextend(jt,(x0),(x1),(x2),(x3))
#define sbgetdata(x)                jtsbgetdata(jt,(x))
#define sbhashstat(x)               jtsbhashstat(jt,(x))
#define sbinsert(x0,x1,x2,x3,x4,x5) jtsbinsert(jt,(x0),(x1),(x2),(x3),(x4),(x5))
#define sblit(x,y)                  jtsblit(jt,(x),(y))
#define sborder(x)                  jtsborder(jt,(x))
#define sbprobe(x0,x1,x2,x3)        jtsbprobe(jt,(x0),(x1),(x2),(x3))
#define sbsetdata(x)                jtsbsetdata(jt,(x))
#define sbstr(x,y)                  jtsbstr(jt,(x),(y))
#define sbtypeinit()                jtsbtypeinit(jt)
#define sbunbox(x)                  jtsbunbox(jt,(x))
#define sbunind(x)                  jtsbunind(jt,(x))
#define sbunlit(x,y)                jtsbunlit(jt,(x),(y))
#define sbunstr(x,y)                jtsbunstr(jt,(x),(y))
#define sc(x)                       jtsc(jt,(x))
#define sc4(x,y)                    jtsc4(jt,(x),(y))
#define scansp(x,y,z)               jtscansp(jt,(x),(y),(z))
#define scb(x)                      jtscb(jt,(x))
#define scc(x)                      jtscc(jt,(x))
#define scf(x)                      jtscf(jt,(x))
#define scfn(x,y)                   jtscfn(jt,(x),(y))
#define scheck(x)                   jtscheck(jt,(x))
#define scib(x)                     jtscib(jt,(x))
#define scind(x)                    jtscind(jt,(x))
#define scuba(x,y,z)                jtscuba(jt,(x),(y),(z))
#define scubb(x,y)                  jtscubb(jt,(x),(y))
#define scubc(x,y,z)                jtscubc(jt,(x),(y),(z))
#define scube(x,y,z)                jtscube(jt,(x),(y),(z))
#define scx(x)                      jtscx(jt,(x))
#define seecall(x)                  jtseecall(jt,(x))
#define seeparse(x)                 jtseeparse(jt,(x))
#define seg(x,y)                    jtseg(jt,(x),(y))
#define selfq(x)                    jtselfq(jt,(x))
#define hasimploc(x)                jthasimploc(jt,(x))
#define selm(x)                     jtselm(jt,(x))
#define selx(x,y,z)                 jtselx(jt,(x),(y),(z))
#define sely(x0,x1,x2,x3)           jtsely(jt,(x0),(x1),(x2),(x3))
#define sent12b(x)              jtsent12b(jt,(x))
#define sent12c(x)            jtsent12c(jt,(x))
#define sesminit()                  jtsesminit(jt)
#define setfv(x,y)                  jtsetfv(jt,(x),(y))
#define sfn(x,y)                    jtsfn(jt,(x),(y))
#define sfne(x)                     jtsfne(jt,(x))
#define shape(x)                    jtshape(jt,(x))
#define shift1(x)                   jtshift1(jt,(x))
#define shift10(x,y)                jtshift10(jt,(x),(y))
#define showerr()                   jtshowerr(jt)
#define signum(x)                   jtatomic1(jt,(x),ds(CSTAR))
#define slash(x)                    jtslash(jt,(x))
#define sldot(x)                    jtsldot(jt,(x))
#define sm_init(x)                  jtsm_init(jt,(x))
#define smallprimes(x0,x1,x2,x3)    jtsmallprimes(jt,(x0),(x1),(x2),(x3))
#define snl(x)                      jtsnl(jt,(x))
#define sortb2(x0,x1,x2)            jtsortb2(jt,(x0),(x1),(x2))
#define sortb4(x0,x1,x2)            jtsortb4(jt,(x0),(x1),(x2))
#define sortc2(x0,x1,x2)            jtsortc2(jt,(x0),(x1),(x2))
#define sortu1(x0,x1,x2)            jtsortu1(jt,(x0),(x1),(x2))
#define sorti1(x0,x1,x2)            jtsorti1(jt,(x0),(x1),(x2))
#define sp(x)                       jtsp(jt,(x))
#define sparse1(x)                  jtsparse1(jt,(x))
#define sparse1a(x0,x1,x2,x3,x4)    jtsparse1a(jt,(x0),(x1),(x2),(x3),(x4))
#define sparseit(x,y,z)             jtsparseit(jt,(x),(y),(z))
#define sparsen1(x)                 jtsparsen1(jt,(x))
#define sparsep1(x)                 jtsparsep1(jt,(x))
#define spbytesinuse()              jtspbytesinuse(jt)
#define spstarttracking()           jtspstarttracking(jt)
#define spendtracking()             jtspendtracking(jt)
#define spc()                       jtspc(jt)
#define spdscell(x0,x1,x2,x3,x4)    jtspdscell(jt,(x0),(x1),(x2),(x3),(x4))
#define spella(x)                   jtspella(jt,(x))
#define spellcon(x)                 jtspellcon(jt,(x))
#define spellout(x)                 jtspellout(jt,(x))
#define spfor1(x)                   jtspfor1(jt,(x))
#define spfree()                    jtspfree(jt)
#define spmult(x0,x1,x2,x3,x4,x5,x6,x7)             jtspmult(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6),(x7))
#define spradv(x0,x1,x2,x3,x4,x5,x6)                jtspradv(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6))
#define sprank1(x0,x1,x2,x3)        jtsprank1(jt,(x0),(x1),(x2),(x3))
#define sprank2(x0,x1,x2,x3,x4,x5)  jtsprank2(jt,(x0),(x1),(x2),(x3),(x4),(x5))
#define sprank2_0w(x0,x1,x2,x3,x4,x5)               jtsprank2_0w(jt,(x0),(x1),(x2),(x3),(x4),(x5))
#define sprank2_a0(x0,x1,x2,x3,x4,x5)               jtsprank2_a0(jt,(x0),(x1),(x2),(x3),(x4),(x5))
#define sprarg(x,y)                 jtsprarg(jt,(x),(y))
#define spredge(x,y,z)              jtspredge(jt,(x),(y),(z))
#define sprinit(x0,x1,x2,x3,x4)     jtsprinit(jt,(x0),(x1),(x2),(x3),(x4))
#define sprintfeD(x0,x1,x2,x3,x4)   jtsprintfeD(jt,(x0),(x1),(x2),(x3),(x4))
#define sprintfnD(x0,x1,x2,x3,x4)   jtsprintfnD(jt,(x0),(x1),(x2),(x3),(x4))
#define sprintfI(x0,x1,x2,x3,x4)    jtsprintfI(jt,(x0),(x1),(x2),(x3),(x4))
#define sprz(x0,x1,x2,x3,x4)        jtsprz(jt,(x0),(x1),(x2),(x3),(x4))
#define spspd(x0,x1,x2,x3)          jtspspd(jt,(x0),(x1),(x2),(x3))
#define spspx(x0,x1,x2,x3)          jtspspx(jt,(x0),(x1),(x2),(x3))
#define spsscell(x0,x1,x2,x3,x4)    jtspsscell(jt,(x0),(x1),(x2),(x3),(x4))
#define sqroot(x)                   jtatomic1(jt,(x),ds(CROOT))
#define square(x)                   jtsquare(jt,(x))
#define ssdo(x,y,z)                 jtssdo(jt,(x),(y),(z))
#define ssel(x,y)                   jtssel(jt,(x),(y))
#define ssnext(x,y)                 jtssnext(jt,(x),(y))
#define stcreate(x0,x1,x2,x3)       jtstcreate(jt,(x0),(x1),(x2),(x3))
#define stdf(x)                     jtstdf(jt,(x))
#define stdnm(x)                    jtstdnm(jt,(x))
#define stfind(x,y,z)               jtstfind(jt,(x),(y),(z))
#define stfindcre(x,y,z)            jtstfindcre(jt,(x),(y),(z))
#define stitch(x,y)                 jtstitch(jt,(x),(y))
#define stitchsp2(x,y)              jtstitchsp2(jt,(x),(y))
#define str(x,y)                    jtstr(jt,(x),(y))
#define strq(x,y)                   jtstrq(jt,(x),(y))
#define str0(x)                     jtstr0(jt,(x))
#define suffix(x,y)                 jtsuffix(jt,(x),(y))
#define sumatgbool(x,y,z)           jtsumatgbool(jt,(x),(y),(z))
#define sup(x,y)                    jtsup(jt,(x),(y))
#define suq(x,y)                    jtsuq(jt,(x),(y))
#define susp()                      jtsusp(jt)
#define swap(x)                     jtswap(jt,(x))
#define swapc(x)                    jtswapc(jt,(x))
#define symbinit()                  jtsymbinit(jt)
#define symbis(x,y,z)               jtsymbis(jt,(x),(y),(z))
#define symbisdel(x,y,z)            jtsymbisdel(jt,(x),(y),(z))
#define symbrd(x)                   jtsymbrd(jt,(x))
#define symbrdlock(x)               jtsymbrdlock(jt,(x))
#define symbrdlocknovalerr(x)       jtsymbrdlocknovalerr(jt,(x))
#define symext(x)                   jtsymext(jt,(x))
#define symfreeha(x)                jtsymfreeha(jt,(x))
#define symnew(x,y)                 jtsymnew(jt,(x),(y))
#define sympoola(x)                 jtsympoola(jt,(x))
#define sybaseloc(x)                jtsybaseloc(jt,(x))
#define syrd(x,y)                   jtsyrd(jt,(x),(y))
#define syrd1(x,y,z,w)              jtsyrd1(jt,(x),(y),(z),(w))
#define syrdforlocale(x)            jtsyrdforlocale(jt,(x))
#define syrd1forlocale(x,y,z,w)     jtsyrd1forlocale(jt,(x),(y),(z),(w))
#define syrdnobuckets(x)            jtsyrdnobuckets(jt,(x))
#define table(x)                    jttable(jt,(x))
#define tail(x)                     jttail(jt,(x))
#define take(x,y)                   jttake(jt,(x),(y))
#define taker(x,y)                  jttaker(jt,(x),(y))
#define tceil(x)                    jttceil(jt,(x))
#define tclosure(x,y)               jttclosure(jt,(x),(y))
#define tconnect(x)                 jttconnect(jt,(x))
#define teq(x,y)                    jtteq(jt,(x),(y))
#define tesa(x,y)                   jttesa(jt,(x),(y))
#define tesos(x,y,z,w)              jttesos(jt,(x),(y),(z),(w))
#define tess2(x,y,z)                jttess2(jt,(x),(y),(z))
#define tf()                        jttf(jt)
#define tfloor(x)                   jttfloor(jt,(x))
#define tg(x)                       jttg(jt,(x))
#define th2a(x0,x1,x2,x3,x4,x5,x6,x7,x8)            jtth2a(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6),(x7),(x8))
#define th2box(x,y)                 jtth2box(jt,(x),(y))
#define th2c(x0,x1,x2,x3,x4,x5,x6,x7,x8,x9)         jtth2c(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6),(x7),(x8),(x9))
#define th2ctrl(x0,x1,x2,x3,x4,x5)  jtth2ctrl(jt,(x0),(x1),(x2),(x3),(x4),(x5))
#define thb(x)                      jtthb(jt,(x))
#define thbox(x,y)                  jtthbox(jt,(x),(y))
#define thdx1(x)                    jtthdx1(jt,(x))
#define thn(x)                      jtthn(jt,(x))
#define thorn1(x)                   jtthorn1(jt,(x))
#define thorn1main(x,y)             jtthorn1main(jt,(x),(y))
#define thorn1u(x)                  jtthorn1u(jt,(x))
#define thq1(x)                     jtthq1(jt,(x))
#define ths(x)                      jtths(jt,(x))
#define thsb(x,y)                   jtthsb(jt,(x),(y))
#define thv(x,y,z)                  jtthv(jt,(x),(y),(z))
#define thx1(x)                     jtthx1(jt,(x))
#define thxqe(x)                    jtthxqe(jt,(x))
#define tine(x)                     jttine(jt,(x),tmonad)
#define tk(x,y)                     jttk(jt,(x),(y))
#define tk0(x,y,z)                  jttk0(jt,(x),(y),(z))
#define tks(x,y)                    jttks(jt,(x),(y))
#define tleaf(x)                    jttleaf(jt,(x))
#define tlt(x,y)                    jttlt(jt,(x),(y))
#define toc1(x,y)                   jttoc1(jt,(x),(y))
#define toc2(x)                     jttoc2(jt,(x))
#define toc2e(x)                    jttoc2e(jt,(x))
#define tokens(x,y)                 jttokens(jt,(x),(y))
#define totient(x)                  jttotient(jt,(x))
#define toutf8(x)                   jttoutf8(jt,(x))
#define toutf8a(x,y)                jttoutf8a(jt,(x),(y))
#define toutf8x(x)                  jttoutf8x(jt,(x))
#define toutf16(x)                  jttoutf16(jt,(x))
#define toutf32(x)                  jttoutf32(jt,(x))
#define tou32(x)                    jttou32(jt,(x))
#define tpop(x)                     jttpop(jt,(x))
// if tg() fails, tpush leaves nextpushx unchanged
// Handle top level of tpush().  push the current block, and recur if it is traversible and does not have recursive usecount
// We can have an inplaceable but recursible block, if it was gc'd.  We never push a PERMANENT block, so that we won't try to free it
// NOTE that PERMANENT blocks are always marked traversible if they are of traversible type, so we will not recur on them internally
#define tpushcommon(x,suffix) {if(!ACISPERM(AC(x))){I tt=AT(x); A *pushp=jt->tnextpushp; *pushp++=(x); \
                              if(!((I)pushp&(NTSTACKBLOCK-1))){RZ(pushp=tg(pushp));} if(((tt^AFLAG(x))&TRAVERSIBLE)!=0)RZ(pushp=jttpush(jt,(x),tt,pushp)); jt->tnextpushp=pushp; suffix}}
#define tpush(x)              tpushcommon(x,)
// Internal version, used when the local name pushp is known to hold jt->tnextpushp
#define tpushi(x)                   {if(!ACISPERM(AC(x))){I tt=AT(x); *pushp++=(x); if(!((I)pushp&(NTSTACKBLOCK-1))){RZ(pushp=tg(pushp));} if(((tt^AFLAG(x))&TRAVERSIBLE!=0))RZ(pushp=jttpush(jt,(x),tt,pushp)); }}
// tpush1 is like tpush, but it does not recur to lower levels.  Used only for virtual block (which cannot be PERMANENT)
#define tpush1(x)                   {A *pushp=jt->tnextpushp; *pushp++=(x); if(!((I)pushp&(NTSTACKBLOCK-1))){RZ(pushp=tg(pushp));} jt->tnextpushp=pushp; }
#define traverse(x,y)               jttraverse(jt,(x),(y))
#define trc(x)                      jttrc(jt,(x))
#define treach(x)                   jttreach(jt,(x))
#define trep(x)                     jttrep(jt,(x))
#define tridiag(x,y,z)              jttridiag(jt,(x),(y),(z))
#define troot(x,y)                  jttroot(jt,(x),(y))
#define trr(x)                      jttrr(jt,(x))
#define tryinit(x,y,z)              jttryinit(jt,(x),(y),(z))
#define ts(x)                       jtts(jt,(x))
#define tsit2(x,y)                  jttsit2(jt,(x),(y))
#define tymes(x,y)                  jtatomic2(jt,(x),(y),ds(CSTAR))
#define tymesA(x,y)                 jtatomic2((J)((I)jt|JTINPLACEA),(x),(y),ds(CSTAR))
#define tymesW(x,y)                 jtatomic2((J)((I)jt|JTINPLACEW),(x),(y),ds(CSTAR))
#define uco1(x)                     jtuco1(jt,(x))
#define uco2(x,y)                   jtuco2(jt,(x),(y))
#define unbinr(x0,x1,x2,x3,x4)      jtunbinr(jt,(x0),(x1),(x2),(x3),(x4))
#define under(x,y)                  jtunder(jt,(x),(y))
#define unh(x)                      jtunh(jt,(x))
#define unhex(x)                    jtunhex(jt,(x))
#define unlj(x)                     jtunlj(jt,(x))
#define unlock2(x,y)                jtunlock2(jt,(x),(y))
#define unname(x)                   jtunname(jt,(x))
#define unparse(x)                  jtunparse(jt,(x))
#define unparse1(x0,x1,x2,x3)       jtunparse1(jt,(x0),(x1),(x2),(x3))
#define unparse1a(x,y,z)            jtunparse1a(jt,(x),(y),(z))
#define unparsem(x,y)               jtunparsem(jt,(x),(y))
#define unquote(x,y,z)              jtunquote(jt,(x),(y),(z))
#define unstackcv(x)                jtunstackcv(jt,(x))
#define unwordil(x,y,z)             jtunwordil(jt,(x),(y),(z))
#define unzero(x)                   jtunzero(jt,(x))
#define upon2(x,y,z)                jtupon2(jt,(x),(y),(z))
#define usebs(x,y,z)                jtusebs(jt,(x),(y),(z))
#define utype(x,y)                  jtutype(jt,(x),(y))
#define v2(x,y)                     jtv2(jt,(x),(y))
#define va1(x,y)                    jtva1(jt,(x),(y))
#define va1s(x0,x1,x2,x3)           jtva1s(jt,(x0),(x1),(x2),(x3))
#define var(x0,x1,x2)               jtvar(jt,(x0),(x1),(x2))
// fetch adocv for an rps function (i. e. f/ f/\ f/\.) in self.  rps is 0-2 for / /\ /\.   t is the type of the input.  Assign result to z, which is a VARPS
#define varps(z,self,t,rps) {VARPSA *rpsa=FAV(self)->localuse.lvp[1]; I tmax=rpsa->nprec; if(jt->jerr<EWOV){I zc=CTTZI(t); VARPS *az=&rpsa->actrtns[tmax*(rps)+zc]; az=zc>=tmax?&rpsnull.actrtns[0]:az; z=*az;}else{RESETERR; z=rpsa->actrtns[3*tmax+(rps)];}  }
#define vasp(x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12)     jtvasp(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6),(x7),(x8),(x9),(x10),(x11),(x12))
#define vasp0(x0,x1,x2,x3,x4,x5)                            jtvasp0(jt,(x0),(x1),(x2),(x3),(x4),(x5))
#define vaspc(x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12)    jtvaspc(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6),(x7),(x8),(x9),(x10),(x11),(x12))
#define vaspeq(x0,x1,x2,x3,x4,x5,x6,x7,x8)                  jtvaspeq(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6),(x7),(x8))
#define vaspeqprep(x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11)   jtvaspeqprep(jt,(x0),(x1),(x2),(x3),(x4),(x5),(x6),(x7),(x8),(x9),(x10),(x11))
#define vaspz(x)                    jtvaspz(jt,(x))
#define vaxis(x,y)                  jtvaxis(jt,(x),(y))
#define vc1(x,y)                    jtvc1(jt,(x),(y))
#define vci(x)                      jtvci(jt,(x))
#define vec(x,y,z)                  jtvec(jt,(x),(y),(z))
#define vecb01(x,y,z)               jtvecb01(jt,(x),(y),(z))
#define vfn(x)                      jtvfn(jt,(x))
#define vi(x)                       jtvi(jt,(x))
#define vib(x)                      jtvib(jt,(x))
#define vip(x)                      jtvip(jt,(x))
#define virtual(x,y,z)              jtvirtual(jt,(x),(y),(z))
#define virtualip(x,y,z)            jtvirtual(jtinplace,(x),(y),(z))
#define vlocnl(x,y)                 jtvlocnl(jt,(x),(y))
#define vnm(x,y)                    jtvnm(jt,(x),(y))
#define vs(x)                       jtvs(jt,(x))
#define vslit(x)                    jtvslit(jt,(x))
#define vtokens(x)                  jtvtokens(jt,(x))
#define vtrans(x)                   jtvtrans(jt,(x))
#define wa(x,y,z)                   jtwa(jt,(x),(y),(z))
#define weight(x,y)                 jtweight(jt,(x),(y))
#define widthdp(x,y,z)              jtwidthdp(jt,(x),(y),(z))
#define wordil(x)                   jtwordil(jt,(x))
#define words(x)                    jtwords(jt,(x),ds(CWORDS))
#define x10(x)                      jtx10(jt,(x))
#define xbin(x,y)                   jtxbin(jt,(x),(y))
#define xbinp(x,y)                  jtxbinp(jt,(x),(y))
#define xc(x)                       jtxc(jt,(x))
#define xco1(x)                     jtxco1(jt,(x))
#define xcompare(x,y)               jtxcompare(jt,(x),(y))
#define xdefn(x,y,z)                jtxdefn(jt,(x),(y),(z))
#define xd1(x,y)                      jtxd1(jt,(x),(y))
#define xdiv(x,y,z)                 jtxdiv(jt,(x),(y),(z))
#define xdivrem(x0,x1,x2,x3)        jtxdivrem(jt,(x0),(x1),(x2),(x3))
#define xev1(x,y)                   jtxev1(jt,(x),(y))
#define xev2(x,y,z)                 jtxev2(jt,(x),(y),(z))
#define xexp(x,y)                   jtxexp(jt,(x),(y))
#define xfact(x)                    jtxfact(jt,(x))
#define xfactor(x)                  jtxfactor(jt,(x))
#define xgcd(x,y)                   jtxgcd(jt,(x),(y))
#define xint(x)                     jtxint(jt,(x))
#define xlcm(x,y)                   jtxlcm(jt,(x),(y))
#define xlog1(x)                    jtxlog1(jt,(x))
#define xlog2(x,y)                  jtxlog2(jt,(x),(y))
#define xlog2sub(x,y)               jtxlog2sub(jt,(x),(y))
#define xlogabs(x)                  jtxlogabs(jt,(x))
#define xlogd1(x)                   jtxlogd1(jt,(x))
#define xlogz1(x)                   jtxlogz1(jt,(x))
#define xminus(x,y)                 jtxminus(jt,(x),(y))
#define xmodpow(x,y,z)              jtxmodpow(jt,(x),(y),(z))
#define xoinit()                    jtxoinit(jt)
#define xop(x)                      jtxop(jt,(x))
#define xop2(x,y,z)                 jtxop2(jt,(x),(y),(z))
#define xopcall(x)                  jtxopcall(jt,(x))
#define xpi(x)                      jtxpi(jt,(x))
#define xplus(x,y)                  jtxplus(jt,(x),(y))
#define xpow(x,y)                   jtxpow(jt,(x),(y))
#define xprimeq(x,y)                jtxprimeq(jt,(x),(y))
#define xprimetest(x)               jtxprimetest(jt,(x))
#define xrand(x)                    jtxrand(jt,(x))
#define xrem(x,y)                   jtxrem(jt,(x),(y))
#define xrep(x,y)                   jtxrep(jt,(x),(y))
#define xroot(x,y)                  jtxroot(jt,(x),(y))
#define xsgn(x)                     jtxsgn(jt,(x))
#define xsinit()                    jtxsinit(jt)
#define xsq(x)                      jtxsq(jt,(x))
#define xsqrt(x)                    jtxsqrt(jt,(x))
#define xstd(x)                     jtxstd(jt,(x))
#define xtymes(x,y)                 jtxtymes(jt,(x),(y))
#define zacos(x)                    jtzacos(jt,(x))
#define zacosh(x)                   jtzacosh(jt,(x))
#define zarc(x)                     jtzarc(jt,(x))
#define zasin(x)                    jtzasin(jt,(x))
#define zasinh(x)                   jtzasinh(jt,(x))
#define zatan(x)                    jtzatan(jt,(x))
#define zatanh(x)                   jtzatanh(jt,(x))
#define zbin(x,y)                   jtzbin(jt,(x),(y))
#define zceil(x)                    jtzceil(jt,(x))
#define zcir(x,y)                   jtzcir(jt,(x),(y))
#define zconjug(x)                  jtzconjug(jt,(x))
#define zcos(x)                     jtzcos(jt,(x))
#define zcosh(x)                    jtzcosh(jt,(x))
#define zdiv(x,y)                   jtzdiv(jt,(x),(y))
#define zeq(x,y)                    jtzeq(jt,(x),(y))
#define zexp(x)                     jtzexp(jt,(x))
#define zfloor(x)                   jtzfloor(jt,(x))
#define zgamma(x)                   jtzgamma(jt,(x))
#define zgauss(x,y)                 jtzgauss(jt,(x),(y))
#define zgcd(x,y)                   jtzgcd(jt,(x),(y))
#define zgps(x)                     jtzgps(jt,(x))
#define zgrecur(x)                  jtzgrecur(jt,(x))
#define zhorner(x,y,z)              jtzhorner(jt,(x),(y),(z))
#define zjx(x)                      jtzjx(jt,(x))
#define zlcm(x,y)                   jtzlcm(jt,(x),(y))
#define zlog(x)                     jtzlog(jt,(x))
#define zm4(x)                      jtzm4(jt,(x))
#define zminus(x,y)                 jtzminus(jt,(x),(y))
#define zmj(x)                      jtzmj(jt,(x))
#define znegate(x)                  jtznegate(jt,(x))
#define zp4(x)                      jtzp4(jt,(x))
#define zp8(x)                      jtzp8(jt,(x))
#define zpad1(x,y,z)                jtzpad1(jt,(x),(y),(z))
#define zpadn(x,y,z)                jtzpadn(jt,(x),(y),(z))
#define zplus(x,y)                  jtzplus(jt,(x),(y))
#define zpow(x,y)                   jtzpow(jt,(x),(y))
#define zrem(x,y)                   jtzrem(jt,(x),(y))
#define zsin(x)                     jtzsin(jt,(x))
#define zsinh(x)                    jtzsinh(jt,(x))
#define zsqrt(x)                    jtzsqrt(jt,(x))
#define zstirling(x)                jtzstirling(jt,(x))
#define ztan(x)                     jtztan(jt,(x))
#define ztanh(x)                    jtztanh(jt,(x))
#define ztrend(x)                   jtztrend(jt,(x))
#define ztridiag(x,y,z)             jtztridiag(jt,(x),(y),(z))
#define ztymes(x,y)                 jtztymes(jt,(x),(y))
