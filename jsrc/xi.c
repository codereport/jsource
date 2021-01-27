/* Copyright 1990-2002, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Xenos: Implementation Internals                                         */

#include "j.h"
#include "x.h"


F1(jtaflag1){return sc(AFLAG(w));}

F2(jtaflag2){I k; RE(k=i0(a)); AFLAG(w)=(FLAGT)k; return w;}

F1(jthash){RZ(w=vs(w)); return sc(hic(AN(w),UAV(w)));}
