/* Copyright 1990-2006, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Words: Numeric Input Conversion                                         */

#include "j.h"

#if (SYS & SYS_LINUX)
#include <stdlib.h>
#endif

#define NUMH(f)  B f(J jt,I n,C*s,void*vv)

/* numd    floating point number (double)                      */
/* numj    complex number                                      */
/* numx    extended precision integer                          */
/* nume    extended precision floating point number (not used) */
/* numr    rational number                                     */
/* numq    nume or numr                                        */
/* numbpx  3b12 or 3p12 or 3x12 number                         */
/*                                                             */
/* numb    subfunction of numbpx                               */
/*                                                             */
/* converts a single number and assigns into result pointer    */
/* returns 0 if error, 1 if ok                                 */

static NUMH(jtnumd){C c,*t;D*v,x,y;
 if(!(n))return 0;
 v=(D*)vv;
 if(((((I)s[0]^'-')-1)&(n-3))<0){   // '-' and n<3
  if(1==n){*v=inf; return 1;}
  else{
   c=s[1];
   if('-'==c){*v=infm; return 1;}
   else if('.'==c){*v=jnan; return 1;}
  }
 }
 x=strtod(s,(char**)&t);
 if(t>=s+n){*v=x; return 1;}  // normal return when field consumed
 if(t<s+n-1&&'r'==*t){y=strtod(1+t,(char**)&t); x=y?x/y:0<x?inf:0>x?infm:0;}  // if r in float value, handle the denominator
 return t>=s+n?(*v=x,1):0;
}

static NUMH(jtnumj){C*t,*ta;D x,y;Z*v;
 v=(Z*)vv;
 if(t=memchr(s,'j',n))ta=0; else t=ta=memchr(s,'a',n);
 if(!(numd(t?t-s:n,s,&x)))return 0;
 if(t){t+=ta?2:1; if(!(numd(n+s-t,t,&y)))return 0;} else y=0;
 if(ta){C c;
  c=*(1+ta);
  if(!(0<=x&&(c=='d'||c=='r')))return 0;
  if(c=='d')y*=PI/180; if(y<=-P2||P2<=y)y-=P2*jfloor(y/P2); if(0>y)y+=P2;
  v->re=y==0.5*PI||y==1.5*PI?0:x*cos(y); v->im=y==PI?0:x*sin(y);
 }else{v->re=x; v->im=y;}
 return 1;
}

static NUMH(jtnumi){I neg;I j;
 neg='-'==s[0]; s+=neg; n-=neg; if(!n)return 0;  // extract & skip sign; exit if no digits
 for(;*s=='0';--n,++s);  // skip leading zeros, even down to nothing, which will be 0 value
 if(!(19>=n))return 0;   // 2^63 is 9223372036854775808.  So a 20-digit input must overflow, and the most a
  // 19-digit number can be is a little way into the negative; so testing for negative will be a valid test for overflow
 j=0; DQ(n, I dig=*s++; if(!BETWEENC(dig,'0','9'))return 0; j=10*j+(dig-'0'););
 *(I*)vv=(j^(-neg))+neg;   // if - was coded, take 2's comp, which will leave IMIN unchanged
 return 1+REPSGN(j&(j-neg));  // overflow if negative AND not the case of -2^63, which shows as IMIN with a negative flag
}     /* called only if 64 bit */

static NUMH(jtnumx){A y;B b,c;C d;I j,k,m,*yv;X*v;
 v=(X*)vv;
 d=s[n-1]; b='-'==s[0]; c='x'==d||'r'==d; s+=b;
 if('-'==d){if(!(2>=n))return 0; if(!(*v=rifvs(vci(1==n?XPINF:XNINF))))return 0; return 1;}
 n-=b+c; if(!(m=(n+XBASEN-1)/XBASEN))return 0; k=n-XBASEN*(m-1);
 GATV0(y,INT,m,1); yv=m+AV(y);
 DQ(m, j=0; DQ(k, I dig=*s++; if(!BETWEENC(dig,'0','9'))return 0; j=10*j+(dig-'0');); *--yv=b?-j:j; k=XBASEN;);
 if(!(*v=yv[m-1]?y:mkwris(xstd(y))))return 0;  // this stores into the extended result
 return 1;
}

