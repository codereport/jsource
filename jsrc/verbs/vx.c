/* Copyright 1990-2008, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Verbs: Extended Precision Integers                                      */

#include "j.h"
#include "ve.h"


X jtxev1(J jt,A w,C*s){A y; 
 RZ(df1(y,jtcvt(jt,XNUM,w),eval(s)));
 ASSERTSYS(!AR(y),"xev1");
 if(!(XNUM&AT(y)))RZ(y=jtcvt(jt,XNUM,y)); 
 return XAV(y)[0];
}

X jtxev2(J jt,A a,A w,C*s){A y; 
 RZ(df2(y,jtcvt(jt,XNUM,a),jtcvt(jt,XNUM,w),eval(s))); 
 ASSERTSYS(!AR(y),"xev2");
 if(!(XNUM&AT(y)))RZ(y=jtcvt(jt,XNUM,y)); 
 return XAV(y)[0];
}

X jtxc(J jt,I n){I m=1,p,*zv;X z; 
 p=n; while(p/=XBASE)++m;
 GATV0(z,INT,m,1); zv=AV(z);
 p=n; DO(m, zv[i]=p%XBASE; p/=XBASE;);
 return z;
}    /* n is non-negative */

D xdouble(X w){D z=0;I c,n,*v;
 n=AN(w); v=n+AV(w); c=*--v;
 if(c==XPINF)return inf; else if(c==XNINF)return infm;
 DQ(n, z=*v--+z*XBASE;);
 return z;
}

I jtxint(J jt,X w){I c,n,*v,z;
 n=AN(w); v=AV(w); v+=n; c=z=*--v;
 ASSERT(n<=XIDIG&&c!=XPINF&&c!=XNINF,EVDOMAIN);
 DQ(n-1, z=*--v+z*XBASE;);
 ASSERT((c^z)>=0,EVDOMAIN);
 return z;
}

 X jtxstd(J jt,    X w){A z;B b;I c=0,d,i,j,k,m=XBASE,n,*zv;
 n=AN(w); RZ(z=ca(w)); zv=AV(z);
 b=0; j=n; DQ(n, --j; if(zv[j]){b=0<zv[j]; break;});
 if(b) for(i=0;i<n;++i){
  k=zv[i]+=c; 
  if     (0>  k){c=-1-(-k)/m; zv[i]=d=m-(-k)%m; if(d== m){zv[i]=0; ++c;}}
  else if(m<= k){c=k/m;       zv[i]=k%m;}
  else          c=0;
 }else for(i=0;i<n;++i){
  k=zv[i]+=c; 
  if     (0<  k){c=1+k/m;     zv[i]=d=(k%m)-m;  if(d==-m){zv[i]=0; --c;}}
  else if(m<=-k){c=-(-k)/m;   zv[i]=-(-k)%m;}
  else          c=0;
 }
 if(c)return apip(z,sc(c));
 j=n-1; while(j&&!zv[j])--j; ++j;
 return j==n?z:vec(INT,j,zv);
}    /* convert to standard form */

