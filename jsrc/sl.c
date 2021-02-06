/* Copyright 1990-2007, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Symbol Table: Locales                                                   */

#include "j.h"

// Interfaces for numbered locales
// Hashed version, without locale reuse
#define HASHSLOT(x,tsize) (((UI)((UI4)(x)*(UI4)2654435761U)*(UI)(tsize))>>32)
// Initialize the numbered-locale system.  Called during initialization, so no need for ras()
static A jtinitnl(J jt){A q;
 I s; FULLHASHSIZE(5*1,INTSIZE,0,0,s);  // at least 5 slots, so we always have at least 2 empties
 GATV0(q,INT,s,0); memset(IAV(q),0,s*SZI);  // allocate hashtable and clear to 0
 jt->stnum=q;  // save address of block
 AK(jt->stnum)=0;  // set next number to allocate
 AM(jt->stnum)=0;  // set number in use
 return q;  // return no error
}

// Get the locale number to use for the next numbered locale.  (0 if error, with error code set)
// This call just gets the number to use, it does not allocate the number.  The locale will be installed later, unless an error intervenes
static I jtgetnl(J jt){
 // If the table is too close to full, reallocate it & copy.
 // We let the table get up to half full, figuring that on average it will be somewhat less
 if(2*AM(jt->stnum)>AN(jt->stnum)){
  // Allocate a new block.  Don't use ext because we have to keep the old one for rehashing
  I s; FULLHASHSIZE(2*AM(jt->stnum)+2,INTSIZE,0,0,s);
  A new; GATV0(new,INT,s,0); memset(IAV(new),0,s*SZI);  // allocate hashtable and clear to 0
  // rehash the old table into the new
  I i; for(i=0;i<AN(jt->stnum);++i){
   A st; if(st=(A)IAV0(jt->stnum)[i]){  // if there is a value hashed...
    I probe=HASHSLOT(NAV(LOCNAME(st))->bucketx,s);  // start of search.  Look backward, wrapping around, until we find an empty.  We never have duplicates
    while(IAV0(new)[probe]){if(--probe<0)probe=AN(new)-1;}  // find empty slot
    IAV0(new)[probe]=(I)st;  // install in new hashtable
   }
  }
  AK(new)=AK(jt->stnum); AM(new)=AM(jt->stnum);  // before freeing the block, copy # locales and next locales#
  fa(jt->stnum); ras(new); jt->stnum=new;  // install the new table, release the old
 }
 return AK(jt->stnum);  // return index of next allocation
}

// Install locale l in the numbered-locale table, at the number returned by the previous jtgetnl.  No error is possible
static void jtinstallnl(J jt, A l){
 ras(l);  // protect new value in table
 I probe=HASHSLOT(AK(jt->stnum),AN(jt->stnum));  // start of search.  Look backward, wrapping around, until we find an empty.  We never have duplicates
 while(IAV0(jt->stnum)[probe]){if(--probe<0)probe=AN(jt->stnum)-1;}  // find empty slot
 IAV0(jt->stnum)[probe]=(I)l;  // put new locale in the empty slot
 ++AK(jt->stnum);  // increment next-locale ticket
 ++AM(jt->stnum);  // increment number of locales outstanding
}

// return the address of the locale block for number n, or 0 if not found
A jtfindnl(J jt, I n){
 I probe=HASHSLOT(n,AN(jt->stnum));  // start of search.  Look backward, wrapping around, until we find match or an empty.
 while(IAV0(jt->stnum)[probe]){if(NAV(LOCNAME((A)IAV0(jt->stnum)[probe]))->bucketx==n)return (A)IAV0(jt->stnum)[probe]; if(--probe<0)probe=AN(jt->stnum)-1;}  // return if locale match; wrap around at beginning of block
 return 0;  // if no match, return failure
}