static X jtx10(J jt,I e){A z;I c,m,r,*zv;
 m=1+e/XBASEN; r=e%XBASEN;
 GATV0(z,INT,m,1); zv=AV(z);
 DQ(m-1, *zv++=0;);
 c=1; DQ(r, c*=10;); *zv=c;
 return z;
}     /* 10^e as a rational number */

static NUMH(jtnume){C*t,*td,*te;I e,ne,nf,ni;Q f,i,*v,x,y;
 v=(Q*)vv;
 nf=0; i.d=iv1; f.d=iv1;
 if(te=memchr(s,'e',n)){ne=n-(te-s)-1; e=strtoI(1+te,(char**)&t,10);  if(!(!*t&&10>ne))return 0;}
 if(td=memchr(s,'.',n)){nf=te?(te-td)-1:n-(td-s)-1; if(nf)if(!(numx(nf,td+1,&f.n)))return 0;}
 ni=td?td-s:te?te-s:n; if(!(numx(ni,s,&i.n)))return 0;
 x=i;
 if(nf){y.n=iv1; y.d=x10(nf); RE(x='-'==*s?qminus(x,qtymes(f,y)):qplus(x,qtymes(f,y)));}
 if(te){if(0>e){y.n=iv1; y.d=x10(-e);}else{y.n=x10(e); y.d=iv1;} RE(x=qtymes(x,y));}
 *v=x;
 return 1;
}

static NUMH(jtnumr){C c,*t;I m,p,q;Q*v;
 v=(Q*)vv;
 m=(t=memchr(s,'r',n))?t-s:n; if(!(numx(m,s,&v->n)))return 0; v->d=iv1;
 if(t){
  c=s[n-1]; if(!('r'!=c&&'x'!=c))return 0;
  if(!(numx(n-m-1,s+m+1,&v->d)))return 0;
  p=AV(v->n)[0]; q=AV(v->d)[0]; 
  if(!(p!=XPINF&&p!=XNINF||q!=XPINF&&q!=XNINF))return 0;
  RE(*v=qstd(*v));
 }
 return 1;
}

static NUMH(jtnumq){B b=0;C c,*t;
 t=s; DQ(n, c=*t++; if(c=='e'||c=='.'){b=1; break;});
 return b?nume(n,s,vv):numr(n,s,vv);
}

static const Z zpi={PI,0};
static const C dig[]="0123456789abcdefghijklmnopqrstuvwxyz";
static B jtnumb(J jt,I n,C*s,Z*v,Z b){A c,d,y;I k;
 I m=strlen(dig);
 if(!n){*v=zeroZ; return 1;}
 if(!(d=indexof(str(m,(C*)dig),str(n,s))))return 0;
 if(!(all0(eps(sc(m),d))))return 0;
 k=sizeof(Z);
 GAT0(c,CMPX,1,0); MC(AV(c),&b,k); if(!(y=base2(c,d)))return 0; MC(v,AV(y),k);
 return 1;
}

