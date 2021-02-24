/* Copyright 1990-2002, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Debug: Single Step                                                      */

#include "j.h"
#include "d.h"

/* d->dcss   - single step code in current function                        */
/* jt->dbssd - the d whose dcss is non-zero                                */
/* jt->dbss  - single step code for propagation to appropriate function    */

DC
jtssnext(J jt, DC d, C c) {
    d = d->dclnk;
    while (d && DCCALL != d->dctype) d = d->dclnk; /* find next call                 */
    if (d && !d->dcsusp) {
        d->dcss   = c;
        jt->dbssd = d;
    } else {
        d         = 0;
        jt->dbssd = 0;
    }
    return d;
} /* set dcss for next stack level */

static A
jtssdo(J jt, A a, A w, C c) {
    DC d, e;
    I n;
    RZ(w = jtvs(jt, w));
    ASSERT(jt->uflags.us.cx.cx_c.db, EVDOMAIN);
    d = jt->sitop;                            /* cut back to topmost suspension  */
    while (d && !d->dcsusp) {                 /* do until topmost suspension     */
        if (d->dctype == DCCALL) DGOTO(d, -1) /* terminate each call             */
        d = d->dclnk;
    }
    ASSERT(d != 0, EVDOMAIN);                      /* must have a suspension          */
    while (d && DCCALL != d->dctype) d = d->dclnk; /* find topmost call               */
    ASSERT(d != 0, EVDOMAIN);                      /* must have a call                */
    if (a) RE(n = lnumcw(jti0(jt, a), d->dcc));    /* source line # to cw line #      */
    jt->dbsusact = SUSSS;
    switch (c) {
        case SSSTEPOVER:
            DGOTO(d, a ? n : d->dcix) jt->dbss = d->dcss = c;
            jt->dbssd                                    = d;
            break;
        case SSSTEPINTO:
            DGOTO(d, a ? n : d->dcix) jt->dbss = d->dcss = c;
            jt->dbssd                                    = d;
            break;
        case SSSTEPOUT:
            DGOTO(d, a ? n : d->dcix) jt->dbss = d->dcss = 0;
            jtssnext(jt, d, c);
            break;
        case SSCUTBACK:
            DGOTO(d, -1) jt->dbss = d->dcss = 0;
            e                               = jtssnext(jt, d, c);
            if (e) DGOTO(e, e->dcix) break;  // terminate current verb and back up in caller
    }
    fa(jt->dbssexec);
    if (AN(w)) {
        RZ(ras(w));
        jt->dbssexec = w;
    } else
        jt->dbssexec = 0;
    return mtm; /* 0 return to terminate call      */
}

A
jtdbcutback(J jt, A w) {
    return jtssdo(jt, 0L, w, SSCUTBACK);
} /* 13!:19 */

A
jtdbstepover1(J jt, A w) {
    return jtssdo(jt, 0L, w, SSSTEPOVER);
} /* 13!:20 */
A
jtdbstepover2(J jt, A a, A w) {
    return jtssdo(jt, a, w, SSSTEPOVER);
}

A
jtdbstepinto1(J jt, A w) {
    return jtssdo(jt, 0L, w, SSSTEPINTO);
} /* 13!:21 */
A
jtdbstepinto2(J jt, A a, A w) {
    return jtssdo(jt, a, w, SSSTEPINTO);
}

A
jtdbstepout1(J jt, A w) {
    return jtssdo(jt, 0L, w, SSSTEPOUT);
} /* 13!:22 */
A
jtdbstepout2(J jt, A a, A w) {
    return jtssdo(jt, a, w, SSSTEPOUT);
}
