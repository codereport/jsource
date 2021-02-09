/* Copyright 1990-2005, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Xenos: file directory, attributes, & permission                         */


#include <sys/types.h>
#include <unistd.h>

#include "j.h"
#include "x.h"

char* toascbuf(char* s){ return s;}
char* tounibuf(char* s){ return s;}

/* jdir produces a 5-column matrix of boxes:                 */
/* 0 name                                                    */
/* 1 time of last write, y m d h m s                         */
/* 2 size                                                    */
/* 3 permission -- 0 read  1 write  2 execute                */
/* 4 attributes                                              */
/*   0 read-only    3 volume label                           */
/*   1 hidden       4 directory                              */
/*   2 system       5 archive (modified since last back-up)  */

/* FIXME:   rename J link() function so we can include unistd.h */
// undefs to avoid darwin warnings - should be a better fix
#undef R_OK
#undef W_OK
#undef X_OK
#define R_OK    4               /* Test for read permission.    */
#define W_OK    2               /* Test for write permission.   */
#define X_OK    1               /* Test for execute permission. */

#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <fnmatch.h>

/* Return mode_t formatted into 11-character buffer supplied by the caller.  The last byte of the buffer is the string terminator \0 */
static C*modebuf(mode_t m,C* b){C c;I t=m;
 strcpy(b+1,"rwxrwxrwx");
 DO(9, if(!(m&1))b[9-i]='-'; m>>=1;); 
 if(t&S_ISUID)b[3]=(b[3]=='x')?'s':'S';
 if(t&S_ISGID)b[6]=(b[6]=='x')?'s':'S';
 if(t&S_ISVTX)b[9]=(b[9]=='x')?'t':'T';
 switch(t&S_IFMT){
  case S_IFBLK:  b[0]='b'; break;
  case S_IFCHR:  b[0]='c'; break;
  case S_IFDIR:  b[0]='d'; break;
  case S_IFLNK:  b[0]='l'; break;
  case S_IFSOCK: b[0]='s'; break;
  case S_IFREG:  b[0]='-'; break;
  default:       b[0]='?';
 }
 return b;
}


static int ismatch(J jt,C*pat,C*name,struct stat *dirstatbuf,C *diratts, C *dirmode,C *dirrwx,C *dirnamebuf,C *dirbase){ 

 strcpy(dirbase,name); if(stat(dirnamebuf,dirstatbuf))return 0;
 if('.'!=*pat && ((!strcmp(name,"."))||(!strcmp(name,".."))))return 0;
 if(fnmatch(pat,name,0)) return 0;
/* Set up dirrwx, diratts, and dirmode for this file */
 dirrwx[0]=access(dirnamebuf,R_OK)?'-':'r';
 dirrwx[1]=access(dirnamebuf,W_OK)?'-':'w';
 dirrwx[2]=access(dirnamebuf,X_OK)?'-':'x';
 strcpy(diratts,"------");
 diratts[0]=(dirrwx[0]=='r'&&dirrwx[1]=='-')?'r':'-';
 diratts[1]=('.'==name[0])?'h':'-';
 modebuf(dirstatbuf[0].st_mode,dirmode);
 diratts[4]=('d'==dirmode[0])?'d':'-';
 return 1;
}

