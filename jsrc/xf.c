/* Copyright 1990-2008, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Xenos: Files                                                            */

/* File functions accept file number or boxed file name or 1 or 2          */


#ifdef __GNUC__
#define _GNU_SOURCE
#include <dlfcn.h>
#endif
#include <sys/types.h>
#include <unistd.h>
#include <fts.h>

#include "j.h"
#include "x.h"

#if (SYS & SYS_UNIX)
#include <stdlib.h>
typedef long long INT64;
static int rmdir2(const char *dir);
#endif



static I fsize(F f){
 if(!f)return 0;

 fpos_t z;
 fseek(f,0L,SEEK_END);
 fgetpos(f,&z);
 return *(I*)&z;
}

static A jtrdns(J jt,F f){A za,z;I n=1024;size_t r,tr=0;
 GAT0(za,LIT,1024,1); clearerr(f);
 while(!feof(f) && (r=fread(CAV(za)+tr,sizeof(C),n-tr,f))){
  tr+=r; if(tr==(U)n){RZ(za=ext(0,za));n*=2;}
 }
 if(tr==(U)n)z=za;
 else {GATV0(z,LIT,(I)tr,1); MC(CAV(z),CAV(za),tr);}
 return z;
}    /* read entire file stream (non-seekable) */

A jtrd(J jt,F f,I j,I n){A z;C*x;I p=0;size_t q=1;
 RZ(f);
 if(0>n){if(j<0) n=-j; else n=fsize(f)-j;}

 {INT64 v; v= j+((0>j)?fsize(f):0); fsetpos(f,(fpos_t*)&v);}

 clearerr(f);
 GATV0(z,LIT,n,1); x=CAV(z);
 while(q&&n>p){
  p+=q=fread(p+x,sizeof(C),(size_t)(n-p),f);
  if(ferror(f))return jerrno();
 }
 return z;
}    /* read file f for n bytes at j */

static B jtwa(J jt,F f,I j,A w){C*x;I n,p=0;size_t q=1;
 RZ(f&&w);
 n=AN(w)*(C4T&AT(w)?4:C2T&AT(w)?2:1); x=CAV(w);

 {INT64 v; v= j+((0>j)?fsize(f):0); fsetpos(f,(fpos_t*)&v);}
 
 clearerr(f);
 while(q&&n>p){
  p+=q=fwrite(p+x,sizeof(C),(size_t)(n-p),f); 
  if(ferror(f))return jerrno()?1:0;
 }
 return 1;
}    /* write/append string w to file f at j */


F1(jtjfread){A z;F f;
 F1RANK(0,jtjfread,UNUSED_VALUE);
 RE(f=stdf(w));  // f=file#, or 0 if w is a filename
 if(f)return 1==(I)f?jgets("\001"):3==(I)f?rdns(stdin):rd(vfn(f),0L,-1L);  // if special file, read it all, possibly with error
 RZ(f=jope(w,FREAD_O)); z=rd(f,0L,-1L); fclose(f);  // otherwise open/read/close named file
 RETF(z);
}

F2(jtjfwrite){B b;F f;
 F2RANK(RMAX,0,jtjfwrite,UNUSED_VALUE);
 if(BOX&AT(w)){ASSERT(1>=AR(a),EVRANK); ASSERT(!AN(a)||AT(a)&LIT+C2T+C4T,EVDOMAIN);}
 RE(f=stdf(w));
 if(2==(I)f){b=jt->tostdout; jt->tostdout=1; jt->mtyo=MTYOFILE; jpr(a); jt->mtyo=0; jt->tostdout=b; return a;}
 if(4==(I)f){return (U)AN(a)!=fwrite(CAV(a),sizeof(C),AN(a),stdout)?jerrno():a;}
 if(5==(I)f){return (U)AN(a)!=fwrite(CAV(a),sizeof(C),AN(a),stderr)?jerrno():a;}
 if(b=!f)RZ(f=jope(w,FWRITE_O)) else RE(vfn(f)); 
 wa(f,0L,a); 
 if(b)fclose(f);else fflush(f);
 RNE(mtm);
}

