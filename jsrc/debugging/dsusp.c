/* Copyright 1990-2006, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Debug: Suspension                                                       */

#include "j.h"
#include "d.h"
#include "words/w.h"

// When we move off of a parser frame, or when we go into debug with a new parser frame, fill the frame with
// the info for the parse that was interrupted
static void movesentencetosi(J jt,void *wds,I nwds,I errwd){if(jt->sitop&&jt->sitop->dctype==DCPARSE){jt->sitop->dcy=(A)wds; jt->sitop->dcn=(I)nwds; jt->sitop->dcix=(I)errwd; }}
static void movecurrtoktosi(J jt){if(jt->sitop&&jt->sitop->dctype==DCPARSE){jt->sitop->dcix=jt->parserstackframe.parsercurrtok; }}
void moveparseinfotosi(J jt){movesentencetosi(jt,jt->parserstackframe.parserqueue,jt->parserstackframe.parserqueuelen,jt->parserstackframe.parsercurrtok);}


/* deba() and debz() must be coded and executed in pairs */
/* in particular, do NOT do error exits between them     */
/* e.g. the following is a NO NO:                        */
/*    d=deba(...);                                       */
/*    ASSERT(blah,EVDOMAIN);                             */
/*    debz()                                             */

DC jtdeba(J jt,C t,void *x,void *y,A fs){DC d;
 {A q; GAT0(q,LIT,sizeof(DST),1); d=(DC)AV(q);}
 memset(d,C0,sizeof(DST));
 if(jt->sitop&&t!=DCJUNK)moveparseinfotosi(jt);  // if we are creating a space between normal and suspension, don't modify the normal stack
 d->dctype=t; d->dclnk=jt->sitop; jt->sitop=d;
 switch(t){
  case DCPARSE:  d->dcy=(A)x; d->dcn=(I)y; break;
  case DCSCRIPT: d->dcy=y; d->dcm=(I)fs; break;
  case DCCALL:   
   d->dcx=x; d->dcy=y; d->dcf=fs; 
   d->dca=jt->curname; d->dcm=NAV(d->dca)->m;
   d->dcstop=-2;
   // dcn fill in in caller
   if(jt->dbss==SSSTEPINTO){d->dcss=SSSTEPINTO; jt->dbssd=d; jt->dbss=0;}
 }
 return d;
}    /* create new top of si stack */

void jtdebz(J jt){jt->sitop=jt->sitop->dclnk;}
     /* remove     top of si stack */

 A jtsiinfo(J jt, A w){A z,*zv;DC d;I c=5,n,*s;
 ASSERTMTV(w);
 n=0; d=jt->sitop; while(d){++n; d=d->dclnk;}
 GATV0(z,BOX,c*n,2); s=AS(z); s[0]=n; s[1]=c; zv=AAV(z);
 d=jt->sitop;
 while(d){
  RZ(zv[0]=sc(d->dctype));
  RZ(zv[1]=d->dcsusp?scc('*'):scc(' '));
  RZ(zv[2]=sc((I)d->dcss));
  RZ(zv[3]=d->dctype==DCCALL?sc(lnumsi(d)):mtv);
  switch(d->dctype){
   case DCPARSE:  RZ(zv[4]=unparse(d->dcy)); break;
   case DCCALL:   RZ(zv[4]=sfn(0,d->dca));   break;
   case DCSCRIPT: zv[4]=d->dcy;              break;
   case DCJUNK:   zv[4]=mtv;                 break; 
  }
  zv+=c; d=d->dclnk;
 }
 return z;
}    /* 13!:32 si info */

I lnumcw(I j,A w){CW*u;
 if(0>j)return -2;
 else if(!w)return j;
 else{u=(CW*)AV(w); DO(AN(w), if(j<=u[i].source)return i;) return IMAX/2;}
}    /* line number in CW corresp. to j */

I lnumsi(DC d){A c;I i;
 if(c=d->dcc){i=d->dcix; return(MIN(i,AN(c)-1)+(CW*)AV(c))->source;}else return 0;
}    /* source line number from DCCALL-type stack entry */



