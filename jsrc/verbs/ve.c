/* Copyright 1990-2008, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Verbs: Elementary Functions (Arithmetic, etc.)                          */

#include "j.h"
#include "vasm.h"
#include "vcomp.h"

#define DIVI(u,v)     (u||v ? ddiv2(u,(D)v) : 0.0)
#define DIVBB(u,v)    (v?u:u?inf:0.0)

#define TYMESID(u,v)  (u   ?u*v:0)
#define TYMESDI(u,v)  (   v?u*v:0)
#define TYMESDD(u,v)  TYMES(u,v)


// BI add, noting overflow and leaving it, possibly in place.  If we add 0, copy the numbers (or leave unchanged, if in place)
 I plusBI(I n,I m,B* RESTRICTI x,I* RESTRICTI y,I* RESTRICTI z,J jt){I u;I v;I oflo=0;
 if(n-1==0)  DQ(m, u=(I)*x; v=*y; if(v==IMAX)oflo+=u; v=u+v; *z++=v; x++; y++; )
 else if(n-1<0){n=~n; DQ(m, u=(I)*x++; if(u){DQ(n, v=*y; if(v==IMAX)oflo=1; v=v+1; *z++=v; y++;)}else{if(z!=y)MC(z,y,n<<LGSZI); z+=n; y+=n;})}
 else      DQ(m, v=*y++; DQ(n, u=(I)*x; if(v==IMAX)oflo+=u; u=u+v; *z++=u; x++;))
 return oflo?EWOVIP+EWOVIPPLUSBI:EVOK;
}

// IB add, noting overflow and leaving it, possibly in place.  If we add 0, copy the numbers (or leave unchanged, if in place)
 I plusIB(I n,I m,I* RESTRICTI x,B* RESTRICTI y,I* RESTRICTI z,J jt){I u;I v;I oflo=0;
 if(n-1==0)  DQ(m, u=*x; v=(I)*y; if(u==IMAX)oflo+=v; u=u+v; *z++=u; x++; y++; )
 else if(n-1<0)DQ(m, u=*x++; DQC(n, v=(I)*y; if(u==IMAX)oflo+=v; v=u+v; *z++=v; y++;))
 else      DQ(m, v=(I)*y++; if(v){DQ(n, u=*x; if(u==IMAX)oflo=1; u=u+1; *z++=u; x++;)}else{if(z!=x)MC(z,x,n<<LGSZI); z+=n; x+=n;})
 return oflo?EWOVIP+EWOVIPPLUSIB:EVOK;
}

APFX( plusDD, D,D,D, PLUS,NAN0;,ASSERTWR(!NANTEST,EVNAN); return EVOK;)
APFX(minusDD, D,D,D, MINUS,NAN0;,ASSERTWR(!NANTEST,EVNAN); return EVOK;)
APFX(minDD, D,D,D, MIN,,return EVOK;)
APFX(maxDD, D,D,D, MAX,,return EVOK;)
APFX(tymesDD, D,D,D, TYMESDD,,return EVOK;)
APFX(  divDD, D,D,D, DIV,NAN0;,ASSERTWR(!NANTEST,EVNAN); return EVOK;)

