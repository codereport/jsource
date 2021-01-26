/* Copyright 1990-2011, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Verbs: Random Numbers                                                   */

#include "j.h"

#define NRNG        5     /* # of available RNGs (excluding 0)             */
#define SMI         0     /* sum of all RNGs                               */
#define GBI         1     /* gb_flip, Knuth 1994                           */
#define MTI         2     /* Mersenne Twister, Matsumoto & Nishimura, 2002 */
#define DXI         3     /* DX-1597-4d, Deng, 2005                        */
#define MRI         4     /* MRG32k3a, L'Ecuyer, 1999                      */

#define x31         ((UI)0x80000000)
#define x63         ((UI)0x8000000000000000)
#define X32         2.32830643653869628906e-10
#define X52         2.22044604925031308085e-16
#define X64         5.42101086242752217004e-20

#define NEXT        (jt->rngf(jt))

#define INITD       {sh=mk=1;}
#define NEXTD1      ((0.5+X52/2)+X64*(I)(NEXT&(UI)0xfffffffffffff000))
#define NEXTD0      NEXTD1

/* m<x, greatest multiple of m less than x */
#define GMOF(m,x)   (((UI)0xffffffffffffffffLL/(m))*(m))
#define GMOF2(m,x,z,zq)  (zq=(UI)0xffffffffffffffffLL/(m), z=zq*(m))
#define GMOTHRESH (UI)0xf000000000000000

/* ----------------------------------------------------------------------- */
/* linear congruential generator                                           */
/* Lehmer, D.H., Proceedings, 2nd Symposium on Large-Scale Digital         */
/* Calculating Machinery, Harvard University Press, 1951, pp. 141-146.     */

static void lcg(I n,I*v,I seed){D c=16807.0,p=2147483647.0,x=(D)seed;
 DQ(n, x*=c; x-=p*(I)(x/p); *v++=(I)x;);
}

F1(jtlcg_test){A x;I n=1597,*v;
 ASSERTMTV(w);
 GATV0(x,INT,n,1); v=AV(x);
 lcg(n,v,1L);
 ASSERTSYS(v[   0]==     16807L, "lcg_test 0");
 ASSERTSYS(v[   1]== 282475249L, "lcg_test 1");
 ASSERTSYS(v[   2]==1622650073L, "lcg_test 2");
 ASSERTSYS(v[   3]== 984943658L, "lcg_test 3");
 ASSERTSYS(v[   4]==1144108930L, "lcg_test 4");
 ASSERTSYS(v[1592]==1476003502L, "lcg_test 1592");
 ASSERTSYS(v[1593]==1607251617L, "lcg_test 1593");
 ASSERTSYS(v[1594]==2028614953L, "lcg_test 1594");
 ASSERTSYS(v[1595]==1481135299L, "lcg_test 1595");
 ASSERTSYS(v[1596]==1958017916L, "lcg_test 1596");
 R num(1);
}


/* ----------------------------------------------------------------------- */
/* gb_flip routines from D.E. Knuth's "The Stanford GraphBase"             */

#define GBN         56

#define mod_diff(x,y)       (((x)-(y))&0x7fffffffL) /* difference modulo 2^31 */
#define two_to_the_31       (0x80000000L)

static UI jtgb_flip_cycle(J jt){I*A=(I*)jt->rngv;register I*i,*j;
 for(i=&A[1],j=&A[32];j<=&A[55];i++,j++)*i=mod_diff(*i,*j);
 for(        j=&A[1 ];i<=&A[55];i++,j++)*i=mod_diff(*i,*j);
 jt->rngi=54;
 R (UI)A[55];
}

static UI jtgb_next(J jt){UI a,b,c;
 a= jt->rngi ? jt->rngv[jt->rngi--] : gb_flip_cycle();
 b= jt->rngi ? jt->rngv[jt->rngi--] : gb_flip_cycle();
 c= jt->rngi ? jt->rngv[jt->rngi--] : gb_flip_cycle();
 R a+(b<<31)+(c<<33&0xc000000000000000UL);
}

static void jtgb_init(J jt,UI s){I*A;register I i,next=1,prev,seed;
 A=(I*)jt->rngv; next=1; prev=seed=(I)s;
 seed=prev=mod_diff(prev,0); /* strip off the sign */
 A[0]=0; A[55]=prev;
 for (i=21; i; i=(i+21)%55) {
  A[i]=next;
  next=mod_diff(prev,next);
  if(seed&1)seed=0x40000000+(seed>>1); else seed>>=1; /* cyclic shift right 1 */
  next=mod_diff(next,seed);    
  prev=A[i];
 }
 gb_flip_cycle();
 gb_flip_cycle();
 gb_flip_cycle();
 gb_flip_cycle();
 gb_flip_cycle();
 jt->rngi=54;
}

static I jtgb_unif_rand(J jt,I m){
 register UI r,t=two_to_the_31-(two_to_the_31 % m);
 do r=gb_next(); while(t<=r);
 R r%m;
}

F1(jtgb_test){I j=jt->rng;
 ASSERTMTV(w);
 RZ(rngselects(sc(GBI)));
 gb_init(-314159);
 ASSERTSYS(gb_next()==119318998,"gb_test 0");
 DQ(133, gb_next(););
 ASSERTSYS(gb_unif_rand(0x55555555L)==748103812,"gb_test 1");
 RZ(rngselects(sc(j)));
 R num(1);
}


/* ----------------------------------------------------------------------- */
/* 
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)  
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

/* Period parameters */

#define MTN         312
#define MTM         156
#define MATRIX_A    0xB5026F5AA96619E9ULL
#define UM          0xFFFFFFFF80000000ULL /* Most significant 33 bits */
#define LM          0x7fffffffUL    /* least significant r bits  */

/* initializes mt[MTN] with a seed */
static void jtmt_init(J jt,UI s){I i;UI*mt=jt->rngv;
 mt[0]= s;
 for (i=1; i<MTN; i++)
  mt[i] = (6364136223846793005ULL * (mt[i-1] ^ (mt[i-1] >> 62)) + i);
  /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
  /* In the previous versions, MSBs of the seed affect   */
  /* only MSBs of the array mt[].                        */
  /* 2002/01/09 modified by Makoto Matsumoto             */
 jt->rngi=MTN;
} 

