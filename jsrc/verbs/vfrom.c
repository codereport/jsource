/* Copyright 1990-2007, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Verbs: From & Associates. See Hui, Some Uses of { and }, APL87.         */

#include "j.h"


F1(jtcatalog){PROLOG(0072);A b,*wv,x,z,*zv;C*bu,*bv,**pv;I*cv,i,j,k,m=1,n,p,*qv,r=0,*s,t=0,*u;
 F1RANK(1,jtcatalog,UNUSED_VALUE);
 if((-AN(w)&-(AT(w)&BOX+SBOX))>=0)R box(w);   // empty or unboxed, just box it
 n=AN(w); wv=AAV(w); 
 DO(n, x=wv[i]; if(AN(x)){p=AT(x); t=t?t:p; ASSERT(HOMO(t,p),EVDOMAIN); RE(t=maxtype(t,p));});  // use vector maxtype; establish type of result
 t=t?t:B01; k=bpnoun(t);  // if all empty, use boolean for result
 GA(b,t,n,1,0);      bv=CAV(b);  // allocate place to build each item of result - one atom from each box.  bv->item 0
 GATV0(x,INT,n,1);    qv=AV(x);   // allocate vector of max-indexes for each box - only the address is used  qv->max-index 0
 GATV0(x,BOX,n,1);    pv=(C**)AV(x);  // allocate vector of pointers to each box's data  pv->box-data-base 0
 RZ(x=apvwr(n,0L,0L)); cv=AV(x);   // allocate vector of current indexes, init to 0  cv->current-index 0
 DO(n, x=wv[i]; if(TYPESNE(t,AT(x)))RZ(x=cvt(t,x)); r+=AR(x); qv[i]=p=AN(x); DPMULDE(m,p,m); pv[i]=CAV(x););  // fill in *qv and *pv; calculate r=+/ #@$@> w, m=*/ */@$@> w
 GATV0(z,BOX,m,r);    zv=AAV(z); s=AS(z);   // allocate result area
 // There is no need to turn off pristinity of w, because nothing was copied out by pointer (everything was copied & then cloned)
 // The result is certainly pristine if it is DIRECT
 AFLAG(z)|=(-(t&DIRECT))&AFPRISTINE;  // set result pristine if boxes DIRECT
 DO(n, x=wv[i]; u=AS(x); DQ(AR(x),*s++=*u++;););   // fill in shape: ; $&.> w
 for(i=0;i<m;i++){  // fill in each box
  bu=bv-k;
  DO(n, MC(bu+=k,pv[i]+k*cv[i],k););  // move in each atom  (we could stop after moving the lowest)
  DO(n, j=n-1-i; if(qv[j]>++cv[j])break; cv[j]=0;);  // increment and roll over the odometer
  RZ(*zv++=ca(b));  // clone the items and move pointer to the result
 }
 EPILOG(z);
}

#define SETNDX(ndxvbl,ndxexp,limexp)    {ndxvbl=(ndxexp); if((UI)ndxvbl>=(UI)limexp){ndxvbl+=(limexp); ASSERT((UI)ndxvbl<(UI)limexp,EVINDEX);}}  // if ndxvbl>p, adding p can never make it OK
#define SETNDXRW(ndxvbl,ndxexp,limexp)    {ndxvbl=(ndxexp); if((UI)ndxvbl>=(UI)limexp){(ndxexp)=ndxvbl+=(limexp); ASSERT((UI)ndxvbl<(UI)limexp,EVINDEX);}}  // this version write to input if the value was negative
#define SETJ(jexp) SETNDX(j,jexp,p)

#define IFROMLOOP(T)        \
 {T   * RESTRICT v=(T*)wv,* RESTRICT x=(T*)zv;  \
  if(1==an){v+=j;   DQ(m,                                    *x++=*v;       v+=p; );}  \
  else              DQ(m, DO(an, SETJ(av[i]);                *x++=v[j];);   v+=p; );   \
 }

