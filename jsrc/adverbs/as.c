/* Copyright 1990-2010, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Adverbs: Suffix and Outfix                                              */

#include "j.h"
#include "verbs/vasm.h"
#include "verbs/ve.h"
#define ZZDEFN
#include "result.h"


#define SUFFIXPFX(f,Tz,Tx,pfx,vecfn,retstmt)  \
 I f(I d,I n,I m,Tx* RESTRICTI x,Tz* RESTRICTI z,J jt){I i;Tz v;                                     \
  x+=m*d*n; z+=m*d*n;                                              \
  if(d==1)DQ(m, *--z=v=    *--x; DQ(n-1, --x; --z; *z=v=pfx(*x,v);))  \
  else{for(i=0;i<m;++i){                                              \
   DQ(d, *--z=    *--x;);                                        \
   DQ(n-1, Tz *y=z; z-=d; x-=d; vecfn(1,d,x,y,z,jt););                     \
 }}retstmt}

#define SUFFIXNAN(f,Tz,Tx,pfx,vecfn)  \
 I f(I d,I n,I m,Tx* RESTRICTI x,Tz* RESTRICTI z,J jt){I i;Tz v;                                      \
  NAN0;                                                               \
  x+=m*d*n; z+=m*d*n;                                              \
  if(d==1)DQ(m, *--z=v=    *--x; DQ(n-1, --x; --z; *z=v=pfx(*x,v);))  \
  else{for(i=0;i<m;++i){                                              \
   DQ(d, *--z=    *--x;);                                        \
   DQ(n-1, Tz *y=z; z-=d; x-=d; vecfn(1,d,x,y,z,jt););                     \
  }}                                                                   \
  return NANTEST?EVNAN:EVOK;                                                              \
 }

#define SUFFICPFX(f,Tz,Tx,pfx)  \
 I f(I d,I n,I m,Tx* RESTRICTI x,Tz* RESTRICTI z,J jt){I i;Tz v,*y;                                        \
  x+=m*d*n; z+=m*d*n;                                              \
  if(d==1)DQ(m, *--z=v=(Tz)*--x; DQ(n-1, --x; --z; *z=v=pfx(*x,v);))  \
  else{for(i=0;i<m;++i){                                              \
   y=z; DQ(d, *--z=(Tz)*--x;);                                        \
   DQ(n-1, DQ(d, --x; --y; --z; *z=pfx(*x,*y);));                     \
 }}return EVOK;}

#define SUFFIXOVF(f,Tz,Tx,fs1,fvv)  \
 I f(I d,I n,I m,I* RESTRICTI x,I* RESTRICTI z,J jt){C er=0;I i,*xx,*y,*zz;                      \
  xx=x+=m*d*n; zz=z+=m*d*n;                              \
  if(d==1){                                                 \
   if(1==n)DQ(m, *--z=*--x;)                                \
   else    DQ(m, z=zz-=d*n; x=xx-=d*n; fs1(n,z,x);)        \
  }else{for(i=0;i<m;++i){                                   \
   DQ(d, *--zz=*--xx;);                                     \
   DQ(n-1, x=xx-=d; y=zz; z=zz-=d; fvv(d,z,x,y););     \
 }}return EVOK;}

#define SUFFIXBFXLOOP(T,pfx)  \
 {T* RESTRICT xx=(T*)x,* RESTRICT yy,* RESTRICT zz=(T*)z;   \
  q=d/sizeof(T);              \
  DQ(m, yy=zz; DQ(q, *--zz=*--xx;); DQ(n-1, DQ(q, --xx; --yy; --zz; *zz=pfx(*xx,*yy);)));  \
 }
  
