/* Copyright 1990-2006, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Xenos: Host Command Facilities                                          */

#include <sys/wait.h>
#include <unistd.h>
#ifndef ANDROID
#include <spawn.h>
#endif
#include <fcntl.h>
#include <stdint.h>

#ifdef MMSC_VER
#define mktemp _mktemp
#define unlink _unlink
#endif

#include "j.h"
#include "x.h"
#include "cpuinfo.h"

#if (SYS & SYS_ARCHIMEDES)
#define Wimp_StartTask 0x400DE
extern int os_swi1(I,I);
#endif


#if (SYS & SYS_MACINTOSH)

F1(jthost  ){ASSERT(0,EVDOMAIN);}
F1(jthostne){ASSERT(0,EVDOMAIN);}

#else

// return string indicating which JEs this hardware would run
// ""         would run j.dll
// "avx"      would run j.dll or javx.dll
// "avx avx2" would run j.dll or javx.dll or javx2.dll
F1(jtjgetx){

R cstr("");
}

F1(jthost){A z;
 F1RANK(1,jthost,UNUSED_VALUE);
 RZ(w=vslit(w));
{
 A t;I b=0;C*fn,*s;F f;I n;
 n=AN(w);
 GATV0(t,LIT,n+5+L_tmpnam+1,1); s=CAV(t);  // +1 for trailing nul
 fn=5+n+s; MC(s,AV(w),n);
 MC(n+s,"   > ",5L);
#ifdef MMSC_VER
 strcpy(fn,"tmp.XXXXXX");
 {A fz; mktemp(fn);
  RZ(fz=toutf16x(t));
  b=!_wsystem(USAV(fz));
 }
#else

 strcpy(fn,"/tmp");
 strcat(fn,"/tmp.XXXXXX");
 {int fd=mkstemp(fn); close(fd);}
#if defined(ANDROID) || (defined(__MACH__) && !defined(TARGET_IOS))
/* no posix_spawn */
 b=!system(s);
#else
// system() is deprecated
 extern char **environ;
 *(n+s)=0;  /* use action to redirect */
 int status;
 pid_t pid;
 posix_spawn_file_actions_t action;
 posix_spawn_file_actions_init(&action);
 posix_spawn_file_actions_addopen (&action, 1, fn, O_WRONLY | O_CREAT | O_TRUNC, 0644);
 char * argv[] = {"/bin/sh","-c",NULL,NULL};
 argv[2] = s;
 if (!(status = posix_spawn(&pid, argv[0], &action, NULL, &argv[0], environ))){
#if defined(__MACH__)
/* macos different behavior (from linux) for SIGCHLD */
/* no interface error will be reported */
   waitpid(pid, &status, 0); b = WIFEXITED(status) && !WEXITSTATUS(status);
#else
   if (-1!=waitpid(pid, &status, 0)) b = WIFEXITED(status) && !WEXITSTATUS(status);
#endif
 }
 posix_spawn_file_actions_destroy(&action);
#endif
 if(b){f=fopen(fn,FREAD_O); z=rd(f,0L,-1L); fclose(f);}
 unlink(fn);
 ASSERT(b&&f,EVFACE);
}
#endif
 R z;
}

F1(jthostne){
 F1RANK(1,jthostne,UNUSED_VALUE);
 RZ(w=vslit(w));
 {
  I b;
#ifdef MMSC_VER
  A fz;
  RZ(fz=toutf16x(w));
  b=_wsystem(USAV(fz));
#else
  b=system(CAV(str0(w)));
#endif
  b=!b;
  ASSERT(b!=0,EVFACE);
 }
#endif
 R mtv;
}



#if !(SYS & SYS_UNIX)

F1(jthostio){ASSERT(0,EVDOMAIN);}
F1(jtjwait ){ASSERT(0,EVDOMAIN);}

#else

#define CL(f) {close(f[0]);close(f[1]);}

F1(jthostio){C*s;A z;F*pz;int fi[2],fo[2],r;int fii[2],foi[2];
 if(pipe(fi)==-1) ASSERT(0,EVFACE);
 if(pipe(fo)==-1){CL(fi); ASSERT(0,EVFACE);}
 fii[0]=fi[0];fii[1]=fi[1];foi[0]=fo[0];foi[1]=fo[1];
 F1RANK(1,jthostio,UNUSED_VALUE);
 RZ(w=vs(w));
 s=CAV(str0(w)); GAT0(z,INT,3,1); pz=(F*)AV(z);
 if((r=pipe(fii))==-1||pipe(foi)==-1){if(r!=-1)CL(fi); ASSERT(0,EVFACE);}
 if(!((pz[1]=fdopen(fi[0],"r"))&&(pz[2]=fdopen(fo[1],"w")))){
  if(pz[1])fclose(pz[1]); CL(fi);CL(fo);}
 if(!add2(pz[1],pz[2],s)){fclose(pz[1]);fclose(pz[2]);
                               CL(fi);CL(fo);}
 switch(r=fork()){
  case -1:CL(fi);CL(fo);ASSERT(0,EVFACE);
  case 0:close(0);{int i=dup(fo[0]);};close(1);{int i=dup(fi[1]);};CL(fi);CL(fo);

         execl("/bin/sh","/bin/sh","-c",s,NULL); exit(-1);

 }close(fo[0]);close(fi[1]);
 add2(NULL,NULL,NULL); pz[0]=(F)(intptr_t)r;
 R z;
}

F1(jtjwait){I k;int s; RE(k=i0(w)); if(-1==waitpid(k,&s,0))jerrno(); R sc(s);}

#endif

/* return errno info from c library */
F1(jtcerrno){C buf[1024],ermsg[1024];
 ASSERTMTV(w);

 if(errno&&!strerror_r(errno,ermsg,1024)) strcpy (buf, ermsg); else strcpy (buf, "");
 R link(sc(errno),cstr(buf));
}    /* 2!:8  errno information */