// a is not boxed and not boolean (except when a is an atom, which we pass through here to allow a virtual result)
F2(jtifrom){A z;C*wv,*zv;I acr,an,ar,*av,j,k,m,p,pq,q,wcr,wf,wk,wn,wr,*ws,zn;
 F1PREFIP;
 ARGCHK2(a,w);
 // IRS supported.  This has implications for empty arguments.
 ar=AR(a); acr=jt->ranks>>RANKTX; acr=ar<acr?ar:acr;
 wr=AR(w); wcr=(RANKT)jt->ranks; wcr=wr<wcr?wr:wcr; wf=wr-wcr; RESETRANK;
 if(ar>acr)R rank2ex(a,w,UNUSED_VALUE,acr,wcr,acr,wcr,jtifrom);  // split a into cells if needed.  Only 1 level of rank loop is used
 // From here on, execution on a single cell of a (on matching cell(s) of w, or all w).  The cell of a may have any rank
 an=AN(a); wn=AN(w); ws=AS(w);
 if(!(INT&AT(a)))RZ(a=cvt(INT,a));
 // If a is empty, it needs to simulate execution on a cell of fills.  But that might produce error, if w has no
 // items, where 0 { empty is an index error!  In that case, we set wr to 0, in effect making it an atom (since failing exec on fill-cell produces atomic result)
// if(an==0 && wn==0 && ws[wf]==0)wcr=wr=0;     defer this pending analysis
 // If w is empty, portions of its shape may overflow during calculations, and there is no data to move (we still need to
 // audit for index error, though).  If w is not empty, there is no need to check for such overflow.  So we split the computation here.
 // Either way, we need   zn: #atoms in result   p: #items in a cell of w
 p=wcr?ws[wf]:1;
 av=AV(a);  // point to the selectors
 I wflag=AFLAG(w);
 if(wn){
  // For virtual results we need: kn: number of atoms in an item of a cell of w;   
  PROD1(k, wcr-1, ws+wf+1);  // number of atoms in an item of a cell
  // Also m: #wcr-cells in w 
  PROD(m,wf,ws); zn=k*m;  DPMULDE(an,zn,zn);
// correct  if(((zn-2)|-(wf|(wflag&(AFNJA))))>=0){  // zn>1 and not (frame or NJA)
  if(((zn-2)|(wr-2)|-(wf|(wflag&(AFNJA))))>=0){  // zn>1 and not (frame or NJA) and rank>1.  Don't take the time creating a virtual block for a list.  User should use ;.0 for that  NJAwhy
   // result is more than one atom and does not come from multiple cells.  Perhaps it should be virtual.  See if the indexes are consecutive
   I index0 = av[0]; index0+=REPSGN(index0)&p;  // index of first item
   // check the last item before checking the middle.
   I indexn = av[an-1]; indexn+=REPSGN(indexn)&p;
   if(indexn==index0+an-1){
     I indexp=index0; DO(an-1, indexn=av[1+i]; indexn+=REPSGN(indexn)&p; if(indexn!=indexp+1){indexn=p; break;} indexp=indexn;);
   }else indexn=p;
   if((index0|(p-indexn-1))>=0){  // index0>0 and indexn<=p-1
    // indexes are consecutive and in range.  Make the result virtual.  Rank of w cell must be > 0, since we have >=2 consecutive result atoms
    RZ(z=virtualip(w,index0*k,ar+wr-1));
    // fill in shape and number of atoms.  ar can be anything.
    I* as=AS(a); AN(z)=zn; I *s=AS(z); MCISH(s,as,ar) MCISH(s+ar,ws+1,wr-1)
    RETF(z);
   }
  }
  // for copying items, we need    k: size in bytes of an item of a cell of w
  k<<=bplg(AT(w));
 } else {zn=0;}  // No data to move
 // Allocate the result area and fill in the shape
 GA(z,AT(w),zn,ar+wr-(I )(0<wcr),0);  // result-shape is frame of w followed by shape of a followed by shape of item of cell of w; start with w-shape, which gets the frame
 MCISH(AS(z),AS(w),wf); MCISH(&AS(z)[wf],AS(a),ar); if(wcr)MCISH(&AS(z)[wf+ar],1+wf+ws,wcr-1);
 if(!zn){DO(an, SETJ(av[i])) R z;}  // If no data to move, just audit the indexes and quit
 // from here on we are moving items
 wk=k*p;   // stride between cells of w
 wv=CAV(w); zv=CAV(z); SETJ(*av);
  switch(k){
  case sizeof(I):
   IFROMLOOP(I);
   break;
  case sizeof(C): IFROMLOOP(C); break; 
  case sizeof(S): IFROMLOOP(S); break;  
  case sizeof(int):IFROMLOOP(int); break;
  default:
  // cells are not simple types.  We can safely move full words, since there is always extra buffer space at the end of any type that is not a word-multiple
   {C* RESTRICT u,* RESTRICT v=(C*)wv,* RESTRICT x=(C*)zv;
    pq=p*k;
    JMCDECL(endmask) JMCSETMASK(endmask,k+(SZI-1),0) 
    if(1==an){v+=j*k; DQ(m,                     u=v;     JMCR(x,u,k+(SZI-1),loop1,0,endmask) x+=k; v+=pq;);}
    else              DQ(m, DO(an, SETJ(av[i]); u=v+j*k; JMCR(x,u,k+(SZI-1),loop2,0,endmask) x+=k;); v+=pq;)
   }
   break;
  }
  RETF(z);
}    /* a{"r w for numeric a */

