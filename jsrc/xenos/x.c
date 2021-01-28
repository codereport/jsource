/* Copyright 1990-2008, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Xenos aka Foreign: External, Experimental, & Extra                      */

#include <sys/types.h>
#include <unistd.h>

#include "j.h"
#include "x.h"

#define SDERIV(id,f1,f2,flag,m,l,r)  \
 fdef(0,id,VERB,secf1,secf2,a,w,v2((I)(f1?f1:jtdomainerr1),(I)(f2?f2:jtdomainerr2)),(flag),(I)m,(I)l,(I)r)

#define SDERI2(id,f1,f2,flag,m,l,r)  \
 fdef(0,id,VERB,f1,   secf2,a,w,v2((I)(f1?f1:jtdomainerr1),(I)(f2?f2:jtdomainerr2)),(flag),(I)m,(I)l,(I)r)


static DF1(secf1){F1PREFIP; A h=FAV(self)->fgh[2]; ASSERT(!jt->seclev,EVSECURE); return CALL1IP((AF)AV(h)[0],  w,self);}
static DF2(secf2){F2PREFIP; A h=FAV(self)->fgh[2]; ASSERT(!jt->seclev,EVSECURE); return CALL2IP((AF)AV(h)[1],a,w,self);}

// undocumented 13!:80 functions, used to test condrange
static DF2(jtfindrange){
 ARGCHK2(a,w);
 I *av = AV(a);
 CR rng = condrange(AV(w),AN(w),av[0],av[1],av[2]);
 return v2(rng.min,rng.range);
} // 13!:80
static DF2(jtfindrange4){
 ARGCHK2(a,w);
 I *av = AV(a);
 CR rng = condrange4(C4AV(w),AN(w),av[0],av[1],av[2]);
 return v2(rng.min,rng.range);
}  // 13!:81
static DF2(jtfindrange2){
 ARGCHK2(a,w);
 I *av = AV(a);
 CR rng = condrange2(USAV(w),AN(w),av[0],av[1],av[2]);
 return v2(rng.min,rng.range);
}  // 13!:82

// 13!:83  Return header info: t, flag, m, type, c, n, r
static DF1(jthdrinfo){A z;
 ARGCHK1(w);
 GAT0(z,INT,7,1);
 IAV(z)[0]=AK(w); IAV(z)[1]=AFLAG(w); IAV(z)[2]=AM(w); IAV(z)[3]=AT(w); IAV(z)[4]=AC(w); IAV(z)[5]=AN(w); IAV(z)[6]=AR(w);
 return z;
}

