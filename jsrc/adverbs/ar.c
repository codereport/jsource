/* Copyright 1990-2014, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Adverbs: Reduce (Insert), Outer Product, and Fold                              */

#include "j.h"
#include "verbs/vasm.h"
#include "verbs/ve.h"
#include "verbs/vcomp.h"
#include "ar.h"

static A jtreduce(J jt,    A w,A self);


#define PARITY2         u=(B*)&s; b=0; b^=*u++; b^=*u++;
#define PARITY4         u=(B*)&s; b=0; b^=*u++; b^=*u++; b^=*u++; b^=*u++; 
#define PARITY8         u=(B*)&s; b=0; b^=*u++; b^=*u++; b^=*u++; b^=*u++; b^=*u++; b^=*u++; b^=*u++; b^=*u++;
#define PARITYW         PARITY8

#define VDONE(T,PAR)  \
 {I q=n/sizeof(T);T s,*y=(T*)x; DQ(m, s=0; DQ(q, s^=*y++;); PAR; *z++=b==pc;);}

static void vdone(I m,I n,B*x,B*z,B pc){B b,*u;
 if(1==m){UI s,*xi;
  s=0; b=0;
  xi=(UI*)x; DQ(n>>LGSZI, s^=*xi++;); 
  u=(B*)xi; DQ(n&(SZI-1), b^=*u++;);
  u=(B*)&s; DQ(SZI,   b^=*u++;);
  *z=b==pc;
 }else if(0==(n&(sizeof(UI  )-1)))VDONE(UI,  PARITYW)
 else  if(0==(n&(sizeof(UINT)-1)))VDONE(UINT,PARITY4)
 else  if(0==(n&(sizeof(US  )-1)))VDONE(US,  PARITY2)
 else  DQ(m, b=0; DQ(n, b^=*x++;); *z++=b==pc;);
}

#define RBFXLOOP(T,pfx)  \
 {T* RESTRICT xx=(T*)x,* RESTRICT yy,*z0,* RESTRICT zz=(T*)z;   \
  q=d/sizeof(T);                  \
  for(j=0;j<m;++j){               \
   yy=xx; xx-=q; z0=zz; DQ(q, --xx; --yy; --zz; *zz=pfx(*xx,*yy););    \
   DQ(n-2,       zz=z0; DQ(q, --xx;       --zz; *zz=pfx(*xx,*zz);););  \
 }}  /* non-commutative */

#define RBFXODDSIZE(pfx,bpfx)  RBFXLOOP(C,bpfx)
#define REDUCECFX              REDUCEBFX

// no errors possible here
#define REDUCEBFX(f,pfx,ipfx,spfx,bpfx,vdo)  \
 I f(I d,I n,I m,B* RESTRICTI x,B* RESTRICTI z,J jt){B*y=0;I j,q;                       \
  if(d==1){vdo; return EVOK;}                                \
  x+=m*d*n; z+=m*d;                           \
          \
  if(0==d%sizeof(UI  ))RBFXLOOP(UI,   pfx)    \
  else if(0==d%sizeof(UINT))RBFXLOOP(UINT,ipfx)    \
  else if(0==d%sizeof(US  ))RBFXLOOP(US,  spfx)    \
  else                      RBFXODDSIZE(pfx,bpfx)  \
 return EVOK;}  /* non-commutative */

REDUCECFX(  eqinsB, EQ,  IEQ,  SEQ,  BEQ,  vdone(m,n,x,z,(B)(n&1)))
REDUCECFX(  neinsB, NE,  INE,  SNE,  BNE,  vdone(m,n,x,z,1       ))
REDUCECFX(  orinsB, OR,  IOR,  SOR,  BOR,  {I c=d*n; DQ(m, *z++=1&&memchr(x,C1,n);                         x+=c;)}) 
REDUCECFX( andinsB, AND, IAND, SAND, BAND, {I c=d*n; DQ(m, *z++=!  memchr(x,C0,n);                         x+=c;}))
REDUCEBFX(  ltinsB, LT,  ILT,  SLT,  BLT,  {I c=d*n; DQ(m, *z++= *(x+n-1)&&!memchr(x,C1,n-1)?1:0;          x+=c;)})
REDUCEBFX(  leinsB, LE,  ILE,  SLE,  BLE,  {I c=d*n; DQ(m, *z++=!*(x+n-1)&&!memchr(x,C0,n-1)?0:1;          x+=c;)})
REDUCEBFX(  gtinsB, GT,  IGT,  SGT,  BGT,  {I c=d*n; DQ(m, y=memchr(x,C0,n); *z++=1&&(y?1&(y-x):1&n);      x+=c;)})
REDUCEBFX(  geinsB, GE,  IGE,  SGE,  BGE,  {I c=d*n; DQ(m, y=memchr(x,C1,n); *z++=!  (y?1&(y-x):1&n);      x+=c;)})
REDUCEBFX( norinsB, NOR, INOR, SNOR, BNOR, {I c=d*n; DQ(m, y=memchr(x,C1,n); d=y?y-x:n; *z++=(1&d)==d<n-1; x+=c;)})
REDUCEBFX(nandinsB, NAND,INAND,SNAND,BNAND,{I c=d*n; DQ(m, y=memchr(x,C0,n); d=y?y-x:n; *z++=(1&d)!=d<n-1; x+=c;)})
REDUCEPFX(plusinsB,I,B,PLUS, plusBB, plusBI)
REDUCEOVF( plusinsI, I, I,  PLUSR, PLUSVV, PLUSRV) 
REDUCEOVF(minusinsI, I, I, MINUSR,MINUSVV,MINUSRV) 
REDUCEOVF(tymesinsI, I, I, TYMESR,TYMESVV,TYMESRV)
REDUCCPFX( plusinsO, D, I,  PLUSO)
REDUCCPFX(minusinsO, D, I, MINUSO) 
REDUCCPFX(tymesinsO, D, I, TYMESO) 

 I plusinsD(I d,I n,I m,D* RESTRICTI x,D* RESTRICTI z,J jt){I i;D* RESTRICT y;
  NAN0;
  // latency of add is 4, so use 4 accumulators
  if(d==1){
   x += m*n; z+=m; DQ(m, D v0=0.0; D v1=0.0; if(((n+1)&3)==0)v1=*--x; D v2=0.0; if(n&2)v2=*--x; D v3=0.0; if(n&3)v3=*--x;
                       DQ(n>>2, v0=PLUS(*--x,v0); v1=PLUS(*--x,v1); v2=PLUS(*--x,v2); v3=PLUS(*--x,v3);); v0+=v1; v2+=v3;*--z=v0+v2;)
  }
  else{z+=(m-1)*d; x+=(m*n-1)*d;
   for(i=0;i<m;++i,z-=d){I rc;
    y=x; x-=d; if(255&(rc=plusDD(1,d,x,y,z,jt)))return rc; x-=d;
    DQ(n-2,    if(255&(rc=plusDD(1,d,x,z,z,jt)))return rc; x-=d; );
   }
  }
  return NANTEST?EVNAN:EVOK;
}