static void jtmt_init_by_array(J jt,UI init_key[], I key_length){I i,j,k;UI*mt=jt->rngv;
 mt_init((UI)19650218);
 i=1; j=0; k=MTN>key_length?MTN:key_length;
 for(; k; k--) {
  mt[i] = (mt[i] ^ (mt[i-1]^mt[i-1]>>62)*3935559000370003845ULL)+init_key[j]+j; /* non linear */
  i++; j++;
  if(i>=MTN){mt[0]=mt[MTN-1]; i=1;}
  if(j>=key_length)j=0;
 }
 for (k=MTN-1; k; k--) {
  mt[i] = (mt[i] ^ (mt[i-1]^mt[i-1]>>62)*2862933555777941757ULL)-i; /* non linear */
  i++;
  if (i>=MTN) { mt[0] = mt[MTN-1]; i=1; }
 }
 mt[0] = 1ULL << 63;   /* MSB is 1; assuring non-zero initial array */
}

/* generates a random 32-or 64-bit number */
static UI jtmt_next(J jt){UI*mt=jt->rngv,*u,*v,*w,y;
 if (MTN<=jt->rngi) {  /* generate MTN words at one time */
  v=1+mt; w=MTM+mt; 
  u=mt; DQ(MTN-MTM, y=(*u&UM)|(*v++&LM); *u++=*w++^(y>>1)^MATRIX_A*(y&0x1UL););
  w=mt; DQ(MTM-1,   y=(*u&UM)|(*v++&LM); *u++=*w++^(y>>1)^MATRIX_A*(y&0x1UL););
  v=mt;             y=(*u&UM)|(*v++&LM); *u++=*w++^(y>>1)^MATRIX_A*(y&0x1UL);
  jt->rngi=0;
 }
 y = mt[jt->rngi++];
 y ^= (y >> 29) & 0x5555555555555555ULL;
 y ^= (y << 17) & 0x71D67FFFEDA60000ULL;
 y ^= (y << 37) & 0xFFF7EEE000000000ULL;
 y ^= (y >> 43);
 R y;
}

F1(jtmt_test){I j=jt->rng;UI init[4]={0x12345ULL, 0x23456ULL, 0x34567ULL, 0x45678ULL},x;
 ASSERTMTV(w);
 RZ(rngselects(sc(MTI)));
 mt_init_by_array(init,(I)4);
 x=mt_next();
 ASSERTSYS(x==7266447313870364031ULL, "mt_test64 0");
 DQ(998, mt_next(););
 x=mt_next();
 ASSERTSYS(x== 994412663058993407ULL, "mt_test64 1");
 RZ(rngselects(sc(j)));
 R num(1);
}

/* ----------------------------------------------------------------------- */
/* DX-1597-4d                                                              */
/* Deng, L.Y., Efficient and Portable Multiple Recursive Generators of     */
/* Large Order, ACM Transactions on Modelling and Computer Simulations 15, */
/* 2005, pp. 1-13.                                                         */

#define DXM  2147483647UL
#define DXB  1073741362UL
#define DXBD 1073741362.0
#define DXN  1597

static UI jtdx_next30(J jt){I j;UI*u,*v,*vv,r,x;
 j=jt->rngi; v=vv=j+jt->rngv; u=DXN+jt->rngv;
 r =*v; v+=532; if(v>=u)v-=DXN;
 r+=*v; v+=532; if(v>=u)v-=DXN; 
 r+=*v; v+=532; if(v>=u)v-=DXN;
 r+=*v;
 *vv=x=(r*DXB)%DXM;
 ++j; jt->rngi=j==DXN?0:j;
 R x;
}

static UI jtdx_next(J jt){UI a,b,c;
 a=dx_next30()&0x000000003fffffff;
 b=dx_next30()&0x000000003fffffff;;
 c=dx_next30()&0x000000003fffffff;;
 R a|b<<30|c<<34&0xf000000000000000UL;
}

static void jtdx_init(J jt,UI s){lcg(DXN,jt->rngv,s); jt->rngi=0;} 

F1(jtdx_test){I j=jt->rng,x;
 ASSERTMTV(w);
 RZ(rngselects(sc(DXI))); dx_init(1UL); 
 x=dx_next(); ASSERTSYS(x== 221240004UL, "dx_test 0");
 x=dx_next(); ASSERTSYS(x==2109349384UL, "dx_test 1");
 x=dx_next(); ASSERTSYS(x== 527768079UL, "dx_test 2");
 x=dx_next(); ASSERTSYS(x== 238300266UL, "dx_test 3");
 x=dx_next(); ASSERTSYS(x==1495348915UL, "dx_test 4");
 RZ(rngselects(sc(j)));
 R num(1);
}


/* ----------------------------------------------------------------------- */
/* MRG32k3a                                                                */
/* L'Ecuyer, P., Good parameters and implementation for combined multiple  */
/* recursive random number generators, Operations Research 47, 1999,       */
/* pp. 159-164.                                                            */

#define MRN  6
#define MRM0 4294967087UL  /*   _209+2^32 */
#define MRM1 4294944443UL  /* _22853+2^32 */

static UI jtmr_next31(J jt){I d,j,*v=jt->rngv,x,y;
 switch(j=jt->rngi){
  case 0: x=1403580*v[1]-810728*v[0]; y=527612*v[5]-1370589*v[3]; jt->rngi=1; break;
  case 1: x=1403580*v[2]-810728*v[1]; y=527612*v[3]-1370589*v[4]; jt->rngi=2; break;
  case 2: x=1403580*v[0]-810728*v[2]; y=527612*v[4]-1370589*v[5]; jt->rngi=0;
 }
 x%=MRM0; if(x<0)x+=MRM0; v[j  ]=x;
 y%=MRM1; if(y<0)y+=MRM1; v[j+3]=y;
 d=(x-y)%MRM0; if(d<0)d+=MRM0;
 R d;
}

static UI jtmr_next(J jt){UI a,b,c;
 a=mr_next31()&0x000000007fffffff;
 b=mr_next31()&0x000000007fffffff;
 c=mr_next31()&0x000000007fffffff;
 R a+(b<<31)+(c<<33&0xc000000000000000UL);
}



static void jtmr_init(J jt,UI s){D*v=(D*)jt->rngv;I t[MRN];
 lcg(MRN,t,s);
 DO(MRN, *v++=(D)t[i];);
 jt->rngi=0;
} 

F1(jtmr_test){I j=jt->rng,x;
 ASSERTMTV(w);
 RZ(rngselects(sc(MRI))); mr_init(1UL);
 x=mr_next(); ASSERTSYS(x==(I)3293966663UL, "mr_test 0");
 x=mr_next(); ASSERTSYS(x==(I)3129388991UL, "mr_test 1");
 x=mr_next(); ASSERTSYS(x==(I)2530141948UL, "mr_test 2");
 x=mr_next(); ASSERTSYS(x==(I)1065433470UL, "mr_test 3");
 x=mr_next(); ASSERTSYS(x==(I)1177634463UL, "mr_test 4");
 DQ(40, mr_next(););
 x=mr_next(); ASSERTSYS(x==(I)1134399356UL, "mr_test 45");
 x=mr_next(); ASSERTSYS(x==(I) 630832201UL, "mr_test 46");
 x=mr_next(); ASSERTSYS(x==(I)2411464992UL, "mr_test 47");
 x=mr_next(); ASSERTSYS(x==(I) 762439568UL, "mr_test 48");
 x=mr_next(); ASSERTSYS(x==(I)3245142153UL, "mr_test 49");
 RZ(rngselects(sc(j)));
 R num(1);
}