static DC suspset(DC d){DC e=0;
 while(d&&DCCALL!=d->dctype){e=d; d=d->dclnk;}  /* find bottommost call                 */
 if(!(d&&DCCALL==d->dctype))return 0;                /* don't suspend if no such call     */
 if(d->dcc){RZ(e); e->dcsusp=1;}               // if explicit, set susp on line - there should always be a following frame, but if not do nothing
 else      d->dcsusp=1;                         /* if not explicit, set susp on call */
 return d;
}    /* find topmost call and set suspension flag */

static B jterrcap(J jt){A y,*yv;
 jt->dbsusact=SUSCLEAR;
 GAT0(y,BOX,4,1); yv=AAV(y);
 RZ(yv[0]=sc(jt->jerr1));
 RZ(yv[1]=str(jt->etxn1,jt->etx));
 RZ(yv[2]=dbcall(mtv));
 RZ(yv[3]=locname(mtv));
 RZ(symbis(nfs(22L,"STACK_ERROR_INFO_base_"),y,mark));
 return 1;
}    /* error capture */

// suspension.  Loop on keyboard input.  Keep executing sentences until something changes dbsusact.
static void jtsusp(J jt){B t;DC d;
 // normally we run with an empty stack frame which is always ready to hold the display of the next sentence
 // to execute; the values are filled in when there is an error.  We are about to call immex to run sentences,
 // and it will create a stack frame for its result.  CREATION of this stack frame will overwrite the current top-of-stack
 // if it holds error information.  So, we create an empty frame to take the store from immex.  This frame has no display.
 jt->dbsusact=SUSCLEAR;  // if we can't add a frame, exit suspension
 if(!deba(DCJUNK,0,0,0))return; // create spacer frame
 jt->dbsusact=SUSCONT;
 A *old=jt->tnextpushp;  // fence must be after we have allocated out stack block
 d=jt->dcs; t=jt->tostdout;
 jt->dcs=0; jt->tostdout=1;
#if USECSTACK
 jt->cstackmin=MAX(jt->cstackinit-(CSTACKSIZE-CSTACKRESERVE),jt->cstackmin-CSTACKSIZE/10);
#else
 jt->fdepn =MIN(NFDEP ,jt->fdepn +NFDEP /10);
#endif
 jt->fcalln=MIN(NFCALL,jt->fcalln+NFCALL/10);
 if     (jt->dbssexec){RESETERR; immex(jt->dbssexec); tpop(old);}
 else if(jt->dbtrap  ){RESETERR; immex(jt->dbtrap  ); tpop(old);}
 while(jt->dbsusact==SUSCONT){A  inp;
  jt->jerr=0;
  if(jt->iepdo&&jt->iep){jt->iepdo=0; immex(jt->iep); tpop(old);}
  if((inp=jgets("      "))==0)jt->dbsusact==SUSCLEAR;else immex(inp); // read and execute a line, but exit debug if error reading line
  tpop(old);
 }
 if(jt->dbuser){
#if USECSTACK
  jt->cstackmin+=CSTACKSIZE/10;
#else
  jt->fdepn-=NFDEP/10;
#endif
  jt->fcalln-=NFCALL/10;
 } else {
#if USECSTACK
  jt->cstackmin=jt->cstackinit-(CSTACKSIZE-CSTACKRESERVE);
#else
  jt->fdepn =NFDEP;
#endif
  jt->fcalln =NFCALL;
 }
 debz(); 
 jt->dcs=d; jt->tostdout=t;
}    /* user keyboard loop while suspended */