#define SUFFIXBFX(f,pfx,ipfx,spfx,bpfx,vexp)  \
 I f(I d,I n,I m,B* RESTRICTI x,B* RESTRICTI z,J jt){B v,* RESTRICT y;I q;                                        \
  x+=m*d*n; z+=m*d*n;                                           \
  if(1==d){DQ(m, *--z=v=*--x; DQ(n-1, --x; --z; *z=v=vexp;)); return EVOK;}  \
  if(0==(d&(sizeof(UI  )-1))){SUFFIXBFXLOOP(UI,   pfx); return EVOK;}              \
  if(0==(d&(sizeof(UI4)-1))){SUFFIXBFXLOOP(UINT,ipfx); return EVOK;}              \
  if(0==(d&(sizeof(US  )-1))){SUFFIXBFXLOOP(US,  spfx); return EVOK;}              \
  DQ(m, y=z; DQ(d, *--z=*--x;); DQ(n-1, DQ(d, --x; --y; --z; *z=bpfx(*x,*y);)));return EVOK;  \
 }

SUFFIXBFX(   orsfxB, OR,  IOR,  SOR,  BOR,  *x||v   ) 
SUFFIXBFX(  andsfxB, AND, IAND, SAND, BAND, *x&&v   )
SUFFIXBFX(   eqsfxB, EQ,  IEQ,  SEQ,  BEQ,  *x==v   )
SUFFIXBFX(   nesfxB, NE,  INE,  SNE,  BNE,  *x!=v   )
SUFFIXBFX(   ltsfxB, LT,  ILT,  SLT,  BLT,  *x< v   )
SUFFIXBFX(   lesfxB, LE,  ILE,  SLE,  BLE,  *x<=v   )
SUFFIXBFX(   gtsfxB, GT,  IGT,  SGT,  BGT,  *x> v   )
SUFFIXBFX(   gesfxB, GE,  IGE,  SGE,  BGE,  *x>=v   )
SUFFIXBFX(  norsfxB, NOR, INOR, SNOR, BNOR, !(*x||v))
SUFFIXBFX( nandsfxB, NAND,INAND,SNAND,BNAND,!(*x&&v))

SUFFIXOVF( plussfxI, I, I,  PLUSS, PLUSVV)
SUFFIXOVF(minussfxI, I, I, MINUSS,MINUSVV)
SUFFIXOVF(tymessfxI, I, I, TYMESS,TYMESVV)

SUFFICPFX( plussfxO, D, I, PLUS  )
SUFFICPFX(minussfxO, D, I, MINUS )
SUFFICPFX(tymessfxO, D, I, TYMES )

SUFFIXPFX( plussfxB, I, B, PLUS, plusBI, return EVOK;  )
 I plussfxD(I d,I n,I m,D* RESTRICTI x,D* RESTRICTI z,J jt){I i;
 NAN0;
 x+=m*d*n; z+=m*d*n;
 if(d==1){
  I n3=n/3; I rem=n-n3*3;  // number of triplets, number of extras
  DQ(m, D t0; D t1; D t2; D t12; D t01; if(rem<1){t0=0.0; t12=t1=0.0;}else {*--z=t0=*--x; if(rem==1){t12=t1=0.0;}else{t12=t1=*--x; *--z=t0+t1;}} t2=0.0;
    DQ(n3, t0+=*--x; *--z =t0+t12; t1+=*--x; t01=t0+t1; *--z =t01+t2; t2+=*--x; *--z =t2+t01; t12=t1+t2;)
  )
 }else{
  for(i=0;i<m;++i){
   DQ(d, *--z=    *--x;);
   DQ(n-1, D *y=z; z-=d; x-=d; plusDD(1,d,x,y,z,jt););
  }
 }
 return NANTEST?EVNAN:EVOK;
}
SUFFIXNAN( plussfxZ, Z, Z, zplus, plusZZ )
SUFFIXPFX( plussfxX, X, X, xplus, plusXX, HDR1JERR; )
SUFFIXPFX( plussfxQ, Q, Q, qplus, plusQQ, HDR1JERR; )

SUFFIXPFX(minussfxB, I, B, MINUS, minusBI, return EVOK; )
SUFFIXNAN(minussfxD, D, D, MINUS, minusDD )
SUFFIXNAN(minussfxZ, Z, Z, zminus, minusZZ)

