/* Copyright 1990-2010, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Representations: Linear and Paren                                       */

#include "j.h"
#include <ctype.h>

static A jtlnoun(J jt, A w, A *ltext);
static A jtlnum(J jt, A w, A *ltext);
static A jtlrr(J jt, A w, A self, A *ltext);

#define NUMV(c) (((1LL << C9) | (1LL << CD) | (1LL << CS) | (1LL << CA) | (1LL << CN) | (1LL << CB)) & (1LL << (c)))

#define parfn ((I)jtinplace & JTPARENS ? jtlcpb : jtlcpa)
#define tiefn ((I)jtinplace & JTPARENS ? jtltieb : jtltiea)

static B
jtlp(J jt, A w) {
    FPREFIP;
    B b = 1, p = 0;
    C c, d, q = CQUOTE, *v;
    I j = 0, n;
    n   = AN(w);
    v   = CAV(w);
    c   = *v;
    d   = *(v + n - 1);
    if (1 == n || (2 == n || 3 >= n && ' ' == c) && (d == CESC1 || d == CESC2) || jtvnm(jt, n, v)) return 0;
    if (C9 == ctype[(UC)c])
        DQ(n - 1, d = c; c = ctype[(UC) * ++v]; if (b = !NUMV(c) || d == CS && c != C9) break;)
    else if (c == q)
        DQ(
          n - 1, c = *v++; p ^= (c == q); if (b = (p ^ 1) & (c != q)) { break; })
    else if (c == '(')
        DQ(n - 1, c = *v++; j += c == '(' ? 1 : c == ')' ? -1 : 0; if (b = !j) break;)
    return b;
} /* 1 iff put parens around w */

static A
jtlcpa(J jt, B b, A w) {
    FPREFIP;
    A z = w;
    C *zv;
    I n;
    if (b) {
        n = AN(w);
        GATV0(z, LIT, 2 + n, 1);
        zv  = CAV(z);
        *zv = '(';
        memcpy(1 + zv, AV(w), n);
        zv[1 + n] = ')';
    }
    return z;
} /* if b then (w) otherwise just w */

static A
jtlcpb(J jt, B b, A w) {
    FPREFIP;
    A z = w;
    B p;
    C c, *v, *wv, *zv;
    I n;
    n  = AN(w);
    wv = CAV(w);
    if (!b) {
        c = ctype[(UC)*wv];
        v = wv;
        p = 0;
        if (c == CQ)
            DQ(
              n - 1, c = ctype[(UC) * ++v]; p ^= (c == CQ); if (p & (c != CQ)) {
                  b = 1;
                  break;
              })
        else if (c == C9)
            DQ(
              n - 1, c = ctype[(UC) * ++v]; if (!(c == C9 || c == CS)) {
                  b = 1;
                  break;
              })
        else
            DQ(
              n - 1, c = *++v; if (!(c == CESC1 || c == CESC2)) {
                  b = 1;
                  break;
              });
        if (b && jtvnm(jt, n, wv)) b = 0;
    }
    if (b) {
        GATV0(z, LIT, 2 + n, 1);
        zv  = CAV(z);
        *zv = '(';
        memcpy(1 + zv, wv, n);
        zv[1 + n] = ')';
    }
    return z;
}

static A
jtlcpx(J jt, A w) {
    FPREFIP;
    return parfn(jtinplace, jtlp(jt, w), w);
}

static A
jtltiea(J jt, A w, A *ltext) {
    FPREFIP;
    A t, *v, *wv, x, y;
    B b;
    C c;
    I n;
    n  = AN(w);
    wv = AAV(w);
    RZ(t = jtspellout(jt, CGRAVE));
    GATV0(y, BOX, n + n, 1);
    v = AAV(y);
    DO(n, *v++ = i ? t : mtv; x = wv[i]; c = ID(x); RZ(x = lrr(x)); b = BETWEENC(c, CHOOK, CFORK) || i && jtlp(jt, x);
       RZ(*v++ = parfn(jtinplace, b, x)););
    return jtraze(jt, y);
}