// delete the locale numbered n, if it exists
static void jterasenl(J jt, I n){
 I probe=HASHSLOT(n,AN(jt->stnum));  // start of search.  Look backward, wrapping around, until we find a match or an empty.
 while(IAV0(jt->stnum)[probe]){if(NAV(LOCNAME((A)IAV0(jt->stnum)[probe]))->bucketx==n)break; if(--probe<0)probe=AN(jt->stnum)-1;}  // wrap around at beginning of block
 // We have found the match, or are at an empty if no match.  Either way, mark the location as empty and scan forward to find the next empty,
 // moving back blocks that might have hashed into the newly vacated spot
 if(IAV0(jt->stnum)[probe])--AM(jt->stnum);  // if we found something to delete, decrement # locales outstanding
 while(1){  // probe points to either the original deletion point or a value that was just copied to an earlier position.  Either way it gets deleted
  IAV0(jt->stnum)[probe]=0;  // delete the now-invalid or -moved location
  I lastdel=probe;    // remember where the hole is
  I probehash;   // will hold original hash of probe
  do{
   if(--probe<0)probe=AN(jt->stnum)-1;  // back up to next location to inspect
   if(!IAV0(jt->stnum)[probe])return;  // if we hit another hole, there can be no more value that need copying, we're done  *** RETURN POINT ***
   probehash=HASHSLOT(NAV(LOCNAME((A)IAV0(jt->stnum)[probe]))->bucketx,AN(jt->stnum));  // see where the probed cell would like to hash
    // If we are not allowed to move the new probe into the hole, because its hash is after the probe position but before-or-equal the hole,
    // we leave it in place and continue looking at the next position.  This test must be performed cyclically, because the probe may have wrapped around 0
  }while((BETWEENO(probehash,probe,lastdel))||(probe>lastdel&&(probe<=probehash||probehash<lastdel)));  // first half is normal, second if probe wrapped around
  // here lastdel is the hole, and probe is a slot that hashed somewhere before lastdel.  We can safely move the probe to cover the hole.
  // This creates a new hole at probe, which we loop back to clear & then try to fill
  IAV0(jt->stnum)[lastdel]=IAV0(jt->stnum)[probe];  // move the hole forward
 }
}

// return list of active numbered locales, using namelist mask
static A jtactivenl(J jt){A y;
 GATV0(y,INT,AN(jt->stnum),1); I *yv=IAV(y);   // allocate place to hold numbers of active locales
 I nloc=0; DO(AN(jt->stnum), if(IAV0(jt->stnum)[i]){yv[nloc]=NAV(LOCNAME((A)IAV0(jt->stnum)[i]))->bucketx; ++nloc;})
 return jtevery(jt,jttake(jt,sc(nloc),y),ds(CTHORN));  // ":&.> nloc{.y
}

// iterator support.  countnl returns a number of iterations.  indexnl returns the A block (or 0) for 
I jtcountnl(J jt) { return AN(jt->stnum); }  // number of locales to reference by index
A jtindexnl(J jt,I n) { return (A)IAV0(jt->stnum)[n]; }  // the locale address, or 0 if none

