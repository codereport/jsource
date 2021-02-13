/* Copyright 1990-2017, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Initializations                                                         */

#include "j.h"
#include "words/w.h"

// These statics get copied into jt for cache footprint.  If you change them,
// change the definition in jt.h

// For each Type, the length of a data-item of that type.  The order
// here is by number of trailing 0s in the (32-bit) type; aka the bit-number index.
// Example: LITX is 1, so location 1 contains sizeof(C)
#define TPSZ(name) [name##X] = name##SIZE
static const UC typesizes[32] = {
TPSZ(B01), TPSZ(LIT), TPSZ(INT), TPSZ(FL), TPSZ(CMPX), TPSZ(BOX), TPSZ(XNUM), TPSZ(RAT), 
TPSZ(SB01), TPSZ(SLIT), TPSZ(SINT), TPSZ(SFL), TPSZ(SCMPX), TPSZ(SBOX), TPSZ(SBT), TPSZ(C2T), 
TPSZ(C4T), TPSZ(ASGN), TPSZ(MARK), TPSZ(NAME), TPSZ(SYMB), TPSZ(CONW), TPSZ(LPAR), TPSZ(RPAR), 
[ADVX] = INTSIZE, [CONJX] = INTSIZE, [VERBX] = INTSIZE  // note ACV sizes are in INTs
};

// Priority is
// B01 LIT C2T C4T INT BOX XNUM RAT SBT FL CMPX
// For sparse types, we encode here the corresponding dense type
static const UC typepriority[] = {   // convert type bit to priority
0, 1, 4, 9, 10, 5, 6, 7,  // B01-RAT
0, 0, 0, 1, 4, 9, 10, 5,  // x x SB01-SBOX
8, 2, 3};  // SBT C2T C4T
static const UC prioritytype[] = {  // Convert priority to type bit
B01X, LITX, C2TX, C4TX, INTX, BOXX, XNUMX, RATX, SBTX, FLX, CMPXX};

// create name block for xyuvmn
static A jtmakename(J jt,C*s){A z;I m;NM*zv;
 m=strlen(s);
 GATV0(z,NAME,m,1); zv=NAV(z);  // Use GATV because GA doesn't support NAME type
 memcpy(zv->s,s,m); *(m+zv->s)=0;
 zv->m   =(UC)m; 
 zv->bucket=0;
 zv->bucketx=0;
 zv->flag=NMDOT+NMXY;
 zv->hash=(UI4)nmhash(m,s);
 ACX(z);
 return z;
}

/* 
JE can be used by multiple tasks and threads
tasks are easier than threads
global memory is distinct between tasks but is shared between threads

JE support for multiple tasks is good

JE support for threads has a few problems
 there are a few global constants not handled in globinit
 they need to be found and sorted out

 global storage that changes after globinit is a bad bug waiting to happen

storage belonging to a task or thread MUST be be in the J structure
there are only a few globals that have storage not in J
*/