#define BSET(x,y0,y1,y2,y3)     *x++=y0; *x++=y1; *x++=y2; *x++=y3;
#define BSETV(b)                MC(v,wv+b*k,k); v+=k;


#define BNNERN(T)       {T*v=(T*)wv,*x=(T*)zv; DQ(m, b=av; DQ(an, *x++=*(v+*b++);); v+=p;);}
#define BNNERM(T,T1)    BNNERN(T)

#define INNER1B(T)  {T*v=(T*)wv,*x=(T*)zv; v+=*av; DQ(m, *x++=*v; v+=p;);}

// a is boolean
static F2(jtbfrom){A z;B*av,*b;C*wv,*zv;I acr,an,ar,k,m,p,q,r,*u=0,wcr,wf,wk,wn,wr,*ws,zn;
 ARGCHK2(a,w);
 ar=AR(a); acr=jt->ranks>>RANKTX; acr=ar<acr?ar:acr;
 wr=AR(w); wcr=(RANKT)jt->ranks; wcr=wr<wcr?wr:wcr; wf=wr-wcr; RESETRANK;
 if(ar>acr)R rank2ex(a,w,UNUSED_VALUE,acr,wcr,acr,wcr,jtbfrom);  // use rank loop if multiple cells of a
 an=AN(a); wn=AN(w); ws=AS(w);
 // If a is empty, it needs to simulate execution on a cell of fills.  But that might produce domain error, if w has no
 // items, where 0 { empty is an index error!  In that case, we set wr to 0, in effect making it an atom (since failing exec on fill-cell produces atomic result)
 p=wcr?ws[wf]:1; q=an>>LGSZI; r=an&(SZI-1);   // p=# items of w
 ASSERT(((p-2)&-(p|an))>=0||(p&all0(a)),EVINDEX);  // OK if p has >1 item, or if it has 0 but an is 0, or is a is all 0 and p has 1 item
 // We always need zn, the number of result atoms
 if(wn){
  // If there is data to move, we also need m: #cells of w   k: #bytes in an items of a cell of w   wk: #bytes in a cell of w
  PROD(m,wf,ws); PROD1(k, wcr-1, ws+wf+1); zn=k*m; k<<=bplg(AT(w)); wk=k*p; DPMULDE(an,zn,zn);
 }else{zn=0;}
 GA(z,AT(w),zn,ar+wr-(I )(0<wcr),0);
 MCISH(AS(z),AS(w),wf); MCISH(&AS(z)[wf],AS(a),ar); if(wcr)MCISH(&AS(z)[wf+ar],1+wf+ws,wcr-1);
 if(!zn)R z;  // If no data to move, just return the shape
 av=BAV(a); wv=CAV(w); zv=CAV(z);
 switch(k+k+(I )(1==an)){
  case   2*sizeof(I): BNNERN(I);   break;
  case   2*sizeof(C): BNNERM(C,I); break; 
  case 1+2*sizeof(C): INNER1B(C);  break;
  case 1+2*sizeof(S): INNER1B(S);  break;
  case 1+2*sizeof(int): INNER1B(int);  break;
  case 1+2*sizeof(I): INNER1B(I);  break;
  default:
   if(1==an){wv+=k**av; DQ(m, MC(zv,wv,k); zv+=k; wv+=wk;);}

   else DQ(m, b=av; DQ(an, MC(zv,wv+k**b++,k); zv+=k;); wv+=wk;);
 }
 RETF(z);
}    /* a{"r w for boolean a */

