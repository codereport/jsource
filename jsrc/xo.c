/* Copyright 1990-2008, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Xenos: File Open/Close                                                  */


       
#include "j.h"
#include "x.h"


B jtxoinit(J jt){A x;

 GAT0(x,BOX,8,1); memset(AV(x),C0,AN(x)*SZI); ras(x); jt->fopa=x;
 GAT0(x,INT,8,1);                             ras(x); jt->fopf=x; AM(jt->fopf)=0;  // AM is # valid files
 return 1;
}

F jtvfn(J jt,F x){I*v=AV(jt->fopf); DQ(AM(jt->fopf),if(x==(F)*v++)return x;); ASSERT(0,EVFNUM);}
     /* check that x is in table of file#s */

I jtfnum(J jt,A w){A y;I h,j;
 if(AT(w)&B01+INT){ASSERT(h=i0(w),EVFNUM); return h;}
 ASSERT(AT(w)&BOX,EVDOMAIN);
 y=AAV(w)[0];
 ASSERT(AN(y),EVLENGTH);
 if(AT(y)&B01+INT){ASSERT(h=i0(y),EVFNUM); return h;}
 RE(j=i0(indexof(vec(BOX,AM(jt->fopf),AAV(jt->fopa)),boxW(fullname(vslit(y)))))); 
 return j<AM(jt->fopf)?*(j+AV(jt->fopf)):0;
}    /* file# corresp. to standard argument w */

F1(jtfname){I j; 
 RE(j=i0(indexof(jt->fopf,w)));
 ASSERT(j<AM(jt->fopf),EVFNUM);
 return ca(*(j+AAV(jt->fopa)));
}    /* string name corresp. to file# w */

F1(jtjfiles){A y,z;
 ASSERTMTV(w);
 RZ(y=vec(INT,AM(jt->fopf),AV(jt->fopf)));
 return grade2(stitch(IRS1(y,0,0,jtbox,z),vec(BOX,AM(jt->fopf),AV(jt->fopa))),y);
}    /* file (number,name) table */

F jtjope(J jt,A w,C*mode){A t;F f;I n;static I nf=25; A z;
 ARGCHK1(w);
 ASSERT(BOX&AT(w),EVDOMAIN);
 RZ(t=str0(vslit(AAV(w)[0])));
 n=AN(t)-1;
 ASSERT(n!=0,EVLENGTH);
#if (SYS&SYS_UNIX)
{
 C* cs=CAV(t);
 f=fopen(cs,mode);
 if(!f&&errno==ENOENT&&!strcmp(mode,FUPDATE_O))f=fopen(cs,FUPDATEC_O);
 if(!f&&errno==EACCES&& strcmp(mode,FREAD_O  ))f=fopen(cs,FREAD_O);
}
#else
{
 US usmode[10]; US*s; I i;
 RZ(z=jttoutf16x(jt,t));
 s=USAV(z);
 for(i=0;i<(I)strlen(mode);++i){usmode[i]=(US)mode[i];}
 usmode[i]=0;
 f=_wfopen(s,usmode);
 if(!f&&errno==ENOENT&&!wcscmp(usmode,FLUPDATE_O))f=_wfopen(s,FLUPDATEC_O);
 if(!f&&errno==EACCES&& wcscmp(usmode,FLREAD_O  ))f=_wfopen(s,FLREAD_O);
}
#endif
 return f?f:(F)jerrno();
}

F1(jtjopen){A z;I h;
 ARGCHK1(w);
 if(!AN(w))return w;
 if(AR(w))return rank1ex0(w,UNUSED_VALUE,jtjopen);
 RE(h=fnum(w));
 if(h){RZ(z=sc(h)); RZ(fname(z)); return z;}
 else{A ww;
  if(AM(jt->fopf)==AN(jt->fopf)){I ct=AM(jt->fopf); RZ(jt->fopa=ext(1,jt->fopa)); RZ(jt->fopf=ext(1,jt->fopf)); AM(jt->fopf)=ct;}
  RZ(*(AM(jt->fopf)+IAV(jt->fopf))=h=(I)jope(w,FUPDATE_O));
  RZ(ww=fullname(vslit(AAV(w)[0]))); RZ(ras(ww));
  RZ(*(AM(jt->fopf)+AAV(jt->fopa))=ww);
 
  ++AM(jt->fopf);
  return sc(h);
}}   /* open the file named w if necessary; return file# */

B jtadd2(J jt,F f1,F f2,C*cmd){A c,x;
 if(f1==NULL) {AM(jt->fopf)+=2;return 1;};
 GATV0(c,LIT,1+strlen(cmd),1);MC(CAV(c)+1,cmd,AN(c)-1);cmd=CAV(c);
 if(AM(jt->fopf)+1>=AN(jt->fopf)){I ct=AM(jt->fopf); RZ(jt->fopa=ext(1,jt->fopa)); RZ(jt->fopf=ext(1,jt->fopf)); AM(jt->fopf)=ct;}
 *cmd='<';x=cstr(cmd); RZ(ras(x)); RZ(*(AM(jt->fopf)+AAV(jt->fopa)  )=x); RZ(*(AM(jt->fopf)+IAV(jt->fopf)  )=(I)f1);
 *cmd='>';x=cstr(cmd); RZ(ras(x)); RZ(*(AM(jt->fopf)+AAV(jt->fopa)+1)=x); RZ(*(AM(jt->fopf)+IAV(jt->fopf)+1)=(I)f2);
 fa(c); return 1;
}   /* add 2 entries to AM(jt->fopf) table (for hostio); null arg commits entries */


F1(jtjclose){A*av;I*iv,j;
 ARGCHK1(w);
 if(!AN(w))return w;
 if(AR(w))return rank1ex0(w,UNUSED_VALUE,jtjclose);
 RE(j=i0(indexof(jt->fopf,sc(fnum(w))))); ASSERT(j<AM(jt->fopf),EVFNUM);
 av=AAV(jt->fopa); iv=IAV(jt->fopf); 
 if(fclose((F)iv[j]))return jerrno();
 --AM(jt->fopf); fa(av[j]); if(j<AM(jt->fopf)){av[j]=av[AM(jt->fopf)]; iv[j]=iv[AM(jt->fopf)];}
 return num(1);
}    /* close file# w */

F jtstdf(J jt,A w){A y;F f;I n,r,t;
 ARGCHK1(w);
 ASSERT(AN(w),EVLENGTH);
 ASSERT(!AR(w),EVRANK);
 if(BOX&AT(w)){
  y=AAV(w)[0]; t=AT(y); n=AN(y); r=AR(y);
  if(t&(LIT+C2T+C4T)){ASSERT(1>=r,EVRANK); ASSERT(n!=0,EVLENGTH); return 0;}
/*!
  if(t&C2T){ASSERT(1>=r,EVRANK); ASSERT(n!=0,EVLENGTH); ASSERT(vc1(n,USAV(y)),EVDOMAIN); return 0;}
     vc1 can now be killed off
*/
  if(t&B01+INT)return stdf(y);
  ASSERT(0,EVDOMAIN);
 }
 f=(F)i0(w); 
 ASSERT(f!=0,EVFNUM); 
 return f;
}    /* 0 if w is a boxed file name; n if w is integer or boxed integer */
