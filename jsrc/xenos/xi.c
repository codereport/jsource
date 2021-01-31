/* Copyright 1990-2002, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Xenos: Implementation Internals                                         */

#include "j.h"
#include "x.h"


 A jtaflag1(J jt, A w){ARGCHK1(w); return sc(AFLAG(w));}

 A jtaflag2(J jt,A a,A w){I k; ARGCHK2(a,w); RE(k=i0(a)); AFLAG(w)=(FLAGT)k; return w;}

 A jthash(J jt, A w){RZ(w=vs(w)); return sc(hic(AN(w),UAV(w)));}