static A
jtltieb(J jt, A w, A *ltext) {
    FPREFIP;
    A pt, t, *v, *wv, x, y;
    B b;
    C c, *s;
    I n;
    n  = AN(w);
    wv = AAV(w);
    RZ(t = jtspellout(jt, CGRAVE));
    RZ(pt = jtover(jt, jtscc(jt, ')'), t));
    GATV0(y, BOX, n + n, 1);
    v = AAV(y);
    if (1 >= n)
        x = mtv;
    else {
        GATV0(x, LIT, n - 2, 1);
        s = CAV(x);
        DQ(n - 2, *s++ = '(';);
    }
    DO(n, x = i == 1 ? t : x; x = i > 1 ? pt : x; *v++ = x; x = wv[i]; c = ID(x); RZ(x = lrr(x));
       b = BETWEENC(c, CHOOK, CFORK) || i && jtlp(jt, x);
       RZ(*v++ = parfn(jtinplace, b, x)););
    return jtraze(jt, y);
}

// return string for the shape: 's$'
static A
jtlsh(J jt, A w, A *ltext) {
    FPREFIP;
    return apip(jtthorn1(jt, shape(jt, w)), jtspellout(jt, CDOLLAR));
}

// return something to turn a list into the shape:
static A
jtlshape(J jt, A w, A *ltext) {
    FPREFIP;
    I r, *s;
    r = AR(w);
    s = AS(w);
    return 2 == r && (1 == s[0] || 1 == s[1]) ? jtspellout(jt, (C)(1 == s[1] ? CCOMDOT : CLAMIN))
           : !r                               ? mtv
           : 1 < r                            ? lsh(w)
           : 1 < AN(w)                        ? mtv
                                              : jtspellout(jt, CCOMMA);
}

static A
jtlchar(J jt, A w, A *ltext) {
    FPREFIP;
    A y;
    B b, p = 1, r1;
    C c, d, *u, *v;
    I j, k, m, n;
    m  = AN(ds(CALP));
    n  = AN(w);
    j  = n - m;
    r1 = 1 == AR(w);
    u = v = CAV(w);
    d     = *v;  // m=256, n=string length, j=n-256, r1 set if rank is 1, u=v->string, d=first char
    if (0 <= j && r1 && !memcmpne(v + j, AV(ds(CALP)), m)) {
        // string ends with an entire a. sequence
        if (!j) return jtcstr(jt, "a.");                          // if that's all there is, use a.
        RZ(y = lchar(1 == j ? jtscc(jt, *v) : jtstr(jt, j, v)));  // recur on the rest of the string to get its lr
        return jtlp(jt, y) ? jtover(jt, jtcstr(jt, "a.,~"), y)
                           : jtover(jt, y, jtcstr(jt, ",a."));  // use rest,a. or a.,~rest depending on () needs
    }
    if (r1 && m == n && (y = jticap(jt, ne(w, ds(CALP)))) &&
        m > AN(y)) {  // if 256-byte string, see where it differs from a.
        if (1 == AN(y)) RZ(y = jthead(jt, y));
        return jtover(jt,
                      jtover(jt, jtcstr(jt, "a. "), jtlcpx(jt, lnum(y))),
                      jtover(jt, jtcstr(jt, "}~"), lchar(jtfrom(jt, y, w))));  // use diff indx} a. or the like
    }
    // we will try for quoted string
    j = 2;
    b = 7 < n ||
        1 < n &&
          1 < AR(w);  // j will be # added chars (init 2 for outer quotes); look for repeated chars if 7 chars or rank>1
    DQ(n, c = *v++; j += c == CQUOTE; b &= c == d;
       p &=
       (C)(c - 32) < (C)(127 - 32););  // b=1 if all chars the same; p=1 if all printable; add to j for each quote found
    if (b) {
        n = 1;
        j = MIN(3, j);
    }          // if all repeated, back to 1 character, which j=2/3 dep whether it is a quote
    if (!p) {  // if the string contains a nonprintable, represent it as nums { a.
        k = (UC)d;
        RZ(y = jtindexof(jt, ds(CALP), w));
        if (r1 && n < m && (!k || k == m - n) && jtequ(jt, y, jtapv(jt, n, k, 1L)))
            return jtover(jt, jtthorn1(jt, jtsc(jt, d ? -n : n)), jtcstr(jt, "{.a."));
        RZ(y = lnum(y));
        return jtlp(jt, y) ? jtover(jt, jtcstr(jt, "a.{~"), y) : jtover(jt, y, jtcstr(jt, "{a."));
    }
    // out the enquoted string, preceded the the shape if repeated or not a list
    GATV0(y, LIT, n + j, 1);
    v  = CAV(y);
    *v = *(v + n + j - 1) = CQUOTE;
    ++v;
    if (2 == j)
        memcpy(v, u, n);
    else
        DQ(n, *v++ = c = *u++; if (c == CQUOTE) *v++ = c;);
    return jtover(jt, b ? lsh(w) : lshape(w), y);
} /* non-empty character array */