/* ----------------------------------------------------------------------- */
/* sum of all RNGs                                                         */

static UI jtsm_next(J jt){UI x;
 jt->rngi=jt->rngI0[GBI]; jt->rngv=jt->rngV0[GBI]; x =gb_next(); jt->rngI0[GBI]=jt->rngi;
 jt->rngi=jt->rngI0[MTI]; jt->rngv=jt->rngV0[MTI]; x+=mt_next(); jt->rngI0[MTI]=jt->rngi; 
 jt->rngi=jt->rngI0[DXI]; jt->rngv=jt->rngV0[DXI]; x+=dx_next(); jt->rngI0[DXI]=jt->rngi;
 jt->rngi=jt->rngI0[MRI]; jt->rngv=jt->rngV0[MRI]; x+=mr_next(); jt->rngI0[MRI]=jt->rngi;
 R x;
}

static void jtsm_init(J jt,UI s){
 jt->rngv=jt->rngV0[GBI]; gb_init(s); jt->rngI0[GBI]=jt->rngi;
 jt->rngv=jt->rngV0[MTI]; mt_init(s); jt->rngI0[MTI]=jt->rngi;
 jt->rngv=jt->rngV0[DXI]; dx_init(s); jt->rngI0[DXI]=jt->rngi;
 jt->rngv=jt->rngV0[MRI]; mr_init(s); jt->rngI0[MRI]=jt->rngi;
}

/* ----------------------------------------------------------------------- */

F1(jtrngraw){A z;I n,*v;
 RE(n=i0(w));
 ASSERT(0<=n,EVDOMAIN);
 GATV0(z,INT,n,1); v=AV(z);
 DQ(n, *v++=NEXT;);
 R z;
}

B jtrnginit(J jt){
 DO(NRNG, jt->rngV[i]=jt->rngV0[i]=0;);
 jt->rngF[0]=jtsm_next; jt->rngS[0]=16807;
 jt->rngF[1]=jtgb_next; jt->rngS[1]=16807;
 jt->rngF[2]=jtmt_next; jt->rngS[2]=16807;
 jt->rngF[3]=jtdx_next; jt->rngS[3]=16807;
 jt->rngF[4]=jtmr_next; jt->rngS[4]=16807;
 jt->rngM[0]=0;             /*   %      2^32 */
 jt->rngM[1]=0;  /*   %      2^31 */
 jt->rngM[2]=0;                     /*   %      2^32 */
 jt->rngM[3]=0;  /*   %   _1+2^31 */  /* fudge; should be _1+2^31 */
 jt->rngM[4]=0;  /*   % _209+2^32 */
 jt->rngI0[GBI]=54;
 rngselects(num(2)); 
 R 1;
}

F1(jtrngselectq){ASSERTMTV(w); R sc(jt->rng);}

static B jtrngga(J jt,I i,UI**vv){
 if(vv[i]){jt->rngv=vv[i]; jt->rngi=jt->rngI[i];}
 else{A x;I n,t;void(*f)(); 
  switch(i){
   case GBI: t=INT; n=GBN; f=jtgb_init; break;
   case MTI: t=INT; n=MTN; f=jtmt_init; break;
   case DXI: t=INT; n=DXN; f=jtdx_init; break;
   case MRI: t=FL;  n=MRN; f=jtmr_init; 
  }
  GA(x,t,n,1,0); ras(x); vv[i]=jt->rngv=AV(x);   // x will never be freed, but that's OK, it's inited only once
  f(jt,jt->rngS[i]); jt->rngI[i]=jt->rngi;
 }
 R 1;
}

F1(jtrngselects){I i;UI**vv=jt->rngV;
 RE(i=i0(w));
 ASSERT(BETWEENO(i,0,NRNG),EVDOMAIN);
 jt->rngI[jt->rng]=jt->rngi;
 switch(jt->rng=i){
  case SMI: vv=jt->rngV0;      jt->rngw=64;
            RZ(rngga(GBI,vv)); RZ(rngga(MTI,vv)); RZ(rngga(DXI,vv)); RZ(rngga(MRI,vv)); break;
  case GBI: RZ(rngga(i,  vv)); jt->rngw=64; break;
  case MTI: RZ(rngga(i,  vv)); jt->rngw=64; break; 
  case DXI: RZ(rngga(i,  vv)); jt->rngw=64; break;
  case MRI: RZ(rngga(i,  vv)); jt->rngw=64; 
 }
 jt->rngf=jt->rngF[jt->rng];
 R mtv;
}

F1(jtrngstateq){A x=0,z,*zv;D*u=0;I n;UI*v;
 ASSERTMTV(w);
 switch(jt->rng){
  case SMI: 
   GAT0(z,BOX,9,1); zv=AAV(z);
   RZ(*zv++=num(0));
   RZ(*zv++=incorp(sc(jt->rngI0[GBI]))); RZ(*zv++=incorp(vec(INT,GBN,jt->rngV0[GBI])));
   RZ(*zv++=incorp(sc(jt->rngI0[MTI]))); RZ(*zv++=incorp(vec(INT,MTN,jt->rngV0[MTI])));
   RZ(*zv++=incorp(sc(jt->rngI0[DXI]))); RZ(*zv++=incorp(vec(INT,DXN,jt->rngV0[DXI])));
   RZ(*zv++=incorp(sc(jt->rngI0[MRI]))); RZ(*zv++=incorp(vec(INT,MRN,jt->rngV0[MRI])));
   R z;
  case GBI: n=GBN; v=jt->rngv; break;
  case MTI: n=MTN; v=jt->rngv; break;
  case DXI: n=DXN; v=jt->rngv; break;
  case MRI: n=MRN; v=jt->rngv; break;
 }
 GAT0(z,BOX,3,1); zv=AAV(z);
 RZ(*zv++=incorp(sc(jt->rng))); RZ(*zv++=incorp(sc(jt->rngi))); RZ(*zv++=incorp(vec(INT,n,v)));
 R z;
}

