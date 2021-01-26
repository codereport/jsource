/* Copyright 1990-2008, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Words: Word Formation                                                   */

#include "j.h"
#include "w.h"

#define SS              0    /* space                           */
#define SS9             1    // space, previous field was numeric
#define SX              2    /* other                           */
#define SA              3    /* alphanumeric                    */
#define SN              4    /* N                               */
#define SNB             5    /* NB                              */
#define SQQ             6    /* even quotes                     */
#define S9              7    /* numeric                         */
#define S99             8    // numeric, previous field was numeric i. e. followon numeric
#define SQ              9    /* quote                           */
#define SNZ             10    // NB.
#define SZ              11    // trailing comment
#define SU              12   // prev char was uninflectable (i. e. LF).
#define SDD             13   // { seen
#define SDDZ            14   // } seen
#define SDDD            15   // doubled {} seen

#define E0              0  // no action
#define EI              1    // end of previous word - emit
#define EN              1    // start of next word - save position
#define EZ              2   // end and start together - +$ eg

#define UNDD            4  // inflection found on {{ }} - abort the DD state, revert to individual primitives

typedef C ST;
#define SE(s,e) (((s)<<4)|((((s)==S99))<<3)|(e))  // set bit 3 inside followon numeric

static const ST state[SDDD+1][16]={
/*SS */ {[CX]=SE(SX,EN),[CDD]=SE(SDD,EN),[CDDZ]=SE(SDDZ,EN),[CU]=SE(SU,EN),[CS]=SE(SS,E0),[CA]=SE(SA,EN),[CN]=SE(SN,EN),[CB]=SE(SA,EN),[C9]=SE(S9,EN),[CD]=SE(SX,EN),[CC]=SE(SX,EN),[CQ]=SE(SQ,EN)},
/*SS9*/ {[CX]=SE(SX,EN),[CDD]=SE(SDD,EN),[CDDZ]=SE(SDDZ,EN),[CU]=SE(SU,EN),[CS]=SE(SS9,E0),[CA]=SE(SA,EN),[CN]=SE(SN,EN),[CB]=SE(SA,EN),[C9]=SE(S99,EN),[CD]=SE(SX,EN),[CC]=SE(SX,EN),[CQ]=SE(SQ,EN)},
/*SX */ {[CX]=SE(SX,EZ),[CDD]=SE(SDD,EZ),[CDDZ]=SE(SDDZ,EZ),[CU]=SE(SU,EZ),[CS]=SE(SS,EI),[CA]=SE(SA,EZ),[CN]=SE(SN,EZ),[CB]=SE(SA,EZ),[C9]=SE(S9,EZ),[CD]=SE(SX,E0),[CC]=SE(SX,E0),[CQ]=SE(SQ,EZ)},
/*SA */ {[CX]=SE(SX,EZ),[CDD]=SE(SDD,EZ),[CDDZ]=SE(SDDZ,EZ),[CU]=SE(SU,EZ),[CS]=SE(SS,EI),[CA]=SE(SA,E0),[CN]=SE(SA,E0),[CB]=SE(SA,E0),[C9]=SE(SA,E0),[CD]=SE(SX,E0),[CC]=SE(SX,E0),[CQ]=SE(SQ,EZ)},
/*SN */ {[CX]=SE(SX,EZ),[CDD]=SE(SDD,EZ),[CDDZ]=SE(SDDZ,EZ),[CU]=SE(SU,EZ),[CS]=SE(SS,EI),[CA]=SE(SA,E0),[CN]=SE(SA,E0),[CB]=SE(SNB,E0),[C9]=SE(SA,E0),[CD]=SE(SX,E0),[CC]=SE(SX,E0),[CQ]=SE(SQ,EZ)},
/*SNB*/ {[CX]=SE(SX,EZ),[CDD]=SE(SDD,EZ),[CDDZ]=SE(SDDZ,EZ),[CU]=SE(SU,EZ),[CS]=SE(SS,EI),[CA]=SE(SA,E0),[CN]=SE(SA,E0),[CB]=SE(SA,E0),[C9]=SE(SA,E0),[CD]=SE(SNZ,E0),[CC]=SE(SX,E0),[CQ]=SE(SQ,EZ)},
/*SQQ*/ {[CX]=SE(SX,EZ),[CDD]=SE(SDD,EZ),[CDDZ]=SE(SDDZ,EZ),[CU]=SE(SU,EZ),[CS]=SE(SS,EI),[CA]=SE(SA,EZ),[CN]=SE(SN,EZ),[CB]=SE(SA,EZ),[C9]=SE(S9,EZ),[CD]=SE(SX,EZ),[CC]=SE(SX,EZ),[CQ]=SE(SQ,E0)},
/*S9 */ {[CX]=SE(SX,EZ),[CDD]=SE(SDD,EZ),[CDDZ]=SE(SDDZ,EZ),[CU]=SE(SU,EZ),[CS]=SE(SS9,EI),[CA]=SE(S9,E0),[CN]=SE(S9,E0),[CB]=SE(S9,E0),[C9]=SE(S9,E0),[CD]=SE(S9,E0),[CC]=SE(SX,E0),[CQ]=SE(SQ,EZ)},
/*S99*/ {[CX]=SE(SX,EZ),[CDD]=SE(SDD,EZ),[CDDZ]=SE(SDDZ,EZ),[CU]=SE(SU,EZ),[CS]=SE(SS9,EI),[CA]=SE(S99,E0),[CN]=SE(S99,E0),[CB]=SE(S99,E0),[C9]=SE(S99,E0),[CD]=SE(S99,E0),[CC]=SE(SX,E0),[CQ]=SE(SQ,EZ)},
/*SQ */ {[CX]=SE(SQ,E0),[CDD]=SE(SQ,E0),[CDDZ]=SE(SQ,E0),[CU]=SE(SQ,E0),[CS]=SE(SQ,E0),[CA]=SE(SQ,E0),[CN]=SE(SQ,E0),[CB]=SE(SQ,E0),[C9]=SE(SQ,E0),[CD]=SE(SQ,E0),[CC]=SE(SQ,E0),[CQ]=SE(SQQ,E0)},
/*SNZ*/ {[CX]=SE(SZ,E0),[CDD]=SE(SZ,E0),[CDDZ]=SE(SZ,E0),[CU]=SE(SU,EZ),[CS]=SE(SZ,E0),[CA]=SE(SZ,E0),[CN]=SE(SZ,E0),[CB]=SE(SZ,E0),[C9]=SE(SZ,E0),[CD]=SE(SX,E0),[CC]=SE(SX,E0),[CQ]=SE(SZ,E0)},
/*SZ */ {[CX]=SE(SZ,E0),[CDD]=SE(SZ,E0),[CDDZ]=SE(SZ,E0),[CU]=SE(SU,EZ),[CS]=SE(SZ,E0),[CA]=SE(SZ,E0),[CN]=SE(SZ,E0),[CB]=SE(SZ,E0),[C9]=SE(SZ,E0),[CD]=SE(SZ,E0),[CC]=SE(SZ,E0),[CQ]=SE(SZ,E0)},
/*SU */ {[CX]=SE(SX,EZ),[CDD]=SE(SDD,EZ),[CDDZ]=SE(SDDZ,EZ),[CU]=SE(SU,EZ),[CS]=SE(SS,EI),[CA]=SE(SA,EZ),[CN]=SE(SN,EZ),[CB]=SE(SA,EZ),[C9]=SE(S9,EZ),[CD]=SE(SX,EZ),[CC]=SE(SX,EZ),[CQ]=SE(SQ,EZ)},
/*SDD*/ {[CX]=SE(SX,EZ),[CDD]=SE(SDDD,E0),[CDDZ]=SE(SX,EZ),[CU]=SE(SU,EZ),[CS]=SE(SS,EI),[CA]=SE(SA,EZ),[CN]=SE(SN,EZ),[CB]=SE(SA,EZ),[C9]=SE(S9,EZ),[CD]=SE(SX,E0),[CC]=SE(SX,E0),[CQ]=SE(SQ,EZ)},
/*SDDZ*/ {[CX]=SE(SX,EZ),[CDD]=SE(SX,EZ),[CDDZ]=SE(SDDD,E0),[CU]=SE(SU,EZ),[CS]=SE(SS,EI),[CA]=SE(SA,EZ),[CN]=SE(SN,EZ),[CB]=SE(SA,EZ),[C9]=SE(S9,EZ),[CD]=SE(SX,E0),[CC]=SE(SX,E0),[CQ]=SE(SQ,EZ)},
/*SDDD*/ {[CX]=SE(SX,EZ),[CDD]=SE(SDD,EZ),[CDDZ]=SE(SDDZ,EZ),[CU]=SE(SU,EZ),[CS]=SE(SS,EI),[CA]=SE(SA,EZ),[CN]=SE(SN,EZ),[CB]=SE(SA,EZ),[C9]=SE(S9,EZ),[CD]=SE(SX,E0)+UNDD,[CC]=SE(SX,E0)+UNDD,[CQ]=SE(SQ,EZ)},
};

