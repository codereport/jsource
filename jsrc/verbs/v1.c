/* Copyright 1990-2007, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Verbs: Match Associates                                                 */

#include "j.h"
#include "vcomp.h"
#include "ve.h"

// To reduce parameter overhead, we call matchsub with trailing parameters omitted if x==0.  This is fine for clang.  In MSVC, the parameter area
// is used as a workarea by the routine, and thus omitting the parms makes for a segfault.
#if defined(__clang__)
#define MATCHSUBDEFAULTS
#else
#define MATCHSUBDEFAULTS ,0,0,1,1,1
#endif
static B jtmatchsub(J,A,A,B* RESTRICT,I,I,I,I,I);
static A jtmatchs(J jt,A a,A w);

#define MCS(q,af,wf)  ((((q>1)+(q>0))<<2)+(af?2:0)+(wf?1:0))
// set *x++ to b1 if *u=*v, b0 otherwise
#define QLOOP         b=b1; DO(q, if(u[i]!=v[i]){b^=1; break;}); *x++=b;
// comparison, with special cases for 1/more than 1, and looping over repeated cells
// we know there is a frame somewhere.  This is for plain byte comparison - no tolerance
#define EQV(T)        \
 {T h,* RESTRICT u=(T*)av,* RESTRICT v=(T*)wv;                                                   \
  q=k/sizeof(T);                                                             \
  switch(MCS(q,af,wf)){                                                      \
   case MCS(1,0,1): h=*u; if(b1)DQ(mn, *x++=h   ==*v++;) else DQ(mn, *x++=h   !=*v++;)  break;  \
   case MCS(1,1,0): h=*v; if(b1)DQ(mn, *x++=*u++==h;   ) else DQ(mn, *x++=*u++!=h;   ); break;  \
   case MCS(1,1,1): if(b1){                                                  \
                     if(1==n)      DQ(m,               *x++=*u++==*v++;  )   \
                     else if(af<wf)DQ(m, h=*u++; DQ(n, *x++=h   ==*v++;);)   \
                     else          DQ(m, h=*v++; DQ(n, *x++=*u++==h;   ););  \
                    }else{                                                   \
                     if(1==n)      DQ(m,               *x++=*u++!=*v++;  )   \
                     else if(af<wf)DQ(m, h=*u++; DQ(n, *x++=h   !=*v++;);)   \
                     else          DQ(m, h=*v++; DQ(n, *x++=*u++!=h;   ););  \
                    } break;                                                 \
   case MCS(2,0,1): DQ(mn, QLOOP;       v+=q;); break;                       \
   case MCS(2,1,0): DQ(mn, QLOOP; u+=q;      ); break;                       \
   case MCS(2,1,1): if(1==n)      DQ(m,       QLOOP; u+=q;   v+=q;)          \
                    else if(af<wf)DQ(m, DQ(n, QLOOP; v+=q;); u+=q;)          \
                    else          DQ(m, DQ(n, QLOOP; u+=q;); v+=q;); break;  \
 }}

// comparison function for non-float arrays, in chunks of size k, storing match results into *x and
// returning the first one as the result (nothing special about first, could be any one)
// This is a generalized version of the INNERT macro below
// If we get here, we know that at least one of the arguments has a frame
// m=#cells of shorter frame, n=#times a cell of shorter frame must be repeated
static B eqv(I af,I wf,I m,I n,I k,C*av,C*wv,B* RESTRICT x,B b1){B b,* RESTRICT xx=x;I mn=m*n,q;
  // select a comparison loop based on the size of the data area.  It's all about the fastest way to compare bytes
 if     (0==(k&(SZI-1))  )EQV(I)
 else if(0==(k&(SZI4-1)))EQV(int)
 else if(0==(k&(SZS-1))  )EQV(S)
 else if(1==k)            EQV(C)
 else{
  if(af<wf)DQ(m, DQ(n, *x++=(!!memcmp(av,wv,k))^b1; wv+=k;); av+=k;)
  else          DQ(m, DQ(n, *x++=(!!memcmp(av,wv,k))^b1; av+=k;); wv+=k;);
 }
 return xx[0];
}    /* what memcmp should have been */

// Return 1 if a and w match, 0 if not
B jtequ(J jt,A a,A w){A x;
 F2PREFIP;
 if(!(a && w)) return 0;  // allow inplace request - it has no effect
 if(a==w)return 1;
 if((SPARSE&(AT(a)|AT(w)))!=0)if(AR(a)&&AR(w)){RZ(x=jtmatchs(jt,a,w)); return BAV(x)[0];}
 return ((B (*)())jtmatchsub)(jt,a,w,0   MATCHSUBDEFAULTS);  // don't check level - it takes too long for big arrays
}

