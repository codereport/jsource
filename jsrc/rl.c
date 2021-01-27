/* Copyright 1990-2010, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Representations: Linear and Paren                                       */

#include "j.h"
#include <ctype.h>

#define F1X(f)           A f(J jt,A w,A *ltext)
#define DF1X(f)           A f(J jt,A w,A self,A *ltext)
#define F2X(f)           A f(J jt,A a,A w,A *ltext)
static F1X(jtlnoun);
static F1X(jtlnum);
static DF1X(jtlrr);

#define NUMV(c)  (((1LL<<C9)|(1LL<<CD)|(1LL<<CS)|(1LL<<CA)|(1LL<<CN)|(1LL<<CB))&(1LL<<(c)))

#define parfn ((I)jtinplace&JTPARENS?jtlcpb:jtlcpa)
#define tiefn ((I)jtinplace&JTPARENS?jtltieb:jtltiea)

static B jtlp(J jt,A w){F1PREFIP;B b=1,p=0;C c,d,q=CQUOTE,*v;I j=0,n;
 ARGCHK1(w);
 n=AN(w); v=CAV(w); c=*v; d=*(v+n-1);
 if(1==n||(2==n||3>=n&&' '==c)&&(d==CESC1||d==CESC2)||vnm(n,v))return 0;
 if(C9==ctype[(UC)c])DQ(n-1, d=c; c=ctype[(UC)*++v]; if(b=!NUMV(c)||d==CS&&c!=C9)break;)
 else if(c==q)   DQ(n-1, c=*v++; p^=(c==q); if(b=(p^1)&(c!=q)){break;})
 else if(c=='(') DQ(n-1, c=*v++; j+=c=='('?1:c==')'?-1:0; if(b=!j)break;)
 return b;
}    /* 1 iff put parens around w */

static A jtlcpa(J jt,B b,A w){F1PREFIP;A z=w;C*zv;I n;
 ARGCHK1(w);
 if(b){n=AN(w); GATV0(z,LIT,2+n,1); zv=CAV(z); *zv='('; MC(1+zv,AV(w),n); zv[1+n]=')';}
 return z;
}    /* if b then (w) otherwise just w */

static A jtlcpb(J jt,B b,A w){F1PREFIP;A z=w;B p;C c,*v,*wv,*zv;I n;
 ARGCHK1(w);
 n=AN(w); wv=CAV(w); 
 if(!b){
  c=ctype[(UC)*wv]; v=wv; p=0;
  if     (c==CQ)DQ(n-1, c=ctype[(UC)*++v]; p^=(c==CQ); if(p&(c!=CQ)){b=1; break;})
  else if(c==C9)DQ(n-1, c=ctype[(UC)*++v]; if(!(c==C9   ||c==CS   )){b=1; break;})
  else          DQ(n-1, c=      *++v ; if(!(c==CESC1||c==CESC2)){b=1; break;});
  if(b&&vnm(n,wv))b=0;
 }
 if(b){GATV0(z,LIT,2+n,1); zv=CAV(z); *zv='('; MC(1+zv,wv,n); zv[1+n]=')';}
 return z;
}

static A jtlcpx(J jt,A w){F1PREFIP;ARGCHK1(w); return parfn(jtinplace,lp(w),w);}

static F1X(jtltiea){F1PREFIP;A t,*v,*wv,x,y;B b;C c;I n;
 ARGCHK1(w);
 n=AN(w); wv=AAV(w);  RZ(t=spellout(CGRAVE));
 GATV0(y,BOX,n+n,1); v=AAV(y);
 DO(n, *v++=i?t:mtv; x=wv[i]; c=ID(x); RZ(x=lrr(x)); 
     b=BETWEENC(c,CHOOK,CFORK)||i&&lp(x); RZ(*v++=parfn(jtinplace,b,x)););
 return raze(y);
}

static F1X(jtltieb){F1PREFIP;A pt,t,*v,*wv,x,y;B b;C c,*s;I n;
 ARGCHK1(w);
 n=AN(w); wv=AAV(w);  RZ(t=spellout(CGRAVE)); RZ(pt=over(scc(')'),t));
 GATV0(y,BOX,n+n,1); v=AAV(y);
 if(1>=n)x=mtv; else{GATV0(x,LIT,n-2,1); s=CAV(x); DQ(n-2, *s++='(';);}
 DO(n, x=i==1?t:x; x=i>1?pt:x; *v++=x; x=wv[i]; c=ID(x); RZ(x=lrr(x)); 
     b=BETWEENC(c,CHOOK,CFORK)||i&&lp(x); RZ(*v++=parfn(jtinplace,b,x)););
 return raze(y);
}