static A
jtlbox(J jt, A w, A *ltext) {
    FPREFIP;
    A p, *v, *vv, *wv, x, y;
    B b = 0;
    I n;
    if (jtequ(jt, ds(CACE), w) && B01 & AT(AAV(w)[0])) return jtcstr(jt, "a:");
    n  = AN(w);
    wv = AAV(w);
    DO(
      n, x = wv[i]; if (BOX & AT(x)) {
          b = 1;
          break;
      });
    b |= 1 == n;
    GATV0(y, BOX, n + n - (1 ^ b), 1);
    v = vv = AAV(y);
    if (b) {
        RZ(p = jtcstr(jt, "),(<"));
        DO(n, x = wv[i]; *v++ = p; RZ(*v++ = lnoun(x)););
        RZ(*vv = jtcstr(jt, 1 == n ? "<" : "(<"));
        if (1 < n) RZ(vv[n + n - 2] = jtcstr(jt, "),<"));
        return jtover(jt, lshape(w), jtraze(jt, y));
    }
    DO(
      n, x = wv[i]; if ((AR(x) ^ 1) | (~AT(x) & LIT)) {
          b = 1;
          break;
      });
    if (!b) {
        C c[256], d, *t;
        UC *s;
        memset(c, 1, sizeof(c));
        RZ(x = jtraze(jt, w));
        s = UAV(x);
        DQ(AN(x), c[*s++] = 0;);
        if (c[CQUOTE] && jtequ(jt, w, words(x))) return jtover(jt, jtcstr(jt, ";:"), lchar(x));
        if (c[d = ' '] || c[d = '|'] || c[d = '/'] || c[d = ','] || c[d = ';']) {
            GATV0(y, LIT, n + AN(x), 1);
            t = CAV(y);
            DO(n, x = wv[i]; *t++ = d; memcpy(t, AV(x), AN(x)); t += AN(x););
            RZ(y = lchar(y));
            return jtover(jt, lshape(w), jtover(jt, jtcstr(jt, isdigit(CAV(y)[0]) ? "<;.(_1) " : "<;._1 "), y));
        }
    }
    RZ(p = jtcstr(jt, ";"));
    DO(n - 1, RZ(*v++ = jtlcpx(jt, lnoun(wv[i]))); *v++ = p;);
    RZ(*v = lnoun(wv[n - 1]));
    return jtover(jt, lshape(w), jtraze(jt, y));
} /* non-empty boxed array */

A
jtdinsert(J jt, A w, A ic, I ix) {
    A l = jtsc4(jt, INT, ix);
    return jtover(jt, jtover(jt, jttake(jt, l, w), ic), jtdrop(jt, l, w));
} /* insert ic before position ix in w */
A
jtdcapp(J jt, A w, C c, A ap) {
    return (memchr(CAV(w), c, AN(w))) ? w : jtover(jt, w, ap);
} /* conditionally append ap to w if it doesn't contain c */