F2(jtforeign){I p,q;
 ARGCHK2(a,w);
 p=i0(a); q=i0(w); RE(0);
 if(11==p)return fdef(0,CIBEAM,VERB, jtwd,0L, a,w,0L, VASGSAFE, 1L,RMAX,RMAX);
 ASSERT((UI)p<=(UI)128 && (UI)q<XCC,EVDOMAIN);
 switch(XC(p,q)){
  case XC(0,  0): 
  case XC(0,100): return SDERI2(CIBEAM, jtscm00,      jtscm002,     VASGSAFE,RMAX,RMAX,RMAX);
  case XC(0,  1): 
  case XC(0,101): return SDERI2(CIBEAM, jtscm01,      jtscm012,     VASGSAFE,RMAX,RMAX,RMAX);
  case XC(0, 10): 
  case XC(0,110): return SDERI2(CIBEAM, jtscm10,      jtscm102,     VASGSAFE,RMAX,RMAX,RMAX);
  case XC(0, 11): 
  case XC(0,111): return SDERI2(CIBEAM, jtscm11,      jtscm112,     VASGSAFE,RMAX,RMAX,RMAX);
  case XC(0,2):   return SDERI2(CIBEAM, jtsct1,       jtsct2,       VASGSAFE,RMAX,RMAX,RMAX);
  case XC(0,3):   return SDERI2(CIBEAM, jtscz1,       jtscz2,       VASGSAFE,RMAX,RMAX,RMAX);

  case XC(1,0):   return SDERIV(CIBEAM, jtjdir,       0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(1,1):   return SDERIV(CIBEAM, jtjfread,     0,            VASGSAFE,0,   RMAX,RMAX);
  case XC(1,2):   return SDERIV(CIBEAM, 0,            jtjfwrite,    VASGSAFE,RMAX,RMAX,0   );
  case XC(1,3):   return SDERIV(CIBEAM, 0,            jtjfappend,   VASGSAFE,RMAX,RMAX,0   );
  case XC(1,4):   return SDERIV(CIBEAM, jtjfsize,     0,            VASGSAFE,0,   RMAX,RMAX);
  case XC(1,5):   return SDERIV(CIBEAM, jtjmkdir,     0,            VASGSAFE,0,   RMAX,RMAX);
  case XC(1,6):   return SDERIV(CIBEAM, jtjfatt1,     jtjfatt2,     VASGSAFE,0,   1,   0   );
  case XC(1,7):   return SDERIV(CIBEAM, jtjfperm1,    jtjfperm2,    VASGSAFE,0,   1,   0   );
  case XC(1,11):  return SDERIV(CIBEAM, jtjiread,     0,            VASGSAFE,1,   RMAX,RMAX);
  case XC(1,12):  return SDERIV(CIBEAM, 0,            jtjiwrite,    VASGSAFE,RMAX,RMAX,1   );
  case XC(1,20):  return SDERIV(CIBEAM, jtjfiles,     0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(1,21):  return SDERIV(CIBEAM, jtjopen,      0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(1,22):  return SDERIV(CIBEAM, jtjclose,     0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(1,43):  return SDERIV(CIBEAM, jtpathcwd,    0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(1,44):  return SDERIV(CIBEAM, jtpathchdir,  0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(1,46):  return CDERIV(CIBEAM, jtpathdll,    0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(1,55):  return SDERIV(CIBEAM, jtjferase,    0,            VASGSAFE,0,   RMAX,RMAX);
  case XC(2,0):   return SDERIV(CIBEAM, jthost,       0,            VASGSAFE,1,   RMAX,RMAX);
  case XC(2,1):   return SDERIV(CIBEAM, jthostne,     0,            VASGSAFE,1,   RMAX,RMAX);
  case XC(2,2):   return SDERIV(CIBEAM, jthostio,     0,            VASGSAFE,1,   RMAX,RMAX);
  case XC(2,3):   return SDERIV(CIBEAM, jtjwait,      0,            VASGSAFE,0,   RMAX,RMAX);
// (2,4) reserved for messages from host OS
  case XC(2,5):   return SDERIV(CIBEAM, jtjgetenv,    0,            VASGSAFE,1,   RMAX,RMAX);
  case XC(2,6):   return SDERIV(CIBEAM, jtjgetpid,    0,            VASGSAFE,1,   RMAX,RMAX);
  case XC(2,7):   return SDERIV(CIBEAM, jtjgetx,      0,            VASGSAFE,1,   RMAX,RMAX);
  case XC(2,8):   return SDERIV(CIBEAM, jtcerrno,     0,            VASGSAFE,RMAX,RMAX,RMAX);
// (2,9) reserved for messages from host OS
  case XC(2,55):  return CDERIV(CIBEAM,jtjoff,0,VASGSAFE,RMAX,0,0);

  case XC(3,0):   return CDERIV(CIBEAM, jtstype,      0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(3,1):   return CDERIV(CIBEAM, jtbinrep1,    jtbinrep2,    VASGSAFE,RMAX,RMAX,RMAX);
  case XC(3,2):   return CDERIV(CIBEAM, jtunbin,      0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(3,3):   return CDERIV(CIBEAM, jthexrep1,    jthexrep2,    VASGSAFE,RMAX,RMAX,RMAX);
  case XC(3,4):   return CDERIV(CIBEAM, 0,            jtic2,        VASGSAFE,RMAX,RMAX,RMAX);
  case XC(3,5):   return CDERIV(CIBEAM, 0,            jtfc2,        VASGSAFE,RMAX,RMAX,RMAX);
  case XC(3,6):   return CDERIV(CIBEAM, jtlock1,      jtlock2,      VASGSAFE,RMAX,RMAX,RMAX);
  case XC(3,10):  return CDERIV(CIBEAM, jttobase64,   0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(3,11):  return CDERIV(CIBEAM, jtfrombase64, 0,            VASGSAFE,RMAX,RMAX,RMAX);

  case XC(3,9):   return CDERIV(CIBEAM, 0,            jtnouninfo2,  VASGSAFE,RMAX,RMAX,RMAX);

  case XC(4,0):   return CDERIV(CIBEAM, jtnc,         0,            VASGSAFE,0,   RMAX,RMAX);
  case XC(4,1):   return CDERIV(CIBEAM, jtnl1,        jtnl2,        VASGSAFE,RMAX,RMAX,RMAX);
  case XC(4,3):   return CDERIV(CIBEAM, jtsnl,        0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(4,4):   return CDERIV(CIBEAM, jtscind,      0,            VASGSAFE,0,   RMAX,RMAX);
  case XC(4,5):   return CDERIV(CIBEAM, jtnch,        0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(4,6):   return CDERIV(CIBEAM, jtscriptstring,        0,   VASGSAFE,RMAX,RMAX,RMAX);
  case XC(4,7):   return CDERIV(CIBEAM, jtscriptnum,        0,      VASGSAFE,RMAX,RMAX,RMAX);
  case XC(4,8):   return CDERIV(CIBEAM, jtscnl,       0,            VASGSAFE,RMAX,0,   0   );
  case XC(4,55):  return CDERIV(CIBEAM, jtex,         0,            VASGSAFE,0,   RMAX,RMAX);

  case XC(5,0):   return fdef(0,CIBEAM,ADV, jtfxx,0L,  a,w,0L,        VASGSAFE,0L,  0L,  0L  );
  case XC(5,1):   return CDERIV(CIBEAM, jtarx,        0,            VASGSAFE,0,   RMAX,RMAX);
  case XC(5,2):   return CDERIV(CIBEAM, jtdrx,        0,            VASGSAFE,0,   RMAX,RMAX);
  case XC(5,4):   return CDERIV(CIBEAM, jttrx,        0,            VASGSAFE,0,   RMAX,RMAX);
  case XC(5,5):   return CDERIV(CIBEAM, jtlrx,        0,            VASGSAFE,0,   RMAX,RMAX);
  case XC(5,6):   return CDERIV(CIBEAM, jtprx,        0,            VASGSAFE,0,   RMAX,RMAX);
  case XC(5,7):   return CDERIV(CIBEAM, 0,            jtxrx,        VASGSAFE,RMAX,0,   0   );
  case XC(5,30):  return CDERIV(CIBEAM, 0,            jtoutstr,     VASGSAFE,RMAX,RMAX,RMAX);

  case XC(6,0):   return CDERIV(CIBEAM, jtts0,        0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(6,1):   return CDERIV(CIBEAM, jttss,        0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(6,2):   return CDERIV(CIBEAM, jttsit1,      jttsit2,      VFLAGNONE,1,   0,   1   );
  case XC(6,3):   return CDERIV(CIBEAM, jtdl,         0,            VASGSAFE,0,   RMAX,RMAX);
  case XC(6,4):   return CDERIV(CIBEAM, jtparsercalls,0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(6,5):   return CDERIV(CIBEAM, jtpeekdata,   0,            VASGSAFE,RMAX,RMAX,RMAX);  // turn on to provide a J window into the running system
  case XC(6,8):   return CDERIV(CIBEAM, jtqpfreq,     0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(6,9):   return CDERIV(CIBEAM, jtqpctr,      0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(6,10):  return CDERIV(CIBEAM, jtpmarea1,    jtpmarea2,    VASGSAFE,RMAX,RMAX,RMAX);
  case XC(6,11):  return CDERIV(CIBEAM, jtpmunpack,   0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(6,12):  return CDERIV(CIBEAM, jtpmctr,      0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(6,13):  return CDERIV(CIBEAM, jtpmstats,    0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(6,14):  return CDERIV(CIBEAM, jtinttoe,    0,      VASGSAFE,RMAX,RMAX,RMAX);
  case XC(6,15):  return CDERIV(CIBEAM, jtetoint,    0,      VASGSAFE,RMAX,RMAX,RMAX);
  case XC(6,16):  return CDERIV(CIBEAM, jtetoiso8601,jtetoiso8601,  VASGSAFE,RMAX,RMAX,RMAX);
  case XC(6,17):  return CDERIV(CIBEAM, jtiso8601toe,jtiso8601toe,  VASGSAFE,RMAX,RMAX,RMAX);

  case XC(7,0):   return CDERIV(CIBEAM, jtsp,         0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(7,1):   return CDERIV(CIBEAM, jtsphwmk,     0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(7,2):   return CDERIV(CIBEAM, jtspit,       0,            VFLAGNONE,1,   RMAX,RMAX);
  case XC(7,3):   return CDERIV(CIBEAM, jtspcount,    0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(7,5):   return CDERIV(CIBEAM, jtspfor,      0,            VASGSAFE,0,   RMAX,RMAX);
  case XC(7,6):   return CDERIV(CIBEAM, jtspforloc,   0,            VASGSAFE,0,   RMAX,RMAX);

  case XC(8,0):   return CDERIV(CIBEAM, jtfmt01,      jtfmt02,      VASGSAFE,RMAX,RMAX,RMAX);
  case XC(8,1):   return CDERIV(CIBEAM, jtfmt11,      jtfmt12,      VASGSAFE,RMAX,RMAX,RMAX);
  case XC(8,2):   return CDERIV(CIBEAM, jtfmt21,      jtfmt22,      VASGSAFE,RMAX,RMAX,RMAX);

  case XC(9,0):   return CDERIV(CIBEAM, jtrngseedq,   0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,1):   return CDERIV(CIBEAM, jtrngseeds,   0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,2):   return CDERIV(CIBEAM, jtdispq,      0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,3):   return CDERIV(CIBEAM, jtdisps,      0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,6):   return CDERIV(CIBEAM, jtboxq,       0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,7):   return CDERIV(CIBEAM, jtboxs,       0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,8):   return CDERIV(CIBEAM, jtevmq,       0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,9):   return CDERIV(CIBEAM, jtevms,       0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,10):  return CDERIV(CIBEAM, jtppq,        0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,11):  return CDERIV(CIBEAM, jtpps,        0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,12):  return CDERIV(CIBEAM, jtsysq,       0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,14):  return CDERIV(CIBEAM, jtversq,      0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,16):  return CDERIV(CIBEAM, jtposq,       0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,17):  return CDERIV(CIBEAM, jtposs,       0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,18):  return CDERIV(CIBEAM, jtctq,        0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,19):  return CDERIV(CIBEAM, jtcts,        0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,20):  return CDERIV(CIBEAM, jtmmaxq,      0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,21):  return CDERIV(CIBEAM, jtmmaxs,      0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,24):  return CDERIV(CIBEAM, jtseclevq,    0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,25):  return CDERIV(CIBEAM, jtseclevs,    0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,26):  return CDERIV(CIBEAM, jtiepq,       0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,27):  return CDERIV(CIBEAM, jtieps,       0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,28):  return CDERIV(CIBEAM, jtiepdoq,     0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,29):  return CDERIV(CIBEAM, jtiepdos,     0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,32):  return CDERIV(CIBEAM, jttlimq,      0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,33):  return CDERIV(CIBEAM, jttlims,      0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,34):  return CDERIV(CIBEAM, jtassertq,    0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,35):  return CDERIV(CIBEAM, jtasserts,    0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,36):  return CDERIV(CIBEAM, jtoutparmq,   0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,37):  return CDERIV(CIBEAM, jtoutparms,   0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,38):  return CDERIV(CIBEAM, jtlocsizeq,   0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,39):  return CDERIV(CIBEAM, jtlocsizes,   0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,40):  return CDERIV(CIBEAM, jtretcommq,   0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,41):  return CDERIV(CIBEAM, jtretcomms,   0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,42):  return CDERIV(CIBEAM, jtrngselectq, 0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,43):  return CDERIV(CIBEAM, jtrngselects, 0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,44):  return CDERIV(CIBEAM, jtrngstateq,  0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,45):  return CDERIV(CIBEAM, jtrngstates,  0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,46):  return CDERIV(CIBEAM, jtbreakfnq,   0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,47):  return CDERIV(CIBEAM, jtbreakfns,   0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,52):  return CDERIV(CIBEAM, jtasgzombq,   0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,53):  return CDERIV(CIBEAM, jtasgzombs,   0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(9,56):  return CDERIV(CIBEAM, jtcpufeature, jtcpufeature2, VASGSAFE,RMAX,RMAX,RMAX);
  case XC(9,57):  return CDERIV(CIBEAM, jtaudittdisab, 0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(9,58):  return CDERIV(CIBEAM, jtgemmtune, jtgemmtune2,    VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(9,60):  return CDERIV(CIBEAM, jtleakblockread, 0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(9,61):  return CDERIV(CIBEAM, jtleakblockreset, 0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(9,62):  return CDERIV(CIBEAM, jtdirectdefq,    0,            VASGSAFE,RMAX,RMAX,RMAX); // scaf
  case XC(9,63):  return CDERIV(CIBEAM, jtdirectdefs,    0,            VASGSAFE,RMAX,RMAX,RMAX); // scaf



/* case XC(11,*):      handled at beginning */
/* case XC(12,*):      reserved for D.H. Steinbrook tree stuff         */

  case XC(13,0):  return CDERIV(CIBEAM, jtdbc,        0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,1):  return CDERIV(CIBEAM, jtdbstack,    0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,2):  return CDERIV(CIBEAM, jtdbstopq,    0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,3):  return CDERIV(CIBEAM, jtdbstops,    0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,4):  return CDERIV(CIBEAM, jtdbrun,      0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,5):  return CDERIV(CIBEAM, jtdbnext,     0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,6):  return CDERIV(CIBEAM, jtdbret,      0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,7):  return CDERIV(CIBEAM, jtdbjump,     0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,8):  return CDERIV(CIBEAM, jtdbsig1,     jtdbsig2,     VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,9):  return CDERIV(CIBEAM, jtdbrr1,      jtdbrr2,      VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,10):  return CDERIV(CIBEAM, 0,0, VFLAGNONE,RMAX,RMAX,RMAX);  // still in stdlib
  case XC(13,11): return CDERIV(CIBEAM, jtdberr,      0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,12): return CDERIV(CIBEAM, jtdbetx,      0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,13): return CDERIV(CIBEAM, jtdbcall,     0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,14): return CDERIV(CIBEAM, jtdbtrapq,    0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,15): return CDERIV(CIBEAM, jtdbtraps,    0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,16):  return CDERIV(CIBEAM, 0,0, VFLAGNONE,RMAX,RMAX,RMAX);  // still in stdlib
  case XC(13,17): return CDERIV(CIBEAM, jtdbq,        0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,18): return CDERIV(CIBEAM, jtdbstackz,   0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,19): return CDERIV(CIBEAM, jtdbcutback,  0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,20): return CDERIV(CIBEAM, jtdbstepover1,jtdbstepover2,VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,21): return CDERIV(CIBEAM, jtdbstepinto1,jtdbstepinto2,VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,22): return CDERIV(CIBEAM, jtdbstepout1, jtdbstepout2, VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,80): return CDERIV(CIBEAM, 0,            jtfindrange,  VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,81): return CDERIV(CIBEAM, 0,            jtfindrange4, VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,82): return CDERIV(CIBEAM, 0,            jtfindrange2, VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(13,83): return CDERIV(CIBEAM, jthdrinfo,            0, VFLAGNONE,RMAX,RMAX,RMAX);

  case XC(15,0):  return SDERIV(CIBEAM, 0,            jtcd,         VASGSAFE|VJTFLGOK2,RMAX,   1L,  1L  );
  case XC(15,1):  return SDERIV(CIBEAM, jtmemr,       0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(15,2):  return SDERIV(CIBEAM, 0,            jtmemw,       VASGSAFE,RMAX,   RMAX,RMAX);
  case XC(15,3):  return SDERIV(CIBEAM, jtmema,       0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(15,4):  return SDERIV(CIBEAM, jtmemf,       0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(15,5):  return SDERIV(CIBEAM, jtcdf,        0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(15,6):  return SDERIV(CIBEAM, jtdllsymget,  0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(15,7):  return SDERIV(CIBEAM, jtdllsymset,  0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(15,8):  return SDERIV(CIBEAM, jtgh15,       0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(15,9):  return SDERIV(CIBEAM, jtfh15,       0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(15,10): return SDERIV(CIBEAM, jtcder,       0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(15,11): return SDERIV(CIBEAM, jtcderx,      0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(15,13): return CDERIV(CIBEAM, jtcallback,   0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(15,14): return SDERIV(CIBEAM, jtdllsymdat,  0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(15,15): return SDERIV(CIBEAM, jtmemu,       jtmemu2,      VASGSAFE|VJTFLGOK1,RMAX,0,   0   );
  case XC(15,16): return SDERIV(CIBEAM, jtnfes,       0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(15,17): return SDERIV(CIBEAM, jtcallbackx,  0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(15,18): return SDERIV(CIBEAM, jtnfeoutstr,  0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(18,0):  return CDERIV(CIBEAM, jtlocnc,      0,            VFLAGNONE,0,   RMAX,RMAX);
  case XC(18,1):  return CDERIV(CIBEAM, jtlocnl1,     jtlocnl2,     VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(18,2):  return CDERIV(CIBEAM, jtlocpath1,   jtlocpath2,   VFLAGNONE,0,   1,   0   );
  case XC(18,3):  return CDERIV(CIBEAM, jtloccre1,    jtloccre2,    VFLAGNONE,RMAX,0,   RMAX);
  case XC(18,4):  return CDERIV(CIBEAM, jtlocswitch,  0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(18,5):  return CDERIV(CIBEAM, jtlocname,    0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(18,30): return CDERIV(CIBEAM, jtlocmap,     0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(18,31): return CDERIV(CIBEAM, jtsympool,    0,            VFLAGNONE,RMAX,RMAX,RMAX);
  case XC(18,55): return CDERIV(CIBEAM, jtlocexmark,  0,            VFLAGNONE,0,   RMAX,RMAX);
  
  case XC(128,0): return CDERIV(CIBEAM, jtqr,         0,            VASGSAFE,2,   RMAX,RMAX);
  case XC(128,1): return CDERIV(CIBEAM, jtrinv,       0,            VASGSAFE,2,   RMAX,RMAX);
  case XC(128,2): return CDERIV(CIBEAM, 0,            jtapplystr,   VFLAGNONE,RMAX,1,   RMAX);
  case XC(128,3): return CDERIV(CIBEAM, jtcrc1,       jtcrc2,       VASGSAFE,RMAX,RMAX,RMAX);
  case XC(128,4): return CDERIV(CIBEAM, jtrngraw,     0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(128,5): return CDERIV(CIBEAM, jtisnan,      0,            VASGSAFE,RMAX,RMAX,RMAX);
  case XC(128,8): return CDERIV(CIBEAM, jtqhash12,    jtqhash12,    VASGSAFE|VJTFLGOK1|VJTFLGOK2,RMAX,RMAX,RMAX);

  default:        ASSERT(0,EVDOMAIN);  // any unknown combination is a domain error right away
}}

/* 64 bit double trick - null routines here to avoid optimization */
void double_trick(D a,D b,D c,D d,D e,D f,D g,D h){;}