static B jtrngstates1(J jt,I j,I n,UI**vv,I i,I k,A x,B p){D*u;UI*xv;
 RZ(x=vi(x)); xv=AV(x);
 ASSERT(1==AR(x),EVRANK);
 ASSERT(n==AN(x),EVLENGTH); 
 ASSERT(i<=k&&k<n+(I )(j==MTI),EVINDEX); 
 if(p)DO(n, ASSERT(x31>xv[i],EVDOMAIN););
 ICPY(vv[j],xv,n);
 jt->rngi=k;
 R 1;
}

F1(jtrngstates){A*wv;I k;UI**vv=jt->rngV;
 ARGCHK1(w);
 ASSERT(1==AR(w),EVRANK);
 ASSERT(BOX&AT(w),EVDOMAIN);
 ASSERT(2<=AN(w),EVLENGTH);
 wv=AAV(w); 
 RZ(rngselects(wv[0]));  /* changes jt->rng */
 ASSERT(AN(w)==(jt->rng?3:9),EVLENGTH);
 switch(jt->rng){
  case SMI: vv=jt->rngV0;
            RE(k=i0(wv[1])); RZ(rngstates1(GBI,GBN,vv,0,k,wv[2],1)); jt->rngI0[GBI]=k;  // We accept 0-55 even though we never produce 55 ourselves
            RE(k=i0(wv[3])); RZ(rngstates1(MTI,MTN,vv,0,k,wv[4],0)); jt->rngI0[MTI]=k;
            RE(k=i0(wv[5])); RZ(rngstates1(DXI,DXN,vv,0,k,wv[6],1)); jt->rngI0[DXI]=k;
            RE(k=i0(wv[7])); RZ(rngstates1(MRI,MRN,vv,0,k,wv[8],0)); jt->rngI0[MRI]=k;
            break;
  case GBI: RE(k=i0(wv[1])); RZ(rngstates1(GBI,GBN,vv,0,k,wv[2],1)); break;  // We accept 0-55 even though we never produce 55 ourselves
  case MTI: RE(k=i0(wv[1])); RZ(rngstates1(MTI,MTN,vv,0,k,wv[2],0)); break;
  case DXI: RE(k=i0(wv[1])); RZ(rngstates1(DXI,DXN,vv,0,k,wv[2],1)); break;
  case MRI: RE(k=i0(wv[1])); RZ(rngstates1(MRI,MRN,vv,0,k,wv[2],0));
 }
 R mtv;
}

// Return the seed info.  This is a scalar jt->rngS[jt->rng] unless the generator is Mersenne Twister and
// jt->rngseed is set, in which case jt->rngseed is the vector of seed info
F1(jtrngseedq){ASSERTMTV(w); R jt->rngseed&&MTI==jt->rng?jt->rngseed:sc(jt->rngS[jt->rng]);}

// Set the vector of RNG seed info
F1(jtrngseeds){I k,r; 
 // Force w to integer; k=first value; r=rank
 RZ(w=vi(w)); k=AV(w)[0]; r=AR(w);
 if(r){
  // w is not an atom.  the RNG had better be Mersenne Twister.  Initialize using w, and save the w list
  ASSERT(1==r&&MTI==jt->rng,EVRANK);
  RZ(ras(w)); fa(jt->rngseed); jt->rngseed=w;   // note ra before fa, in case same buffers
  mt_init_by_array(AV(w),AN(w));
 }else switch(jt->rng){
  // atomic w.  We can use that for any generator.  Choose the current one.
  case SMI: ASSERT(k!=0,EVDOMAIN); sm_init(k);     break;
  case GBI:                     gb_init(k);     break;
  case MTI:                     mt_init((UI)k); break;
  case DXI: ASSERT(k!=0,EVDOMAIN); dx_init(k);     break;
  case MRI: ASSERT(k!=0,EVDOMAIN); mr_init(k);
 }
 jt->rngS[jt->rng]=k;  // Save first value, in case k is atomic
 if(!r&&MTI==jt->rng&&jt->rngseed){fa(jt->rngseed); jt->rngseed=0;}   // If k is atomic, discard jt->rngseed if there is one
 // Now jt->rngseed is set iff the w for Mersenne Twister was a list.  jt->rngS[jt->rng] is always set.
 R mtv;
}


static F2(jtrollksub){A z;I an,*av,k,m1,n,p,q,r,sh;UI m,mk,s,t,*u,x=jt->rngM[jt->rng];
 ARGCHK2(a,w);
 an=AN(a); RE(m1=i0(w)); ASSERT(0<=m1,EVDOMAIN); m=m1;
 RZ(a=vip(a)); av=AV(a); PRODX(n,an,av,1);
 GA(z,0==m?FL:2==m?B01:INT,n,an,av); u=(UI*)AV(z);
 if(!m){D*v=DAV(z); INITD; if(sh)DQ(n, *v++=NEXTD1;)else DQ(n, *v++=NEXTD0;);}  // floating-point output
 else if(2==m){I nslice; I j;
  // binary output
  p = (BW/8) * (nslice = (8 - (BW-jt->rngw)));  // #bits/slice, times number of slices
  // See how many p-size blocks we can have, and how many single leftovers
  q=n/p; r=n%p;   // q=# p-size blocks, r=#single-bit leftovers
  mk=(UI)0x0101010101010101;
  // Loop to output all the p-size blocks
  for(j=0;j<q;++j){
   t=NEXT;
   DQ(nslice, *u++=mk&t; t>>=1;);
  }
  // Get a random # for finishing slices, & out them
  t=NEXT;  // Get random # for slices
  DQ(r>>LGSZI, *u++=mk&t; t>>=1;);
  // Output the rest, one bit at a time
  t=NEXT;  // Get random # for bits
  B*c=(B*)u; DQ(r&(SZI-1), *c++=1&t; t>>=1;);
 }else{
  // integer output
  r=n; s=GMOF(m,x); if(s==x)s=0;
  if(m>1&&!(m&(m-1))){
   if(s==0)s=0-m;  // since we reject t>=s, we must make s less than IMAX.  This is the max possible multiple of s.  We don't check for s=0 in this path.  s==0 possible only in 32-bit
   // here if w is a power of 2, >2; take bits from each value.  s cannot be 0
   k=CTTZI(m);  // lg(m)
   p=jt->rngw/k; mk=m-1;  // p=#results per random number; r is number of values left after bit processing
   r-=p; while(r>=0){do{t=NEXT;}while(s<=t); DQU(p, *u++=mk&t; t>>=k;) r-=p;}  // deal p at a time till we are as close to n as we can get
   r+=p;  // rebias to get # values still needed
  }
  if(BW==64&&m<(1LL<<50)){ 
   // If we can do the calculation in the floating-point unit, do
   D md=m*X64; DQ(r, *u++=(I)(md*((D)(I)NEXT+(D)x63)); )   // avoid unsigned conversion, which requires conditional correction
  }else{
   if(r&&s)DQ(r, while(s<=(t=NEXT)); *u++=t%m;) else DQ(r, *u++=NEXT%m;);
  }
 }
 R z;
}