// Apply decoration as needed to a numeric character string w to give it the correct type t
// Result is A block for decorated string
A
jtdecorate(J jt, A w, I t) {
    if (AN(w) == 0) return w;  // if empty string, don't decorate
    if (t & FL) {
        // float: make sure there is a . somewhere, or infinity/indefinite ('_' followed by space/end/.), else put '.'
        // at end
        B needdot = !memchr(CAV(w), '.', AN(w));  // check for decimal point
        if (needdot) {
            DO(
              AN(w), if (CAV(w)[i] == '_' && (i == AN(w) - 1 || CAV(w)[i + 1] == ' ')) {
                  needdot = 0;
                  break;
              })
        }  // check for infinity
        if (needdot) {
            w = jtover(jt, w, jtscc(jt, '.'));
            RZ(w = jtmkwris(jt, w));
            DQ(
              AN(w), if (CAV(w)[i] == ' ') return w; if (CAV(w)[i] == 'e') {
                  C f  = '.';
                  C *s = &CAV(w)[i];
                  DO(AN(w) - i, C ff = s[i]; s[i] = f; f = ff;)
              })
        }
    } else if (t & INT) {
        // integer: if the string contains nothing but one-digit 0/1 values, insert '0' before last number
        I l  = AN(w);
        C *s = CAV(w);
        do {
            if ((*s & -2) != '0') break;
            ++s;
            if (--l == 0) break;
            if (*s != ' ') break;
            ++s;
        } while (--l);
        if (l == 0) {
            I ls = 0;
            DQ(AN(w), if (CAV(w)[i] == ' ') ls = i;);
            w = ls ? jtdinsert(jt, w, jtscc(jt, '0'), ls + 1) : jtover(jt, jtscc(jt, '0'), w);
        }
    } else if (t & XNUM)
        w = jtdcapp(jt,
                    w,
                    'x',
                    jtscc(jt, 'x'));  // extended: make sure there is an x somewhere in the string, else put 'x' at end
    else if (t & RAT)
        w =
          jtdcapp(jt,
                  w,
                  'r',
                  jtcstr(jt, "r1"));  // rational: make sure there is an r somewhere in the string, else put 'r1' at end
    else if (t & CMPX)
        w = jtdcapp(jt,
                    w,
                    'j',
                    jtcstr(jt, "j0"));  // complex: make sure there is a j somewhere in the string, else put "j0" at end
    return w;
}

static A
jtlnum1(J jt, A w, A *ltext) {
    FPREFIP;
    A z, z0;
    I t;
    t = AT(w);
    RZ(z = t & FL + CMPX ? df1(z0, w, jtfit(jt, ds(CTHORN), jtsc(jt, (I)18))) : jtthorn1(jt, w));
    return jtdecorate(jt, z, t);
} /* dense non-empty numeric vector */

static A
jtlnum(J jt, A w, A *ltext) {
    FPREFIP;
    A b, d, t, *v, y;
    B p;
    I n;
    RZ(t = jtravel(jt, w));
    n = AN(w);
    if (7 < n || 1 < n && 1 < AR(w)) {
        // see if we can use a clever encoding
        d = minus(jtfrom(jt, jtrue, t), b = jtfrom(jt, jfalse, t));
        p = jtequ(jt, t, plus(b, tymes(d, IX(n))));
        if (p) {
            if (jtequ(jt, d, jfalse)) return jtover(jt, lsh(w), lnum1(b));
            GAT0(y, BOX, 6, 1);
            v    = AAV(y);
            v[0] = v[1] = v[2] = v[3] = mtv;
            if (p = !(jtequ(jt, b, jtsc(jt, n - 1)) && jtequ(jt, d, num(-1)))) {
                if (!jtequ(jt, b, jfalse)) {
                    v[0] = lnum1(b);
                    v[1] = jtspellout(jt, CPLUS);
                }
                if (jtequ(jt, d, num(-1)))
                    v[1] = jtspellout(jt, CMINUS);
                else if (!jtequ(jt, d, jtrue)) {
                    v[2] = lnum1(d);
                    v[3] = jtspellout(jt, CSTAR);
                }
            }
            v[4] = jtspellout(jt, CIOTA);
            v[5] = jtthorn1(jt, p ? shape(jt, w) : jtnegate(jt, shape(jt, w)));
            RE(y);
            return jtraze(jt, y);
        }
        RESETERR;  // if there was an error getting to p, clear it
    }
    // not clever; just out the atoms
    return jtover(jt, lshape(w), lnum1(t));
} /* dense numeric non-empty array */

