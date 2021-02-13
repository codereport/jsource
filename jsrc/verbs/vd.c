/* Copyright 1990-2004, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Verbs: Domino                                                           */

#include "j.h"


static A jtnorm(J jt, A w){return sqroot(jtpdt(jt,w,conjug(w)));}

// take inverse of upper-triangular w.  We ASSUME w is inplaceable
// n is the size of the nxn matrix w; ncomp codes for special processing
// if n<=ncomp, this is a small FL matrix & we take the inverse inplace
static A jtrinvip(J jt,A w,I n,I ncomp){PROLOG(0066);A ai,bx,di,z;I m;
 if(n<=ncomp){
  // Handle 2x2 and smaller FL quickly and inplace to avoid recursion and memory-allocation overhead
  // result is 1/w00 w01/(w00*w11)
  //             0     1/w11     for 1x1, only the top-left
  D w00r, w11r;
  w00r=DAV2(w)[0]=1.0/DAV2(w)[0];  // 1/w00
  if(n>1){
   w11r=DAV2(w)[3]=1.0/DAV2(w)[3];  // 1/w11
   DAV2(w)[1]*=-w00r*w11r;  //  w01/(w00*w11)
   // w10 is already 0
  }
  return w;
  // Don't bother marking so small a matrix as uppertri
 }
 // fall through for other types & shapes
 if(1>=n)return jtrecip(jt,w);  // if an atom, inverse = reciprocal.  Must be CMPX or RAT
 m=n>>1; I tom=(0x01222100>>((n&7)<<2))&3; m=(m+tom<n)?m+tom:m;  // Minimize number of wasted multiply slots, processing in batches of 4
 // construe w as a block-matrix Wij where w00 and w11 are upper-triangular, w10 is 0, and w01 is a full matrix
 ai=jtrinvip(jt,jttake(jt,jtv2(jt,m,m),w),m,ncomp);  // take inverse of w00  kludge could use faux block to avoid take overhead esp for 2x2 FL results
 di=jtrinvip(jt,jtdrop(jt,jtv2(jt,m,m),w),n-m,ncomp);  // take inverse of w11
 bx=negateW(jtpdt(jt,ai,jtpdt(jt,jttake(jt,jtv2(jt,m,m-n),w),di)));  // -w00^_1 mp w01 mp w11^_1
 if(AT(w)&SPARSE){z=jtover(jt,jtstitch(jt,ai,bx),jttake(jt,jtv2(jt,n-m,-n),di));  // should copy this over w, inplace
 }else{
  // copy in the pieces, line by line, writing over the input area
  I leftlen = m<<bplg(AT(w)); I rightlen=(n-m)<<bplg(AT(w));
  z=w; void *zr=voidAV(z);  // reuse input area, set pointer to output
  // copy top part: ai,.bx
  void *leftr=voidAV(ai), *rightr=voidAV(bx);  // input pointers
  DQ(m, memcpy(zr,leftr,leftlen); zr=(C*)zr+leftlen; leftr=(C*)leftr+leftlen; memcpy(zr,rightr,rightlen); zr=(C*)zr+rightlen; rightr=(C*)rightr+rightlen;)
  // copy bottom part: 0,.di
  rightr=voidAV(di);
  DQ(n-m, memset(zr,C0,leftlen); zr=(C*)zr+leftlen; memcpy(zr,rightr,rightlen); zr=(C*)zr+rightlen; rightr=(C*)rightr+rightlen;)
  AFLAG(z)|=AFUPPERTRI; // Mark result as upper-triangular in case we multiply a matrix by it
 }
 //  w00^_1     -w00^_1 mp w01 mp w11^_1
 //    0         w11^_1
 EPILOG(z);
}    /* R.K.W. Hui, Uses of { and }, APL87, p. 56 */


// 128!:1 Invert Upper-triangular matrix return
 A jtrinv(J jt, A w){
 F1RANK(2,jtrinv,UNUSED_VALUE);
 ASSERT(AR(w)==2,EVRANK);  // rank at least 2
 ASSERT(AS(w)[0]==AS(w)[1],EVLENGTH);  // error if not square
 if(!AN(w))return w;  // if empty, return empty
 return jtrinvip(jt,jtca(jt,w),AS(w)[0],AT(w)&FL?2:0);  // take the inverse.  Since it runs in place, clone w.  For float, reduce overhead at bottom of recursion
}