SUFFIXPFX(tymessfxD, D, D, TYMES, tymesDD , return EVOK;)
SUFFIXPFX(tymessfxZ, Z, Z, ztymes, tymesZZ, return EVOK;)
SUFFIXPFX(tymessfxX, X, X, xtymes, tymesXX, return EVOK;)
SUFFIXPFX(tymessfxQ, Q, Q, qtymes, tymesQQ, return EVOK;)

SUFFIXNAN(  divsfxD, D, D, DIV, divDD   )
SUFFIXNAN(  divsfxZ, Z, Z, zdiv, divZZ  )

SUFFIXPFX(  maxsfxI, I, I, MAX, maxII   ,return EVOK;)
SUFFIXPFX(  maxsfxD, D, D, MAX, maxDD   ,return EVOK;)
SUFFIXPFX(  maxsfxX, X, X, XMAX, maxXX  ,return EVOK;)
SUFFIXPFX(  maxsfxQ, Q, Q, QMAX, maxQQ  ,return EVOK;)
SUFFIXPFX(  maxsfxS, SB,SB,SBMAX, maxSS ,return EVOK;)

SUFFIXPFX(  minsfxI, I, I, MIN, minII  ,return EVOK;)
SUFFIXPFX(  minsfxD, D, D, MIN, minDD  ,return EVOK;)
SUFFIXPFX(  minsfxX, X, X, XMIN, minXX  ,return EVOK;)
SUFFIXPFX(  minsfxQ, Q, Q, QMIN, minQQ  ,return EVOK;)
SUFFIXPFX(  minsfxS, SB,SB,SBMIN, minSS ,return EVOK;)

SUFFIXPFX(bw0000sfxI, UI,UI, BW0000, bw0000II,return EVOK;)
SUFFIXPFX(bw0001sfxI, UI,UI, BW0001, bw0001II,return EVOK;)
SUFFIXPFX(bw0010sfxI, UI,UI, BW0010, bw0010II,return EVOK;)
SUFFIXPFX(bw0011sfxI, UI,UI, BW0011, bw0011II,return EVOK;)
SUFFIXPFX(bw0100sfxI, UI,UI, BW0100, bw0100II,return EVOK;)
SUFFIXPFX(bw0101sfxI, UI,UI, BW0101, bw0101II,return EVOK;)
SUFFIXPFX(bw0110sfxI, UI,UI, BW0110, bw0110II,return EVOK;)
SUFFIXPFX(bw0111sfxI, UI,UI, BW0111, bw0111II,return EVOK;)
SUFFIXPFX(bw1000sfxI, UI,UI, BW1000, bw1000II,return EVOK;)
SUFFIXPFX(bw1001sfxI, UI,UI, BW1001, bw1001II,return EVOK;)
SUFFIXPFX(bw1010sfxI, UI,UI, BW1010, bw1010II,return EVOK;)
SUFFIXPFX(bw1011sfxI, UI,UI, BW1011, bw1011II,return EVOK;)
SUFFIXPFX(bw1100sfxI, UI,UI, BW1100, bw1100II,return EVOK;)
SUFFIXPFX(bw1101sfxI, UI,UI, BW1101, bw1101II,return EVOK;)
SUFFIXPFX(bw1110sfxI, UI,UI, BW1110, bw1110II,return EVOK;)
SUFFIXPFX(bw1111sfxI, UI,UI, BW1111, bw1111II,return EVOK;)


static A jtsuffix(J jt,    A w,A self){DECLF;I r;
 r=(RANKT)jt->ranks; RESETRANK; if(r<AR(w))return rank1ex(w,self,r,jtsuffix);
 return eachl(IX(SETIC(w,r)),w,jtatop(jt,fs,ds(CDROP)));
}    /* f\."r w for general f */