static A
jtlsparse(J jt, A w, A *ltext) {
    FPREFIP;
    A a, e, q, t, x, y, z;
    B ba, be, bn;
    I j, r, *v;
    P *p;
    r  = AR(w);
    p  = PAV(w);
    a  = SPA(p, a);
    e  = SPA(p, e);
    y  = SPA(p, i);
    x  = SPA(p, x);
    bn = 0;
    v  = AS(w);
    DQ(
      r, if (!*v++) {
          bn = 1;
          break;
      });
    ba = 0;
    if (r == AR(a)) {
        v = AV(a);
        DO(
          r, if (i != *v++) {
              ba = 1;
              break;
          });
    }
    be = !(AT(w) & SFL && 0 == *DAV(e));
    if (be)
        RZ(z = jtover(jt,
                      lnoun(e),
                      jtcstr(jt,
                             SB01 & AT(w)   ? ""
                             : SINT & AT(w) ? "+-~2"
                             : SFL & AT(w)  ? "+-~2.1"
                                            : "+-~2j1")));
    if (be || ba) {
        RZ(z = be ? jtover(jt, jtlcpx(jt, lnoun(a)), jtover(jt, jtscc(jt, ';'), z)) : lnoun(a));
        RZ(z = jtover(jt, jtlcpx(jt, lnoun(shape(jt, w))), jtover(jt, jtscc(jt, ';'), z)));
    } else
        RZ(z = lnoun(shape(jt, w)));
    RZ(z = jtover(jt, jtcstr(jt, "1$."), z));
    if (bn || !AS(y)[0]) return z;
    if (AN(a)) {
        RZ(x = jtlcpx(jt, lnoun(x)));
        RZ(y = 1 == r ? lnoun(jtravel(jt, y)) : jtover(jt, jtcstr(jt, "(<\"1)"), lnoun(y)));
        RZ(t = jtover(jt, x, jtover(jt, jtcstr(jt, " ("), jtover(jt, y, jtcstr(jt, ")}")))));
    } else
        RZ(t = jtover(jt, jtlcpx(jt, lnoun(jthead(jt, x))), jtcstr(jt, " a:}")));
    ba = 0;
    v  = AV(a);
    DO(
      AN(a), if (i != *v++) {
          ba = 1;
          break;
      });
    if (!ba) return jtover(jt, t, z);
    RZ(q = jtless(jt, IX(r), a));
    RZ(z = jtover(jt, jtover(jt, jtlcpx(jt, lnoun(q)), jtcstr(jt, "|:")), z));
    RZ(z = jtover(jt, t, z));
    RZ(q = jtgrade1(jt, jtover(jt, jtless(jt, IX(r), q), q)));
    j = r;
    v = AV(q);
    DO(
      r, if (i != *v++) {
          j = i;
          break;
      });
    return jtover(jt, jtlcpx(jt, lnoun(jtdrop(jt, jtsc(jt, j), q))), jtover(jt, jtcstr(jt, "|:"), z));
} /* sparse array */

