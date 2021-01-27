/* Copyright 1990-2009, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Conjunctions: Inner Product                                             */

#include "j.h"
#include "verbs/vasm.h"
#include "gemm.h"

#define MAXAROWS 384  // max rows of a that we can process to stay in L2 cache   a strip is m*CACHEHEIGHT, z strip is m*CACHEWIDTH   this is wired to 128*3 - check if you change

// Analysis for inner product
// a,w are arguments
// zt is type of result
// *pm is # 1-cells of a
// *pn is # atoms in an item of w (but if w is a list, it is treated as a pxn column, and n=1)
// *pp is number of inner-product muladds
//   (in each, an atom of a multiplies an item of w)
static A jtipprep(J jt,A a,A w,I zt,I*pm,I*pn,I*pp){A z=mark;I*as,ar,ar1,m,mn,n,p,*ws,wr,wr1;
 ar=AR(a); as=AS(a); ar1=ar-1>=0?ar-1:0; PRODX(m,ar1,as,1) *pm=m;  // m=# 1-cells of a.  It could overflow, if there are no atoms
 wr=AR(w); ws=AS(w); wr1=wr-1>=0?wr-1:0; PRODX(n,wr1,ws+1,1) *pn=n; DPMULDE(m,n,mn);  // n=#atoms in item of w; mn = #atoms in result
 I t=AS(w)[0]; p=wr?t:1; t=AS(a)[ar1]; p=ar?t:p; *pp=p;  // if a is an array, the length of a 1-cell; otherwise, the number of items of w
 ASSERT(!(ar&&wr)||p==ws[0],EVLENGTH);
 GA(z,zt,mn,ar1+wr1,0);   // allocate result area
 MCISH(AS(z),      as,ar1);  // Set shape: 1-frame of a followed by shape of item of w
 MCISH(AS(z)+ar1,1+ws,wr1);
 return z;
}    /* argument validation & result for an inner product */

#define IINC(x,y)    {b=0>x; x+=y; BOV(b==0>y&&b!=0>x);}
#define DINC(x,y)    {x+=y;}
#define ZINC(x,y)    {(x).re+=(y).re; (x).im+=(y).im;}

#define PDTBY(Tz,Tw,INC)          \
 {Tw*v,*wv;Tz c,*x,zero,*zv;      \
  v=wv=(Tw*)AV(w); zv=(Tz*)AV(z); memset(&zero,C0,sizeof(Tz));     \
  if(1==n)DQ(m, v=wv; c=zero;           DQ(p,       if(*u++)INC(c,*v);             ++v;);             *zv++=c;)   \
  else    DQ(m, v=wv; memset(zv,C0,zk); DQ(p, x=zv; if(*u++)DQ(n, INC(*x,*v); ++x; ++v;) else v+=n;); zv+=n;  );  \
 }

#define PDTXB(Tz,Ta,INC,INIT)     \
 {Ta*u;Tz c,*x,zero,*zv;          \
  u=   (Ta*)AV(a); zv=(Tz*)AV(z); memset(&zero,C0,sizeof(Tz));     \
  if(1==n)DQ(m, v=wv; c=zero;           DQ(p,                   if(*v++)INC(c,*u); ++u;  ); *zv++=c;)   \
  else    DQ(m, v=wv; memset(zv,C0,zk); DQ(p, x=zv; INIT; DQ(n, if(*v++)INC(*x,c); ++x;);); zv+=n;  );  \
 }

static F2(jtpdtby){A z;B b,*u,*v,*wv;C er=0;I at,m,n,p,t,wt,zk;
 at=AT(a); wt=AT(w); t=at&B01?wt:at;
 RZ(z=ipprep(a,w,t,&m,&n,&p)); zk=n<<bplg(t); u=BAV(a); v=wv=BAV(w);
 NAN0;
 switch(CTTZ(t)){
  default:   ASSERT(0,EVDOMAIN);
  case CMPXX:  if(at&B01)PDTBY(Z,Z,ZINC) else PDTXB(Z,Z,ZINC,c=*u++   ); break;
  case FLX:    if(at&B01)PDTBY(D,D,DINC) else PDTXB(D,D,DINC,c=*u++   ); break;
  case INTX:   if(at&B01)PDTBY(I,I,IINC) else PDTXB(I,I,IINC,c=*u++   ); 
             if(er>=EWOV){
              RZ(z=ipprep(a,w,FL,&m,&n,&p)); zk=n*sizeof(D); u=BAV(a); v=wv=BAV(w);
              if(at&B01)PDTBY(D,I,IINC) else PDTXB(D,I,IINC,c=(D)*u++);
 }}
 NAN1;
 RETF(z);
}    /* x +/ .* y where x or y (but not both) is Boolean */

