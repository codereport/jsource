/* Copyright 1990-2008, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Verbs: a#"r w                                                           */

#include "j.h"


#define REPF(f)         A f(J jt,A a,A w,I wf,I wcr)


static REPF(jtrepzdx){A p,q,x;P*wp;
 F2PREFIP;ARGCHK2(a,w);
 if(SPARSE&AT(w)){wp=PAV(w); x=SPA(wp,e);}
 else x=jt->fill&&AN(jt->fill)?jt->fill:filler(w);
 RZ(p=repeat(ravel(rect(a)),ravel(stitch(IX(wcr?*(wf+AS(w)):1),num(-1)))));
 RZ(IRS2(w,x,0L,wcr,0L,jtover,q));
 R IRS2(p,q,0L,1L,wcr+!wcr,jtfrom,x);
}    /* (dense complex) # (dense or sparse) */

static REPF(jtrepzsx){A q,x,y;I c,d,j,k=-1,m,p=0,*qv,*xv,*yv;P*ap;
 F2PREFIP;ARGCHK2(a,w);
 ap=PAV(a); x=SPA(ap,x); m=AN(x);
 if(!AN(SPA(ap,a)))R repzdx(ravel(x),w,wf,wcr);
 y=SPA(ap,i); yv=AV(y);
 RZ(x=cvt(INT,vec(FL,2*m,AV(x)))); xv=AV(x);
 if(equ(num(0),SPA(ap,e))){
  k=c=*(wf+AS(w));
  if(!wf&&SPARSE&AT(w)){A a,y;I m,n,q,*v;P*wp;
   wp=PAV(w); a=SPA(wp,a);
   if(AN(a)&&!*AV(a)){
    y=SPA(wp,i); v=AS(y); m=v[0]; n=v[1]; v=AV(y);
    k=m?v[(m-1)*n]+1:0; q=0; 
    DQ(m, if(q==*v)++q; else if(q<*v){k=q; break;} v+=n;);
  }}
  ASSERT(k<=IMAX-1,EVLIMIT);
  if(c==k)RZ(w=irs2(sc(1+k),w,0L,0L,wcr,jttake));
  DO(2*m, ASSERT(0<=xv[i],EVDOMAIN); p+=xv[i]; ASSERT(0<=p,EVLIMIT););
  GATV0(q,INT,p,1); qv=AV(q);
  DO(m, c=*xv++; d=*xv++; j=yv[i]; DQ(c, *qv++=j;); DQ(d, *qv++=k;);); 
  R irs2(q,w,0L,1L,wcr,jtfrom);
 }
 ASSERT(0,EVNONCE);
}    /* (sparse complex) #"r (dense or sparse) */