// a is array whose 1-cells are index lists, w is array
// result is the indexed items
// the numbers in a have been audited for validity
// w is length of the frame
A jtfrombu(J jt,A a,A w,I wf){F1PREFIP;A p,q,z;I ar,*as,h,m,r,*u,*v,wcr,wr,*ws;
 ar=AR(a); as=AS(a); h=as[ar-1];  // h is length of the index list, i. e. number of axes of w that disappear during indexing
 wr=AR(w); ws=AS(w); wcr=wr-wf;
 if((-AN(a)&-AN(w))>=0){  // empty array, either a or w
  // allocate empty result, move in shape: frame of w, frame of a, shape of item
  GA(z,AT(w),0,wf+(wcr-h)+(ar-1),0); MCISH(AS(z),AS(w),wf) MCISH(AS(z)+wf,AS(a),ar-1)  MCISH(AS(z)+wf+ar-1,AS(w)+wf+h,wcr-h)
  R z;
 }
 fauxblockINT(pfaux,4,1); fauxINT(p,pfaux,h,1) v=AV(p)+h; u=ws+wf+h; m=1; DQ(h, *--v=m; m*=*--u;);  // m is number of items in the block of axes that index into w
 r=wr+1-h;  // rank of intermediate w arg is rank of w, minus h axes that go away and are replaced by 1 axis
 // We will use pdt to create an index to the cell
 A ind; RZ(ind=pdt(a,p));
 PROLOG(777);
 if(r==wr){
  q=w;
 }else{
  //  reshape w to combine the first h axes of each cell.  Be careful with the copying, because w and q may be the same block.  Some axes may be closed up by the copy
  RZ(q=virtualip(w,0,r)); AN(q)=AN(w); v=AS(q); MCISH(v,ws,wf); v[wf]=m; MCISH(v+wf+1,ws+wf+h,wcr-h);  /* q is reshape(.,w) */
 }
 IRS2(ind,q,0, RMAX,wcr+1-h,jtifrom,z);
 EPILOG(z);  // we have to release the virtual block so that w is inplaceable later on in the sentence
}    /* (<"1 a){"r w, dense w, integer array a */

#define AUDITPOSINDEX(x,lim) if(!BETWEENO((x),0,(lim))){if((x)<0)break; ASSERT(0,EVINDEX);}
// a is boxed list, w is array, wf is frame of operation, *ind will hold the result
// if the opened boxes have contents with the same item shape (treating atoms as same as singleton lists), create an array of all the indexes; put that into *ind and return 1.
// otherwise return 0
B jtaindex(J jt,A a,A w,I wf,A*ind){A*av,q,z;I an,ar,c,j,k,t,*u,*v,*ws;
 ARGCHK2(a,w);
 an=AN(a); *ind=0;
 if(!an)R 0;
 ws=wf+AS(w); ar=AR(a); av=AAV(a);  q=av[0]; c=AN(q);   // q=addr, c=length of first box
 if(!c)R 0;
 ASSERT(c<=AR(w)-wf,EVLENGTH);
 GATV(z,INT,an*c,1+ar,AS(a)); AS(z)[ar]=c; v=AV(z);  // allocate array for result
 for(j=0;j<an;++j){
  q=av[j]; t=AT(q);
  if(t&BOX)R 0;   // if empty boxed array, error
  if(!(t&INT))RZ(q=cvt(INT,q));  // if can't convert to INT, error
  if((((c^AN(q))-1)&(AR(q)-2))>=0)R 0;   // if not the same length, or rank>1, error
  u=AV(q);
  DO(c, SETNDX(k,u[i],ws[i]) *v++=k;);   // copy in the indexes, with correction for negative indexes
 }
 *ind=z;
 R 1;
}    /* <"1 a to a where a is an integer index array */