// II add, noting overflow and leaving it, possibly in place
 I plusII(I n,I m,I* RESTRICTI x,I* RESTRICTI y,I* RESTRICTI z,J jt){I u;I v;I w;I oflo=0;
 // overflow is (input signs equal) and (result sign differs from one of them)
 // If u==0, v^=u is always 0 & therefore no overflow
 if(n-1==0) DQ(m, u=*x; v=*y; w= ~u; u+=v; *z=u; ++x; ++y; ++z; w^=v; v^=u; if(XANDY(w,v)<0)++oflo;)
 else if(n-1<0)DQ(m, u=*x++; I thresh = IMIN-u; if (u<=0){DQC(n, v=*y; if(v<thresh)++oflo; v=u+v; *z++=v; y++;)}else{DQC(n, v=*y; if(v>=thresh)++oflo; v=u+v; *z++=v; y++;)})
 else      DQ(m, v=*y++; I thresh = IMIN-v; if (v<=0){DQ(n, u=*x; if(u<thresh)++oflo; u=u+v; *z++=u; x++;)}else{DQ(n, u=*x; if(u>=thresh)++oflo; u=u+v; *z++=u; x++;)})
 return oflo?EWOVIP+EWOVIPPLUSII:EVOK;
// II subtract, noting overflow and leaving it, possibly in place
}
 I minusII(I n,I m,I* RESTRICTI x,I* RESTRICTI y,I* RESTRICTI z,J jt){I u;I v;I w;I oflo=0;
 // overflow is (input signs differ) and (result sign differs from minuend sign)
 if(n-1==0)  {DQ(m, u=*x; v=*y; w=u-v; *z=w; ++x; ++y; ++z; v^=u; u^=w; if(XANDY(u,v)<0)++oflo;)}
// if u<0, oflo if u-v < IMIN => v > u-IMIN; if u >=0, oflo if u-v>IMAX => v < u+IMIN+1 => v <= u+IMIN => v <= u-IMIN
 else if(n-1<0)DQ(m, u=*x++; I thresh = u-IMIN; if (u<0){DQC(n, v=*y; if(v>thresh)++oflo; w=u-v; *z++=w; y++;)}else{DQC(n, v=*y; if(v<=thresh)++oflo; w=u-v; *z++=w; y++;)})
 // if v>0, oflo if u-v < IMIN => u < v+IMIN = v-IMIN; if v<=0, oflo if u-v > IMAX => u>v+IMAX => u>v-1-IMIN => u >= v-IMIN
 else      DQ(m, v=*y++; I thresh = v-IMIN; if (v<=0){DQ(n, u=*x; if(u>=thresh)++oflo; u=u-v; *z++=u; x++;)}else{DQ(n, u=*x; if(u<thresh)++oflo; u=u-v; *z++=u; x++;)})
 return oflo?EWOVIP+EWOVIPMINUSII:EVOK;
}
APFX(  minII, I,I,I, MIN,,return EVOK;)
APFX(  maxII, I,I,I, MAX,,return EVOK;)

// BD DB add similarly?

// BI subtract, noting overflow and leaving it, possibly in place.  If we add 0, copy the numbers (or leave unchanged, if in place)
 I minusBI(I n,I m,B* RESTRICTI x,I* RESTRICTI y,I* RESTRICTI z,J jt){I u;I v;I w;I oflo=0;
 if(n-1==0)  DQ(m, u=(I)*x; v=*y; u=u-v; if((v&u)<0)++oflo; *z++=u; x++; y++; )
 else if(n-1<0)DQ(m, u=(I)*x++; DQC(n, v=*y; w=u-v; if((v&w)<0)++oflo; *z++=w; y++;))
 else      DQ(m, v=*y++; DQ(n, u=(I)*x; u=u-v; if((v&u)<0)++oflo; *z++=u; x++;))
 return oflo?EWOVIP+EWOVIPMINUSBI:EVOK;
}

// IB subtract, noting overflow and leaving it, possibly in place.  If we add 0, copy the numbers (or leave unchanged, if in place)
 I minusIB(I n,I m,I* RESTRICTI x,B* RESTRICTI y,I* RESTRICTI z,J jt){I u;I v;I w;I oflo=0;
 if(n-1==0)  DQ(m, u=*x; v=(I)*y; if(u==IMIN)oflo+=v; u=u-v; *z++=u; x++; y++; )
 else if(n-1<0)DQ(m, u=*x++; DQC(n, v=(I)*y; if(u==IMIN)oflo+=v; w=u-v; *z++=w; y++;))
 else      DQ(m, v=(I)*y++; if(v){DQ(n, u=*x; if(u==IMIN)oflo=1; u=u-1; *z++=u; x++;)}else{if(z!=x)MC(z,x,n<<LGSZI); z+=n; x+=n;})
 
 return oflo?EWOVIP+EWOVIPMINUSIB:EVOK;
}