static REPF(jtrepbdx){A z;I c,k,m,p;
 // wf and wcr are set
 F2PREFIP;ARGCHK2(a,w);
 if(SPARSE&AT(w))R irs2(ifb(AN(a),BAV(a)),w,0L,1L,wcr,jtfrom);
 m=AN(a);
 void *zvv; void *wvv=voidAV(w); I n=0; // pointer to output area; pointer to input data; number of prefix bytes to skip in first cell
 p=bsum(m,BAV(a));  // p=# 1s in result, i. e. length of result item axis
 if(m==p){RETF(w);}  // if all the bits are 1, we can return very quickly.  It's rare, but so cheap to test for.
 PROD(c,wf,AS(w)); PROD(k,wcr-1,AS(w)+wf+1); // c=#cells, k=#atoms per item of cell
 I zn=c*k*p;  // zn=#atoms in result
 k<<=bplg(AT(w));   // k is now # bytes/cell
 // if the result is inplaceable, AND it is not getting much shorter, keep the same result area
 // We retain the old block as long as the new one is at least half as big, without looking at total size of the allocation,
 // This could result in a very small block's remaining in a large allocation after repeated trimming.  We will accept the risk.
 // Accept only DIRECT blocks so we don't have to worry about explicitly freeing uncopied cells
 if(!ASGNINPLACESGN(SGNIF((I)jtinplace,JTINPLACEWX)&(m-2*p)&(-(AT(w)&DIRECT)),w)) {
  // normal non-in-place copy
    // no overflow possible unless a is empty; nothing  moved then, and zn is 0
  GA(z,AT(w),zn,AR(w),0); MCISH(AS(z),AS(w),AR(w)) // allocate result
  zvv=voidAV(z);  // point to the output area
  // pristine status can be transferred to the result, because we know we are not repeating any cells
  AFLAG(z)|=AFLAG(w)&((SGNTO0(AC(w))&((I)jtinplace>>JTINPLACEWX))<<AFPRISTINEX);  // result pristine if innplaceable input was
 }else{
  z=w; // inplace
  AN(z)=zn;  // Install the correct atom count
  // see how many leading values of the result are already in position.  We don't need to copy them in the first cell
  UI *avv=IAV(a); for(;n<(m>>LGSZI);++n)if(avv[n]!=VALIDBOOLEAN)break;
  // now n has the number of words to skip.  Convert that to bytes, and advance wvv to point to the first cell that may move
  n<<=LGSZI; wvv=(C*)wvv+k*n;
  zvv=CAV(z)+k*n;   // step the output pointer over the initial items left in place
 }
 AS(z)[wf]=p;  // move in length of item axis, #bytes per item of cell
 if(!zn)R z;  // If no atoms to process, return empty

// original  DO(c, DO(m, if(b[i]){MC(zv,wv,k); zv+=k;} wv+=k;);); break;

 
 while(--c>=0){
  // at top of loop n is biased by the number of leading bytes to skip. wvv points to the first byte to process
  UI *avv=(UI*)(CAV(a)+n); n=m-n+((m&(SZI-1))?SZI:0); UI bits=*avv++;  // prime the pipeline for top of loop
  while(n>0){    // where we load bits SZI at a time
   // skip empty words, to get best speed on near-zero a.  This exits with the first unskipped word in bits
   while(bits==0 && n>=(2*SZI)){bits=*avv++; n-=SZI; wvv=(C*)wvv+(k<<LGSZI);}  // fast-forward over zeros.  Always leave 1 word so we have a batch to process
   I batchsize=n>>LGSZI; batchsize=MIN(BB,batchsize);
   UI bitstack=0; while(--batchsize>0){I bits2=*avv++; PACKBITSINTO(bits,bitstack); bits=bits2;};  // keep read pipe ahead
   // Handle the last word of the batch.  It might have non-Boolean data at the end, AFTER the Boolean padding.  Just clear the non-boolean part in this line
   bits&=VALIDBOOLEAN; PACKBITSINTO(bits,bitstack);
   // Now handle the last batch, by discarding garbage bits at the end and then shifting the lead bit down to bit 0
   if(n>=BW+SZI)bits=*avv++;else {n-=n&(SZI-1)?SZI:0; bitstack<<=(BW-n)&(SZI-1); bitstack>>=BW-n;}  // discard invalid trailing bits; shift leading byte to position 0.  For non-last batches, start on next batch
   switch(k){  // copy the words
   case sizeof(C): while(bitstack){I bitx=CTTZI(bitstack); *(C*)zvv=((C*)wvv)[bitx]; zvv=(C*)zvv+k; bitstack&=bitstack-1;} break;
   case sizeof(US): while(bitstack){I bitx=CTTZI(bitstack); *(US*)zvv=((US*)wvv)[bitx]; zvv=(C*)zvv+k; bitstack&=bitstack-1;} break;
#if BW==64
   case sizeof(UI4): while(bitstack){I bitx=CTTZI(bitstack); *(UI4*)zvv=((UI4*)wvv)[bitx]; zvv=(C*)zvv+k; bitstack&=bitstack-1;} break;
#endif
   case sizeof(UI): while(bitstack){I bitx=CTTZI(bitstack); *(UI*)zvv=((UI*)wvv)[bitx]; zvv=(C*)zvv+k; bitstack&=bitstack-1;} break;
   default: while(bitstack){I bitx=CTTZI(bitstack); MC(zvv,(C*)wvv+k*bitx,k); zvv=(C*)zvv+k; bitstack&=bitstack-1;} break;
   }

   wvv=(C*)wvv+(k<<LGBW);  // advance base to next batch of 64
   n-=BW;  // decr count left
  }
  wvv=(C*)wvv+(k*n);  // in case we loop back, back wvv to start of next input area, taking away the part of the last BW section we didn't use
  n=0;  // no bias for cells after the first
 } 

 R z;
}    /* (dense boolean)#"r (dense or sparse) */