I jtxcompare(J jt,X a,X w){I*av,j,m,n,x,y,*wv;int s,t;
 RE(1);
 m=AN(a); av=AV(a); x=av[m-1]; s=SGN(x);
 n=AN(w); wv=AV(w); y=wv[n-1]; t=SGN(y);
 if(s!=t)return s?s:-t;
 if(1==m&&(x==XPINF||x==XNINF))return 0<x? !(1==n&&y==XPINF):-!(1==n&&y==XNINF);
 if(1==n&&(y==XPINF||y==XNINF))return 0<y?-!(1==m&&x==XPINF): !(1==m&&x==XNINF);
 if(m!=n)return m>n?s:-s;
 j=m; DQ(m, --j; if(av[j]!=wv[j])return av[j]>wv[j]?1:-1;);
 return 0;
}    /* _1 0 1 depending on whether a<w, a=w, a>w */


 X jtxsgn(J jt,    X w){I x=XDIG(w); return xc(SGN(x));}

 X jtxplus(J jt,X a,X w){PROLOG(0094);A z;I an,*av,c,d,m,n,wn,*wv,*zv;
 if(!(a && w)) return 0;
 an=AN(a); av=AV(a); c=av[an-1];
 wn=AN(w); wv=AV(w); d=wv[wn-1];
 if(c==XPINF||c==XNINF||d==XPINF||d==XNINF){
  ASSERT(!(c==XPINF&&d==XNINF||c==XNINF&&d==XPINF),EVNAN);
  return rifvsdebug(vci(c==XPINF||d==XPINF?XPINF:XNINF));
 }
 m=MAX(an,wn); n=MIN(an,wn);
 GATV0(z,INT,m,1); zv=AV(z);
 DQ(n, *zv++=*av+++*wv++;);
 if(m>n)ICPY(zv,an>wn?av:wv,m-n);
 z=xstd(z);
 EPILOGNOVIRT(z);
}

 X jtxminus(J jt,X a,X w){PROLOG(0095);A z;I an,*av,c,d,m,n,wn,*wv,*zv;
 an=AN(a); av=AV(a); c=av[an-1];
 wn=AN(w); wv=AV(w); d=wv[wn-1];
 if(c==XPINF||c==XNINF||d==XPINF||d==XNINF){
  ASSERT(c!=d,EVNAN);
  return rifvsdebug(vci(c==XPINF||d==XNINF?XPINF:XNINF));
 }
 m=MAX(an,wn); n=MIN(an,wn);
 GATV0(z,INT,m,1); zv=AV(z);
 DQ(n, *zv++=*av++-*wv++;);
 if(m>n){if(an>wn)ICPY(zv,av,m-n); else DQ(m-n, *zv++=-*wv++;);}
 z=xstd(z);
 EPILOGNOVIRT(z);
}

 X jtxtymes(J jt,X a,X w){A z;I an,*av,c,d,e,i,j,m=XBASE,n,*v,wn,*wv,*zv;
 if(!(a && w)) return 0;
 an=AN(a); av=AV(a); c=av[an-1];
 wn=AN(w); wv=AV(w); d=wv[wn-1];
 if(!c||!d)return iv0;
 if(c==XPINF||c==XNINF||d==XPINF||d==XNINF)return rifvsdebug(vci(0<c*d?XPINF:XNINF));
 n=an+wn; GATV0(z,INT,n,1); zv=v=AV(z); memset(zv,C0,n*SZI);
 for(i=0;i<an;++i,++zv){
  if(c=av[i])for(j=0;j<wn;++j){
   d=zv[j]+=c*wv[j];
   if     (m<= d){e=  d /m; zv[j]-=e*m; zv[1+j]+=e;}
   else if(m<=-d){e=(-d)/m; zv[j]+=e*m; zv[1+j]-=e;}
 }}
 return rifvsdebug(v[n-1]?z:vec(INT,v[n-2]?n-1:1,v));
}

static X jtshift10(J jt,I e,X w){A z;I c,d,k,m,n,q,r,*wv,*zv;
 n=AN(w); wv=AV(w); c=wv[n-1];
 q=e/XBASEN; r=e%XBASEN; d=0==r?1:1==r?10:2==r?100:1000;
 m=n+q+(I )(XBASE<=c*d);
 GATV0(z,INT,m,1); zv=AV(z);
 DQ(q, *zv++=0;);
 if(r){c=0; DQ(n, k=c+d**wv++; *zv++=k%XBASE; c=k/XBASE;); if(c)*zv=c;}
 else DQ(n, *zv++=*wv++;);
 return z;
}    /* w*10^e, positive w */