// Create symbol table: k is 0 for named, 1 for numbered, 2 for local; p is the number of hash entries including SYMLINFOSIZE;
// n is length of name (or locale# to allocate, for numbered locales), u->name
// Result is SYMB type for the symbol table.  For global tables only, ras() has been executed
// on the result and on the name and path
// For named/numbered types, SYMLINFO (hash chain #0) is filled in to point to the name and path
//   the name is an A type holding an NM, which has hash filled in, and, for numbered locales, the bucketx filled in with the locale number
// For local symbol tables, hash chain 0 is repurposed to hold symbol-index info for x/y (filled in later)
A jtstcreate(J jt,C k,I p,I n,C*u){A g,x,xx;C s[20];L*v;
 GATV0(g,SYMB,(p+1)&-2,0);   // have odd number of hashchains, excluding LINFO
 // Allocate a symbol for the locale info, install in special hashchain 0.  Set flag; // (it is queried by 18!:31)
 // The allocation clears all the hash chain bases, including the one used for SYMLINFO
 switch(k){
  case 0:  /* named    locale */
   RZ(v=jtsymnew(jt,&LXAV0(g)[SYMLINFO],0)); v->flag|=LINFO;    // allocate at head of chain
   RZ(x=jtnfs(jt,n,u));  // this fills in the hash for the name
   // Install name and path.  Path is 'z' except in z locale itself, which has empty path
   RZ(ras(x)); LOCNAME(g)=x; xx=1==n&&'z'==*u?vec(BOX,0L,0L):zpath; ras(xx); LOCPATH(g) = xx;   // ras() is never VIRTUAL
   // Assign this name in the locales symbol table to point to the allocated SYMB block
   // This does ras() on g
   symbisdel(x,g,jt->stloc);
   break;
  case 1:  /* numbered locale */
   RZ(v=jtsymnew(jt,&LXAV0(g)[SYMLINFO],0)); v->flag|=LINFO;    // allocate at head of chain
   sprintf(s,FMTI,n); RZ(x=jtnfs(jt,strlen(s),s)); NAV(x)->bucketx=n; // this fills in the hash for the name; we save locale# if numeric
   RZ(ras(x)); LOCNAME(g)=x; ras(zpath); LOCPATH(g)=zpath;  // ras() is never virtual
   // Put this locale into the in-use list at an empty location.  ras(g) at that time
   jtinstallnl(jt, g);  // put the locale into the numbered list at the value most recently returned (which must be n)
   break;
  case 2:  /* local symbol table */
   // Don't invalidate ACV lookups, since the local symbol table is not in any path
   AR(g)|=LLOCALTABLE;  // flag this as a local table so the first hashchain is not freed
   // The first hashchain is not used as a symbol pointer - it holds xy bucket info
   ;
 }
 return g;
}    /* create locale, named (0==k) or numbered (1==k) */

B jtsymbinit(J jt){A q;
 jt->locsize[0]=3;  /* default hash table size for named    locales */
 jt->locsize[1]=2;  /* default hash table size for numbered locales */
 RZ(symext(0));     /* initialize symbol pool                       */
 I p; FULLHASHSIZE(400,SYMBSIZE,1,SYMLINFOSIZE,p);
 GATV0(q,SYMB,p,0); jt->stloc=q;  // alloc space, clear hashchains.  No name/val for stloc
 jtinitnl(jt);  // init numbered locales
 FULLHASHSIZE(1LL<<10,SYMBSIZE,1,SYMLINFOSIZE,p);  // about 2^11 chains
 RZ(jt->global=stcreate(0,p,sizeof(jt->baselocale),jt->baselocale));
 FULLHASHSIZE(1LL<<12,SYMBSIZE,1,SYMLINFOSIZE,p);  // about 2^13 chains
 RZ(           stcreate(0,p,1L,"z"   ));
 // Allocate a symbol table with just 1 (empty) chain; then set length to 1 indicating 0 chains; make this the current local symbols, to use when no explicit def is running
 RZ(jt->locsyms=stcreate(2,2,0,0)); AKGST(jt->locsyms)=jt->global; AN(jt->locsyms)=1; AM(jt->locsyms)=(I)jt->locsyms;  // close chain so u. at top level has no effect
 return 1;
}


 A jtlocsizeq(J jt, A w){C*v; ASSERTMTV(w); v=jt->locsize; return jtv2(jt,v[0],v[1]);}
     /* 9!:38 default locale size query */

 A jtlocsizes(J jt, A w){I p,q,*v;
 ASSERT(1==AR(w),EVRANK);
 ASSERT(2==AN(w),EVLENGTH);
 RZ(w=vi(w)); v=AV(w); p=v[0]; q=v[1];
 ASSERT((p|q)>=0,EVDOMAIN);
 ASSERT(((p-14)&(q-14))<0,EVLIMIT);
 jt->locsize[0]=p;
 jt->locsize[1]=q;
 return mtm;
}    /* 9!:39 default locale size set */