// w points to a string A-block
// result is word index & length; z is (i0,end0+1),(i1,end1+1),...
// AM(z) gives # of words not counting a final comment, ({.$z) - hascomment
// If there are mismatched quotes, AM(z) is set to -1 and the number of valid tokens is in AS(z)[0]
F1(jtwordil){A z;I s,i,m,n,nv,*x;UC*v;
 ARGCHK1(w);  // if no string, could be empty line from keyboard; return null A in that case
 nv=0;    // set not creating numeric constant
 n=AN(w); v=UAV(w); GATV0(z,INT,n+n,3); x=AV(z); AS(z)[1]=2; AS(z)[2]=1;  // get count of characters n and address v; turn into suitable shape for ;.0 (x 2 1)
  // allocate absolute worst-case output area (each char is 1 word); point x to output indexes
 s=SE(SS,0);
 for(i=0;i<n;++i){   // run the state machine
  I prevs=s;  // state before new character
  I currc=ctype[v[i]];  // get class of input character
if(!jt->directdef&&(currc==CDD||currc==CDDZ))currc=CX;  // scaf  if direct def disabled, treat { } as CX
  C *statebase=(C*)state+currc; s=statebase[s&0xf0];  // top 4 bits of s are state line; currc indexes the char code.  This fetches new state
  // Handle followon numerics.  If the previous state was 'followon numeric' and the new character is CX/CS/CQ, we will emit after this state but
  // we need to overwrite the previous numeric.  Decrement the pointer by 2 before writing.  This runs while the state-fetch is happening and is fast enough to allow
  // the store addresses to be calculated before the next fetch
  prevs&=8; prevs&=((8LL<<CX)|(8LL<<CS)|(8LL<<CQ)|(8LL<<CDD)|(8LL<<CDDZ)|(8LL<<CU))>>currc; x=(I*)((I)x-(prevs<<((LGSZI+1)-3)));  // if followon numeric and CX/CS/CQ/CDD/CDDZ, subtract from x, to move x back 2 positions if coming out of followon numeric with a number
  // handle {{. etc.  It looked like a DD but turns out to be inflected.  We have outed the start for the first character;
  // we also outed the second character but did not accept the word.  We accept it now by advancing x over it
  // Use an IF because this is rare and we don't want it in the normal path
  if(unlikely(s&UNDD))x+=2;
  // do two stores, and advance over any that are to be emitted.  0, 1, or 2 may be emitted (0 when the state has no action, 1 when the
  // state is an end+1 or start value, and 2 if an end+1 AND start value)
  x[0]=i; x[1]=i; x+=s&3;
 }
 // force an EI at the end, as if with a space.  We will owe one increment of x, repaid when we calculate m.
 //  If the line ends without a token being open (spaces perhaps) this will be half of a field and will be shifted away
 x=(I*)((I)x-(((s<<1)&16)>>(3-LGSZI)));    // same as above, with CS as the character
 *x=i;
 m=((x-AV(z))+1)>>1; AS(z)[0]=m; AM(z)=m-((((1LL<<SNZ)|(1LL<<SZ))>>(s>>4))&1); // Calculate & install count; if last field is NB., subtract 1 from word count
 if(unlikely((s>>4)==SQ))AM(z)=-1;  // error if open quote, indicated by nag AM
 R z;
}    // word index & end+1; z is m 2 1$(i0,e0),(i1,e1),... AM(z) is # words not including any final NB, or -1 if quote error