B jtxdivrem(J jt,X a,X w,X*qz,X*rz){B b,c;I*av,d,j,n,*qv,r,y;X q;
 j=n=AN(a); av=AV(a); b=0<=av[n-1];
 y=AV(w)[0]; c=0<=y; if(!c)y=-y; r=0;
 GATV0(q,INT,n,1); qv=AV(q);
 switch(2*b+c){
  case 0: DQ(n, --j; d=r*XBASE-av[j]; r=d%y; qv[j]=  d/y ;); r=-r;      break;
  case 1: DQ(n, --j; d=r*XBASE-av[j]; r=d%y; qv[j]=-(d/y);); r=r?y-r:0; break;
  case 2: DQ(n, --j; d=r*XBASE+av[j]; r=d%y; qv[j]=-(d/y);); r=r?r-y:0; break;
  case 3: DQ(n, --j; d=r*XBASE+av[j]; r=d%y; qv[j]=  d/y ;);            break;
 }
 if(r&&b!=c){--qv[0]; DO(n-1, if(qv[i]>-XBASE)break; qv[i]=0; --qv[1+i];);}
 if(1<n&&!qv[n-1])AN(q)=AS(q)[0]=n-1;
 *qz=q; *rz=rifvsdebug(vec(INT,1L,&r)); return 1;
}    /* (<.a%w),(w|a) where w has a single "digit" and is nonzero */
 
X jtxdiv(J jt,X a,X w,I mode){PROLOG(0096);B di;I an,*av,c,c0,d,e,k,s,u[2],u1,wn,*wv,yn;X q,r,y;
 RZ(a&&w&&!jt->jerr);
 an=AN(a); av=AV(a); c=c0=av[an-1];
 wn=AN(w); wv=AV(w); d=   wv[wn-1]; di=d==XPINF||d==XNINF;
 if(c&&!d)return rifvsdebug(vci(0<c?XPINF:XNINF));
 if(c==XPINF||c==XNINF){ASSERT(!di,EVNAN); return rifvsdebug(vci(0<c*d?XPINF:XNINF));}
 if(di)return iv0;
 if(1==wn&&d){I*v;
  RZ(xdivrem(a,w,&q,&r));  // must not return virtual
  if(!AV(r)[0]||mode==XMFLR)return q;
  ASSERT(mode==XMCEIL,EWRAT);
  v=AV(q); ++*v; 
  return rifvsdebug(XBASE>*v?q:xstd(q));
 }
 switch((0<=c?2:0)+(I )(0<=d)){
  case 0: return rifvsdebug(xdiv(negate(a),negate(w),mode));
  case 1: return rifvsdebug(negate(xdiv(negate(a),w,mode==XMFLR?XMCEIL:mode==XMCEIL?XMFLR:mode)));
  case 2: return rifvsdebug(negate(xdiv(a,negate(w),mode==XMFLR?XMCEIL:mode==XMCEIL?XMFLR:mode)));
  default:
   if(1!=(e=xcompare(a,w))){
    ASSERT(!(c&&e&&mode==XMEXACT),EWRAT); 
    d=c&&(mode||!e); 
    return rifvsdebug(vec(INT,1L,&d));
   }
   if(1<an)c=av[an-2]+c*XBASE;
   if(1<wn)d=wv[wn-2]+d*XBASE;
   e=c>=d?c/d:(I)((XBASE*(D)c)/d); u[0]=e%XBASE; u[1]=u1=e/XBASE; 
   RZ(q=vec(INT,u1?2L:1L,u)); 
   RZ(y=xtymes(w,q)); yn=AN(y); e=AV(y)[yn-1];
   k=c0>=e?c0/e:e/c0; 
   k=(k>3)+(k>32)+(k>316)+(k>3162);
   s=XBASEN*(an-yn)+(c0>=e?k:-k); 
   if(s){q=jtshift10(jt,s,q); y=jtshift10(jt,s,y);}
   A z=xplus(q,xdiv(xminus(a,y),w,mode));
   EPILOGNOVIRT(z);
  }
}   /* <.a%w (mode=XMFLR) or >.a%w (mode=XMCEIL) or a%w (mode=XMEXACT) */

 X jtxrem(J jt,X a,X w){I c,d,e;X q,r,y;
 c=XDIG(a); d=XDIG(w);
 if(!c)return rifvs(w);
 ASSERT(!(d==XPINF||d==XNINF),EVNAN);
 if(c==XPINF)return 0<=d?w:a;
 if(c==XNINF)return 0>=d?w:a;
 if(1==AN(a)&&c){RZ(xdivrem(w,a,&q,&r)); return rifvsdebug(r);}
 switch((0<=c?2:0)+(I )(0<=d)){
  case 0:  return rifvsdebug(negate(xrem(negate(a),negate(w))));
  case 1:  y=xrem(negate(a),w); return rifvsdebug(xcompare(y,iv0)? xplus(a,y):y);
  case 2:  y=xrem(a,negate(w)); return rifvsdebug(xcompare(y,iv0)?xminus(a,y):y);
  default: return rifvsdebug(0<=(e=xcompare(a,w)) ? (e?w:iv0) : xminus(w,xtymes(a,xdiv(w,a,XMFLR))));
}}
                                             
 X jtxgcd(J jt,X a,X w){I c,d;X p,q,t;
 c=XDIG(a); if(0>c)RZ(a=negate(a)); 
 d=XDIG(w); if(0>d)RZ(w=negate(w));
 ASSERT(!(c==XPINF||c==XNINF||d==XPINF||d==XNINF),EVNAN);
 if(!c)return rifvsdebug(w);
 if(!d)return rifvsdebug(a);
 p=a; q=w; A *old=jt->tnextpushp;
 while(XDIG(p)){
  t=p;
  RZ(p=xrem(p,q));
  q=t;
  if(!gc3(&p,&q,0L,old))return 0;
 }
 return rifvsdebug(q);
}

 X jtxlcm(J jt,X a,X w){return rifvsdebug(xtymes(a,xdiv(w,xgcd(a,w),XMEXACT)));}