// find the symbol table for locale with name u which has length n and hash/number bucketx
// locale name is known to be valid
// n=0 means 'use base locale'
// n=-1 means 'numbered locale, don't bother checking digits'   u is invalid
A jtstfind(J jt,I n,C*u,I bucketx){L*v;
 if(!n){n=sizeof(jt->baselocale); u=jt->baselocale;bucketx=jt->baselocalehash;}
 if(n>0&&'9'<*u){  // named locale   > because baselocale is known to be non-empty
  v=probe(n,u,(UI4)bucketx,jt->stloc);
  if(v)return v->val;   // if there is a symbol, return its value
 }else{
  return findnl(bucketx);
 }
 return 0;  // not found
}

// look up locale name, and create the locale if not found
// bucketx is hash (for named locale) or number (for numeric locale)
// n=0 means 'use base locale'
// n=-1 means 'numbered locale, don't bother checking digits'   u is invalid
A jtstfindcre(J jt,I n,C*u,I bucketx){
 A v = stfind(n,u,bucketx);  // lookup
 if(v)return v;  // return if found
 if(n>=0&&'9'<*u){  // nonnumeric locale:
  I p; FULLHASHSIZE(1LL<<(5+jt->locsize[0]),SYMBSIZE,1,SYMLINFOSIZE,p);
  return stcreate(0,p,n,u);  // create it with name
 }else{
  ASSERT(0,EVLOCALE); // illegal to create numeric locale explicitly
 }
}

// b is flags: 1=check name for validity, 2=do not allow numeric locales (whether atomic or not)
static A jtvlocnl(J jt,I b,A w){A*wv,y;C*s;I i,m,n;
 if(((b-2) & (SGNIF(AT(w),INTX) | (SGNIF(AT(w),B01X) & (AR(w)-1))))<0)return w;  // integer list or scalar boolean is OK
 n=AN(w);
 ASSERT(((n-1)|SGNIF(AT(w),BOXX))<0,EVDOMAIN);
 wv=AAV(w); 
 for(i=0;i<n;++i){
  y=wv[i];  // pointer to box
  if(((b-2)&(AR(y)-1)&-(AT(y)&(INT|B01)))<0)continue;   // scalar numeric locale is ok
  m=AN(y); s=CAV(y);
  ASSERT(1>=AR(y),EVRANK);
  ASSERT(m!=0,EVLENGTH);
  ASSERT(LIT&AT(y),EVDOMAIN);
  ASSERT(((1-b) & (I)((UI)CAV(y)[0]-('9'+1)))>=0,EVDOMAIN);  // numeric locale not allowed except when called for in b
  if(b&1)ASSERTN(vlocnm(m,s),EVILNAME,jtnfs(jt,m,s));
 }
 return w;
}    /* validate namelist of locale names  Returns list if all valid, else 0 for error */

// p points to string of length l.  Result stored in z, which must be a declared lvalue
// overflow is ignored, much as strtoI did (it clamped, which is similarly invalid).  We expect the user to have audited the length
#define strtoI10(p,l,z) {z=0; C* p_ = p; DQ(l, z=4*z+z; z=2*z + (*p_++-'0');)}
// subroutine version
I strtoI10s(I l,C* p) {I z; strtoI10(p,l,z); return z; }

 A jtlocnc(J jt, A w){A*wv,y,z;C c,*u;I i,m,n,*zv;
 RZ(jtvlocnl(jt,0,w));
 n=AN(w); wv=AAV(w); 
 GATV(z,INT,n,AR(w),AS(w)); zv=AV(z);
 if(!n)return z;  // if no input, return empty before handling numeric-atom case
 if(AT(w)&(INT|B01)){IAV(z)[0]=findnl(BIV0(w))?1:-1; return z;}  // if integer, must have been atomic or empty.  Handle the one value
 for(i=0;i<n;++i){
  y=wv[i];
  if(!AR(y)&&AT(y)&((INT|B01))){  // atomic numeric locale
   zv[i]=findnl(BIV0(y))?1:-1;  // OK because the boxed value cannot be virtual, thus must have padding
  }else{
   // string locale, whether number or numeric
   m=AN(y); u=CAV(y); c=*u; 
   if(!vlocnm(m,u))zv[i]=-2;
   else if(c<='9') zv[i]=findnl(strtoI10s(m,u))?1:-1;
   else            zv[i]=probe(m,u,(UI4)nmhash(m,u),jt->stloc)?0:-1;
  }
 }
 return z;
}    /* 18!:0 locale name class */