// Turn word list back into string
// wil is the word list from wordil (we ignore AM and AN), w is the original character list
// result is a single string containing all the words with no separators
// The result is always writable
A jtunwordil(J jt, A wil, A w, I opts){A z;
 I n=AS(wil)[0]; I (* RESTRICT wilv)[2]=voidAV(wil); C * RESTRICT wv=CAV(w);  // n=#words; wilv->start,end pairs; wv->chars
 I cc=0; DO(n, cc-=wilv[i][0]; cc+=wilv[i][1];)  // cc=# characters in all words
 I buflen=(cc<<(opts&1))+((opts&1)<<1)+((opts&2)<<1);  // len to get: one byte per word for delimiter, plus # chars (doubled if we are doubling quotes), plus 2 if enclosing in quotes, plus 4 if requested
 GATV0(z,LIT,buflen,1); C *zv0=CAV(z), *zv=zv0;  // allocate the buffer, point to beginning, set scan pointer
 UI dupchar=(opts&1)?'\'':256; *zv='\''; zv+=(opts&1);  // if dup requested, set to look for quotes and install leading quote
 DO(n, C *w0=wv+wilv[i][0]; C *wend=wv+wilv[i][1]; while(w0!=wend){C c=*w0++; *zv++=c; if(c==dupchar)*zv++=c;})  // copy all words, with 1 space between, duplicating where needed
 AS(z)[0]=AN(z)=zv-zv0;  // install length of result
 RETF(z);
}