static X jtxexp(J jt,X w,I mode){I k,m;X s,y;
 k=XDIG(w);
 ASSERT(!k||mode!=XMEXACT,EWIRR);
 if(0>k)return rifvsdebug(xc(mode));
 m=(I)(2.718281828*xint(w)); k=2; s=xplus(iv1,w); y=w;
 DQ(m, y=xtymes(y,w); s=xplus(xtymes(s,xc(k)),y); ++k;);
 return rifvsdebug(xdiv(s,jtxev1(jt,apv(1+m,1L,1L),"*/"),mode));
}

 X jtxpow(J jt,X a,X w){PROLOG(0097);I c,d,e,r;X m,t,z;
 c=XDIG(a); d=XDIG(w); e=AV(w)[0];
 if(c==XPINF||c==XNINF){
  ASSERT(0<c||d!=XPINF,EVDOMAIN); 
  return rifvsdebug(vci(!d?1L:0>d?0L:0<c?c:1&e?XNINF:XPINF));
 }
 if(d==XPINF||d==XNINF){
  ASSERT(0<=c||d==XNINF,EVDOMAIN); 
  return rifvsdebug(vci(1==c&&1==AN(a)?1L:!c&&0>d||c&&0<d?XPINF:0L));
 }
 if(1==AN(a)&&(1==c||-1==c))return 1==c||0==(e&1)?iv1:xc(-1L);
 if(!c){ASSERT(0<=d,EWRAT); return d?iv0:iv1;}
 if(0>d){
  ASSERT(!jt->xmod,EVDOMAIN); 
  ASSERT(jt->xmode!=XMEXACT,EWRAT); 
  r=jt->xmode==XMCEIL; return rifvsdebug(xc(0<c?r:1&e?r-1:r));
 }
 t=a; z=iv1; m=jt->xmod?XAV(jt->xmod)[0]:0;
 if(!m||1>xcompare(w,xc(IMAX))){
  ASSERT(m||2>=AN(w),EVLIMIT);
  RE(e=xint(w));
  if(m)while(e){if(1&e)RZ(z=xrem(m,xtymes(z,t))); RZ(t=xrem(m,xsq(t))); e>>=1;}
  else while(e){if(1&e)RZ(z=       xtymes(z,t) ); RZ(t=       xsq(t) ); e>>=1;}
 }else{B b;I n,*u,*v;X e;
  RZ(e=ca(w)); n=AN(e); v=AV(e);
  while(n){
   if(1&*v)RZ(z=xrem(m,xtymes(z,t))); 
   RZ(t=xrem(m,xtymes(t,t))); 
   b=1; c=0; u=v+n;
   DQ(n, d=c+*--u; c=1&d?XBASE:0; *u=d>>1; if(b&=!*u)--n;);  /* e=.<.e%2 */
 }}
 EPILOGNOVIRT(z);
}

 X jtxsq(J jt,    X w){return xtymes(w,w);}

 X jtxsqrt(J jt,    X w){I c,m,n,p,q,*wv;X e,x;
 n=AN(w); wv=AV(w); c=wv[n-1];
 ASSERT(0<=c,EWIMAG);
 if(!(1&n))c=wv[n-2]+c*XBASE;
 m=(1+n)>>1; RZ(x=apvwr(m,0L,0L)); AV(x)[m-1]=(I)sqrt((D)c);
 RZ(e=xc(2L));
 p=m*XBASEN; q=0; while(p){++q; p>>=1;} 
 DQ(1+q, RZ(x=xdiv(xplus(x,xdiv(w,x,XMFLR)),e,XMFLR)););
 p=xcompare(w,xsq(x));
 switch(jt->xmode){
  default:     ASSERTSYS(0,"xsqrt");
  case XMFLR:  if(-1==p){--AV(x)[0]; return xstd(x);}else return rifvsdebug(x);
  case XMCEIL: if( 1==p){++AV(x)[0]; return xstd(x);}else return rifvsdebug(x);
  case XMEXACT: 
   if(!p)return rifvsdebug(x);
   AV(x)[0]+=p; RZ(x=xstd(x));
   ASSERT(!xcompare(w,xsq(x)),EWIRR);
   return rifvsdebug(x);
}}