F2(jtjfappend){B b;F f;
 F2RANK(RMAX,0,jtjfappend,UNUSED_VALUE);
 RE(f=stdf(w));
 if(2==(I)f){B b=jt->tostdout; jt->tostdout=1; jpr(a); jt->tostdout=b; return a;}
 ASSERT(!AN(a)||AT(a)&LIT+C2T+C4T,EVDOMAIN);
 ASSERT(1>=AR(a),EVRANK);
 if(b=!f)RZ(f=jope(w,FAPPEND_O)) else RE(vfn(f));
 wa(f,fsize(f),a);
 if(b)fclose(f);else fflush(f);
 RNE(mtm);
}

F1(jtjfsize){B b;F f;I m;
 F1RANK(0,jtjfsize,UNUSED_VALUE);
 RE(f=stdf(w));
 if(b=!f)RZ(f=jope(w,FREAD_O)) else RE(vfn(f)); 
 m=fsize(f); 
 if(b)fclose(f);else fflush(f);
 RNE(sc(m));
}

static F jtixf(J jt,A w){F f;
 ASSERT(2<=AN(w),EVLENGTH);
 switch(CTTZNOFLAG(AT(w))){
  default:  ASSERT(0,EVDOMAIN);
  case B01X: ASSERT(0,EVFNUM);
  case BOXX: ASSERT(2==AN(w),EVLENGTH); f=stdf(head(w)); break; 
  case INTX: f=(F)AV(w)[0]; ASSERT(2<(UI)f,EVFNUM);
 }
 return f?vfn(f):f;
}    /* process index file arg for file number; 0 if a file name */

static B jtixin(J jt,A w,I s,I*i,I*n){A in,*wv;I j,k,m,*u;
 if(AT(w)&BOX){wv=AAV(w);  RZ(in=vi(wv[1])); k=AN(in); u=AV(in);}
 else{in=w; k=AN(in)-1; u=1+AV(in);}
 ASSERT(1>=AR(in),EVRANK);
 ASSERT(k&&k<=(n?2:1),EVLENGTH);
 j=u[0]; j=0>j?s+j:j; m=1==k?s-j:u[1];
 ASSERT(0<=j&&(!n||j<s&&j+m<=s&&0<=m),EVINDEX);
 *i=j; if(n)*n=m;
 return 1;
}    /* process index file arg for index and length */

F1(jtjiread){A z=0;B b;F f;I i,n;
 F1RANK(1,jtjiread,UNUSED_VALUE);
 RE(f=ixf(w)); if(b=!f)RZ(f=jope(w,FREAD_O));
 if(ixin(w,fsize(f),&i,&n))z=rd(f,i,n);
 if(b)fclose(f);else fflush(f);
 return z;
}

F2(jtjiwrite){B b;F f;I i;
 F2RANK(RMAX,1,jtjiwrite,UNUSED_VALUE);
 ASSERT(!AN(a)||AT(a)&LIT+C2T+C4T,EVDOMAIN);
 ASSERT(1>=AR(a),EVRANK);
 RE(f=ixf(w)); if(b=!f)RZ(f=jope(w,FUPDATE_O));
 if(ixin(w,fsize(f),&i,0L))wa(f,i,a); 
 if(b)fclose(f);else fflush(f);
 RNE(mtm);
}

F1(jtjmkdir){A y,z;
 F1RANK(0,jtjmkdir,UNUSED_VALUE);
 ASSERT(AT(w)&BOX,EVDOMAIN);
 RZ(y=str0(vslit(AAV(w)[0])));
 return mkdir(CAV(y),0775)?jerrno():num(1);
}

F1(jtjferase){A y,fn;US*s;I h;
 F1RANK(0,jtjferase,UNUSED_VALUE);
 RE(h=fnum(w));
 if(h) {RZ(y=str0(fname(sc(h))))} else ASSERT(y=vslit(AAV(w)[0]),EVFNUM);
 if(h)RZ(jclose(sc(h)));
 A y0=str0(y); return !unlink(CAV(y0))||!rmdir(CAV(y0))?num(1):jerrno();

}    /* erase file or directory */