// ;: y
DF1(jtwords){A t,*x,z;C*s;I k,n,*y;
 F1RANK(1,jtwords,self);
 RZ(w=vs(w));  // convert w to LIT if it's not already
 RZ(t=wordil(w)); ASSERT(AM(t)>=0,EVOPENQ)
 R jtboxcut0(jt,t,w,self);  // result of wordil has shape suitable for <;.0, so we use that
}


static A jtconstr(J jt,I n,C*s){A z;C b,c,p,*t,*x;I m=0;
 p=0; t=s; DQ(n-2, c=*++t; b=c==CQUOTE; m+=(b^1)|p; p=((b^1)|p)^1;);  // should just take half the # '
 if(0==m)R aqq; 
 GATV0(z,LIT,m,1!=m); x=CAV(z);
 p=0; t=s; DQ(n-2, *x=c=*++t; b=c==CQUOTE; x+=(b^1)|p; p=((b^1)|p)^1;);  // This may overstore by 1 character but that's OK because LIT types have allocated space at the end
 R z;
}

#define TNAME(i)    (NAME&AT(v[i]))
#define TASGN(i)    (ASGN&AT(v[i]))
//#define TVERB(i,c)  (y=v[i], c      ==ID(y))  // true if this word is the given verb
#define TVERB(i,c)  (v[i]==ds(c))  // true if this word is the given verb
#define TRBRACE(i)  TVERB(i,CRBRACE)  // true if given word is }
#define TAIA(i,j)   (TASGN(1) && TNAME(j) && AN(v[i])==AN(v[j]) && TNAME(i) && \
                        !memcmpne(NAV(v[i])->s,NAV(v[j])->s,AN(v[i])))
  // true if the two words are names, word 1 is assignment, and the names are equal