static X jtxroot(J jt,X a,X w){A q;D x;I an,*av,c,d,r,wn,*wv;X n,n1,p,t,z;
 an=AN(a); av=AV(a); c=av[an-1];
 wn=AN(w); wv=AV(w); d=wv[wn-1]; 
 ASSERT(0<=d,EWIMAG);
 if(1==wn&&((d&~1)==0))return 1==d?iv1:0<=c?iv0:vci(XPINF);
 if(!c&&0<d)return rifvsdebug(vci(XPINF));
 r=xint(a); if(jt->jerr){RESETERR; return iv1;}
 if(2==r)return xsqrt(w);
 x=xlogabs(w)/r;
 if(x<709.78){RZ(q=ceil1(jtcvt(jt,RAT,scf(exp(x))))); z=XAV(q)[0];}
 else        {RZ(q=jtcvt(jt,XNUM,scf(jceil(x)))); z=jtxexp(jt,XAV(q)[0],XMCEIL);}
 RZ(n=xc(r)); RZ(n1=xc(r-1));
 RZ(t=xdiv(w,p=xpow(z,n1),XMFLR));
 RZ(z=xdiv(xplus(t,xtymes(z,n1)),n,XMFLR))
 while(1){
  RZ(t=xdiv(w,p=xpow(z,n1),XMFLR));
  if(1>xcompare(z,t))break;
  RZ(z=xdiv(xplus(t,xtymes(z,n1)),n,XMFLR))
 }
 if(XMFLR==jt->xmode||!xcompare(w,xtymes(z,p)))return rifvsdebug(z);
 if(XMCEIL==jt->xmode)return rifvsdebug(xplus(z,iv1));
 ASSERT(0,EWIRR);
}

