/* Copyright 1990-2010, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Adverbs: Inverse & Identity Functions                                   */

#include "j.h"


static A jtinvamp(J jt, A w);

static B ip(A w,C c,C d){A f,g;V*v;
 v=FAV(w); f=v->fgh[0]; g=v->fgh[1];
 return CSLASH==ID(f)&&c==ID(FAV(f)->fgh[0])&&d==ID(g);
}

static B consf(A w){A f;C c;
 c=ID(w);
 if(c==CFCONS||c==CQQ&&(f=FAV(w)->fgh[0],NOUN&AT(f)))return 1;
 return 0;
}    /* 1 iff w is a constant function */

static A jtfong(J jt,A a,A w){A f;C c;V*v;
 v=FAV(a); c=v->id; f=v->fgh[0];
 return c==CRIGHT ? w : c==CFORK&&(NOUN&AT(f)||CCAP==ID(f)) ? folk(f,v->fgh[1],jtfong(jt,v->fgh[2],w)) : folk(ds(CCAP),a,w);
}   // [: f g  with simplifications: [: ] w -> w;  [: (N/[: x y) w -> N/[: x [: y w   and y omittrd if ]

static A jtinvfork(J jt, A w){A f,fi,g,gi,h,k;B b,c;V*v;
 v=FAV(w); RZ(f=unname(v->fgh[0])); g=v->fgh[1]; RZ(h=unname(v->fgh[2]));
 if(CCAP==ID(f))return jtfong(jt,invrecur(h),invrecur(g));
 c=1&&NOUN&AT(f); b=c||consf(f);
 ASSERT(b!=consf(h),EVDOMAIN);
 RZ(k=c?f:df1(gi,num(0),b?f:h));
 RZ(gi=invrecur(b?jtamp(jt,k,g):jtamp(jt,g,k)));
 RZ(fi=invrecur(b?h:f));
 if(CAMP==ID(gi)){
  v=FAV(gi); 
  if     (NOUN&AT(v->fgh[0]))RZ(gi=folk(v->fgh[0],     v->fgh[1], ds(CRIGHT)))
  else if(NOUN&AT(v->fgh[1]))RZ(gi=folk(v->fgh[1],swap(v->fgh[0]),ds(CRIGHT)));
 }
 return jtfong(jt,fi,gi);
}

static A jtexpandf(J jt,    A w,A self){A f; f=FAV(self)->fgh[0]; return jtexpand(jt,VAV(f)->fgh[0],w);}

static A jtexpandg(J jt,    A w,A self){A f,g,z;V*v;
 f=FAV(self)->fgh[0]; v=FAV(f); g=v->fgh[1];
 jt->fill=FAV(g)->fgh[1]; z=jtexpand(jt,v->fgh[0],w); jt->fill=0;   // elements of FAV cannot be virtual
 return z;
}

static A jtdiag(J jt,A a,A w){I d,m,p,r,t,*v;
 r=AR(w); t=AT(w);
 v=AS(w);   m=0;      DO(r, m=MIN(m,v[i]););
 v=AS(w)+r; p=1; d=0; DQ(r, d+=p; p*=*--v;);
 if(TYPESNE(t,AT(a)))RZ(a=jtcvt(jt,t,a));
 if(AR(a)){
  ASSERT(m==AN(a),EVLENGTH);
  ASSERT(0,EVNONCE);
 }else{
  ASSERT(0,EVNONCE);
}}

