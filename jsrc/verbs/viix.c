/* Copyright 1990-2014, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Verbs: Interval Index, mostly by Binary Search                          */

#include "j.h"


#define BXLOOP(T)  \
 {T*wv=(T*)AV(w),x;                                                  \
  if(descend){DQ(m, x=*wv++; I t=n; t=x>=0?p:t; t=x>=1?0:t; *zv++=t;);        /* p q */  \
  }else if(p<0){DQ(m, *zv++=n*(1<*wv++);) /*   q */  \
  }else if(q<0){DQ(m, *zv++=n*(0<*wv++););   /* p   */  \
  }else{DQ(m, x=*wv++; I t=n; t=x<=1?q:t; t=x<=0?0:t; *zv++=t;) /* p q */  \
  }  \
 }

static B jtiixBX(J jt,I n,I m,A a,A w,I*zv){B*av,*b,descend;I p,q;
 av=BAV(a); descend=av[0]>av[n-1];
 b=memchr(av,C0,n); p=b?b-av:-1;
 b=memchr(av,C1,n); q=b?b-av:-1;
 switch(UNSAFE(AT(w))){
 case INT: BXLOOP(I); break;
 case FL:  BXLOOP(D); break;
 case B01:
  b=BAV(w);
  if(descend){DQ(m, *zv++=p*!*b++;)         /* p q */
  }else if(p<0){memset(zv,C0,m*SZI);  /*   q */
  }else if(q<0){DQ(m, *zv++=n* *b++;)  /* p   */
  }else{DQ(m, *zv++=q* *b++;)  /* p q */
  }
 }
 R 1;
}    /* a I."r w where a is a boolean list */

static B jtiixI(J jt,I n,I m,A a,A w,I*zv){A t;B ascend;I*av,j,p,q,*tv,*u,*v,*vv,*wv,x,y;
 av=AV(a); wv=AV(w);
 p=av[0]; q=av[n-1]; ascend=p<=q; if(!ascend){x=p; p=q; q=x;}
 GATV0(t,INT,1+q-p,1); v=AV(t); tv=v-p; vv=v+AN(t);  // v->buffer; tv->virtual buffer origin, where p=0; vv->buffer end
  // This could be recoded to allocate slots for <p and >q, but it would be better only if those cases were common
 if(ascend){u=av;     x=*u++; *v++=j=0; DQ(n-1, ++j; y=*u++; ASSERT(BETWEENC(y,x,q),EVDOMAIN); DQ(y-x, *v++=j;); x=y;);}
 else      {u=av+n-1; x=*u--;      j=n; DQ(n-1, --j; y=*u--; ASSERT(BETWEENC(y,x,q),EVDOMAIN); DQ(y-x, *v++=j;); x=y;);}
 if(ascend)DQ(m, x=*wv++; *zv++=x<=p?0:q<x?n:tv[x];)
 else      DQ(m, x=*wv++; *zv++=x>=q?0:p>x?n:tv[x];);
 R 1;
}    /* a I. w where a is a list of small range integers */

#define SBCOMP(x,y) (SBGT((x),(y))-SBLT((x),(y)))
#define COMPVLOOP(T,c)       \
 {T*u=(T*)uu,*v=(T*)vv; DQ(c, if(*u!=*v){cc=*u<*v?-1:1; break;} ++u; ++v;);}
#define COMPVLOOF(T,c,COMP)  \
 {T*u=(T*)uu,*v=(T*)vv; DQ(c, if(cc=COMP(*u,*v))break;          ++u; ++v;);}

#define MID(k,p,q)   k=(UI)((p)+(q))>>1  /* beware integer overflow */

#define BSLOOP1(CMP)           \
  p=0; q=n-1; y=*wv++;         \
  while(p<=q){MID(k,p,q); CMP; \
      if(b)q=k-1; else p=k+1;}
// Binary search without misprediction, on atoms
#define BSLOOP1x(CMP)    /* if CMP is true, move q; otherwise p */       \
  y=*wv++; p=0; q=n-1; k=q>>1;         \
  do{   /* empty lists handled earlier */ \
   x=av[k]; /* read the next value */ \
   I p2=k+1; I q1=k-1; I k2; MID(k2,p2,q); MID(k,p,q1); /* calculate new values for kpq depending on which way the compare turns out.  2=upper side (ie if p moved), 1=lower side (if q moved) */\
   k=CMP?k:k2; p=CMP?p:p2; q=CMP?q1:q; /* move k first to start next fetch */\
  }while(q>=p); 