static B jtaindex1(J jt,A a,A w,I wf,A*ind){A z;I c,i,k,n,t,*v,*ws;
 ARGCHK2(a,w);
 n=AN(a); t=AT(a); *ind=0; if(AR(a)==0)R 0;  // revert to normal code for atomic a
 ws=wf+AS(w); c=AS(a)[AR(a)-1];   // c = length of 1-cell
 if(((n-1)|(c-1)|SGNIF(t,BOXX))<0)R 0;  // revert to normal code for empty or boxed a
 ASSERT(c<=AR(w)-wf,EVLENGTH);
 PROD(n,AR(a)-1,AS(a));  v=AV(a); // n now=number of 1-cells of a   v=running pointer through a
 // Go through a fast verification pass.  If all values nonnegative and valid, return original a
 if(t&INT){  // if it's INT already, we don't need to move it.
  switch(c){I c0,c1,c2;
  case 2:
   c0=ws[0], c1=ws[1]; for(i=n;i>0;--i){AUDITPOSINDEX(v[0],c0) AUDITPOSINDEX(v[1],c1)  v+=2;} break;
  case 3:
   c0=ws[0], c1=ws[1], c2=ws[2]; for(i=n;i>0;--i){AUDITPOSINDEX(v[0],c0) AUDITPOSINDEX(v[1],c1) AUDITPOSINDEX(v[2],c2) v+=3;} break;
  default:
   for(i=n;i>0;--i){DO(c, k=*v; AUDITPOSINDEX(k,ws[i]) ++v;); if(k<0)break;} break; 
  }
 }else i=1;  // if not INT to begin with, we must convert
 if(i==0){z=a;  // If all indexes OK, return the original block
 }else{
  // There was a negative index.  Allocate a new block for a and copy to it.
  RZ(z=t&INT?ca(a):cvt(INT,a));  v=AV(z);
  DQ(n, DO(c, SETNDXRW(k,*v,ws[i]) ++v;););  // convert indexes to nonnegative & check for in-range
 }
 *ind=z;
 R 1;
}    /* verify that <"1 a is valid for (<"1 a){w - used only for sparse matrices */

static A jtafrom2(J jt,A p,A q,A w,I r){A z;C*wv,*zv;I d,e,j,k,m,n,pn,pr,* RESTRICT pv,
  qn,qr,* RESTRICT qv,* RESTRICT s,wf,wr,* RESTRICT ws,zn;
 wr=AR(w); ws=AS(w); wf=wr-r; 
 pn=AN(p); pr=AR(p); pv=AV(p);
 qn=AN(q); qr=AR(q); qv=AV(q);
 if(AN(w)){
  // Set zn=#atoms of result  d=#atoms in a _2-cell of cell of w
  // e=length of axis corresponding to q  n=#_2-cells in a cell of w   m=#cells of w (frame*size of 2-cell*(# _2-cells = pn*qn))
  PROD(m,wf,ws); PROD(d,r-2,ws+wf+2); e=ws[1+wf]; n=e*ws[wf]; DPMULDE(qn,d*m,zn) DPMULDE(pn,zn,zn);
 }else{zn=0;}
 GA(z,AT(w),zn,wf+pr+qr+r-2,ws);
 s=AS(z)+wf; MCISH(s,AS(p),pr); MCISH(s+pr,AS(q),qr); MCISH(s+pr+qr,ws+wf+2,r-2);
 if(!zn)R z;  // If no data to move, exit with empty.  Rank is right
 wv=CAV(w); zv=CAV(z); 
 switch(k=d<<bplg(AT(w))){   // k=*bytes in a _2-cell of a cell of w
#define INNER2(T) {T* RESTRICT v=(T*)wv,* RESTRICT x=(T*)zv;   \
   DQ(m, DO(pn, j=e*pv[i]; DO(qn, *x++=v[j+qv[i]];         )); v+=n;);} break; // n=#_2-cells in a cell of w.
 case sizeof(I): INNER2(I);
 case sizeof(C): INNER2(C);
 case sizeof(S): INNER2(S);
 case sizeof(I4): INNER2(I4);

 default:        {C* RESTRICT v=wv,* RESTRICT x=zv-k;n=k*n;   // n=#bytes in a cell of w
  JMCDECL(endmask) JMCSETMASK(endmask,k+(SZI-1),0) 
  DQ(m, DO(pn, j=e*pv[i]; DO(qn, x+=k; JMCR(x,v+k*(j+qv[i]),k+(SZI-1),loop1,0,endmask);)); v+=n;);} break;
 }
 RETF(z);   // return block
}   /* (<p;q){"r w  for positive integer arrays p,q */

// n is length of axis, w is doubly-unboxed selector
// result is list of selectors - complementary if w is boxed, with a: standing for axis taken in full
static A jtafi(J jt,I n,A w){A x;
 if((-AN(w)&SGNIF(AT(w),BOXX))>=0)R pind(n,w);   // empty or not boxed
 ASSERT(!AR(w),EVINDEX);  // if boxed, must be an atom
 x=AAV(w)[0];
 R AN(x)?less(IX(n),pind(n,x)):ds(CACE);   // complementary
}

