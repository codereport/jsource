/* Copyright 1990-2006, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Verbs: Permutations                                                     */

#include "j.h"


static I jtord(J jt,A w){I j,n,*v,z;
 if(!w) return 0;
 n=AN(w); z=-n;
 if(n){if(!(INT&AT(w)))RZ(w=jtcvt(jt,INT,w)); v=AV(w); DQ(n, j=*v++; z=z<j?j:z;); ++z;}
 return z;
}  // the order of the permutation w: max element of w (could be negative)

 A jtpinv(J jt, A w){I m=-1,n,*v;  // empty perm will set m=0
 if (!w) return 0;
 F1RANK(1,jtpinv,UNUSED_VALUE);
 RZ(w=vi(w));
 n=AN(w); v=AV(w);
 DO(n, I r=v[i]^REPSGN(v[i]); m=r>m?r:m;); ++m;  // take 1s-comp of negative ele#, then find max; add 1 to get #eles
 return jtindexof(jt,jtpfill(jt,m,w),IX(m));
}    /* permutation inverse */

// w contains indexes (its shape is immaterial).  n is the length of the axis.  Result is new array, same shape, with equivalent positive indexes
A jtpind(J jt,I n,A w){A z;I j,*v;
 RE(n);
 RZ(z=AT(w)&INT?w:jtcvt(jt,INT,w));  // z is now an INT vector, possibly the input argument
 // Make a quick scan to see if all are positive, as they usually are
 for(j=AN(z), v=IAV(z);j;--j)if((UI)*v++>=(UI)n)break;
 if(j==0)return z;  // if all indices in range, keep the original vector
 // There was an out-of-bounds or negative index.  We may have to modify the index vector.  Reallocate it if we didn't already
 if(z==w)RZ(z=ca(z));  // Create temp area if we don't have one already
 for(j=AN(z), v=IAV(z);j;--j){if((UI)*v>=(UI)n){*v+=n; ASSERT((UI)*v<(UI)n,EVINDEX);} ++v;}  // replace negative indexes
 return z;
}    /* positive indices */

// n is the order of permutation w; w may omit some indexes
// result is full permutation, with the omitted values coming first, in ascending order, followed by w
// if w has negative indexes, they are first made positive
A jtpfill(J jt,I n,A w){PROLOG(0081);A b,z;B*bv;I*wv,*zv;
 RZ(w=jtpind(jt,n,w));  wv=AV(w);  // convert to positive indexes, wv-> indexes
 GATV0(z,INT,n,1); zv=AV(z);  // allocate result area
 GATV0(b,B01,n,1); bv=BAV(b); memset(bv,C1,n);   // binary vector, init to 1
 DO(AN(w), bv[wv[i]]=0;);  // clear flag in indexes that appear
 DO(n, *zv=i; zv+=bv[i];); ASSERT((zv-AV(z))+AN(w)==n,EVINDEX); ICPY(zv,wv,AN(w));  // prefix result with missing indexes; verify the result accounts for all indexes
 EPILOG(z);
}

static A jtcfd(J jt, A w){A b,q,x,z,*zv;B*bv;I c,i,j,n,*qv,*u,*v,zn;
 if(c=1&&INT&AT(w)){
  n=AN(w); v=AV(w);
  GATV0(b,B01,1+n,1); bv=BAV(b); memset(bv,C0,n);
  DO(n, j=v[i]; if((UI)j>=(UI)n||bv[j]){c=0; break;} bv[j]=1;);
 }
 if(!c){n=ord(w); RZ(w=jtpfill(jt,n,w)); v=AV(w); GATV0(b,B01,1+n,1);}
 bv=BAV(b); memset(bv,C0,1+n); ++bv;
 i=0; j=n-1; zn=(I)(log((D)n)+1.6); 
 GATV0(q,INT,n, 1); qv= AV(q);
 GATV0(z,BOX,zn,1); zv=AAV(z);
 while(1){
  while(bv[j])--j; if(0>j)break;
  u=qv; c=j;
  do{bv[c]=1; *u++=c; c=v[c];}while(c!=j);
  if(i==zn){RZ(z=jtext(jt,0,z)); zv=AAV(z); zn=AN(z);}
  RZ(zv[i++]=incorp(vec(INT,u-qv,qv)));
 }
 AN(z)=AS(z)[0]=zn=i; j=zn-1; DO(zn>>1, x=zv[i]; zv[i]=zv[j]; zv[j]=x; --j;);
 AFLAG(z)|=AFPRISTINE;  // what we generated is always pristine
 return z;
}    /* cycle from direct */