#define BSLOOPN(NE,CMP)        \
  p=0; q=n-1;                  \
  while(p<=q){MID(k,p,q); u=av+c*k; v=wv; b=1; DQ(c, x=*u++; y=*v++; if(NE){CMP; break;}); /* make this compare fixed length? */   \
      if(b)q=k-1; else p=k+1;}
// Without misprediction
#define BSLOOPNx(NE,CMP)    /* if CMP is true, move q; otherwise p */       \
  p=0; q=n-1; k=(q>>1);         \
  do{   /* empty lists handled earlier */ \
   u=av+k*c; v=wv; b=1; DQ(c, x=*u; y=*v; if(NE){b=(CMP); break;} ++u; ++v;);  \
   I p2=k+1; I q1=k-1; I k2; MID(k2,p2,q); MID(k,p,q1);   /* calculate new values for kpq depending on which way the compare turns out.  2=upper side (ie if p moved), 1=lower side (if q moved) */\
   k=b?k:k2; p=b?p:p2; q=b?q1:q; /* move k first to start next fetch */\
  }while(q>=p); 

#define BSLOOP(Ta,Tw)       \
 {Ta*av=(Ta*)AV(a),x; Tw y; Tw*wv=(Tw*)AV(w);                                             \
   if(c==1){if(ge!=1)DQ(m, BSLOOP1x(x>=y); *zv++=q+1;)else DQ(m, BSLOOP1x(x<=y); *zv++=q+1;)}      \
   else{if(ge!=1)DQ(m, Ta* u; Tw *v; BSLOOPNx(x!=y,x>y); *zv++=q+1; wv+=c;)else DQ(m, Ta* u; Tw *v; BSLOOPNx(x!=y,x<y); *zv++=q+1; wv+=c;)}      \
 }

#define BSLOOF(Ta,Tw,COMP)  \
 {Ta*av=(Ta*)AV(a),*u,x;                                              \
  Tw*wv=(Tw*)AV(w),*v,y;                                              \
  if(1==c) DQ(m, BSLOOP1(b=ge!=COMP(av[k],y));   *zv++=1+q;       )   \
  else     DQ(m, BSLOOPN(cc=COMP(x,y),b=gt==cc); *zv++=1+q; wv+=c;);  \
 }

// Combine two types into a single value.  Originally this was 7*s+t which produced
// large values unsuitable for a branch table, and also took advantage of the fact that
// codes produced by multiple combinations, such as LIT,B01 and B01,FL which both produce
// 1111 would not generate spurious accepted cases because only one of them is HOMO.
#define TT(s,t) (((s)&16)<<3)+(((s)&7)<<4)+(((t)&16)>>1)+((t)&7)

