/* Copyright 1990-2006, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Xenos: CRC calculation and base64 encode/decode                         */

#include "j.h"
#include "x.h"

#include "../base64/include/libbase64.h"
#define B64CODEC BASE64_FORCE_PLAIN

// Calculate byte-at-a-time CRC table in *crctab, and return the starting value as the result
static UINT jtcrcvalidate(J jt,A w, UINT* crctab){A*wv;B*v;I m;UINT p,x,z=-1;
 ASSERT(1>=AR(w),EVRANK);
 m=AN(w);
 if(m&&BOX&AT(w)){ASSERT(2>=m,EVLENGTH); wv=AAV(w);  w=wv[0]; if(2==m)RE(z=(UINT)i0(wv[1]));}
 if(B01&AT(w)){ASSERT(32==AN(w),EVLENGTH); v=BAV(w); p=0; DQ(32, p<<=1; p|=*v++;);}
 else RE(p=(UINT)i0(w));
 DO(256, x=(UINT)i; DO(8, x=(x>>1)^(p&((UINT)-(I4)(x&1)));); crctab[i]=x;); 
 return z;
}

F1(jtcrc1){return crc2(sc(-306674912),w);}

F2(jtcrc2){I n;UINT z;UC*v; UINT crctab[256];
 ARGCHK2(a,w);
 ASSERT(1>=AR(a)&&1>=AR(w),EVRANK);
 n=AN(w); v=UAV(w);
// ASSERT(!n||AT(w)&LIT+C2T+C4T,EVDOMAIN);
 ASSERT(!n||AT(w)&LIT,EVDOMAIN);
 RE(z=crcvalidate(a,crctab));
 n=AT(w)&C4T?(4*n):AT(w)&C2T?n+n:n;
 DQ(n, z=z>>8^crctab[255&(z^*v++)];);  // do the computation using unsigned z
 return sc((I)(I4)(z^-1L));  // sign-extend result if needed to make 64-bit and 32-bit the same numeric value
}

F1(jtcrccompile){A h,*hv;UINT z; UINT crctab[256];
 GAT0(h,BOX,2,1); hv=AAV(h);
 RE(z=crcvalidate(w,crctab));
 RZ(hv[0]=rifvs(vec(LIT,sizeof(crctab),crctab)));  // Save the table.  We don't have any other good type to use
 RZ(hv[1]=rifvs(sc((I)z)));
 return h;
}

DF1(jtcrcfixedleft){A h,*hv;I n;UINT*t,z;UC*v;
 h=FAV(self)->fgh[2]; hv=AAV(h); t=(UINT*)AV(hv[0]); z=(UINT)AV(hv[1])[0];
 n=AN(w); v=UAV(w);
 ASSERT(!n||AT(w)&LIT+C2T+C4T,EVDOMAIN);
 n=AT(w)&C4T?(4*n):AT(w)&C2T?n+n:n;
 DQ(n, z=z>>8^t[255&(z^*v++)];);
 return sc((I)(I4)(z^-1L));
}

// CRC-based hash.  Bivalent
#ifndef CRC32L
#define CRC32L(acc,in) (0xffffffff&((acc*15015)^(in)))   // if no hardware CRC (rare), mix the bits a little
#endif
F2(jtqhash12){F2PREFIP; I hsiz; UI crc;
 ARGCHK2(a,w);
 if(AT(w)&NOUN){RE(hsiz=i0(vib(a)));} else{w=a; hsiz=0;}  // fetch hashtable size; set w=data to hash
 ASSERT(hsiz>=0,EVDOMAIN);
 ASSERT(AT(w)&DENSE,EVNONCE);  // not sparse for now
 if(AT(w)&DIRECT){ // Direct value, calculate CRC of atoms
  crc=hic(AN(w)<<bplg(AT(w)),UCAV(w));  // sign-extend result if needed to make 64-bit and 32-bit the same numeric value
 }else{   // not DIRECT, calculate CRC of component CRCs
  crc=-1;  // where we accumulate CRC
  I lpct=AN(w)<<((AT(w)>>RATX)&1);  // number of component values
  A *av=AAV(w);  // pointer to subvalues
  DQ(lpct, crc=CRC32L(crc,i0(jtqhash12(jt,zeroionei(0),*av++)));)  // recur
 }
 if(hsiz)crc=(crc*(UI)hsiz)>>32;   // convert hash to user's range
 return sc((I)(I4)crc);   // make the result a valid integer.  Could reuse the a arg inplace
}