REDUCENAN( plusinsZ, Z, Z, zplus, plusZZ )
REDUCEPFX( plusinsX, X, X, xplus, plusXX, plusXX )

REDUCEPFX(minusinsB, I, B, MINUS, minusBB, minusBI ) 
REDUCENAN(minusinsD, D, D, MINUS, minusDD ) 
REDUCENAN(minusinsZ, Z, Z, zminus, minusZZ) 

REDUCEPFX(tymesinsD, D, D, TYMES, tymesDD, tymesDD ) 
REDUCEPFX(tymesinsZ, Z, Z, ztymes, tymesZZ, tymesZZ) 

REDUCENAN(  divinsD, D, D, DIV, divDD   )
REDUCENAN(  divinsZ, Z, Z, zdiv, divZZ  )

REDUCEPFXIDEM2(  maxinsI, I, I, MAX, maxII   )
REDUCEPFXIDEM2PRIM256(  maxinsD, D, D, MAX, maxDD, _mm256_max_pd, infm  )
REDUCEPFX(  maxinsX, X, X, XMAX, maxXX , maxXX )
REDUCEPFX(  maxinsS, SB,SB,SBMAX, maxSS, maxSS )

REDUCEPFXIDEM2(  mininsI, I, I, MIN, minII   )
REDUCEPFXIDEM2PRIM256(  mininsD, D, D, MIN, minDD, _mm256_min_pd, inf   )
REDUCEPFX(  mininsX, X, X, XMIN, minXX, minXX  )
REDUCEPFX(  mininsS, SB,SB,SBMIN, minSS, minSS )


static A jtred0(J jt,    A w,A self){DECLF;A x,z;I f,r,wr,*s;
 wr=AR(w); r=(RANKT)jt->ranks; r=wr<r?wr:r; f=wr-r; RESETRANK; s=AS(w);
 if((AT(w)&DENSE)!=0){GA(x,AT(w),0L,r,f+s);}else{GASPARSE(x,AT(w),1,r,f+s);}
 return reitem(vec(INT,f,s),lamin1(df1(z,x,(AT(w)&SBT)?idensb(fs):iden(fs))));
}    /* f/"r w identity case */

// general reduce.  We inplace the results into the next iteration.  This routine cannot inplace its inputs.
static A jtredg(J jt,    A w,A self){F1PREFIP;PROLOG(0020);DECLF;AD * RESTRICT a;I i,n,r,wr;
 ASSERT(DENSE&AT(w),EVNONCE);
 // loop over rank
 wr=AR(w); r=(RANKT)jt->ranks; r=wr<r?wr:r; RESETRANK;
 if(r<wr)return rank1ex(w,self,r,jtredg);
 // From here on we are doing a single reduction
 n=AS(w)[0]; // n=#cells
 // Allocate virtual block for the running x argument.
 fauxblock(virtafaux); fauxvirtual(a,virtafaux,w,r-1,ACUC1);  // allocate UNINCORPORABLE block
 // wfaux will hold the result from the iterations.  Init to value of last cell
 // Allocate fauxvirtual arg for the first cell, so it can be inplaceable/pristine if needed (tail returned a virtual block, which messed things up for high rank)
 fauxblock(virtwfaux); A wfaux; fauxvirtual(wfaux,virtwfaux,w,r-1,ACUC1);  // allocate UNINCORPORABLE block, mark inplaceable - used only once
   // finish filling the virt block
 A *old=jt->tnextpushp; // save stack mark for subsequent frees.  We keep the a argument over the calls, but allow the w to be deleted
 // fill in the shape, offset, and item-count of the virtual block
 I k; PROD(k,r-1,AS(w)+1);  // k=#atoms of cell of w
 AN(wfaux)=k; AN(a)=k;
 k<<=bplg(AT(w)); // k now=length of input cell in bytes, where it will remain
 AK(wfaux)+=(n-1)*k; AK(a)+=(n-2)*k; MCISH(AS(wfaux),AS(w)+1,r-1); MCISH(AS(a),AS(w)+1,r-1);  // make the virtual block look like the tail, except for the offset
 // Calculate inplaceability.  We can inplace the left arg, which is always virtual, if w is inplaceable and (w is direct or fs is &.>)
 // We include contextual inplaceability (from jtinplace) here because if the block is returned, its pristinity will be checked if it is inplaceable.  Thus
 // we do not want to call a faux argument inplaceable if it really isn't.  This gives us leeway with jtinplace itself
 I aipok = (SGNIF((I)jtinplace&(((AT(w)&TYPEVIPOK)!=0)|f2==jtevery2self),JTINPLACEWX)&AC(w))+ACUC1;   // requires JTINPLACEWX==0.  This is 1 or 8..1
 // We can inplace the right arg the first time if it is direct inplaceable, and always after that (assuming it is an inplaceable result).
 // and the input jtinplace.  We turn off WILLBEOPENED status in jtinplace for the callee.
 AC(wfaux)=aipok;   // first cell is inplaceable if second is
 jtinplace = (J)(intptr_t)(((I)jt) + (JTINPLACEW+JTINPLACEA)*((FAV(fs)->flag>>(VJTFLGOK2X-JTINPLACEWX)) & JTINPLACEW));  // all items are used only once

 // We need to free memory in case the called routine leaves it unfreed (that's bad form & we shouldn't expect it), and also to free the result of the
 // previous iteration.  We don't want to free every time, though, because that does ra() on w which could be a costly traversal if it's a nonrecusive recursible type.
 // As a compromise we free every few iterations: at least one per 8 iterations, and at least 8 times through the process
#define LGMINGCS 3  // lg2 of minimum number of times we call gc
#define MINGCINTERVAL 8  // max spacing between frees
 I freedist=MIN((n+((1<<LGMINGCS)-1))>>LGMINGCS,MINGCINTERVAL); I freephase=freedist;
 i=n-1; while(1){  // for each cell except the last
  AC(a)=aipok;   // in case we created a virtual block from it, restore inplaceability to the UNINCORPABLE block
  RZ(wfaux=CALL2IP(f2,a,wfaux,fs));
  if(--i==0)break;   // stop housekeeping after last iteration
  // if w happens to be the same virtual block that we passed in as x, we have to clone it before we change the pointer
  if(a==wfaux){RZ(wfaux=virtual(wfaux,0,AR(a))); AN(wfaux)=AN(a); MCISH(AS(wfaux),AS(a),AR(a));}
  if(--freephase==0){wfaux=jtgc(jt,wfaux,old); freephase=freedist;}   // free the buffers we allocated, except for the result
  // move to next input cell
  AK(a) -= k;
 }
 // At the end of all this, it is possible that the result is the original first or last cell, resting in its original virtual block.
 // In that case, we have to realize it, so that we don't let the fauxvirtual block escape
 if((AFLAG(wfaux)&AFUNINCORPABLE)!=0)wfaux=realize(wfaux);
 EPILOG(wfaux);  // this frees the virtual block, at the least
}    /* f/"r w for general f and 1<(-r){$w */