static A jtgsuffix(J jt,    A w,A self){A h,*hv,z,*zv;I m,n,r;
 r=(RANKT)jt->ranks; RESETRANK; if(r<AR(w))return rank1ex(w,self,r,jtgsuffix);
 SETIC(w,n); 
 h=VAV(self)->fgh[2]; hv=AAV(h); m=AN(h);
 GATV0(z,BOX,n,1); zv=AAV(z); I imod=0;
 DO(n, imod=(imod==m)?0:imod; RZ(zv[i]=df1(h,jtdrop(jt,sc(i),w),hv[imod])); ++imod;);
 return ope(z);
}    /* g\."r w for gerund g */


static A jtssg(J jt,    A w,A self){F1PREFIP;PROLOG(0020);A a,z;I i,n,r,wr;
 ASSERT(DENSE&AT(w),EVNONCE);
 // loop over rank - we claim to handle IRS
 wr=AR(w); r=(RANKT)jt->ranks; r=wr<r?wr:r; RESETRANK; if(r<wr)return rank1ex(w,self,r,jtssg);

 // From here on we are doing a single scan
 n=AS(w)[0]; // n=#cells
#define ZZFLAGWORD state
 I state;  // init flags, including zz flags

 A fs=FAV(FAV(self)->fgh[0])->fgh[0]; AF f2=FAV(fs)->valencefns[1]; // self = f/\.   FAV(self)->fgh[0] = f/  FAV(FAV(self)->fgh[0])->fgh[0] = f   fetch dyad for f
 // Set BOXATOP if appropriate.  Since {:y is always the last cell, BOXATOP is allowed only when the rank of w is 1, meaning that
 // {:y is a single box, just like the other results.  Also require that w be boxed, lest we make the first z-cell invalid
 state = ZZFLAGINITSTATE|(REPSGN(wr-2)&(AT(w)>>(BOXX-ZZFLAGBOXATOPX))&((FAV(fs)->flag2&VF2BOXATOP2)>>(VF2BOXATOP2X-ZZFLAGBOXATOPX)));  // If rank OK, extract flag.  Rank cannot be 0.  Don't touch fs yet, since we might not loop
 state &= ~((FAV(fs)->flag2&VF2ATOPOPEN2W)>>(VF2ATOPOPEN2WX-ZZFLAGBOXATOPX));  // We don't handle &.> here; ignore it

 // We cannot honor WILLBEOPENED, because the same box that goes into the result must also be released into the next application of f.
 // Without WILLBEOPENED, there is no need to set COUNTITEMS
// not checked state |= (-state) & (I)jtinplace & JTCOUNTITEMS; // remember if this verb is followed by ; - only if we BOXATOP, to avoid invalid flag setting at assembly
#define ZZWILLBEOPENEDNEVER 1

 // Allocate virtual block for the running x argument.  UNINCORPABLE.  Should be inplaceable, but we don't yet
 fauxblock(virtafaux); fauxvirtual(a,virtafaux,w,r-1,ACUC1);
 // z will hold the result from the iterations.  Init to value of last cell
 // Since there are multiple cells, z will be in a virtual block to begin with (usually)
 // Allocate fauxvirtual arg for the first cell, so it can be inplaceable/pristine if needed (tail returned a noninplaceable virtual block, which messed things up for high rank)
 fauxblock(virtwfaux); fauxvirtual(z,virtwfaux,w,r-1,ACUC1);  // allocate UNINCORPORABLE block, mark inplaceable - used only once
 // fill in the shape, offset, and item-count of the virtual block
 I k; PROD(k,r-1,AS(w)+1);  // k=#atoms of cell of w
 AN(z)=k; AN(a)=k;
 k<<=bplg(AT(w)); // k now=length of input cell in bytes, where it will remain
 AK(z)+=(n-1)*k; AK(a)+=(n-1)*k; MCISH(AS(z),AS(w)+1,r-1); MCISH(AS(a),AS(w)+1,r-1);  // a points to tail; it will be decremented before first use
 // Calculate inplaceability.  We can inplace the left arg, which is always virtual, if w is inplaceable and (w is direct or fs is &.>)
 // We include contextual inplaceability (from jtinplace) here because if the block is returned, its pristinity will be checked if it is inplaceable.  Thus
 // we do not want to call a faux argument inplaceable if it really isn't.  This gives us leeway with jtinplace itself
 state |= (UI)(SGNIF((I)jtinplace,JTINPLACEWX)&~((AT(w)&TYPEVIPOK)-(f2!=jtevery2self))&AC(w))>>(BW-1-ZZFLAGVIRTAINPLACEX);   // requires JTINPLACEWX==0.  Single flag bit
 // We can inplace the right arg the first time if it is direct inplaceable, and always after that (assuming it is an inplaceable result).
 // and the input jtinplace.  We turn off WILLBEOPENED status in jtinplace for the callee.
 AC(z)=ACUC1 + ((state&ZZFLAGVIRTAINPLACE)<<(ACINPLACEX-ZZFLAGVIRTAINPLACEX));   // first cell is inplaceable if second is
 jtinplace = (J)(intptr_t)(((I)jt) + (JTINPLACEW+JTINPLACEA)*((FAV(fs)->flag>>(VJTFLGOK2X-JTINPLACEWX)) & JTINPLACEW));  // all items are used only once

#define ZZPOPNEVER 1   // we mustn't TPOP after copying the result atoms, because they are reused.  This will leave the memory used for type-conversions unclaimed.

 // We have to dance a bit for BOXATOP verbs, because the result comes back unboxed, but it has to be put into a box
 // to be fed into the next iteration.  This is still a saving, because we can use the same box to point to each successive result.
 // Exception: if the reusable box gets incorporated, it is no longer reusable and must be reallocated.  We will use the original z box,
 // which will NEVER be virtual because it is an atom whenever BOXATOP is set, as the starting pointer to the prev boxed result
 A boxedz=z; z=AAV(z)[0]; z=(state&ZZFLAGBOXATOP)?z:boxedz;  // init current pointer for the temp box; if BOXATOP, use >{:y as the first (to-be-boxed) result (always safe to fetch 1 value from z)

#define ZZDECL
#define ZZSTARTATEND 1   // build result from bottom up
#include "result.h"

  ZZPARMS(1,n,2)
#define ZZINSTALLFRAME(optr) *optr++=n;

 AD * RESTRICT zz=0;
 for(i=0;i<n;++i){   // loop through items, noting that the first is the tail itself
  if(i){RZ(z=CALL2IP(f2,a,z,fs));}   // apply the verb to the arguments (except the first time)
  // In case we are performing f&.>, we must set z non-PRISTINE, which it is by definition because all its boxes have escaped into the running result
  AFLAG(z)&=~AFPRISTINE;  // we just stored z, so it's never pristine here

#define ZZBODY
#include "result.h"
  // If BOXATOP, we need to reinstate the boxing around z for the next iteration.
  if(state&ZZFLAGBOXATOP){
   // If boxedz itself has been incorporated into the result, we have to reallocate it.  We don't need the usual check for z==boxedz, because we know we INCORPed z into
   // the boxed result, so if it was the same as boxedz, the usecount of boxedz was incremented then
   if(!ACIPISOK(boxedz))GAT0(boxedz,BOX,1,0);   // reallocate boxedz if needed
   ACIPNO(z); AAV(boxedz)[0]=z; z=boxedz;  // incorporate z into boxedz; point boxedz to the previous result, and make that the new argument for next time
  }
  // if result happens to be the same virtual block that we passed in, we have to clone it before we change the pointer
  else if(a==z){RZ(z=virtual(z,0,AR(a))); AN(z)=AN(a); MCISH(AS(z),AS(a),r-1);}

  AK(a)-=k;  // back up to next input
  AC(a)=ACUC1 + ((state&ZZFLAGVIRTAINPLACE)<<(ACINPLACEX-ZZFLAGVIRTAINPLACEX));   // in case we created a virtual block from it, restore inplaceability to the UNINCORPABLE block
 }
#define ZZEXIT
#include "result.h"
 EPILOG(zz);  // this frees the virtual block, at the least
}    /* f/\."r w for general f and 1<(-r){$w and -.0 e.$w */