// Convert a constant that is base, p/x-type or complex
static NUMH(jtnumbpx){B ne,ze;C*t,*u;I k,m;Z b,p,q,*v,x,y;
 v=(Z*)vv;
 if(t=memchr(s,'b',n)){
  // base given
  if(!(numbpx(t-s,s,&b)))return 0;
  ++t; if(ne='-'==*t)++t;  // t->first nonsign digit
  m=k=n+s-t; if(u=memchr(t,'.',m))k=u-t;  // m=total # digits, k=# digits before decimal point
  if(!(m>(1&&u)))return 0;   // assert negative, or ((>1 digit)  or (1 digit) and (there is no decimal point)) i. e. there is at least one non-decimal-point
#ifdef NANFLAG
  // if the base is an integer, we can't just do everything in the complex domain because of loss of precision.
  // in that case reproduce the calculation from numb, but with the integer base, and if the result is still integral, flag it
  I intbase=(I)b.re; if(!u && b.im==0.0 && b.re==(D)intbase){A d;
   if(!(d=indexof(str(strlen(dig),(C*)dig),str(m,t))))return 0;  // convert digits to index numbers
   if(!(all0(eps(sc(strlen(dig)),d))))return 0;   // verify only allowed digits in the field
   if(ne)if(!(d=negate(d)))return 0;  // recover negative sign
   if(!(d=bcvt(0,base2(sc(intbase),d))))return 0;  // d =. base #. d converted to smallest possible precision
   if(AT(d)&INT){*(I*)&v->re=IAV(d)[0]; *(I*)&v->im=NANFLAG; return 1;}  // if result is INT, keep it at full precision
  }
#endif
  if(!(numb(k,t,&p,b)))return 0;
  if(u){
   k=m-(1+k);
   if(ze=!(b.re||b.im))b.re=1;
   if(!(numb(k,1+u,&q,b)))return 0;
   if(ze){if(q.re)p.re=inf;} else{DQ(k,q=zdiv(q,b);); p=zplus(p,q);}
  }
  *v=p; if(ne){v->re=-v->re; v->im=-v->im;}
  return 1;
 }
 // not base, must be complex or p/x-type.  Can be complex or (complex)p(complex) or (complex)x(complex)
 if(t=memchr(s,'p',n))u=0; else t=u=memchr(s,'x',n);  // t=0 means 'no p/x, just plain complex' nonzero t-> p/x u=0 means 'p' non0 u means 'x'
 if(!t)return numj(n,s,v);   // if it's a single (complex) number, return it
 // We have to make sure that numeric parts passed to strtod end with null or a certifiable nonnumeric.  On Linux/Mac, 'x' looks numeric if
 // the value starts with '0x'.  So we zap the p/x character before converting the mantissa/exponent, and restore.  We know there will be no exceptions, and that
 // the input area is writable.  The exponent part ends with a natural NUL.
 C savpx = *t; *t=0; B rc = numj(t-s,s,&x); *t = savpx; RZ(rc);
 ++t; if(!(numj(n+s-t,t,&y)))return 0;  // if p- or x-type, get x=mantissa y=exponent
 y = u ? zexp(y) : zpow(zpi,y);  // ^y or pi^y
 *v = ztymes(x,y);   // calculate x*^y or x*pi^y
 return 1;
}

// indicate types found in the input string.
// If x is not set here, numbers ending in x are ill-formed
// Example: '1j1 1x' sets j but not x, so 1x is ill-formed
// Example:  '16b4 1x' similarly, and '4.0 1x' similarly
/* (n,s) string containing the vector constant                */
// returns type bits set in a mask
/* CMPX:  1 iff contains 1j2 or 1ad2 or 1ar2                     */
/* LIT:  1 iff has 1b1a or 1p2 or 1x2 (note: must handle 1j3b4) */
/* XNUM:  1 iff contains 123x                                    */
/* RAT:  1 iff contains 3r4                                     */
/* INT: 1 iff integer (but not x)                              */

#define WDDOT (I)0x2e2e2e2e2e2e2e2e  // a word of '.'
static I jtnumcase(J jt,I n,C*s){B e;C c;I ret;
 // First, a quick pass to see if there are any alphabetics or .
 I *si=(I*)s;  // running pointer to data
 I allor=0, anydot=0;  // will hold mask info over all characters
 DQ(n>>LGSZI, allor|=*si; anydot|=(*si^WDDOT)-VALIDBOOLEAN; ++si;)  // sets a sign bit if anything is '.', valid only if no alphas
 I tailmsk=~((~(I)0)<<((n&(SZI-1))<<3)); allor|=(*si&tailmsk); anydot|=((*si&tailmsk)^WDDOT)-VALIDBOOLEAN;
 if(!(allor&(3*VALIDBOOLEAN<<6))){   // if no 0xc0 bit set, there are no lower-case alphas or non-ASCII chars
  return ((anydot&(VALIDBOOLEAN<<6))==0?INT:0);  // no byte had 0xC0 set; if byte^'.' - 1 had 0x40 set, it must have been '.'.  Set ii if there are none such
 }else{
  // if there are alphabetics/non-ASCII, do the full analysis
  // if it contains 'b' or 'p', that becomes the type regardless of others
  // (types incompatible with that raise errors later)
  ret=(memchr(s,'j',n)||memchr(s,'a',n)?CMPX:0) + (memchr(s,'b',n)||memchr(s,'p',n)?LIT:0);
  if(ret==0){
   ret|=INT;  // default to 'nothing seen except integers'
   // if not j or b type, scan again. x indicates 1x2 or 23x.  Set both
   if(memchr(s,'x',n)){ret|=LIT+XNUM; ret&=~INT;}
   // if string contains r, it's rational (since not ar)
   if(memchr(s,'r',n)){ret|=RAT;    ret&=~INT;}
   // if no x or r found, exit as float
   if(!(ret&RAT+XNUM+INT))return ret;
   // If any . or e found, or 'x' not at the end, treat as float, with exact modes cleared.  LIT could still be set for 1x2
   // Thus, 4. 1r3 produces float, while 4 1r3 produces rational
   e=s[0]; DO(n, c=e; e=s[i+1]; if(c=='.'||c=='e'||c=='x'&&e){return ret&~(XNUM+RAT+INT);});  // must look at stopper because comma strings have multiple NULs at the end
  }
  return ret;
 }
}