// general boxed a
static F2(jtafrom){PROLOG(0073);A c,ind,p=0,q,*v,y=w;B bb=1;I acr,ar,i=0,j,m,n,pr,*s,t,wcr,wf,wr;
 ARGCHK2(a,w);
 ar=AR(a); acr=jt->ranks>>RANKTX; acr=ar<acr?ar:acr;
 wr=AR(w); wcr=(RANKT)jt->ranks; wcr=wr<wcr?wr:wcr; wf=wr-wcr; RESETRANK;
 if(ar){  // if there is an array of boxes
  if(((ar^acr)|(wr^wcr))==0){RE(aindex(a,w,wf,&ind)); if(ind)R frombu(ind,w,wf);}  // if boxing doesn't contribute to shape, open the boxes of a and copy the values
  R wr==wcr?rank2ex(a,w,UNUSED_VALUE,0L,wcr,0L,wcr,jtafrom):  // if a has frame, rank-loop over a
      df2(p,IRS1(a,0L,acr,jtbox,c),IRS1(w,0L,wcr,jtbox,ind),amp(ds(CLBRACE),ds(COPE)));  // (<"0 a) {&> <"0 w
 }
 c=AAV(a)[0]; t=AT(c); SETIC(c,n); v=AAV(c);   // B prob not reqd 
 s=AS(w)+wr-wcr;
 ASSERT(1>=AR(c),EVRANK);
 ASSERT(n<=wcr,EVLENGTH);
 if((-n&SGNIFNOT(t,BOXX))<0){RE(aindex(a,w,wf,&ind)); if(ind)R frombu(ind,w,wf);}  // not empty and not boxed, handle as 1 index list
 if(wcr==wr){
  for(i=m=pr=0;i<n;++i){
   p=afi(s[i],v[i]);
   if(!(p&&(((AN(p)^1)-1)&-(AT(p)&NUMERIC))<0))break;  // if 1 selection from numeric axis, do selection here, by adding offset to selected cell
   pr+=AR(p); 
   RANKT rsav=AR(p); AR(p)=0; I px; PRODX(px,wcr-i-1,1+i+s,i0(p)) m+=px; AR(p)=rsav;  // kludge but easier than creating a fauxvirtual block
  }
 }
 if(i){I*ys;
  RZ(y=virtual(w,m,pr+wcr-i));
  ys=AS(y); DO(pr, *ys++=1;); MCISH(ys,s+i,wcr-i);
  I temp; PROD(temp,AR(y),AS(y)); AN(y)=temp;
 }
 // take remaining axes 2 at a time, properly handling omitted axes (ds(CACE)).  First time through p is set if there has been no error
 for(;i<n;i+=2){
  j=1+i; if(!p)p=afi(s[i],v[i]); q=j<n?afi(s[j],v[j]):ds(CACE); if(!(p&&q))break;  // pq are 0 if error.  Result of ds(CACE)=axis in full
  if(p!=ds(CACE)&&q!=ds(CACE)){y=afrom2(p,q,y,wcr-i);}
  else{
   if(q==ds(CACE)){q=p; j=i;}
   if(q!=ds(CACE))y=IRS2(q,y,0L,AR(q),wcr-j,jtifrom,w);
  }
  RZ(y); p=0;
 }
 // We have to make sure that a virtual NJA block does not become the result, because x,y and x u}y allow modifying those even when the usecount is 1.  Realize in that case
 RE(y); if(AFLAG(y)&AFNJA){RZ(y=ca(y));}
 EPILOG(y);   // If the result is NJA, it must be virtual.  NJAwhy can it happen?  scaf
}    /* a{"r w for boxed index a */

