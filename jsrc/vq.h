/* Copyright 1990-2007, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Rational Numbers                                                        */

#define QASSERT(b,e)    {if(!(b)){jsignal(e); return zeroQ;}}
#define QEPILOG(q)      {Q z9; z9=(qstd(q)); if(!gc3(&z9.n,&z9.d,0L,_ttop))return zeroQ; return z9;}
#define QRE(exp)        {if((exp),jt->jerr)return zeroQ;}
#define QRZ(exp)        {if(!(exp))        return zeroQ;}
#define QX1(x)          (1==AN(x)&&1==XDIG(x))

#define QEQ(x,y)        (jtequ(jt,(x).n,(y).n)&&jtequ(jt,(x).d,(y).d))
#define QCOMP(x,y)      (jtxcompare(jt,xtymes((x).n,(y).d),xtymes((y).n,(x).d)))
#define QLT(x,y)        (0> QCOMP(x,y))
#define QLE(x,y)        (0>=QCOMP(x,y))
#define QGT(x,y)        (0< QCOMP(x,y))
#define QGE(x,y)        (0<=QCOMP(x,y))
#define QMAX(x,y)       (QGE(x,y)?x:y)
#define QMIN(x,y)       (QLE(x,y)?x:y)

extern Q jtqstd(J jt,   Q  w);

extern Q jtqdiv(J jt,Q a,Q w);
extern Q jtqminus(J jt,Q a,Q w);
extern Q jtqplus(J jt,Q a,Q w);
extern Q jtqtymes(J jt,Q a,Q w);

extern Q zeroQ;