A jtscansp(J jt,A w,A self,AF sf){A e,ee,x,z;B*b;I f,m,j,r,t,wr;P*wp,*zp;
 wr=AR(w); r=(RANKT)jt->ranks; r=wr<r?wr:r; RESETRANK; f=wr-r;
 wp=PAV(w); e=SPA(wp,e); RZ(ee=over(e,e));
 if(!equ(ee,CALL1(sf,ee,self))){
  RZ(x=denseit(w));
  return IRS1(x,self,r,sf,z);
 }else{
  RZ(b=bfi(wr,SPA(wp,a),1));
  if(r&&b[f]){b[f]=0; RZ(w=jtreaxis(jt,jtifb(jt,wr,b),w));}
  j=f; m=0; DQ(wr-f, m+=!b[j++];);
 }
 wp=PAV(w); e=SPA(wp,e); x=SPA(wp,x);
 RZ(x=IRS1(x,self,m,sf,z));
 t=maxtype(AT(e),AT(x)); RZ(e=jtcvt(jt,t,e)); if(TYPESNE(t,AT(x)))RZ(x=jtcvt(jt,t,x));
 GASPARSE(z,STYPE(t),1,wr+!m,AS(w)); if(!m)*(wr+AS(z))=1;
 zp=PAV(z); 
 SPB(zp,e,e); 
 SPB(zp,x,x); 
 SPB(zp,i,ca(SPA(wp,i))); 
 SPB(zp,a,ca(SPA(wp,a)));
 return z;
}    /* f/\"r or f/\."r on sparse w */