static A jtlocnlx(J jt, A w){A y,z=mtv;B*wv;I m=0;
 RZ(w=jtcvt(jt,B01,w)); wv=BAV(w); DO(AN(w), m|=1+wv[i];);  // accumulate mask of requested types
 if(1&m)z=nlsym(jt->stloc);  // named locales
 if(2&m){RZ(y=jtactivenl(jt)); z=over(y,z); }  // get list of active numbered locales
 return jtgrade2(jt,z,ope(z));
}

 A jtlocnl1(J jt, A w){memset(jt->workareas.namelist.nla,C1,256); return locnlx(w);}
    /* 18!:1 locale name list */

 A jtlocnl2(J jt,A a,A w){UC*u;
 ASSERT(LIT&AT(a),EVDOMAIN);
 memset(jt->workareas.namelist.nla,C0,256); 
 u=UAV(a); DQ(AN(a),jt->workareas.namelist.nla[*u++]=1;);
 return locnlx(w);
}    /* 18!:1 locale name list */

static A jtlocale(J jt,B b,A w){A g=0,*wv,y;
 if(((AR(w)-1) & -(AT(w)&(INT|B01)))<0)return (b?jtstfindcre:jtstfind)(jt,-1,0,BIV0(w));  // atomic integer is OK
 RZ(jtvlocnl(jt,1,w));
 wv=AAV(w); 
 DO(AN(w), y=AT(w)&BOX?AAV(w)[i]:sc(IAV(w)[i]); if(!(g=(b?jtstfindcre:jtstfind)(jt,AT(y)&(INT|B01)?-1:AN(y),CAV(y),AT(y)&(INT|B01)?BIV0(y):BUCKETXLOC(AN(y),CAV(y)))))return 0;);
 return g;
}    /* last locale (symbol table) from boxed locale names; 0 if none or error.  if b=1, create locale */

 A jtlocpath1(J jt, A w){AD * RESTRICT g; AD * RESTRICT z; F1RANK(0,jtlocpath1,UNUSED_VALUE); ASSERT(jtvlocnl(jt,1,w),EVDOMAIN); RZ(g=jtlocale(jt,1,w));
 g=LOCPATH(g); RZ(z=ca(g)); DO(AN(g), A t; RZ(t=ca(AAV(g)[i])); AS(t)[0]=AN(t); ACIPNO(t); AAV(z)[i]=t;) return z;
}
 // for paths, the shape holds the bucketx.  We must create a new copy that has the shape restored, and must incorporate it
     /* 18!:2  query locale path */

 A jtlocpath2(J jt,A a,A w){A g; AD * RESTRICT x;
 F2RANK(1,0,jtlocpath2,UNUSED_VALUE);
 if(AN(a))RZ(  jtlocale(jt,1,a)); RZ(x=jtevery(jt,ravel(a),ds(CCOMMA)));  // Don't audit empty a
 RZ(g=jtlocale(jt,1,w));
 // paths are special: the shape of each string holds the bucketx for the string.  Install that.
 AD * RESTRICT z; RZ(z=ca(x)); DO(AN(x), A t; RZ(t=ca(AT(AAV(x)[i])&((INT|B01))?thorn1(AAV(x)[i]):AAV(x)[i]));  AS(t)[0]=BUCKETXLOC(AN(t),CAV(t)); AAV(z)[i]=t;)  // ? why so many copies?  test before thorn1 not reqd
 fa(LOCPATH(g)); ras(z); LOCPATH(g)=z;
 ++jt->modifiercounter;  // invalidate any extant lookups of modifier names
 return mtm;
}    /* 18!:2  set locale path */


