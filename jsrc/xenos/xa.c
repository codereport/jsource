/* Copyright 1990-2006, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Xenos: Miscellaneous                                                    */

#include "j.h"
#include "x.h"

#include <string.h>


F1(jtassertq){ASSERTMTV(w); return scb(jt->assert);}

F1(jtasserts){B b; RE(b=b0(w)); jt->assert=b; return mtm;}

F1(jtdirectdefq){ASSERTMTV(w); return scb(jt->directdef);}  // scaf 9!:62

F1(jtdirectdefs){B b; RE(b=b0(w)); jt->directdef=b; return mtm;}  // scaf 9!:63

F1(jtboxq){ASSERTMTV(w); return str(sizeof(jt->bx),jt->bx);}

F1(jtboxs){A x;
 RZ(w=vs(w));
 ASSERT(sizeof(jt->bx)==AS(w)[0],EVLENGTH);
 MC(jt->bx,CAV(w),sizeof(jt->bx));
 return mtv;
}  // box-display characters

F1(jtctq){ASSERTMTV(w); return scf(1.0-jt->cct);}

F1(jtcts){D d;
 ASSERT(!AR(w),EVRANK);
 RZ(w=cvt(FL,w)); d=DAV(w)[0];
 ASSERT(0<=d,EVDOMAIN); 
 ASSERT(d<=5.820766091e-11,EVDOMAIN);
 jt->cctdefault=jt->cct=1.0-d;
 return mtv;
}

F1(jtdispq){A z; ASSERTMTV(w); GATV0(z,INT,*jt->disp,1); ICPY(AV(z),1+jt->disp,*jt->disp); return z;}

F1(jtdisps){UC n;
 RZ(w=vi(w));
 n=(UC)AN(w);
 ASSERT(1>=AR(w),EVRANK);
 ASSERT(all1(nubsieve(w)),EVDOMAIN);
 ASSERT(all1(eps(w,eval("1 2 4 5 6"))),EVINDEX);
 *jt->disp=n; DO(n, jt->disp[1+i]=(UC)IAV(w)[i];);
 return mtv;
}

F1(jtevmq){ASSERTMTV(w); return behead(jt->evm);}

F1(jtevms){A t,*tv,*wv;
 ARGCHK1(w);
 ASSERT(1==AR(w),EVRANK);
 ASSERT(NEVM==AN(w),EVLENGTH);
 ASSERT(BOX&AT(w),EVDOMAIN);
 GAT0(t,BOX,1+NEVM,1); tv=AAV(t); 
 *tv++=mtv;
 wv=AAV(w);
 DQ(NEVM, RZ(*tv=ca(vs(*wv))); CAV(*tv)[AN(*tv)]=0; ++tv; ++wv;);  // NUL-terminate.  ca to make sure there's room
 ras(t); fa(jt->evm); jt->evm=t;
 return mtv;
}

F1(jtfxx){
 ARGCHK1(w);
 ASSERT(AT(w)&LIT+BOX,EVDOMAIN);
 ASSERT(1>=AR(w),EVRANK);
 return fx(ope(w));
}

F1(jtiepdoq){ASSERTMTV(w); return scb(jt->iepdo);}

F1(jtiepdos){B b; RE(b=b0(w)); jt->iepdo=b; return mtm;}

F1(jtiepq){
 ASSERTMTV(w); 
 ASSERT(1==AR(w),EVRANK);
 ASSERT(!AN(w),EVDOMAIN); 
 return jt->iep?jt->iep:mtv;
}

F1(jtieps){
 ARGCHK1(w);
 ASSERT(1>=AR(w),EVRANK);
 ASSERT(!AN(w)||AT(w)&LIT,EVDOMAIN);
 fa(jt->iep);
 RZ(ras(w)); RZ(jt->iep=w); 
 return mtm;
}