static A
jtlnoun0(J jt, A w, A *ltext) {
    FPREFIP;
    A s, x;
    B r1;
    r1 = 1 == AR(w);
    RZ(s = jtthorn1(jt, shape(jt, w)));
    switch (CTTZ(AT(w))) {
        default: return apip(s, jtcstr(jt, "$00"));  // jtover(jt,jtcstr(jt,"i."),s);
        case LITX: x = jtcstr(jt, "''"); return r1 ? x : apip(apip(s, jtscc(jt, '$')), x);
        case C2TX: x = jtcstr(jt, "u: ''"); return r1 ? x : apip(apip(s, jtscc(jt, '$')), x);
        case C4TX: x = jtcstr(jt, "10&u: ''"); return r1 ? x : apip(apip(s, jtscc(jt, '$')), x);
        case BOXX: return apip(s, jtcstr(jt, "$a:"));
        case B01X: return apip(s, jtcstr(jt, "$0"));
        case FLX: return apip(s, jtcstr(jt, "$0."));
        case CMPXX: return apip(s, jtcstr(jt, "$0j0"));
        case XNUMX: return apip(s, jtcstr(jt, "$0x"));
        case RATX: return apip(s, jtcstr(jt, "$0r0"));
        case SBTX: return apip(s, jtcstr(jt, "$s: ' '"));
    }
} /* empty dense array */

static A
jtlnoun(J jt, A w, A *ltext) {
    FPREFIP;
    I t;
    t = AT(w);
    if ((t & SPARSE) != 0) return lsparse(w);
    if (!AN(w)) return lnoun0(w);
    switch (CTTZ(t)) {
        case LITX: return lchar(w);
        case C2TX: return jtover(jt, jtcstr(jt, "u: "), lnum(jtuco2(jt, num(3), w)));
        case C4TX: return jtover(jt, jtcstr(jt, "10&u: "), lnum(jtuco2(jt, num(3), w)));
        case BOXX: return lbox(w);
        case SBTX: return jtover(jt, jtcstr(jt, "s: "), lbox(jtsb2(jt, num(5), w)));
        default: return lnum(w);
    }
}

static A
jtlsymb(J jt, C c, A w, A *ltext) {
    FPREFIP;
    A t;
    C buf[20], d, *s;
    I *u;
    V *v = FAV(w);
    if (VDDOP & v->flag) {
        u    = AV(v->fgh[2]);
        s    = buf;
        *s++ = ' ';
        *s++ = '(';
        s += sprintf(s, FMTI, *u);
        spellit(CIBEAM, s);
        s += 2;
        s += sprintf(s, FMTI, u[1]);
        *s++ = ')';
        RZ(t = jtstr(jt, s - buf, buf));
    } else {
        RZ(t = jtspella(jt, w));
        if (AN(t) == 1 && (CAV(t)[0] == '{' || CAV(t)[0] == '}')) {
            RZ(t = jtmkwris(jt, t));
            AS(t)[0] = AN(t) = 2;
            CAV(t)[1]        = ' ';
        }
    }  // add trailing space to { } to avoid DD codes
    d = cf(t);
    return d == CESC1 || d == CESC2 ? jtover(jt, chrspace, t) : t;
}

static B
laa(A a, A w) {
    C c, d;
    if (!(a && w)) return 0;
    c = ctype[(UC)cl(a)];
    d = ctype[(UC)cf(w)];
    return ((c | d) & (0xf & ~(CA | C9))) ^ 1;  // 1 if c,d both alphameric
}

// Is a string a number?  Must start with a digit and end with digit, x, or .
static B
lnn(A a, A w) {
    C c;
    if (!(a && w)) return 0;
    c = cl(a);
    return ('x' == c || '.' == c || C9 == ctype[(UC)c]) && C9 == ctype[(UC)cf(w)];
}

