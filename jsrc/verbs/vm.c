/* Copyright 1990-2008, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Verbs: "Mathematical" Functions (Irrational, Transcendental, etc.)      */

#include "j.h"
#include "ve.h"

D jtintpow(J jt,D x,I n){D r=1;
 if(0>n){x=1/x; if(n==IMIN){r=x; n=IMAX;} else n=-n;}  // kludge use r=x; n=-1-n;
 while(n){if(1&n)r*=x; x*=x; n>>=1;}
 return r;
}    /* x^n where x is real and n is integral */

D jtpospow(J jt,D x,D y){
 if(0==y)return 1.0;
 if(0==x)return 0<y?0.0:inf;
 if(0<x){
  if(y== inf)return 1<x?inf:1>x?0.0:1.0;
  if(y==-inf)return 1<x?0.0:1>x?inf:1.0;
  return exp(y*log(x));
 }
 if(y==-inf){ASSERT(-1>x,EVDOMAIN); return 0.0;}
 if(y== inf){ASSERT(-1<x,EVDOMAIN); return 0.0;}
 jt->jerr=EWIMAG;
 return 0;
}    /* x^y where x and y are real */

#define POWXB(u,v)  (v?u:1)
#define POWBX(u,v)  (u?1.0:v<0?inf:!v)
#define POWII(u,v)  jtintpow(jt,(D)u,v)
#define POWID(u,v)  pospow((D)u,v)

APFX(powBI, D,B,I, POWBX ,,return EVOK;)
APFX(powBD, D,B,D, POWBX ,,return EVOK;)
APFX(powIB, I,I,B, POWXB ,,return EVOK;)
APFX(powII, D,I,I, POWII ,,HDR1JERR)
APFX(powID, D,I,D, POWID ,,HDR1JERR)
APFX(powDB, D,D,B, POWXB ,,return EVOK;)
APFX(powZZ, Z,Z,Z, zpow  ,,HDR1JERR)



APFX(cirZZ, Z,Z,Z, zcir  ,NAN0;,HDR1JERRNAN)


AMON(expD,   D,D, *z=*x<EMIN?0.0:EMAX<*x?inf:exp(   *x);)
AMON(logD,   D,D, ASSERTWR(0<=*x,EWIMAG); *z=log(   *x);)
APFX(powDI, D,D,I, intpow,,HDR1JERR)
APFX(powDD, D,D,D, pospow,,HDR1JERR)

AMON(expI,   D,I, *z=*x<EMIN?0.0:EMAX<*x?inf:exp((D)*x);)
AMON(logI,   D,I, ASSERTWR(0<=*x,EWIMAG); *z=log((D)*x);)

