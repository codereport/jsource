/* Copyright 1990-2008, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Adverbs: f/ defns                                                       */

// n=#items in a CELL of w
// m=#cells to operate on
// c=#atoms in a cell
// d=#atoms in an item of a cell of w (thus c=d*n)

#define REDUCEPFX(f, Tz, Tx, pfx, vecfn1, vecfnn)                                          \
    I f(I d, I n, I m, Tx* x, Tz* z, J jt) {                           \
        I i;                                                                               \
        Tz v;                                                                              \
        if (d == 1) {                                                                      \
            x += m * n;                                                                    \
            z += m;                                                                        \
            DQ(m, v = *--x; DQ(n - 1, --x; v = pfx(*x, v);); *--z = v;)                    \
        }                                                                                  \
                                                                                           \
        else {                                                                             \
            z += (m - 1) * d;                                                              \
            x += (m * n - 1) * d;                                                          \
            for (i = 0; i < m; ++i, z -= d) {                                              \
                I rc;                                                                      \
                Tx* y = x;                                                        \
                x -= d;                                                                    \
                if (255 & (rc = vecfn1(1, d, x, y, z, jt))) return rc;                     \
                x -= d;                                                                    \
                DQ(n - 2, if (255 & (rc = vecfnn(1, d, x, z, z, jt))) return rc; x -= d;); \
            }                                                                              \
        }                                                                                  \
        return EVOK;                                                                       \
    }

// used on idempotent verbs, using 2 accumulators
#define REDUCEPFXIDEM2(f, Tz, Tx, pfx, vecfn)                                     \
    I f(I d, I n, I m, Tx* x, Tz* z, J jt) {                  \
        I i;                                                                      \
        if (d == 1) {                                                             \
            x += m * n;                                                           \
            z += m;                                                               \
            DQ(m, Tz v0 = (Tz) * --x; Tz v1 = v0; if (!(n & 1)) v1 = *--x;        \
               DQ((n - 1) >> 1, x -= 2; v0 = pfx(x[0], v0); v1 = pfx(x[1], v1);); \
               v0   = pfx(v0, v1);                                                \
               *--z = v0;)                                                        \
        }                                                                         \
                                                                                  \
        else {                                                                    \
            z += (m - 1) * d;                                                     \
            x += (m * n - 1) * d;                                                 \
            for (i = 0; i < m; ++i, z -= d) {                                     \
                Tx* y = x;                                               \
                x -= d;                                                           \
                vecfn(1, d, x, y, z, jt);                                         \
                x -= d;                                                           \
                DQ(n - 2, vecfn(1, d, x, z, z, jt); x -= d;);                     \
            }                                                                     \
        }                                                                         \
        return EVOK;                                                              \
    }

// used on idempotent verbs, using 4 accumulators but using the 256-bit instructions if available
#define REDUCEPFXIDEM2PRIM256(f, Tz, Tx, pfx, vecfn, prim, identity) REDUCEPFXIDEM2(f, Tz, Tx, pfx, vecfn)

#define REDUCENAN(f, Tz, Tx, pfx, vecfn)                                                  \
    I f(I d, I n, I m, Tx* x, Tz* z, J jt) {                          \
        I i;                                                                              \
        Tz v;                                                                             \
        NAN0;                                                                             \
        if (d == 1) {                                                                     \
            x += m * n;                                                                   \
            z += m;                                                                       \
            DQ(m, v = *--x; DQ(n - 1, --x; v = pfx(*x, v);); *--z = v;)                   \
        }                                                                                 \
                                                                                          \
        else {                                                                            \
            z += (m - 1) * d;                                                             \
            x += (m * n - 1) * d;                                                         \
            for (i = 0; i < m; ++i, z -= d) {                                             \
                I rc;                                                                     \
                Tx* y = x;                                                       \
                x -= d;                                                                   \
                if (255 & (rc = vecfn(1, d, x, y, z, jt))) return rc;                     \
                x -= d;                                                                   \
                DQ(n - 2, if (255 & (rc = vecfn(1, d, x, z, z, jt))) return rc; x -= d;); \
            }                                                                             \
        }                                                                                 \
        return NANTEST ? EVNAN : EVOK;                                                    \
    }

#define REDUCCPFX(f, Tz, Tx, pfx)                                             \
    I f(I d, I n, I m, Tx* x, Tz* z, J jt) {              \
        I i;                                                                  \
        Tx* y;                                                       \
        Tz v, *zz;                                                   \
        if (d == 1) {                                                         \
            x += m * n;                                                       \
            z += m;                                                           \
            DQ(m, v = (Tz) * --x; DQ(n - 1, --x; v = pfx(*x, v);); *--z = v;) \
        }                                                                     \
                                                                              \
        else {                                                                \
            zz = z += m * d;                                                  \
            x += m * d * n;                                                   \
            for (i = 0; i < m; ++i, zz -= d) {                                \
                y = x;                                                        \
                x -= d;                                                       \
                z = zz;                                                       \
                DQ(d, --z; --x; --y; *z = pfx(*x, *y););                      \
                DQ(n - 2, z = zz; DQ(d, --z; --x; *z = pfx(*x, *z);));        \
            }                                                                 \
        }                                                                     \
        return EVOK;                                                          \
    }

#define REDUCEOVF(f, Tz, Tx, fr1, fvv, frn)                    \
    I f(I d, I n, I m, I* x, I* z, J jt) { \
        I er = EVOK;                                           \
        I i, *xx, *y, *zz;                   \
        if (d == 1) {                                          \
            xx = x;                                            \
            zz = z;                                            \
            DQ(m, z = zz++; x = xx; fr1(n, z, x); xx += n;);   \
            return er;                                         \
        }                                                      \
        if (1 == n) {                                          \
            if (sizeof(Tz) != sizeof(Tx)) {                    \
                DQ(d, *z++ = *x++;)                            \
            } else {                                           \
                memcpy((C*)z, (C*)x, d * sizeof(Tz));          \
            }                                                  \
            return er;                                         \
        }                                                      \
        zz = z += m * d;                                       \
        xx = x += m * d * n;                                   \
        xx -= d;                                               \
        zz -= d;                                               \
        for (i = 0; i < m; ++i, xx -= d, zz -= d) {            \
            y = xx;                                            \
            x = xx -= d;                                       \
            z = zz;                                            \
            fvv(d, z, x, y);                                   \
            DQ(n - 2, x = xx -= d; z = zz; frn(d, z, x););     \
        }                                                      \
        return er;                                             \
    }
