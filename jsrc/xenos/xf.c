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
#include <sys/stat.h>
#include <unistd.h>
#include <fts.h>

#include "j.h"
#include "x.h"

#include <stdlib.h>
typedef long long INT64;



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
  tr+=r; if(tr==(U)n){RZ(za=jtext(jt,0,za));n*=2;}
 }
 if(tr==(U)n)z=za;
 else {GATV0(z,LIT,(I)tr,1); memcpy(CAV(z),CAV(za),tr);}
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
  if(ferror(f))return jtjerrno(jt);
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
  if(ferror(f))return jtjerrno(jt)?1:0;
 }
 return 1;
}    /* write/append string w to file f at j */


 A jtjfread(J jt, A w){A z;F f;
 F1RANK(0,jtjfread,UNUSED_VALUE);
 RE(f=stdf(w));  // f=file#, or 0 if w is a filename
 if(f)return 1==(I)f?jgets("\001"):3==(I)f?rdns(stdin):rd(vfn(f),0L,-1L);  // if special file, read it all, possibly with error
 RZ(f=jtjope(jt,w,FREAD_O)); z=rd(f,0L,-1L); fclose(f);  // otherwise open/read/close named file
 return z;
}

 A jtjfwrite(J jt,A a,A w){B b;F f;
 F2RANK(RMAX,0,jtjfwrite,UNUSED_VALUE);
 if(BOX&AT(w)){ASSERT(1>=AR(a),EVRANK); ASSERT(!AN(a)||AT(a)&LIT+C2T+C4T,EVDOMAIN);}
 RE(f=stdf(w));
 if(2==(I)f){b=jt->tostdout; jt->tostdout=1; jt->mtyo=MTYOFILE; jpr(a); jt->mtyo=0; jt->tostdout=b; return a;}
 if(4==(I)f){return (U)AN(a)!=fwrite(CAV(a),sizeof(C),AN(a),stdout)?jtjerrno(jt):a;}
 if(5==(I)f){return (U)AN(a)!=fwrite(CAV(a),sizeof(C),AN(a),stderr)?jtjerrno(jt):a;}
 if(b=!f)RZ(f=jtjope(jt,w,FWRITE_O)) else RE(vfn(f)); 
 wa(f,0L,a); 
 if(b)fclose(f);else fflush(f);
 RNE(mtm);
}

 A jtjfappend(J jt,A a,A w){B b;F f;
 F2RANK(RMAX,0,jtjfappend,UNUSED_VALUE);
 RE(f=stdf(w));
 if(2==(I)f){B b=jt->tostdout; jt->tostdout=1; jpr(a); jt->tostdout=b; return a;}
 ASSERT(!AN(a)||AT(a)&LIT+C2T+C4T,EVDOMAIN);
 ASSERT(1>=AR(a),EVRANK);
 if(b=!f)RZ(f=jtjope(jt,w,FAPPEND_O)) else RE(vfn(f));
 wa(f,fsize(f),a);
 if(b)fclose(f);else fflush(f);
 RNE(mtm);
}

 A jtjfsize(J jt, A w){B b;F f;I m;
 F1RANK(0,jtjfsize,UNUSED_VALUE);
 RE(f=stdf(w));
 if(b=!f)RZ(f=jtjope(jt,w,FREAD_O)) else RE(vfn(f)); 
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

 A jtjiread(J jt, A w){A z=0;B b;F f;I i,n;
 F1RANK(1,jtjiread,UNUSED_VALUE);
 RE(f=ixf(w)); if(b=!f)RZ(f=jtjope(jt,w,FREAD_O));
 if(ixin(w,fsize(f),&i,&n))z=rd(f,i,n);
 if(b)fclose(f);else fflush(f);
 return z;
}

 A jtjiwrite(J jt,A a,A w){B b;F f;I i;
 F2RANK(RMAX,1,jtjiwrite,UNUSED_VALUE);
 ASSERT(!AN(a)||AT(a)&LIT+C2T+C4T,EVDOMAIN);
 ASSERT(1>=AR(a),EVRANK);
 RE(f=ixf(w)); if(b=!f)RZ(f=jtjope(jt,w,FUPDATE_O));
 if(ixin(w,fsize(f),&i,0L))wa(f,i,a); 
 if(b)fclose(f);else fflush(f);
 RNE(mtm);
}

 A jtjmkdir(J jt, A w){A y,z;
 F1RANK(0,jtjmkdir,UNUSED_VALUE);
 ASSERT(AT(w)&BOX,EVDOMAIN);
 RZ(y=str0(vslit(AAV(w)[0])));
 return mkdir(CAV(y),0775)?jtjerrno(jt):num(1);
}

 A jtjferase(J jt, A w){A y,fn;US*s;I h;
 F1RANK(0,jtjferase,UNUSED_VALUE);
 RE(h=fnum(w));
 if(h) {RZ(y=str0(fname(sc(h))))} else ASSERT(y=vslit(AAV(w)[0]),EVFNUM);
 if(h)RZ(jclose(sc(h)));
 A y0=str0(y); return !unlink(CAV(y0))||!rmdir(CAV(y0))?num(1):jtjerrno(jt);

}    /* erase file or directory */

 A jtpathcwd(J jt, A w){C path[1+NPATH];US wpath[1+NPATH];
 ASSERTMTV(w);
 ASSERT(getcwd(path,NPATH),EVFACE);
 return cstr(path);
}

 A jtpathchdir(J jt, A w){A z;
 ASSERT(1>=AR(w),EVRANK);
 ASSERT(AN(w),EVLENGTH);
 ASSERT((LIT+C2T+C4T)&AT(w),EVDOMAIN);
 ASSERT(!chdir(CAV(toutf8x(w))),EVFACE);
 return mtv;
}

 A jtjgetenv(J jt, A w){
 F1RANK(1,jtjgetenv,UNUSED_VALUE);
 ASSERT((LIT+C2T+C4T)&AT(w),EVDOMAIN);
 {
  C*s;
  return(s=getenv(CAV(toutf8x(w))))?cstr(s):num(0); // toutf8x has trailing nul
 }
 return num(0);
}

 A jtjgetpid(J jt, A w){
 ASSERTMTV(w);

 return(sc(getpid()));
}

 A jtpathdll(J jt, A w){
 ASSERTMTV(w); return cstr((C*)"");
}

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