DF2(jtrollk){A g,z;V*sv;
 ARGCHK3(a,w,self);
 sv=FAV(self); g=sv->fgh[2]?sv->fgh[2]:sv->fgh[1];
 if(AT(w)&XNUM+RAT||!(!AR(w)&&1>=AR(a)&&(g==ds(CDOLLAR)||1==AN(a))))R roll(df2(z,a,w,g));
 RETF(rollksub(a,vi(w)));
}    /* ?@$ or ?@# or [:?$ or [:?# */

static X jtxrand(J jt,X x){PROLOG(0090);A q,z;B b=1;I j,m,n,*qv,*xv,*zv;
 n=AN(x); xv=AV(x);  // number of Digits in x, &first digit
 m=n;  // m is number of result digits, same as input.  If input is 10000... this will always be 1 digit too many, but that's not worth checking for
 GATV0(q,INT,m,1); qv=AV(q);  // allocate place to hold base, qv-> result digits
 DO(m-1, qv[i]=XBASE;); qv[m-1]=xv[n-1]+1;  // init base to the largest possible value in each Digit
 // loop to roll random values until we get one that is less than x
 do{
  RZ(z=roll(q)); zv=AV(z);  // roll one value in each Digit position
  DQ(j=m, --j; if(xv[j]!=zv[j]){b=xv[j]<zv[j]; break;});  // MS mismatched Digit tells the tale; if no mismatch, that's too high, keep b=1
 }while(b);  // loop till b=0
 j=m-1; while(0<j&&!zv[j])--j; AN(z)=AS(z)[0]=++j;  // remove leading 0s from (tail of) result
 EPILOG(z);
}    /* ?x where x is a single strictly positive extended integer */

static F1(jtrollxnum){A z;B c=0;I d,n;X*u,*v,x;
 if(!(AT(w)&XNUM))RZ(w=cvt(XNUM,w));  // convert rational to numeric
 n=AN(w); v=XAV(w);
 GATV(z,XNUM,n,AR(w),AS(w)); u=XAV(z);
 // deal an extended random for each input number.  Error if number <0; if 0, put in 0 as a placeholder
 DQ(n, x=*v++; d=XDIG(x); ASSERT(0<=d,EVDOMAIN); if(d)RZ(*u++=rifvs(xrand(x))) else{*u++=iv0; c=1;});
 // If there was a 0, convert the whole result to float, and go back and fill the original 0s with random floats
 if(c){D*d;I mk,sh;
  INITD;
  RZ(z=cvt(FL,z)); d=DAV(z); v=XAV(w);
  DQ(n, x=*v++; if(!XDIG(x))*d=sh?NEXTD1:NEXTD0; ++d;);
 } 
 R z;
}    /* ?n$x where x is extended integer */


static F1(jtrollbool){A z;B*v;D*u;I n,sh;UINT mk;
 n=AN(w); v=BAV(w); INITD;
 GATV(z,FL,n,AR(w),AS(w)); u=DAV(z);
 if(sh)DQ(n, *u++=*v++?0.0:NEXTD1;)
 else  DQ(n, *u++=*v++?0.0:NEXTD0;)
 R z;
}    /* ?n$x where x is boolean */

// If w is all 2, deal Booleans, with each each bit of a random number providing a single Boolean
// Result is Boolean array, or mark if w is not all 2
// *b=0 if w contained non-2, 1 otherwise (i. e. result is valid if *b=1)
static A jtroll2(J jt,A w,B*b){A z;I j,n,nslice,p,q,r,*v;UI mk,t,*zv;
 *b=0; n=AN(w); v=AV(w);  // init failure return; n=#atoms of w, v->first atom
 // If w contains non-2, return with error
 DO(n, if(v[i]!=2)R mark;);   // return fast if not all-Boolean result
 // See how many RNG values to use.  jt->rngw gives the number of bits in a generated random #
 // We will shift these out 4 or 8 bits at a time; the number of slices we can get out of
 // a random number is 8 - the number of non-random bits at the top of a word.  p will be the number
 // of bits we can get per random number
 p = (BW/8) * (nslice = (8 - (BW-jt->rngw)));  // #bits/slice, times number of slices
 // See how many p-size blocks we can have, and how many single leftovers
 q=n/p; r=n%p;   // q=# p-size blocks, r=#single-bit leftovers
  mk=(UI)0x0101010101010101;
 GATV(z,B01,n,AR(w),AS(w)); zv=(UI*)AV(z);  // Allocate result area
 // Loop to output all the p-size blocks
 for(j=0;j<q;++j){
  t=NEXT;
  DQ(nslice, *zv++=mk&t; t>>=1;);
 }
 // Get a random # for finishing slices, & out them
 t=NEXT;  // Get random # for slices
 DQ(r>>LGSZI, *zv++=mk&t; t>>=1;);
 // Output the rest, one bit at a time
 t=NEXT;  // Get random # for bits
 B*c=(B*)zv; DQ(r&(SZI-1), *c++=1&t; t>>=1;);
 *b=1; R z;
}    /* ?n$x where x is 2, maybe */

static A jtrollnot0(J jt,A w,B*b){A z;I j,m1,n,*u,*v;UI m,s,t,x=jt->rngM[jt->rng];
 *b=0; n=AN(w);
 if(n){v=AV(w); m1=*v++; j=1; DQ(n-1, if(m1!=*v++){j=0; break;});}
 if(n&&j)RZ(z=rollksub(shape(w),sc(m1)))
 else{
  GATV(z,INT,n,AR(w),AS(w));
  v=AV(w); u=AV(z);
  for(j=0;j<n;++j){
   m1=*v++; if(!m1)R mark; ASSERT(0<=m1,EVDOMAIN); m=m1;
   s=GMOF(m,x); t=NEXT; if(s)while(s<=t)t=NEXT; *u++=t%m;
 }}
 *b=1; R z;
}    /* ?n$x where x is not 0, maybe */

static A jtrollany(J jt,A w,B*b){A z;D*u;I j,m1,n,sh,*v;UI m,mk,s,t,x=jt->rngM[jt->rng];
 *b=0; n=AN(w); v=AV(w); INITD;
 GATV(z,FL,n,AR(w),AS(w)); u=DAV(z);
 for(j=0;j<n;++j){
  m1=*v++; ASSERT(0<=m1,EVDOMAIN); m=m1;
  if(0==m)*u++=sh?NEXTD1:NEXTD0; 
  else{s=GMOF(m,x); t=NEXT; if(s)while(s<=t)t=NEXT; *u++=(D)(t%m);}
 }
 *b=1; R z;
}    /* ?s$x where x can be anything and 1<#x */