static A
jtlinsert(J jt, A a, A w, A *ltext) {
    FPREFIP;
    A *av, f, g, h, t, t0, t1, t2, *u, y;
    B b, ft, gt, ht;
    C c, id;
    I n;
    V *v;
    n      = AN(a);
    av     = AAV(a);
    v      = VAV(w);
    id     = v->id;
    b      = id == CCOLON && VXOP & v->flag;
    I fndx = (id == CBDOT) && !v->fgh[0];
    A fs   = v->fgh[fndx];
    A gs =
      v->fgh[fndx ^
             1];  // In verb for m b., if f is empty look to g for the left arg.  It would be nice to be more general
                  // ?t tells whether () is needed around the f/g/h component
    if (1 <= n) {
        f  = av[0];
        t  = fs;
        c  = ID(t);
        ft = BETWEENC(c, CHOOK, CADVF) || (b || id == CFORK) && NOUN & AT(t) && jtlp(jt, f);
    }  // f: () if it's hook fork && or noun left end of nvv or n (op)
    if (2 <= n) {
        g  = av[1];
        t  = gs;
        c  = ID(t);
        gt = VERB & AT(w) ? BETWEENC(c, CHOOK, CFORK) : jtlp(jt, g);
    }
    if (3 <= n) {
        h  = av[2];
        t  = v->fgh[2];
        c  = ID(t);
        ht = VERB & AT(w) && !b ? c == CHOOK : jtlp(jt, h);
    }
    switch (!b ? id : 2 == n ? CHOOK : CFORK) {
        case CADVF:
        case CHOOK:
            GAT0(y, BOX, 3, 1);
            u    = AAV(y);
            u[0] = f = parfn(jtinplace, ft || lnn(f, g), f);
            u[2] = g = parfn(jtinplace, gt || b, g);
            u[1] = jtstr(jt, ' ' == cf(g) || id == CADVF && !laa(f, g) && !(jtlp(jt, f) && jtlp(jt, g)) ? 0L : 1L, " ");
            RE(0);
            return jtraze(jt, y);
        case CFORK:
            GAT0(y, BOX, 5, 1);
            u = AAV(y);
            RZ(u[0] = f = parfn(jtinplace, ft || lnn(f, g), f));
            RZ(u[2] = g = parfn(jtinplace, gt || lnn(g, h) || b, g));
            RZ(u[1] = jtstr(jt, ' ' == cf(g) ? 0L : 1L, " "));
            RZ(u[4] = h = parfn(jtinplace, ht, h));
            RZ(u[3] = jtstr(jt, ' ' == cf(h) ? 0L : 1L, " "));
            return jtraze(jt, y);
        default:
            t0 = parfn(jtinplace, ft || NOUN & AT(fs) && !(VGERL & v->flag) && jtlp(jt, f), f);
            t1 = lsymb(id, w);
            y  = jtover(jt, t0, laa(t0, t1) ? jtover(jt, chrspace, t1) : t1);
            if (1 == n) return y;
            t2 = jtlcpx(jt, g);
            return jtover(jt, y, laa(y, t2) ? jtover(jt, chrspace, t2) : t2);
    }
}

// create linear rep for m : n
static A
jtlcolon(J jt, A w, A *ltext) {
    FPREFIP;
    A *v, x, y;
    C *s, *s0;
    I m, n;
    RZ(y = jtunparsem(jt, jtrue, w));
    n = AN(y);
    v = AAV(y);
    RZ(x = lrr(VAV(w)->fgh[0]));
    if (2 > n || 2 == n && 1 == AN(v[0]) && ':' == CAV(v[0])[0]) {
        if (!n) return jtover(jt, x, jtstr(jt, 5L, " : \'\'"));
        y = lrr(v[2 == n]);
        if (2 == n) y = jtover(jt, jtstr(jt, 5L, "\':\'; "), y);
        return jtover(jt, jtover(jt, x, jtstr(jt, 3L, " : ")), jtlcpx(jt, y));
    }
    m = 0;
    DO(n, m += AN(v[i]););
    GATV0(y, LIT, 2 + n + m, 1);
    s = s0 = CAV(y);
    DO(n, *s++ = CLF; y = v[i]; m = AN(y); memcpy(s, CAV(y), m); s += m;);
    *s++ = CLF;
    *s++ = ')';
    RZ(y = jtstr(jt, s - s0, s0));
    *ltext = *ltext ? jtover(jt, *ltext, y) : y;
    return jtover(jt, x, jtstr(jt, 4L, " : 0"));
}