static A jtsscan(J jt,    A w,A self){A y,z;I d,f,m,n,r,t,wn,wr,*ws,wt;
 F1PREFIP;
 if(!w) return 0;
 wt=AT(w);
 if((SPARSE&wt)!=0)return scansp(w,self,jtsscan);
 wn=AN(w); wr=AR(w); r=(RANKT)jt->ranks; r=wr<r?wr:r; f=wr-r; ws=AS(w); RESETRANK;
 PROD(m,f,ws); PROD1(d,r-1,f+ws+1); I *nn=&ws[f]; nn=r?nn:&I1mem; n=*nn;   // will not be used if WN==0, so PROD ok.  n is # items along the selected rank
 y=FAV(self)->fgh[0]; // y is f/
 if(((n-2)|(wn-1))<0){if(FAV(FAV(y)->fgh[0])->flag&VISATOMIC2){return r?RETARG(w):jtreshape(jt,apip(shape(w),zeroionei(1)),w);}else return IRS1(w,self,r,jtsuffix,z);}  // if empty arg, or just 1 cell in selected axis, convert to f/\ which handles the short arg

   // note that the above line always takes the r==0 case
 VARPS adocv; varps(adocv,self,wt,2);  // analyze f - get suffix routine
 if(!adocv.f)return IRSIP1(w,self,r,jtssg,z);   // if not supported atomically, go do general suffix
 // The rest handles primitives with fast suffix scans
 if((t=atype(adocv.cv))&&TYPESNE(t,wt))RZ(w=jtcvt(jt,t,w));
 if(ASGNINPLACESGN(SGNIF((I)jtinplace,JTINPLACEWX)&SGNIF(adocv.cv,VIPOKWX),w))z=w; else GA(z,rtype(adocv.cv),wn,wr,ws);
 I rc=((AHDRSFN*)adocv.f)(d,n,m,AV(w),AV(z),jt);
 if(rc&255){jsignal(rc); return jt->jerr>=EWOV?IRS1(w,self,r,jtsscan,z):0;} else return adocv.cv&VRI+VRD?jtcvz(jt,adocv.cv,z):z;
}    /* f/\."r w main control */