static REPF(jtrepbsx){A ai,c,d,e,g,q,x,wa,wx,wy,y,y1,z,zy;B*b;I*dv,*gv,j,m,n,*u,*v,*v0;P*ap,*wp,*zp;
 F2PREFIP;ARGCHK2(a,w);
 ap=PAV(a); e=SPA(ap,e); 
 y=SPA(ap,i); u=AV(y);
 x=SPA(ap,x); n=AN(x); b=BAV(x);
 if(!AN(SPA(ap,a)))R irs2(ifb(n,b),w,0L,1L,wcr,jtfrom);
 if(!*BAV(e)){
  GATV0(q,INT,n,1); v=v0=AV(q); 
  DO(n, if(*b++)*v++=u[i];); 
  AN(q)=AS(q)[0]=v-v0; 
  R irs2(q,w,0L,1L,wcr,jtfrom);
 }
 wp=PAV(w);
 if(DENSE&AT(w)||all0(eq(sc(wf),SPA(wp,a)))){RZ(q=denseit(a)); R irs2(ifb(AN(q),BAV(q)),w,0L,1L,wcr,jtfrom);}  // here if dense w
 wa=SPA(wp,a); wy=SPA(wp,i); wx=SPA(wp,x);
 RZ(q=aslash(CPLUS,a));
 GASPARSE(z,AT(w),1,AR(w),AS(w)); *(wf+AS(z))=m=*AV(q);
 RZ(c=indexof(wa,sc(wf)));
 RZ(y1=fromr(c,wy));
 RZ(q=__not(eps(y1,ravel(repeat(__not(x),y)))));
 m=AS(a)[0]-m;
 GATV0(ai,INT,m,1); v=AV(ai); DO(n, if(!*b++)*v++=u[i];);
 RZ(g=grade1(over(ai,repeat(q,y1)))); gv=AV(g);
 GATV0(d,INT,AN(y1),1); dv=AV(d); j=0; DO(AN(g), if(m>gv[i])++j; else dv[gv[i]-m]=j;);
 RZ(zy=mkwris(repeat(q,wy))); v=AV(zy)+*AV(c); m=AS(zy)[1]; DO(AS(zy)[0], *v-=dv[i]; v+=m;);
 zp=PAV(z);
 SPB(zp,a,ca(wa));
 SPB(zp,e,SPA(wp,e));
 SPB(zp,i,zy);
 SPB(zp,x,repeat(q,wx));
 R z;
}    /* (sparse boolean) #"r (dense or sparse) */

static REPF(jtrepidx){A y;I j,m,p=0,*v,*x;
 F2PREFIP;ARGCHK2(a,w);
 RZ(a=vi(a)); x=AV(a);
 m=AS(a)[0];
 DO(m, ASSERT(0<=x[i],EVDOMAIN); p+=x[i]; ASSERT(0<=p,EVLIMIT););  // add up total # result slots
 GATV0(y,INT,p,1); v=AV(y); 
 DO(m, j=i; DQ(x[j], *v++=j;););  // fill index vector with all the indexes
 A z; R IRS2(y,w,0L,1L,wcr,jtfrom,z);
}    /* (dense  integer) #"r (dense or sparse) */

static REPF(jtrepisx){A e,q,x,y;I c,j,m,p=0,*qv,*xv,*yv;P*ap;
 F2PREFIP;ARGCHK2(a,w);
 ap=PAV(a); e=SPA(ap,e); 
 y=SPA(ap,i); yv=AV(y);
 x=SPA(ap,x); if(!(INT&AT(x)))RZ(x=cvt(INT,x)); xv=AV(x);
 if(!AN(SPA(ap,a)))R repidx(ravel(x),w,wf,wcr);
 if(!*AV(e)){
  m=AN(x);  
  DO(m, ASSERT(0<=xv[i],EVDOMAIN); p+=xv[i]; ASSERT(0<=p,EVLIMIT););
  GATV0(q,INT,p,1); qv=AV(q); 
  DO(m, c=xv[i]; j=yv[i]; DQ(c, *qv++=j;);); 
  R irs2(q,w,0L,1L,wcr,jtfrom);
 }
 ASSERT(0,EVNONCE);
}    /* (sparse integer) #"r (dense or sparse) */