// recursive subroutine for qr decomposition, returns q;r
static A jtqrr(J jt, A w){PROLOG(0067);A a1,q,q0,q1,r,r0,r1,t,*tv,t0,t1,y,z;I m,n,p,*s;
 if(2>AR(w)){p=AN(w); n=1;}else{s=AS(w); p=s[0]; n=s[1];}  // p=#rows, n=#columns
 m=n>>1; I tom=(0x01222100>>((n&7)<<2))&3; m=(m+tom<n)?m+tom:m;  // Minimize number of wasted multiply slots, processing in batches of 4
 if(1>=n){  // just 1 col
  t=jtnorm(jt,jtravel(jt,w));  // norm of col 
  ASSERT(!AN(w)||!jtequ(jt,t,num(0)),EVDOMAIN);  // norm must not be 0 unless column is empty
  RZ(q=tymes(w,jtrecip(jt,t)));
  return link(2>AR(q)?jttable(jt,q):q,jtreshape(jt,jtv2(jt,n,n),p?t:num(1)));
 }
 // construe w as w0 w1 w0t w1t
 RZ(t0=jtqrr(jt,jttake(jt,jtv2(jt,p,m),w)));  // find QR of w0 pxm   w0t
 tv=AAV(t0); q0=*tv++; r0=*tv;  // point to Q and return of w0  pxm mxm  w0t    
 RZ(a1=jtdrop(jt,jtv2(jt,0L,m),w));  // a1=w1  pxn-m  w1t
 RZ(y=jtpdt(jt,conjug(jtcant1(jt,q0)),a1));  // q0* w1 mxpxn-m     w1t q0t*   q0t*=/q0      result is mxn-m
 RZ(t1=jtqrr(jt,minus(a1,jtpdt(jt,q0,y))));  // pxmxn-m  get QR of w1-(q0 q0* w1)    w1t-(w1t q0t* q0t)    
 tv=AAV(t1); q1=*tv++; r1=*tv;  
 RZ(q=jtstitch(jt,q0,q1));  // overall q is q0t    Q of (w1t-(w1t q0t* q0t))
 RZ(r=jtover(jt,jtstitch(jt,r0,y),jttake(jt,jtv2(jt,n-m,-n),r1)));
 // r is   r0    q0* w1
 //        0     return of w1-(q0 q0* w1)
 // qr is  q0 r0    (q0 q0* w1) + (Q of w1-(q0 q0* w1))(return of w1-(q0 q0* w1))
 // = w0 w1 = w
 z=link(q,r); EPILOG(z);
}

#define verifyinplace(to,from) if(to!=from){memcpy(CAV(to),CAV(from),AN(to)<<bplg(AT(to)));}
// this version operates on rows, inplace.  w is not empty
// q is the ADJOINT of the original q matrix
// result is adjoint of the L in LQ decomp, therefore upper-triangular
static A jtltqip(J jt, A w){PROLOG(0067);A l0,l1,y,z;
 A q0; fauxblock(virtwq0); D *w0v=DAV(w);  // q0 & q1 data
 I rw=AS(w)[0]; I cl=AS(w)[1];  // # rows, # columns
  // handle case of 2 rows
 if(rw<=2) {
  // If not FL length 2, handle any length 1
  if(rw<=1){  // just 1 row
   // Use a faux-virtual block to take the norm of w, so that we leave w inplaceable for when we normalize it in place
   fauxvirtual(q0,virtwq0,w,1,ACUC1); AS(q0)[0]=cl; AN(q0)=cl;  // kludge use sumattymesprod to create a table result directly
   A t; RZ(t=jtnorm(jt,q0));  // norm of row
   ASSERT(!jtequ(jt,t,num(0)),EVDOMAIN);  // norm must not be 0
   A z; RZ(z=tymesA(w,jtrecip(jt,t))); verifyinplace(w,z);
   RZ(t=jttable(jt,t)); realizeifvirtual(t); return t; // this is real, so it is also the adjoint of L
  }
 }
 // continue for blocks that must be subdivided
 I m=rw>>1; I tom=(0x01222100>>((rw&7)<<2))&3; m=(m+tom<rw)?m+tom:m;  // Minimize number of wasted multiply slots, processing in batches of 4
 // construe w as w0 w1
 fauxvirtual(q0,virtwq0,w,2,ACUC1|ACINPLACE); AS(q0)[0]=m; AS(q0)[1]=cl; AN(q0)=m*cl;
 RZ(l0=jtltqip(jt,q0));  // form q0 in place, return l0
 A q1; fauxblock(virtwq1);  fauxvirtual(q1,virtwq1,w,2,ACUC1|ACINPLACE); AK(q1)+=(m*cl)<<bplg(AT(w)); AS(q1)[0]=rw-m; AS(q1)[1]=cl; AN(q1)=(rw-m)*cl; 
 // calculate w1 - (w1 q0*) q0
 {
  // general case for all types
  RZ(y=jtpdt(jt,q1,conjug(jtcant1(jt,q0))));  // w1 q0*   n-mxpxm
  RZ(z=minusA(q1,jtpdt(jt,y,q0))); verifyinplace(q1,z);   // w1 - (w1 q0*) q0   n-mxmxp
 }
 RZ(l1=jtltqip(jt,q1));  //  get QR of   w1 - (w1 q0*) q0  
 // copy in the pieces, line by line
 I leftlen = m<<bplg(AT(w)); I rightlen=(rw-m)<<bplg(AT(w));
 GA(z,AT(w),rw*rw,2,AS(w)); AS(z)[1]=rw; void *zr=voidAV(z);  // allocate result, set pointer to output
 // copy top part: l0*,. (w1 q0*)*
 void *leftr=voidAV(l0), *rightr=voidAV(conjug(jtcant1(jt,y)));  // input pointers
 DQ(m, memcpy(zr,leftr,leftlen); zr=(C*)zr+leftlen; leftr=(C*)leftr+leftlen; memcpy(zr,rightr,rightlen); zr=(C*)zr+rightlen; rightr=(C*)rightr+rightlen;)
 // copy bottom part: 0,.(L of w1 - (w1 q0*) q0)*
 rightr=voidAV(l1);
 DQ(rw-m, memset(zr,C0,leftlen); zr=(C*)zr+leftlen; memcpy(zr,rightr,rightlen); zr=(C*)zr+rightlen; rightr=(C*)rightr+rightlen;)
 // q is    q0     (Q of w1 - (w1 q0*) q0)
 // l* is   l0*    (w1 q0*)*
 //         0      (L of w1 - (w1 q0*) q0)*
 // lq is  l0 q0
 //        (w1 q0*) q0 + (L of w1 - (w1 q0*) q0)(Q of w1 - (w1 q0*) q0)
 // = w
 AFLAG(z)|=AFUPPERTRI; // Mark result as upper-triangular in case we multiply a matrix by it
 EPILOG(z);
}

