/* Copyright 1990-2006, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Debug: Error Signalling and Display                                     */


#include "j.h"
#include "d.h"

// All the 'display' routines in this file simply add characters to the error buffer.  They can't be typed
// here, because the error may be captured by a higher-level routine

// add n chars at *s to the error buffer at jt->etxn, increment jt->etxn
static void jtep(J jt,I n,C*s){I m;
 m=NETX-jt->etxn; m=MIN(n,m); 
 if(0<m){MC(jt->etx+jt->etxn,s,m); jt->etxn+=m;}
}

static void jteputs(J jt,C*s){ep((I)strlen(s),s);}

static void jteputc(J jt,C c){ep(1L,&c);}

static void jteputl(J jt,A w){ep(AN(w),CAV(w)); eputc(CLF);}

static void jteputv(J jt,A w){I m=NETX-jt->etxn; if(m>0){jt->etxn+=thv(w,MIN(m,200),jt->etx+jt->etxn);}} // stop writing when there is no room in the buffer
     /* numeric vector w */

static void jteputq(J jt,A w,I nflag){C q=CQUOTE,*s;
 if(equ(ds(CALP),w))eputs(" a."+!nflag);
 else{
  eputc(q);
  s=CAV(w); DO(AN(w), eputc(s[i]); if(q==s[i])eputc(q););
  eputc(q);
}}   /* string w, possibly with quotes */

static void jtefmt(J jt,C*s,I i){
 if(15<NETX-jt->etxn){C*v=jt->etx+jt->etxn; sprintf(v,s,i); jt->etxn+=strlen(v);}
}

void jtshowerr(J jt){C b[1+2*NETX],*p,*q,*r;
 if(jt->etxn&&jt->tostdout){
  p=b; q=jt->etx; r=q+jt->etxn;
  while(q<r&&p<b+2*NETX-3){if(*q==CLF){strcpy(p,jt->outseq); p+=strlen(jt->outseq); ++q;}else *p++=*q++;}  // avoid buffer overrun on huge typeouts
  *p=0;

  jsto(jt,MTYOER,b);
 }
 jt->etxn=0;
}

static void jtdspell(J jt,C id,A w,I nflag){C c,s[5];
 if(id==CFCONS){if(nflag)eputc(' '); eputv(FAV(w)->fgh[2]); eputc(':');}
 else{
  s[0]=' '; s[4]=0;
  spellit(id,1+s);
  c=s[1]; 
  eputs(s+!(c==CESC1||c==CESC2||nflag&&((ctype[(UC)c]&~CA)==0)));
}}

static F1(jtsfn0){return sfn(0,w);}  // return string form of full name for a NAME block
EVERYFS(sfn0overself,jtsfn0,jtover,0,VFLAGNONE)

// print a noun; nflag if space needed before name/numeric; return new value of nflag
static I jtdisp(J jt,A w,I nflag){B b=1&&AT(w)&NAME+NUMERIC;
 if(b&&nflag)eputc(' ');
 switch(CTTZ(AT(w))){
 case B01X:
 case INTX:
 case FLX:
 case CMPXX: 
 case XNUMX: 
 case RATX:  eputv(w);                break;
 case BOXX:
  if(!(AT(w)&BOXMULTIASSIGN)){eputs(" a:"+!nflag); break;}
  // If this is an array of names, turn it back into a character string with spaces between
  else{w=curtail(raze(every2(every(w,(A)&sfn0overself),chrspace,(A)&sfn0overself)));}  // }: (string&.> names) ,&.> ' '  then fall through to display it
 case LITX:  eputq(w,nflag);                break;
 case NAMEX: ep(AN(w),NAV(w)->s);     break;
 case LPARX: eputc('(');              break;
 case RPARX: eputc(')');              break;
 case ASGNX: dspell(CAV(w)[0],w,nflag);       break;
 case MARKX:                          break;
 default:   dspell(FAV(w)->id,w,nflag);     break;
 }
 return b;  // new nflag
}

