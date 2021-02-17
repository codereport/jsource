/* Copyright 1990-2002, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Xenos: Implementation Internals                                         */

#include "j.h"
#include "x.h"

A
jtaflag1(J jt, A w) {
    return jtsc(jt, AFLAG(w));
}

A
jtaflag2(J jt, A a, A w) {
    I k;
    RE(k = jti0(jt, a));
    AFLAG(w) = (FLAGT)k;
    return w;
}

A
jthash(J jt, A w) {
    RZ(w = jtvs(jt, w));
    return jtsc(jt, hic(AN(w), UAV(w)));
}