// Convert text sentence to a sequence of words to be the queue for parsing
// a MUST BE the result of wordil, which is an integer list of word index & end+1: (i0,e0),(i1,e1)...  # words not including final NB. is in AM(a)
// w holds the string text of the sentence
// env is the environment for which this is being parsed: 0=tacit translator, 1=keyboard/immex with no locals, 2=for explicit defn
// result is a list of parsable words, with types right.  The result is input only to parsing, never to verbs, and thus may be nonrecursive
A jtenqueue(J jt,A a,A w,I env){A*v,*x,y,z;B b;C d,e,p,*s,*wi;I i,n,*u,wl;UC c;
 ARGCHK2(a,w);
 s=CAV(w); u=AV(a);
 n=AM(a);  // get # words not including any final NB.
 GATV0(z,BOX,n,1); x=v=AAV(z);   //  allocate list of words; set running word pointer x, and static
   // beginning-of-list pointer v, to start of list of output pointers
 for(i=0;i<n;i++,x++){  // for each word
  wl=u[1]-u[0]; wi=u[0]+s; c=e=*wi; p=ctype[(UC)c]; b=0;   // wi=first char, wl=length, c=e=first char, p=type of first char, b='no inflections'
  if(wl==1||(b=((p!=C9)&(wi[wl-1]==CESC1))|(wi[wl-1]==CESC2)))e=spellin(wl,wi);else e=0;  // if word has 1 character, or is a--. or ---:, convert to pseudocharacter
  if(BETWEENO(c,32,128)&&AK(y=ds(e))){
   // If first char is ASCII, see if the form including inflections is a primitive;
   // if so, that is the word to put into the queue.  No need to copy it
   // Since the address of the shared primitive block is used, we can use that to compare against to identify the primitive later
   // We keep track of We keep track of whether }~ was found.  If } starts the sentence, this will compare garbage, but
   // without risk of program check
   if((-env & SGNIF(AT(y),ASGNX))<0) {
    // If the word is an assignment, use the appropriate assignment block, depending on the previous word and the environment
    // In environment 0 (tacit translator), leave as simple assignment
    if(e==CASGN && (env==1 || (i && AT(x[-1])&NAME && (NAV(x[-1])->flag&(NMLOC|NMILOC))))){y=asgnforceglo;}   // sentence is NOT for explicit definition, or preceding word is a locative.  Convert to a global assignment.  This will make the display show =:
    if(i&& AT(x[-1])&NAME){y= y==asgnforceglo?asgnforcegloname:y==ds(CGASGN)?asgngloname:asgnlocsimp;}  // if ASGN preceded by NAME, flag it thus, by switching to the block with the ASGNTONAME flag set
   }
   if(AT(y)&NAME&&(NAV(y)->flag&NMDOT)){RZ(y=ca(y)); if((env==2)&&(NAV(*x)->flag&NMXY)){AT(*x)|=NAMEBYVALUE;}}  // The inflected names are the old-fashioned x. y. etc.  They must be cloned lest we modify the shared copy
   *x=y;   // install the value
  } else if(e==CFCONS){RZ(*x=FCONS(connum(wl-1,wi)))  // if the inflected form says [_]0-9:, create word for that
  } else {
   p=b?b:p;    // otherwise, it's not a primitive, but data, either numeric, string, or name.  Check first character, but fail if inflected form, which must be invalid.  p must be non0
    // If the name is a call-by-value name (x y u. etc), we mark it as BYVALUE if it is slated for execution in an explicit definition
   if((p&~CA)==0){ASSERTN(vnm(wl,wi),EVILNAME,nfs(wl,wi)); RZ(*x=nfs(wl,wi)); if((env==2)&&(NAV(*x)->flag&NMXY)){AT(*x)|=NAMEBYVALUE;}  // starts with alphabetic, make it a name, error if invalid name
   }else if(p==C9){if(unlikely(!(*x=connum(wl,wi)))){I lje=jt->jerr; RESETERR; jsignal3(lje,w,u[0]); R 0;}   // starts with numeric, create numeric constant.. If error, give a message showing the bad number
   }else if(p==CQ){ RZ(*x=constr(wl,wi));   // start with ', make string constant
   }else{jsignal3(EVSPELL,w,wi-s); R 0;}   // bad first character or inflection
  }
  // Since the word is being incorporated into a list, we must realize it
  rifv(*x);
  // Mark the word as not-inplaceable.  Since all allocations start life marked in-placeable, we get into trouble if words in
  // a explicit definition are left that way, because the sentences may get reexecuted and any constant marked inplaceable could
  // be modified by each use.  The trouble happens only if the definition is not assigned (if it's assigned all the usecounts
  // are incremented).  But just do it for all words in sentences
  ACIPNO(*x);  // mark the stored word not inplaceable
  u+=2;   // advance to next word
 }

 // We have created the queue.  Here we check for special in-place forms.  If we find one, we
 // replace the verb/adverb with a version that performs in-place operation.

 // The game here is to make tests that fail as quickly as possible, so that this overhead
 // falls lightly on most sentences
 if (6 <= n) {
  if(TRBRACE(3) && TVERB(n-2,CLAMIN) && TNAME(0) && TASGN(1) && TNAME(2)){A p,*yv,z1;I c,j,k,m;  // if we match }~, can't match this
   // abc=: pqr}x,...y,:z  (must be odd # words, but that's not worth checking)
   // Verify the form is present: alternating names and commas
   b=1; m=(n-3)>>1;   // number of nouns x..z
   j=4;      DQ(m,   if(!TNAME(j)       ){b=0; break;} j+=2;);  // Check all names
   j=5; if(b)DQ(m-2, if(!TVERB(j,CCOMMA)){b=0; break;} j+=2;);  // Check inbetween, except after last 2 names 
   if(b){  // if the form is right...
    // Synthesize a new argument to replace the list of arrays, and a new sentence to execute on this argument
    // The sentence is abc =: CCASEV (new argument)
    // The new argument is (the list of names), then
    // (_3) the name pqr, (_2) the position if any of abc in the right-hand list (-1 if absent), (_1) the parsed queue before this replacement
    GAT0(z1,BOX,4,1); x=AAV(z1);   // Allocate the sentence, point to its data
    GATV0(y,BOX,m+3,1); yv=AAV(y);   // Allocate the argument
    c=-1; k=AN(v[0]); s=NAV(v[0])->s;   // get length and address of abc
    j=4; DO(m, yv[i]=p=v[j]; j+=2; if(AN(p)==k&&!memcmpne(s,NAV(p)->s,k))c=i;);  // move name into argument, remember if matched abc
    yv[m]=v[2]; RZ(yv[m+1]=rifvs(sc(c))); yv[m+2]=z;    // add the 3 ending elements
    x[0]=v[0]; x[1]=v[1]; x[2]=ds(CCASEV); x[3]=y;  // build the sentence
    RETF(z1);  // that's what we'll execute
   }
  }
 }
 RETF(z);
}    /* produce boxed list of words suitable for parsing */
                                                            
