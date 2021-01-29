/* Copyright 1990-2007, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Xenos: Custom                                                           */


static A jtcblas_dgemm(J jt,A a,A w,A self);
static A jtcblas_dscal(J jt,A a,A w,A self);
static A jtdgeev(J jt,    A w,A self);
static A jtdgeqrf(J jt,    A w,A self);
static A jtdmultmatmat_32x32(J jt,A a,A w,A self);
static A jtmatadd(J jt,A a,A w,A self);
static A jtseqex(J jt,A a,A w,A self);
static A jtseqpow(J jt,A a,A w,A self);
static A jtx102d0(J jt,A a,A w,A self);
static A jtx102d1(J jt,A a,A w,A self);
static A jtx103d0(J jt,    A w,A self);
static A jtwex(J jt, A w);
static A jttiebox(J jt,A a,A w);
static A jtbg(J jt, A w);
static A jtgb(J jt, A w);
static A jttconj(J jt,    A w,A self);
static A jtoa(J jt,A a,A w,A self);

#define bg(x)                   jtbg(jt, (x))
#define consfn(x)               jtconsfn(jt,(x))
#define gb(x)                   jtgb(jt, (x))
#define oa(x)                   jtoa(jt,(x))
#define seqdo(x0,x1,x2,x3,x4)   jtseqdo(jt,(x0),(x1),(x2),(x3),(x4))
#define seqlim(x,y,z)           jtseqlim(jt,(x),(y),(z))
#define tconj(x)                jttconj(jt, (x))
#define tiebox(x,y)             jttiebox(jt,(x),(y))
#define wex(x)                  jtwex(jt,(x))