// return string for the shape: 's$'
static F1X(jtlsh){F1PREFIP;return apip(thorn1(shape(w)),spellout(CDOLLAR));}

// return something to turn a list into the shape:
static F1X(jtlshape){F1PREFIP;I r,*s;
 ARGCHK1(w);
 r=AR(w); s=AS(w);
 return 2==r&&(1==s[0]||1==s[1]) ? spellout((C)(1==s[1]?CCOMDOT:CLAMIN)) : !r ? mtv :
     1<r ? lsh(w) : 1<AN(w) ? mtv : spellout(CCOMMA);
}

static F1X(jtlchar){F1PREFIP;A y;B b,p=1,r1;C c,d,*u,*v;I j,k,m,n;
 ARGCHK1(w);
 m=AN(ds(CALP)); n=AN(w); j=n-m; r1=1==AR(w); u=v=CAV(w); d=*v;  // m=256, n=string length, j=n-256, r1 set if rank is 1, u=v->string, d=first char
 if(0<=j&&r1&&!memcmpne(v+j,AV(ds(CALP)),m)){
  // string ends with an entire a. sequence
  if(!j)return cstr("a.");  // if that's all there is, use a.
  RZ(y=lchar(1==j?scc(*v):str(j,v)));  // recur on the rest of the string to get its lr
  return lp(y)?over(cstr("a.,~"),y):over(y,cstr(",a."));  // use rest,a. or a.,~rest depending on () needs
 }
 if(r1&&m==n&&(y=icap(ne(w,ds(CALP))))&&m>AN(y)){  // if 256-byte string, see where it differs from a.
  if(1==AN(y))RZ(y=head(y));
  return over(over(cstr("a. "),lcpx(lnum(y))),over(cstr("}~"),lchar(from(y,w))));   // use diff indx} a. or the like
 }
 // we will try for quoted string
 j=2; b=7<n||1<n&&1<AR(w);  // j will be # added chars (init 2 for outer quotes); look for repeated chars if 7 chars or rank>1
 DQ(n, c=*v++; j+=c==CQUOTE; b&=c==d; p&=(C)(c-32)<(C)(127-32);); // b=1 if all chars the same; p=1 if all printable; add to j for each quote found
 if(b){n=1; j=MIN(3,j);}  // if all repeated, back to 1 character, which j=2/3 dep whether it is a quote
 if(!p){  // if the string contains a nonprintable, represent it as nums { a.
  k=(UC)d; RZ(y=indexof(ds(CALP),w));
  if(r1&&n<m&&(!k||k==m-n)&&equ(y,apv(n,k,1L)))return over(thorn1(sc(d?-n:n)),cstr("{.a."));
  RZ(y=lnum(y));
  return lp(y)?over(cstr("a.{~"),y):over(y,cstr("{a."));
 }
 // out the enquoted string, preceded the the shape if repeated or not a list
 GATV0(y,LIT,n+j,1); v=CAV(y);
 *v=*(v+n+j-1)=CQUOTE; ++v;
 if(2==j)MC(v,u,n); else DQ(n, *v++=c=*u++; if(c==CQUOTE)*v++=c;);
 return over(b?lsh(w):lshape(w),y);
}    /* non-empty character array */