static A jtbminv(J jt, A w){A*wv,x,z=w;I i,j,m,r,*s,t=0,*u,**v,*y,wn,wr,*ws;
 ASSERT(0,EVNONCE);
 ASSERT(BOX&AT(w),EVDOMAIN);
 wn=AN(w); wr=AR(w); ws=AS(w); wv=AAV(w); 
 if(1>=wr)return raze(w);
 if(!wn)return iota(jtreshape(jt,sc(wr),num(0)));
 GATV0(x,INT,wr,1); u=AV(x); memset(u,C0,wr*SZI);
 GATV0(x,INT,wr,1); v=(I**)AV(x);
 DO(wr, m=ws[i]; GATV0(x,INT,m,1); memset(v[i]=AV(x),CFF,m*SZI););
 for(i=0;i<wn;++i){
  x=wv[i]; r=AR(x); s=AS(x);
  if(AN(x)){if(!t)t=AT(x); ASSERT(HOMO(t,AT(x)),EVDOMAIN);}
  ASSERT(2>r||r==wr,EVRANK);
  if(2>r)z=0;
  else DO(wr, y=v[i]+u[i]; if(0>*y)*y=s[i]; else ASSERT(*y==s[i],EVLENGTH););
  j=wr; while(1){--j; ++u[j]; if(ws[j]>u[j])break; u[j]=0;}
 }
 if(!z){A f,h,*zv;I*hv;
  GATVR(z,BOX,wn,2,ws); zv=AAV(z);
  GATV0(h,INT,wr,1); hv=AV(h);
  GA(f,t,1,1,0); RZ(f=filler(f)); memset(u,C0,wr*SZI);
  for(i=0;i<wn;++i){
   zv[i]=x=wv[i];
   if(2>AR(x)){DO(wr, hv[i]=*(v[i]+u[i]);); RZ(zv[i]=jtdiag(jt,x,jtreshape(jt,h,f)));}
   j=wr-1; while(1){--j; ++u[j]; if(ws[j]>u[j])break; u[j]=0;}
 }}
 DO(wr, A t=z; RZ(df1(z,t,slash(jtunder(jt,qq(ds(CCOMMA),sc(wr-i)),ds(COPE))))););
 return ope(z);
}    /* <;.1 or <;.2 inverse on matrix argument */