// n is string length, s is string representing valid J numbers
A jtconnum(J jt,I n,C*s){PROLOG(0101);A y,z;B (*f)(J,I,C*,void*),p=1;C c,*v;I d=0,e,k,m,t,*yv;
 if(1==n)                {if(k=s[0]-'0',(UI)k<=(UI)9)return num( k); else return ainf;}  // single digit - a number or _
 else if(2==n&&CSIGN==*s){if(k=s[1]-'0',(UI)k<=(UI)9)return num(-k);}
 RZ(y=mkwris(str(1+n,s))); s=v=CAV(y); s[n]=0;  // s->null-terminated string in new copy, which we will modify
 GATV0(y,INT,1+n,1); yv=AV(y);  // allocate area for start/end positions
 C bcvtmask=0;  // bit 1 set to suppress B01, bit 2 to suppress INT
 DO(n, c=*v; c=c==CSIGN?'-':c; c=(c==CTAB)|(c==' ')?C0:c; *v++=c; B b=C0==c; bcvtmask=bcvtmask|(4*(c=='.')+2*((p|b)^1)); yv[d]=i; d+=p^b; p=b;);  // replace _ with -, whitespace with \0; and record start and end positions
   // if we encounter '.', make sure the result is at least FL; if we encounter two non-whitespace in a row, make sure result is at least INT
 yv[d++]=n; m=d>>1;  // append end for last field in case it is missing; m=#fields.  If end was not missing the extra store is harmless
 I tt=numcase(n,s);   // analyze contents of values; returns type flags for chars, as expected except LIT for b
 bcvtmask|=(tt&CMPX+LIT)==CMPX?8:0; // flag to force complex if we have j but not b
 f=jtnumd; t=FL;  f=tt&INT?jtnumi:f; t=tt&INT?INT:t;  f=tt&CMPX+LIT?jtnumbpx:f; t=tt&CMPX+LIT?CMPX:t;  f=tt&XNUM?jtnumx:f; t=tt&XNUM?XNUM:t;  f=tt&RAT?jtnumq:f; t=tt&RAT?RAT:t;  // routine to use, and type of result
 k=bpnoun(t);   // size in bytes of 1 result value
 GA(z,t,m,1!=m,0); v=CAV(z);
 if(t==INT){  // if we think the values are ints, see if they really are
  DO(m, d=i+i; e=yv[d]; if(!numi(yv[1+d]-e,e+s,v)){t=FL; break;} v+=k;);  // read all values, stopping if a value overflows
  if(t!=INT){f=jtnumd; if(SZI==SZD){AT(z)=FL;}else{GATV0(z,FL,m,1!=m);} v=CAV(z);}  // if there was overflow, repurpose/allocate the input with enough space for floats
 }
 if(t!=INT)DO(m, d=i+i; e=yv[d]; ASSERT(f(jt,yv[1+d]-e,e+s,v),EVILNUM); v+=k;);  // read the values as larger-than-int
 z=bcvt(bcvtmask,z);
 EPILOG(z);
}