/* locals in enqueue:                                           */
/* b:  1 iff current word is a primitive spelled with . or :    */
/* c:  first character in current word                          */
/* d:  last  character in current word                          */
/* e:  first character in current word, after spellin           */
/* i:  index of current word                                    */
/* n:  number of words                                          */
/* p:  character type of current character                      */
/* s:  ptr to value part of input string w                      */
/* u:  ptr to value part of word index & length info a          */
/* v:  ptr to value part of z                                   */
/* wi: index of current word in input string                    */
/* wl: length of current word                                   */
/* x:  ptr to result word being built                           */
/* y:  array temp                                               */
/* z:  result array of boxed list of words                      */

// env is the environment: 0=tacit translator, 1=keyboard/immex with no local symbol, 2=explicit definition running
// w is either a string block or a string block that has been processed into words in wordil format, with AM set
A jttokens(J jt,A w,I env){A t; RZ(t=wordil(w)); ASSERT(AM(t)>=0,EVOPENQ) R enqueue(t,w,env);}
// enqueue produces nonrecursive result, and so does tokens.  This is OK because the result is always parsed and is never an argument to a verb


#define CHKJ(j)             ASSERT(0<=(j),EVINDEX);
#define EXTZ(T,p)           while(uu<p+u){k=u-(T*)AV(z); RZ(z=ext(0,z)); u=k+(T*)AV(z); uu=(T*)AV(z)+AN(z);}

#define EMIT0c(T,j,i,r,c)   {CHKJ(j); p=(i)-(j); EXTZ(T,1); RZ(*u++=rifvsdebug(str(p,(j)+wv)));}
#define EMIT0b(T,j,i,r,c)   {CHKJ(j); p=(i)-(j); EXTZ(T,1); RZ(*u++=rifvsdebug(vec(B01,p,(j)+wv)));}
#define EMIT0x(T,j,i,r,c)   {CHKJ(j); p=(i)-(j); EXTZ(T,1); GA(x,t0,p*wm,wr,AS(w0));  \
                                AS(x)[0]=p; MC(AV(x),wv0+wk*(j),wk*p); *u++=x;}
#define EMIT1(T,j,i,r,c)    {CHKJ(j); p=(i)-(j);            cc=(j)+wv; DQ(p, *u++=*cc++;);}
#define EMIT1x(T,j,i,r,c)   {CHKJ(j); p=wk*((i)-(j));       MC(u,wv0+j*wk,p); u+=p;}
#define EMIT2(T,j,i,r,c)    {CHKJ(j); p=(i)-(j); EXTZ(T,2); *u++=(j); *u++=p;}
#define EMIT3(T,j,i,r,c)    {CHKJ(j);            EXTZ(T,1);                   *u++=(c)+q*(r);}
#define EMIT4(T,j,i,r,c)    {CHKJ(j); p=(i)-(j); EXTZ(T,3); *u++=(j); *u++=p; *u++=(c)+q*(r);}
#define EMIT5(T,j,x,r,c)    {if(0>(j))i=n;}

#define DO_ONE(T,EMIT) \
 switch(e=v[1]){                                                          \
  case 6:         i=n; break;                                             \
  case 2: case 3: if(0<=vi){EMIT(T,vj,vi,vr,vc); vi=vr=-1;} EMIT(T,j,i,r,c);       j=2==e?i:-1; break;  \
  case 4: case 5: if(r!=vr){if(0<=vi)EMIT(T,vj,vi,vr,vc); vj=j; vr=r; vc=c;} vi=i; j=4==e?i:-1; break;  \
  case 1:         j=i; break;                                                    \
  case 7: i-=2; i=i<0?0:i; break; \
 }

#define ZVAx                {}
#define ZVA5                {*zv++=i; *zv++=j; *zv++=r; *zv++=c; *zv++=v[0]; *zv++=v[1];}

#define FSMF(T,zk,zt,zr,zm,cexp,EMIT,ZVA)    \
 {T*u,*uu;                                                                  \
  RZ(z=exta((zt),(zr),(zm),(f|4)==5?n+1:n/3));                              \
  if(1<(zr)){I*s=AS(z); s[1]=(zm); if(1==f&&2<wr)MCISH(1+s,1+AS(w0),wr-1);}  \
  zv=AV(z); u=(T*)zv; uu=u+AN(z);                                           \
  for(;i<n;++i,r=*v){c=(cexp); v=sv+2*(c+r*q); ZVA; DO_ONE(T,EMIT);}        \
  if(6!=e){                                                                 \
   if(0<=d)         {c=d;      v=sv+2*(c+r*q); ZVA; DO_ONE(T,EMIT);}        \
   else{                                                                    \
    if(0<=vi      )EMIT(T,vj,r==vr?n:vi,vr,vc);                             \
    if((-(r!=vr)&~j)<0)EMIT(T,j,n,r,c);    /*  r!=vr and j>=0 */                                     \
  }}                                                                        \
  if(5==f)u=(T*)zv;                                                         \
  i=AN(z); AN(z)=j=(u-(T*)AV(z))/zk; AS(z)[0]=j/(zm); if(i>3*j)RZ(z=ca(z));        \
 }