// base64 stuff

static C base64tab[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
F1(jttobase64){
 F1RANK(1,jttobase64,UNUSED_VALUE);  // here we deal only with rank<= 1
 ASSERT(AT(w)&LIT,EVDOMAIN);  // we work only on ASCII (which includes UTF-8).  Other types must be converted first
 // Calculate # triples, #extras
 I n3=AN(w)/3; I ne=AN(w)%3;
 I zn=n3+SGNTO0(-ne);  // total # result 4-byte groups
 // Allocate result
 A z; GATV0(z,LIT,zn<<2,1); UI4 *zv=UI4AV(z);  // result block, pointer into it
 C *wv=CAV(w);  // input pointer
size_t zlen=AN(z);
base64_encode(wv, AN(w), CAV(z), &zlen, B64CODEC );
ASSERT((I)zlen==AN(z),EVDOMAIN);  // make sure length agreed
 return z;

}

static C base64invtab[256] = {
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,['+']=62,0xff,0xff,0xff,['/']=63,['0']=52,['1']=53,['2']=54,['3']=55,['4']=56,['5']=57,['6']=58,['7']=59,['8']=60,['9']=61,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,['A']=0,['B']=1,['C']=2,['D']=3,['E']=4,['F']=5,['G']=6,['H']=7,['I']=8,['J']=9,['K']=10,['L']=11,['M']=12,['N']=13,['O']=14,['P']=15,['Q']=16,['R']=17,['S']=18,['T']=19,['U']=20,['V']=21,['W']=22,['X']=23,['Y']=24,['Z']=25,0xff,0xff,0xff,0xff,0xff,
0xff,['a']=26,['b']=27,['c']=28,['d']=29,['e']=30,['f']=31,['g']=32,['h']=33,['i']=34,['j']=35,['k']=36,['l']=37,['m']=38,['n']=39,['o']=40,['p']=41,['q']=42,['r']=43,['s']=44,['t']=45,['u']=46,['v']=47,['w']=48,['x']=49,['y']=50,['z']=51,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
 };

F1(jtfrombase64){
 F1RANK(1,jtfrombase64,UNUSED_VALUE);  // here we deal only with rank<= 1
 ASSERT(AT(w)&LIT,EVDOMAIN);  // we work only on ASCII
 // Calculate the number of result bytes.  We take the number of input bytes, and discard trailing '=' on a 2- or 3-boundary
 I wn=AN(w);
 if((wn&3)==0 && wn && CAV(w)[wn-1]=='=')--wn;  // discard trailing = at boundary
 if((wn&3)==3 && CAV(w)[wn-1]=='=')--wn;  // discard trailing = at boundary
 ASSERT((wn&3)!=1,EVDOMAIN);  // cannot have remnant of 1 char
 // Allocate result
 A z; GATV0(z,LIT,(wn>>2)*3 + (((wn&3)+1)>>1),1);  // 3 bytes per full set, plus 0, 1, or 2
 // process the input in full 4-byte groups.  We may overread the input AND overwrite the result, but we will always stay in the padding area,
 // which is OK because we allocated the result here
size_t zlen=AN(z);
int rc=base64_decode(CAV(w), AN(w), CAV(z), &zlen, B64CODEC );
ASSERT(rc==1&&(I)zlen==AN(z),EVDOMAIN);  // make sure no invalid input bytes
 return z;
}
  // the 3 bytes are characters AAAAABB BBBBCCCC CCDDDDDD in bits
  //                            7     0 15     8 23    16