// Return 1 if a and w match intolerantly, 0 if not
B jtequ0(J jt,A a,A w){
 F2PREFIP;  // allow inplace request - it has no effect
 PUSHCCT(1.0) B res=equ(a,w); POPCCT return res;
}

// Test for equality of functions, 1 if they match.  To match, the functions must have the same pseudocharacter and fgh
static B jteqf(J jt,A a,A w){A p,q;V*u=FAV(a),*v=FAV(w);
 if(TYPESXOR(AT(a),AT(w))+(u->id^v->id))return 0;   // must match on type and id
 p=u->fgh[0]; q=v->fgh[0]; if(!((p==q||p&&q&&((B (*)())jtmatchsub)(jt,p,q,0   MATCHSUBDEFAULTS))))return 0;
 p=u->fgh[1]; q=v->fgh[1]; if(!((p==q||p&&q&&((B (*)())jtmatchsub)(jt,p,q,0   MATCHSUBDEFAULTS))))return 0;
 p=u->fgh[2]; q=v->fgh[2];    return (p==q||p&&q&&((B (*)())jtmatchsub)(jt,p,q,0   MATCHSUBDEFAULTS));
}

// compare function for boxes.  Do a test on the single contents of the box.  Reset comparison direction to normal.
#define EQA(a,w) \
 ((-(a!=w)&((AN(a)^AN(w))-1))>=0?(a==w):((B (*)())jtmatchsub)(jt,a,w,0   MATCHSUBDEFAULTS))
// compare rationals
#define EQQ(a,w)  (equ(a.n,w.n)&&equ(a.d,w.d))

// compare arrays for equality of all values.  f is the compare function
// m=#cells of shorter frame, n=#times a cell of shorter frame must be repeated
// x[] is result array.  This can be 0 if we are doing a comparison inside a box, in which case
// we don't store the result.  In any case, b holds the result of the last comparison
#define INNERT(T,f)                  \
 {T* RESTRICT u=(T*)av,* RESTRICT v=(T*)wv; I m0=m, n0=n; m0=x==0?1:m0; n0=x==0?1:n0;  /* u->a data, v->w data */           \
  if(1==n0){      DO(m0,       B b=b1; DO(c, if(!f(u[i],v[i])){b^=1; break;}); if(!x)return b^b1^1; x[j++]=b; u+=c; v+=c;)}  \
  else if(af>wf)DO(m0, DO(n0, B b=b1; DO(c, if(!f(u[i],v[i])){b^=1; break;}); x[j++]=b; u+=c;); v+=c;)  \
  else          DO(m0, DO(n0, B b=b1; DO(c, if(!f(u[i],v[i])){b^=1; break;}); x[j++]=b; v+=c;); u+=c;)  \
 }

// compare functions for float/complex intolerant comparison
#define DEQCT0(a,w) ((a)==(w))
#define ZEQCT0(a,w) ( ((a).re==(w).re) && ((a).im==(w).im) )