F1(jtroll){A z;B b=0;I m,wt;
 ARGCHK1(w);
 wt=AT(w);
 ASSERT(wt&DENSE,EVDOMAIN);
 if(!AN(w)){GATV(z,B01,0,AR(w),AS(w)); R z;}
 if(wt&B01)R rollbool(w);
 if(wt&XNUM+RAT)R rollxnum(w);
 RZ(w=vi(w)); m=AV(w)[0];
 if(    2==m)RZ(z=roll2   (w,&b));
 if(!b&&0!=m)RZ(z=rollnot0(w,&b));
 if(!b      )RZ(z=rollany (w,&b));
 RETF(z&&!(FL&AT(z))&&wt&XNUM+RAT?xco1(z):z);
}

F2(jtdeal){A z;I at,j,k,m,n,wt,*zv;UI c,s,t,x=jt->rngM[jt->rng];UI sq;
 ARGCHK2(a,w);
 at=AT(a); wt=AT(w);
 ASSERT(at&DENSE&at&&wt&DENSE,EVDOMAIN);
 F2RANK(0,0,jtdeal,UNUSED_VALUE);
 RE(m=i0(a)); RE(c=n=i0(w));  // c starts as max#+1
 ASSERT(0<=m&&m<=n,EVDOMAIN);  // m and n must both be positive
 if(0==m)z=mtv;
 else if(m*3.0<n||(x&&x<=(UI)n)){  // TUNE for about m=100000; the cutoff would be higher for smaller n
#if BW==64
  // calculate the number of values to deal: m, plus a factor times the expected number of collisions, plus 2 for good measure.  Will never exceed n.  Repeats a little less than 1% of the time for n between 30 and 300
  A h=sc(m+4+(I)((n<1000?2.4:2.2)*((D)m+(D)n*(pow((((D)(n-1))/(D)n),(D)m)-1)))); do{RZ(z=nub(rollksub(h,w)));}while(AN(z)<m); RZ(z=jttake(JTIPW,a,z));
#else
  A h,y; I d,*hv,i,i1,p,q,*v,*yv;
  FULLHASHSIZE(2*m,INTSIZE,1,0,p);
  GATV0(h,INT,p,1); hv=AV(h); DO(p, hv[i]=0;);
  GATV0(y,INT,2+2*m,1); yv=AV(y); d=2;
  GATV0(z,INT,m,1); zv=AV(z);
  I qp=0; GMOF2(c,x,s,sq);
  for(i=0;i<m;++i){
   if(s<GMOTHRESH)GMOF2(c,x,s,sq);
   t=NEXT; if(s)while(s<=t)t=NEXT; j=i+t%c--; s-=sq;
   q=qp; ++qp; qp=qp==p?0:qp; while(hv[q]&&(v=yv+hv[q],i!=*v))++q, q=q==p?0:q; i1=hv[q]?v[1]:i;
   q=j%p; while(hv[q]&&(v=yv+hv[q],j!=*v))++q, q=q==p?0:q;
   if(hv[q]){++v; *zv++=*v; *v=i1;}
   else{v=yv+d; *zv++=*v++=j; *v=i1; hv[q]=d; d+=2;}
  }
#endif
 }else{
  RZ(z=apvwr(n,0L,1L)); zv=AV(z);
  if(BW==64&&n<(1LL<<50)){ 
   // If we can do the calculation in the floating-point unit, do
   D cd=c*X64; DO(m, j=i+(I)(cd*((D)(I)NEXT+(D)x63)); cd-=X64; k=zv[i]; zv[i]=zv[j]; zv[j]=k;)  // avoid unsigned conversion, which requires conditional correction
  }else{
   GMOF2(c,x,s,sq); DO(m, if(s<GMOTHRESH)GMOF2(c,x,s,sq); t=NEXT; if(s)while(s<=t)t=NEXT; s-=sq; j=i+t%c--; k=zv[i]; zv[i]=zv[j]; zv[j]=k;);
  }
  
  AN(z)=AS(z)[0]=m;  // lots of wasted space!
 }
 RETF(at&XNUM+RAT||wt&XNUM+RAT?xco1(z):z);
}

// support for ?.
// To the extent possible, ?. is frozen.  Changed modules need to be copied here to preserve compatibility
#undef GMOF
           /* m<x, greatest multiple of m less than x */
#define GMOF(m,x)   (            x63+(x63-(2*(x63%m))%m))

#undef rollksub
#define rollksub(a,w) jtrollksubdot(jt,(a),(w))
static F2(jtrollksubdot){A z;I an,*av,k,m1,n,p,q,r,sh;UI j,m,mk,s,t,*u,x=jt->rngM[jt->rng];
 ARGCHK2(a,w);
 an=AN(a); RE(m1=i0(w)); ASSERT(0<=m1,EVDOMAIN); m=m1;
 RZ(a=vip(a)); av=AV(a); PRODX(n,an,av,1);
 GA(z,0==m?FL:2==m?B01:INT,n,an,av); u=(UI*)AV(z);
 if(!m){D*v=DAV(z); INITD; if(sh)DQ(n, *v++=NEXTD1;)else DQ(n, *v++=NEXTD0;);}
 else if(2==m){I nslice; I j;
  p = (BW/8) * (nslice = (8 - (BW-jt->rngw)));  // #bits/slice, times number of slices
  // See how many p-size blocks we can have, and how many single leftovers
  q=n/p; r=n%p;   // q=# p-size blocks, r=#single-bit leftovers
  mk=(UI)0x0101010101010101;
  // Loop to output all the p-size blocks
  for(j=0;j<q;++j){
   t=NEXT;
   DQ(nslice, *u++=mk&t; t>>=1;);
  }
  // Get a random # for finishing slices, & out them
  t=NEXT;  // Get random # for slices
  DQ(r>>LGSZI, *u++=mk&t; t>>=1;);
  // Output the rest, one bit at a time
  t=NEXT;  // Get random # for bits
  B*c=(B*)u; DQ(r&(SZI-1), *c++=1&t; t>>=1;);
 }else{
  r=n; s=GMOF(m,x); if(s==x)s=0;
  k=0; j=1; while(m>j){++k; j<<=1;}
  if(k&&j==m){  /* m=2^k but is not 1 or 2 */
   p=jt->rngw/k; q=n/p; r=n%p; mk=m-1;
   switch((s?2:0)+(1<p)){
    case 0: DQ(q,           t=NEXT;         *u++=mk&t;         ); break;
    case 1: DQ(q,           t=NEXT;   DQ(p, *u++=mk&t; t>>=k;);); break;
    case 2: DQ(q, while(s<=(t=NEXT));       *u++=mk&t;         ); break;
    case 3: DQ(q, while(s<=(t=NEXT)); DQ(p, *u++=mk&t; t>>=k;););
  }}
  if(r&&s)DQ(r, while(s<=(t=NEXT)); *u++=t%m;) else DQ(r, *u++=NEXT%m;);
 }
 R z;
}