static A jtinvamp(J jt, A w){A f,ff,g,h,x,y;B nf,ng;C c,d,*yv;I n;V*u,*v;
 v=FAV(w);
 f=v->fgh[0]; nf=!!(NOUN&AT(f));
 g=v->fgh[1]; ng=!!(NOUN&AT(g));
 h=nf?g:f; x=nf?f:g; c=ID(h); u=VAV(h);   // h=verb arg, x=noun arg
 switch(c){
  case CPLUS:    return jtamp(jt,negate(x),h);
  case CSTAR:    return jtamp(jt,recip(x), h);
  case CMINUS:   return nf?w:jtamp(jt,x,ds(CPLUS));
  case CDIV:     return nf?w:jtamp(jt,x,ds(CSTAR));
  case CROOT:    return jtamp(jt,ds(nf?CEXP:CLOG),x);
  case CEXP:     return ng&&equ(x,num(2))?ds(CROOT):jtamp(jt,x,ds(nf?CLOG:CROOT));
  case CLOG:     return nf?jtamp(jt,x,ds(CEXP)):jtamp(jt,ds(CROOT),x);
  case CJDOT:    return nf?jtatop(jt,invrecur(ds(CJDOT)),jtamp(jt,ds(CMINUS),x)):jtamp(jt,ds(CMINUS),jdot1(x));
  case CRDOT:    return nf?jtatop(jt,invrecur(ds(CRDOT)),jtamp(jt,ds(CDIV  ),x)):jtamp(jt,ds(CDIV  ),rdot1(x));
  case CLBRACE:  return nf?jtamp(jt,pinv(x),h):jtamp(jt,x,ds(CIOTA));
  case COBVERSE: ff=FAV(h)->fgh[1]; return jtamp(jt,nf?x:ff,nf?ff:x);
  case CPDERIV:  if(nf&&!AR(x))return ds(CPDERIV); break;  // only atom&p.. is invertible
  case CXCO:     RE(n=i0(x)); ASSERT(n&&BETWEENC(n,-2,2),EVDOMAIN);  // fall through to create (-x)&u
  case CROT:          // fall through to create (-x)&u
  case CCIRCLE:       // fall through to create (-x)&u
  case CSPARSE:  if(nf)return jtamp(jt,negate(x),h);   break;
  case CABASE:   if(nf)return jtamp(jt,x,ds(CBASE));   break;
  case CIOTA:    if(nf)return jtamp(jt,ds(CLBRACE),x); break;
  case CTHORN:   if(nf)return ds(CEXEC);          break;
  case CTILDE:   
   if(ff=FAV(h)->fgh[0],VERB&AT(ff))return invamp(jtamp(jt,nf?ff:x,nf?x:ff));
   else{ff=unname(h); return invamp(jtamp(jt,nf?x:ff,nf?ff:x));}
  case CSCO:     
   ASSERT(nf!=0,EVDOMAIN); 
   RE(n=i0(x)); ASSERT(n&&BETWEENC(n,-6,6),EVDOMAIN);
   return jtamp(jt,sc(-n),h);
  case CUCO:
   ASSERT(nf!=0,EVDOMAIN); 
   RE(n=i0(x)); ASSERT(BETWEENC(n,1,8)&!BETWEENC(n,5,6),EVDOMAIN);
   return jtamp(jt,sc(-(1^(-n))),h);
  case CCANT:    
   ASSERT(nf!=0,EVDOMAIN); 
   return jtobverse(jt,jteva(jt,x,"] |:~ u C.^:_1 i.@#@$"),w);
  case CPCO:
   if(nf){
    RE(n=i0(x));
    switch(n){
     case -4: case 4: return jtamp(jt,negate(x),h);
     case -1:         return ds(CPCO);
     case  2:         return jtobverse(jt,eval("*/@(^/)\"2"),w);
     case  3:         return eval("*/");
   }}
   break;
  case CQCO:     
   if(nf){
    ASSERT(!AR(x),EVRANK);
    return jtobverse(jt,eval(all1(lt(x,zeroionei(0)))?"*/@(^/)\"2":"(p:@i.@# */ .^ ])\"1"),w);
   }
   break;
  case CFIT:
   ASSERT(nf&&(CPOUND==ID(FAV(g)->fgh[0])),EVDOMAIN);
   ASSERT(1==AR(x),EVRANK);
   return fdef(0,CPOWOP,VERB, jtexpandg,0L, w,num(-1),0L, VFLAGNONE, RMAX,0L,0L);
  case CPOUND:
   ASSERT(nf!=0,EVDOMAIN);
   ASSERT(1==AR(x),EVRANK);
   return fdef(0,CPOWOP,VERB, jtexpandf,0L, w,num(-1),0L, VFLAGNONE, RMAX,0L,0L);
   break;
  case CPOWOP:
   if(VGERL&u->flag){ff=*(1+AAV(u->fgh[2])); return jtamp(jt,nf?x:ff,nf?ff:x);}
   break;
  case CCOMMA:  
   SETIC(x,n); 
   return jtobverse(jt,1==n?ds(nf?CDROP:CCTAIL):jtamp(jt,sc(nf?n:-n),ds(CDROP)),w);
  case CBASE:   
   if(!nf)break;
   return AR(x) ? jtamp(jt,x,ds(CABASE)) :
    jtobverse(jt,evc(x,mag(x),"$&u@>:@(v&(<.@^.))@(1&>.)@(>./)@:|@, #: ]"),w);
  case CATOMIC:
   if(ng){ASSERT(equ(x,nub(x)),EVDOMAIN); return jtobverse(jt,jtatop(jt,f,jtamp(jt,x,ds(CIOTA))),w);}  // fall through to common obverse (?)
  case CCYCLE:
   if(nf&&AR(x)<=(c==CCYCLE))return jtobverse(jt,jteva(jt,w,"/:@u@(i.@#) { ]"),w); break;
  case CDROP:
   if(!(nf&&1>=AR(x)))break;
   RZ(x=jtcvt(jt,INT,x));
   RZ(y=jteps(jt,jtv2(jt,-1L,1L),signum(x))); yv=CAV(y);
   f=jtamp(jt,mag(x),ds(CPLUS));
   g=1==AN(x)?ds(CPOUND):jtatop(jt,jtamp(jt,tally(jt, x),ds(CTAKE)),ds(CDOLLAR));
   h=!yv[1]?f:jtatop(jt,!yv[0]?ds(CMINUS):jtamp(jt,negate(signum(x)),ds(CSTAR)),f);
   return jtobverse(jt,jthook(jt,swap(ds(CTAKE)),jtatop(jt,h,g)),w);
  case CDOMINO:
   if(!(2==AR(x)&&*AS(x)==*(1+AS(x))))break;
   ff=eval("+/ .*");
   return nf?jtatop(jt,h,jtamp(jt,ff,minv(x))):jtamp(jt,x,ff);
  case CDOT:
   if(ip(h,CPLUS,CSTAR)){
    ASSERT(2==AR(x),EVRANK);
    ASSERT(AS(x)[0]==AS(x)[1],EVLENGTH);
    return nf?jtamp(jt,ds(CDOMINO),x):jtamp(jt,h,minv(x));
   }
   break;
  case CQQ:
   if(ng&&equ(x,num(1))&&equ(f,eval("i.\"1")))return jthook(jt,ds(CFROM),ds(CEQ));
   break;
  case CBSLASH:
   if(nf&&(n=i0(x),0>n)&&(d=ID(u->fgh[0]),(d&-2)==CLEFT))return slash(ds(CCOMMA));  // LEFT || RIGHT
   break;
  case CIBEAM:
   x=FAV(h)->fgh[0]; y=FAV(h)->fgh[1];
   if(NOUN&AT(x)&&equ(x,num(3))&&NOUN&AT(y)){
    RE(n=i0(f));
    if(all1(jteps(jt,y,jtv2(jt,4L,5L)))){ASSERT(n&&BETWEENC(n,-2,2),EVDOMAIN); return jtamp(jt,sc(-n),g);}
    if(all1(jteps(jt,y,jtv2(jt,1L,3L)))){ASSERT(0==n||1==n||10==n||11==n,EVDOMAIN); return jtforeign(jt,x,num(2));}
   }
   break;
  case CBDOT:
   RE(n=i0(x));
   switch(i0(FAV(h)->fgh[1])){
    case 22: case 25:          return w;
    case 19: case 28:          if(ng)return w; break;
    case 21: case 26:          if(nf)return w; break;
    case 32: case 33: case 34: ASSERT(nf!=0,EVDOMAIN); return jtamp(jt,negate(x),h);
   }
   break;
  case CPOLY:
   if(nf&&1==AR(x)&&2==AN(x)&&NUMERIC&AT(x)&&!equ(zeroionei(0),tail(x))){  // linear polynomial only
    RZ(y=recip(tail(x)));
    return jtamp(jt,apip(tymes(y,negate(head(x))),y),h);
 }}
 ASSERT(0,EVDOMAIN);
}

