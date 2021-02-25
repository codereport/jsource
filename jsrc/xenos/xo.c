/* Copyright 1990-2008, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Xenos: File Open/Close                                                  */

#include "j.h"
#include "x.h"

B
jtxoinit(J jt) {
    A x;

    GAT0(x, BOX, 8, 1);
    memset(AV(x), C0, AN(x) * SZI);
    ras(x);
    jt->fopa = x;
    GAT0(x, INT, 8, 1);
    ras(x);
    jt->fopf     = x;
    AM(jt->fopf) = 0;  // AM is # valid files
    return 1;
}

F
jtvfn(J jt, F x) {
    I* v = AV(jt->fopf);
    DQ(AM(jt->fopf), if (x == (F)*v++) return x;);
    ASSERT(0, EVFNUM);
}
/* check that x is in table of file#s */

I
jtfnum(J jt, A w) {
    A y;
    I h, j;
    if (AT(w) & B01 + INT) {
        ASSERT(h = jti0(jt, w), EVFNUM);
        return h;
    }
    ASSERT(AT(w) & BOX, EVDOMAIN);
    y = AAV(w)[0];
    ASSERT(AN(y), EVLENGTH);
    if (AT(y) & B01 + INT) {
        ASSERT(h = jti0(jt, y), EVFNUM);
        return h;
    }
    RE(j = jti0(jt, jtindexof(jt, jtvec(jt, BOX, AM(jt->fopf), AAV(jt->fopa)), boxW(jtfullname(jt, jtvslit(jt, y))))));
    return j < AM(jt->fopf) ? *(j + AV(jt->fopf)) : 0;
} /* file# corresp. to standard argument w */

A
jtfname(J jt, A w) {
    I j;
    RE(j = jti0(jt, jtindexof(jt, jt->fopf, w)));
    ASSERT(j < AM(jt->fopf), EVFNUM);
    return jtca(jt, *(j + AAV(jt->fopa)));
} /* string name corresp. to file# w */

A
jtjfiles(J jt, A w) {
    A y, z;
    ASSERTMTV(w);
    RZ(y = jtvec(jt, INT, AM(jt->fopf), AV(jt->fopf)));
    return jtgrade2(jt, jtstitch(jt, IRS1(y, 0, 0, jtbox, z), jtvec(jt, BOX, AM(jt->fopf), AV(jt->fopa))), y);
} /* file (number,name) table */

F
jtjope(J jt, A w, C* mode) {
    A t;
    F f;
    I n;
    static I nf = 25;
    A z;
    ASSERT(BOX & AT(w), EVDOMAIN);
    RZ(t = jtstr0(jt, jtvslit(jt, AAV(w)[0])));
    n = AN(t) - 1;
    ASSERT(n != 0, EVLENGTH);
    {
        C* cs = CAV(t);
        f     = fopen(cs, mode);
        if (!f && errno == ENOENT && !strcmp(mode, FUPDATE_O)) f = fopen(cs, FUPDATEC_O);
        if (!f && errno == EACCES && strcmp(mode, FREAD_O)) f = fopen(cs, FREAD_O);
    }
    return f ? f : (F)jtjerrno(jt);
}

A
jtjopen(J jt, A w) {
    A z;
    I h;
    if (!AN(w)) return w;
    if (AR(w)) return jtrank1ex0(jt, w, UNUSED_VALUE, jtjopen);
    RE(h = jtfnum(jt, w));
    if (h) {
        RZ(z = jtsc(jt, h));
        RZ(jtfname(jt, z));
        return z;
    } else {
        A ww;
        if (AM(jt->fopf) == AN(jt->fopf)) {
            I ct = AM(jt->fopf);
            RZ(jt->fopa = jtext(jt, 1, jt->fopa));
            RZ(jt->fopf = jtext(jt, 1, jt->fopf));
            AM(jt->fopf) = ct;
        }
        RZ(*(AM(jt->fopf) + IAV(jt->fopf)) = h = (I)jtjope(jt, w, FUPDATE_O));
        RZ(ww = jtfullname(jt, jtvslit(jt, AAV(w)[0])));
        RZ(ras(ww));
        RZ(*(AM(jt->fopf) + AAV(jt->fopa)) = ww);

        ++AM(jt->fopf);
        return jtsc(jt, h);
    }
} /* open the file named w if necessary; return file# */

B
jtadd2(J jt, F f1, F f2, C* cmd) {
    A c, x;
    if (f1 == NULL) {
        AM(jt->fopf) += 2;
        return 1;
    };
    GATV0(c, LIT, 1 + strlen(cmd), 1);
    memcpy(CAV(c) + 1, cmd, AN(c) - 1);
    cmd = CAV(c);
    if (AM(jt->fopf) + 1 >= AN(jt->fopf)) {
        I ct = AM(jt->fopf);
        RZ(jt->fopa = jtext(jt, 1, jt->fopa));
        RZ(jt->fopf = jtext(jt, 1, jt->fopf));
        AM(jt->fopf) = ct;
    }
    *cmd = '<';
    x    = jtcstr(jt, cmd);
    RZ(ras(x));
    RZ(*(AM(jt->fopf) + AAV(jt->fopa)) = x);
    RZ(*(AM(jt->fopf) + IAV(jt->fopf)) = (I)f1);
    *cmd = '>';
    x    = jtcstr(jt, cmd);
    RZ(ras(x));
    RZ(*(AM(jt->fopf) + AAV(jt->fopa) + 1) = x);
    RZ(*(AM(jt->fopf) + IAV(jt->fopf) + 1) = (I)f2);
    fa(c);
    return 1;
} /* add 2 entries to AM(jt->fopf) table (for hostio); null arg commits entries */

A
jtjclose(J jt, A w) {
    A* av;
    I *iv, j;
    if (!AN(w)) return w;
    if (AR(w)) return jtrank1ex0(jt, w, UNUSED_VALUE, jtjclose);
    RE(j = jti0(jt, jtindexof(jt, jt->fopf, jtsc(jt, jtfnum(jt, w)))));
    ASSERT(j < AM(jt->fopf), EVFNUM);
    av = AAV(jt->fopa);
    iv = IAV(jt->fopf);
    if (fclose((F)iv[j])) return jtjerrno(jt);
    --AM(jt->fopf);
    fa(av[j]);
    if (j < AM(jt->fopf)) {
        av[j] = av[AM(jt->fopf)];
        iv[j] = iv[AM(jt->fopf)];
    }
    return jtrue;
} /* close file# w */

F
jtstdf(J jt, A w) {
    A y;
    F f;
    I n, r, t;
    ASSERT(AN(w), EVLENGTH);
    ASSERT(!AR(w), EVRANK);
    if (BOX & AT(w)) {
        y = AAV(w)[0];
        t = AT(y);
        n = AN(y);
        r = AR(y);
        if (t & (LIT + C2T + C4T)) {
            ASSERT(1 >= r, EVRANK);
            ASSERT(n != 0, EVLENGTH);
            return 0;
        }
        /*!
          if(t&C2T){ASSERT(1>=r,EVRANK); ASSERT(n!=0,EVLENGTH); ASSERT(jtvc1(jt,n,USAV(y)),EVDOMAIN); return 0;}
             vc1 can now be killed off
        */
        if (t & B01 + INT) return jtstdf(jt, y);
        ASSERT(0, EVDOMAIN);
    }
    f = (F)jti0(jt, w);
    ASSERT(f != 0, EVFNUM);
    return f;
} /* 0 if w is a boxed file name; n if w is integer or boxed integer */
