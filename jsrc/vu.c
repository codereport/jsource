/* Copyright 1990-2014, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Unicode (2-byte unsigned characters)                                    */

#include "j.h"


B jtvc1(J jt,I n,US*v){DQ(n, RZ(255>=*v++);); return 1;}
     /* verify that 2-byte chars have high-order 0 bytes */

// allocate new datablock and return the ASCII for the characters in w
// if b is 0, raise error if high byte of unicode is not 0
A jttoc1(J jt,B h,A w){A z;C*wv,*zv;I n;C4*w4;
 if(LIT&AT(w))RCA(w);  // if already ASCII, return
 n=AN(w); wv=CAV(w);    // number of characters, pointer to characters if any
 w4=C4AV(w);
 ASSERT(!n||(C2T+C4T)&AT(w),EVDOMAIN);  // must be empty or unicode
 GATV(z,LIT,n,AR(w),AS(w)); zv=CAV(z);  // allocate ASCII area with same data shape
 if(C2T&AT(w))
 {
 if(h)DQ(n, *zv++=*wv++; wv++;) else DQ(n, *zv++=*wv++; ASSERT(!*wv++,EVDOMAIN);)
 }
 else
 {
 if(h)DQ(n, *zv++=(UC)*w4++; ) else DQ(n, *zv++=(UC)*w4++; ASSERT(*(w4-1)<256UL,EVDOMAIN);)
 }
 // copy the low byte of the data (if there is any).  if b==0, verify high byte is 0
 // where low and high are depends on endianness
 RETF(z);
}    /* convert 2-byte or 4-byte chars to 1-byte chars; 0==h iff high order byte(s) must be 0 */

static F1(jttoc2){A z;C*wv,*zv;I n;C4*w4;US*z2;
 if(C2T&AT(w))RCA(w);
 n=AN(w); wv=CAV(w); w4=C4AV(w);
 ASSERT(!n||(LIT+C4T)&AT(w),EVDOMAIN);
 GATV(z,C2T,n,AR(w),AS(w)); zv=CAV(z); z2=USAV(z);
 if(LIT&AT(w))
 {
 DQ(n, *zv++=*wv++; *zv++=0;);
 }
 else
 {
 DQ(n, *z2++=(US)*w4++;);
 }
 return z;
}    /* convert 1-byte chars or 4-byte chars(discard high order half) to 2-byte chars */

static F1(jttoc2e){A z;I m,n,r;
 n=AN(w); r=AR(w);
 ASSERT(r!=0,EVRANK);
 ASSERT(!n||LIT&AT(w),EVDOMAIN);
 m=AS(w)[r-1];
 ASSERT(0==(m&1),EVLENGTH);
 GATV(z,C2T,n>>1,r,AS(w)); AS(z)[r-1]=m>>1;
 MC(AV(z),AV(w),n);
 RETF(z);
}    /* convert pairs of 1-byte chars to 2-byte chars */

// extended to C4
static F1(jtifc2){A z;I n,t,*zv;
 n=AN(w); t=AT(w);
 ASSERT(((n-1)&((t&JCHAR)-1))>=0,EVDOMAIN);
 GATV(z,INT,n,AR(w),AS(w)); zv=AV(z);
 if(t&LIT){UC*v=UAV(w); DQ(n, *zv++=*v++;);}
 else if(t&C2T){US*v=USAV(w); DQ(n, *zv++=*v++;);}
 else          {C4*v=C4AV(w); DQ(n, *zv++=*v++;);}
 RETF(z);
}    /* integers from 1- or 2-byte or 4-byte chars */

static F1(jtc2fi){A z;I j,n,*v;US*zv;
 RZ(w=vi(w));
 n=AN(w); v=AV(w);
 GATV(z,C2T,n,AR(w),AS(w)); zv=USAV(z);
 DQ(n, j=*v++; ASSERT(BETWEENC(j,SMIN,SMAX),EVINDEX); *zv++=(US)j;);
 RETF(z);
}    /* 2-byte chars from integers */

F1(jtuco1){I t;
 t=AT(w);
 ASSERT(!AN(w)||t&JCHAR+NUMERIC,EVDOMAIN);
 return t&NUMERIC?c2fi(w):t&C2T?w:toc2(w);  // was ca(w)
}    /* return 2-byte chars unchanged; convert 1-byte or 4-byte to 2-byte */

F2(jtuco2){I j;
 RE(j=i0(a));
 switch(j){
  case 1: return toc1(1,w);
  case 2: return toc2(w);
  case 3: return ifc2(w);
  case 4: return c2fi(w);
  case 5: return toc1(0,w);
  case 6: return toc2e(w);
  case 7: return toutf16(w);
  case 8: return toutf8(w);
  case 9: return toutf32(w);
  case 10: return tou32(w);
  default: ASSERT(0,EVDOMAIN);
}}
