/* Copyright 1990-2004, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Grades                                                                  */


extern A jtgr1(J jt, A w);
extern A jtgr2(J jt,A a,A w);
extern A jtgrd1sp(J jt, A w);
extern A jtgrd2sp(J jt,A a,A w);

extern B compcu(I,UC*,UC*);
extern B compcd(I,UC*,UC*);
extern B compuu(I,US*,US*);
extern B compud(I,US*,US*);
extern B comptu(I,C4*,C4*);
extern B comptd(I,C4*,C4*);
extern B compr(I,A*,A*);
extern B comppu(I,D*,D*);
extern B comppd(I,D*,D*);
extern B compxu(I,X*,X*);
extern B compxd(I,X*,X*);
extern B compqu(I,Q*,Q*);
extern B compqd(I,Q*,Q*);
extern B compp(I,I*,I*);

extern int  compspdsB(J,I,I);
extern int  compspdsD(J,I,I);
extern int  compspdsI(J,I,I);
extern int  compspdsZ(J,I,I);

extern int  compspssB(J,I,I);
extern int  compspssD(J,I,I);
extern int  compspssI(J,I,I);
extern int  compspssZ(J,I,I);

extern I grcol4(I,I,UI4*,I,I*,I*,const I,US*,I);
extern I grcol2(I,I,US*,I,I*,I*,const I,US*,I);

extern void jtmsort(J,I,void**,void**);

// Convert 2 Booleans to a code 0-3
#define IND2(x) {US xx = (x); ii = ((xx<<9)|xx)>>8;}

// Convert 4 Booleans to a code 0-15
#define IND4(x) {UINT xx = (x); xx|=xx<<9; xx|=xx<<18; ii = xx>>24;}  // first byte (bit 0) is the MSB when a word is loaded

// endian constants for 16-bit radix sorts
#define FPLSBWDX  (0)
#define FPMSBWDX  (3)
#define INTLSBWDX (0)
#define INTMSBWDX (SZI/2-1)
#define WDINC     (1)
