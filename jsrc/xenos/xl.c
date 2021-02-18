/* Copyright 1990-2005, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Xenos: File Lock/Unlock                                                 */

#include "j.h"
#ifdef link
#undef link /* name conflict ? */
#endif
#include "x.h"

typedef long long INT64;
#include <stdint.h>
#include <stdio.h>
#define LOCK 1
#include <unistd.h>
#include <fcntl.h>

static B
jtdolock(J jt, B lk, F f, I i, I n) {
    I e;
    long c;
    fpos_t v;
    fpos_t q;
    c = fgetpos(f, (fpos_t*)&q);
    if (0 != c) return (B)(intptr_t)jtjerrno(jt);
    {
        INT64 vv;
        vv = i;
        v  = *(fpos_t*)&vv;
    }
    c = fsetpos(f, (fpos_t*)&v);
    if (0 != c) return (B)(intptr_t)jtjerrno(jt);

    e = lockf(fileno(f), lk ? F_TLOCK : F_ULOCK, (I)n);
    fsetpos(f, (fpos_t*)&q);
    return !e ? 1 : errno == EACCES ? 0 : (B)(intptr_t)jtjerrno(jt);
}

#ifndef LOCK
static B
jtdolock(J jt, B lk, F f, I i, I n) {
    I e;
    e = lk ? lock(fileno(f), i, n) : unlock(fileno(f), i, n);
    return !e ? 1 : errno == EACCES ? 0 : (intptr_t)jtjerrno(jt);
}
#endif

#define LKC 3 /* number of columns in jt->flkd table       */

A
jtjlocks(J jt, A w) {
    A y;
    ASSERTMTV(w);
    y = jttake(jt, jtsc(jt, AM(jt->flkd)), jt->flkd);
    return jtgrade2(jt, y, y);
}
/* return the locks, a 3-column table of (number,index,length) */

A
jtjlock(J jt, A w) {
    B b;
    I* v;
    F1RANK(1, jtjlock, UNUSED_VALUE);
    RZ(w = jtvi(jt, w));
    ASSERT(LKC == AN(w), EVLENGTH);
    v = AV(w);
    RE(jtvfn(jt, (F)*v));
    ASSERT(0 <= v[1] && 0 <= v[2], EVDOMAIN);
    if (AM(jt->flkd) == AS(jt->flkd)[0]) {
        I ct = AM(jt->flkd);
        RZ(jt->flkd = jtext(jt, 1, jt->flkd));
        AM(jt->flkd) = ct;
    }
    RE(b = dolock(1, (F)v[0], v[1], v[2]));
    if (!b) return num(0);
    ICPY(AV(jt->flkd) + LKC * AM(jt->flkd), v, LKC);
    ++AM(jt->flkd);
    return num(1);
} /* w is (number,index,length); lock the specified region */

static A
jtunlj(J jt, I j) {
    B b;
    I *u, *v;
    RE(j);
    ASSERT(BETWEENO(j, 0, AM(jt->flkd)), EVINDEX);
    u = AV(jt->flkd);
    v = u + j * LKC;
    RE(b = dolock(0, (F)v[0], v[1], v[2]));
    if (!b) return num(0);
    --AM(jt->flkd);
    if (j < AM(jt->flkd))
        ICPY(v, u + AM(jt->flkd) * LKC, LKC);
    else
        *v = 0;
    return num(1);
} /* unlock the j-th entry in jt->flkd */

B
jtunlk(J jt, I x) {
    I j = 0, *v = AV(jt->flkd);
    while (j < AM(jt->flkd)) {
        while (x == *v) RZ(jtunlj(jt, j));
        ++j;
        v += LKC;
    }
    return 1;
} /* unlock all existing locks for file# x */

A
jtjunlock(J jt, A w) {
    F1RANK(1, jtjunlock, UNUSED_VALUE);
    ASSERT(INT & AT(w), EVDOMAIN);
    return jtunlj(jt, jti0(jt, jtindexof(jt, jt->flkd, w)));
} /* w is (number,index,length); unlock the specified region */
