/* Copyright 1990-2007, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Conjunctions: Variants (!.)                                             */

#include "j.h"


static A jtfitct1(J jt,    A w,A self){DECLFG;F1PREFIP;A z; PUSHCCT(FAV(self)->localuse.lD) z=CALL1IP(f1,  w,fs); POPCCT return z;}  // lD has the complementary ct

#define fitctvector(name,vector) static A name(J jt,A a,A w,A self){DECLFG;F2PREFIP;A z; PUSHCCT(FAV(self)->localuse.lD) z=vector; POPCCT return z;}
fitctvector(jtfitct2,CALL2IP(f2,a,w,fs))
fitctvector(jtfitcteq,jtatomic2(jtinplace,a,w,fs))

// To avoid multiple indirect branches, we vector the common comparisons to a routine that jumps directly to them
static const AF aff2[] = {jtfitct2, jtfitcteq};
static A jtfitct(J jt,A a,A w,I cno){V*sv;
 ASSERT(!AR(w),EVRANK);
 sv=FAV(a);
 // Get the tolerance, as a float
 D d; if(w==num(0))d=0.0; else{if(!(AT(w)&FL))RZ(w=jtcvt(jt,FL,w)); d=DAV(w)[0];}  // 0 is usual; otherwise it better be FL, but convert in case its value is 0
 ASSERT(0<=d&&d<5.82076609134675e-11,EVDOMAIN);  // can't be greater than 2^_34
 A fn = fdef(0,CFIT,VERB,(AF)(jtfitct1),aff2[cno],a,w ,0L,sv->flag&(VIRS1|VIRS2|VJTFLGOK1|VJTFLGOK2|VISATOMIC1),(I)(sv->mr),lrv(sv),rrv(sv));  // preserve INPLACE flags
 RZ(fn); FAV(fn)->localuse.lD = 1.0-d; return fn;  // save the fit value in this verb
}

static A jtfitexp2(J jt,A a,A w,A self){
 F2RANK(0,0,jtfitexp2,self);
 ASSERT(0<=i0(w)&&!jt->jerr,EVDOMAIN);
 A z; return jtaslash(jt,CSTAR,plus(a,df2(z,iota(w),FAV(self)->fgh[1],slash(ds(CSTAR)))));
}    /* a ^!.s w */

static A jtfitpoly2(J jt,A a,A w,A self){I j;
 F2RANK(1,0,jtfitpoly2,self);
 A z; return jtaslash(jt,CPLUS,tymes(a,jtascan(jt,CSTAR,shift1(plus(w,df2(z,IX(SETIC(a,j)),FAV(self)->fgh[1],slash(ds(CSTAR))))))));
}    /* a p.!.s w */

static A jtfitfill1(J jt,    A w,A self){DECLFG;F1PREFIP;A z; jt->fill=gs; z=CALL1IP(f1,  w,fs); jt->fill=0; return z;}  // gs cannot be virtual
static A jtfitfill2(J jt,A a,A w,A self){DECLFG;F2PREFIP;A z; jt->fill=gs; z=CALL2IP(f2,a,w,fs); jt->fill=0; return z;}

static A jtfitpp1(J jt,    A w,A self){DECLFG;A z;C d[8],*s=3+jt->pp;
 memcpy(d,s,8L); 
 sprintf(s,FMTI"g",AV(gs)[0]); 
 z=CALL1(f1,w,fs); memcpy(s,d,8L);
 return z;
}

static A jtfitf1(J jt,    A w,A self){V*sv=FAV(self); A z; return df1(z,  w,jtfit(jt,jtfix(jt,sv->fgh[0],zeroionei(0)),sv->fgh[1]));}
static A jtfitf2(J jt,A a,A w,A self){V*sv=FAV(self); A z; return df2(z,a,w,jtfit(jt,jtfix(jt,sv->fgh[0],zeroionei(0)),sv->fgh[1]));}

// Fit conjunction u!.n
// Preserve IRS1/IRS2 from u in result verb (exception: CEXP)
// Preserve VISATOMIC1 from u (applies only to numeric atomic ops)
 A jtfit(J jt,A a,A w){A f;C c;I k,l,m,r;V*sv;
 ASSERTVN(a,w);
 sv=FAV(a); m=sv->mr; l=lrv(sv); r=rrv(sv);
 I cno=0;
 switch(sv->id){
  case CLE: case CLT: case CGE: case CGT: case CNE: case CEQ: cno=1;
  case CMATCH: case CEPS:   case CIOTA:  case CICO:      case CNUB:     case CSTAR:  
  case CFLOOR: case CCEIL:  case CSTILE: case CPLUSDOT:  case CSTARDOT: case CABASE:
  case CNOT:   case CXCO:   case CSLDOT: case CSPARSE:   case CEBAR:
   return fitct(a,w,cno);
  case CEXP:
   ASSERT(AT(w)&NUMERIC,EVDOMAIN);
   return CDERIV(CFIT,0L,jtfitexp2,0L, m,l,r);
  case CPOLY:
   ASSERT(AT(w)&NUMERIC,EVDOMAIN);
   return CDERIV(CFIT,0L,jtfitpoly2,0L, m,l,r);   // CPOLY has no VIRS
  case CPOWOP:  // support for #^:_1!.n
   if(sv->fgh[1]!=num(-1))return fitct(a,w,0);
   f=sv->fgh[0]; c=ID(f);
   if(c==CPOUND){ASSERT(!AR(w),EVRANK); return CDERIV(CFIT,0,jtfitfill2,VFLAGNONE,m,l,r);}  // CPOWOP has no VIRS
   ASSERT(c==CAMP,EVDOMAIN);
   f=FAV(f)->fgh[1]; ASSERT(CPOUND==ID(f),EVDOMAIN);
  case CPOUND:  case CTAKE:  case CTAIL: case CCOMMA:  case CCOMDOT: case CLAMIN: case CRAZE:
   ASSERT(!AR(w),EVRANK);  /* fall thru */
  case CROT: case CDOLLAR: 
   ASSERT(1>=AR(w),EVRANK);
   ASSERT(!AR(w)||!AN(w),EVLENGTH);
   return CDERIV(CFIT,jtfitfill1,jtfitfill2,sv->flag&(VIRS1|VIRS2|VJTFLGOK1|VJTFLGOK2),m,l,r); // preserve INPLACE flags
  case CTHORN:
   RE(w=sc(k=i0(w)));
   ASSERT(0<k,EVDOMAIN);
   ASSERT(k<=NPP,EVLIMIT); 
   return CDERIV(CFIT,jtfitpp1,sv->valencefns[1],0L,m,l,r);  // CTHORN lacks VIRS
  case CCYCLE:
   RE(k=i0(w)); ASSERT(2==k,EVDOMAIN); RZ(w=sc(k));
   return CDERIV(CFIT,jtpparity,0L,0L,m,RMAX,RMAX);  // CCYCLE lacks VIRS
  case CTILDE:
   ASSERT(NOUN&AT(sv->fgh[0]),EVDOMAIN);
   return CDERIV(CFIT,jtfitf1,jtfitf2,0L,m,l,r);  // m~ has no VIRS
  default:
   ASSERT(0,EVDOMAIN);
}}