static F1X(jtlbox){F1PREFIP;A p,*v,*vv,*wv,x,y;B b=0;I n;
 ARGCHK1(w);
 if(equ(ds(CACE),w)&&B01&AT(AAV(w)[0]))return cstr("a:");
 n=AN(w); wv=AAV(w); 
 DO(n, x=wv[i]; if(BOX&AT(x)){b=1; break;}); b|=1==n;
 GATV0(y,BOX,n+n-(1^b),1); v=vv=AAV(y);
 if(b){
  RZ(p=cstr("),(<"));
  DO(n, x=wv[i]; *v++=p; RZ(*v++=lnoun(x)););
  RZ(*vv=cstr(1==n?"<":"(<")); if(1<n)RZ(vv[n+n-2]=cstr("),<"));
  return over(lshape(w),raze(y));
 }
 DO(n, x=wv[i]; if((AR(x)^1)|(~AT(x)&LIT)){b=1; break;});
 if(!b){C c[256],d,*t;UC*s;
  memset(c,1,sizeof(c)); 
  RZ(x=raze(w)); s=UAV(x);
  DQ(AN(x), c[*s++]=0;);
  if(c[CQUOTE]&&equ(w,words(x)))return over(cstr(";:"),lchar(x));
  if(c[d=' ']||c[d='|']||c[d='/']||c[d=',']||c[d=';']){
   GATV0(y,LIT,n+AN(x),1); t=CAV(y);
   DO(n, x=wv[i]; *t++=d; MC(t,AV(x),AN(x)); t+=AN(x););
   RZ(y=lchar(y));
   return over(lshape(w),over(cstr(isdigit(CAV(y)[0])?"<;.(_1) ":"<;._1 "),y));
 }}
 RZ(p=cstr(";"));
 DO(n-1, RZ(*v++=lcpx(lnoun(wv[i]))); *v++=p;);
 RZ(*v=lnoun(wv[n-1]));
 return over(lshape(w),raze(y));
}    /* non-empty boxed array */

A jtdinsert(J jt,A w,A ic,I ix){A l=sc4(INT,ix); return over(over(take(l,w),ic),drop(l,w));}   /* insert ic before position ix in w */
A jtdcapp(J jt,A w,C c,A ap){return (memchr(CAV(w),c,AN(w)))?w:over(w,ap);}  /* conditionally append ap to w if it doesn't contain c */

// Apply decoration as needed to a numeric character string w to give it the correct type t
// Result is A block for decorated string
A jtdecorate(J jt,A w,I t){
 if(AN(w)==0)return w;  // if empty string, don't decorate
 if(t&FL){
  // float: make sure there is a . somewhere, or infinity/indefinite ('_' followed by space/end/.), else put '.' at end
  B needdot = !memchr(CAV(w),'.',AN(w));  // check for decimal point
  if(needdot){DO(AN(w), if(CAV(w)[i]=='_' && (i==AN(w)-1 || CAV(w)[i+1]==' ')){needdot=0; break;} )}  // check for infinity
  if(needdot){w=over(w,scc('.')); RZ(w=mkwris(w)); DQ(AN(w) , if(CAV(w)[i]==' ')return w;  if(CAV(w)[i]=='e'){C f='.'; C *s=&CAV(w)[i]; DO(AN(w)-i, C ff=s[i]; s[i]=f; f=ff;)}) }
 }else if(t&INT){
 // integer: if the string contains nothing but one-digit 0/1 values, insert '0' before last number
  I l=AN(w); C *s=CAV(w); do{if((*s&-2)!='0')break; ++s; if(--l==0)break; if(*s!=' ')break; ++s;}while(--l);
  if(l==0){I ls=0; DQ(AN(w), if(CAV(w)[i]==' ') ls=i;); w=ls?jtdinsert(jt,w,scc('0'),ls+1):over(scc('0'),w);}
 }else if(t&XNUM) w=jtdcapp(jt, w,'x',scc('x')); // extended: make sure there is an x somewhere in the string, else put 'x' at end
 else if(t&RAT) w=jtdcapp(jt, w,'r',cstr("r1")); // rational: make sure there is an r somewhere in the string, else put 'r1' at end
 else if(t&CMPX) w=jtdcapp(jt, w,'j',cstr("j0")); // complex: make sure there is a j somewhere in the string, else put "j0" at end
 return w;
}


static F1X(jtlnum1){F1PREFIP;A z,z0;I t;
 ARGCHK1(w);
 t=AT(w);
 RZ(z=t&FL+CMPX?df1(z0,w,fit(ds(CTHORN),sc((I)18))):thorn1(w));
 return decorate(z,t);
}    /* dense non-empty numeric vector */

static F1X(jtlnum){F1PREFIP;A b,d,t,*v,y;B p;I n;
 RZ(t=ravel(w));
 n=AN(w);
 if(7<n||1<n&&1<AR(w)){
  // see if we can use a clever encoding
  d=minus(from(num(1),t),b=from(num(0),t));
  p=equ(t,plus(b,tymes(d,IX(n))));
  if(p){
   if(equ(d,num(0)))return over(lsh(w),lnum1(b));
   GAT0(y,BOX,6,1); v=AAV(y); v[0]=v[1]=v[2]=v[3]=mtv;
   if(p=!(equ(b,sc(n-1))&&equ(d,num(-1)))){
    if     (!equ(b,num(0)   )){v[0]=lnum1(b); v[1]=spellout(CPLUS);}
    if     ( equ(d,num(-1))) v[1]=spellout(CMINUS);
    else if(!equ(d,num(1)    )){v[2]=lnum1(d); v[3]=spellout(CSTAR);}
   }
   v[4]=spellout(CIOTA); v[5]=thorn1(p?shape(w):negate(shape(w)));
   RE(y); return raze(y);
  }
  RESETERR;   // if there was an error getting to p, clear it
 }
 // not clever; just out the atoms
 return over(lshape(w),lnum1(t));
}    /* dense numeric non-empty array */