// simpleinv[i] is the inverse function for function i
static const C simpleinv[128] = {
[CDIV&127]=CDIV, [CPLUS&127]=CPLUS, [CMINUS&127]=CMINUS, [CLEFT&127]=CLEFT, [CRIGHT&127] =CRIGHT , [CREV&127] =CREV , [CCANT&127]=CCANT, [CPOLY&127]=CPOLY,
[CNOT&127]=CNOT, [CGRADE&127]=CGRADE, [CCYCLE&127] =CCYCLE , [CDOMINO&127]=CDOMINO, [COPE&127] =CBOX , [CBOX&127]=COPE, [CLOG&127] =CEXP , [CEXP&127]=CLOG,
[CGE&127]=CLE, [CLE&127]=CGE, [CHALVE&127]=CPLUSCO, [CPLUSCO&127]=CHALVE, [CSQRT&127]=CSTARCO, [CSTARCO&127]=CSQRT, [CHEAD&127]=CLAMIN, [CLAMIN&127] =CHEAD ,
[CABASE&127]=CBASE, [CBASE&127]=CABASE, [CTHORN&127]=CEXEC, [CEXEC&127]=CTHORN,
};

// Return inverse of monad w.  recur is a recursion indicator, always forced to 0 for the initial call, and
// set to 1 here for recursive calls
A jtinv(J jt, A w, I recur){A f,ff,g;B b,nf,ng,vf,vg;C id;I p,q;V*v;
 if(!w) return 0;
 STACKCHKOFL  // make sure we don't have a recursion loop through inv
 ASSERT(VERB&AT(w),EVDOMAIN); 
 id=ID(w); v=FAV(w);  // id=pseudochar for w, v->verb info
#define simpleinvvalues(w) CCM(w,CDIV)+CCM(w,CPLUS)+CCM(w,CMINUS)+CCM(w,CLEFT)+CCM(w,CRIGHT)+CCM(w,CREV)+CCM(w,CCANT)+CCM(w,CPOLY)+ \
 CCM(w,CNOT)+CCM(w,CGRADE)+CCM(w,CCYCLE)+CCM(w,CDOMINO)+CCM(w,COPE)+CCM(w,CLOG)+CCM(w,CBOX)+CCM(w,CEXP)+ \
 CCM(w,CGE)+CCM(w,CLE)+CCM(w,CHALVE)+CCM(w,CPLUSCO)+CCM(w,CSQRT)+CCM(w,CSTARCO)+CCM(w,CHEAD)+CCM(w,CLAMIN)+ \
 CCM(w,CABASE)+CCM(w,CBASE)+CCM(w,CTHORN)+CCM(w,CEXEC)
 CCMWDS(simpleinv) CCMCAND(simpleinv,cand,id) if(CCMTST(cand,id))return ds(simpleinv[id&127]);
 // in case id indicates a modifier, set (f|g) to the operand, n? if it is a noun or name, v? if it is a verb
 f=v->fgh[0]; nf=f&&AT(f)&NOUN+NAME; vf=f&&!nf;
 g=v->fgh[1]; ng=g&&AT(g)&NOUN+NAME; vg=g&&!ng;
 switch(id){
  case CCIRCLE:  return eval("1p_1&*");
  case CJDOT:    return eval("0j_1&*");
  case CRDOT:    return eval("%&0j1@^.");
  case CPLUSDOT: return eval("j./\"1\"_ :. +.");
  case CSTARDOT: return eval("r./\"1\"_ :. *.");
  case CDGRADE:  return eval("/:@|.");
  case CWORDS:   return eval("}:@;@(,&' '&.>\"1) :. ;:");
  case CBANG:    return eval("3 : '(-(y -~ !)%0.001&* (0.001%~[:-/[:! 0.001 0 +/ ]) ])^:_<.&170^:(-:+)^.y' :. !");
  case CXCO:     return jtamp(jt,num(-1),w);
  case CSPARSE:  return fdef(0,CPOWOP,VERB,jtdenseit,0L, w,num(-1),0L, VFLAGNONE, RMAX,RMAX,RMAX);
  case CPCO:     return fdef(0,CPOWOP,VERB,jtplt,    0L, w,num(-1),0L, 0L, 0L,  0L,  0L  );
  case CQCO:     return eval("*/");
  case CUCO:     return jtamp(jt,num(3),w);
  case CUNDER:   return jtunder(jt,invrecur(f),g);
  case CFORK:    return invfork(w);
  case CAMP:     if(nf!=ng){A z=invamp(w); if(nf^ng)return z;}  // may fall through... but avoid tail-recursion so we get out of loop
  case CAT:      if(vf&&vg)return jtatop(jt,invrecur(g),invrecur(f));   break;
  case CAMPCO:
  case CATCO:    if(vf&&vg)return jtatco(jt,invrecur(g),invrecur(f));   break;
  case CSLASH:   if(CSTAR==ID(f))return ds(CQCO);        break;
  case CQQ:      if(vf)return qq(invrecur(f),g);              break;
  case COBVERSE: if(vf&&vg)return jtobverse(jt,g,f);          break;  // if defined obverse, return it
  case CSCO:     return jtamp(jt,num(5),w);
  case CPOWOP:   
   if(vf&&ng){RE(p=i0(g)); return -1==p?f:1==p?invrecur(f):powop(0>p?f:invrecur(f),sc(ABS(p)),0);}
   if(VGERL&v->flag)return*(1+AAV(v->fgh[2]));
   break;
  case CTILDE:
   if(nf)return invrecur(symbrd(f));  // name~ - resolve name & try again
   switch(ID(f)){   // inverses for reflexive monads
    case CPLUS:  return ds(CHALVE);
    case CSTAR:  return ds(CSQRT);
    case CJDOT:  return eval("0.5j_0.5&*");
    case CLAMIN: return eval("{. :. (,:~)");
    case CSEMICO:return eval(">@{. :. (;~)");
    case CCOMMA: return eval("<.@-:@# {. ] :. (,~)");
    case CEXP:   return eval("3 : '(- -&b@(*^.) % >:@^.)^:_ ]1>.b=.^.y' \" 0 :. (^~)");
   }
   break;
  case CBSLASH:
  case CBSDOT:
   if(CSLASH==ID(f)&&(ff=FAV(f)->fgh[0],ff&&VERB&AT(ff))){  //  ff/\  or ff/\.
    b=id==CBSDOT;
    switch(ID(ff)){
     case CPLUS: return jtobverse(jt,eval(b?"- 1&(|.!.0)":" - |.!.0"),w);
     case CSTAR: return jtobverse(jt,eval(b?"% 1&(|.!.1)":" % |.!.1"),w);
     case CEQ:   return jtobverse(jt,eval(b?"= 1&(|.!.1)":" = |.!.1"),w);
     case CNE:   return jtobverse(jt,eval(b?"~:1&(|.!.0)":" ~:|.!.0"),w);
     case CMINUS:return jtobverse(jt,eval(b?"+ 1&(|.!.0)":"(- |.!.0) *\"_1 $&1 _1@#"),w);
     case CDIV:  return jtobverse(jt,eval(b?"* 1&(|.!.1)":"(% |.!.1) ^\"_1 $&1 _1@#"),w);
   }}
   break;
  case CCUT:
   if(CBOX==ID(f)&&ng&&(p=i0(g),1==p||2==p))return fdef(0,CPOWOP,VERB, jtbminv,0L, w,num(-1), 0L,VFLAGNONE, RMAX,RMAX,RMAX);
   break;
  case CIBEAM:
   p=i0(f); q=i0(g);
   if(3==p&&1==q)return jtforeign(jt,f,num(2));
   if(3==p&&2==q)return jtforeign(jt,f,num(1)   );
   if(3==p&&3==q)return jtforeign(jt,f,num(2));
   break;
  case CHOOK:
    if(BOTHEQ8(ID(f),ID(g),CFROM,CEQ))return eval("i.\"1&1");
   break;
 }
 // Failure - no inverse found.  If there are names in w, try fixing w and try on that.
 // But only fix once, at the top recursion level, (1) to avoid an infinite loop if
 // there is a circular reference that leaves names in the fixed form of w; (2) to avoid
 // repeated fixing of lower branches, which will only be tried again when higher levels are fixed
 if(!recur&&!nameless(w))return invrecur(jtfix(jt,w,zeroionei(0)));
 ASSERT(0,EVDOMAIN);
}

