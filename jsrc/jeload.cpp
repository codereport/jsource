/* Copyright 1990-2008, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
// utilities for JFE to load JE, initiallize, and run profile sentence
// JFEs are jconsole, jwdw, and jwdp

extern "C" {

#define PLEN 1000 // path length
#ifdef _WIN32
 #include <windows.h>
 #define GETPROCADDRESS(h,p) GetProcAddress(h,p)
 #define JDLLNAME "j.dll"
 #define filesep '\\'
 #define filesepx "\\"
 #ifdef MMSC_VER
 #define strcasecmp _stricmp
 #endif
#else
 #include <unistd.h>
 #include <dlfcn.h>
 #define GETPROCADDRESS(h,p)	dlsym(h,p)
 #define _stdcall
 #define filesep '/'
 #define filesepx "/"
 #ifdef __MACH__
  extern int _NSGetExecutablePath(char*, int*);
  #define JDLLNAME "libj.dylib"
 #else
  #include <sys/utsname.h>
  #define JDLLNAME "libj.so"
 #endif
#endif
#include "j.h"
#include "jversion.h"
#include <stdint.h>

static void* hjdll; // handle to J DLL
static J jt;
static JDoType jdo;
static JFreeType jfree;
static JgaType jga;
static JGetLocaleType jgetlocale;
static JGetAType jgeta;
static JSetAType jseta;
char path[PLEN];
char pathdll[PLEN];
static char jdllver[20];
static int FHS=0;


int jedo(char* sentence)
{
	return jdo(jt,(C*)sentence);
}

A jegeta(I n, char* s){return jgeta(jt,n,(C*)s);}
I jeseta(I n,char* name,I x,char* d){return jseta(jt,n,(C*)name,x,(C*)d);}
void jefree(){jfree(jt);}
char* jegetlocale(){return (char*)jgetlocale(jt);}
A jega(I t, I n, I r, I*s){return jga(jt,t,n,r,s);}
void* jehjdll(){return hjdll;}

// load JE, Jinit, getprocaddresses, JSM
J jeload(void* callbacks)
{
 hjdll=dlopen(pathdll,RTLD_LAZY);
 if(!hjdll) {
	char* error = dlerror();
 	printf( "ERROR\tCould not open library globally: %s\n", error ? error : "" );
	return nullptr;
 }
 jt = static_cast<JST*>(reinterpret_cast<JInitType>(dlsym(hjdll,"JInit"))());
 if(!jt) return nullptr;
 ((JSMType)GETPROCADDRESS(hjdll,"JSM"))(jt,callbacks);
 jdo=(JDoType)GETPROCADDRESS(hjdll,"JDo");
 jfree=(JFreeType)GETPROCADDRESS(hjdll,"JFree");
 jga=(JgaType)GETPROCADDRESS(hjdll,"Jga");
 jgetlocale=(JGetLocaleType)GETPROCADDRESS(hjdll,"JGetLocale");
 jgeta=(JGetAType)GETPROCADDRESS(hjdll,"JGetA");
 jseta=(JSetAType)GETPROCADDRESS(hjdll,"JSetA");
 return jt;
}

// set path and pathdll (wpath also set for win)
// WIN arg is 0, Unix arg is argv[0]
void jepath(char* arg,char* lib)
{
#ifndef _WIN32
 struct stat st;
#endif
#ifdef _WIN32
 WCHAR wpath[PLEN];
 GetModuleFileNameW(0,wpath,_MAX_PATH);
 *(wcsrchr(wpath, '\\')) = 0;
 WideCharToMultiByte(CP_UTF8,0,wpath,1+(int)wcslen(wpath),path,PLEN,0,0);
#endif

#ifndef _WIN32
#define sz 4000
 char arg2[sz],arg3[sz];
 char* src,*snk;int n,len=sz;
 // fprintf(stderr,"arg0 %s\n",arg);
 // try host dependent way to get path to executable
 // use arg if they fail (arg command in PATH won't work)
#ifdef __MACH__ 
 n=_NSGetExecutablePath(arg2,&len);
 if(0!=n) strcat(arg2,arg);
#else
 n=readlink("/proc/self/exe",arg2,sizeof(arg2));
 if(-1==n) strcpy(arg2,arg); else arg2[n]=0;
#endif
 // fprintf(stderr,"arg2 %s\n",arg2);
 // arg2 is path (abs or relative) to executable or soft link
 n=readlink(arg2,arg3,sz);
 if(-1==n) strcpy(arg3,arg2); else arg3[n]=0;
 // fprintf(stderr,"arg3 %s\n",arg3);
 if('/'==*arg3)
  strcpy(path,arg3);
 else
 {
  if(!getcwd(path,sizeof(path)))path[0]=0;
  strcat(path,"/");
  strcat(path,arg3);
 }
 *(1+strrchr(path,'/'))=0;
 // remove ./ and backoff ../
 snk=src=path;
 while(*src)
 {
	 if('/'==*src&&'.'==*(1+src)&&'.'==*(2+src)&&'/'==*(3+src))
	 {
		 *snk=0;
		 snk=strrchr(path,'/');
		 snk=nullptr==snk?path:snk;
		 src+=3;
	 }
	 else if('/'==*src&&'.'==*(1+src)&&'/'==*(2+src))
      src+=2;
	 else
	  *snk++=*src++;
 }
 *snk=0;
 snk=path+strlen(path)-1;
 if('/'==*snk) *snk=0;
#endif
 strcpy(pathdll,path);
 strcat(pathdll,filesepx);
 strcat(pathdll,JDLLNAME);
#if !defined(_WIN32) && !defined(__MACH__) && !defined(ANDROID) // Not sure what this should be for???
 char pathdllpx[10];
 strncpy(pathdllpx,pathdll,10); pathdllpx[9]=0;
 if(stat(pathdll,&st)&&!strcmp(pathdllpx,"/usr/bin/")) FHS=1;
 if (FHS) {
  char _jdllver[20];
  strcpy(_jdllver,jversion);
  jdllver[0]=_jdllver[0];
  jdllver[1]='.';
  strcat(jdllver+2,_jdllver+1);
  strcpy(pathdll,JDLLNAME);
  strcat(pathdll,".");
  strcat(pathdll,jdllver);
 }
#endif
 if(*lib)
 {
	 if(filesep==*lib || ('\\'==filesep && ':'==lib[1]))
		 strcpy(pathdll,lib); // absolute path
	 else
	 {
		 strcpy(pathdll,path);
		 strcat(pathdll,filesepx);
		 strcat(pathdll,lib); // relative path
	 }
 }
}

// called by jwdp (java jnative.c) to set path
void jesetpath(char* arg)
{
	strcpy(pathdll,arg); // jwdp gives path to j.dll
	strcpy(path,arg);
	if(strrchr(path,filesep))*(strrchr(path,filesep)) = 0;
}

// build and run first sentence to set BINPATH, ARGV, and run profile
// arg is command line ready to set in ARGV_z_
// type is 0 normal, 1 -jprofile xxx, 2 ijx basic, 3 nothing
// profile[ARGV_z_=:...[BINPATH=:....
// profile is from BINPATH, ARGV, ijx basic, or nothing
int jefirst(int type,char* arg)
{
	int r; 
	char* p;
	char* q;
	char* input=static_cast<char* >(malloc(2000+strlen(arg)));
#if 0
	char buf[50];
#endif
	*input=0;
	if(0==type)
	{
      if (!FHS)
            strcat(input,"(3 : '0!:0 y')<BINPATH,'");
      else {
            strcat(input,"(3 : '0!:0 y')<'/etc/j/");
            strcat(input,jdllver);
      }
      strcat(input,filesepx);
      strcat(input,"profile.ijs'");
	}
	else if(1==type)
		strcat(input,"(3 : '0!:0 y')2{ARGV");
	else if(2==type)
		strcat(input,"");
	else
		strcat(input,"i.0 0");
	strcat(input,"[ARGV_z_=:");
	strcat(input,arg);

#if defined(_WIN32)
	strcat(input,"[UNAME_z_=:'Win'");
#elif defined(__MACH__)
	strcat(input,"[UNAME_z_=:'Darwin'");
#endif
#if 0
	sprintf(buf,"(" FMTI ")",(I)(intptr_t)hjdll);
	strcat(input,"[HLIBJ_z_=:");
	strcat(input,buf);
#endif
	strcat(input,"[BINPATH_z_=:'");
	p=path;
	q=input+strlen(input);
	while(*p)
	{
		if(*p=='\'') *q++='\'';	// 's doubled
		*q++=*p++;
	}
	*q=0;
	strcat(input,"'");

	strcat(input,"[LIBFILE_z_=:'");
	p=pathdll;
	q=input+strlen(input);
	while(*p)
	{
		if(*p=='\'') *q++='\'';	// 's doubled
		*q++=*p++;
	}
	*q=0;
	strcat(input,"'");

	r=jedo(input);
	free(input);
	return r;
}

void jefail(char* msg)
{
	strcpy(msg, "Load library ");
	strcat(msg, pathdll);
	strcat(msg," failed: ");
#ifdef _WIN32
	char buf[256];
	FormatMessageA(
	 FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
	 NULL, GetLastError(),
	 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  /* Default language */
	 buf, (sizeof(buf)/sizeof(char)), 0);
	strcat(msg,buf);
#else
	char ermsg[1024];
	if(errno&&!strerror_r(errno,ermsg,1024))strcat(msg,ermsg);
#endif
	strcat(msg,"\n");
}

}