// II multiply, in double precision.  Always return error code so we can clean up
 I tymesII(I n,I m,I* RESTRICTI x,I* RESTRICTI y,I* RESTRICTI z,J jt){DPMULDECLS I u;I v; if(jt->mulofloloc<0)return EWOVIP+EWOVIPMULII; I *zi=z;   // could use a side channel to avoid having main loop look at rc
 if(n-1==0)  DQ(m, u=*x; v=*y; DPMUL(u,v,z, goto oflo;) z++; x++; y++; )
 else if(n-1<0)DQ(m, u=*x; DQC(n, v=*y; DPMUL(u,v,z, goto oflo;) z++; y++;) x++;)
 else      DQ(m, v=*y; DQ(n, u=*x; DPMUL(u,v,z, goto oflo;) z++; x++;) y++;)
 jt->mulofloloc += z-zi; return EVOK;  // not yet for EVOKCLEANUP
oflo: *x=u; *y=v; jt->mulofloloc = ~(jt->mulofloloc + z-zi);return EWOVIP+EWOVIPMULII;  // back out the last store, in case it's in-place; gcc stores before overflow
}

// BI multiply, using clear/copy
 I tymesBI(I n,I m,B* RESTRICTI x,I* RESTRICTI y,I* RESTRICTI z,J jt){I v;
 if(n-1==0)  DQ(m, I u=*x; *z++=*y&-u; x++; y++; )
 else if(n-1<0){n=~n; DQ(m, B u=*x++; if(u){if(z!=y)MC(z,y,n<<LGSZI);}else{memset(z,0,n<<LGSZI);} z+=n; y+=n;)}
 else DQ(m, v=*y++; DQ(n, I u=*x; *z++=v&-u; x++;))
 return EVOK;
}

// IB multiply, using clear/copy
 I tymesIB(I n,I m,I* RESTRICTI x,B* RESTRICTI y,I* RESTRICTI z,J jt){I u;
 if(n-1==0)  DQ(m, I v=*y; *z++=*x&-v; x++; y++; )
 else if(n-1<0)DQ(m, u=*x++; DQC(n, I v=*y; *z++=u&-v; y++;))
 else DQ(m, B v=*y++; if(v){if(z!=x)MC(z,x,n<<LGSZI);}else{memset(z,0,n<<LGSZI);} z+=n; x+=n;)
 return EVOK;
}

// BD multiply, using clear/copy
 I tymesBD(I n,I m,B* RESTRICTI x,D* RESTRICTI y,D* RESTRICTI z,J jt){
 if(n-1==0)  DQ(m, D *yv=(D*)&dzero; yv=*x?y:yv; *z++=*yv; x++; y++; )
 else if(n-1<0){n=~n; DQ(m, B u=*x++; if(u){if(z!=y)MC(z,y,n*sizeof(D));}else{memset(z,0,n*sizeof(D));} z+=n; y+=n;)}
 else DQ(m, DQ(n, D *yv=(D*)&dzero; yv=*x?y:yv; *z++=*yv; x++;) ++y;)
 return EVOK;
}

// DB multiply, using clear/copy
 I tymesDB(I n,I m,D* RESTRICTI x,B* RESTRICTI y,D* RESTRICTI z,J jt){
 if(n-1==0)  DQ(m, D *yv=(D*)&dzero; yv=*y?x:yv; *z++=*yv; x++; y++; )
 else if(n-1<0)DQ(m, DQC(n, D *yv=(D*)&dzero; yv=*y?x:yv; *z++=*yv; y++;) ++x;)
 else DQ(m, B v=*y++; if(v){if(z!=x)MC(z,x,n*sizeof(D));}else{memset(z,0,n*sizeof(D));} z+=n; x+=n;)
 return EVOK;
}

// Overflow repair routines
// *x is a non-in-place argument, and n and m advance through it
//  each atom of *x is repeated n times
// *y is the I result of the operation that overflowed
// *z is the D result area (which might be the same as *y)
// b is unused for plus
 I plusIIO(I n,I m,I* RESTRICTI x,I* RESTRICTI y,D* RESTRICTI z,J jt){I u; I absn=n^REPSGN(n);
 DQ(m, u=*x++; DQ(absn, *z=(D)u + (D)(*y-u); ++y; ++z;));
 return EVOK;
}
 I plusBIO(I n,I m,B* RESTRICTI x,I* RESTRICTI y,D* RESTRICTI z,J jt){I u; I absn=n^REPSGN(n);
 DQ(m, u=(I)*x++; DQ(absn, *z=(D)u + (D)(*y-u); ++y; ++z;));
 return EVOK;
}