static A jtdfc(J jt,I n,A w){PROLOG(0082);A b,q,*wv,z;B*bv;I c,j,qn,*qv,*x;
 RE(n);
 ASSERT(0<=n,EVINDEX);
 GATV0(b,B01,n,1); bv=BAV(b); memset(bv,C1,n);
 RZ(z=apvwr(n,0L,1L)); x=AV(z);
 wv=AAV(w); 
 for(j=AN(w)-1;0<=j;j--){
  RZ(q=jtpind(jt,n,wv[j])); qv=AV(q); qn=AN(q);
  if(!qn)continue;
  DO(qn, ASSERT(bv[qv[i]],EVINDEX); bv[qv[i]]=0;); DO(qn,bv[qv[i]]=1;);
  c=x[qv[0]]; DO(qn-1,x[qv[i]]=x[qv[i+1]];); x[qv[qn-1]]=c;
 }
 EPILOG(z);
}    /* direct from cycle */

 A jtcdot1(J jt, A w){F1RANK(1,jtcdot1,UNUSED_VALUE); return BOX&AT(w)?jtdfc(jt,ord(raze(w)),w):cfd(w);}

 A jtcdot2(J jt,A a,A w){A p;I k;
 F2RANK(1,RMAX,jtcdot2,UNUSED_VALUE);
 SETIC(w,k);
 RZ(p=BOX&AT(a)?jtdfc(jt,k,a):jtpfill(jt,k,a));
 return AR(w)?jtfrom(jt,p,w):w;
}

 A jtpparity(J jt, A w){A x,y,z;B *u;I i,j,k,m,n,p,r,*s,*v,*zv;
 RZ(x=jtcvt(jt,INT,w)); makewritable(x);   // we ALWAYS create a copy of w, because we modify it here
 r=AR(x); s=AS(x); n=AS(x)[r-1]; n=r?n:1; PRODX(m,r-1,s,1); v=AV(x);
 GATV0(y,B01,n,1); u=BAV(y);
 GATV(z,INT,m,r?r-1:0,s); zv=AV(z);
 for(i=0;i<m;++i){
  j=0; p=1; memset(u,C1,n);
  DO(n, k=v[i]; if(0>k)v[i]=k+=n; if((UI)k<(UI)n&&u[k])u[k]=0; else{j=1+n; break;});  // if there are repeated indexes, set j high to force 0 result
  for(;j<n;++j)if(j!=v[j]){k=j; DQ(n-j-1, ++k; if(j==v[k]){v[k]=v[j]; p=-p; break;});}  // if interchange required, find it and do it (half of it, anyway)
  zv[i]=p&(j-n-1);   // return parity or 0
  v+=n;
 }
 return z;
}    /* permutation parity; # interchanges to get i.n */

// reduced form seems to be (>:i.-$y) #: y where y is the permutation number (note there is a redundant 0 at the end)
static A jtdfr(J jt, A w){A z;I c,d,i,j,m,n,*v,*x;
 if(!w) return 0;
 n=AS(w)[AR(w)-1]; PROD(m,AR(w)-1,AS(w)); v=AV(w);  // n=length of each permutation, m=#permutations
 GATV(z,INT,AN(w),AR(w),AS(w)); x=AV(z);
 for(i=0;i<m;++i){   // for each permutation
  DO(n, x[i]=i;);  // initialize an identity permutation
  DO(n-1, j=i; c=x[j+v[j]]; DO(1+v[j], d=x[j+i]; x[j+i]=c; c=d;););   // within the length-j suffix of the permutation, right-rotate the first v[j] elements 
  x+=n; v+=n;
 }
 return z;
}    /* direct from reduced */

static A jtrfd(J jt, A w){A z;I j,k,m,n,r,*s,*x;
 RZ(z=ca(w)); x=AV(z);
 r=AR(w); s=AS(w); PROD(m,r-1,s);
 if(n=s[r-1])DO(m, j=n-1; ++x; DO(n-1, k=0; DO(j--, k+=*x>x[i];); *x++=k;););
 return z;
}    /* reduced from direct */

 A jtadot1(J jt, A w){A y;I n;
 F1RANK(1,jtadot1,UNUSED_VALUE);
 RZ(y=BOX&AT(w)?cdot1(w):jtpfill(jt,ord(w),w));
 SETIC(y,n);
 return jtbase2(jt,jtcvt(jt,XNUM,apv(n,n,-1L)),rfd(y));
}

 A jtadot2(J jt,A a,A w){A m,p;I n;
 SETIC(w,n); p=sc(n); if(XNUM&AT(a))p=jtcvt(jt,XNUM,p); RZ(m=fact(p));
 ASSERT(all1(le(negate(m),a))&&all1(lt(a,m)),EVINDEX);
 if(!AR(w)){RZ(vi(a)); return w;}
 RZ(p=dfr(vi(jtabase2(jt,apv(n,n,-1L),a))));
 return equ(w,IX(n))?p:jtfrom(jt,p,w);  // special case when w is index vector - just return permutation.  Otherwise shuffle items of w
 // pristinity unchanged here: if w boxed, it was set by {
}