D jtxlogabs(J jt,X w){D c;I m,n,*v;
 n=AN(w); m=MIN(n,20/XBASEN); v=n+AV(w);
 c=0.0; DQ(m, c=c*XBASE+(D)*--v;);
 return log(ABS(c))+XBASEN*(n-m)*2.3025850929940457;
}

static X jtxlog1(J jt,    X w){B b;I c;
 c=XDIG(w); b=1==c&&1==AN(w);
 ASSERT(0<=c,EWIMAG);
 ASSERT(b||jt->xmode!=XMEXACT,EWIRR);
 return rifvsdebug(xc((I)xlogabs(w)+(I )(!b&&jt->xmode==XMCEIL)));
}

static D jtxlogd1(J jt,X w){ASSERT(0<=XDIG(w),EWIMAG); return xlogabs(w);}

static Z jtxlogz1(J jt,X w){Z z; z.re=xlogabs(w); z.im=0>XDIG(w)?PI:0.0; return z;}


static X jtxlog2sub(J jt,X a,X w){ASSERT(0,EVNONCE);}

static X jtxlog2(J jt,X a,X w){D c,d,x,y;I an,*av,j,k,m,n,wn,*wv;X p,q;
 an=AN(a); av=AV(a); c=(D)av[an-1]; if(1<an)c=av[an-2]+c*XBASE;
 wn=AN(w); wv=AV(w); d=(D)wv[wn-1]; if(1<wn)d=wv[wn-2]+d*XBASE;
 if(2<an)return rifvsdebug(xlog2sub(a,w));
 ASSERT(0<=c,EWIMAG);
 if(!c){ASSERT(d!=0,EVDOMAIN); return iv0;}
 if(!d){ASSERT(0<c,EVDOMAIN); return rifvsdebug(vci(XNINF));}
 ASSERT(0<d,EVDOMAIN);
 if(1==c)return rifvsdebug(1==d?iv0:vci(XPINF));
 x=log(c)+XBASEN*(2<an?an-2:0)*2.3025850929940457;
 y=log(d)+XBASEN*(2<wn?wn-2:0)*2.3025850929940457;
 m=n=(I)(y/x+(I )(an<wn));
 RZ(p=q=xpow(a,xc(m))); j=k=xcompare(p,w);
 if     (0<j){--m; RZ(p=xdiv(p,a,XMEXACT)); j=xcompare(p,w); if(0<j)return rifvsdebug(xlog2sub(a,w));}
 else if(0>j){++n; RZ(q=xtymes(p,a));       k=xcompare(q,w); if(0>k)return rifvsdebug(xlog2sub(a,w));}
 ASSERT(jt->xmode!=XMEXACT||!j||!k,EWIRR); 
 return rifvsdebug(xc(!j?m:!k?n:jt->xmode==XMCEIL?n:m));
}

 A jtxlog2a(J jt,A a,A w){A z; GAT0(z,XNUM,1L,0L); XAV(z)[0]=rifvsdebug(xlog2(XAV(a)[0],XAV(w)[0])); RNE(z);}
 A jtxroota(J jt,A a,A w){A z; GAT0(z,XNUM,1L,0L); XAV(z)[0]=rifvsdebug(xroot(XAV(a)[0],XAV(w)[0])); RNE(z);}

 X jtxfact(J jt,    X w){I n;
 n=AV(w)[0];
 if(n==XPINF||n==XNINF)return vci(XPINF);
 RE(n=xint(w)); 
 if(0>n)return rifvsdebug(vci(n&1?XPINF:XNINF));
 return rifvsdebug(jtxev1(jt,apv(n,1L,1L),"*/"));
}