// For subtract repair, b is 1 if x was the subtrahend, 0 if the minuend
 I minusIIO(I n,I m,I* RESTRICTI x,I* RESTRICTI y,D* RESTRICTI z,J jt){I u; I absn=n^REPSGN(n);
 DQ(m, u=*x++; DQ(absn, *z=n<0?((D)(*y+u)-(D)u):((D)u - (D)(u-*y)); ++y; ++z;));
 return EVOK;
}
 I minusBIO(I n,I m,B* RESTRICTI x,I* RESTRICTI y,D* RESTRICTI z,J jt){I u; I absn=n^REPSGN(n);
 DQ(m, u=(I)*x++; DQ(absn, *z=n<0?((D)(*y+u)-(D)u):((D)u - (D)(u-*y)); ++y; ++z;));
 return EVOK;
}

// In multiply repair, z points to result, x and y to inputs
// Parts of z before mulofloloc have been filled in already
// We have to track the inputs just as for any other action routine
 I tymesIIO(I n,I m,I* RESTRICTI x,I* RESTRICTI y,D* RESTRICTI z,J jt){I u,v; I absn=n^REPSGN(n);
 // if all the multiplies are to be skipped, skip them quickly
 I skipct=jt->mulofloloc;
 if(skipct>=m*absn){skipct-=m*absn;
 }else{
  // There are unskipped multiplies.  Do them.
  if(n-1==0)  DQ(m, u=*x; v=*y; if(skipct){--skipct;}else{*z=(D)u * (D)v;} z++; x++; y++; )
  else if(n-1<0)DQ(m, u=*x++; DQC(n, v=*y; if(skipct){--skipct;}else{*z=(D)u * (D)v;} z++; y++;))
  else      DQ(m, v=*y++; DQ(n, u=*x; if(skipct){--skipct;}else{*z=(D)u * (D)v;} z++; x++;))
 }
 // Store the new skipct
 jt->mulofloloc=skipct;
 return EVOK;
}


// All these lines define functions for various operand combinations
// The comments indicate special cases that are defined by verbs that don't follow the
// AOVF/AIFX/ANAN etc. template


// These routines are used by sparse processing, which doesn't do in-place overflow
APFX( plusIO, D,I,I,  PLUSO,,return EVOK;)
APFX(minusIO, D,I,I, MINUSO,,return EVOK;)
APFX(tymesIO, D,I,I, TYMESO,,return EVOK;)

AIFX( plusBB, I,B,B, +     )    /* plusBI */                AIFX( plusBD, D,B,D, +   )
   /* plusIB */                 /* plusII */                AIFX( plusID, D,I,D, +   )
AIFX( plusDB, D,D,B, +     )  AIFX( plusDI, D,D,I, +)       /* plusDD */
APFX( plusZZ, Z,Z,Z, zplus,NAN0;,ASSERTWR(!NANTEST,EVNAN); return EVOK; )


AIFX(minusBB, I,B,B, -     )    /* minusBI */               AIFX(minusBD, D,B,D, -    )
  /* minusIB */                 /* minusII */               AIFX(minusID, D,I,D, -    )
AIFX(minusDB, D,D,B, -     )  AIFX(minusDI, D,D,I, -)       /* minusDD */
APFX(minusZZ, Z,Z,Z, zminus,NAN0;,ASSERTWR(!NANTEST,EVNAN); return EVOK;)

    /* andBB */                 /* tymesBI */                   /* tymesBD */            
    /* tymesIB */               /* tymesII */               APFX(tymesID, D,I,D, TYMESID,,return EVOK;)
    /* tymesDB */             APFX(tymesDI, D,D,I, TYMESDI,,return EVOK;)    /* tymesDD */
APFX(tymesZZ, Z,Z,Z, ztymes,NAN0;,ASSERTWR(!NANTEST,EVNAN); return EVOK; )