// match two values, returning b1 if match, 1^b1 if no match.  If the values are functions, that's all we return.  If the values are nouns, we
// store the match value(s) in *x.  x may be 0, if af and wf are 0 and m and n are 1.  In this case we don't store anything.
// but return the match status.  We use this when comparing boxed arrays or functions
// For recursive calls, x=0, af=wf=0, and m=n=1.  If there is frame, arguments have been exchanged so that a has the shorter frame, i. e. af<=wf, and a will be repeated
// b1 is the value to use for 'match' - 1 normally, but 0 for top level of -.@-:
// m=#cells of shorter frame, n=#times a cell of shorter frame must be repeated
// the comparands may not be sparse
// arguments after x may be omitted if x==0, and are then assumed to be 0 0 1 1 1
static B jtmatchsub(J jt,A a,A w,B* RESTRICT x,I af,I wf,I m,I n,I b1){C*av,*wv;I at,c,j=0,t,wt;
 // we tested for a==w before the call, to save on call overhead (usually)
 // m*n cannot be 0.  If this is a recursive call, m=n=1; while if it is the first call, empty m/n were handled at the top level
 {
 af&=(-(x!=0)); wf&=(-(x!=0));   // if x not given, use defaults for af, wf
 I p=AR(a)-af;
 I q=AR(w)-wf; 
 // p=cell-rank of a; q=cell-rank of w; ?t=type;
 // c=#atoms in a cell, b is 1 if rank or cell-shape mismatches, or if cells are not empty and types are incompatible
 // We know that either there is no frame or both arguments are nonempty (Empty arguments with frame can happen only at the top level
 // and were handled there).
 // Do all the tests for miscompare and combine the results at the end
 // We assume the caller checked for a==w and handled it, so we don't.
 I shapediff=p;  // see if ranks differ
 p=q<p?q:p; q^=shapediff; TESTDISAGREE(shapediff,af+AS(a),wf+AS(w),p); // now p is smaller rank; q=ranks differ; shapediff=shapes differ
 shapediff|=q;  // shapes or ranks differ
 PROD(c,p,af+AS(a));  // get c=length of a cell in atoms
 at=UNSAFE(AT(a)); wt=UNSAFE(AT(w));   // save types, now that register pressure is over
 p=NEGIFHOMO(at,wt);  // now p= neg if homogeneous args
 if(((shapediff-1)&(-c)&p)>=0){  // skip compare if rank differ, or if shapes differ, or if inhomo, or if empty; not checking for identical args
  // create result, !b1 if there was a difference in shape or inhomo, b1 otherwise
  p=1^SGNTO0((shapediff-1)&((c-1)|p));  // p=1 if error, =no match  ignore inhomo error if empty
  if(x)memset(x,p^b1,m*n);else b1=1; return p^b1;  // write 'error' if writing enabled; return false
 }
 }




 // If we're comparing functions, return that result
 t=at;  //  in case types identical, pick one
 if(t&FUNC)return (!jteqf(jt,a,w))^(x==0?1:b1);  // true value, but switch if return is not 'match'
 // If the types mismatch, convert as needed to the common (unsafe) type calculated earlier
 if(at!=wt) {
  t=maxtypedne(at,wt);
  if(at!=t)RZ(a=jtcvt(jt,t|VFRCEXMT,a));
  if(wt!=t)RZ(w=jtcvt(jt,t|VFRCEXMT,w));
 }
 // If a has no frame, it might be the shorter frame and therefore repeated; but in that case
 // m will be 1 (1 cell in shorter frame).  So it is safe to increment each address by c in the compare loops
 av=CAV(a);
 wv=CAV(w);
 // do the comparison, leaving the last result in b
 switch(CTTZ(t)){
  // Take the case of no frame quickly, because it happens on each recursion and also in much user code
 default:
  c <<= bplg(t);
  if(!x){
    return !memcmp(av,wv,c);   // single call, thus not stored - return it immediately
  }else{
   return eqv(af,wf,m,n,c,av,wv,x,b1);  // stored version loops & stores
  }
  break;
 case CMPXX: if(1.0!=jt->cct){INNERT(Z,zeq) break;}  // tolerant, must use complex distance
  INNERT(Z,ZEQCT0) break;
 case FLX:
   if(1.0!=jt->cct)INNERT(D,TEQ)else INNERT(D,DEQCT0)
  break;
 case XNUMX: INNERT(X,equ); break;
 case RATX:  INNERT(Q,EQQ); break;
 case BOXX:
   INNERT(A,EQA); break;
 }
 return 0;  // Return value matters only for single compare (x=0); we have returned already in that case
}