// Go into debug mode.  Run sentences in suspension until we come out of suspension
// Result is the value that will be used for the failing sentence.  This should not be 0 unless there is an error, because
// jtxdefn requires nonzero z during normal operation
static A jtdebug(J jt){A z=0;C e;DC c,d;
 if(jt->dbssd){jt->dbssd->dcss=0; jt->dbssd=0;}  // clear previous single-step state - should do at end instead
// create debug state frame scaf
 RZ(d=suspset(jt->sitop));
 if(d->dcix<0)return 0;  // if the verb has exited, all we can do is return
 e=jt->jerr; jt->jerr=0;
// pass in & rcv debug state frame
 susp();
 switch(jt->dbsusact){
  case SUSRUN:      
   DGOTO(d,d->dcix); break;
  case SUSRET:      
   DGOTO(d,-1) z=jt->dbresult; jt->dbresult=0; break;
  case SUSJUMP: 
   DGOTO(d,lnumcw(jt->dbjump,d->dcc)) break;
  case SUSCLEAR:
   jt->jerr=e;    
   c=jt->sitop; 
   while(c){if(DCCALL==c->dctype)DGOTO(c,-1) c=c->dclnk;} break;
 }
 if(jt->dbsusact!=SUSCLEAR)jt->dbsusact=SUSCONT;
 d->dcsusp=0;
 // If there is an error, set z=0; if not, make sure z is nonzero (use i. 0 0)
  if(jt->jerr)z=0; // return z=0 to cause us to look for resumption address
// return debug state frame scaf
 return z;
}

// post-execution error.  Used to signal an error on sentences whose result is bad only in context, i. e. non-nouns or assertions
// we reconstruct conditions at the beginning of the parse, and set an error on token 1.
A jtpee(J jt,A *queue,CW*ci,I err,I lk,DC c){A z=0;
 ASSERT(lk<=0,err);  //  locked fn is totally opaque, with no stack.  Exit with 0 result, indicating error
 jt->parserstackframe.parserqueue=queue+ci->i; jt->parserstackframe.parserqueuelen=(I4)ci->n; jt->parserstackframe.parsercurrtok=1;  // unless locked, indicate failing-sentence info
 jsignal(err);   // signal the requested error
 // enter debug mode if that is enabled
 if(c&&jt->uflags.us.cx.cx_c.db){jt->sitop->dcj=jt->jerr; z=debug(); jt->sitop->dcj=0;} //  d is PARSE type; set d->dcj=err#; d->dcn must remain # tokens debz();  not sure why we change previous frame
 if(jt->jerr)z=0; return z;  // if we entered debug, the error may have been cleared.  If not, clear the result.  Return debug result, which is result to use or 0 to indicate jump
}

// parsex: parse an explicit defn line when the debugger is running
/* w  - line to be parsed                           */
/* lk - 1 iff locked function; _1 to signal noun error at beginning of sentence */
/* ci - current row of control matrix               */
/* c  - stack entry for dbunquote for this function */
// d - DC area to use in deba

A jtparsex(J jt,A* queue,I m,CW*ci,DC c){A z;B s;
 movesentencetosi(jt,queue,m,0);  // install sentence-to-be-executed for stop purposes
 if(s=dbstop(c,ci->source)){z=0; jsignal(EVSTOP);}
 else                      {z=parsea(queue,m);     }
 // If we hit a stop, or if we hit an error outside of try./catch., enter debug mode.  But if debug mode is off now, we must have just
 // executed 13!:0]0, and we should continue on outside of debug mode.  Error processing filled the current si line with the info from the parse
 if(!z&&jt->uflags.us.cx.cx_c.db){DC t=jt->sitop->dclnk; t->dcj=jt->sitop->dcj=jt->jerr; z=debug(); t->dcj=0;} //  d is PARSE type; set d->dcj=err#; d->dcn must remain # tokens
 return z;
}