APFX(  divBB, D,B,B, DIVBB,,return EVOK;)   APFX(  divBI, D,B,I, DIVI,,return EVOK;)    APFX(  divBD, D,B,D, DIV,,return EVOK;)
APFX(  divIB, D,I,B, DIVI ,,return EVOK;)   APFX(  divII, D,I,I, DIVI,,return EVOK;)    APFX(  divID, D,I,D, DIV,,return EVOK;)
APFX(  divDB, D,D,B, DIVI ,,return EVOK;)   APFX(  divDI, D,D,I, DIVI,,return EVOK;)       /* divDD */
APFX(  divZZ, Z,Z,Z, zdiv,NAN0;,HDR1JERRNAN  )

     /* orBB */               APFX(  minBI, I,B,I, MIN,,return EVOK;)     APFX(  minBD, D,B,D, MIN,,return EVOK;)
APFX(  minIB, I,I,B, MIN,,return EVOK;)     /* minII */                   APFX(  minID, D,I,D, MIN,,return EVOK;)
APFX(  minDB, D,D,B, MIN,,return EVOK;)     APFX(  minDI, D,D,I, MIN,,return EVOK;)        /* minDD */
APFX(  minSS, SB,SB,SB, SBMIN,,return EVOK;)

    /* andBB */               APFX(  maxBI, I,B,I, MAX,,return EVOK;)     APFX(  maxBD, D,B,D, MAX,,return EVOK;)
APFX(  maxIB, I,I,B, MAX,,return EVOK;)     /* maxII */                   APFX(  maxID, D,I,D, MAX,,return EVOK;)
APFX(  maxDB, D,D,B, MAX,,return EVOK;)     APFX(  maxDI, D,D,I, MAX,,return EVOK;)         /* maxDD */
APFX(  maxSS, SB,SB,SB, SBMAX,,return EVOK;)

D jtremdd(J jt,D a,D b){D q,x,y;
 if(!a)return b;
 ASSERT(!INF(b),EVNAN);
 if(a==inf )return 0<=b?b:a;
 if(a==infm)return 0>=b?b:a;
 q=b/a; x=tfloor(q); y=tceil(q); return TEQ(x,y)?0:b-a*x;
}

APFX(remDD, D,D,D, remdd,,HDR1JERR)
APFX(remZZ, Z,Z,Z, zrem ,,HDR1JERR)

I jtremid(J jt,I a,D b){D r;I k;
 ASSERT(a&&-9e15<b&&b<9e15,EWOV);
 r=b-a*jfloor(b/a); k=(I)r;
 ASSERT(k==r,EWOV);   // not really overflow - just has a fractional part
 return k;
}

APFX(remID, I,I,D, remid,,HDR1JERR)

I remii(I a,I b){I r; return (a!=REPSGN(a))?(r=b%a,0<a?r+(a&REPSGN(r)):r+(a&REPSGN(-r))):a?0:b;}  // must handle IMIN/-1, which overflows.  If a=0, return b.

 I remII(I n,I m,I* RESTRICTI x,I* RESTRICTI y,I* RESTRICTI z,J jt){I u,v;
 if(n-1==0){DQ(m,*z++=remii(*x,*y); x++; y++; )
 }else if(n-1<0){   // repeated x.  Handle special cases and avoid integer divide
  DQ(m, u=*x++;
   if(0<=u&&!(u&(u-1))){--u; DQC(n, *z++=u&*y++;);}
   else DQC(n, *z++=remii( u,*y);      y++;)
  )
 }else      DQ(m, v=*y++; DQ(n, *z++=remii(*x, v); x++;     ));
 return EVOK;
}


static I igcd1(I a,I b){return a?igcd1(b%a,a):b;}  // Emulate Euclid

I jtigcd(J jt,I a,I b){
 if(a>IMIN&&b>IMIN){a=ABS(a); b=ABS(b);}
 else{
  if(a==b||!a||!b){jt->jerr=EWOV; return 0;}
  if(a==IMIN){b=ABS(b); a=-(a+b);}else{a=ABS(a); b=-(a+b);}
 }
 return a?igcd1(b%a,a):b;
}

D jtdgcd(J jt,D a,D b){D a1,b1,t;B stop = 0;
 a=ABS(a); b=ABS(b); if(a>b){t=a; a=b; b=t;}
 ASSERT(inf!=b,EVNAN);
 if(!a)return b;
 a1=a; b1=b;
 while(remdd(a1/jround(a1/a),b1)){t=a; if((a=remdd(a,b))==0)break; b=t;}  // avoid infinite loop if a goes to 0
 return a;
}    /* D.L. Forkes 1984; E.E. McDonnell 1992 */
I jtilcm(J jt,I a,I b){I z;I d;
 if(a&&b){RZ(d=igcd(a,b)); if(0==(z=jtmult(0,a,b/d)))jt->jerr=EWOV; return z;}else return 0;
}