static A jtdir1(J jt,struct dirent*f,struct stat *dirstatbuf,C *diratts, C *dirmode,C *dirrwx){A z,*zv;C*s,att[16];I n,ts[6],i,m,sz;S x;struct tm tmr,*tm=&tmr;
 tm=localtime_r(&dirstatbuf[0].st_mtime,tm);
 ts[0]=1900+tm->tm_year; ts[1]=1+tm->tm_mon; ts[2]=tm->tm_mday;
 ts[3]=tm->tm_hour; ts[4]=tm->tm_min; ts[5]=tm->tm_sec;
 s=f->d_name; n=strlen(s);
 GAT0(z,BOX,6,1); zv=AAV(z);
 RZ(zv[0]=vec(LIT,n,s)); 
 RZ(zv[1]=vec(INT,6L,ts));
 sz=dirstatbuf[0].st_size;
 sz=sz<0?-1:sz;
 RZ(zv[2]=sc(sz));
 RZ(zv[3]=vec(LIT,3L, dirrwx ));
 RZ(zv[4]=vec(LIT, 6L,diratts));
 RZ(zv[5]=vec(LIT,10L,dirmode));
 return z;
}

 A jtjdir(J jt, A w){PROLOG(0103);A*v,z,*zv;C*dir,*pat,*s,*x;I j=0,n=32;DIR*DP;struct dirent *f;
 C diratts[7]; C dirmode[11];  C dirrwx[3];
 struct stat dirstatbuf[3];  // for some reason there were 2 dummy blocks reserved after the buffer for 32-bit Linux.  Preserve that
 C dirnamebuf[NPATH];  // workarea
 RZ(w=str0(vslit(!AR(w)&&BOX&AT(w)?ope(w):w)));
 s=CAV(w);
 if(x=strrchr(s,'/')){dir=s==x?(C*)"/":s; pat=x+1; *x=0;}else{dir="."; pat=s;}
 if(NULL==(DP=opendir(dir)))return jtreshape(jt,jtv2(jt,0L,6L),ds(CACE));
 /*
  * SYSV and BSD have different return types for sprintf(),
  * so we use less efficient but portable code.
  */
 sprintf(dirnamebuf,"%s/",dir); C * dirbase=dirnamebuf+strlen(dirnamebuf); f=readdir(DP);
 GATV0(z,BOX,n,1); zv=AAV(z);
 while(f){
  if(ismatch(jt,pat,f->d_name,dirstatbuf,diratts,dirmode,dirrwx,dirnamebuf,dirbase)){
   if(j==n){RZ(z=jtext(jt,0,z)); n=AN(z); zv=AAV(z);}
   RZ(zv[j++]=jtdir1(jt,f,dirstatbuf,diratts,dirmode,dirrwx));
  }
  f=readdir(DP);
 }
 closedir(DP);
 z=j?ope(j<n?vec(BOX,j,zv):z):jtreshape(jt,jtv2(jt,0L,6L),ds(CACE));
 EPILOG(z);
}



 A jtjfatt1(J jt, A w){ASSERT(0,EVNONCE);}
 A jtjfatt2(J jt,A a,A w){ASSERT(0,EVNONCE);}


 A jtjfperm1(J jt, A w){A y;F f;C b[11];
 struct stat dirstatbuf[3];
 F1RANK(0,jtjfperm1,UNUSED_VALUE);
 RE(f=stdf(w)); if(f){RZ(y=fname(sc((I)f)));y=str0(y);} else ASSERT(y=str0(vslit(AAV(w)[0])),EVFNUM)
 if(0!=stat(CAV(y),dirstatbuf))return jerrno();
 return vec(LIT,9L,1+modebuf(dirstatbuf[0].st_mode,b));
}


static const struct tperms {C*c;I p[4];} permtab[]=
    {  {"-r"  ,{0,S_IRUSR}},
       {"-w"  ,{0,S_IWUSR}},
       {"-xSs",{0,S_IXUSR,S_ISUID,S_ISUID+S_IXUSR}},
       {"-r"  ,{0,S_IRGRP}},
       {"-w"  ,{0,S_IWGRP}},
       {"-xSs",{0,S_IXGRP,S_ISGID,S_ISGID+S_IXGRP}},
       {"-r"  ,{0,S_IROTH}},
       {"-w"  ,{0,S_IWOTH}},
       {"-xTt",{0,S_IXOTH,S_ISVTX,S_ISVTX+S_IXOTH}},
    };

 A jtjfperm2(J jt,A a,A w){A y;C*s;F f;int x=0,i;C*m;
 F2RANK(1,0,jtjfperm2,UNUSED_VALUE);
 RE(f=stdf(w)); if(f){RZ(y=fname(sc((I)f)));y=str0(y);} else ASSERT(y=str0(vslit(AAV(w)[0])),EVFNUM)
 RZ(a=vslit(a)); ASSERT(9==AN(a),EVLENGTH); s=CAV(a);
 for(i=0;i<9;i++)
    {ASSERT(NULL!=(m=strchr(permtab[i].c,s[i])),EVDOMAIN);
     x|=permtab[i].p[m-permtab[i].c];}
 return chmod(CAV(y),x)?jerrno():mtm;
}



/* ----------------------------------------------------------------------- */

 A jtfullname(J jt, A w){return w;}