static const C fca[]={CSTAR, CPLUS, CEQ, CMIN, CMAX, CPLUSDOT, CSTARDOT, CNE, 0};  /* commutative & associative */

static A jtredsp1a(J jt,C id,A z,A e,I n,I r,I*s){A t;B b,p=0;D d=1;
 switch(id){
  default:       ASSERT(0,EVNONCE);
  case CPLUSDOT: return n?gcd(z,e):ca(e);
  case CSTARDOT: return n?lcm(z,e):ca(e);
  case CMIN:     return n?minimum(z,e):ca(e);
  case CMAX:     return n?maximum(z,e):ca(e);
  case CPLUS:    if(n&&equ(e,num(0)))return z; DO(r, d*=s[i];); t=tymes(e,d>=FLIMAX?scf(d-n):sc((I)d-n)); return n?plus(z,t):t;
  case CSTAR:    if(n&&equ(e,num(1) ))return z; DO(r, d*=s[i];); t=expn2(e,d>=FLIMAX?scf(d-n):sc((I)d-n)); return n?tymes(z,t):t;
  case CEQ:      p=1;  /* fall thru */
  case CNE:
   ASSERT(B01&AT(e),EVNONCE); 
   if(!n)BAV(z)[0]=p; 
   b=1; DO(r, if(!(s[i]&1)){b=0; break;}); 
   return !p==*BAV(e)&&b!=(n&1)?__not(z):z;
}}   /* f/w on sparse vector w, post processing */

static A jtredsp1(J jt,A w,A self,C id,VARPSF ado,I cv,I f,I r,I zt){A e,x,z;I m,n;P*wp;
 wp=PAV(w); e=SPA(wp,e); x=SPA(wp,x); n=AN(x); m=*AS(w);
 GA(z,zt,1,0,0);
 if(n){I rc=((AHDRRFN*)ado)(1L,n,1L,AV(x),AV(z),jt); if(255&rc)jsignal(rc); RE(0); if(m==n)return z;}
 return redsp1a(id,z,e,n,AR(w),AS(w));
}    /* f/"r w for sparse vector w */

 A jtredravel(J jt,    A w,A self){A f,x,z;I n;P*wp;
 F1PREFIP;
 f=FAV(self)->fgh[0];  // f/
 if(!(SPARSE&AT(w)))return reduce(jtravel(jtinplace,w),f);
 // The rest is sparse
 wp=PAV(w); x=SPA(wp,x); n=AN(x);
 I rc=EVOK;
 while(1){  // Loop to handle restart on overflow
  VARPS adocv; varps(adocv,f,AT(x),0);
  ASSERT(adocv.f,EVNONCE);
  GA(z,rtype(adocv.cv),1,0,0);
  if(n)rc=((AHDRRFN*)adocv.f)((I)1,n,(I)1,AV(x),AV(z),jt);  // mustn't adocv on empty
  rc&=255; if(rc)jsignal(rc); if(rc<EWOV){if(rc)return 0; return redsp1a(FAV(FAV(f)->fgh[0])->id,z,SPA(wp,e),n,AR(w),AS(w));}  // since f has an insert fn, its id must be OK
 }
}  /* f/@, w */