#define GCDIO(u,v)      (dgcd((D)u,(D)v))
#define LCMIO(u,v)      (dlcm((D)u,(D)v))

D jtdlcm(J jt,D a,D b){ASSERT(!(INF(a)||INF(b)),EVNAN); return a&&b?a*(b/dgcd(a,b)):0;}

APFX(gcdIO, D,I,I, GCDIO,,HDR1JERR)
APFX(gcdII, I,I,I, igcd ,,HDR1JERR)
APFX(gcdDD, D,D,D, dgcd ,,HDR1JERR)
APFX(gcdZZ, Z,Z,Z, zgcd ,,HDR1JERR)

APFX(lcmII, I,I,I, ilcm ,,HDR1JERR)
APFX(lcmIO, D,I,I, LCMIO,,HDR1JERR)
APFX(lcmDD, D,D,D, dlcm ,,HDR1JERR)
APFX(lcmZZ, Z,Z,Z, zlcm ,,HDR1JERR)


#define GETD          {d=*wv++; if(!d){z=0; break;}}
#define INTDIVF(c,d)  (((c^d)>=0)?c/d:c%d?c/d-1:c/d)  // c/d - (c^d)<0 && c%d
#define INTDIVC(c,d)  (((c^d)<0)?c/d:c%d?c/d+1:c/d)   // c/d + (c^d)>=0 && c%d

 A jtintdiv(J jt,A a,A w){A z;B b,flr;I an,ar,*as,*av,c,d,j,k,m,n,p,p1,r,*s,wn,wr,*ws,*wv,*zv;
 an=AN(a); ar=AR(a); as=AS(a); av=AV(a);
 wn=AN(w); wr=AR(w); ws=AS(w); wv=AV(w); b=ar>=wr; r=b?wr:ar; s=b?as:ws;
 ASSERTAGREE(as,ws,r);
 if(an&&wn){PROD(m,r,s); PROD(n,b?ar-r:wr-r,r+s);}else m=n=0; 
 GATV(z,INT,b?an:wn,b?ar:wr,s); zv=AV(z);
 d=wn?*wv:0; p=0<d?d:-d; p1=d==IMIN?p:p-1; flr=XMFLR==jt->xmode;
 if(!wr&&p&&!(p&p1)){
  k=0; j=1; while(p>j){++k; j<<=1;}
  switch((0<d?0:2)+(flr?0:1)){
   case 0: DQ(n,          *zv++=*av++>>k;);                    break;
   case 1: DQ(n, c=*av++; *zv++=0< c?1+((c-1)>>k):(c+p1)>>k;); break;
   case 2: DQ(n, c=*av++; *zv++=c>IMIN?-c>>k:-(-c>>k););       break;
   case 3: DQ(n, c=*av++; *zv++=0<=c?-(c>>k):1+(-(1+c)>>k););
 }}else if(flr){
  if(1==n)    DQ(m, c=*av++; GETD;                *zv++=INTDIVF(c,d);  )
  else   if(b)DQ(m,          GETD; DQ(n, c=*av++; *zv++=INTDIVF(c,d););)
  else        DQ(m, c=*av++;       DQ(n, GETD;    *zv++=INTDIVF(c,d););)
 }else{
  if(1==n)    DQ(m, c=*av++; GETD;                *zv++=INTDIVC(c,d);  )
  else   if(b)DQ(m,          GETD; DQ(n, c=*av++; *zv++=INTDIVC(c,d););)
  else        DQ(m, c=*av++;       DQ(n, GETD;    *zv++=INTDIVC(c,d););)
 }
 return z?z:flr?floor1(divide(a,w)):ceil1(divide(a,w));
}    /* <.@% or >.@% on integers */