// globals 
// thread-safe/one-time initialization of all global constants
// Use GA for all these initializations, to save space since they're done only once
B jtglobinit(J jt){A x,y;A *oldpushx=jt->tnextpushp;
 memcpy(jt->typesizes,typesizes,sizeof(jt->typesizes));  // required for ma.  Repeated for each thread in jtinit3
 memcpy(jt->typepriority,typepriority,sizeof(jt->typepriority));  // may not be needed
 memcpy(jt->prioritytype,prioritytype,sizeof(jt->prioritytype));  // may not be needed
 jt->adbreakr=jt->adbreak=&breakdata; /* required for ma to work */
 jtmeminit(jt);  /* required for ma to work */
 RZ(y=jtrifvs(jt,jtstr(jt,1L,"z")));     ACX(y); AS(y)[0]=BUCKETXLOC(1,"z");   // for paths, the shape holds the bucketx
 GA(x,BOX, 1,1,0     ); ACX(x); AAV(x)[0]=y;                zpath      =x;  AFLAG(zpath) |= (AT(zpath)&TRAVERSIBLE);  // ensure that traversible types in pst are marked traversible, so tpush/ra/fa will not recur on them
 RZ(mnuvxynam[0]=jtmakename(jt,"m"));
 RZ(mnuvxynam[1]=jtmakename(jt,"n"));
 RZ(mnuvxynam[2]=jtmakename(jt,"u"));
 RZ(mnuvxynam[3]=jtmakename(jt,"v"));
 RZ(mnuvxynam[4]=jtmakename(jt,"x"));
 RZ(mnuvxynam[5]=jtmakename(jt,"y"));
 // can be left at initial value v00[0]=v00[1]=0;   // vector 0 0, for rank
 pf=qpf();
 jtpinit(jt);

 // take all the permanent blocks off the tpop stack so that we don't decrement their usecount.  All blocks allocated here must be permanent
 jt->tnextpushp=oldpushx;
 DO(IOTAVECLEN, iotavec[i]=i+IOTAVECBEGIN;)  // init our vector of ascending integers

 return 1;
}

static B jtevinit(J jt){A q,*v;
 GA(q,BOX,1+NEVM,1,0); v=AAV(q);
 DO(AN(q), v[i]=mtv;);
 v[EVALLOC  ]=jtcstr(jt,"allocation error"           );
 v[EVASSERT ]=jtcstr(jt,"assertion failure"          );
 v[EVATTN   ]=jtcstr(jt,"attention interrupt"        );
 v[EVBREAK  ]=jtcstr(jt,"break"                      );
 v[EVCTRL   ]=jtcstr(jt,"control error"              );
 v[EVDOMAIN ]=jtcstr(jt,"domain error"               );
 v[EVFACCESS]=jtcstr(jt,"file access error"          );
 v[EVFNAME  ]=jtcstr(jt,"file name error"            );
 v[EVFOLDLIMIT]=jtcstr(jt,"fold limit"            );
 v[EVFNUM   ]=jtcstr(jt,"file number error"          );
 v[EVILNAME ]=jtcstr(jt,"ill-formed name"            );
 v[EVILNUM  ]=jtcstr(jt,"ill-formed number"          );
 v[EVINDEX  ]=jtcstr(jt,"index error"                );
 v[EVINPRUPT]=jtcstr(jt,"input interrupt"            );
 v[EVFACE   ]=jtcstr(jt,"interface error"            );
 v[EVLENGTH ]=jtcstr(jt,"length error"               );
 v[EVLIMIT  ]=jtcstr(jt,"limit error"                );
 v[EVLOCALE ]=jtcstr(jt,"locale error"               );
 v[EVNAN    ]=jtcstr(jt,"NaN error"                  );
 v[EVNONCE  ]=jtcstr(jt,"nonce error"                );
 v[EVNONNOUN]=jtcstr(jt,"noun result was required"   );
 v[EVSPARSE ]=jtcstr(jt,"non-unique sparse elements" );
 v[EVOPENQ  ]=jtcstr(jt,"open quote"                 );
 v[EVWSFULL ]=jtcstr(jt,"out of memory"              );
 v[EVRANK   ]=jtcstr(jt,"rank error"                 );
 v[EVRO     ]=jtcstr(jt,"read-only data"             );
 v[EVSECURE ]=jtcstr(jt,"security violation"         );
 v[EVSPELL  ]=jtcstr(jt,"spelling error"             );
 v[EVSTACK  ]=jtcstr(jt,"stack error"                );
 v[EVSTOP   ]=jtcstr(jt,"stop"                       );
 v[EVSYNTAX ]=jtcstr(jt,"syntax error"               );
 v[EVSYSTEM ]=jtcstr(jt,"system error"               );
 v[EVTHROW  ]=jtcstr(jt,"uncaught throw."            );
 v[EVTIME   ]=jtcstr(jt,"time limit"                 );
 v[EVVALUE  ]=jtcstr(jt,"value error"                );
 ras(q); jt->evm=q;
 if(jt->jerr){printf("evinit failed; error %hhi\n", jt->jerr); return 0;} else return 1;
}