static REPF(jtrep1d){A z;C*wv,*zv;I c,k,m,n,p=0,q,t,*ws,zk,zn;
 F2PREFIP;ARGCHK2(a,w);
 t=AT(a); m=AN(a); ws=AS(w); SETICFR(w,wf,wcr,n);   // n=length of item axis in input.  If atom, is repeated to length of a
 if(t&CMPX){
  if(wcr)R repzdx(from(apv(n,0L,0L),a),w,                wf,wcr);
  else{A za; RZ(za=apv(m,0L,0L)); R repzdx(a,IRS2(za,w,0L,1L,0L,jtfrom,z),wf,1L );}
 }
 if(t&B01){p=bsum(m,BAV(a)); // bsum in case a is big.  Atomic boolean was handled earlier
 }else{I*x; 
  RZ(a=vi(a)); x=AV(a); 
  DO(m, ASSERT(0<=x[i],EVDOMAIN); p+=x[i]; ASSERT(0<=p,EVLIMIT););  // p=#items in result
 }
 DPMULDE(p,n,q);  // q=length of result item  axis.  +/a copies, each of length n
 DPMULDE(p,AN(w),zn);
 GA(z,AT(w),zn,AR(w)+!wcr,0); MCISH(AS(z),AS(w),AR(z)) AS(z)[wf]=q;
 if(!zn)R z;
 wv=CAV(w); zv=CAV(z);
 PROD(c,wf+(I )(wcr!=0),ws); PROD1(k,wcr-1,ws+wf+1); k <<=bplg(AT(w));  // c=#cell-items to process  k=#atoms per cell-item
 zk=p*k;  // # bytes to fill per item
 DQ(c, mvc(zk,zv,k,wv); zv+=zk; wv+=k;);
 R z;
}    /* scalar #"r dense   or   dense #"0 dense */

static B jtrep1sa(J jt,A a,I*c,I*d){A x;B b;I*v;
 b=1&&AT(a)&CMPX;
 if(b)RZ(x=rect(a)) else x=a; 
 if(AR(a)){ASSERT(equ(num(1),aslash(CSTARDOT,le(zeroionei(0),ravel(x)))),EVDOMAIN); RZ(x=aslash(CPLUS,x));} 
 if(!(INT&AT(x)))RZ(x=cvt(INT,x));
 v=AV(x); *c=v[0]; *d=b?v[1]:0;
 ASSERT(0<=*c&&0<=*d,EVDOMAIN);
 R 1;
}    /* process a in a#"0 w */