static A jtweight(J jt,A a,A w){ A z; return df1(z,behead(over(AR(w)?w:reshape(a,w),num(1))),bsdot(slash(ds(CSTAR))));}  // */\. }. (({:$a)$w),1

 A jtbase1(J jt, A w){A z;B*v;I c,m,n,p,r,*s,t,*x;
 n=AN(w); t=AT(w); r=AR(w); s=AS(w); c=AS(w)[r-1]; c=r?c:1;
 ASSERT(t&DENSE,EVNONCE);
 if(((c-BW)&SGNIF(t,B01X))>=0)return jtpdt(jt,w,weight(sc(c),t&RAT+XNUM?jtcvt(jt,XNUM,num(2)):num(2)));  //
 CPROD1(n,m,r-1,s);
 GATV(z,INT,m,r?r-1:0,s); x=AV(z); v=BAV(w);
 if(c)DQ(m, p=0; DQ(c, p=2*p+*v++;); *x++=p;)
 else memset(x,C0,m*SZI);
 return z;
}

 A jtbase2(J jt,A a,A w){I ar,*as,at,c,t,wr,*ws,wt;
 at=AT(a); ar=AR(a); as=AS(a);
 wt=AT(w); wr=AR(w); ws=AS(w); c=AS(w)[wr-1]; c=wr?c:1;
 ASSERT(!((at|wt)&SPARSE),EVNONCE); t=maxtyped(at,wt);
 if(!(t&at))RZ(a=jtcvt(jt,t,a));
 if(!(t&wt))RZ(w=jtcvt(jt,t,w));
 return 1>=ar?jtpdt(jt,w,weight(sc(c),a)):rank2ex(w,rank2ex(sc(c),a,UNUSED_VALUE,0L,MIN(ar,1),0L,MIN(ar,1),jtweight),UNUSED_VALUE,MIN(wr,1),1L,MIN(wr,1),1L,jtpdt);
}