static X jtxbinp(J jt,X a,X w){PROLOG(0098);D m;I i,n;X c,d,p,q,r,s;
 RZ(d=xminus(w,a)); s=1==xcompare(a,d)?d:a; RE(n=xint(s));
 m=xdouble(w);
 if(m<=FLIMAX){
  RZ(p=jtless(jt,ravel(factor(apv(n,(I)m,-1L))),zeroionei(0)));
  RZ(q=jtless(jt,ravel(factor(apv(n,1L,   1L))),zeroionei(0)));
  c=over(p,q);
  d=jtrepeat(jt,jtv2(jt,AN(p),AN(q)),jtv2(jt,1L,-1L));
  A z=jtxev1(jt,jtrepeat(jt,ev2(c,d,"+//."),nub(c)),"*/");
  EPILOGNOVIRT(z);
 }else{
  p=q=iv1; r=w;  
  for(i=0;i<n;++i){
   p=xtymes(p,r); r=xminus(r,iv1);  
   q=xtymes(q,s); s=xminus(s,iv1);
   d=xgcd(p,q); p=xdiv(p,d,XMEXACT); q=xdiv(q,d,XMEXACT);
   if(jt->jerr)return 0;
  }
  EPILOGNOVIRT(p);
}}   /* non-negative x,y; x<=y */

 X jtxbin(J jt,X a,X w){X d,z;
 RZ(d=xminus(w,a));
 switch(4*(I )(0>XDIG(a))+2*(I )(0>XDIG(w))+(I )(0>XDIG(d))){
  default:             ASSERTSYS(0,"xbin");
  case 2: /* 0 1 0 */  /* Impossible */
  case 5: /* 1 0 1 */  /* Impossible */
  case 1: /* 0 0 1 */ 
  case 4: /* 1 0 0 */ 
  case 7: /* 1 1 1 */  return iv0;
  case 0: /* 0 0 0 */  return rifvsdebug(jtxbinp(jt,a,w));
  case 3: /* 0 1 1 */  
   z=jtxbinp(jt,a,xminus(a,xplus(w,iv1)));           return rifvsdebug(AV(a)[0]&1?negate(z):z);
  case 6: /* 1 1 0 */  
   z=jtxbinp(jt,xminus(xc(-1L),w),xminus(xc(-1L),a)); return rifvsdebug(AV(d)[0]&1?negate(z):z);
 }
}

static A jtpiev(J jt,I n,X b){A e;I ek,i,n1=n-1;X bi,e0,e1,*ev,t;
 GATV0(e,XNUM,n,1); ev=XAV(e);
 bi=e0=e1=iv1;
 for(i=0,ek=1;i<n1;++i,ek+=3){
  ev[i]=xtymes(e0,xtymes(XCUBE(e1),bi));
  t=xtymes(xc(ek),xtymes(xc(1+ek),xc(2+ek)));
  e0=xtymes(e0,t);        /* e0 = ! 3*i */
  e1=xtymes(e1,xc(1+i));  /* e1 = ! i   */
  bi=xtymes(bi,b);        /* bi = b^i   */
 }
 ev[i]=rifvsdebug(xtymes(e0,xtymes(XCUBE(e1),bi)));
 RE(e); return e;
}

static X jtxpi(J jt,    X w){A e;B p;I i,n,n1,sk;X a,b,c,d,*ev,k,f,m,q,s,s0,t;
 if(!XDIG(w))return iv0;
 ASSERT(jt->xmode!=XMEXACT,EVDOMAIN);
 RZ(a=xc(545140134L));
 RZ(b=XCUBE(xc(640320L)));
 RZ(c=xc(13591409L));
 RZ(d=xplus(xc(541681608L),xtymes(xc(10L),xc(600000000L))));
 n1=(13+AN(w)*XBASEN)/14; n=1+n1;
 RZ(e=jtpiev(jt,n,b)); ev=XAV(e); m=ev[n1];
 f=iv0; s0=iv1; sk=1;
 for(i=p=0;;++i,p^=1){
  s=xtymes(s0,xplus(c,xtymes(a,xc(i))));
  t=xdiv(xtymes(s,m),ev[i],XMEXACT);
  f=p?xminus(f,t):xplus(f,t);
  if(i==n1)break;
  DO(6, s0=xtymes(s0,xc(sk++));); RE(s0);  /* s0 = ! 6*i */
 }
 f=xtymes(d,f);
 q=xpow(xc(10L),xc(14*n1));
 k=xtymes(xtymes(a,m),xsqrt(xtymes(b,xsq(q))));
 return rifvsdebug(xdiv(xtymes(k,w),xtymes(f,q),jt->xmode));
}    /* Chudnovsky Bros. */