static F1X(jtlsparse){F1PREFIP;A a,e,q,t,x,y,z;B ba,be,bn;I j,r,*v;P*p;
 ARGCHK1(w);
 r=AR(w); p=PAV(w); a=SPA(p,a); e=SPA(p,e); y=SPA(p,i); x=SPA(p,x);
 bn=0; v=AS(w); DQ(r, if(!*v++){bn=1; break;});
 ba=0; if(r==AR(a)){v=AV(a); DO(r, if(i!=*v++){ba=1; break;});}
 be=!(AT(w)&SFL&&0==*DAV(e));
 if(be)RZ(z=over(lnoun(e),cstr(SB01&AT(w)?"":SINT&AT(w)?"+-~2":SFL&AT(w)?"+-~2.1":"+-~2j1")));
 if(be||ba){
  RZ(z=be?over(lcpx(lnoun(a)),       over(scc(';'),z)):lnoun(a));
  RZ(z=   over(lcpx(lnoun(shape(w))),over(scc(';'),z))         );
 }else RZ(z=lnoun(shape(w))); 
 RZ(z=over(cstr("1$."),z));
 if(bn||!AS(y)[0])return z;
 if(AN(a)){
  RZ(x=lcpx(lnoun(x)));
  RZ(y=1==r?lnoun(ravel(y)):over(cstr("(<\"1)"),lnoun(y)));
  RZ(t=over(x,over(cstr(" ("),over(y,cstr(")}"))))); 
 }else RZ(t=over(lcpx(lnoun(head(x))),cstr(" a:}"))); 
 ba=0; v=AV(a); DO(AN(a), if(i!=*v++){ba=1; break;});
 if(!ba)return over(t,z);
 RZ(q=less(IX(r),a));
 RZ(z=over(over(lcpx(lnoun(q)),cstr("|:")),z));
 RZ(z=over(t,z));
 RZ(q=grade1(over(less(IX(r),q),q)));
 j=r; v=AV(q); DO(r, if(i!=*v++){j=i; break;});
 return over(lcpx(lnoun(drop(sc(j),q))),over(cstr("|:"),z));
}    /* sparse array */

static F1X(jtlnoun0){F1PREFIP;A s,x;B r1;
 ARGCHK1(w);
 r1=1==AR(w); RZ(s=thorn1(shape(w)));
 switch(CTTZ(AT(w))){
  default:    return apip(s,cstr("$00"    ));  // over(cstr("i."),s);
  case LITX:  x=cstr(   "''"); return r1?x:apip(apip(s,scc('$')),x);
  case C2TX:  x=cstr("u: ''"); return r1?x:apip(apip(s,scc('$')),x);
  case C4TX:  x=cstr("10&u: ''"); return r1?x:apip(apip(s,scc('$')),x);
  case BOXX:  return apip(s,cstr("$a:"    ));
  case B01X:  return apip(s,cstr("$0"     ));
  case FLX:   return apip(s,cstr("$0."    ));
  case CMPXX: return apip(s,cstr("$0j0"   ));
  case XNUMX: return apip(s,cstr("$0x"    ));
  case RATX:  return apip(s,cstr("$0r0"   ));
  case SBTX:  return apip(s,cstr("$s: ' '"));
}}   /* empty dense array */


static F1X(jtlnoun){F1PREFIP;I t;
 ARGCHK1(w);
 t=AT(w);
 if(unlikely((t&SPARSE)!=0))return lsparse(w);
 if(!AN(w))return lnoun0(w);
 switch(CTTZ(t)){
  case LITX: return lchar(w);
  case C2TX: return over(cstr("u: "),lnum(uco2(num(3),w)));
  case C4TX: return over(cstr("10&u: "),lnum(uco2(num(3),w)));
  case BOXX: return lbox(w);
  case SBTX: return over(cstr("s: "),lbox(sb2(num(5),w)));
  default:  return lnum(w);
}}