/* static void sigflpe(int k){jtjsignal(jt,EVDOMAIN); signal(SIGFPE,sigflpe);} */

static B jtconsinit(J jt){D y;
// This is an initialization routine, so memory allocations performed here are NOT
// automatically freed by jttpop(jt,)
jt->asgzomblevel = 1;  // allow premature change to zombie names, but not data-dependent errors
jt->assert = 1;
jt->directdef = 1;  // scaf
 memcpy(jt->bx,"+++++++++|-",sizeof(jt->bx));
 jt->cctdefault=jt->cct= 1.0-FUZZ;
 jt->disp[0]=1; jt->disp[1]=5;
 jt->fcalln=NFCALL;
 jt->cstackinit=(uintptr_t)&y;  // use a static variable to get the stack address
 jt->cstackmin=jt->cstackinit-(CSTACKSIZE-CSTACKRESERVE);
 jt->outmaxafter=222;
 jt->outmaxlen=256;
 strcpy(jt->outseq,"\x0a");
 strcpy(jt->pp,"%0.6g");
 jt->retcomm=1;
 jt->tostdout=1;
 jt->transposeflag=1;
// jt->int64rflag=0;
 jt->xmode=XMEXACT;
 memcpy(jt->baselocale,"base",sizeof(jt->baselocale));   // establish value & hash of "base"
 jt->baselocalehash=(UI4)nmhash(sizeof(jt->baselocale),jt->baselocale);
 RESETRANK;  // init both ranks to RMAX
  // Init for u./v.
 A uimp=jtca(jt,mnuvxynam[2]); NAV(uimp)->flag|=NMIMPLOC;  // create the name for u.
 jt->implocref[0] = fdef(0,CTILDE,VERB, 0,0, uimp,0L,0L, 0, RMAX,RMAX,RMAX);  //create 'u.'~
 A vimp=jtca(jt,mnuvxynam[3]); NAV(vimp)->flag|=NMIMPLOC;
 jt->implocref[1] = fdef(0,CTILDE,VERB, 0,0, vimp,0L,0L, 0, RMAX,RMAX,RMAX);  //create 'v.'~

 return 1;
}

static C jtjinit3(J jt){
/* required for jdll and doesn't hurt others */
 memcpy(jt->typesizes,typesizes,sizeof(jt->typesizes));  // required for ma.
 memcpy(jt->typepriority,typepriority,sizeof(jt->typepriority));  // required for ma.  Repeated for each thread in jtinit3
 memcpy(jt->prioritytype,prioritytype,sizeof(jt->prioritytype));  // required for ma.  Repeated for each thread in jtinit3
 jt->tssbase=tod();
 jtmeminit(jt);
 jtsesminit(jt);
 jtevinit(jt);
 jtconsinit(jt);
 jtsymbinit(jt);  // must be after consinit
 jtparseinit(jt);
 jtxoinit(jt);
 jtxsinit(jt);
 jtsbtypeinit(jt);
 jtrnginit(jt);
 jtecvtinit(jt);
 // We have completed initial allocation.  Everything allocated so far will not be freed by a tpop, because
 // jttpop(jt,) isn't called during initialization.  So, to keep the memory auditor happy, we reset ttop so that it doesn't
 // look like those symbols have a free outstanding.
 jt->tnextpushp=(A*)(((I)jt->tstackcurr+NTSTACKBLOCK)&(-NTSTACKBLOCK))+1;  // first store is to entry 1 of the first block
 return !jt->jerr;
}

C jtjinit2(J jt,int dummy0,C**dummy1){jt->sesm=1; return jtjinit3(jt);}