// display DCPARSE stack frame
static void jtseeparse(J jt,DC d){A*v;I m;
 v=(A*)d->dcy;  /* list of tokens */
 m=d->dcix-1;         /* index of active token when error found */
 I nflag=0;
 I m1=jt->etxn;  // starting index of sentence text
 DO(d->dcn, if(i==m)eputs("    "); nflag=disp(v[i],nflag););  // display tokens with spaces before error
 if(jt->etxn<NETX){  // if we overran the buffer, don't reformat it.  Reformatting requires splitting to words
  // We displayed the sentence.  See if it contains (9 :'string'); if so, replace with {{ string }}
  fauxblock(fauxw); A z=(A)&fauxw;
  AK(z)=jt->etx+m1-(C*)z; AFLAG(z)=0; AT(z)=LIT; AC(z)=ACUC1; AR(z)=1; AN(z)=AS(z)[0]=jt->etxn-m1;  // point to etx for parsed line
  jtunDD((J)((I)jt|JTINPLACEW|JTINPLACEA),z);  // reformat in place
  jt->etxn=m1+AN(z);  // set new end-of-sentence pointer
 }
}    /* display error line */

F1(jtunparse){A*v,z;
 ARGCHK1(w);
 jt->etxn=0; I nflag=0;
 v=AAV(w); DO(AN(w), nflag=disp(v[i],nflag);); z=str(jt->etxn,jt->etx);
 jt->etxn=0;
 return z;
}

// Display DCCALL stack frame
static void jtseecall(J jt,DC d){A a;
 if(a=d->dca)ep(AN(a),NAV(a)->s); 
 efmt(d->dcx&&d->dcy?"[:"FMTI"]":"["FMTI"]",lnumsi(d));
}    /* display function line */

// display error-message line
static void jtdhead(J jt,C k,DC d){C s[]="    "; 
 s[0]=d&&d->dcsusp?'*':'|'; 
 ep(k+1L,s);
}    /* preface stack display line */

void jtdebdisp(J jt,DC d){A*x,y;I e,t;
 e=d->dcj;   // error #, or 0 if no error (if DCCALL or DCPARSE frame)
 t=d->dctype;
 if(e&&!jt->etxn&&(t==DCPARSE||t==DCCALL)){x=e+AAV(jt->evm); dhead(0,0L); eputl(*x);}  // if error, display error header
 switch(t){
  case DCPARSE:  dhead(3,d); seeparse(d); if(NETX==jt->etxn)--jt->etxn; eputc(CLF); break;
  case DCCALL:   dhead(0,d); seecall(d);  eputc(CLF); break;
  case DCSCRIPT: dhead(0,d); efmt("[-"FMTI"] ", d->dcn-1); 
                 if(0<=d->dcm){y=*(d->dcm+AAV(jt->slist)); ep(AN(y),CAV(y));}
                 eputc(CLF); break;
}}

static B jtdebsi1(J jt,DC d){I t;
 RZ(d);
 t=d->dctype;
 debdisp(d); 
 d=d->dclnk;
 RZ(d&&t==DCPARSE);
 t=d->dctype;
 RZ(t==DCSCRIPT||t==DCCALL&&d->dcloc);
 debdisp(d);
 return 1;
}

F1(jtdbstack){DC d=jt->sitop; 
 ASSERTMTV(w);
 if(d){if(DCCALL!=d->dctype)d=d->dclnk; while(d){debdisp(d); d=d->dclnk;}}
 return mtm;
}    /* 13!:1  display SI stack */

F1(jtdbstackz){A y,z; 
 RE(dbstack(w)); 
 RZ(y=str(jt->etxn,jt->etx)); 
 jt->etxn=0; 
 return df1(z,y,cut(ds(CLEFT),num(-2)));
}    /* 13!:18  SI stack as result */