static A jtneutral(J jt, A w){A x,y;B b;V*v;
 v=FAV(w);
 ASSERT(!v->lrr,EVDOMAIN);
 RZ(y=jtv2(jt,0L,1L));
 RZ(x=scf(infm)); b=equ(y,CALL2(v->valencefns[1],x,y,w)); RESETERR; if(b)return x;
 x=ainf;          b=equ(y,CALL2(v->valencefns[1],x,y,w)); RESETERR; if(b)return x;
 x=zeroionei(0);          b=equ(y,CALL2(v->valencefns[1],x,y,w)); RESETERR; if(b)return num(0);
 x=zeroionei(1);           b=equ(y,CALL2(v->valencefns[1],x,y,w)); RESETERR; if(b)return num(1);
 RZ(x=scf(infm)); b=equ(y,CALL2(v->valencefns[1],y,x,w)); RESETERR; if(b)return x;
 x=ainf;          b=equ(y,CALL2(v->valencefns[1],y,x,w)); RESETERR; if(b)return x;
 x=zeroionei(0);          b=equ(y,CALL2(v->valencefns[1],y,x,w)); RESETERR; if(b)return num(0);
 x=zeroionei(1);           b=equ(y,CALL2(v->valencefns[1],y,x,w)); RESETERR; if(b)return num(1);
 ASSERT(0,EVDOMAIN);
}    /* neutral of arbitrary rank-0 function */

 A jtiden(J jt, A w){A f,g,x=0;V*u,*v;
 RZ(w=jtfix(jt,w,zeroionei(0))); ASSERT(VERB&AT(w),EVDOMAIN);
 v=FAV(w); f=v->fgh[0]; g=v->fgh[1];
 switch(v->id){
  default:      RZ(x=neutral(w)); break;
  case CCOMMA:  return eval("i.@(0&,)@(2&}.)@$");
  case CDOT:    if(!(ip(w,CPLUS,CSTAR)||ip(w,CPLUSDOT,CSTARDOT)||ip(w,CNE,CSTARDOT)))break;  // if matrix multiply, fall through to...
  case CDOMINO: return jtatop(jt,atop(ds(CEQ),ds(CGRADE)),ds(CHEAD));
  case CCYCLE:
  case CLBRACE: return jtatop(jt,ds(CGRADE),ds(CHEAD));
  case CSLASH:  if(VERB&AT(f))return jtatop(jt,iden(f),ds(CPOUND)); break;
  case CPLUS: case CMINUS: case CSTILE:   case CNE:
  case CGT:   case CLT:    case CPLUSDOT: case CJDOT:   case CRDOT:
                x=num(0); break;
  case CSTAR: case CDIV:   case CEXP:     case CROOT:   case CBANG:
  case CEQ:   case CGE:    case CLE:      case CSTARDOT:
                x=num(1); break;
  case CMAX:    x=scf(infm); break;
  case CMIN:    x=ainf; break;
  case CUNDER:  ; A t; df1(x,df1(t,mtv,iden(f)),inv(g)); break;
  case CAT:
   if(CAMP==ID(f)&&(u=FAV(f),NOUN&AT(u->fgh[0])&&!AR(u->fgh[0])&&CSTILE==ID(u->fgh[1])))switch(ID(g)){
    case CSTAR: case CEXP: x=num(1);  break;
    case CPLUS:            x=num(0);
   }
   break;
  case CBDOT: ;  // canned inverses for (bt b.)
   I bt=i0(g);
#define INVM1 BMK(25)
#define INV0 (BMK(2)+BMK(4)+BMK(5)+BMK(6)+BMK(7)+BMK(18)+BMK(20)+BMK(21)+BMK(22)+BMK(23))
#define INV1 (BMK(1)+BMK(9)+BMK(11)+BMK(13)+BMK(17)+BMK(27)+BMK(29))
   if(bt<32&&((INVM1|INV0|INV1)&(1LL<<bt))){I bi;
    bi=-1; bi=INV0&(1LL<<bt)?0:bi; bi=INV1&(1LL<<bt)?1:bi;
    x=num(bi);
   }
   break;
  }
 ASSERT(x!=0,EVDOMAIN);
 return folk(x,swap(ds(CDOLLAR)),jtatop(jt,ds(CBEHEAD),ds(CDOLLAR)));
}

 A jtidensb(J jt, A w){A f,g,x=0,w0=w;V*v;
 RZ(w=jtfix(jt,w,zeroionei(0))); ASSERT(VERB&AT(w),EVDOMAIN);
 v=FAV(w); f=v->fgh[0]; g=v->fgh[1];
 switch(v->id){
  default:      return iden(w0);
  case CMAX:    GATV0(x,SBT,1,0);SBAV(x)[0]=0; break;
  case CMIN:    GATV0(x,SBT,1,0);SBAV(x)[0]=jt->sbuv[0].down; break;
 }
 ASSERT(x!=0,EVDOMAIN);
 return folk(x,swap(ds(CDOLLAR)),jtatop(jt,ds(CBEHEAD),ds(CDOLLAR)));
}
