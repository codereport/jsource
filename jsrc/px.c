/* Copyright 1990-2009, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Execute and Associates                                                  */

/* Variables for controlling execution                                     */
/*                                                                         */
/* B     asgn      1 iff last operation on this line is assignment         */
/* DC    dcs       ptr to debug stack entry for current script             */
/* I     etxn      strlen(etx)                                             */
/* C     etx[]     space for holding error display                         */
/* I     jbrk      1 iff user has entered "break"                          */
/* C     jerr      error number for this line                              */
/* A     slist     script files used in right argument to 0!:              */
/* I     slisti    index in slist for current script                       */
/* I     slistn    slist # real entries; AN(slist) is reserved entries     */
/* B     tostdout  1 iff output to standard output                         */

#include "j.h"
#include "parsing/p.h"


A jteval(J jt,C*s){return parse(jttokens(jt,cstr(s),1+(AN(jt->locsyms)>1)));}

A jtev1(J jt,    A w,C*s){A z; return df1(z,  w,eval(s));}  // parse *s and apply to w
A jtev2(J jt,A a,A w,C*s){A z; return df2(z,a,w,eval(s));}  // parse *s and apply to a and w
A jteva(J jt,    A w,C*s){A z; return df1(z,  w,jtcolon(jt,num(1),   cstr(s)));}
A jtevc(J jt,A a,A w,C*s){A z; return df2(z,a,w,jtcolon(jt,num(2),cstr(s)));}

// ". y
 A jtexec1(J jt, A w){A z;
 if(AT(w)&NAME){z=jtnameref(jt,w,jt->locsyms);  // the case ".@'name' which is the fastest way to refer to a deferred name
 }else{
  F1RANK(1,jtexec1,UNUSED_VALUE);
  A savself = jt->sf;  // in case we are in a recursion, preserve the restart point
  STACKCHKOFL FDEPINC(1); z=parse(jtddtokens(jt,vs(w),4+1+(AN(jt->locsyms)>1))); jt->asgn=0; FDEPDEC(1);  // replace DDs, but require that they be complete within the string (no jgets)
  jt->sf=savself;
 }
 return z&&!(AT(z)&NOUN)?mtv:z;  // if non-noun result, return empty $0
}

// execute w, which is either a string or the boxed words of a string (as if from jttokens(jt,))
 A jtimmex(J jt, A w){A z;
 if(!w)return A0;  // if no string, return error
 // When we start a sentence, we need to establish AKGST in locsyms as a shadow of jt->global, because that's
 // the normal condition and u./v. will rely on it.  This is not needed for a recursive call, but it doesn't hurt either,
 // because if AKGST has been set it will already hold jt->global.  Of course, any event code must restore everything
 // to its previous state, including locales
 AKGST(jt->locsyms)=jt->global; // in case the sentence has operators, set a locale for it
 STACKCHKOFL FDEPINC(1); z=parse(AT(w)&BOX?w:jttokens(jt,w,1+(AN(jt->locsyms)>1))); FDEPDEC(1);
 if(z&&!jt->asgn)jpr(z);
 return z;
}

// execute for assert: check result for all 1
 A jtimmea(J jt, A w){A t,z,z1;
 RZ(w=jtddtokens(jt,w,4+1+(AN(jt->locsyms)>1))); z=immex(w);   // check for DD, but don't allow continuation read
 ASSERT(jt->asgn||!z||!(AT(z)&NOUN)||(t=eq(num(1),z),
     all1(AT(z)&SPARSE?df1(z1,t,jtatop(jt,slash(ds(CSTARDOT)),ds(CCOMMA))):t)),EVASSERT);  // apply *./@, if sparse
 return z;
}

static A jtcex(J jt,A w,AF f,A self){A z; RE(w); z=f(jt,w,self); RESETERR; return z;}
     /* conditional execute - return 0 if error */

// convert the gerund (i.e  AR) in w into a verb
 A jtexg(J jt, A w){A*v,*wv,x,y,z;I n;
 n=AN(w); wv=AAV(w); 
 ASSERT(n!=0,EVLENGTH);
 ASSERT(1>=AR(w),EVRANK);
 if(VERB&AT(w))return w;
 ASSERT(BOX&AT(w),EVDOMAIN);
 GATV0(z,BOX,n,1); v=AAV(z);
 DO(n, x=wv[i]; RZ(*v++=(y=cex(x,jtfx,0L))?y:exg(x)););  // if the AR can be converted to an A, do so; otherwise it should be a list of ARs, recur on each
 return parse(z);
}

L* jtjset(J jt,C*name,A x){return symbisdel(jtnfs(jt,(I)strlen(name),name),x,jt->global);}

 A jtapplystr(J jt,A a,A w){PROLOG(0054);A fs,z;
 F2RANK(1,RMAX,jtapplystr,UNUSED_VALUE);
 RZ(fs=parse(jttokens(jt,vs(a),1+(AN(jt->locsyms)>1))));
 ASSERT(VERB&AT(fs),EVSYNTAX);
 STACKCHKOFL FDEPINC(d=fdep(fs)); z=CALL1(FAV(fs)->valencefns[0],w,fs); FDEPDEC(d);
 EPILOG(z); 
}    /* execute string a on argument w */