// Main routine for () and linear rep.  w is to be represented
static A
jtlrr(J jt, A w, A self, A *ltext) {
    FPREFIP;
    A hs, t, *tv;
    C id;
    I fl, m;
    V *v;
    if (!w) return 0;
    // If name, it must be in ".@'name', or (in debug mode) the function name, which we will discard
    if (AT(w) & NAME) { RZ(w = jtsfn(jt, 0, w)); }
    if (AT(w) & NOUN) return lnoun(w);
    v  = VAV(w);
    id = v->id;  // outer verb, & its id
    // if f is 0, we take f from g.  In other words, adverbs can put their left arg in either f or g.  u b. uses g so
    // that it can leave f=0 to allow it to function as an ATOMIC2 op
    I fndx = (id == CBDOT) && !v->fgh[0];
    A fs   = v->fgh[fndx];
    A gs =
      v->fgh[fndx ^
             1];  // In verb for m b., if f is empty look to g for the left arg.  It would be nice to be more general
    hs = v->fgh[2];
    fl = v->flag;
    if (id == CBOX) gs = 0;             // ignore gs field in BOX, there to simulate BOXATOP
    if (fl & VXOPCALL) return lrr(hs);  // pseudo-named entity created during debug of operator.  The defn is in h
    m = (I) !!fs + (I)(gs && id != CBOX) + (I)(id == CFORK) +
        (I)(hs && id == CCOLON && VXOP & fl);  // BOX has g for BOXATOP; ignore it; get # nonzero values in f g h
    if (!m) return lsymb(id, w);               // if none, it's a primitive, out it
    if (evoke(w)) {
        RZ(w = jtsfne(jt, w));
        if (FUNC & AT(w)) w = lrr(w);
        return w;
    }  // keep named verb as a string, UNLESS it is NMDOT, in which case use the (f.'d) verb value
    if (!(VXOP & fl) && hs && BOX & AT(hs) && id == CCOLON) return lcolon(w);  // x : boxed - must be explicit defn
    GATV0(t, BOX, m, 1);
    tv = AAV(t);
    if (2 < m) RZ(tv[2] = jtincorp(jt, lrr(hs)));  // fill in h if present
    // for top-level of gerund (indicated by self!=0), any noun type could not have come from an AR, so return it as is
    if (1 < m)
        RZ(tv[1] = jtincorp(jt,
                            fl & VGERR ? tiefn(jtinplace, jtfxeach(jt, gs, (A)&jtfxself[!!self]), ltext)
                                       : lrr(gs)));  // fill in g if present
    if (0 < m)
        RZ(tv[0] = jtincorp(jt,
                            fl & VGERL ? tiefn(jtinplace, jtfxeach(jt, fs, (A)&jtfxself[!!self]), ltext)
                                       : lrr(fs)));  // fill in f (always present)
    return linsert(t, w);
}

// Create linear representation of w.  Call lrr, which creates an A for the text plus jt->ltext which is appended to it.
// jt->lcp and jt->ltie are routines for handling adding enclosing () and handling `
A
jtlrep(J jt, A w) {
    PROLOG(0056);
    A z;
    A ltextb = 0, *ltext = &ltextb;
    RE(z = jtlrr(jt, w, w, ltext));  // the w for self is just any nonzero to indicate top-level call
    if (*ltext) z = apip(z, *ltext);
    EPILOG(z);
}

// Create paren representation of w.  Call lrr, which creates an A for the text plus jt->ltext which is appended to it.
// jt->lcp and jt->ltie are routines for handling adding enclosing () and handling `
A
jtprep(J jt, A w) {
    PROLOG(0057);
    A z;
    A ltextb = 0, *ltext = &ltextb;
    RE(z = jtlrr((J)((I)jt | JTPARENS), w, w, ltext));
    if (*ltext) z = apip(z, *ltext);
    EPILOG(z);
}