static A jtredspd(J jt,A w,A self,C id,VARPSF ado,I cv,I f,I r,I zt){A a,e,x,z,zx;I c,m,n,*s,t,*v,wr,*ws,xf,xr;P*wp,*zp;
 ASSERT(strchr(fca,id),EVNONCE);
 wp=PAV(w); a=SPA(wp,a); e=SPA(wp,e); x=SPA(wp,x); s=AS(x);
 xr=r; v=AV(a); DO(AN(a), if(f<v[i])--xr;); xf=AR(x)-xr;
 m=prod(xf,s); c=m?AN(x)/m:0; n=s[xf];
 GA(zx,zt,AN(x)/n,AR(x)-1,s); MCISH(xf+AS(zx),1+xf+s,xr-1); 
 I rc=((AHDRRFN*)ado)(c/n,n,m,AV(x),AV(zx),jt); if(255&rc)jsignal(rc); RE(0);
 switch(id){
  case CPLUS: if(!equ(e,num(0)))RZ(e=tymes(e,sc(n))); break; 
  case CSTAR: if(!equ(e,num(1) )&&!equ(e,num(0)))RZ(e=expn2(e,sc(n))); break;
  case CEQ:   ASSERT(B01&AT(x),EVNONCE); if(!BAV(e)[0]&&0==(n&1))e=num(1); break;
  case CNE:   ASSERT(B01&AT(x),EVNONCE); if( BAV(e)[0]&&1==(n&1))e=num(0);
 }
 if(TYPESNE(AT(e),AT(zx))){t=jtmaxtype(jt,AT(e),AT(zx)); if(TYPESNE(t,AT(zx)))RZ(zx=jtcvt(jt,t,zx));}
 wr=AR(w); ws=AS(w);
 GASPARSE(z,STYPE(AT(zx)),1,wr-1,ws); if(1<wr)MCISH(f+AS(z),f+1+ws,wr-1);
 zp=PAV(z);
 RZ(a=ca(a)); v=AV(a); DO(AN(a), if(f<v[i])--v[i];);
 SPB(zp,a,a);
 SPB(zp,e,jtcvt(jt,AT(zx),e));
 SPB(zp,i,SPA(wp,i));
 SPB(zp,x,zx);
 return z;
}    /* f/"r w for sparse w, rank > 1, dense axis */

static B jtredspsprep(J jt,C id,I f,I zt,A a,A e,A x,A y,I*zm,I**zdv,B**zpv,I**zqv,C**zxxv,A*zsn){
     A d,p,q,sn=0,xx;B*pv;C*xxv;I*dv,j,k,m,mm,*qv=0,*u,*v,yc,yr,yr1,*yv;
 v=AS(y); yr=v[0]; yc=v[1]; yr1=yr-1;
 RZ(d=grade1(eq(a,sc(f)))); dv=AV(d); 
 DO(AN(a), if(i!=dv[i]){RZ(q=jtgrade1p(jt,d,y)); qv=AV(q); break;});
 GATV0(p,B01,yr,1); pv=BAV(p); memset(pv,C0,yr);
 u=yv=AV(y); m=mm=0; j=-1; if(qv)v=yv+yc*qv[0];
 for(k=0;k<yr1;++k){
  if(qv){u=v; v=yv+yc*qv[1+k];}else v=u+yc;
  DO(yc-1, if(u[dv[i]]!=v[dv[i]]){++m; pv[k]=1; mm=MAX(mm,k-j); j=k; break;});
  if(!qv)u+=yc;
 }
 if(yr){++m; pv[yr1]=1; mm=MAX(mm,yr1-j);}
 if(qv){j=mm*aii(x); GA(xx,AT(x),j,1,0); xxv=CAV(xx);}
 switch(id){
  case CPLUS: case CPLUSDOT: j=!equ(e,num(0)); break;
  case CSTAR: case CSTARDOT: j=!equ(e,num(1));  break;
  case CMIN:                 j=!equ(e,zt&B01?num(1) :zt&INT?sc(IMAX):ainf     ); break;
  case CMAX:                 j=!equ(e,zt&B01?num(0):zt&INT?sc(IMIN):scf(infm)); break;
  case CEQ:                  j=!*BAV(e);     break;
  case CNE:                  j= *BAV(e);     break;
 }
 if(j)GATV0(sn,INT,m,1);
 *zm=m; *zdv=dv; *zpv=pv; *zqv=qv; *zxxv=xxv; *zsn=sn;
 return 1;
}

static B jtredspse(J jt,C id,I wm,I xt,A e,A zx,A sn,A*ze,A*zzx){A b;B nz;I t,zt;
 RZ(b=ne(num(0),sn)); nz=!all0(b); zt=AT(zx);
 switch(id){
  case CPLUS:    if(nz)RZ(zx=plus (zx,       tymes(e,sn) )); RZ(e=       tymes(e,sc(wm)) ); break; 
  case CSTAR:    if(nz)RZ(zx=tymes(zx,jtbcvt(jt,1,expn2(e,sn)))); RZ(e=jtbcvt(jt,1,expn2(e,sc(wm)))); break;
  case CPLUSDOT: if(nz)RZ(zx=gcd(zx,jtfrom(jt,b,over(num(0),e))));                 break;
  case CSTARDOT: if(nz)RZ(zx=lcm(zx,jtfrom(jt,b,over(num(1),e))));                 break;
  case CMIN:     if(nz)RZ(zx=minimum(zx,jtfrom(jt,b,over(zt&B01?num(1): zt&INT?sc(IMAX):ainf,     e)))); break;
  case CMAX:     if(nz)RZ(zx=maximum(zx,jtfrom(jt,b,over(zt&B01?num(0):zt&INT?sc(IMIN):scf(infm),e)))); break;
  case CEQ:      ASSERT(B01&xt,EVNONCE); if(nz)RZ(zx=eq(zx,eq(num(0),residue(num(2),sn)))); if(!(wm&1))e=num(1);  break;
  case CNE:      ASSERT(B01&xt,EVNONCE); if(nz)RZ(zx=ne(zx,eq(num(1), residue(num(2),sn)))); if(!(wm&1))e=num(0); break;
 }
 if(TYPESNE(AT(e),AT(zx))){t=jtmaxtype(jt,AT(e),AT(zx)); if(TYPESNE(t,AT(zx)))RZ(zx=jtcvt(jt,t,zx));}
 *ze=e; *zzx=zx;
 return 1;
}