F2(jticap2){A*av,*wv,z;C*uu,*vv;I ar,*as,at,b,c,ck,cm,ge,gt,j,k,m,n,p,q,r,t,wr,*ws,wt,* RESTRICT zv;I cc;
 ARGCHK2(a,w);
 ar=AR(a); at=AT(a); as=AS(a); SETIC(a,n); r=ar-1<0?0:ar-1;  // n=length of 1-cell of a, r=frame of a
 wr=AR(w); wt=AT(w); ws=AS(w);
 ASSERT(r<=wr,EVRANK);
 ASSERTAGREE(as+ar-r,ws+wr-r,r)
 ASSERT((POSIFHOMO(at,wt)&-AN(a)&-AN(w))>=0,EVDOMAIN); ASSERT(!((at|wt)&SPARSE),EVNONCE); // if no empties, verify agreement & non-sparse
 CPROD(AN(w),m,wr-r,ws); CPROD(AN(w),c,r,ws+wr-r);  // m=#atoms in result   c=# atoms in a cell of w
 GATV(z,INT,m,wr-r,ws); zv=AV(z);
 if(((m-1)|(n-1)|(c-1))<0){DQ(m, *zv++=0;); R z;}  // exit with zeros for empty args
 t=maxtyped(at,wt);
 if(1==c){  // the search is for atoms
  if(at&B01&&wt&B01+INT+FL){RZ(iixBX(n,m,a,w,zv)); R z;}
  if(at&wt&INT){
   // Integer search.  check for small-range
   UI r=IAV(a)[n-1]-IAV(a)[0]; r=IAV(a)[n-1]<IAV(a)[0]?0-r:r;  // get range, which may overflow I but will stay within UI
   UI4 nlg; CTLZI(n,nlg); nlg=(nlg<<1)+(SGNTO0(SGNIF((n<<1),nlg)));   // approx lg with 1 bit frac precision.  Can't shift 64 bits in case r=1
   if((I)((r>>2)+2*n)<(I)(m*nlg)){RZ(iixI(n,m,a,w,zv)); R z;}  // weight misbranches as equiv to 8 stores
 }}
 jt->workareas.compare.complt=-1; cc=0; uu=CAV(a); vv=CAV(a)+(c*(n-1)<<bplg(at));
 // first decide if the input array is ascending or descending, comparing the first & last items atom by atom
 switch(CTTZ(at)){
  default:   ASSERT(0,EVNONCE);
  case B01X:  COMPVLOOP(B, c);           break;
  case LITX:  COMPVLOOP(UC,c);           break;
  case INTX:  COMPVLOOP(I, c);           break;
  case FLX:   COMPVLOOP(D, c);           break;
  case CMPXX: COMPVLOOP(D, c+c);         break;
  case C2TX:  COMPVLOOP(US,c);           break;
  case C4TX:  COMPVLOOP(C4,c);           break;
  case SBTX:  COMPVLOOF(SB,c, SBCOMP  ); break;
  case XNUMX: COMPVLOOF(X, c, xcompare); break;
  case RATX:  COMPVLOOF(Q, c, qcompare); break;
  case BOXX:  
   av=AAV(a); wv=AAV(w);
   DO(c, if(cc=compare(av[i],av[i+c*(n-1)]))break;);
 }
 ge=cc; gt=-ge;
 switch(TT(CTTZ(at),CTTZ(wt))){
  case TT(B01X, B01X ): BSLOOP(C, C ); break;
  case TT(B01X, INTX ): BSLOOP(C, I ); break;
  case TT(B01X, FLX  ): BSLOOP(C, D ); break;
  case TT(LITX, C2TX ): BSLOOP(UC,US); break;
  case TT(LITX, C4TX ): BSLOOP(UC,C4); break;
  case TT(LITX, LITX ): BSLOOP(UC,UC); break;
  case TT(C2TX, C2TX ): BSLOOP(US,US); break;
  case TT(C2TX, C4TX ): BSLOOP(US,C4); break;
  case TT(C2TX, LITX ): BSLOOP(US,UC); break;
  case TT(C4TX, C2TX ): BSLOOP(C4,US); break;
  case TT(C4TX, C4TX ): BSLOOP(C4,C4); break;
  case TT(C4TX, LITX ): BSLOOP(C4,UC); break;
  case TT(SBTX, SBTX ): BSLOOF(SB,SB, SBCOMP  ); break;
  case TT(INTX, B01X ): BSLOOP(I, C ); break;
  case TT(INTX, INTX ): BSLOOP(I, I ); break;
  case TT(INTX, FLX  ): BSLOOP(I, D ); break;
  case TT(FLX,  B01X ): BSLOOP(D, C ); break;
  case TT(FLX,  INTX ): BSLOOP(D, I ); break;
  case TT(CMPXX,CMPXX): c+=c;  /* fall thru */
  case TT(FLX,  FLX  ): BSLOOP(D, D ); break;
  case TT(XNUMX,XNUMX): BSLOOF(X, X, xcompare); break;
  case TT(RATX, RATX ): BSLOOF(Q, Q, qcompare); break;
  case TT(BOXX, BOXX ):
   for(j=0,cm=c*m;j<cm;j+=c){
    p=0; q=n-1;
    while(p<=q){
     MID(k,p,q); ck=c*k; I b=1; 
     DO(c, if(cc=compare(av[i+ck],wv[i+j])){b=gt==cc; break;});
     if(b)q=k-1; else p=k+1;
    } 
    *zv++=1+q;
   }
   break;
  default:
   ASSERT(TYPESNE(at,wt),EVNONCE);
   if(TYPESNE(t,at))RZ(a=cvt(t,a));
   if(TYPESNE(t,wt))RZ(w=cvt(t,w));
   switch(UNSAFE(t)){
    case CMPX: c+=c;  /* fall thru */ 
    case FL:   BSLOOP(D,D);           break;
    case XNUM: BSLOOF(X,X, xcompare); break;
    case RAT:  BSLOOF(Q,Q, qcompare); break;
    default:   ASSERT(0,EVNONCE);
 }}
 RETF(z);
}    /* a I. w */