static I jtcirx(J jt,I n,I k,D*z,D*x){D p,t;
 NAN0;
 switch(k){
 default: ASSERTWR(0,EWIMAG);
 case  0: DQ(n, t=*x++; ASSERTWR(ABS(t)<=1.0, EWIMAG ); *z++=sqrt(1.0-t*t);); break;
 case  1: ;
   DQ(n, t=*x++; ASSERTWR(ABS(t)<THMAX,EVLIMIT); *z++=sin(t););
   break;
 case  2:  ;
 DQ(n, t=*x++; ASSERTWR(ABS(t)<THMAX,EVLIMIT); *z++=cos(t););
 break;
 case  3:  ;
 DQ(n, t=*x++; ASSERTWR(ABS(t)<THMAX,EVLIMIT); *z++=tan(t););       
 break;
 case  4: DQ(n, t=*x++;                                     *z++=t<-1e8?-t:1e8<t?t:sqrt(t*t+1.0););       break;
 case  5: DQ(n, t=*x++;                                     *z++=t<-EMAX2?infm:EMAX2<t?inf:sinh(t););     break;
 case  6: DQ(n, t=*x++;                                     *z++=t<-EMAX2||    EMAX2<t?inf:cosh(t););     break;
 case  7: ;
// math library tanh is slooooow  case  7: DQ(n, t=*x++;                                     *z++=t<-TMAX?-1:TMAX<t?1:tanh(t););           break;
 DQ(n, t=*x++;                                     *z++=t<-TMAX?-1:TMAX<t?1:(1.0-exp(-2*t))/(1.0+exp(-2*t)););
 break;
 case -1: ;
  DQ(n, t=*x++; ASSERTWR( -1.0<=t&&t<=1.0, EWIMAG ); *z++=asin(t););
 break;
 case -2: ;
  DQ(n, t=*x++; ASSERTWR( -1.0<=t&&t<=1.0, EWIMAG ); *z++=acos(t););
  break;
 case -3: ;
  DQ(n,                                             *z++=atan(*x++););
  break;
 case -4: DQ(n, t=*x++; ASSERTWR(t<=-1.0||1.0<=t,  EWIMAG ); *z++=t<-1e8||1e8<t?t:t==-1?0:(t+1)*sqrt((t-1)/(t+1));); break;
 case -5: p=log(2.0); 
           DQ(n, t=*x++; *z++=1.0e8<t?p+log(t):-7.8e3>t?-(p+log(-t)):log(t+sqrt(t*t+1.0)););               break;
 case -6: p=log(2.0); 
           DQ(n, t=*x++; ASSERTWR(          1.0<=t, EWIMAG ); *z++=1.0e8<t?p+log(t):log(t+sqrt(t*t-1.0));); break;
 case -7: DQ(n, t=*x++; ASSERTWR( -1.0<=t&&t<=1.0, EWIMAG ); *z++=0.5*log((1.0+t)/(1.0-t)););              break;
 case  9: DQ(n,         *z++=*x++;);           break;    
 case 10: DQ(n, t=*x++; *z++=ABS(t););         break;
 case 11: DQ(n,         *z++=0.0;);            break;
 case 12: DQ(n,         *z++=0<=*x++?0.0:PI;); break;
 }
 ASSERTWR(!NANTEST,EVNAN);
 return EVOK;
}

 I cirBD(I n,I m,B* RESTRICTI x,D* RESTRICTI y,D* RESTRICTI z,J jt){ASSERTWR(n<=1&&1==m,EWIMAG); n^=REPSGN(n); return cirx(n,   (I)*x,z,y);}
 I cirID(I n,I m,I* RESTRICTI x,D* RESTRICTI y,D* RESTRICTI z,J jt){ASSERTWR(n<=1&&1==m,EWIMAG); n^=REPSGN(n); return cirx(n,   *x,z,y);}

 I cirDD(I n,I m,D* RESTRICTI x,D* RESTRICTI y,D* RESTRICTI z,J jt){I k=(I)jround(*x);
 ASSERTWR(k==*x,EVDOMAIN); 
 ASSERTWR(n<=1&&1==m,EWIMAG); // if more than one x value, retry as general case
 n^=REPSGN(n);   // convert complementary n to nonneg
 return cirx(n,k,z,y);
}


A jtlogar2(J jt,A a,A w){
  A z;I t;
  RE(t=maxtype(AT(a),AT(w)));
  jt->xmode=XMEXACT; 
  return jtatomic2(JTIPAW,logar1(w),logar1(a),ds(CDIV));  // better to multiply by recip, but not much, & it makes 0 ^. 0 not fail
}
    
 A jtroot(J jt,A a,A w){A z;I t;
 RE(t=maxtype(AT(a),AT(w)));
 A ma=a; if(TYPESNE(t,AT(a)))RZ(ma=jtcvt(jt, t,a));
 A mw=w; if(TYPESNE(t,AT(w)))RZ(mw=jtcvt(jt, t,w));
 return expn2(mw,recip(ma));  // not inplaceable - could be IMAG
}

 A jtjdot1(J jt, A w){return tymes(a0j1,w);}
 A jtjdot2(J jt,A a,A w){return plus(a,tymes(a0j1,w));}
 A jtrdot1(J jt, A w){return expn1(jdot1(w));}
 A jtrdot2(J jt,A a,A w){return tymes(a,rdot1(w));}


 A jtpolar(J jt, A w){ A z; return jtcvt(jt,SPARSE&AT(w)?SFL:FL,df2(z,jtv2(jt,10L,12L),w,qq(ds(CCIRCLE),jtv2(jt,1L,0L))));}

 A jtrect(J jt, A w){A e,z;B b;I r,t;P*wp,*zp;Z c;
 t=AT(w); r=AR(w); RESETRANK;   // Run as infinite rank
 ASSERT(!AN(w)||t&NUMERIC,EVDOMAIN);
 if(t&CMPX){GATV(z,FL,2*AN(w),1+r,AS(w)); AS(z)[r]=2; memcpy(AV(z),AV(w),AN(z)*sizeof(D)); return z;}
 else if((t&SPARSE)!=0){
  b=1&&t&SCMPX;
  GASPARSE(z,b?SFL:t,1,1+r,AS(w)); AS(z)[r]=2;
  wp=PAV(w); zp=PAV(z);
  if(b){e=SPA(wp,e); c=*ZAV(e); ASSERT(FFEQ(c.re,c.im),EVSPARSE); SPB(zp,e,scf(c.re));}
  else SPB(zp,e,ca(SPA(wp,e)));
  SPB(zp,a,ca(SPA(wp,a)));
  SPB(zp,i,ca(SPA(wp,i)));
  SPB(zp,x,rect(SPA(wp,x)));
  return z;
 }else return df2(z,w,num(0),qq(ds(CCOMMA),zeroionei(0)));
}