static A jtlsymb(J jt,C c,A w,A *ltext){F1PREFIP;A t;C buf[20],d,*s;I*u;V*v=FAV(w);
 if(VDDOP&v->flag){
  u=AV(v->fgh[2]); s=buf; 
  *s++=' '; *s++='('; s+=sprintf(s,FMTI,*u); spellit(CIBEAM,s); s+=2; s+=sprintf(s,FMTI,u[1]); *s++=')';
  RZ(t=str(s-buf,buf)); 
 }else{RZ(t=spella(w)); if(AN(t)==1&&(CAV(t)[0]=='{'||CAV(t)[0]=='}')){RZ(t=mkwris(t)); AS(t)[0]=AN(t)=2; CAV(t)[1]=' '; }}  // add trailing space to { } to avoid DD codes
 d=cf(t);
 return d==CESC1||d==CESC2?over(chrspace,t):t;
}

static B laa(A a,A w){C c,d;
 if(!(a&&w))return 0;
 c=ctype[(UC)cl(a)]; d=ctype[(UC)cf(w)];
 return ((c|d)&(0xf&~(CA|C9)))^1;  // 1 if c,d both alphameric
}

// Is a string a number?  Must start with a digit and end with digit, x, or .
static B lnn(A a,A w){C c; if(!(a&&w))return 0; c=cl(a); return ('x'==c||'.'==c||C9==ctype[(UC)c])&&C9==ctype[(UC)cf(w)];}

static F2X(jtlinsert){F1PREFIP;A*av,f,g,h,t,t0,t1,t2,*u,y;B b,ft,gt,ht;C c,id;I n;V*v;
 ARGCHK2(a,w);
 n=AN(a); av=AAV(a);  
 v=VAV(w); id=v->id;
 b=id==CCOLON&&VXOP&v->flag;
 I fndx=(id==CBDOT)&&!v->fgh[0]; A fs=v->fgh[fndx]; A gs=v->fgh[fndx^1];  // In verb for m b., if f is empty look to g for the left arg.  It would be nice to be more general
// ?t tells whether () is needed around the f/g/h component
 if(1<=n){f=av[0]; t=fs; c=ID(t); ft=BETWEENC(c,CHOOK,CADVF)||(b||id==CFORK)&&NOUN&AT(t)&&lp(f);}  // f: () if it's hook fork && or noun left end of nvv or n (op)
 if(2<=n){g=av[1]; t=gs; c=ID(t); gt=VERB&AT(w)    ?BETWEENC(c,CHOOK,CFORK):lp(g);}
 if(3<=n){h=av[2]; t=v->fgh[2]; c=ID(t); ht=VERB&AT(w)&&!b?c==CHOOK          :lp(h);}
 switch(!b?id:2==n?CHOOK:CFORK){
  case CADVF:
  case CHOOK:
   GAT0(y,BOX,3,1); u=AAV(y);
   u[0]=f=parfn(jtinplace,ft||lnn(f,g),f);
   u[2]=g=parfn(jtinplace,gt||b,       g);
   u[1]=str(' '==cf(g)||id==CADVF&&!laa(f,g)&&!(lp(f)&&lp(g))?0L:1L," ");
   RE(0); return raze(y);
  case CFORK:
   GAT0(y,BOX,5,1); u=AAV(y);
   RZ(u[0]=f=parfn(jtinplace,ft||lnn(f,g),   f));
   RZ(u[2]=g=parfn(jtinplace,gt||lnn(g,h)||b,g)); RZ(u[1]=str(' '==cf(g)?0L:1L," "));
   RZ(u[4]=h=parfn(jtinplace,ht,             h)); RZ(u[3]=str(' '==cf(h)?0L:1L," "));
   return raze(y);
  default:
   t0=parfn(jtinplace,ft||NOUN&AT(fs)&&!(VGERL&v->flag)&&lp(f),f);
   t1=lsymb(id,w);
   y=over(t0,laa(t0,t1)?over(chrspace,t1):t1);
   if(1==n)return y;
   t2=lcpx(g);
   return over(y,laa(y,t2)?over(chrspace,t2):t2);
}}