#undef rollk
#define rollk(a,w,self) jtrollkdot(jt,(a),(w),(self))
DF2(jtrollkdot){A g,z;V*sv;
 ARGCHK3(a,w,self);
 sv=FAV(self); g=sv->fgh[2]?sv->fgh[2]:sv->fgh[1];
 if(AT(w)&XNUM+RAT||!(!AR(w)&&1>=AR(a)&&(g==ds(CDOLLAR)||1==AN(a))))R roll(df2(z,a,w,g));
 RETF(rollksub(a,vi(w)));
}    /* ?@$ or ?@# or [:?$ or [:?# */

#undef xrand
#define xrand(w) jtxranddot(jt,(w))
static X jtxranddot(J jt,X x){PROLOG(0090);A q,z;B b=1;I j,m,n,*qv,*xv,*zv;
 n=AN(x); xv=AV(x);  // number of Digits in x, &first digit
 m=n;  // m is number of result digits, same as input.  If input is 10000... this will always be 1 digit too many, but that's not worth checking for
 GATV0(q,INT,m,1); qv=AV(q);  // allocate place to hold base, qv-> result digits
 DO(m-1, qv[i]=XBASE;); qv[m-1]=xv[n-1]+1;  // init base to the largest possible value in each Digit
 // loop to roll random values until we get one that is less than x
 do{
  RZ(z=roll(q)); zv=AV(z);  // roll one value in each Digit position
  DQ(j=m, --j; if(xv[j]!=zv[j]){b=xv[j]<zv[j]; break;});  // MS mismatched Digit tells the tale; if no mismatch, that's too high, keep b=1
 }while(b);  // loop till b=0
 j=m-1; while(0<j&&!zv[j])--j; AN(z)=AS(z)[0]=++j;  // remove leading 0s from (tail of) result
 EPILOG(z);
}    /* ?x where x is a single strictly positive extended integer */

#undef rollxnum
#define rollxnum(w) jtrollxnumdot(jt,(w))
static F1(jtrollxnumdot){A z;B c=0;I d,n;X*u,*v,x;
 if(!(AT(w)&XNUM))RZ(w=cvt(XNUM,w));  // convert rational to numeric
 n=AN(w); v=XAV(w);
 GATV(z,XNUM,n,AR(w),AS(w)); u=XAV(z);
 // deal an extended random for each input number.  Error if number <0; if 0, put in 0 as a placeholder
 DQ(n, x=*v++; d=XDIG(x); ASSERT(0<=d,EVDOMAIN); if(d)RZ(*u++=rifvs(xrand(x))) else{*u++=iv0; c=1;});
 // If there was a 0, convert the whole result to float, and go back and fill the original 0s with random floats
 if(c){D*d;I mk,sh;
  INITD;
  RZ(z=cvt(FL,z)); d=DAV(z); v=XAV(w);
  DQ(n, x=*v++; if(!XDIG(x))*d=sh?NEXTD1:NEXTD0; ++d;);
 } 
 R z;
}    /* ?n$x where x is extended integer */

#undef rollbool
#define rollbool(w) jtrollbooldot(jt,(w))
static F1(jtrollbooldot){A z;B*v;D*u;I n,sh;UINT mk;
 n=AN(w); v=BAV(w); INITD;
 GATV(z,FL,n,AR(w),AS(w)); u=DAV(z);
 if(sh)DQ(n, *u++=*v++?0.0:NEXTD1;)
 else  DQ(n, *u++=*v++?0.0:NEXTD0;)
 R z;
}    /* ?n$x where x is boolean */

// If w is all 2, deal Booleans, with each each bit of a random number providing a single Boolean
// Result is Boolean array, or mark if w is not all 2
// *b=0 if w contained non-2, 1 otherwise (i. e. result is valid if *b=1)
#undef roll2
#define roll2(w,b) jtroll2dot(jt,(w),(b))
static A jtroll2dot(J jt,A w,B*b){A z;I j,n,nslice,p,q,r,*v;UI mk,t,*zv;
 *b=0; n=AN(w); v=AV(w);  // init failure return; n=#atoms of w, v->first atom
 // If w contains non-2, return with error
 DO(n, if(v[i]!=2)R mark;);   // return fast if not all-Boolean result
 // See how many RNG values to use.  jt->rngw gives the number of bits in a generated random #
 // We will shift these out 4 or 8 bits at a time; the number of slices we can get out of
 // a random number is 8 - the number of non-random bits at the top of a word.  p will be the number
 // of bits we can get per random number
 p = (BW/8) * (nslice = (8 - (BW-jt->rngw)));  // #bits/slice, times number of slices
 // See how many p-size blocks we can have, and how many single leftovers
 q=n/p; r=n%p;   // q=# p-size blocks, r=#single-bit leftovers
  mk=(UI)0x0101010101010101;
 GATV(z,B01,n,AR(w),AS(w)); zv=(UI*)AV(z);  // Allocate result area
 // Loop to output all the p-size blocks
 for(j=0;j<q;++j){
  t=NEXT;
  DQ(nslice, *zv++=mk&t; t>>=1;);
 }
 // Get a random # for finishing slices, & out them
 t=NEXT;  // Get random # for slices
 DQ(r>>LGSZI, *zv++=mk&t; t>>=1;);
 // Output the rest, one bit at a time
 t=NEXT;  // Get random # for bits
 B*c=(B*)zv; DQ(r&(SZI-1), *c++=1&t; t>>=1;);
 *b=1; R z;
}    /* ?n$x where x is 2, maybe */

#undef rollnot0
#define rollnot0(w,b) jtrollnot0dot(jt,(w),(b))
static A jtrollnot0dot(J jt,A w,B*b){A z;I j,m1,n,*u,*v;UI m,s,t,x=jt->rngM[jt->rng];
 *b=0; n=AN(w);
 if(n){v=AV(w); m1=*v++; j=1; DQ(n-1, if(m1!=*v++){j=0; break;});}
 if(n&&j)RZ(z=rollksub(shape(w),sc(m1)))
 else{
  GATV(z,INT,n,AR(w),AS(w));
  v=AV(w); u=AV(z);
  for(j=0;j<n;++j){
   m1=*v++; if(!m1)R mark; ASSERT(0<=m1,EVDOMAIN); m=m1;
   s=GMOF(m,x); t=NEXT; if(s)while(s<=t)t=NEXT; *u++=t%m;
 }}
 *b=1; R z;
}    /* ?n$x where x is not 0, maybe */

