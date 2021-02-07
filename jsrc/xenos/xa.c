/* Copyright 1990-2006, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Xenos: Miscellaneous                                                    */

#include "j.h"
#include "x.h"

#include <string.h>


 A jtassertq(J jt, A w){ASSERTMTV(w); return scb(jt->assert);}

 A jtasserts(J jt, A w){B b; RE(b=b0(w)); jt->assert=b; return mtm;}

 A jtdirectdefq(J jt, A w){ASSERTMTV(w); return scb(jt->directdef);}  // scaf 9!:62

 A jtdirectdefs(J jt, A w){B b; RE(b=b0(w)); jt->directdef=b; return mtm;}  // scaf 9!:63

 A jtboxq(J jt, A w){ASSERTMTV(w); return str(sizeof(jt->bx),jt->bx);}

 A jtboxs(J jt, A w){A x;
 RZ(w=vs(w));
 ASSERT(sizeof(jt->bx)==AS(w)[0],EVLENGTH);
 memcpy(jt->bx,CAV(w),sizeof(jt->bx));
 return mtv;
}  // box-display characters

 A jtctq(J jt, A w){ASSERTMTV(w); return scf(1.0-jt->cct);}

 A jtcts(J jt, A w){D d;
 ASSERT(!AR(w),EVRANK);
 RZ(w=cvt(FL,w)); d=DAV(w)[0];
 ASSERT(0<=d,EVDOMAIN);
 ASSERT(d<=5.820766091e-11,EVDOMAIN);
 jt->cctdefault=jt->cct=1.0-d;
 return mtv;
}

 A jtdispq(J jt, A w){A z; ASSERTMTV(w); GATV0(z,INT,*jt->disp,1); ICPY(AV(z),1+jt->disp,*jt->disp); return z;}

 A jtdisps(J jt, A w){UC n;
 RZ(w=vi(w));
 n=(UC)AN(w);
 ASSERT(1>=AR(w),EVRANK);
 ASSERT(all1(nubsieve(w)),EVDOMAIN);
 ASSERT(all1(eps(w,eval("1 2 4 5 6"))),EVINDEX);
 *jt->disp=n; DO(n, jt->disp[1+i]=(UC)IAV(w)[i];);
 return mtv;
}

 A jtevmq(J jt, A w){ASSERTMTV(w); return behead(jt->evm);}

 A jtevms(J jt, A w){A t,*tv,*wv;
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

 A jtfxx(J jt, A w){
 ASSERT(AT(w)&LIT+BOX,EVDOMAIN);
 ASSERT(1>=AR(w),EVRANK);
 return fx(ope(w));
}

 A jtiepdoq(J jt, A w){ASSERTMTV(w); return scb(jt->iepdo);}

 A jtiepdos(J jt, A w){B b; RE(b=b0(w)); jt->iepdo=b; return mtm;}

 A jtiepq(J jt, A w){
 ASSERTMTV(w);
 ASSERT(1==AR(w),EVRANK);
 ASSERT(!AN(w),EVDOMAIN);
 return jt->iep?jt->iep:mtv;
}

 A jtieps(J jt, A w){
 ASSERT(1>=AR(w),EVRANK);
 ASSERT(!AN(w)||AT(w)&LIT,EVDOMAIN);
 fa(jt->iep);
 RZ(ras(w)); RZ(jt->iep=w);
 return mtm;
}

 A jtoutparmq(J jt, A w){A z;D*u;I*v;
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

 A jtoutparms(J jt, A w){I*v;
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

 A jtposq(J jt, A w){ASSERTMTV(w); return v2(jt->pos[0],jt->pos[1]);}

 A jtposs(J jt, A w){I n,p,q,*v;
 RZ(w=vi(w));
 n=AN(w); v=AV(w);
 ASSERT(1>=AR(w),EVRANK);
 ASSERT(1==n||2==n,EVLENGTH);
 if(1==n)p=q=*v; else{p=v[0]; q=v[1];}
 ASSERT(BETWEENC(p,0,2)&&BETWEENC(q,0,2),EVDOMAIN);
 jt->pos[0]=p; jt->pos[1]=q;
 return mtv;
}

 A jtppq(J jt, A w){C*end;I k;
 ASSERTMTV(w);
 k = strtoI(3+jt->pp, (char**)&end, 10);
 return sc(k);
}

 A jtpps(J jt, A w){I k;
 RE(sc(k=i0(w))); ASSERT(0<k,EVDOMAIN); ASSERT(k<=NPP,EVLIMIT);
 sprintf(3+jt->pp,FMTI"g", k);
 return mtv;
}

 A jtretcommq(J jt, A w){ASSERTMTV(w); return scb(jt->retcomm);}

 A jtretcomms(J jt, A w){B b; RE(b=b0(w)); jt->retcomm=b; return mtm;}

 A jtseclevq(J jt, A w){ASSERTMTV(w); return sc(jt->seclev);}

 A jtseclevs(J jt, A w){I k;
 RE(k=i0(w));
 ASSERT(0==k||1==k,EVDOMAIN);
 if(!jt->seclev&&1==k)jt->seclev=(UC)k;
 return mtm;
}

 A jtsysq(J jt, A w){I j;
 ASSERTMTV(w);
 j=5;
 return sc(j);
}

 A jtxepq(J jt, A w){
 ASSERTMTV(w);
 return jt->xep?jt->xep:mtv;
}

 A jtxeps(J jt, A w){
 ASSERT(1>=AR(w),EVRANK);
 ASSERT(!AN(w)||AT(w)&LIT,EVDOMAIN);
 fa(jt->xep);
 RZ(ras(w)); RZ(jt->xep=w);
 return mtm;
}

 A jtasgzombq(J jt, A w){ASSERTMTV(w); return sc(jt->asgzomblevel);}

 A jtasgzombs(J jt, A w){I k;
 RE(k=i0(w));
 ASSERT(BETWEENC(k,0,2),EVDOMAIN);
 jt->asgzomblevel=(C)k;
 return mtm;
}

// enable/disable tstack auditing, since some testcases run too long with it enabled
// bit 0 is set to disable, bit 1 is a one-shot to ask for an audit
// result is old value
 A jtaudittdisab(J jt, A w){
 return sc(0);
}