// qr (?) decomposition of w, returns q;r
 A jtqr(J jt, A w){A r,z;D c=inf,d=0,x;I n1,n,*s,wr;
 F1RANK(2,jtqr,UNUSED_VALUE);
 ASSERT(DENSE&AT(w),EVNONCE);
 ASSERT(AT(w)&B01+INT+FL+CMPX,EVDOMAIN);
 wr=AR(w); s=AS(w);
 ASSERT(2>wr||s[0]>=s[1],EVLENGTH);
 RZ(z=jtqrr(jt,w)); r=AAV(z)[1]; n=AS(r)[0]; n1=1+n;
 if(FL&AT(r)){D*v=DAV(r);  DQ(n, x= ABS(*v); if(x<c)c=x; if(x>d)d=x; v+=n1;);}
 else        {Z*v=ZAV(r);  DQ(n, x=zmag(*v); if(x<c)c=x; if(x>d)d=x; v+=n1;);}
 ASSERT(!n||c>d*FUZZ,EVDOMAIN);
return z;
}

// return inverse of w, calculated by lq applied to adjoint
// result has rank 2
static A jtlq(J jt, A w){A l;D c=inf,d=0,x;I n1,n,*s,wr;
 F1RANK(2,jtqr,UNUSED_VALUE);
 ASSERT(DENSE&AT(w),EVNONCE);
 ASSERT(AT(w)&B01+INT+FL+CMPX,EVDOMAIN);
 wr=AR(w); s=AS(w);
 ASSERT(2>wr||s[0]>=s[1],EVLENGTH);
 if(AT(w)&B01+INT)RZ(w=jtcvt(jt,FL,w));  // convert boolean/integer to real
 if(wr==1)w=jttable(jt,w);  // convert column vector to column matrix
 w=conjug(jtcant1(jt,w));  // create w*, where the result will be built inplace
 RZ(l=jtltqip(jt,w)); n=AS(l)[0]; n1=1+n;
 // build determinant for integer correction, if that is enabled (i. e. nonzero)
 if(FL&AT(l)){D*v=DAV(l); D determ=jt->workareas.minv.determ; DQ(n, x= ABS(*v); if(determ!=0){determ*=x; if(determ>1e20)determ=0.0;} if(x<c)c=x; if(x>d)d=x; v+=n1;); jt->workareas.minv.determ=determ;} 
 else        {Z*v=ZAV(l);  DQ(n, x=zmag(*v); if(x<c)c=x; if(x>d)d=x; v+=n1;);}
 ASSERT(!n||c>d*FUZZ,EVDOMAIN);
 return jtpdt(jt,jtrinvip(jt,l,n,AT(w)&FL?2:0),w);  // engage fast reciprocal for float matrices
}