static A jtfsmdo(J jt,I f,A s,A m,I*ijrd,A w,A w0){A x,z;C*cc,*wv0;
     I c,d,e,i,j,k,*mv,n,p,q,r,*sv,t,t0,*v,vc,vi,vj,vr,wk,wm,wr,*zv;
 SETIC(w,n); t=AT(w);
 q=*(1+AS(s));
 sv=AV(s); mv=AV(m);
 i=ijrd[0]; j=ijrd[1]; r=ijrd[2]; d=ijrd[3]; vi=vj=vr=vc=-1;
 if(t&INT){t0=AT(w0); wr=AR(w0); PROD(wm,AR(w0)-1,AS(w0)+1) wk=wm<<bplg(AT(w0)); wv0=CAV(w0);}
 switch(f+(t&(B01+LIT))*6){
  case 0+0: {I *wv= AV(w); FSMF(A,1,BOX,1, 1,   wv[i] ,EMIT0x,ZVAx);} break; // other
  case 0+1: {I *wv= AV(w); FSMF(C,bp(AT(w0)),t0, wr,wm,  wv[i] ,EMIT1x,ZVAx);} break;
  case 0+2: {I *wv= AV(w); FSMF(I,1,INT,2, 2,   wv[i] ,EMIT2, ZVAx);} break;
  case 0+3: {I *wv= AV(w); FSMF(I,1,INT,1, 1,   wv[i] ,EMIT3, ZVAx);} break;
  case 0+4: {I *wv= AV(w); FSMF(I,1,INT,2, 3,   wv[i] ,EMIT4, ZVAx);} break;
  case 0+5: {I *wv= AV(w); FSMF(I,1,INT,2, 6,   wv[i] ,EMIT5, ZVA5);} break;

  case 6+0: {B *wv=BAV(w); FSMF(A,1,BOX,1, 1,   wv[i] ,EMIT0b,ZVAx);} break;  // B01
  case 6+1: {B *wv=UAV(w); FSMF(B,1,B01,1, 1,   wv[i] ,EMIT1, ZVAx);} break;
  case 6+2: {B *wv=BAV(w); FSMF(I,1,INT,2, 2,   wv[i] ,EMIT2, ZVAx);} break;
  case 6+3: {B *wv=BAV(w); FSMF(I,1,INT,1, 1,   wv[i] ,EMIT3, ZVAx);} break;
  case 6+4: {B *wv=BAV(w); FSMF(I,1,INT,2, 3,   wv[i] ,EMIT4, ZVAx);} break;
  case 6+5: {B *wv=BAV(w); FSMF(I,1,INT,2, 6,   wv[i] ,EMIT5, ZVA5);} break;

  case 12+0: {UC*wv=UAV(w); FSMF(A,1,BOX,1, 1,mv[wv[i]],EMIT0c,ZVAx);} break;  // LIT
  case 12+1: {UC*wv=UAV(w); FSMF(C,1,LIT,1, 1,mv[wv[i]],EMIT1, ZVAx);} break;
  case 12+2: {UC*wv=UAV(w); FSMF(I,1,INT,2, 2,mv[wv[i]],EMIT2, ZVAx);} break;
  case 12+3: {UC*wv=UAV(w); FSMF(I,1,INT,1, 1,mv[wv[i]],EMIT3, ZVAx);} break;
  case 12+4: {UC*wv=UAV(w); FSMF(I,1,INT,2, 3,mv[wv[i]],EMIT4, ZVAx);} break;
  case 12+5: {UC*wv=UAV(w); FSMF(I,1,INT,2, 6,mv[wv[i]],EMIT5, ZVA5);} break;
 }
 R z;
}