F1(jtpathcwd){C path[1+NPATH];US wpath[1+NPATH];
 ASSERTMTV(w);
#if (SYS & SYS_UNIX)
 ASSERT(getcwd(path,NPATH),EVFACE);
#else
 ASSERT(_wgetcwd(wpath,NPATH),EVFACE);
 jttoutf8w(jt,path,NPATH,wpath);
#endif
 return cstr(path);
}

F1(jtpathchdir){A z;
 ARGCHK1(w);
 ASSERT(1>=AR(w),EVRANK);
 ASSERT(AN(w),EVLENGTH);
 ASSERT((LIT+C2T+C4T)&AT(w),EVDOMAIN);
#if (SYS & SYS_UNIX)
 ASSERT(!chdir(CAV(toutf8x(w))),EVFACE);
#else
 RZ(z=toutf16x(toutf8(w))); USAV(z)[AN(z)]=0;  // install termination
 ASSERT(0==_wchdir(USAV(z)),EVFACE);
#endif
 return mtv;
}

F1(jtjgetenv){
 F1RANK(1,jtjgetenv,UNUSED_VALUE);
 ASSERT((LIT+C2T+C4T)&AT(w),EVDOMAIN);
#if (SYS & SYS_UNIX)
 {
  C*s;
  return(s=getenv(CAV(toutf8x(w))))?cstr(s):num(0); // toutf8x has trailing nul
 }
#else // # i think this corresponds to WIN -ish stuff
 {
  A z; US* us;
  RZ(z=toutf16x(toutf8(w))); USAV(z)[AN(z)]=0;  // install termination
  us=_wgetenv(USAV(z));
  if(!us)return num(0);
  GATV0(z,C2T,wcslen(us),1);
  MC(USAV(z),us,2*wcslen(us));
  return toutf8(z);
 }
#endif
 return num(0);
}

F1(jtjgetpid){
 ASSERTMTV(w);

 return(sc(getpid()));
}

#if (SYS & SYS_UNIX)
// #ifdef __GNUC__
F1(jtpathdll){
 ASSERTMTV(w); return cstr((C*)"");
}
#else
F1(jtpathdll){char p[MAX_PATH]; extern C dllpath[];
 ASSERTMTV(w);
 strcpy(p,dllpath);
 if('\\'==p[strlen(p)-1]) p[strlen(p)-1]=0;
 return cstr(p);
}
#endif

#if (SYS & SYS_UNIX)
int rmdir2(const char *dir)
{
 int ret=0;
 FTS *ftsp=NULL;
 FTSENT *curr;

 char *files[]={ (char *) dir, NULL };

 // FTS_NOCHDIR  - Avoid changing cwd, which could cause unexpected behavior
 //                in multithreaded programs
 // FTS_PHYSICAL - Don't follow symlinks. Prevents deletion of files outside
 //                of the specified directory
 // FTS_XDEV     - Don't cross filesystem boundaries
 ftsp=fts_open(files, FTS_NOCHDIR | FTS_PHYSICAL | FTS_XDEV, NULL);
 if (!ftsp) {
//  fprintf(stderr, "%s: fts_open failed: %s\n", dir, strerror(errno));
  ret=-1;
  goto finish;
 }

 while ((curr=fts_read(ftsp))) {
  switch (curr->fts_info) {
   case FTS_NS:
   case FTS_DNR:
   case FTS_ERR:
//   fprintf(stderr, "%s: fts_read error: %s\n", curr->fts_accpath, strerror(curr->fts_errno));
   break;

   case FTS_DC:
   case FTS_DOT:
   case FTS_NSOK:
   // Not reached unless FTS_LOGICAL, FTS_SEEDOT, or FTS_NOSTAT were
   // passed to fts_open()
   break;

   case FTS_D:
   // Do nothing. Need depth-first search, so directories are deleted
   // in FTS_DP
   break;

   case FTS_DP:
   case FTS_F:
   case FTS_SL:
   case FTS_SLNONE:
   case FTS_DEFAULT:
    if (remove(curr->fts_accpath) < 0) {
//    fprintf(stderr, "%s: Failed to remove: %s\n", curr->fts_path, strerror(errno));
    ret=-1;
   }
   break;
  }
 }

finish:
 if (ftsp) {
  fts_close(ftsp);
 }

 return ret;
}
#endif
