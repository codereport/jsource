/* Copyright 1990-2006, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Adverbs: Utilities                                                      */

#include "j.h"


#if !USECSTACK
I jtfdep(J jt,A w){A f,g;I d=0,k;V*v;
 v=VAV(w);
 if(v->fdep)return v->fdep;  // for speed, use previous value if it has been calculated
 if(f=v->fgh[0]) d=VERB&AT(f)?fdep(f):NOUN&AT(f)&&VGERL&v->flag?fdepger(f):0;
 if(g=v->fgh[1]){k=VERB&AT(g)?fdep(g):NOUN&AT(g)&&VGERR&v->flag?fdepger(g):0; d=MAX(d,k);}
 if(CFORK==v->id){k=fdep(v->fgh[2]); d=MAX(d,k);}
 if(!jt->jerr)v->fdep=(UI4)(1+d);  //Save computed value for next time, but not if error; that would lose the error next time
 return 1+d;
}    /* function depth:  1 + max depth of components */

 A jtfdepadv(J jt, A w){ ASSERT(VERB&AT(w),EVDOMAIN); return sc(fdep(w));}
#endif


 A jtdfs1(J jt,    A w,A self){F1PREFIP;A s=jt->sf,z; RZ(self); z=CALL1IP(FAV(self)->valencefns[0],  w,jt->sf=self); jt->sf=s; return z;}
 A jtdfs2(J jt,A a,A w,A self){F2PREFIP;
A s=jt->sf,z; 
RZ(self); 
z=CALL2IP(FAV(self)->valencefns[1],a,w,jt->sf=self); jt->sf=s; 
return z;}
     /* for monads and dyads that can possibly involve $: */


// $: itself
 A jtself1(J jt, A w){A z; FDEPINC(d=fdep(jt->sf)); STACKCHKOFL df1(z,  w,jt->sf);  FDEPDEC(d); forcetomemory(w); return z;}
 A jtself2(J jt,A a,A w){A z; FDEPINC(d=fdep(jt->sf)); STACKCHKOFL df2(z,a,w,jt->sf);  FDEPDEC(d); forcetomemory(w); return z;}

A jtac1(J jt,AF f){return fdef(0,0,VERB, f,0L, 0L,0L,0L, VFLAGNONE, RMAX,RMAX,RMAX);}
A jtac2(J jt,AF f){return fdef(0,0,VERB, 0L,f, 0L,0L,0L, VFLAGNONE, RMAX,RMAX,RMAX);}

 A jtdomainerr1(J jt, A w){ASSERT(0,EVDOMAIN);}
 A jtdomainerr2(J jt,A a,A w){ASSERT(0,EVDOMAIN);}

// create a block for a function (verb/adv/conj).  The meanings of all fields depend on the function executed in f1/f2
// if there has been a previous error this function returns 0
A jtfdef(J jt,I flag2,C id,I t,AF f1,AF f2,A fs,A gs,A hs,I flag,I m,I l,I r){A z;V*v;
 RE(0);
 GAT0(z,INT,(VERBSIZE+SZI-1)>>LGSZI,0); v=FAV(z);  // allocate as fixed size, and as INT to avoid clearing the area, which will be all filled in
 AT(z)=t;  // install actual type
 if(fs)INCORP(fs); if(gs)INCORP(gs); if(hs)INCORP(hs);   // indicate fgh are about to be incorporated
 v->localuse.lclr[0]=v->localuse.lclr[1]=0;  // clear the private field
 v->valencefns[0]    =f1?f1:jtdomainerr1;  /* monad C function */
 v->valencefns[1]    =f2?f2:jtdomainerr2;  /* dyad  C function */
 v->fgh[0]     =fs;                  /* monad            */
 v->fgh[1]     =gs;                  /* dyad             */      
 v->fgh[2]     =hs;                  /* fork right tine or other auxiliary stuff */
 v->flag  =(UI4)flag;
#if !USECSTACK
 v->fdep  =0;                   /* function depth   */
#endif
 v->flag2 = (UI4)flag2;         // more flags
 v->mr    =(RANKT)m;                   /* monadic rank     */
 v->lrr=(RANK2T)((l<<RANKTX)+r);
 v->id    =(C)id;                  /* spelling         */
 return z;
}

B nameless(A w){A f,g,h;C id;V*v;
 if(!w||NOUN&AT(w))return 1;
 v=FAV(w);
 id=v->id; f=v->fgh[0]; g=v->fgh[1]; h=v->fgh[2];
 return !(id==CTILDE&&f&&NAME&AT(f)) && nameless(f) && nameless(g) && (id==CFORK?nameless(h):1);
}

B jtprimitive(J jt,A w){A x=w;V*v;
 v=VAV(w);
 if(CTILDE==v->id&&NOUN&AT(v->fgh[0]))RZ(x=jtfix(jt,w,zeroionei(0)));
 return!VAV(x)->fgh[0];
}    /* 1 iff w is a primitive */


// w is a conj, f C n
// Return 1 if f is of the form <@:g  (or <@g when g has infinite rank)
B jtboxatop(J jt,A w){ return 1&boxat(FAV(w)->fgh[0],RMAX,RMAX,RMAX);}

// x is a verb
// Return if verb is of the form <@:g  (or <@g when g has rank >= input rank)
// bit 0 for monad case, bit 1 for dyad
I boxat(A x, I m, I l, I r){C c;V*v;
 if(!x)return 0;
 v=FAV(x); c=v->id;   // x->fgh[0], v->value, c=id of f
 if(!COMPOSE(c))return 0;  // Return if not @ @: & &:
 if(CBOX==ID(v->fgh[0])) {  // if u is <...
   if(c==CATCO) return 3; if(c==CAMPCO) return 1;  // @: is always good, and &: for monad
   I res = 0;
   if(v->fgh[1]&&VERB&AT(v->fgh[1])){
    res |= FAV(v->fgh[1])->mr>=m;    // monad ok if rank big enough  for either @ or &
    res |= (c==CAT)&&lr(v->fgh[1])>=(UI)l&&rr(v->fgh[1])>=(UI)r?2:0;  // dyad for @ only, if rank big enough
   }
   return res;
 }
 return 0;
}    /* 1 iff w is <@:f or <@f where f has infinite rank */

// w is a verb
// Result has bit 0 set if the verb is [ or ...@[, bit 1 set if ] or ...@]   (or @:)
// The set bit indicates that argument WILL NOT be examined when w is executed
I atoplr(A w){
 if(!w)return 0;
 V *v=FAV(w);     // v->verb info, c=id of w
 C id = v->id;if((v->id&-2)==CATCO)id = FAV(v->fgh[1])->id;  // @ @:
 return (id-(CLEFT-1)) & REPSGN((CLEFT-1)-(I)id) & REPSGN((I)id-(CRIGHT+1));  // LEFT->1, RIGHT->2 punning with JINPLACEW/A; but 0 if not LEFT or RIGHT
}