// Boolean/integer correction.  If the inversand was B01 or INT, we can eliminate some rounding error by forcing the
// determinant to integer and then each value to an integer multiple of the determinant.
// The determinant was calculated when we inverted the matrix
static A jticor(J jt, A w){D d,*v;
 if (!w) return 0;
 d=jt->workareas.minv.determ;  // fetch flag/determinant
 if(d==0.0)return w;  // if not enabled or not applicable, return input unchanged
 d=jround(ABS(d));  // force determinant to integer
 D recipd=1/d;
 v=DAV(w); DO(AN(w), v[i]=jround(d*v[i])*recipd;);  // force each value to multiple of recip of determinant, then divide
 return w;
}

 A jtminv(J jt, A w){PROLOG(0068);A q,y,z;I m,n,*s,t,wr;
 F1RANK(2,jtminv,UNUSED_VALUE);
 jt->workareas.minv.determ=0.0;
 t=AT(w); wr=AR(w); s=AS(w); m=wr?s[0]:1; n=1<wr?s[1]:1;
 if(!wr)return jtrecip(jt,w);
 if(!AN(w)){ASSERT(1==wr||m>=n,EVLENGTH); return jtcant1(jt,w);}
 if(AN(w)&&t&RAT+XNUM){
  ASSERT(m>=n,EVLENGTH);
  if(t&XNUM)RZ(w=jtcvt(jt,RAT,w));
  if(1<wr&&m==n)y=w; else{q=jtcant1(jt,w); y=jtpdt(jt,q,w);}
  z=jtdrop(jt,jtv2(jt,0L,n),jtgausselm(jt,jtstitch(jt,y,jtreshape(jt,jtv2(jt,n,n),jttake(jt,jtsc(jt,1+n),jtxco1(jt,jtscf(jt,1.0)))))));
  if(2>wr)z=tymes(jtreshape(jt,mtv,z),w); else if(m>n)z=jtpdt(jt,z,q);
 }else{
  // not RAT/XNUM.  Calculate inverse as R^-1 Q^-1 after taking QR decomp & using Q^-1=Q*
  if(t&B01+INT&&2==wr&&m==n)jt->workareas.minv.determ=1.0;  // if taking inverse of square int, allow setting up for correction afterward
  z=jtlq(jt,w);
  z=jticor(jt,z);  // if integer correction called for, do it
  z=2==wr?z:jtreshape(jt,shape(jt,w),z);
 }
 EPILOG(z);
}

static B jttridiag(J jt,I n,A a,A x){D*av,d,p,*xv;I i,j,n1=n-1;
 av=DAV(a); xv=DAV(x); d=xv[0];
 for(i=j=0;i<n1;++i){
  ASSERT(d!=0,EVDOMAIN);  
  p=xv[j+2]/d;  
  d=xv[j+3]-=p*xv[j+1]; 
  av[i+1]-=p*av[i]; 
  j+=3;
 }
 ASSERT(d!=0,EVDOMAIN); 
 i=n-1; j=AN(x)-1; av[i]/=d;
 for(i=n-2;i>=0;--i){j-=3; av[i]=(av[i]-xv[j+1]*av[i+1])/xv[j];}
 return 1;
}

static A jtmdivsp(J jt,A a,A w){A a1,x,y;I at,d,m,n,t,*v,xt;P*wp;
 ASSERT(2==AR(w),EVRANK);
 v=AS(w); n=v[0]; 
 ASSERT(n>=v[1]&&n==AN(a),EVLENGTH); 
 ASSERT(n==v[1],EVNONCE);
 wp=PAV(w); x=SPA(wp,x); y=SPA(wp,i); a1=SPA(wp,a);
 ASSERT(2==AN(a1),EVNONCE);
 v=AV(y); m=AS(y)[0];
 ASSERT(m==3*n-2,EVNONCE);
 DQ(m, d=*v++; d-=*v++; ASSERT(-1<=d&&d<=1,EVNONCE););
 at=AT(a); xt=AT(x); RE(t=maxtype(at,xt)); RE(t=maxtype(t,FL));
 RZ(a=jtcvt(jt,t,a)); RZ(x=jtcvt(jt,t,x));
 if(t&CMPX)RZ(ztridiag(n,a,x)) else RZ(tridiag(n,a,x));
 return a;
}    /* currently only handles tridiagonal sparse w */


// a %. w  for all types
 A jtmdiv(J jt,A a,A w){PROLOG(0069);A z;I t;
 F2RANK(RMAX,2,jtmdiv,UNUSED_VALUE);
 if(AT(a)&SPARSE)RZ(a=jtdenseit(jt,a));
 t=AT(w);
 if(t&SPARSE)return jtmdivsp(jt,a,w);
 z=jtminv(jt,w);  // take generalized inverse of w, setting up for icor if needed
 z=jtpdt(jt,2>AR(w)?jtreshape(jt,shape(jt,w),z):z,a);  // a * w^-1
 if(AT(a)&B01+INT)z=jticor(jt,z);  // integer correct if a is not float (& correction is possible)
 EPILOG(z);
}