static REPF(jtrep1s){A ax,e,x,y,z;B*b;I c,d,cd,j,k,m,n,p,q,*u,*v,wr,*ws;P*wp,*zp;
 F2PREFIP;ARGCHK2(a,w);
 if(AT(a)&SCMPX)R rep1d(denseit(a),w,wf,wcr);
 RE(rep1sa(a,&c,&d)); cd=c+d;
 if(DENSE&AT(w))R rep1d(d?jdot2(sc(c),sc(d)):sc(c),w,wf,wcr);  // here if dense w
 wr=AR(w); ws=AS(w); n=wcr?*(wf+ws):1; RE(m=mult(n,cd));
 wp=PAV(w); e=SPA(wp,e); ax=SPA(wp,a); y=SPA(wp,i); x=SPA(wp,x);
 GASPARSE(z,AT(w),1,wr+!wcr,ws); *(wf+AS(z))=m; zp=PAV(z);
 RE(b=bfi(wr,ax,1));
 if(wcr&&b[wf]){    /* along sparse axis */
  u=AS(y); p=u[0]; q=u[1]; u=AV(y);
  RZ(x=repeat(sc(c),x));
  RZ(y=mkwris(repeat(sc(c),y)));
  if(p&&1<c){
   j=0; DO(wf, j+=b[i];); v=j+AV(y);
   if(AN(ax)==1+j){u+=j; DO(p, m=cd**u; u+=q; DO(c, *v=m+i; v+=q;););}
   else{A xx;I h,i,j1=1+j,*uu;
    GATV0(xx,INT,j1,1); uu=AV(xx);
    k=0; DO(j1, uu[i]=u[i];);
    for(i=0;i<p;++i,u+=q)
     if(ICMP(uu,u,j1)||i==p-1){
      h=(I )(i==p-1)+i-k; k=i; m=cd*uu[j]; 
      DO(j1, uu[i]=u[i];);
      DO(h, DO(c, *v=m+i; v+=q;););
     } 
    RZ(xx=grade1(y));
    RZ(x=from(xx,x));
    RZ(y=from(xx,y));
 }}}else{A xx;      /* along dense  axis */
  j=0; DO(wcr, j+=!b[wf+i];);
  RZ(y=ca(y));
  if(d){xx=jt->fill; jt->fill=e;}  // e cannot be virtual
  x=irs2(AR(a)&&CMPX&AT(a)?a:d?jdot2(sc(c),sc(d)):sc(c),x,0L,1L,j,jtrepeat); 
  if(d)jt->fill=xx; 
  RZ(x);
 }
 SPB(zp,e,e);
 SPB(zp,a,ax);
 SPB(zp,i,y);
 SPB(zp,x,x);
 R z;
}    /* scalar #"r sparse   or  sparse #"0 (dense or sparse) */

A (*reptab[])() = {jtrepisx,jtrepidx,jtrepbsx,jtrepbdx,jtrep1s,jtrep1d,jtrepzsx,jtrepzdx};
F2(jtrepeat){A z;I acr,ar,wcr,wf,wr;
 F2PREFIP;ARGCHK2(a,w);
 ar=AR(a); acr=jt->ranks>>RANKTX; acr=ar<acr?ar:acr;
 wr=AR(w); wcr=(RANKT)jt->ranks; wcr=wr<wcr?wr:wcr; wf=wr-wcr; RESETRANK;
I adense=-(AT(a)&DENSE);  // sign set if a is dense
I att=SGNTO0(-(AT(a)&B01+SB01))+((UI)(-(AT(a)&CMPX+SCMPX))>>(BW-1-1));  // 0 if INT/FL 1 if B01 3 if CMPX
 att=(-acr&-wcr)>=0?2:att;  // override with 2 if either a or w is an atom
 adense&=(-acr&-wcr)|-(AT(w)&DENSE);  // if a or w is an atom, require both a and w dense
 A (*repfn)() = reptab[2*att+SGNTO0(adense)];  // fetch address to call

 // special case: if a is atomic 1, and cells of w are not atomic.  a=0 is fast in the normal path
 if(((-wcr)&(ar-1)&(-(AT(a)&(B01|INT))))<0){I aval = BIV0(a);  // no fast support for float; take all of INT, or 1 bit of B01
  if(!(aval&-2LL)){  // 0 or 1
   if(aval==1)R RETARG(w);   // 1 # y, return y
   if(!(AT(w)&SPARSE)){GA(z,AT(w),0,AR(w),0); MCISH(AS(z),AS(w),AR(w)) AS(z)[wf]=0; RETF(z);}  // 0 # y, return empty
  }
 }
 if(((1-acr)|(acr-ar))<0){z=rank2ex(a,w,UNUSED_VALUE,MIN(1,acr),wcr,acr,wcr,jtrepeat); PRISTCLRF(w) RETF(z);}  // multiple cells - must losr pristinity  // loop if multiple cells of a
 ASSERT((-acr&-wcr)>=0||(AS(a)[0]==AS(w)[wf]),EVLENGTH);
 z=(*repfn)(jtinplace,a,w,wf,wcr);
 // mark w not pristine, since we pulled from it
 PRISTCLRF(w)
 RETF(z);
}    /* a#"r w main control */