static A jtloccre(J jt,A a,A w){A g,y;C*s;I n,p;L*v;
 if(MARK&AT(a))p=jt->locsize[0]; else{RE(p=i0(a)); ASSERT(0<=p,EVDOMAIN); ASSERT(p<14,EVLIMIT);}
 y=AAV(w)[0]; n=AN(y); s=CAV(y);
 if(v=probe(n,s,(UI4)nmhash(n,s),jt->stloc)){   // scaf this is disastrous if the named locale is on the stack
  // named locale exists.  Verify no defined names, then delete it
  g=v->val; 
  LX *u=SYMLINFOSIZE+LXAV0(g); DO(AN(g)-SYMLINFOSIZE, ASSERT(!u[i],EVLOCALE););
  probedel(n,s,(UI4)nmhash(n,s),jt->stloc);  // delete the symbol for the locale, and the locale itself
 }
 FULLHASHSIZE(1LL<<(p+5),SYMBSIZE,1,SYMLINFOSIZE,p);  // get table, size 2^p+6 minus a little
 RZ(stcreate(0,p,n,s));
 return boxW(ca(y));
}    /* create a locale named w with hash table size a */

static A jtloccrenum(J jt, A w){C s[20];I k,p;
 if(MARK&AT(w))p=jt->locsize[1]; else{RE(p=i0(w)); ASSERT(0<=p,EVDOMAIN); ASSERT(p<14,EVLIMIT);}
 RE(k=jtgetnl(jt));
 FULLHASHSIZE(1LL<<(p+5),SYMBSIZE,1,SYMLINFOSIZE,p);  // get table, size 2^p+6 minus a little
 RZ(stcreate(1,p,k,0L));
 sprintf(s,FMTI,k); 
 return boxW(cstr(s));
}    /* create a numbered locale with hash table size n */

 A jtloccre1(J jt, A w){
 if(AN(w))return rank2ex0(mark,jtvlocnl(jt,2+1,w),UNUSED_VALUE,jtloccre);
 ASSERT(1==AR(w),EVRANK);
 return loccrenum(mark);
}    /* 18!:3  create locale */

 A jtloccre2(J jt,A a,A w){
 if(AN(w))return rank2ex0(a,jtvlocnl(jt,2+1,w),UNUSED_VALUE,jtloccre);
 ASSERT(1==AR(w),EVRANK);
 return rank1ex0(a,UNUSED_VALUE,jtloccrenum);
}    /* 18!:3  create locale with specified hash table size */


 A jtlocswitch(J jt, A w){A g;
 ASSERT(!AR(w),EVRANK); 
 RZ(g=jtlocale(jt,1,w));
 // put a marker for the operation on the call stack
 // If there is no name executing, there would be nothing to process this push; so don't push for unnamed execs (i. e. from console)
 if(jt->curname)pushcallstack1(CALLSTACKPOPFROM,jt->global);
 SYMSETGLOBAL(jt->locsyms,g);
 ++jt->modifiercounter;  // invalidate any extant lookups of modifier names

 return mtm;
}    /* 18!:4  switch locale */

 A jtlocname(J jt, A w){A g=jt->global;
 ASSERTMTV(w);
 ASSERT(g!=0,EVLOCALE);
 return boxW(jtsfn(jt,0,LOCNAME(g)));
}    /* 18!:5  current locale name */