#define EXEC2F(f,f1,t,T) \
 A f(J jt,A a,A w,I n,I m,I c){A z;B b;C d,*u,*uu,*x,*y;I i,j,k,mc,r;T a0,*zv;  \
  i=0; mc=m*c; u=CAV(w); y=u+n; j=c; uu=u+AN(w); if(mc)*(uu-1)=' ';         \
  r=AR(w)-(I )(1==c); r=MAX(0,r);                                               \
  GA(z,t,mc,r,AS(w)); if(0<r&&1!=c)AS(z)[r-1]=c; zv=(T*)AV(z);            \
  RZ(a=cvt(t,a)); a0=*(T*)AV(a);                                            \
  while(i<mc){                                                              \
   while(u<uu&&C0==*u)++u;                                                  \
   while(u>=y){while(i<j)zv[i++]=a0; j+=c; y+=n; if(i==mc)return z;}             \
   x=strchr(u,C0); if(x<uu)k=x-u; else{*(uu-1)=C0; k=uu-1-u;}               \
   b=','==u[0]||','==u[k-1];                                                \
   x=u; DO(k, d=u[i]; if(','!=d)*x++=d==CSIGN?'-':d;); *x=C0;               \
   if(b||!f1(x-u,u,i+zv))zv[i]=a0;                                          \
   ++i; u+=1+k;                                                             \
  }                                                                         \
  return z;                                                                      \
 }

static EXEC2F(jtexec2x,numx,  XNUM,X)  /* note: modifies argument w */
static EXEC2F(jtexec2q,numq,  RAT, Q)
static EXEC2F(jtexec2z,numbpx,CMPX,Z)

// Try to convert a numeric field to integer.  Result is the integer, and *out is the
// output pointer after the integer - this will be either the \0 at the end of the field, or
// ==in to indicate failure.  Result is 0 on failure, and the output pointer equals the input.
// This routine must recognize all valid ints,  We accept at most one sign, followed by any number
// of digits or commas, followed optionally by a decimal point, followed optionally by a
// string of 0s.  So, 123,456.00 is recognized as an integer.  If the input overflows an
// integer, failure.  For compatibility with non-integer code, we allow commas anywhere in
// the number except the beginning or end.
// This routine assumes 64-bit signed integers.
I strtoint(C* in, C** out) {
 UI res = 0; // init result
 I neg, dig;  // negative flag, digit value
 *out = in;  // assume failure
 if(neg = '-'==*in){   // starting with - could be negative sign.  Remember the sign
  if('.'==*++in){if(in[1]==C0)return 0;}   // if not followed by . it is negative sign.  Followed by . is
    // _. if . is last character.  Step to next character, and fail if _.
 } else if('+'==*in)++in;  // skip + sign
 if(','==*in || C0==*in)return 0;  // if number begins with , or is empty (after sign), reject it.
   // could be just + (error) or just - (inf)
 for(;;++in) {   // Read integer part:
  dig = (I)*in - (I)'0';
  if((UI)dig<=(UI)9){  // numeric digit.  Accept it and check for overflow
   if(res >= 1+IMAX/10) return 0;  // fail if this will overflow for sure.  res could be IMIN
   res = res * 10 + dig; // accept the digit.  This may overflow, but that's not fatal yet if it overflows to IMIN
   if((I)((UI)0-res) > 0)return 0;  // If result overflowed to neg, fail.  We allow IMIN to continue on, representing IMAX+1
   continue;
  }
  if(*in==C0 || *in=='.')break;  // end-of-field or end-of-integer part: exit
  if(','==*in)continue;  // skip comma
  return 0;  // unknown character encountered, fail
 }
 // We have read the integer part.  Check for .[0000]
 if('.'==*in) {   // is '.'?
  for(++in;*in!=C0;++in) {  // Step over '.', then loop till end-of-field or error
   if(*in=='0' || *in==',')continue;   // skip over trailing 0 or comma
   return 0;   // non-zero encountered in field; this can't be an int
  }
 }
 if(','==in[-1])return 0;  // fail if last character of the field is comma.  *in here must be \0
 // It passed as an int.  Return it, unless it overflows on sign
 // If the value is IMIN, it has already had the sign switched; otherwise apply the sign
 if((I)res==IMIN){if(!neg)return 0;}   // -2^63 is OK, but not +2^63
 res=(res^(-neg))+neg;   // change sign if neg
 *out=in;   // Finally, signal success
 return (I)res;  // Return the int value
}

// Install the default into zv[k++]
#define INSDEFAULT {if(tryingint && AT(a)&FL){tryingint=0; DO(k, zv[i] = (D)((I *)zv)[i];)} zv[k++]=a0;}