F2(jtfrom){I at;A z;
 F2PREFIP;
 ARGCHK2(a,w);
 at=AT(a);
 if(likely(!((AT(a)|AT(w))&(SPARSE)))){
  // if B01|INT|FL atom { INT|FL|BOX array, and no frame, just pluck the value.  If a is inplaceable and not unincorpable, use it
  // If we turn the result to BOX it will have the original flags, i. e. it will be nonrecursive.  Thus fa will not free the contents, which do not have incremented usecount (and are garbage on error)
  if(!((AT(a)&(NOUN&~(B01|INT|FL)))+(AT(w)&(NOUN&~(INT|FL|BOX)))+AR(a)+(SGNTO0((((RANKT)jt->ranks-AR(w))|(AR(w)-1))))+(AFLAG(w)&AFNJA))){   // NJAwhy
   I av;  // selector value
   if(AT(a)&(B01|INT)){av=BIV0(a);  // INT index
   }else{  // FL index
    D af=DAV(a)[0], f=jround(af); av=(I)f;
    ASSERT(f==af || FFIEQ(f,af),EVDOMAIN);  // if index not integral, complain.  IMAX/IMIN will fail presently.  We rely on out-of-bounds conversion to peg out one side or other (standard violation)
   }
   I wr1=AR(w)-1;
   if(wr1<=0){  // w is atom or list, result is atom
    // Get the area to use for the result: the a input if possible, else an INT atom
    if((SGNIF(jtinplace,JTINPLACEAX)&AC(a)&SGNIFNOT(AFLAG(a),AFUNINCORPABLEX))<0)z=a; else{GAT0(z,INT,1,0)}
    // Move the value and transfer the block-type
    I j; AT(z)=AT(w); SETNDX(j,av,AN(w)); IAV(z)[0]=IAV(w)[j];
    // Here we transferred an atom out of w.  We must mark w non-pristine.  If it was inplaceable, we can transfer the pristine status.  We overwrite w because it is no longer in use
    PRISTXFERF(z,w)  // this destroys w
   }else{
    // rank of w > 1, return virtual cell
    I *ws=AS(w);  // shape of w
    I m; PROD(m,wr1,ws+1);  // number of atoms in a cell
    I j; SETNDX(j,av,ws[0]);  // j=positive index
    RZ(z=virtualip(w,j*m,wr1));   // if w is rank 2, could reuse inplaceable a for this virtual block
    // fill in shape and number of atoms.  ar can be anything.
    AN(z)=m; MCISH(AS(z),ws+1,wr1)
    // When we create a virtual block we do not actually copy anything out of w, so it remains pristine.  The result is not.
   }
  }else{
   // not atom{array.  Process according to type of a
   A (*fn)(J,A,A);
   fn=jtifrom; fn=at&BOX?jtafrom:fn; fn=at&(AN(a)!=1)?jtbfrom:fn; jtinplace=fn!=jtifrom?jt:jtinplace;
   z=(*fn)(jtinplace,a,w);
   // Here we transferred out of w.  We must mark w non-pristine.  Since there may have been duplicates, we cannot mark z as pristine.  We overwrite w because it is no longer in use
   PRISTCLRF(w)
  }
 }else if(!((AT(a)|AT(w))&(NOUN&~SPARSE))){z=fromss(a,w);}  // sparse cases
 else if(AT(w)&SPARSE){z=at&BOX?frombs(a,w) : fromis(a,w);}
 else{z=fromsd(a,w);}
 RETF(z);
}   /* a{"r w main control */

F2(jtsfrom){
 if(!(SPARSE&AT(w))){
  // Not sparse.  Verify the indexes are numeric and not empty
  if(((AN(a)-1)|(AR(a)-2)|((AT(a)&NUMERIC)-1))>=0){A ind;   // a is an array with rank>1 and numeric.  Rank 1 is unusual & unimportant & we'll ignore it
   // Check indexes for validity; if valid, turn each row into a cell offset
   if(ind=celloffset(w,a)){
    // Fetch the cells and return.  ind is now an array of cell indexes.  View w as an array of those cells
    // We could do this with ifrom, but it validates the input and checks for virtual, neither of which is germane here.  Also, we would have
    // to reshape w into an array of cells.  Easier just to copy the data right here
    // Find the number of axes included in each cell offset; get the cell size
    I cellsize; PROD(cellsize,AR(w)-AS(a)[AR(a)-1],AS(w)+AS(a)[AR(a)-1]);  // number of atoms per index in ind
    I * RESTRICT iv=AV(ind);  // start of the cell-index array
    A z; GA(z,AT(w),cellsize*AN(ind),AR(ind)+AR(w)-AS(a)[AR(a)-1],0)  MCISH(AS(z),AS(ind),AR(ind)) MCISH(AS(z)+AR(ind),AS(w)+AS(a)[AR(a)-1],AR(w)-AS(a)[AR(a)-1])  // shape from ind, then w
    cellsize <<= bplg(AT(w));   // change cellsize to bytes
    switch(cellsize){
    case sizeof(C):
     {C * RESTRICT zv=CAV(z); C *RESTRICT wv=CAV(w); DQ(AN(ind), *zv++=wv[*iv++];) break;}  // scatter-copy the data
    case sizeof(I):  // may include D
     {I * RESTRICT zv=IAV(z); I *RESTRICT wv=IAV(w); DQ(AN(ind), *zv++=wv[*iv++];) break;}  // scatter-copy the data, 8-byte chunks
    default: ;
     // It is OK to pad to an I boundary, because any block with cells not a multiple of I is padded to an I
     C* RESTRICT zv=CAV(z); C *RESTRICT wv=CAV(w);     JMCDECL(endmask) JMCSETMASK(endmask,cellsize+(SZI-1),0) 
     DQ(AN(ind), JMCR(zv,wv+*iv++*cellsize,cellsize+(SZI-1),loop1,0,endmask); zv+=cellsize;)  // use memcpy
     break;
    }
    RETF(z);
   }
  }
 }else{A ind;
  // sparse.  See if we can audit the index list.  If we can, use it, else execute the slow way
  RE(aindex1(a,w,0L,&ind)); if(ind)R frombsn(ind,w,0L);
 }
 // If we couldn't handle it as a special case, do it the hard way
 A z; RETF(from(IRS1(a,0L,1L,jtbox,z),w));
}    /* (<"1 a){w */