static A jtmatchs(J jt,A a,A w){A ae,ax,p,q,we,wx,x;B*b,*pv,*qv;D d;I acr,an=0,ar,c,j,k,m,n,r,*s,*v,wcr,wn=0,wr;P*ap,*wp;
 ar=AR(a); acr=jt->ranks>>RANKTX; acr=ar<acr?ar:acr; r=ar;
 wr=AR(w); wcr=(RANKT)jt->ranks; wcr=wr<wcr?wr:wcr; RESETRANK;
 if(ar>acr||wr>wcr)return rank2ex(a,w,UNUSED_VALUE,acr,wcr,acr,wcr,jtmatchs);
 if(ar!=wr||memcmpne(AS(a),AS(w),r*SZI)||!HOMO(AT(a),AT(w)))return num(0);
 GATV0(x,B01,r,1L); b=BAV(x); memset(b,C0,r);
 if(SPARSE&AT(a)){ap=PAV(a); x=SPA(ap,a); v=AV(x); an=AN(x); DO(an, b[v[i]]=1;);}
 if(SPARSE&AT(w)){wp=PAV(w); x=SPA(wp,a); v=AV(x); wn=AN(x); DO(wn, b[v[i]]=1;);} 
 c=0; DO(r, c+=b[i];);
 if(an<c||DENSE&AT(a))RZ(a=reaxis(jtifb(jt,r,b),a)); ap=PAV(a); ae=SPA(ap,e); ax=SPA(ap,x); m=*AS(ax);
 if(wn<c||DENSE&AT(w))RZ(w=reaxis(jtifb(jt,r,b),w)); wp=PAV(w); we=SPA(wp,e); wx=SPA(wp,x); n=*AS(wx);
 RZ(x=jtindexof(jt,SPA(ap,i),SPA(wp,i))); v=AV(x);
 GATV0(p,B01,m,1); pv=BAV(p);
 GATV0(q,B01,n,1); qv=BAV(q); 
 memset(pv,C1,m); DO(n, j=*v++; if(j<m)pv[j]=qv[i]=0; else qv[i]=1;);
 if(memchr(pv,C1,m)&&!all1(eq(we,repeat(p,ax))))return num(0);
 if(memchr(qv,C1,n)&&!all1(eq(ae,repeat(q,wx))))return num(0);
 j=0; DO(m, if(pv[i])++j;);
 k=0; DO(n, if(qv[i])++k; qv[i]=!qv[i];);
 if(!equ(jtfrom(jt,repeat(q,x),ax),repeat(q,wx)))return num(0);
 x=SPA(ap,a); v=AV(x); s=AS(a); d=1.0; DO(AN(x), d*=s[v[i]];);
 return d==m+k&&d==n+j||equ(ae,we)?num(1):num(0);
}    /* a -:"r w on sparse arrays */


// x -:"r y or x -.@-:"r y depending on LSB of jt
 A jtmatch(J jt,A a,A w){A z;I af,m,n,mn,wf;
 I eqis0 = (I)jt&1; jt=(J)((I)jt&~1);
 I isatoms = (-AN(a))&(-AN(w));  // neg if both args have atoms
 if((SPARSE&(AT(a)|AT(w)))!=0)return ne(num(eqis0),jtmatchs(jt,a,w));
 af=AR(a)-(I)(jt->ranks>>RANKTX); af=af<0?0:af; wf=AR(w)-(I)((RANKT)jt->ranks); wf=wf<0?0:wf; RESETRANK;
 // exchange a and w as needed to ensure a has the shorter frame, i. e. is the repeated argument
 {A ta=a; I ti=af; I afhi=af-wf; a=afhi>=0?w:a; w=afhi>=0?ta:w; af=afhi>=0?wf:af; wf=afhi>=0?ti:wf;} 
 // If either operand is empty return without any comparisons.  In this case we have to worry that the
 // number of cells may overflow, even if there are no atoms
 if(isatoms>=0){B b; I p;  // AN(a) is 0 or AN(w) is 0
  // no atoms.  The shape of the result is the length of the longer frame.  See how many cells that is
  PRODX(mn,wf,AS(w),1)
  // The compare for each cell is 1 if the cell-shapes are the same
  p=AR(a)-af; b=p==(AR(w)-wf)&&!ICMP(af+AS(a),wf+AS(w),p);   // b =  shapes are the same
  // Allocate & return result
  GATV(z,B01,mn,wf,AS(w)); memset(BAV(z),b^eqis0,mn); return z;
 }
 // There are atoms.  If there is only 1 cell to compare, do it quickly
 if(wf==0){
 I nocall = (-(a!=w)&((AN(a)^AN(w))-1));
 return num((nocall>=0?SGNTO0(nocall)+(a==w):((B (*)())jtmatchsub)(jt,a,w,0   MATCHSUBDEFAULTS))^eqis0);  // SGNTO0 to prevent misbranch
 }
 // Otherwise we are doing match with rank.  Set up for the repetition in matchsub
 // Create m: #cells in shorter (i. e. common) frame  n: # times cell of shorter frame is repeated
 PROD(m,af,AS(w)); PROD(n,wf-af,AS(w)+af);
 mn=m*n;  // total number of matches to do, i. e. # results
 GATV(z,B01,mn,wf,AS(w)); matchsub(a,w,BAV(z),af,wf,m,n,eqis0^1);  // matchsub stores, and we ignore the result
 // We do not check for a==w here & thus will compare them
 return z;
}    /* a -:"r w */

 A jtnotmatch(J jt,A a,A w){return jtmatch((J)((I)jt+1),a,w);}   /* a -.@-:"r w */