// create linear rep for m : n
static F1X(jtlcolon){F1PREFIP;A*v,x,y;C*s,*s0;I m,n;
 RZ(y=unparsem(num(1),w));
 n=AN(y); v=AAV(y); RZ(x=lrr(VAV(w)->fgh[0]));
 if(2>n||2==n&&1==AN(v[0])&&':'==CAV(v[0])[0]){
  if(!n)return over(x,str(5L," : \'\'"));
  y=lrr(v[2==n]);
  if(2==n)y=over(str(5L,"\':\'; "),y);
  return over(over(x,str(3L," : ")),lcpx(y));
 }
 m=0; DO(n, m+=AN(v[i]););
 GATV0(y,LIT,2+n+m,1);
 s=s0=CAV(y);
 DO(n, *s++=CLF; y=v[i]; m=AN(y); MC(s,CAV(y),m); s+=m;);
 *s++=CLF; *s++=')'; 
 RZ(y=str(s-s0,s0));
 *ltext=*ltext?over(*ltext,y):y;
 return over(x,str(4L," : 0"));
}

// Main routine for () and linear rep.  w is to be represented
static DF1X(jtlrr){F1PREFIP;A hs,t,*tv;C id;I fl,m;V*v;
 ARGCHK1(w);
 // If name, it must be in ".@'name', or (in debug mode) the function name, which we will discard
 if(AT(w)&NAME){RZ(w=sfn(0,w));}
 if(AT(w)&NOUN)return lnoun(w);
 v=VAV(w); id=v->id;  // outer verb, & its id
 // if f is 0, we take f from g.  In other words, adverbs can put their left arg in either f or g.  u b. uses g so that it can leave f=0 to allow it to function as an ATOMIC2 op
 I fndx=(id==CBDOT)&&!v->fgh[0]; A fs=v->fgh[fndx]; A gs=v->fgh[fndx^1];  // In verb for m b., if f is empty look to g for the left arg.  It would be nice to be more general
 hs=v->fgh[2]; fl=v->flag; if(id==CBOX)gs=0;  // ignore gs field in BOX, there to simulate BOXATOP
 if(fl&VXOPCALL)return lrr(hs);   // pseudo-named entity created during debug of operator.  The defn is in h
 m=(I )!!fs+(I )(gs&&id!=CBOX)+(I )(id==CFORK)+(I )(hs&&id==CCOLON&&VXOP&fl);  // BOX has g for BOXATOP; ignore it; get # nonzero values in f g h
 if(!m)return lsymb(id,w);  // if none, it's a primitive, out it
 if(evoke(w)){RZ(w=sfne(w)); if(FUNC&AT(w))w=lrr(w); return w;}  // keep named verb as a string, UNLESS it is NMDOT, in which case use the (f.'d) verb value
 if(!(VXOP&fl)&&hs&&BOX&AT(hs)&&id==CCOLON)return lcolon(w);  // x : boxed - must be explicit defn
 GATV0(t,BOX,m,1); tv=AAV(t);
 if(2<m)RZ(tv[2]=incorp(lrr(hs)));   // fill in h if present
 // for top-level of gerund (indicated by self!=0), any noun type could not have come from an AR, so return it as is
 if(1<m)RZ(tv[1]=incorp(fl&VGERR?tiefn(jtinplace,fxeach(gs,(A)&jtfxself[!!self]),ltext):lrr(gs)));  // fill in g if present
 if(0<m)RZ(tv[0]=incorp(fl&VGERL?tiefn(jtinplace,fxeach(fs,(A)&jtfxself[!!self]),ltext):lrr(fs)));  // fill in f (always present)
 return linsert(t,w);
}

// Create linear representation of w.  Call lrr, which creates an A for the text plus jt->ltext which is appended to it.
// jt->lcp and jt->ltie are routines for handling adding enclosing () and handling `
F1(jtlrep){PROLOG(0056);A z;A ltextb=0, *ltext=&ltextb;
 RE(z=jtlrr(jt,w,w,ltext));  // the w for self is just any nonzero to indicate top-level call
 if(*ltext)z=apip(z,*ltext);
 EPILOG(z);
}

// Create paren representation of w.  Call lrr, which creates an A for the text plus jt->ltext which is appended to it.
// jt->lcp and jt->ltie are routines for handling adding enclosing () and handling `
F1(jtprep){PROLOG(0057);A z;A ltextb=0, *ltext=&ltextb;
 RE(z=jtlrr((J)((I)jt|JTPARENS),w,w,ltext));
 if(*ltext)z=apip(z,*ltext);
 EPILOG(z);
}