static A jtredsps(J jt,A w,A self,C id,VARPSF ado,I cv,I f,I r,I zt){A a,a1,e,sn,x,x1=0,y,z,zx,zy;B*pv;
     C*xv,*xxv,*zv;I*dv,i,m,n,*qv,*sv,*v,wr,xk,xt,wm,*ws,xc,yc,yr,*yu,*yv,zk;P*wp,*zp;
 ASSERT(strchr(fca,id),EVNONCE);
 wr=AR(w); ws=AS(w); wm=ws[f];
 wp=PAV(w); a=SPA(wp,a); e=SPA(wp,e); 
 y=SPA(wp,i); v=AS(y); yr=v[0]; yc=v[1]; yv=AV(y);
 x=SPA(wp,x); xt=AT(x); xc=aii(x);
 RZ(redspsprep(id,f,zt,a,e,x,y,&m,&dv,&pv,&qv,&xxv,&sn));
 xv=CAV(x); xk=xc<<bplg(xt);
 GA(zx,zt,m*xc,AR(x),AS(x)); AS(zx)[0]=m; zv=CAV(zx); zk=xc<<bplg(zt);
 GATV0(zy,INT,m*(yc-1),2); v=AS(zy); v[0]=m; v[1]=yc-1; yu=AV(zy);
 v=qv; if(sn)sv=AV(sn);
 for(i=0;i<m;++i){A y;B*p1;C*u;I*vv;
  p1=1+(B*)memchr(pv,C1,yr); n=p1-pv; if(sn)sv[i]=wm-n; pv=p1;
  vv=qv?yv+yc**v:yv; DO(yc-1, *yu++=vv[dv[i]];);
  if(1<n){if(qv){u=xxv; DO(n, MC(u,xv+xk*v[i],xk); u+=xk;);} I rc=((AHDRRFN*)ado)(xc,n,1L,qv?xxv:xv,zv,jt); if(255&rc)jsignal(rc); RE(0);}
  else   if(zk==xk)MC(zv,qv?xv+xk**v:xv,xk);
  else   {if(!x1)GA(x1,xt,xc,1,0); MC(AV(x1),qv?xv+xk**v:xv,xk); RZ(y=jtcvt(jt,zt,x1)); MC(zv,AV(y),zk);}
  zv+=zk; if(qv)v+=n; else{xv+=n*xk; yv+=n*yc;}
 }
 if(sn)RZ(redspse(id,wm,xt,e,zx,sn,&e,&zx));
 RZ(a1=ca(a)); v=AV(a1); n=0; DO(AN(a), if(f!=v[i])v[n++]=v[i]-(I )(f<v[i]););
 GASPARSE(z,STYPE(AT(zx)),1,wr-1,ws); if(1<r)MCISH(f+AS(z),f+1+ws,r-1);
 zp=PAV(z);
 SPB(zp,a,vec(INT,n,v)); 
 SPB(zp,e,jtcvt(jt,AT(zx),e));
 SPB(zp,x,zx); 
 SPB(zp,i,zy);
 return z;
}    /* f/"r w for sparse w, rank > 1, sparse axis */

static A jtreducesp(J jt,    A w,A self){A a,g,z;B b;I f,n,r,*v,wn,wr,*ws,wt,zt;P*wp;
J jtinplace=jt;
 wr=AR(w); r=(RANKT)jt->ranks; r=wr<r?wr:r; f=wr-r;  // no RESETRANK
 wn=AN(w); ws=AS(w); n=r?ws[f]:1;
 wt=AT(w); wt=wn?DTYPE(wt):B01;
 g=VAV(self)->fgh[0];  // g is the f in f/
 if(!n)return red0(w,self);  // red0 uses ranks, and resets them
 C id; if(AT(g)&VERB){id=FAV(g)->id; id=FAV(g)->flag&VISATOMIC2?id:0;}else id=0;
 VARPS adocv; varps(adocv,self,wt,0);
 if(2==n&&!(adocv.f&&strchr(fca,id))){
  A x; IRS2(num(0),w,0L,0,r,jtfrom,x); A y; IRS2(num(1),w,0L,0,r,jtfrom,y);
  return df2(z,x,y,g);  // rank has been reset for this call
 }
 // original rank still set
 if(!adocv.f)return redg(w,self);
 if(1==n)return tail(w);
 zt=rtype(adocv.cv);
 RESETRANK;
 if(1==wr)z=redsp1(w,self,id,adocv.f,adocv.cv,f,r,zt);
 else{
  wp=PAV(w); a=SPA(wp,a); v=AV(a);
  b=0; DO(AN(a), if(f==v[i]){b=1; break;});
  z=b?redsps(w,self,id,adocv.f,adocv.cv,f,r,zt):redspd(w,self,id,adocv.f,adocv.cv,f,r,zt);
 }
 return jt->jerr>=EWOV?IRS1(w,self,r,jtreducesp,z):z;
}    /* f/"r for sparse w */