A jtdbunquote(J jt,A a,A w,A self,L *stabent){A t,z;B b=0,s;DC d;V*sv;
 sv=FAV(self); t=sv->fgh[0]; 
 RZ(d=deba(DCCALL,a,w,self)); d->dcn=(I)stabent;
 if(CCOLON==sv->id&&(sv->flag&VXOP||t&&NOUN&AT(t))){  // : and executable body: either OP (adv/conj now with noun operands) or m : n
  ras(self); z=a?dfs2(a,w,self):dfs1(w,self); fa(self);
 }else{                              /* tacit    */
  d->dcix=0;  // set a pseudo-line-number for display purposes for the tacit 
  do{
   d->dcnewlineno=0;  // turn off 'reexec requested' flag
   if(s=dbstop(d,0L)){z=0; jsignal(EVSTOP);}  // if this line is a stop
   else              {ras(self); z=a?dfs2(a,w,self):dfs1(w,self); fa(self);}
   // If we hit a stop, or if we hit an error outside of try./catch., enter debug mode.  But if debug mode is off now, we must have just
   // executed 13!:8]0, and we should continue on outside of debug mode
   if(!z&&jt->uflags.us.cx.cx_c.db){d->dcj=jt->jerr; movecurrtoktosi(jt); z=debug(); if(self!=jt->sitop->dcf)self=jt->sitop->dcf;}
// look at debug frame for looping scaf
   if(b){fa(a); fa(w);}
   if(b=jt->dbalpha||jt->dbomega){a=jt->dbalpha; w=jt->dbomega; jt->dbalpha=jt->dbomega=0;}
  }while(d->dcnewlineno&&d->dcix!=-1);  // if suspension tries to reexecute a line other than -1 (which means 'exit'), reexecute the tacit definition
 }
 if(d->dcss)ssnext(d,d->dcss);
 if(jt->dbss==SSSTEPINTOs)jt->dbss=0;
 debz();
 return z;
}    /* function call, debug version */


 A jtdbc(J jt, A w){UC k;
 if(!w) return 0;
 if(AN(w)){
  RE(k=(UC)i0(w));
  ASSERT(!(k&~1),EVDOMAIN);
  ASSERT(!k||!jt->uflags.us.cx.cx_c.glock,EVDOMAIN);
 }
 jt->redefined=0;
 if(AN(w)){
  jt->uflags.us.cx.cx_c.db=jt->dbuser=k; jt->cxspecials=1;
#if USECSTACK
  jt->cstackmin=jt->cstackinit-((CSTACKSIZE-CSTACKRESERVE)>>k);
#else
  jt->fdepn=NFDEP>>k;
#endif
  jt->fcalln=NFCALL/(k?2:1);
 }
 jt->dbsusact=SUSCLEAR; 
 return mtm;
}    /* 13!:0  clear stack; enable/disable suspension */

 A jtdbq(J jt, A w){ASSERTMTV(w); return sc(jt->dbuser);}
     /* 13!:17 debug flag */

 A jtdbrun (J jt, A w){ASSERTMTV(w); jt->dbsusact=SUSRUN;  return mtm;}
     /* 13!:4  run again */

 A jtdbnext(J jt, A w){ASSERTMTV(w); jt->dbsusact=SUSNEXT; return mtm;}
     /* 13!:5  run next */

 A jtdbret (J jt, A w){if(!w) return 0; jt->dbsusact=SUSRET; ras(w); jt->dbresult=w; return mtm;}
     /* 13!:6  exit with result */

 A jtdbjump(J jt, A w){RE(jt->dbjump=i0(w)); jt->dbsusact=SUSJUMP; return mtm;}
     /* 13!:7  resume at line n (return result error if out of range) */

static A jtdbrr(J jt,A a,A w){DC d;
 RE(0);
 d=jt->sitop; while(d&&DCCALL!=d->dctype)d=d->dclnk; 
 ASSERT(d&&VERB&AT(d->dcf)&&!d->dcc,EVDOMAIN);  /* must be explicit verb */
 RZ(ras(a)); jt->dbalpha=a; RZ(ras(w)); jt->dbomega=w; 
 jt->dbsusact=SUSRUN;
 return mtm;
}

 A jtdbrr1 (J jt, A w){return dbrr(0L,w);}   /* 13!:9   re-run with arg(s) */
 A jtdbrr2 (J jt,A a,A w){return dbrr(a, w);}

 A jtdbtrapq(J jt, A w){ASSERTMTV(w); return jt->dbtrap?jt->dbtrap:mtv;}
     /* 13!:14 query trap */

 A jtdbtraps(J jt, A w){RZ(w=vs(w)); fa(jt->dbtrap); if(AN(w)){RZ(ras(w)); jt->dbtrap=w;}else jt->dbtrap=0L; return mtm;}
     /* 13!:15 set trap */