static F2(jtmapx);
static EVERYFS(mapxself,0,jtmapx,0,VFLAGNONE)

static F2(jtmapx){A z1,z2,z3;
 ARGCHK2(a,w);
 if(!(BOX&AT(w)))R ope(a);
 RZ(z1=catalog(every(shape(w),ds(CIOTA))));  // create index list of each box
 IRS1(z1,0,0,jtbox,z2);
 RZ(z2=every2(a,z2,(A)&sfn0overself));
 IRS1(z2,0,0,jtbox,z3);
 R every2(z3,w,(A)&mapxself);
}

F1(jtmap){R mapx(ds(CACE),w);}

// extract the single box a from w and open it.  Don't mark it no-inplace.  If w is not boxed, it had better be an atom, and we return it after auditing the index
static F2(jtquicksel){I index;
 RE(index=i0(a));  // extract the index
 SETNDX(index,index,AN(w))   // remap negative index, check range
 R AT(w)&BOX?AAV(w)[index]:w;  // select the box, or return the entire unboxed w
}

F2(jtfetch){A*av, z;I n;F2PREFIP;
 F2RANKW(1,RMAX,jtfetch,UNUSED_VALUE);  // body of verb applies to rank-1 a, and must turn pristine off if used higher, since there may be repetitions.
 if(!(BOX&AT(a))){
  // look for the common special case scalar { boxed vector.  This path doesn't run EPILOG
  if(((AT(w)>>BOXX)&1)>=(2*AR(a)+AR(w))){  // a is an atom, w is atom or list of boxes   AR(a)==0 && (AR(w)==0 || (AR(w)==1 && AT(w)&BOX))
   RZ(z=jtquicksel(jt,a,w));  // fetch selected box, opened.  If not a box, just return w
   // if the result is w, leave its inplaceability.  If contents, it will perforce be uninplaceable
#if AUDITBOXAC
   if(!(AFLAG(w)&AFVIRTUALBOXED)&&AC(z)<0)SEGFAULT;
#endif
   // Since the whole purpose of fetch is to copy one contents by address, we turn off pristinity of w
   PRISTCLRF(w)
   RETF(z);   // turn off inplace if w not inplaceable, or jt not inplaceable.
  }
  RZ(a=jtbox(JTIPAtoW,a));  // if not special case, box any unboxed a
 }
 n=AN(a); av=AAV(a); 
 if(!n)R w; z=w;
 DO(n, A next=av[i]; if(((AT(z)>>BOXX)&1)>=(2*(AR(next)+(AT(next)&BOX))+AR(z))){RZ(z=jtquicksel(jt,next,z))}  // next is unboxed atom, z is boxed atom or list, use fast indexing  AR(next)==0 && !(AT(next)&BOX) && (AR(z)==0 || (AR(z)==1 && AT(z)&BOX))
      else{RZ(z=afrom(box(next),z)); ASSERT(((i+1-n)&-AR(z))>=0,EVRANK); if(((AR(z)-1)&SGNIF(AT(z),BOXX))<0)RZ(z=ope(z));}  // Rank must be 0 unless last; open if boxed atom
   );
 // Since the whole purpose of fetch is to copy one contents by address, we turn off pristinity of w
 PRISTCLRF(w)
 RETF(z);   // Mark the box as non-inplaceable, as above
}