F1(jtoutparmq){A z;D*u;I*v;
 ASSERTMTV(w);
 if(IMAX==jt->outmaxlen||IMAX==jt->outmaxbefore||IMAX==jt->outmaxafter){
  GAT0(z,FL, 4,1); u=DAV(z);
  u[0]=(D)jt->outeol;
  u[1]=jt->outmaxlen==IMAX?inf:(D)jt->outmaxlen;
  u[2]=jt->outmaxbefore==IMAX?inf:(D)jt->outmaxbefore;
  u[3]=jt->outmaxafter==IMAX?inf:(D)jt->outmaxafter;
 }else{
  GAT0(z,INT,4,1); v= AV(z);
  v[0]=jt->outeol;
  v[1]=jt->outmaxlen;
  v[2]=jt->outmaxbefore;
  v[3]=jt->outmaxafter;
 }
 return z;
}

F1(jtoutparms){I*v;
 RZ(w=vib(w));
 ASSERT(1==AR(w),EVRANK);
 ASSERT(4==AN(w),EVLENGTH);
 v=AV(w);
 ASSERT(0==v[0]||2==v[0],EVINDEX);
 ASSERT(0<=v[1],EVDOMAIN);
 ASSERT(0<=v[2],EVDOMAIN);
 ASSERT(0<=v[3],EVDOMAIN);
 jt->outeol      =(UC)v[0];
 jt->outmaxlen   =v[1];
 jt->outmaxbefore=v[2];
 jt->outmaxafter =v[3];
 return mtv;
}

F1(jtposq){ASSERTMTV(w); return v2(jt->pos[0],jt->pos[1]);}

F1(jtposs){I n,p,q,*v;
 RZ(w=vi(w));
 n=AN(w); v=AV(w);
 ASSERT(1>=AR(w),EVRANK);
 ASSERT(1==n||2==n,EVLENGTH);
 if(1==n)p=q=*v; else{p=v[0]; q=v[1];} 
 ASSERT(BETWEENC(p,0,2)&&BETWEENC(q,0,2),EVDOMAIN);
 jt->pos[0]=p; jt->pos[1]=q;   
 return mtv;
}

F1(jtppq){C*end;I k;
 ASSERTMTV(w);
 k = strtoI(3+jt->pp, (char**)&end, 10);
 return sc(k);
}

F1(jtpps){I k;
 RE(sc(k=i0(w))); ASSERT(0<k,EVDOMAIN); ASSERT(k<=NPP,EVLIMIT);
 sprintf(3+jt->pp,FMTI"g", k);
 return mtv;
}

F1(jtretcommq){ASSERTMTV(w); return scb(jt->retcomm);}

F1(jtretcomms){B b; RE(b=b0(w)); jt->retcomm=b; return mtm;}

F1(jtseclevq){ASSERTMTV(w); return sc(jt->seclev);}

F1(jtseclevs){I k; 
 RE(k=i0(w)); 
 ASSERT(0==k||1==k,EVDOMAIN); 
 if(!jt->seclev&&1==k)jt->seclev=(UC)k;
 return mtm;
}

F1(jtsysq){I j;
 ASSERTMTV(w);
 j=5;
 return sc(j);
}

F1(jtxepq){
 ASSERTMTV(w); 
 return jt->xep?jt->xep:mtv;
}

F1(jtxeps){
 ARGCHK1(w);
 ASSERT(1>=AR(w),EVRANK);
 ASSERT(!AN(w)||AT(w)&LIT,EVDOMAIN);
 fa(jt->xep);
 RZ(ras(w)); RZ(jt->xep=w); 
 return mtm;
}

F1(jtasgzombq){ASSERTMTV(w); return sc(jt->asgzomblevel);}

F1(jtasgzombs){I k; 
 RE(k=i0(w)); 
 ASSERT(BETWEENC(k,0,2),EVDOMAIN);
 jt->asgzomblevel=(C)k;
 return mtm;
}

// enable/disable tstack auditing, since some testcases run too long with it enabled
// bit 0 is set to disable, bit 1 is a one-shot to ask for an audit
// result is old value
F1(jtaudittdisab){
 return sc(0);
}