APFX( plusXX, X,X,X, xplus ,,HDR1JERR)
APFX(minusXX, X,X,X, xminus,,HDR1JERR)
APFX(tymesXX, X,X,X, xtymes,,HDR1JERR)
APFX(  divXX, X,X,X, XDIV  ,,HDR1JERR)
APFX(  remXX, X,X,X, xrem  ,,HDR1JERR)
APFX(  gcdXX, X,X,X, xgcd  ,,HDR1JERR)
APFX(  lcmXX, X,X,X, xlcm  ,,HDR1JERR)
APFX(  minXX, X,X,X, XMIN  ,,HDR1JERR)
APFX(  maxXX, X,X,X, XMAX  ,,HDR1JERR)
APFX(  powXX, X,X,X, xpow  ,,HDR1JERR)
APFX(  binXX, X,X,X, xbin  ,,HDR1JERR)

AMON( sgnX, X,X, *z=  rifvsdebug(xsgn(*x));)
AMONPS(sqrtX, X,X, , *z= rifvsdebug(xsqrt(*x)); , HDR1JERR)
AMONPS( expX, X,X, , *z=  rifvsdebug(jtxexp(jt,*x,jt->xmode)); , HDR1JERR)
AMONPS( logX, X,X, , *z= rifvsdebug(xlog1(*x)); , HDR1JERR)
AMONPS(logXD, D,X, , *z=xlogd1(*x); , HDR1JERR)
AMON(logXZ, Z,X, *z=xlogz1(*x);)
AMONPS( absX, X,X, , *z=   rifvs(mag(*x)); , HDR1JERR)
AMONPS(factX, X,X, , *z= rifvsdebug(xfact(*x)); , HDR1JERR)
AMONPS( pixX, X,X, , *z=   rifvsdebug(xpi(*x)); , HDR1JERR)


 A jtdigits10(J jt, A w){A z;B b=0;I c,m,n,*v,*zv,*zv0;X x;
 if(!AR(w))switch(CTTZ(AT(w))){
  case INTX:  b=0<=AV(w)[0]; break;
  case XNUMX: x=XAV(w)[0]; n=AN(x); v=AV(x); b=0<=v[n-1]; break;
  case RATX:  x=XAV(w)[0]; n=AN(x); v=AV(x); b=0<=v[n-1]&&equ(iv1,QAV(w)->d);
 }
 if(!b)return rank1ex0(thorn1(w),UNUSED_VALUE,jtexec1);
 m=INT&AT(w)?(19):XBASEN*AN(x);
 GATV0(z,INT,m,1); zv=zv0=AV(z);
 if(INT&AT(w)){c=AV(w)[0]; *zv++=c%10; while(c/=10)*zv++=c%10;}
 else{
  DQ(n-1, c=*v++; DQ(XBASEN, *zv++=c%10; c/=10;););
  c=*v++; if(c||1==n)*zv++=c%10; while(c/=10)*zv++=c%10;
 }
 AN(z)=AS(z)[0]=n=zv-zv0; 
 zv=zv0; v=zv0+n-1; DQ(n>>1, c=*zv; *zv++=*v; *v--=c;); /* reverse in place */
 return z;
}    /* "."0@": w */