#define BBLOCK(nn)  \
     d=ti; DQ(nw, *d++=0;);                                           \
     DQ(nn, if(*u++){vi=(UI*)v; d=ti; DQ(nw, *d+++=*vi++;);} v+=n;);  \
     x=zv; c=tc; DQ(n, *x+++=*c++;);

// cache-blocking code
#define OPHEIGHT 2  // height of outer-product block
#define OPWIDTH 4  // width of outer-product block
#define CACHEWIDTH 64  // width of resident cache block (in D atoms)
#define CACHEHEIGHT 16  // height of resident cache block
static D missingrow[CACHEHEIGHT]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
// Floating-point matrix multiply, hived off to a subroutine to get fresh register allocation
// *zv=*av * *wv, with *cv being a cache-aligned region big enough to hold CACHEWIDTH*CACHEHEIGHT floats
// a is shape mxp, w is shape pxn.  Result is 0 if OK, 1 if overflow
I cachedmmult(J jt,D* av,D* wv,D* zv,I m,I n,I p,I flgs){D c[(CACHEHEIGHT+1)*CACHEWIDTH + (CACHEHEIGHT+1)*OPHEIGHT*OPWIDTH*2 + 2*CACHELINESIZE/sizeof(D)];  // 2 in case complex
 // m is # 1-cells of a
 // n is # values in an item of w (and result)
 // p is number of inner-product muladds (length of a row of a, and # items of w)
 // point to cache-aligned areas we will use for staging the inner-product info
 // flgs is 0 for float, 1 for complex, i. e. lg2(# values per atom).  If flgs is set, n and p are even, and give the lengths of the arguments in values
 D *cvw = (D*)(((I)&c+(CACHELINESIZE-1))&-CACHELINESIZE);  // place where cache-blocks of w are staged
 D *cva = (D*)(((I)cvw+(CACHEHEIGHT+1)*CACHEWIDTH*sizeof(D)+(CACHELINESIZE-1))&-CACHELINESIZE);   // place where expanded rows of a are staged
 // zero the result area
 memset(zv,C0,m*n*sizeof(D));
 // process each 64-float vertical stripe of w, producing the corresponding columns of z
 D* w0base = wv; D* z0base = zv; I w0rem = n;
 for(;w0rem>0;w0rem-=CACHEWIDTH,w0base+=CACHEWIDTH,z0base+=CACHEWIDTH){
  // process each 16x64 section of w, adding each result to the columns of z.  Each section goes through a different set of 16/32 columns of a 
  D* a1base=av; D* w1base=w0base; D* z1base=z0base; I w1rem=p>>(flgs&FLGCMP);
  for(;w1rem>0;w1rem-=CACHEHEIGHT,a1base+=CACHEHEIGHT<<(flgs&FLGCMP),w1base+=CACHEHEIGHT*n){D* RESTRICT cvx;D* w1next=w1base;I i;
   // read the 16x64 section of w into the cache area (8KB, 2 ways of cache), with prefetch of rows
   for(i=MIN(CACHEHEIGHT,w1rem),cvx=cvw;i;--i){I j;
    D* RESTRICT w1x=w1next; w1next+=n;  // save start of current input row, point to next row...
    // I don't think it's worth the trouble to move the data with avx instructions - though it was to prefetch it
    for(j=0;j<MIN(CACHEWIDTH,w0rem);++j){D fv = *w1x; if(flgs&FLGINT)fv=(D)*(I*)w1x; *cvx++=fv; w1x++;}  // move the valid data
    for(;j<CACHEWIDTH;++j)*cvx++=0.0;   // fill the rest with 0
   }
   // Because of loop unrolling, we fetch and multiply one extra value in each cache column.  We make sure those values are 0 to avoid NaN errors
   for(i=0;i<MIN(CACHEWIDTH,w0rem);++i)*cvx++=0.0;

   // w1next is left pointing to the next cache block in the column.  We will use that to prefetch
#ifdef PREFETCH
   D *nextprefetch=w1next;  // start prefetches for the next block at the beginning
#endif

   // the mx16 vertical strip of a (mx32 if flgs) will be multiplied by the 16x64 section of w and accumulated into z
   // process each 2x16 section of a against the 16x64 cache block
   D *a2base0=a1base; D* w2base=w1base; I a2rem=m; D* z2base=z1base; D* c2base=cvw;
   for(;a2rem>0;a2rem-=OPHEIGHT,a2base0+=OPHEIGHT*p,z2base+=OPHEIGHT*n){
    // Prepare for the 2x16 block of a (2x32 if flgs)
    // If the second row of a is off the end of the data, we mustn't fetch it - switch the pointer to a row of 1s so it won't give NaN error on multiplying by infinity
    D *a2base1 = (a2rem>1)?a2base0+p:missingrow;
#ifdef PREFETCH
    // While we are processing the sections of a, move the next cache block into L2 (not L1, so we don't overrun it)
    // We would like to do all the prefetches for a CACHEWIDTH at once to stay in page mode
    // but that might overrun the prefetch queue, which holds 10 prefetches.
    // The length of a cache row is (CACHEWIDTH*sizeof(D))/CACHELINESIZE=8 cache lines, plus one in case the data is misaligned.
    // We start the prefetches when we get to within 3*CACHEHEIGHT iterations from the end, which gives us 3 iterations
    // to fetch each row of the cache, 3 fetches per iteration.
    if(a2rem<=3*OPHEIGHT*CACHEHEIGHT+(OPHEIGHT-1)){
     PREFETCH2((C*)nextprefetch); PREFETCH2((C*)nextprefetch+CACHELINESIZE); PREFETCH2((C*)nextprefetch+2*CACHELINESIZE);  // 3 prefetches
     if(nextprefetch==(D*)((C*)w1next+6*CACHELINESIZE)){nextprefetch = w1next += n;}else{nextprefetch+=(3*CACHELINESIZE)/sizeof(*nextprefetch);}  // next col, or next row after 9 prefetches
    }
#endif
    // process each 16x4 section of cache, accumulating into z (this holds 16x2 complex values, if flgs)
    I a3rem=MIN(w0rem,CACHEWIDTH);
    D* RESTRICT z3base=z2base; D* c3base=c2base;
    for(;a3rem>0;a3rem-=OPWIDTH,c3base+=OPWIDTH,z3base+=OPWIDTH){
     // initialize accumulator with the z values accumulated so far.
     D z00,z01,z02,z03,z10,z11,z12,z13;
     z00=z3base[0];
     if(a3rem>3){z01=z3base[1],z02=z3base[2],z03=z3base[3]; if(a2rem>1)z10=z3base[n],z11=z3base[n+1],z12=z3base[n+2],z13=z3base[n+3];
     }else{if(a3rem>1){z01=z3base[1];if(a3rem>2)z02=z3base[2];}; if(a2rem>1){z10=z3base[n];if(a3rem>1)z11=z3base[n+1];if(a3rem>2)z12=z3base[n+2];}}
     // process outer product of each 2x1 section on each 1x4 section of cache

     // Prefetch the next lines of a and z into L2 cache.  We don't prefetch all the way to L1 because that might overfill L1,
     // if all the prefetches hit the same line (we already have 2 lines for our cache area, plus the current z values)
     // The number of prefetches we need per line is (CACHEHEIGHT*sizeof(D)/CACHELINESIZE)+1, and we need that for
     // OPHEIGHT*(OPWIDTH/4) lines for each of a and z.  We squeeze off half the prefetches for a row at once so we can use fast page mode
     // to read the data (only half to avoid overfilling the prefetch buffer), and we space the reads as much as possible through the column-swatches
#ifdef PREFETCH   // if architecture doesn't support prefetch, skip it
     if((3*OPWIDTH)==(a3rem&(3*OPWIDTH))){  // every fourth swatch
      I inc=((a3rem&(8*OPWIDTH))?p:n)*sizeof(D);    // counting down, a then z
      C *base=(C*)((a3rem&(8*OPWIDTH))?a2base0:z2base) + inc + (a3rem&(4*OPWIDTH)?0:inc);
      PREFETCH2(base); PREFETCH2(base+CACHELINESIZE); PREFETCH2(base+2*CACHELINESIZE);
     }
#endif

     // Now do the 16 outer products for the block, each 2ax4w (or 2ax2w if flgs)
     I a4rem=MIN(w1rem,CACHEHEIGHT);
     D * RESTRICT c4base=c3base;
     if(!(flgs&(FLGCMP|FLGINT))){  // real
      D* RESTRICT a4base0=a2base0; D* RESTRICT a4base1=a2base1; 
      do{  // loop for each small outer product
       // read the 2x1 a values and the 1x4 cache values
       // form outer product, add to accumulator
       z00+=a4base0[0]*c4base[0];
       z01+=a4base0[0]*c4base[1];
       z02+=a4base0[0]*c4base[2];
       z03+=a4base0[0]*c4base[3];
       z10+=a4base1[0]*c4base[0];
       z11+=a4base1[0]*c4base[1];
       z12+=a4base1[0]*c4base[2];
       z13+=a4base1[0]*c4base[3];
       a4base0++,a4base1++;
       c4base+=CACHEWIDTH;
      }while(--a4rem>0);
     }else if(flgs&FLGINT){  // INT
      I* RESTRICT a4base0=(I*)a2base0; I* RESTRICT a4base1=(I*)a2base1; 
      do{  // loop for each small outer product
       // read the 2x1 a values and the 1x4 cache values
       // form outer product, add to accumulator
       z00+=(D)a4base0[0]*(D)c4base[0];
       z01+=(D)a4base0[0]*(D)c4base[1];
       z02+=(D)a4base0[0]*(D)c4base[2];
       z03+=(D)a4base0[0]*(D)c4base[3];
       z10+=(D)a4base1[0]*(D)c4base[0];
       z11+=(D)a4base1[0]*(D)c4base[1];
       z12+=(D)a4base1[0]*(D)c4base[2];
       z13+=(D)a4base1[0]*(D)c4base[3];
       a4base0++,a4base1++;
       c4base+=CACHEWIDTH;
      }while(--a4rem>0);
     }else{
      // complex.  The 1x4 cache values represent 1x2 complex values.  The a is fetched as 2x1 complex values.  Result is 2x2 conplex values
      D* RESTRICT a4base0=a2base0; D* RESTRICT a4base1=a2base1; 
      do{  // loop for each small outer product
       // read the 2x1 a values and the 1x4 cache values
       // form outer product, add to accumulator
       z00+=a4base0[0]*c4base[0]-a4base0[1]*c4base[1];
       z01+=a4base0[0]*c4base[1]+a4base0[1]*c4base[0];
       z02+=a4base0[0]*c4base[2]-a4base0[1]*c4base[3];
       z03+=a4base0[0]*c4base[3]+a4base0[1]*c4base[2];
       z10+=a4base1[0]*c4base[0]-a4base1[1]*c4base[1];
       z11+=a4base1[0]*c4base[1]+a4base1[1]*c4base[0];
       z12+=a4base1[0]*c4base[2]-a4base1[1]*c4base[3];
       z13+=a4base1[0]*c4base[3]+a4base1[1]*c4base[2];
       a4base0+=2,a4base1+=2;
       c4base+=CACHEWIDTH;
      }while(--a4rem>0);
     }

     // Store accumulator into z.  Don't store outside the array
     z3base[0]=z00;
     if(a3rem>3){z3base[1]=z01,z3base[2]=z02,z3base[3]=z03; if(a2rem>1)z3base[n]=z10,z3base[n+1]=z11,z3base[n+2]=z12,z3base[n+3]=z13;
     }else{if(a3rem>1){z3base[1]=z01;if(a3rem>2)z3base[2]=z02;}; if(a2rem>1){z3base[n]=z10;if(a3rem>1){z3base[n+1]=z11;if(a3rem>2)z3base[n+2]=z12;}}}
    }
   }
  }
 }
 return 1;
}