static A jtreduce(J jt,    A w,A self){A z;I d,f,m,n,r,t,wr,*ws,zt;
 F1PREFIP;
 if((SPARSE&AT(w))!=0)return reducesp(w,self);  // If sparse, go handle it
 wr=AR(w); ws=AS(w);
 // Create  r: the effective rank; f: length of frame; n: # items in a CELL of w
 r=(RANKT)jt->ranks; r=wr<r?wr:r; f=wr-r; SETICFR(w,f,r,n);  // no RESETRANK
 // Handle the special cases: neutrals, single items
 if(n>1){
  // Normal case, >1 item.
  // The case of empty w is interesting, because the #cells, and the #atoms in an item of a cell, may both overflow if
  // n=0.  But we have handled that case above.  If n is not 0, there may be other zeros in the shape that allow
  // an overflow when an infix of the shape is multiplied; but that won't matter because the other 0 will guarantee that there
  // are no atoms written
  I wt=AT(w); wt=AN(w)?wt:B01;   // Treat empty as Boolean type

  // Normal processing for multiple items.  Get the routine & flags to process it
  VARPS adocv; varps(adocv,self,wt,0);
  // If there is no special routine, go perform general reduce
  if(!adocv.f)return redg(w,self);  // jt->ranks is still set.  redg will clear the ranks
  // Here for primitive reduce handled by special code.
  // Calculate m: #cells of w to operate on; d: #atoms in an item of a cell of w cell of w (a cell to which u is applied);
  // zn: #atoms in result
  PROD(d,r-1,f+ws+1);  //  */ }. $ cell
  // m=*/ frame (i. e. #cells to operate on)
  // r cannot be 0 (would be handled above).  Calculate low part of zn first
  PROD(m,f,ws);
  RESETRANK;   // clear rank now that we've used it - not really required here?
  // Allocate the result area
  zt=rtype(adocv.cv);
  GA(z,zt,m*d,MAX(0,wr-1),ws); if(1<r)MCISH(f+AS(z),f+1+ws,r-1);  // allocate, and install shape
  if(m*d==0){return z;}  // mustn't call the function on an empty argument!
  // Convert inputs if needed 
  if((t=atype(adocv.cv))&&TYPESNE(t,wt))RZ(w=jtcvt(jt,t,w));
  // call the selected reduce routine.
  I rc=((AHDRRFN*)adocv.f)(d,n,m,AV(w),AV(z),jt);
  // if return is EWOV, it's an integer overflow and we must restart, after restoring the ranks
  // EWOV1 means that there was an overflow on a single result, which was calculated accurately and stored as a D.  So in that case all we
  // have to do is change the type of the result.
  if(255&rc){if(jt->jerr==EWOV1){AT(z)=FL;return z;}else {jsignal(rc); return rc>=EWOV?IRS1(w,self,r,jtreduce,z):0;}} else {return adocv.cv&VRI+VRD?jtcvz(jt,adocv.cv,z):z;}

  // special cases:
 }else if(n==1)return head(w);    // reduce on single items - ranks are still set
 else return red0(w,self);    // 0 items - neutrals - ranks are still set



}    /* f/"r w main control */

static A jtredcatsp(J jt,A w,A z,I r){A a,q,x,y;B*b;I c,d,e,f,j,k,m,n,n1,p,*u,*v,wr,*ws,xr;P*wp,*zp;
 ws=AS(w); wr=AR(w); f=wr-r; p=ws[1+f];
 wp=PAV(w); x=SPA(wp,x); y=SPA(wp,i); a=SPA(wp,a); v=AV(a); 
 m=AS(y)[0]; n=AN(a); n1=n-1; xr=AR(x);
 RZ(b=bfi(wr,a,1));
 c=b[f]; d=b[1+f]; if(c&&d)b[f]=0; e=f+!c;
 j=0; DO(n, if(e==v[i]){j=i; break;}); 
 k=1; DO(f, if(!b[i])++k;);
 zp=PAV(z); SPB(zp,e,ca(SPA(wp,e)));
 GATV0(q,INT,n-(I )(c&&d),1); v=AV(q); DO(wr, if(b[i])*v++=i-(I )(i>f);); SPB(zp,a,q);
 if(c&&d){          /* sparse sparse */
  SPB(zp,x,ca(x));
  SPB(zp,i,repeatr(ne(a,sc(f)),y)); q=SPA(zp,i);  // allow for virtualization of SPB
  v=j+AV(q); u=j+AV(y);
  DQ(m, *v=p*u[0]+u[1]; v+=n1; u+=n;);
 }else if(!c&&!d){  /* dense dense */
  u=AS(x); GA(q,AT(x),AN(x),xr-1,u); v=k+AS(q); *v=u[k]*u[1+k]; MCISH(1+v,2+k+u,xr-k-2);
  MC(AV(q),AV(x),AN(x)<<bplg(AT(x)));
  SPB(zp,x,q); SPB(zp,i,ca(y));
 }else{             /* other */
  GATV0(q,INT,xr,1); v=AV(q); 
  if(1!=k){*v++=0; *v++=k; e=0; DQ(xr-1, ++e; if(e!=k)*v++=e;); RZ(x=jtcant2(jt,q,x));}
  v=AV(q); u=AS(x); *v=u[0]*u[1]; MCISH(1+v,2+u,xr-1); RZ(x=reshape(vec(INT,xr-1,v),x));
  e=ws[f+c]; RZ(y=repeat(sc(e),y)); RZ(y=mkwris(y)); v=j+AV(y);
  if(c)DO(m, k=p**v; DO(e, *v=k+  i; v+=n;);)
  else DO(m, k=  *v; DO(e, *v=k+p*i; v+=n;);); 
  RZ(q=grade1(y)); RZ(y=jtfrom(jt,q,y)); RZ(x=jtfrom(jt,q,x));
  SPB(zp,x,x); SPB(zp,i,y);
 }
 return z;
}    /* ,/"r w for sparse w, 2<r */

// ,&.:(<"r)  run together all axes above the last r.  r must not exceed AR(w)-1
// w must not be sparse or empty
A jtredcatcell(J jt,A w,I r){A z;
 F1PREFIP;
 I wr=AR(w);  // get original rank, which may change if we inplace into the same block
 if(r>=wr-1)return RETARG(w);  // if only 1 axis left to run together, return the input
 if((ASGNINPLACESGN(SGNIF((I)jtinplace,JTINPLACEWX)&(-r),w) && !(AFLAG(w)&AFUNINCORPABLE))){  // inplace allowed, usecount is right
  // operation is loosely inplaceable.  Just shorten the shape to frame,(#atoms in cell).  We do this here rather than relying on
  // the self-virtual-block code in virtual() because we can do it for indirect types also, since we know we are not changing
  // the number of atoms
  z=w; AR(z)=(RANKT)(r+1);  // inplace it, install new rank
 }else{
  RZ(z=virtual(w,0,r+1)); AN(z)=AN(w);   // create virtual block
 }
 I m; PROD(m,wr-r,AS(w));   // # cells being strung together
 AS(z)[0]=m; MCISH(AS(z)+1,AS(w)+wr-r,r);  // install # cells and then shift the last r axes back 
 return z;
}


 A jtredcat(J jt,    A w,A self){A z;B b;I f,r,*s,*v,wr;
 F1PREFIP;
 wr=AR(w); r=(RANKT)jt->ranks; r=wr<r?wr:r; f=wr-r; s=AS(w); RESETRANK;
 b=1==r&&1==s[f];  // special case: ,/ on last axis which has length 1: in that case, the rules say the axis disappears (because of the way ,/ works on length-1 lists)
 if(2>r&&!b)return w;  // in all OTHER cases, result=input for ranks<2
 // use virtual block (possibly self-virtual) for all cases except sparse
 if(!(SPARSE&AT(w))){
  RZ(z=jtvirtual(jtinplace,w,0,wr-1)); AN(z)=AN(w); // Allocate the block.  Then move in AN and shape
  I *zs=AS(z); MCISH(zs,s,f); if(!b){DPMULDE(s[f],s[f+1],zs[f]); MCISH(zs+f+1,s+f+2,r-2);}
  return z;
 }else{
  GASPARSE(z,AT(w),AN(w),wr-1,s); 
  if(!b){v=f+AS(z); DPMULDE(s[f],s[1+f],*v); MCISH(1+v,2+f+s,r-2);}
  return redcatsp(w,z,r);
 }
}    /* ,/"r w */