static A jtomask(J jt,A a,A w){A c,r,x,y;I m,n,p;
 RE(m=i0(a)); p=ABS(m); SETIC(w,n);
 r=sc(0>m?(n+p-1)/p:MAX(0,1+n-m)); c=tally(jt, w);
 x=jtreshape(jt,sc(p),  num(0));
 y=jtreshape(jt,0>m?c:r,num(1) );
 return reshapeW(over(r,c),over(x,y));
}

static A jtgoutfix(J jt,A a,A w,A self){A h,*hv,x,z,*zv;I m,n;
 RZ(x=jtomask(jt,a,w));
 SETIC(x,n);
 h=VAV(self)->fgh[2]; hv=AAV(h); m=AN(h);
 GATV0(z,BOX,n,1); zv=AAV(z); I imod=0;
 DO(n, imod=(imod==m)?0:imod; RZ(zv[i]=df1(h,jtrepeat(jt,jtfrom(jt,sc(i),x),w),hv[imod])); ++imod;);
 return ope(z);
}

static AS2(jtoutfix, eachl(jtomask(jt,a,w),w,jtatop(jt,fs,ds(CPOUND))),0117)

static A jtofxinv(J jt,A a,A w,A self){A f,fs,z;C c;I t;V*v;
 F2RANKW(0,RMAX,jtofxinv,self);
 fs=FAV(self)->fgh[0]; f=FAV(fs)->fgh[0]; v=FAV(f); c=v->id; t=AT(w);  // self = f/\. fs = f/  f = f  v = verb info for f
 if(!(c==CPLUS||c==CBDOT&&t&INT||((c&-2)==CEQ)&&t&B01))return outfix(a,w,self);  // if not +/\. or m b./\. or =/\. or ~:/\.
 A z0,z1; z=irs2(df1(z0,w,fs),df2(z1,a,w,bslash(fs)),c==CPLUS?ds(CMINUS):f, RMAX,-1L,jtatomic2);
 if(jt->jerr==EVNAN){RESETERR; return outfix(a,w,self);}else return z;
}    /* a f/\. w where f has an "undo" */

static A jtofxassoc(J jt,A a,A w,A self){A f,i,j,p,s,x,z;C id,*zv;I c,d,k,kc,m,r,t;V*v;VA2 adocv;
 F2RANKW(0,RMAX,jtofxassoc,self);
 SETIC(w,m); RE(k=i0(a)); c=ABS(k);  // m = # items in w; k is value of a; c is # items per suffix
 f=FAV(self)->fgh[0]; x=FAV(f)->fgh[0]; v=FAV(x); id=CBDOT==v->id?(C)AV(v->fgh[1])[0]:v->id;  // self = f/\. f = f/  x = f  v = verb info for f
 if(k==IMIN||m<=c||id==CSTARDOT&&!(B01&AT(w)))return outfix(a,w,self);  // if there is not >1 outfix, do general code which handles empties
 if(-1<=k){d=m-c;     RZ(i=IX(d)); RZ(j=apv(d,c,1L));}
 else     {d=(m-1)/c; RZ(i=apv(d,c-1,c )); RZ(j=apv(d,c,c ));}
 // d is (number of result cells)-1; i is indexes of last item of the excluded infix for cells AFTER the first
 // j is indexes of first item AFTER the excluded infix for cells BEFORE the last
 RZ(p=jtfrom(jt,i,df1(z,w,bslash(f)))); // p is i { u\ w; that is, the totals of the prefixes after the first
 RZ(s=jtfrom(jt,j,df1(z,w,bsdot(f))));  // s is j { u\. w; totals of suffixes except the last
 // We need to make sure that p, s, and (p f s) all have the same type.  This is problematic, since we don't actually see
 // the type of (p f s) which is encoded in cv below.  But since this case is limited to atomic associative verbs, we
 // know that if p and s have the same type, p f s will also, except that it might overflow, which we will detect after we
 // run it.
 //
 // If we modify this code to use this path for other associative verbs, we would need to check the type of (p f s)
 I rc;  // return code from execution of verb
 if(!TYPESEQ(AT(p),AT(s))){rc=EWOV;} else {I klg;  // simulate overflow if different precisions - will convert everything to float
  r=AR(p); PROD(c,AR(p)-1,AS(p)+1) t=AT(p); klg=bplg(t); kc=c<<klg;
  adocv=var(x,t,t); // analyze the u operand
  ASSERTSYS(adocv.f,"ofxassoc");  // scaf
  GA(z,t,c*(1+d),r,AS(p)); AS(z)[0]=1+d; zv=CAV(z);  // allocate result assuming no overflow
  memcpy(zv,     AV(s),          kc);                     // first cell is {.s, i. e. all but the first infix
  rc=(1<d)?((AHDR2FN*)adocv.f)((I)1,c*(d-1),AV(p),kc+CAV(s),zv+kc,jt):EVOK;  /* (}:p) f (}.s), with result stored into the result area */  // don't call with 0 length!
  memcpy(zv+kc*d,CAV(p)+kc*(d-1),kc);                     // last cell is {:p, i. e. all but the last infix
  // If there was overflow on the ado, we have to redo the operation as a float.
  // We also have to redo if the types of p and s were different (for example, if one overflowed to float and the other didn't)
 }
 if((rc&255)>=EWOV){ return ofxassoc(a,jtcvt(jt,FL,w),self);}
 if(rc)jsignal(rc);  // if there was an error, signal it
 return z;
}    /* a f/\. w where f is an atomic associative fn */