// +/ . *
F2(jtpdt){PROLOG(0038);A z;I ar,at,i,m,n,p,p1,t,wr,wt;
 // ?r = rank, ?t = type (but set Boolean type for an empty argument)
 ar=AR(a); at=AT(a); at=AN(a)?at:B01;
 wr=AR(w); wt=AT(w); wt=AN(w)?wt:B01;
 if(unlikely(((at|wt)&SPARSE))!=0)return pdtsp(a,w);  // Transfer to sparse code if either arg sparse
 if(unlikely(((at|wt)&XNUM+RAT))!=0)return df2(z,a,w,atop(slash(ds(CPLUS)),qq(ds(CSTAR),v2(1L,AR(w)))));  // On indirect numeric, execute as +/@(*"(1,(wr)))
 if(unlikely(B01&(at|wt)&&TYPESNE(at,wt)&&((ar-1)|(wr-1)|(AN(a)-1)|(AN(w)-1))>=0))return pdtby(a,w);   // If exactly one arg is boolean, handle separately
 {t=maxtyped(at,wt); if(!TYPESEQ(t,AT(a))){RZ(a=cvt(t,a));} if(!TYPESEQ(t,AT(w))){RZ(w=cvt(t,w));}}  // convert args to compatible precisions, changing a and w if needed.  B01 if both empty
 ASSERT(t&NUMERIC,EVDOMAIN);
 // Allocate result area and calculate loop controls
 // m is # 1-cells of a
 // n is # atoms in an item of w
 // p is number of inner-product muladds (length of a row of a)

 // INT multiplies convert to float, for both 32- and 64-bit systems.  It is converted back if there is no overflow
 m=t; m=t&INT?FL:m; m=t&B01?INT:m;  // type of result, promoting bool and int
 RZ(z=ipprep(a,w,m,&m,&n,&p));  // allocate the result area, with the needed shape and type
 if(AN(z)==0)return z;  // return without computing if result is empty
 if(!p){memset(AV(z),C0,AN(z)<<bplg(AT(z))); return z;}  // if dot-products are all 0 length, set them all to 0
 // If either arg is atomic, reshape it to a list
 if(!ar!=!wr){if(ar)RZ(w=reshape(sc(p),w)) else RZ(a=reshape(sc(p),a));}
 p1=p-1;
 // Perform the inner product according to the type
 switch(CTTZNOFLAG(t)){
 case B01X:
  if(0==(n&(SZI-1))||!SY_ALIGN){A tt;B*u,*v,*wv;I nw,*x,*zv;UC*c,*tc;UI*d,*ti,*vi;
   nw=(n+SZI-1)>>LGSZI;
   GATV0(tt,INT,nw,1); ti=(UI*)AV(tt); tc=(UC*)ti;
   u=BAV(a); v=wv=BAV(w); zv=AV(z);
   for(i=0;i<m;++i,v=wv,zv+=n){x=zv; DQ(n, *x++=0;); I pp=p; while((pp-=255)>=0){BBLOCK(255);} BBLOCK(pp+255);}
  }else{B*u,*v,*wv;I*x,*zv;
   u=BAV(a); v=wv=BAV(w); zv=AV(z);
   for(i=0;i<m;++i,v=wv,zv+=n){
           x=zv; if(*u++)DQ(n, *x++ =*v++;) else{v+=n; DQ(n, *x++=0;);}
    DQ(p1, x=zv; if(*u++)DQ(n, *x+++=*v++;) else v+=n;);
   }
  }
  break;
 case INTX:
  {
 /*
   for(i=0;i<m;++i,v=wv,zv+=n){
     x=zv; c=*u++; er=asmtymes1v(n,x,c,v);    if(er)break; v+=n;
     DQ(p1, x=zv; c=*u++; er=asminnerprodx(n,x,c,v); if(er)break; v+=n;);

 */
#if C_NA   // non-assembler version
   // INT product is problematic, because it is used for many internal purposes, such as #. and indexing of { and m} .  For these uses,
   // one argument (usually w) has only one item, a list that is reused.  So, we check for that case; if found we go through faster code that just
   // performs vector inner products, accumulating in registers.  And we have multiple versions of that: one when the totals can't get close to
   // overflow, and other belt-and-suspenders variants for arbitrary inputs
   if(n==1){DPMULDDECLS I tot;I* RESTRICT zv, * RESTRICT av;
    // vector products
    // The fast loop will be used if each multiplicand, and each product, fits in 32 bits
    I er=0;  // will be set if overflow detected
    I* RESTRICT wv=AV(w); tot=0; DQ(p, I wvv=*wv; if((I4)wvv!=wvv){er=1; break;} wvv=wvv<0?-wvv:wvv; tot+=wvv; wv++;)
    if(!er){
     // w fits in 32 bits.  Try to accumulate the products.  If we can be sure that the total will not exceed 32 bits unless
     // an a-value does, do the fastest loop
     zv=AV(z); av=AV(a);
     if((UI)(p*tot)<(UI)0x100000000){ // integer overflow
      // The total in w is so small that a mere m values each less than 2^31 cannot overflow
      DQ(m, I tot=0; wv=AV(w); DQ(p, I mpcnd=*av++; I prod=mpcnd**wv++; if(mpcnd!=(I4)mpcnd)goto oflo1; tot+=prod;) *zv++=tot;)
     }else{
      // w has some big numbers, so we have to check each individual product
      DQ(m, I tot=0; wv=AV(w); DQ(p, I mpcnd=*av++; I prod=mpcnd**wv++; if(mpcnd!=(I4)mpcnd||prod!=(I4)prod)goto oflo1; tot+=prod;) *zv++=tot;)
     }
     AT(z)=INT; break;
    }
oflo1:
    // Something overflowed 32 bits.  See if it fits in 64.
    zv=AV(z); av=AV(a);
    DQ(m, I lp; I tot=0; wv=AV(w); DQ(p, DPMULD(*av++,*wv++, lp, goto oflo2;) I oc=(~tot)^lp; tot+=lp; lp^=tot; if(XANDY(oc,lp)<0)goto oflo2;) *zv++=tot;)
    AT(z)=INT; break;
oflo2:
    // Result does not fit in INT.  Do the computation as float, with float result
    if(m)RZ(jtsumattymesprods(jt,INT,voidAV(w),voidAV(a),p,1,1,1,m,voidAV(z)));  // use +/@:*"1 .  Exchange w and a because a is the repeated arg in jtsumattymesprods.  If error, clear z (should not happen here)
   }else{
     // full matrix products
     I probsize = m*n*(IL)p;  // This is proportional to the number of multiply-adds.  We use it to select the implementation
     if((UI)probsize < (UI)jt->igemm_thres){RZ(a=cvt(FL,a)); RZ(w=cvt(FL,w)); cachedmmult(jt,DAV(a),DAV(w),DAV(z),m,n,p,0);}  // Do our matrix multiply - converting   TUNE
     else {
      // for large problem, use BLAS
      memset(DAV(z),C0,m*n*sizeof(D));
      igemm_nn(m,n,p,1,(I*)DAV(a),p,1,(I*)DAV(w),n,1,0,DAV(z),n,1);
    }
    // If the result has a value that has been truncated, we should keep it as a float.  Unfortunately, there is no way to be sure that some
    // overflow has not occurred.  So we guess.  If the result is much less than the dynamic range of a float integer, convert the result
    // to integer.
    I i; D *zv=DAV(z);
    for(zv=DAV(z), i=AN(z); i; --i, ++zv)if(*zv>1e13 || *zv<-1e13)break;   // see if any value is out of range
    if(!i){AT(z)=INT;for(zv=DAV(z), i=AN(z); i; --i, ++zv)*(I*)zv=(I)*zv;}  // if not, convert all to integer
   }
#else  // !C_NA
    for(i=0;i<m;++i,v=wv,zv+=n){
     x=zv; c=*u++; TYMES1V(n,x,c,v); if(er)break; v+=n;
     DQ(p1, x=zv; c=*u++; er=asminnerprodx(n,x,c,v); if(er)break; v+=n;);
     if(er)break;
    }
    if(er){A z1;D c,* RESTRICT x,* RESTRICT zv;I* RESTRICT u,* RESTRICT v,* RESTRICT wv;
     GATV(z1,FL,AN(z),AR(z),AS(z)); z=z1;
     u=AV(a); v=wv=AV(w); zv=DAV(z);
     for(i=0;i<m;++i,v=wv,zv+=n){
             x=zv; c=(D)*u++; DQ(n, *x++ =c**v++;);
      DQ(p1, x=zv; c=(D)*u++; DQ(n, *x+++=c**v++;););
   }}}
#endif

  }
  break;
 case FLX:
  {I smallprob; 
   // As for INT, handle the cases where n=1 separately, because they are used internally & don't require as much setup as matrix multiply
   NAN0;
   if(n==1){
    if(m)RZ(jtsumattymesprods(jt,FL,voidAV(w),voidAV(a),p,1,1,1,m,voidAV(z)));  // use +/@:*"1 .  Exchange w and a because a is the repeated arg in jtsumattymesprods.  If error, clear z
    smallprob=0;  // Don't compute it again
   }else{
    // not single column.  Choose the algorithm to use
    I probsize = (m-1)*n*(IL)p;  // This is proportional to the number of multiply-adds.  We use it to select the implementation.  If m==1 we are doing dot-products; no gain from fancy code then
    if(!(smallprob = (m<=4||probsize<1000LL))){  // if small problem, avoid the startup overhead of the matrix version  TUNE
     if((UI)probsize < (UI)jt->dgemm_thres)
      cachedmmult(jt,DAV(a),DAV(w),DAV(z),m,n,p,((AFLAG(a)>>(AFUPPERTRIX-FLGAUTRIX))&FLGAUTRI)|((AFLAG(w)>>(AFUPPERTRIX-FLGWUTRIX))&FLGWUTRI));  // Do our one-core matrix multiply - real   TUNE this is 160x160 times 160x160.  Tell routine if uppertri
     else{
      // If the problem is really big, use BLAS
      memset(DAV(z),C0,m*n*sizeof(D));
      dgemm_nn(m,n,p,1.0,DAV(a),p,1,DAV(w),n,1,0.0,DAV(z),n,1);
     }
    }
   }
   // If there was a floating-point error, retry it the old way in case it was _ * 0
   if(smallprob||NANTEST){D c,s,t,*u,*v,*wv,*x,*zv;
    u=DAV(a); v=wv=DAV(w); zv=DAV(z);
    NAN0;
    if(1==n){DQ(m, v=wv; c=0.0; DQ(p, s=*u++; t=*v++; c+=TYMES(s,t);); *zv++=c;);}
    else for(i=0;i<m;++i,v=wv,zv+=n){
            x=zv; if(c=*u++){if(INF(c))DQ(n, *x++ =TYMES(*v,c); ++v;)else DQ(n, *x++ =c**v++;);}else{v+=n; DQ(n, *x++=0.0;);}
     DQ(p1, x=zv; if(c=*u++){if(INF(c))DQ(n, *x+++=TYMES(*v,c); ++v;)else DQ(n, *x+++=c**v++;);}else v+=n;);
    }
    NAN1;
   }
  }
  break;
 case CMPXX:
  {NAN0;
   I probsize = m*n*(IL)p;  // This is proportional to the number of multiply-adds.  We use it to select the implementation
   I smallprob=probsize<1000;  // set if we do the old-fashioned way, possibly after error
   if(!smallprob){  // use old-fashioned way if small.  16b3.4 comes though here
    if((UI)probsize<(UI)jt->zgemm_thres){smallprob=1^cachedmmult(jt,DAV(a),DAV(w),DAV(z),m,n*2,p*2,((AFLAG(a)>>(AFUPPERTRIX-FLGAUTRIX))&FLGAUTRI)|((AFLAG(w)>>(AFUPPERTRIX-FLGWUTRIX))&FLGWUTRI)|FLGCMP);}  // Do the fast matrix multiply - complex.  Change widths to widths in D atoms, not complex atoms  TUNE  this is 130x130 times 130x130
    else {
      // Large problem - start up BLAS
      memset(DAV(z),C0,2*m*n*sizeof(D));
      zgemm_nn(m,n,p,zone,(dcomplex*)DAV(a),p,1,(dcomplex*)DAV(w),n,1,zzero,(dcomplex*)DAV(z),n,1);
    }
   }
   if(smallprob||NANTEST){Z c,*u,*v,*wv,*x,*zv;
    // There was a floating-point error.  In case it was 0*_ retry old-style
    u=ZAV(a); v=wv=ZAV(w); zv=ZAV(z);
    NAN0;
    if(1==n)DQ(m, v=wv; c=zeroZ; DQ(p, c.re+=ZRE(*u,*v); c.im+=ZIM(*u,*v); ++u; ++v;); *zv++=c;)
    else for(i=0;i<m;++i,v=wv,zv+=n){
            x=zv; c=*u++; DQ(n, x->re =ZRE(c,*v); x->im =ZIM(c,*v); ++x; ++v;);
     DQ(p1, x=zv; c=*u++; DQ(n, x->re+=ZRE(c,*v); x->im+=ZIM(c,*v); ++x; ++v;););
    }
    NAN1;
   }
  }
  break;
 }
 EPILOG(z);
}