// #: y
 A jtabase1(J jt, A w){A d,z;B*zv;I c,n,p,r,t,*v;UI x;
 // n = #atoms, r=rank, t=type
 n=AN(w); r=AR(w); t=AT(w);
 ASSERT(t&DENSE,EVNONCE);
 // Result has rank one more than the input.  If there are no atoms,
 // return (($w),0)($,)w; if Boolean, return (($w),1)($,)w
 if((-n&SGNIFNOT(t,B01X))>=0)return reshape(apip(shape(w),zeroionei(n!=0)),w);
 if(!(t&INT)){
  // Not integer.  Calculate # digits-1 as d = 2 <.@^. >./ | , w  
  df2(d,num(2),maximum(zeroionei(1),jtaslash(jt,CMAX,mag(ravel(w)))),jtatop(jt,ds(CFLOOR),ds(CLOG)));
  // Calculate z = ((1+d)$2) #: w
  RZ(z=jtabase2(jt,reshape(increm(d),num(2)),w));
  // If not float, result is exact or complex; either way, keep it
  if(!(t&FL))return z;
  // If float, see if we had one digit too many (could happen, if the log was too close to an integer)
  // calculate that as (0 = >./ ({."1 z)).  If so, return }."1 z ,  otherwise z
  // But we can't delete a digit if any of the values were negative - all are significant then
  // We also can't delete a digit if there is only 1 digit in the numbers
  if(AS(z)[AR(z)-1]<=1 || i0(jtaslash(jt,CPLUSDOT,ravel(lt(w,zeroionei(0))))))return z;
  if(0==i0(jtaslash(jt,CMAX,ravel(IRS1(z,0L,1L,jthead,d)))))return IRS1(z,0L,1L,jtbehead,d);
  return z;
 }
 // Integer.  Calculate x=max magnitude encountered (minimum of 1, to leave 1 output value)
 x=1; v=AV(w);
 DQ(n, p=*v++; x|=(UI)(p>0?p:-p););  // overflow happens on IMIN, no prob
 for(c=0;x;x>>=1){++c;}  // count # bits in result
 GATV(z,B01,n*c,1+r,AS(w)); AS(z)[r]=c;  // Allocate result area, install shape
 v=n+AV(w); zv=AN(z)+BAV(z);  // v->last input location (prebiased), zv->last result location (prebiased)
 DQ(n, x=*--v; DQ(c, *--zv=(B)(x&1); x>>=1;));  // copy in the bits, starting with the LSB
 return z;
}

 A jtabase2(J jt,A a,A w){A z;I an,ar,at,t,wn,wr,wt,zn;
 if(!(a && w)) return 0;
 an=AN(a); ar=AR(a); at=AT(a);
 wn=AN(w); wr=AR(w); wt=AT(w);
 ASSERT(!((at|wt)&SPARSE),EVNONCE);
 if(1>ar)return residue(a,w);
 if(1==ar&&!((at|wt)&(NOUN-(B01+INT)))){I*av,d,r,*u,*wv,x,*zv;
  // both types are int/boolean, and ar is a list
  {t=INT; if(!TYPESEQ(t,at)){RZ(a=jtcvt(jt,t,a));} if(!TYPESEQ(t,wt)){RZ(w=jtcvt(jt,t,w));}}  // convert args to compatible precisions, changing a and w if needed.  INT if both empty
  // If a ends with _1 followed by any number of 1, there will be overflow if w contains any imin.  Detect that very rare case
  av=an+AV(a); wv=wn+AV(w);
  for(zv=av, d=an;d&&*--zv==1;--d);
  if(d&&*zv==-1){zv=wv; DQ(wn, if(*--zv==IMIN){d=0; break;}) if(!d){RZ(a=jtcvt(jt,FL,a)); return jtabase2(jt,a,w);}}
  DPMULDE(an,wn,zn); GATV(z,INT,zn,1+wr,AS(w)); AS(z)[wr]=an;  // allocate result area
  zv=zn+AV(z);
  if((((2^an)-1)&(av[-2]-1)&-(d=av[-1]))<0){I d1,k;
   // Special case: a is (0,d) where d is positive
   if(d&(d1=d-1)){I q,r,xs;
    // d is not a power of 2
    DQ(wn, x=*--wv; xs=REPSGN(x); q=(x-xs)/d+xs; r=x-q*d; *--zv=r; *--zv=q;)  // remainder has same sign as dividend.  If neg, add 1, divide, sub 1 from quotient; then make remainder right
   }else{
    // d is a power of 2
    k=CTTZ(d);  // k = #zeros below the 1 in d
    DQ(wn, x=*--wv; *--zv=x&d1; *--zv=x>>k;)
   }
  }else DQ(wn, x=*--wv; u=av; DQ(an, d=*--u; *--zv=r=remii(d,x); x=d?(x-r)/d:0;););
  return z;
 }
 {PROLOG(0070);A y,*zv;C*u,*yv;I k;
  F2RANK(1,0,jtabase2,UNUSED_VALUE);
  k=bpnoun(at); u=an*k+CAV(a);
  GA(y,at, 1, 0,0); yv=CAV(y);
  GATV0(z,BOX,an,1); zv=an+AAV(z);
  DQ(an, MC(yv,u-=k,k); A tt; RZ(w=divide(minus(w,tt=residue(y,w)),y)); INCORP(tt); *--zv=tt;);
  z=ope(z);
  EPILOG(z);
}}

// Compute power-of-2 | w for INT w, by ANDing.  Result is boolean if mod is 1 or 2
A jtintmod2(J jt,A w,I mod){A z;B *v;I n,q,r,*u;UI m=0;  // init m for warning
 F1PREFIP;
 if(mod>2)return jtatomic2(jtinplace,sc(mod-1),w,ds(CBW0001));  // INT result, by AND
 // the rest is boolean result
 n=AN(w); v=BAV(w);  // littleendian only
 GATV(z,B01,n,AR(w),AS(w)); RZ(n);  // loops below can't handle empty
 u=AV(z); q=(n-1)>>(LGSZI); r=((n-1)&(SZI-1))+1;   // there is always a remnant
 I mask=mod==2?VALIDBOOLEAN:0;  // if mod is 1, all results will be 0; otherwise boolean result
 DQ(q, DQ(SZI, m=(m>>8)+((UI)*v<<((SZI-1)*8)); v+=SZI;); *u++=m&mask;)
 DQ(r, m=(m>>8)+((UI)*v<<((SZI-1)*8)); v+=SZI;);  // 1-8 bytes
 STOREBYTES(v,m&mask,8-r);
 return z;
}