// Normal numeric-to-character conversion.
// a is the default, w is the character buffer to convert, m is the number of rows of characters,
// n is the length of each row, c is the number of values in each row
// fillreqd is negative if the lines have different lengths
static A jtexec2r(J jt,A a,A w,I n,I m,I c,I fillreqd){A z;B b,e;C d,*u,*uu,*v,*x,*y;D a0,*zv;I k,j,mc,r;
 B tryingint; // set if we have to attempt to convert to int before float, if ints can hold
   // higher precision than float
B valueisint; // set if the value we are processing is really an int
  // Calculate total # result values; set input scan pointer u; set &next row of input y; set end-of-input pointer uu
 // set end-of-result-row counter j
 k=0; mc=m*c; u=CAV(w); y=u+n; j=c; uu=u+AN(w);
 // Rank of result is rank of w, unless the rows have only 1 value; make rows atoms then, removing them from rank
 r=AR(w)-(I )(1==c); r=MAX(0,r); 
 // Allocate the result array, as floats.  If the last atom of shape was not removed, replace it with c, the output length per list
 GATV(z,FL,mc,r,AS(w)); if(0<r&&1!=c)AS(z)[r-1]=c; zv=DAV(z);
 if(!mc)return z;  // If no fields at all, exit with empty result (avoids infinite loop below)
 // Convert the default to float, unless we are trying big integers.  We try ints if the default is int or infinite,
 // but only on 64-bit systems where int and float have the same size
 if(!(tryingint = sizeof(D)==sizeof(I) && (AT(a)&B01+INT || (fillreqd>=0 && AT(a)&FL)))){RZ(a=cvt(FL,a));}
 else if(AT(a)&B01)RZ(a=cvt(INT,a));  // If we are trying ints, we must promote Bool to int
 // Get the default value; supposedly a (D) but if we are trying ints it might be really an (I)
 a0=DAV(a)[0];
 // loop till all results have been produced.  Some values require a restart, so we control this field-by-field
 // rather than row-by-row
 while(1){   // Loop for each field - we know there's at least one, thus n>0
  // Skip over any \0 characters we are pointing at
  while(u<uu&&C0==*u)++u;
  // If we have consumed all the input for the current row, fill the rest of the
  // row with defaults, then advance input & output to next row; return if all done
  while(u>=y){while(k<j)INSDEFAULT j+=c; y+=n;}
  if(k>=mc)break;   // exit loop if all inputs processed
  // Read a number from the input, leaving v pointing to the character that stopped the conversion
  // If we are trying ints first to avoid floating-point truncation, do so
  if(valueisint = tryingint) {
    ((I *)zv)[k] = strtoint(u,&v);  // returns an I, which we store into the nominally-D result array
    if(u==v){valueisint = 0;}
      // The conversion to int failed, but that's not enough for us to write off ints.  Maybe the
      // value was invalid, and we will use the default, which is known to be int.
  }
  if(!valueisint)zv[k]=strtod(u,(char**)&v);
  // We have read a number, either as an int or a float.  Analyze the stopper character
  switch(*v){
   case ',':
    // comma.  We will remove commas from the number and then rescan it.
    b=u==v; x=v;   // b='first character was comma'; x is output pointer for the copy
    while(d=*++v)if(','!=d)*x++=d;   // copy to end-of-field, discarding commas
    if(b||','==*(v-1)){INSDEFAULT u=v;}else while(v>x)*x++=C0;  // if first or last character is comma, use default and continue, skipping the field;
       // otherwise put \0 over the characters after the last copied one, and go back to rescan the number
    continue;
   case '-':
    // hyphen (remember, we converted _ to hyphen to begin with).  It wasn't interpreted as a sign, so it must
    // be an infinity, if it is valid.
    e=u==v; v++; d=*v++; b=e&&C0==*v;  // e means 'first character was -'; d is character after the hyphen;
       // b is set if the - was the first character and the third character is end-of-field
    if     (e&& C0==d){zv[k]=inf;}   // -<end> infinity: take it
    else if(b&&'-'==d){zv[k]=infm;}   // --<end> neginfinity
    else if(b&&'.'==d){zv[k]=jnan;}  // -.<end> NaN
    else{INSDEFAULT --v; while(C0!=*v++); u=v;continue;}   // NOTA; invalid including - in the middle, use default; advance to end-of-field
    // the non-error cases fall through to process the input value...
   case C0:
    // \0 (normal end-of-field), or fallthrough from '-' non-error.  Either way it's a number.  Accept the number and continue.  But if this is the
    // first float that forces us to switch to floats, we have to do that
    if (valueisint != tryingint) {
     // We hit a float.  Forget about ints
     tryingint = 0;
     // Convert the default value to float if it isn't already
     if(AT(a)&INT)a0=(D)IAV(a)[0];
     // Convert all previously-read values to float.  Also converted the default value above
     // We have to use pointer aliasing to read the value in zv as an int
     DO(k, zv[i] = (D)((I *)zv)[i];)
    }
    k++; u=v; continue;   // move to the next input number
   case 'a':  case 'b':  case 'j':  case 'p':  case 'r':  case 'x':
    // case requiring analysis for complex numbers - switch over to that code, abandoning our work here
    if(u!=v)return exec2z(a,w,n,m,c);
    // but if special character at beginning of field, that's not a valid complex number, fall through to...
   default:
    // Other stopper character, that's invalid, use default, skip the field
    INSDEFAULT while(C0!=*++v); u=v;
 }}
 // All done.  If we ended still looking for ints, the whole result must be int, so flag it as such
 if(tryingint)AT(z) = INT;
 return z;
}