#define IPBX0  0
#define IPBX1  1
#define IPBXW  2
#define IPBXNW 3

// a f/ . g w  for boolean a and w
// c is pseudochar for f, d is pseudochar for g
static A jtipbx(J jt,A a,A w,C c,C d){A g=0,x0,x1,z;B*av,*av0,b,*v0,*v1,*zv;C c0,c1;
    I ana,i,j,m,n,p,q,r,*uu,*vv,wc;
 RZ(z=ipprep(a,w,B01,&m,&n,&p));
 // m=#1-cells of a, n=# bytes in 1-cell of w, p=length of individual inner product creating an atom
 ana=!!AR(a); wc=AR(w)?n:0; q=(n-1)>>LGSZI; r=(-n)&(SZI-1);  // ana = 1 if a is not atomic; wc = stride between items of w; q=#fullwords to proc, r=#bytes of last one NOT to proc
 // Set c0 & c1 to classify the g operation
 // See if we can use special techniques
#define Q9(f,c0,c1) ((I)(((c1)<<2)+(c0))<<(((f)-CSTARCO)<<2))
#define PMSK Q9(CSTARCO,IPBX1,IPBXNW)+Q9(CPLUSCO,IPBXNW,IPBX0)+Q9(CSTAR,IPBX0,IPBXW)+Q9(CPLUS,0,0)+Q9(CPLUSDOT,IPBXW,IPBX1)+Q9(CSTARDOT,IPBX0,IPBXW)+Q9(CEQ,IPBXNW,IPBXW)+Q9(CNE,IPBXW,IPBXNW)+Q9(CLT,IPBXW,IPBX0)+Q9(CLE,IPBX1,IPBXW)+Q9(CGE,IPBXNW,IPBX1)+Q9(CGT,IPBX0,IPBXNW)+Q9(CEBAR,0,0)+Q9(CEPS,0,0)+Q9(CMIN,IPBX0,IPBXW)+Q9(CMAX,IPBXW,IPBX1)
 I pmsk=0; pmsk=BETWEENC(d,CSTARCO,CMAX)?PMSK:pmsk; pmsk&=-(B01&AT(w));
 c0=(pmsk>>((d-CSTARCO)<<2))&3; c1=(pmsk>>(((d-CSTARCO)<<2)+2))&3;  // get control bits if known fn, (0,0) if unknown
 // create x0 and x1, the result of (g 0) and (g 1).  If we know the function we can avoid invoking g sometimes
 if(c0+c1==0){
  // unsupported g.  Set c0/c1 to invalid and execute g to find x0/x1
  c0=c1=-1; g=ds(d); RZ(df2(x0,num(0),w,g)); RZ(df2(x1,num(0),w,g));
 }else{
  RZ(x0=c0==IPBX0?reshape(sc(n),num(0)):c0==IPBX1?reshape(sc(c==CNE?AN(w):n),num(1)):c0==IPBXW?w:__not(w));
  RZ(x1=c1==IPBX0?reshape(sc(n),num(0)):c1==IPBX1?reshape(sc(c==CNE?AN(w):n),num(1)):c1==IPBXW?w:__not(w));
 }
 // av->a arg, zv->result, v0->input for 0, v1->input for 1
 av0=BAV(a); zv=BAV(z); v0=BAV(x0); v1=BAV(x1);

 // See if the operations are such that a 0 or 1 from a can skip the innerproduct, or perhaps
 // terminate the entire operation
 I esat=2, eskip=2;  // if byte of a equals esat, the entire result-cell is set to its limit value; if it equals eskip, the innerproduct is skipped
 if(c==CPLUSDOT&&(c0==IPBX1||c1==IPBX1)||c==CSTARDOT&&(c0==IPBX0||c1==IPBX0)){
 // +./ . [+. <: >: *:}   *./ . [*. < > +:]   a byte of a can saturate the entire result-cell: see which value does that
  esat=c==CPLUSDOT?c1==IPBX1:c1==IPBX0;  // esat==1 if +./ . (+. >:)   or *./ . (< +:) where x=1 overrides y (producing 1);  if esat=0, x=0 overrides y (producing 1)
 }else if(c==CPLUSDOT&&(c0==IPBX0||c1==IPBX0)||c==CSTARDOT&&(c0==IPBX1||c1==IPBX1)||
     c==CNE&&(c0==IPBX0||c1==IPBX0)){
  // [+. ~:]/ . [*. < > +:]   *./ . [+. <: >: *:]  a byte of a can guarantee the current innerproduct has no effect
  eskip=c==CSTARDOT?c1==IPBX1:c1==IPBX0;  // eskip==1 if  (+. ~:)/ . (+:)   *./ . (+. >:)  where x=1 has no effect; if esat=0, x=0 has no effect
 }

 switch(c){
  case CPLUSDOT:
#define F |=
#include "cip_t.h"
   break;
  case CSTARDOT:
#define F &=
#include "cip_t.h"
   break;
  case CNE:
#define F ^=
#include "cip_t.h"
   break;
 }
 return z;
}    /* a f/ . g w  where a and w are nonempty and a is boolean */