static A jtredsemi(J jt,    A w,A self){I f,n,r,*s,wr;
 wr=AR(w); r=(RANKT)jt->ranks; r=wr<r?wr:r; f=wr-r; s=AS(w); SETICFR(w,f,r,n);   // let the rank run into tail   n=#items  in a cell of w
 if(2>n){ASSERT(n!=0,EVDOMAIN); return tail(w);}  // rank still set
 if(BOX&AT(w))return jtredg(jt,w,self);  // the old way failed because it did not mimic scalar replication; revert to the long way.  ranks are still set
 else{A z; return IRS1(w,0L,r-1,jtbox,z);}  // unboxed, just box the cells
}    /* ;/"r w */

static A jtredstitch(J jt,    A w,A self){A c,y;I f,n,r,*s,*v,wr;
 wr=AR(w); r=(RANKT)jt->ranks; r=wr<r?wr:r; f=wr-r; RESETRANK;
 s=AS(w); SETICFR(w,f,r,n);
 ASSERT(n!=0,EVDOMAIN);
 if(1==n)return IRS1(w,0L,r,jthead,y);
 if(1==r){if(2==n)return RETARG(w); A z1,z2,z3; RZ(IRS2(num(-2),w,0L,0L,1L,jtdrop,z1)); RZ(IRS2(num(-2),w,0L,0L,1L,jttake,z2)); return IRS2(z1,z2,0L,1L,0L,jtover,z3);}
 if(2==r)return IRS1(w,0L,2L,jtcant1,y);
 RZ(c=apvwr(wr,0L,1L)); v=AV(c); v[f]=f+1; v[f+1]=f; RZ(y=jtcant2(jt,c,w));  // transpose last 2 axes
 if((SPARSE&AT(w))!=0){A x;
  GATV0(x,INT,f+r-1,1); v=AV(x); MCISH(v,AS(y),f+1);
  DPMULDE(s[f],s[f+2],v[f+1]); MCISH(v+f+2,s+3+f,r-3);
  return reshape(x,y);
 }else{
  v=AS(y); 
  DPMULDE(s[f],s[f+2],v[f+1]); MCISH(v+f+2,s+3+f,r-3);
  --AR(y); 
  return y;
}}   /* ,./"r w */

static A jtredstiteach(J jt,    A w,A self){A*wv,y;I n,p,r,t;
 n=AN(w);
 if(!(2<n&&1==AR(w)&&BOX&AT(w)))return reduce(w,self);
 wv=AAV(w);  y=wv[0]; SETIC(y,p); t=AT(y);
 DO(n, y=wv[i]; r=AR(y); if(!((((r-1)&-2)==0)&&p==SETIC(y,n)&&TYPESEQ(t,AT(y))))return reduce(w,self););  // rank 1 or 2, rows match, equal types
 return box(razeh(w));
}    /* ,.&.>/ w */

static A jtredcateach(J jt,    A w,A self){A*u,*v,*wv,x,*xv,z,*zv;I f,m,mn,n,r,wr,*ws,zm,zn;I n1=0,n2=0;
 wr=AR(w); ws=AS(w); r=(RANKT)jt->ranks; r=wr<r?wr:r; f=wr-r; RESETRANK;
 SETICFR(w,f,r,n);
 if(!r||1>=n)return reshape(repeat(ne(sc(f),IX(wr)),shape(w)),n?w:ds(CACE));
 if(!(BOX&AT(w)))return df1(z,jtcant2(jt,sc(f),w),qq(ds(CBOX),zeroionei(1)));  // handle unboxed args
// bug: ,&.>/ y does scalar replication wrong
// wv=AN(w)+AAV(w); DQ(AN(w), if(AN(*--wv)&&AR(*wv)&&n1&&n2) ASSERT(0,EVNONCE); if((!AR(*wv))&&n1)n2=1; if(AN(*wv)&&1<AR(*wv))n1=1;);
 zn=AN(w)/n; PROD(zm,f,ws); PROD(m,r-1,ws+f+1); mn=m*n;
 GATV(z,BOX,zn,wr-1,ws); MCISH(AS(z)+f,ws+f+1,r-1);
 GATV0(x,BOX,n,1); xv=AAV(x);
 zv=AAV(z); wv=AAV(w); 
 DO(zm, u=wv; DO(m, v=u++; DO(n, xv[i]=*v; v+=m;); A Zz; RZ(Zz=raze(x)); INCORP(Zz); *zv++ = Zz;); wv+=mn;);  // no need to incorp *v since it's already boxed
 return z;
}    /* ,&.>/"r w */