static void jtjsigstr(J jt,I e,I n,C*s){
 if(jt->jerr){jt->curname=0; return;}   // clear error-name indicator
 if(e!=EVSTOP)moveparseinfotosi(jt); jt->jerr=(C)e; jt->jerr1=(C)e; jt->etxn=0;  // before we display, move error info from parse variables to si; but if STOP, it's already installed
 dhead(0,0L);
 if(jt->uflags.us.cx.cx_c.db&&!spc()){eputs("ws full (can not suspend)"); eputc(CLF); jt->uflags.us.cx.cx_c.db=0;}
 ep(n,s);
 if(jt->curname){if(!jt->uflags.us.cx.cx_c.glock){eputs(": "); ep(AN(jt->curname),NAV(jt->curname)->s);} jt->curname=0;}
 eputc(CLF);
 if(n&&!jt->uflags.us.cx.cx_c.glock)debsi1(jt->sitop);
 jt->etxn1=jt->etxn;
}    /* signal error e with error text s of length n */ 

static void jtjsig(J jt,I e,A x){jsigstr(e,AN(x),CAV(x));}
     /* signal error e with error text x */ 

void jtjsigd(J jt,C*s){C buf[100],*d="domain error: ";I m,n,p;
 m=strlen(d); MC(buf,d,m);
 n=strlen(s); p=MIN(n,100-m); MC(buf+m,s,p);
 jsigstr(EVDOMAIN,m+p,buf);
}

void jtjsignal(J jt,I e){A x;
 if(EVATTN==e||EVBREAK==e||e==EVINPRUPT) *jt->adbreak=0;
// template for debug break point
// if(EVDOMAIN==e){
// fprintf(stderr,"domain error\n");
// }
 // Errors > NEVM are internal-only errors that should never make it to the end of execution.
 // Ignore them here - they will not be displayed
 x=BETWEENC(e,1,NEVM)?AAV(jt->evm)[e]:mtv; jsigstr(e,AN(x),CAV(x));
}

void jtjsignal3(J jt,I e,A w,I j){
 if(jt->jerr)return;
 moveparseinfotosi(jt); jt->jerr=(C)e; jt->jerr1=(C)e; jt->etxn=0;  // before we display, move error info from parse variables to si
 dhead(0,0L);
 if(jt->uflags.us.cx.cx_c.db&&!spc()){eputs("ws full (can not suspend)"); eputc(CLF); jt->uflags.us.cx.cx_c.db=0;}
 eputl(AAV(jt->evm)[jt->jerr]);
 if(!jt->uflags.us.cx.cx_c.glock){
  if(e==EVCTRL){dhead(3,0L); efmt("["FMTI"]",j); eputl(w);}
  else{
   dhead(3,0L); eputl(w);
   dhead(3,0L); DQ(j, eputc(' ');); eputc('^'); eputc(CLF);
  }
  debsi1(jt->sitop);
 }
 jt->etxn1=jt->etxn;
}    /* signal error e on line w with caret at j */

static F2(jtdbsig){I e;
 RE(0);
 if(!AN(w))return mtm;
 RZ(w=vi(w)); e=AV(w)[0]; 
 ASSERT(1<=e,EVDOMAIN);
 ASSERT(e<=255,EVLIMIT);
 if(a||e>NEVM){if(!a)a=mtv; RZ(a=vs(a)); jsig(e,a);} else jsignal(e);
 return 0;
}    

F1(jtdbsig1){return dbsig(0L,w);}   /* 13!:8  signal error */
F2(jtdbsig2){return dbsig(a, w);}


F1(jtdberr){ASSERTMTV(w); return sc(jt->jerr1);}           /* 13!:11 last error number   */
F1(jtdbetx){ASSERTMTV(w); return str(jt->etxn1,jt->etx);}  /* 13!:12 last error text     */


A jtjerrno(J jt){
 switch(errno){
  case EMFILE:
  case ENFILE: jsignal(EVLIMIT  ); return 0;
  case ENOENT: jsignal(EVFNAME  ); return 0;
  case EBADF:  jsignal(EVFNUM   ); return 0;
  case EACCES: jsignal(EVFACCESS); return 0;
  default:     jsignal(EVFACE); return 0;
}}   /* see <errno.h> / <winerror.h> */