F1(jtfsmvfya){PROLOG(0099);A a,*av,m,s,x,z,*zv;I an,c,e,f,ijrd[4],k,p,q,*sv,*v;
 RZ(a=w);
 ASSERT(1==AR(a),EVRANK);
 ASSERT(BOX&AT(a),EVDOMAIN);
 an=AN(a); av=AAV(a); 
 ASSERT(BETWEENC(an,2,4),EVLENGTH);
 RE(f=i0(av[0]));
 ASSERT((UI)f<=(UI)5,EVINDEX);
 RZ(s=vi(av[1])); sv=AV(s);
 ASSERT(3==AR(s),EVRANK);
 v=AS(s); p=v[0]; q=v[1]; ASSERT(2==v[2],EVLENGTH);
 v=sv; DQ(p*q, k=*v++; e=*v++; ASSERT((UI)k<(UI)p&&(UI)e<=(UI)7,EVINDEX););
 ijrd[0]=0; ijrd[1]=-1; ijrd[2]=0; ijrd[3]=-1;
 if(4==an){I d,i,j,n,r;
  RZ(x=vi(av[3])); n=AN(x); v=AV(x);
  ASSERT(1==AR(x),EVRANK);
  ASSERT(4>=n,EVLENGTH);
  if(1<=n) ijrd[0]=i=*v++;
  if(2<=n){ijrd[1]=j=*v++; ASSERT(BETWEENO(j, -1, i),EVINDEX);}
  if(3<=n){ijrd[2]=r=*v++; ASSERT((UI)r<(UI)p,EVINDEX);}
  if(4==n){ijrd[3]=d=*v++; ASSERT(BETWEENO(d, -1, q),EVINDEX);}
 }
 m=2==an?mtv:av[2]; c=AN(m);
 ASSERT(1>=AR(m),EVRANK);
 if(!c&&1==AR(m)){   /* m is empty; w must be integer vector */  }
 else if(NUMERIC&AT(m)){
  ASSERT(c==AN(ds(CALP)),EVLENGTH);
  RZ(m=vi(m)); v=AV(m); DO(c, k=v[i]; ASSERT((UI)k<(UI)q,EVINDEX););
 }else ASSERT(BOX&AT(m),EVDOMAIN);
 GAT0(z,BOX,4,1); zv=AAV(z);
 RZ(zv[0]=rifvs(sc(f))); RZ(zv[1]=rifvs(s)); RZ(zv[2]=rifvs(m)); RZ(zv[3]=rifvs(vec(INT,4L,ijrd)));
 EPILOG(z);
}    /* check left argument of x;:y */

static A jtfsm0(J jt,A a,A w,C chka){PROLOG(0100);A*av,m,s,x,w0=w;B b;I c,f,*ijrd,k,n,p,q,*v;
 ARGCHK2(a,w);
 if(chka)RZ(a=fsmvfya(a)); 
 av=AAV(a); 
 f=i0(av[0]); s=av[1]; m=av[2]; ijrd=AV(av[3]);
 n=AN(w); v=AS(s); p=v[0]; q=v[1];
 ASSERT((UI)ijrd[0]<(UI)n,EVINDEX);
 b=1>=AR(w)&&(!n||LIT&AT(w)); c=AN(m);  // b=w is atom/list, either literal or empty; c is # columns mapped to input through m
 if(((c-1)&((AR(m)^1)-1))<0){  // m is omitted or empty, use column numbers in y; audit them first   m is empty list
  ASSERT(1>=AR(w),EVRANK);
  if(!(B01&AT(w))){RZ(w=w0=vi(w)); v=AV(w); DO(n, k=v[i]; ASSERT((UI)k<(UI)q,EVINDEX););}
 }else if(NUMERIC&AT(m)){  // m is numeric list
  ASSERT(b!=0,EVDOMAIN);   // w must be ASCII
 }else{A*mv,t,y;I j,r;
  ASSERT(BOX&AT(m),EVDOMAIN);  // otherwise m must be boxes
  RZ(y=raze(m)); r=AR(y); k=AS(y)[0];  // y = all the input values run together, k=# input values
  ASSERT((UI)(r-AR(w))<=(UI)1,EVRANK);  // items of m must match rank of w, or the entire w (which will be treated as a single input)
  GATV0(x,INT,1+k,1); v=AV(x); v[k]=c; mv=AAV(m);  // x will hold translated column numbers.  Install 'not found' value at the end
  DO(c, j=i; t=mv[i]; if((-r&((r^AR(t))-1))<0)DQ(AS(t)[0], *v++=j;) else *v++=j;);  // go through m; for each box, install index for that box for each item in that box.
  if(b){RZ(m=from(indexof(y,ds(CALP)),x)); v=AV(m); DO(AN(ds(CALP)), k=v[i]; ASSERT((UI)k<(UI)q,EVINDEX););}  // for ASCII input, translate & check size
  else {ASSERT(q>c,EVINDEX); RZ(w=from(indexof(y,w),x));}  // # columns of machine must be at least c+1; look up the rest
 }
 A z=fsmdo(f,s,m,ijrd,w,w0);
 EPILOG(z);
}

F2(jtfsm){R fsm0(a,w,1);}
     /* x;:y */

DF1(jtfsmfx){ARGCHK2(w,self); R fsm0(FAV(self)->fgh[0],w,0);}
     /* x&;: y */