#undef rollany
#define rollany(w,b) jtrollanydot(jt,(w),(b))
static A jtrollanydot(J jt,A w,B*b){A z;D*u;I j,m1,n,sh,*v;UI m,mk,s,t,x=jt->rngM[jt->rng];
 *b=0; n=AN(w); v=AV(w); INITD;
 GATV(z,FL,n,AR(w),AS(w)); u=DAV(z);
 for(j=0;j<n;++j){
  m1=*v++; ASSERT(0<=m1,EVDOMAIN); m=m1;
  if(0==m)*u++=sh?NEXTD1:NEXTD0; 
  else{s=GMOF(m,x); t=NEXT; if(s)while(s<=t)t=NEXT; *u++=(D)(t%m);}
 }
 *b=1; R z;
}    /* ?s$x where x can be anything and 1<#x */

#undef roll
#define roll(w) jtrolldot(jt,(w))
static F1(jtrolldot){A z;B b=0;I m,wt;
 ARGCHK1(w);
 wt=AT(w);
 ASSERT(wt&DENSE,EVDOMAIN);
 if(!AN(w)){GATV(z,B01,0,AR(w),AS(w)); R z;}
 if(wt&B01)R rollbool(w);
 if(wt&XNUM+RAT)R rollxnum(w);
 RZ(w=vi(w)); m=AV(w)[0];
 if(    2==m)RZ(z=roll2   (w,&b));
 if(!b&&0!=m)RZ(z=rollnot0(w,&b));
 if(!b      )RZ(z=rollany (w,&b));
 RETF(z&&!(FL&AT(z))&&wt&XNUM+RAT?xco1(z):z);
}

#undef deal
#define deal(a,w) jtdealdot(jt,(a),(w))
static F2(jtdealdot){A h,y,z;I at,d,*hv,i,i1,j,k,m,n,p,q,*v,wt,*yv,*zv;UI c,s,t,x=jt->rngM[jt->rng];
 ARGCHK2(a,w);
 at=AT(a); wt=AT(w);
 ASSERT(at&DENSE&at&&wt&DENSE,EVDOMAIN);
 F2RANK(0,0,jtdealdot,UNUSED_VALUE);
 RE(m=i0(a)); RE(c=n=i0(w));
 ASSERT(0<=m&&m<=n,EVDOMAIN);  // m and n must both be positive
 if(0==m)z=mtv;
 else if(m<n/5.0||x<=(UI)n){
  FULLHASHSIZE(2*m,INTSIZE,1,0,p);
  GATV0(h,INT,p,1); hv=AV(h); DO(p, hv[i]=0;);
  GATV0(y,INT,2+2*m,1); yv=AV(y); d=2;
  GATV0(z,INT,m,1); zv=AV(z);
  for(i=0;i<m;++i){
   s=GMOF(c,x); t=NEXT; if(s)while(s<=t)t=NEXT; j=i+t%c--;
   q=i%p; while(hv[q]&&(v=yv+hv[q],i!=*v))q=(1+q)%p; i1=hv[q]?v[1]:i;
   q=j%p; while(hv[q]&&(v=yv+hv[q],j!=*v))q=(1+q)%p;
   if(hv[q]){++v; *zv++=*v; *v=i1;}
   else{v=yv+d; *zv++=*v++=j; *v=i1; hv[q]=d; d+=2;}
 }}else{
  RZ(z=apvwr(n,0L,1L)); zv=AV(z);
  DO(m, s=GMOF(c,x); t=NEXT; if(s)while(s<=t)t=NEXT; j=i+t%c--; k=zv[i]; zv[i]=zv[j]; zv[j]=k;);
  AN(z)=AS(z)[0]=m;
 }
 RETF(at&XNUM+RAT||wt&XNUM+RAT?xco1(z):z);
}




#define FXSDECL     A z;I i,j=jt->rng;UI*v=jt->rngV[GBI];
#define FXSDO       {i=j==GBI?jt->rngi:jt->rngI[GBI];                                \
                     if(!jt->rngfxsv){GAT0(z,INT,GBN,1); ras(z); jt->rngfxsv=AV(z);}  \
                     jt->rngV[GBI]=jt->rngfxsv; rngselects(sc(GBI)); gb_init(16807);}
#define FXSOD       {jt->rngV[GBI]=v; jt->rngI[GBI]=jt->rngi=i; rngselects(sc(j));}

F1(jtrollx  ){FXSDECL; ARGCHK1(w);                 FXSDO; z=roll(w);         FXSOD; R z;}       
F2(jtdealx  ){FXSDECL; F2RANK(0,0,jtdealx,UNUSED_VALUE); FXSDO; z=deal(a,w);       FXSOD; R z;}        
DF2(jtrollkx){FXSDECL; ARGCHK3(a,w,self);        FXSDO; z=rollk(a,w,self); FXSOD; R z;}      


/*
static F1(jtroll){A z;D rl=jt->rl;static D dm=16807,p=2147483647L;I c,n,*v,*x;
 ARGCHK1(w);
 n=AN(w); v=AV(w);
 RZ(z=reshape(shape(w),num(2))); x=AV(z);
 if(ICMP(v,x,n))
  DQ(n, c=*v++; ASSERT(0<c,EVDOMAIN); rl=fmod(rl*dm,p); *x++=(I)jfloor(rl*c/p);)
 else{B*x;D q=p/2;
  GATV(z,B01,n,AR(w),AS(w)); x=BAV(z);
  DQ(n, rl=fmod(rl*dm,p); *x++=rl>q;);
 }
 jt->rl=(I)rl;
 R z;
} */    /* P.C. Berry, Sharp APL Reference Manual, 1979, p. 126. */

/*
static A jtbigdeal(J jt,I m,I n){A t,x,y;
 RZ(x=sc((I)jfloor(1.11*m)));
 RZ(y=sc(n));
 do{RZ(t=nub(roll(reshape(x,y))));}while(m>AN(t));
 R vec(INT,m,AV(t));
} */    /* E.E. McDonnell circa 1966, small m and large n */

/*
static A jtdeal(J jt,I m,I n){A y;D rl=jt->rl;static D dm=16807,p=2147483647L;I j,k,*yv;
 if(m<0.01*n)R bigdeal(m,n);
 RZ(y=apv(n,n-1,-1L)); yv=AV(y);
 DO(m, rl=fmod(rl*dm,p); j=i+(I)jfloor(rl*(n-i)/(1+p)); k=yv[i]; yv[i]=yv[j]; yv[j]=k;);
 jt->rl=(I)rl;
 R vec(INT,m,yv);
} */   /* P.C. Berry, Sharp APL Reference Manual, 1979, p. 178. */