static SYMWALK(jtlocmap1,I,INT,18,3,1,
    {I t=AT(d->val);
     *zv++=i; 
     I zc=2; zc=((0x20034>>(((t)>>(ADVX-1))&(CONJ+ADV+VERB>>(ADVX-1))))&3);   // C x V A (N) -> 16 x 4 2 0 -> 2 x 3 1 0    10 xx xx xx xx xx 11 01 00
     zc=t==SYMB?6:zc; zc=t&(NOUN|VERB|ADV|CONJ|SYMB)?zc:-2;
     *zv++=zc;
     *zv++=(I)rifvs(jtsfn(jt,SFNSIMPLEONLY,d->name));})  // this is going to be put into a box

 A jtlocmap(J jt, A w){A g,q,x,y,*yv,z,*zv;I c=-1,d,j=0,m,*qv,*xv;
 ASSERT(!AR(w),EVRANK);
 RE(g=equ(w,zeroionei(0))?jt->stloc:equ(w,zeroionei(1))?jt->locsyms:jtlocale(jt,0,w));
 ASSERT(g!=0,EVLOCALE);
 RZ(q=locmap1(g)); qv=AV(q);
 m=AS(q)[0];
 // split the q result between two boxes
 GATVR(x,INT,m*3,2,AS(q)); xv= AV(x);
 GATV0(y,BOX,m,  1); yv=AAV(y);
 DQ(m, *xv++=d=*qv++; *xv++=j=c==d?1+j:0; *xv++=*qv++; c=d; *yv++=incorp((A)*qv++););
 GAT0(z,BOX,2,1); zv=AAV(z); zv[0]=incorp(x); zv[1]=incorp(y);
 return z;
}    /* 18!:30 locale map */

static SYMWALK(jtredefg,B,B01,100,1,1,RZ(jtredef(jt,mark,d)))
     /* check for redefinition (erasure) of entire symbol table */

 A jtlocexmark(J jt, A w){A g,*wv,y,z;B *zv;C*u;I i,m,n;L*v;
 RZ(jtvlocnl(jt,1,w));
 if(AT(w)&B01)RZ(w=jtcvt(jt,INT,w));  // Since we have an array, we must convert b01 to INT
 n=AN(w); wv=AAV(w); 
 GATV(z,B01,n,AR(w),AS(w)); zv=BAV(z);
 for(i=0;i<n;++i){
  g=0;
  if(AT(w) & (INT)){zv[i]=1; g = findnl(IAV(w)[i]);
  }else{
   zv[i]=1; y=wv[i];
   if(AT(y)&(INT|B01)){g = findnl(BIV0(y));
   }else{
    m=AN(y); u=CAV(y);
    if('9'>=*u){g = findnl(strtoI10s(m,u));}
    else {v=probe(m,u,(UI4)nmhash(m,u),jt->stloc); if(v)g=v->val;}  // g is locale block for named locale
   }
  }
  if(g){I k;  // if the specified locale exists in the system...
   // See if we can find the locale on the execution stack.  If so, set the DELETE flag
   for(k=0;k<jt->callstacknext;++k)if(jt->callstack[k].value==g)break;
   if(k<jt->callstacknext)
    jt->callstack[k].type|=CALLSTACKDELETE;  // name active on stack; mark for deletion
   else if(g==jt->global){
    // Name is not on stack but it is executing now.  Add a change+delete entry for it.  There may be multiple of these outstanding
    pushcallstack1(CALLSTACKCHANGELOCALE|CALLSTACKDELETE,g);  // mark locale for deletion
   } else locdestroy(g);  // not on stack and not running - destroy immediately
  }
 }
 return z;
}    /* 18!:55 destroy a locale (but only mark for destruction if on stack) */

// destroy symbol table g.  
B jtlocdestroy(J jt,A g){
 ++jt->modifiercounter;  // invalidate any extant lookups of modifier names
 // Look at the name to see whether the locale is named or numbered
 NM *locname=NAV(LOCNAME(g));  // NM block for name
 B isnum = '9'>=locname->s[0];  // first char of name tells the type
 if(isnum){
  // For numbered locale, find the locale in the list of numbered locales, wipe it out, free the locale, and decrease the number of those locales
  RZ(redefg(g)); jterasenl(jt,locname->bucketx); fr(g);  // remove the locale from the hash table, then free the locale and its names
 } else {
  // For named locale, find the entry for this locale in the locales symbol table, and free the locale and the entry for it
  RZ(redefg(g)); probedel(locname->m,locname->s,locname->hash,jt->stloc);  // free the L block for the locale, which frees the locale itself and its names
 }
 if(g==jt->global)SYMSETGLOBAL(jt->locsyms,0);
 return 1;
}    /* destroy locale jt->callg[i] (marked earlier by 18!:55) */