static A jtoprod(J jt,A a,A w,A self){A z; return df2(z,a,w,FAV(self)->fgh[2]);}  // x u/ y - transfer to the u"lr,_ verb (precalculated)


 A jtslash(J jt, A w){A h;AF f1;C c;V*v;I flag=0;
 if(NOUN&AT(w))return jtevger(jt,w,sc(GINSERT));  // treat m/ as m;.6.  This means that a node with CSLASH never contains gerund u
 v=FAV(w); 
 switch(v->id){  // select the monadic case
  case CCOMMA:  f1=jtredcat; flag=VJTFLGOK1;   break;
  case CCOMDOT: f1=jtredstitch; flag=0; break;
  case CSEMICO: f1=jtredsemi; flag=0; break;
  case CUNDER:  f1=jtreduce; if(COPE==ID(v->fgh[1])){c=ID(v->fgh[0]); if(c==CCOMMA)f1=jtredcateach; else if(c==CCOMDOT)f1=jtredstiteach;} flag=0; break;
  default: f1=jtreduce; flag=(v->flag&VJTFLGOK2)>>(VJTFLGOK2X-VJTFLGOK1X); break;  // monad is inplaceable if the dyad for u is
 }
 RZ(h=qq(w,v2(lr(w),RMAX)));  // create the rank compound to use if dyad
 RZ(h=fdef(0,CSLASH,VERB, f1,jtoprod, w,0L,h, flag|FAV(ds(CSLASH))->flag, RMAX,RMAX,RMAX));
 // set lvp[1] to point to the VARPSA block for w if w is atomic dyad; otherwise to the null VARPSA block
 FAV(h)->localuse.lvp[1]=v->flag&VISATOMIC2?((VA*)v->localuse.lvp[0])->rps:&rpsnull;
 return h;
}

A jtaslash (J jt,C c,    A w){RZ(   w); A z; return df1(z,  w,   slash(ds(c))     );}
A jtaslash1(J jt,C c,    A w){RZ(   w); A z; return df1(z,  w,qq(slash(ds(c)),zeroionei(1)));}
A jtatab   (J jt,C c,A a,A w){ A z; return df2(z,a,w,   slash(ds(c))     );}

 A jtmean(J jt,    A w,A self){
 I wr=AR(w); I r=(RANKT)jt->ranks; r=wr<r?wr:r;
 I n=AS(w)[wr-r]; n=r?n:1;
 // leave jt->ranks unchanged to pass into +/
A sum=reduce(w,FAV(self)->fgh[0]);  // calculate +/"r
 RESETRANK;  // back to infinite rank for the divide
 RZ(sum);
 RZ(w=jtatomic2(JTIPA,sum,sc(n),ds(CDIV)));  // take quotient inplace and return it
 return w;
}    // (+/%#)"r w, implemented as +/"r % cell-length

// entry point to execute monad/dyad Fold after the noun arguments are supplied
static A jtfoldx(J jt,A a,A w,A self){F2PREFIP;  // this stands in place of jtxdefn, which inplaces
 // see if this is monad or dyad
 I foldflag=((~AT(w))>>(VERBX-3))&8;  // flags: dyad mult fwd rev  if w is not conj, this must be a dyad call
 self=foldflag?self:w; w=foldflag?w:a; a=foldflag?a:mtv; // if monad, it's w self garbage,  move to '' w self
 // get the rest of the flags from the original ID byte, which was moved to lc
 foldflag|=FAV(self)->lc-CFDOT;  // this sets mult fwd rev
 // define the flags as the special global
 RZ(symbis(nfs(11,"Foldtype_j_"),sc(foldflag),jt->locsyms));
 // execute the Fold.  While it is running, set the flag to allow Z:
 B foldrunning=jt->foldrunning; jt->foldrunning=1; A z=(*(FAV(self)->localuse.lfns[1]))(jt,a,w,self); jt->foldrunning=foldrunning;
 // if there was an error, save the error code and recreate the error at this level, to cover up details inside the script
 if(jt->jerr){I e=jt->jerr; RESETERR; jsignal(e);}
 return z;
}

// entry point for monad and dyad F. F.. F.: F: F:. F::
 A jtfold(J jt,A a,A w,A self){
 // The name Fold_j_ should have been loaded at startup.  If not, try loading its script.  If that still fails, quit
 A foldconj; I step;
 for(step=0;step<2;++step){
  switch(step){  // try the startup, from the bottom up
  case 1: eval("load'~addons/dev/fold/foldr.ijs'");  // fall through
  case 0: if((foldconj=jtnameref(jt,nfs(8,"Foldr_j_"),jt->locsyms))&&AT(foldconj)&CONJ)goto found;  // there is always a ref, but it may be to [:
  }
  RESETERR;  // if we loop back, clear errors
 }
 ASSERT(0,EVNONCE);  // not found or not conjunction - error
found: ;

 // Apply Fold_j_ to the input arguments, creating a derived verb to do the work
 A derivvb; RZ(derivvb=unquote(a,w,foldconj));
 // Modify the derived verb to go to our preparatory stub.  Save the dyadic entry point for the derived verb so the stub can call it
 FAV(derivvb)->localuse.lfns[1]=FAV(derivvb)->valencefns[1];
 FAV(derivvb)->valencefns[0]=FAV(derivvb)->valencefns[1]=jtfoldx;
 // Tell the stub what the original fold type was
 FAV(derivvb)->lc=FAV(self)->id;
 return derivvb;
}

// x Z: y
 A jtfoldZ(J jt,A a,A w,A self){
 ASSERT(jt->foldrunning,EVSYNTAX);  // If fold not running, fail.  Should be a semantic error rather than syntax
 // The name FoldZ_j_ should have been loaded at startup.  If not, fail
 A foldvb; RZ(foldvb=jtnameref(jt,nfs(8,"FoldZ_j_"),jt->locsyms)); ASSERT((AT(foldvb)&VERB),EVNONCE);   // error if undefined or not verb
 // Apply FoldZ_j_ to the input arguments, creating a derived verb to do the work
 A z=unquote(a,w,foldvb);
 // if there was an error, save the error code and recreate the error at this level, to cover up details inside the script
 if(jt->jerr){I e=jt->jerr; RESETERR; jsignal(e);}
 return z;
}