// x ". y
F2(jtexec2){A z;B b,p;C d,*v;I at,c,i,k,m,n,r,*s;
 ARGCHK2(a,w);
 ASSERT(!AR(a),EVRANK);  // x must be an atom
 at=AT(a);
 ASSERT(at&NUMERIC,EVDOMAIN);  // x must be numeric
 if(!(LIT&AT(w)))RZ(w=toc1(0,w));  // convert y to ASCII if it isn't already; error if there are non-ASCII characters; error if not character type
 m=n=c=0; r=AR(w);   // get rank of y argument

 // process each list of the input to see how many numbers it contains.  We will
 // use this to set the shape of the result area
 I fillreqd=0;  // will be <0 if lines have different lengths
 if(!r||AS(w)[r-1]){    // skip the count if y is atom, or the last axis of y has dimension 0.   Nothing to count.
  // Calculate w ,"1 0 ' '   to end each (or only) line with delimiter
  {A t; RZ(w=IRS2(w,chrspace,0L,1L,0L,jtover,t)); makewritable(w);}  // New w will be created
  v=CAV(w); r=AR(w); s=AS(w); n=s[r-1]; PRODX(m,r-1,s,1)  // v->data, m = #lists, n = length of each list
  for(i=0;i<m;++i){I j;
   // b is set when the current character is significant (i. e. not whitespace); p when the previous character was significant
   // k counts the number of words on this line
   // c is the max # words found on a line
   b=0; k=0; 
   for(j=0;j<n;++j){
    p=b; d=*v;
    // replace ' ' and TAB with \0; replace _ with -
    b=(d!=' ')&(d!=CTAB); d=d==CSIGN?'-':d; d&=-b;
    *v=d; ++v; k+=b&~p;  // write out the possibly-changed character; if char is a new start-of-field, increment word count
   }
   fillreqd |= -i&-(c^k);   // indic fill needed if c!=k except on the first line
   c=k>c?k:c;  // c is running max of linelengths
 }}
 // c is length of each output list; the list has had _ replaced by - and space/TAB replaced by \0
 // Classify the input y according the types it contains
 I tt=numcase(m*n,CAV(w));

 // Select the conversion routine.  We allow -0 in the result now
 if(at&CMPX)                z=exec2z(a,w,n,m,c);  // If x argument is complex, force that mode
 else if(tt&RAT)                 z=exec2q(a,w,n,m,c);  // Otherwise, if data contains rationals, use that mode
 else if(tt&XNUM&&at&B01+INT+XNUM)z=exec2x(a,w,n,m,c);   // Otherwise if data contains extended integers, use that mode as long as x is compatible
 else                       z=exec2r(a,w,n,m,c,fillreqd);  // otherwise do normal int/float conversion, with failover to other types
 // Select the precision to use: the smallest that can hold the data, but never less than the precision of x
 C cvtmask=(~AT(a)&B01)<<1;  // if x is not B01, set mask to suppress conversion to B01
 cvtmask=AT(a)&B01+INT?cvtmask:6;  // if not B01 or INT, suppress conversion to INT (but it may be INT already)
 cvtmask=AT(a)&B01+INT+FL?cvtmask:14;  // if not B01/INT/FL, suppress conversion to FL
 return bcvt(cvtmask,z);
}