static A jtiota1rev(J jt,    A w,A self){I j; SETIC(w,j); return apv(j,j,-1L);}

 A jtbsdot(J jt, A w){A f;AF f1=jtsuffix,f2=jtoutfix;I flag=FAV(ds(CBSDOT))->flag;C id;V*v;  // init flag is IRS1
 if(NOUN&AT(w))return fdef(0,CBSLASH,VERB, jtgsuffix,jtgoutfix, w,0L,jtfxeachv(jt,1L,w), VGERL|VAV(ds(CBSLASH))->flag, RMAX,0L,RMAX);
 v=FAV(w);  // verb info for w
 switch(v->id){
  case CPOUND: f1=jtiota1rev; break;
  case CSLASH:  // v is f/, but not when f is a gerund
   f1=jtsscan;  // code for f/\. - take inplaceability from dyad f
   f=v->fgh[0]; flag|=(FAV(f)->flag&VJTFLGOK2)>>(VJTFLGOK2X-VJTFLGOK1X); id=ID(f); if(id==CBDOT){f=VAV(f)->fgh[1]; if(INT&AT(f)&&!AR(f))id=(C)AV(f)[0];}
#define xinvvalues(w) CCM(w,CPLUS)+CCM(w,CEQ)+CCM(w,CNE)+CCM(w,CBW0110)+CCM(w,CBW1001)
   {CCMWDS(xinv) CCMCAND(xinv,cand,id) f2=CCMTST(cand,id)?jtofxinv:f2;}
#define xassocvalues(w) CCM(w,CSTAR)+CCM(w,CMAX)+CCM(w,CMIN)+CCM(w,CPLUSDOT)+CCM(w,CSTARDOT)+CCM(w,CBW0000)+CCM(w,CBW0001)+CCM(w,CBW0011)+CCM(w,CBW0101)+CCM(w,CBW0111)+CCM(w,CBW1111)
   {CCMWDS(xassoc) CCMCAND(xassoc,cand,id) f2=CCMTST(cand,id)?jtofxassoc:f2;}
  }
 RZ(f=ADERIV(CBSDOT,f1,f2,flag,RMAX,0,RMAX));
 // Fill in the lvp[1] field: with 0 if not f/\; with the lookup field for f/ if f/\ .
 FAV(f)->localuse.lvp[1]=v->id==CSLASH?v->localuse.lvp[1]:0;  // f is nonnull if f/\ .
 return f;
}