static DF2(jtdotprod){A fs,gs;C c;I r;V*sv;
 ARGCHK3(a,w,self);
 sv=FAV(self); fs=sv->fgh[0]; gs=sv->fgh[1];  // op is fs . gs
 if((SGNIF(AT(a)&AT(w),B01X)&-AN(a)&-AN(w)&-(FAV(gs)->flag&VISATOMIC2))<0&&CSLASH==ID(fs)&&  // fs is c/
     (c=FAV(FAV(fs)->fgh[0])->id,c==CSTARDOT||c==CPLUSDOT||c==CNE))return ipbx(a,w,c,FAV(gs)->id);  // [+.*.~:]/ . boolean
r=lr(gs);   // left rank of v
 A z; return df2(z,a,w,atop(fs,qq(gs,v2(r==RMAX?r:1+r,RMAX))));  // inner product according to the Dic
}


static F1(jtminors){A d,z;
 RZ(d=apvwr(3L,-1L,1L)); AV(d)[0]=0;
 return drop(d,df2(z,num(1),w,bsdot(ds(CLEFT))));  // 0 0 1 }. 1 [\. w
}

static DF1(jtdet){DECLFG;A h=sv->fgh[2];I c,r,*s;
 r=AR(w); s=AS(w);
 A z; if(h&&1<r&&2==s[r-1]&&s[r-2]==s[r-1])return df1(z,w,h);
 F1RANK(2,jtdet,self);
 c=2>r?1:s[1];
 return !c ? df1(z,mtv,slash(gs)) : 1==c ? CALL1(f1,ravel(w),fs) : h && c==s[0] ? gaussdet(w) : detxm(w,self);
}

DF1(jtdetxm){A z; return dotprod(IRS1(w,0L,1L,jthead,z),det(minors(w),self),self);}
     /* determinant via expansion by minors. w is matrix with >1 columns */

F2(jtdot){A f,h=0;AF f2=jtdotprod;C c,d;
 ASSERTVV(a,w);
 if(CSLASH==ID(a)){
  f=FAV(a)->fgh[0]; c=ID(f); d=ID(w);  // op was c/ . d
  if(d==CSTAR){
   if(c==CPLUS )f2=jtpdt;   // +/ . * is a special function
   if(c==CMINUS)RZ(h=eval("[: -/\"1 {.\"2 * |.\"1@:({:\"2)"));  // -/ . * - calculate some function used by determinant?
 }}
 return fdef(0,CDOT,VERB, jtdet,f2, a,w,h, 0L, 2L,RMAX,RMAX);
}
