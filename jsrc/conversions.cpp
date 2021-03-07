/* Copyright 1990-2010, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Conversions Amongst Internal Types                                      */

#include <algorithm>
#include <limits>
#include <numeric>

#include "array.hpp"
extern "C" {
#include "verbs/vcomp.h"
}

#define CVCASE(a, b) (((a) << 3) + (b))  // The main cases fit in low 8 bits of mask

// FIEQ are used in bcvt, where FUZZ may be set to 0 to ensure only exact values are demoted to lower precision
#define FIEQ(u, v, fuzz) \
    (ABS((u) - (v)) <=   \
     fuzz *              \
       ABS(v))  // used when v is known to be exact integer.  It's close enough, maybe ULP too small on the high end

template <typename T, typename V>
[[nodiscard]] constexpr auto
in_range(V value) -> bool {
    return std::numeric_limits<T>::min() <= value && value <= std::numeric_limits<T>::max();
}

template <typename T, typename V>
[[nodiscard]] constexpr auto
in_range() -> bool {
    return in_range<T>(std::numeric_limits<V>::min()) && in_range<T>(std::numeric_limits<V>::max());
}

template <typename From, typename To>
[[nodiscard]] auto
convert(J jt, array w, void *yv) -> bool {
    auto *v = pointer_to_values<From>(w);
    if constexpr (!in_range<To, From>()) {
        // TODO: replace with short circuiting solution
        auto out = static_cast<To *>(yv);
        return out + AN(w) == std::copy_if(v, v + AN(w), out, [](auto v) { return in_range<To>(v); });
    }
    std::copy(v, v + AN(w), static_cast<To *>(yv));
    return 1;
}

template <typename From, typename To, typename Transform>
[[nodiscard]] auto
convert(J jt, array w, void *yv, Transform t) -> bool {
    auto *v = pointer_to_values<From>(w);
    std::transform(v, v + AN(w), static_cast<To*>(yv), t);
    return 1;
}

template <>
[[nodiscard]] auto
convert<D, bool>(J jt, A w, void *yv, D fuzz) -> bool {
    auto n = AN(w);
    auto v = pointer_to_values<double>(w);
    auto x = (B *)yv;
    DQ(n, auto p = *v++; if (p < -2 || 2 < p) return 0;  // handle infinities
       I val = 2;
       val   = (p == 0) ? 0 : val;
       val   = FIEQ(p, 1.0, fuzz) ? 1 : val;
       if (val == 2) return 0;
       *x++ = (B)val;)
    return 1;
}

template <>
[[nodiscard]] auto
convert<D, I>(J jt, A w, void *yv, D fuzz) -> bool {
    auto n = AN(w);
    auto v = pointer_to_values<double>(w);
    auto x = (I *)yv;
    for (int64_t i = 0; i < n; ++i) {
        auto const p = v[i];
        auto const q = jround(p);
        I rq         = (I)q;
        if (!(p == q || FIEQ(p, q, fuzz))) return 0;  // must equal int, possibly out of range
        // out-of-range values don't convert, handle separately
        if (p < (D)IMIN) {
            if (!(p >= IMIN * (1 + fuzz))) return 0;
            rq = IMIN;
        }  // if tolerantly < IMIN, error; else take IMIN
        else if (p >= FLIMAX) {
            if (!(p <= -(D)IMIN * (1 + fuzz))) return 0;
            rq = IMAX;
        }  // if tolerantly > IMAX, error; else take IMAX
        *x++ = rq;
    }
    return 1;
}

template <>
[[nodiscard]] auto
convert<Z, D>(J jt, A w, void *yv, D fuzz) -> bool {
    auto const n  = AN(w);
    auto const *v = pointer_to_values<Z>(w);
    auto x        = (D *)yv;
    if (fuzz)
        DQ(
          n, auto d = std::abs(v->im); if (d != inf && d <= fuzz * std::abs(v->re)) {
              *x++ = v->re;
              v++;
          } else return 0;)
    else
        DQ(
          n, if (!v->im) {
              *x++ = v->re;
              v++;
          } else return 0;);
    return 1;
}

template <>
[[nodiscard]] auto
convert<bool, X>(J jt, A w, void *yv) -> bool {
    return convert<B, X>(jt,
                         w,
                         yv,
                         [=](auto v) {
                             int64_t u[] = {v};
                             return jtvec(jt, INT, 1L, u);
                         }) &&
           !jt->jerr;
}

template <typename T>
static auto
inplace_negate(T *u, int64_t n) {
    std::transform(u, u + n, u, [](auto v) { return -v; });
}

template <>
[[nodiscard]] auto
convert<I, X>(J jt, A w, void *yv) -> bool {
    I u[XIDIG];
    auto const convert_one = [&](auto c) {
        auto const b   = c == IMIN;
        auto d         = b ? -(1 + c) : std::abs(c);
        int64_t length = 0;
        for (int64_t i = 0; i < XIDIG; ++i) {
            u[i] = d % XBASE;
            d    = d / XBASE;
            if (u[i]) length = i;
        }
        ++length;
        *u += b;
        if (0 > c) inplace_negate(u, XIDIG);
        return jtvec(jt, INT, length, u);
    };
    return convert<I, X>(jt, w, yv, convert_one) && !jt->jerr;
}

static X
jtxd1(J jt, D p, I mode) {
    PROLOG(0052);
    D e = jttfloor(jt, p);
    switch (mode) {
        case XMFLR: p = e; break;
        case XMCEIL: p = ceil(p); break;
        case XMEXACT:
            ASSERT(TEQ(p, e), EVDOMAIN);
            p = e;
            break;
        case XMEXMT:
            if (!TEQ(p, e)) return jtvec(jt, INT, 0L, &iotavec[-IOTAVECBEGIN]);
    }
    if (p == inf) return jtvci(jt, XPINF);
    if (p == -inf) return jtvci(jt, XNINF);
    A t;
    GAT0(t, INT, 30, 1);
    auto u = pointer_to_values(t);
    int64_t m = 0;
    auto d = std::abs(p);
    while (0 < d) {
        auto const q = floor(d / XBASE);
        auto const r = d - q * XBASE;
        u[m++]       = (I)r;
        d            = q;
        if (m == AN(t)) {
            RZ(t = jtext(jt, 0, t));
            u = pointer_to_values(t);
        }
    }
    if (!m) {
        u[0] = 0;
        ++m;
    } else if (0 > p) {
        inplace_negate(u, m);
    }
    A z = jtxstd(jt, jtvec(jt, INT, m, u));
    EPILOG(z);
}

template <>
[[nodiscard]] auto
convert<D, X>(J jt, A w, void *yv, I mode) -> bool {
    return convert<D, X>(jt, w, yv, [=](auto v){ return jtxd1(jt, v, mode); }) && !jt->jerr;
}

template <>
[[nodiscard]] auto
convert<X, bool>(J jt, A w, void *yv) -> bool {
    auto v = pointer_to_values<X>(w);
    auto x = (B *)yv;
    DO(AN(w), A q = v[i]; I e = pointer_to_values(q)[0]; if ((AN(q) ^ 1) | (e & -2)) return 0; x[i] = (B)e;);
    return 1;
}

template <typename T>
[[nodiscard]] static auto
value_from_X(X p) -> T {
        auto const n = AN(p);
        auto const v = std::reverse_iterator(pointer_to_values(p) + n);
        return std::accumulate(v, v + n, T{}, [](auto d, auto v) { return v + d * XBASE; });
}

template <>
[[nodiscard]] auto
convert<X, I>(J jt, A w, void *yv) -> bool {
    auto v = pointer_to_values<X>(w);
    auto x = (I *)yv;
    auto n = AN(w);
    X p, q;
    if (!(p = jtxc(jt, IMAX))) return 0;
    if (!(q = jtxminus(jt, jtnegate(jt, p), jtxc(jt, 1L)))) return 0;
    for (int64_t i = 0; i < n; ++i) {
        auto c = v[i];
        if (!(1 != jtxcompare(jt, q, c) && 1 != jtxcompare(jt, c, p))) return 0;
        x[i] = value_from_X<int64_t>(c);
    }
    return 1;
}

template <>
[[nodiscard]] auto
convert<X, D>(J jt, A w, void *yv) -> bool {
    return convert<X, D>(jt, w, yv, [](auto p) {
        auto const c = pointer_to_values(p)[AN(p) - 1];
        if (c == XPINF) return inf;
        if (c == XNINF) return infm;
        return value_from_X<double>(p);
    });
}

template <>
[[nodiscard]] auto
convert<X, Q>(J jt, A w, void *yv) -> bool {
    return convert<X, Q>(jt, w, yv, [](auto v) -> Q { return {v, iv1}; });
}

template <>
[[nodiscard]] auto
convert<D, Q>(J jt, A w, void *yv, I mode) -> bool {
    if (!(w)) return 0;
    auto const n  = AN(w);
    auto const wv = pointer_to_values<double>(w);
    auto x        = (Q *)yv;
    D t;
    auto tv = 3 + (S *)&t;
    Q q;
    for (int64_t i = 0; i < n; ++i) {
        t = wv[i];
        ASSERT(!_isnan(t), EVNAN);
        bool const neg = 0 > t;
        if (neg) t = -t;
        q.d = iv1;
        if (t == inf)
            q.n = jtvci(jt, XPINF);
        else if (t == 0.0)
            q.n = iv0;
        else if (1.1102230246251565e-16 < t && t < 9.007199254740992e15) {
            auto const d = jround(1 / jtdgcd(jt, 1.0, t));
            auto const c = jround(d * t);
            q            = jtqstd(jt,
                       {
                         .n = jtxd1(jt, c, mode),
                         .d = jtxd1(jt, d, mode),
                       });
        } else {
            bool const recip = 1 > t;
            if (recip) t = 1.0 / t;
            auto e = (I)(0xfff0 & *tv);
            e >>= 4;
            e -= 1023;
            if (recip) {
                q.d = jtxtymes(jt, jtxd1(jt, t / pow(2.0, e - 53.0), mode), jtxpow(jt, jtxc(jt, 2L), jtxc(jt, e - 53)));
                q.n = jtca(jt, iv1);
            } else {
                q.n = jtxtymes(jt, jtxd1(jt, t / pow(2.0, e - 53.0), mode), jtxpow(jt, jtxc(jt, 2L), jtxc(jt, e - 53)));
                q.d = jtca(jt, iv1);
            }
        }
        if (neg) inplace_negate(pointer_to_values(q.n), AN(q.n));
        *x++ = q;
    }
    return !jt->jerr;
}

template <>
[[nodiscard]] auto
convert<Q, D>(J jt, A w, void *yv) -> bool {
    auto const xb = (D)XBASE;
    auto const wn = AN(w);
    auto const wv = pointer_to_values<Q>(w);
    auto const x  = (D *)yv;
    auto const nn = 308 / XBASEN;

    // TODO: figure out nice algorithm for this
    auto const add_digits = [&](auto n, auto v) {
        auto f = 1.0;
        auto d = 0.0;
        DO(n, d += f * v[i]; f *= xb;);
        return d;
    };

    X x2 = 0;
    for (int64_t i = 0; i < wn; ++i) {
        auto const p  = wv[i].n;
        auto const pn = AN(p);
        auto const k  = 1 == pn ? pointer_to_values(p)[0] : 0;
        auto const q  = wv[i].d;
        auto const qn = AN(q);
        if (k == XPINF)
            x[i] = inf;
        else if (k == XNINF)
            x[i] = infm;
        else if (pn <= nn && qn <= nn) {
            auto const n = add_digits(pn, pointer_to_values(p));
            auto const d = add_digits(qn, pointer_to_values(q));
            x[i]         = n / d;
        } else {
            if (!x2)
                if (!(x2 = jtxc(jt, 2L))) return 0;
            auto const k = 5 + qn;
            auto c       = jtxdiv(jt, jttake(jt, jtsc(jt, -(k + pn)), p), q, XMFLR);
            if (!c) return 0;
            auto const cn = AN(c);
            auto const m  = MIN(cn, 5);
            auto const r  = cn - (m + k);
            auto const v  = pointer_to_values(c) + cn - m;
            auto const n  = add_digits(m, v);
            auto d        = std::pow(xb, std::abs(r));
            x[i]          = 0 > r ? n / d : n * d;
        }
    }
    return 1;
}

template <>
[[nodiscard]] auto
convert<Q, X>(J jt, A w, void *yv) -> bool {
    auto v = pointer_to_values<Q>(w);
    auto x = (X *)yv;
    DQ(AN(w), if (!(jtequ(jt, iv1, v->d))) return 0; *x++ = v->n; ++v;);
    return !jt->jerr;
}

template <typename T>
auto
set_real_part(Z *z, int64_t n, T *t) {
    for (int64_t i = 0; i < n; ++i) z[i].re = t[i];
}

// Imaginary parts have already been cleared
template <>
[[nodiscard]] auto
convert<D, Z>(J jt, A w, void *yv) -> bool {
    set_real_part(static_cast<Z*>(yv), AN(w), pointer_to_values<double>(w));
    return 1;
}

// Convert the data in w to the type t.  w and t must be noun types.  A new buffer is always created (with a
// copy of the data if w is already of the right type), and returned in *y.  Result is
// 0 if error, 1 if success.  If the conversion loses precision, error is returned
// Calls through bcvt are tagged with a flag in jt, indicating to set fuzz=0
B
jtccvt(J jt, I tflagged, A w, A *y) {
    FPREFIP;
    I const t = tflagged & NOUN;
    if (!w) return 0;
    auto const r = AR(w);
    auto const s = AS(w);
    if (((t | AT(w)) & SPARSE) != 0) {
        // Handle sparse
        RANK2T oqr = jt->ranks;
        RESETRANK;
        switch ((t & SPARSE ? 2 : 0) + (AT(w) & SPARSE ? 1 : 0)) {
            I t1;
            P *wp, *yp;
            case 1: RZ(w = jtdenseit(jt, w)); break;  // sparse to dense
            case 2:
                RZ(*y = jtsparseit(jt, jtcvt(jt, DTYPE(t), w), IX(r), jtcvt(jt, DTYPE(t), jfalse)));
                jt->ranks = oqr;
                return 1;  // dense to sparse; convert type first (even if same dtype)
            case 3:        // sparse to sparse
                t1 = DTYPE(t);
                GASPARSE(*y, t, 1, r, s);
                yp = pointer_to_values<P>(*y);
                wp = pointer_to_values<P>(w);
                SPB(yp, a, jtca(jt, SPA(wp, a)));
                SPB(yp, i, jtca(jt, SPA(wp, i)));
                SPB(yp, e, jtcvt(jt, t1, SPA(wp, e)));
                SPB(yp, x, jtcvt(jt, t1, SPA(wp, x)));
                jt->ranks = oqr;
                return 1;
        }
        jt->ranks = oqr;
    }
    // Now known to be non-sparse
    auto const wt = AT(w);
    // If type is already correct, return a clone - used to force a copy.  Should get rid of this kludge
    if (TYPESEQ(t, wt)) {
        RZ(*y = jtca(jt, w));
        return 1;
    }
    // Kludge on behalf of result assembly: we want to be able to stop converting after the valid cells.  If
    // NOUNCVTVALIDCT is set in the type, we use the input *y as as override on the # cells to convert.  We use it to
    // replace n (for use here) and yv, and AK(w) and AN(w) for the subroutines. If NOUNCVTVALIDCT is set, w is
    // modified: the caller must restore AN(w) and AK(w) if it needs it
    // TODO: same-length conversion could be done in place
    auto n = AN(w);
    A d;
    GA(d, t, n, r, s);
    auto yv = pointer_to_values<void>(d);  // allocate the same # atoms, even if we will convert fewer
    if (tflagged & NOUNCVTVALIDCT) {
        I inputn = *(I *)y;  // fetch input, in case it is called for
        if (inputn > 0) {    // if converting the leading values, just update the counts
            n = inputn;      // set the counts for local use, and in the block to be converted
        } else {             // if converting trailing values...
            AK(w) += (n + inputn) << bplg(wt);
            yv = (I *)((C *)yv + ((n + inputn) << bplg(t)));  // advance input and output pointers to new area
            n  = -inputn;                                     // get positive # atoms to convert
        }
        AN(w) = n;  // change atomct of w to # atoms to convert
    }
    // If n and AN have been modified, it doesn't matter for rank-1 arguments whether the shape of the result is listed
    // as n or s[0] since only n atoms will be used.  For higher ranks, we need the shape from s.  So it's just as well
    // that we take the shape from s now
    *y      = d;
    auto wv = pointer_to_values<void>(w);                       // return the address of the new block
    if (t & CMPX) jtfillv(jt, t, n, (C *)yv);  // why??  just fill in imaginary parts as we need to
    if (!n) return 1;
    // Perform the conversion based on data types
    // For branch-table efficiency, we split the C2T and C4T and BIT conversions into one block, and
    // the rest in another
    if ((t | wt) & (C2T + C4T + BIT + SBT)) {
        // there are no SBT conversions, but we have to show domain error we
        // must account for all NOUN types.  Low 8 bits have most of them, and
        // we know type can't be sparse.  This picks up the others
        ASSERT(!((t | wt) & SBT), EVDOMAIN);  // No conversions for these types
        switch (CVCASE(CTTZ(t), CTTZ(wt))) {
            case CVCASE(LITX, C2TX): return convert<US, UC>(jt, w, yv);
            case CVCASE(LITX, C4TX): return convert<C4, UC>(jt, w, yv);
            case CVCASE(C2TX, LITX): return convert<UC, US>(jt, w, yv);
            case CVCASE(C2TX, C4TX): return convert<C4, US>(jt, w, yv);
            case CVCASE(C4TX, LITX): return convert<UC, C4>(jt, w, yv);
            case CVCASE(C4TX, C2TX): return convert<US, C4>(jt, w, yv);
            default: ASSERT(0, EVDOMAIN);
        }
    }
    switch (CVCASE(CTTZ(t), CTTZ(wt))) {
        case CVCASE(INTX, B01X): std::copy_n(static_cast<B *>(wv), n, static_cast<I *>(yv)); return 1;
        case CVCASE(XNUMX, B01X): return convert<bool, X>(jt, w, yv);
        case CVCASE(RATX, B01X):
            GATV(d, XNUM, n, r, s);
            return convert<bool, X>(jt, w, pointer_to_values(d)) && convert<X, Q>(jt, d, yv);
        case CVCASE(FLX, B01X): std::copy_n(static_cast<B *>(wv), n, static_cast<D *>(yv)); return 1;
        case CVCASE(CMPXX, B01X): set_real_part(static_cast<Z*>(yv), n, static_cast<B*>(wv)); return 1;
        case CVCASE(B01X, INTX): return convert<I, bool>(jt, w, yv);
        case CVCASE(XNUMX, INTX): return convert<I, X>(jt, w, yv);
        case CVCASE(RATX, INTX): GATV(d, XNUM, n, r, s); return convert<I, X>(jt, w, pointer_to_values(d)) && convert<X, Q>(jt, d, yv);
        case CVCASE(FLX, INTX): std::copy_n(static_cast<I *>(wv), n, static_cast<D *>(yv)); return 1;
        case CVCASE(CMPXX, INTX): set_real_part(static_cast<Z *>(yv), n, static_cast<I *>(wv)); return 1;
        case CVCASE(B01X, FLX): return convert<D, bool>(jt, w, yv, (I)jtinplace & JTNOFUZZ ? 0.0 : FUZZ);
        case CVCASE(INTX, FLX): return convert<D, I>(jt, w, yv, (I)jtinplace & JTNOFUZZ ? 0.0 : FUZZ);
        case CVCASE(XNUMX, FLX):
            return convert<D, X>(
              jt, w, yv, (jt->xmode & REPSGN(SGNIFNOT(tflagged, XCVTXNUMORIDEX))) | (tflagged >> XCVTXNUMCVX));
        case CVCASE(RATX, FLX):
            return convert<D, Q>(
              jt, w, yv, (jt->xmode & REPSGN(SGNIFNOT(tflagged, XCVTXNUMORIDEX))) | (tflagged >> XCVTXNUMCVX));
        case CVCASE(CMPXX, FLX): return convert<D, Z>(jt, w, yv);
        case CVCASE(B01X, CMPXX):
            GATV(d, FL, n, r, s);
            if (!(convert<Z, D>(jt, w, pointer_to_values(d), (I)jtinplace & JTNOFUZZ ? 0.0 : FUZZ))) return 0;
            return convert<D, bool>(jt, d, yv, (I)jtinplace & JTNOFUZZ ? 0.0 : FUZZ);
        case CVCASE(INTX, CMPXX):
            GATV(d, FL, n, r, s);
            if (!(convert<Z, D>(jt, w, pointer_to_values(d), (I)jtinplace & JTNOFUZZ ? 0.0 : FUZZ))) return 0;
            return convert<D, I>(jt, d, yv, (I)jtinplace & JTNOFUZZ ? 0.0 : FUZZ);
        case CVCASE(XNUMX, CMPXX):
            GATV(d, FL, n, r, s);
            if (!(convert<Z, D>(jt, w, pointer_to_values(d), (I)jtinplace & JTNOFUZZ ? 0.0 : FUZZ))) return 0;
            return convert<D, X>(
              jt, d, yv, (jt->xmode & REPSGN(SGNIFNOT(tflagged, XCVTXNUMORIDEX))) | (tflagged >> XCVTXNUMCVX));
        case CVCASE(RATX, CMPXX):
            GATV(d, FL, n, r, s);
            if (!(convert<Z, D>(jt, w, pointer_to_values(d), (I)jtinplace & JTNOFUZZ ? 0.0 : FUZZ))) return 0;
            return convert<D, Q>(
              jt, d, yv, (jt->xmode & REPSGN(SGNIFNOT(tflagged, XCVTXNUMORIDEX))) | (tflagged >> XCVTXNUMCVX));
        case CVCASE(FLX, CMPXX): return convert<Z, D>(jt, w, yv, (I)jtinplace & JTNOFUZZ ? 0.0 : FUZZ);
        case CVCASE(B01X, XNUMX): return convert<X, bool>(jt, w, yv);
        case CVCASE(INTX, XNUMX): return convert<X, I>(jt, w, yv);
        case CVCASE(RATX, XNUMX): return convert<X, Q>(jt, w, yv);
        case CVCASE(FLX, XNUMX): return convert<X, D>(jt, w, yv);
        case CVCASE(CMPXX, XNUMX):
            GATV(d, FL, n, r, s);
            if (!(convert<X, D>(jt, w, pointer_to_values(d)))) return 0;
            return convert<D, Z>(jt, d, yv);
        case CVCASE(B01X, RATX):
            GATV(d, XNUM, n, r, s);
            if (!(convert<Q, X>(jt, w, pointer_to_values(d)))) return 0;
            return convert<X, bool>(jt, d, yv);
        case CVCASE(INTX, RATX):
            GATV(d, XNUM, n, r, s);
            if (!(convert<Q, X>(jt, w, pointer_to_values(d)))) return 0;
            return convert<X, I>(jt, d, yv);
        case CVCASE(XNUMX, RATX): return convert<Q, X>(jt, w, yv);
        case CVCASE(FLX, RATX): return convert<Q, D>(jt, w, yv);
        case CVCASE(CMPXX, RATX):
            GATV(d, FL, n, r, s);
            if (!(convert<Q, D>(jt, w, pointer_to_values(d)))) return 0;
            return convert<D, Z>(jt, d, yv);
        default: ASSERT(0, EVDOMAIN);
    }
}

// clear rank before calling ccvt - needed for sparse arrays only but returns the block as the result
A
jtcvt(J jt, I t, A w) {
    A y;
    bool const b = jtccvt(jt, t, w, &y);
    ASSERT(b, EVDOMAIN);
    return y;
}

// Convert numeric type to lowest precision that fits.  Push fuzz/rank onto a stack,
// and use 'exact' and 'no rank' for them.  If mode=0, do not promote XNUM/RAT to fixed-length types.
// If mode bit 1 is set, minimum precision is INT; if mode bit 2 is set, minimum precision is FL; if mode bit 3 is set,
// minimum precision is CMPX Result is a new buffer, always
A
jtbcvt(J jt, C mode, A w) {
    FPREFIP;
    if (!w) return 0;

    auto const as_integer = [](auto const &v) { return *(I *)&v; };
    auto const isflag     = [&](auto const &z) { return as_integer(z.im) == NANFLAG; };

    // there may be values (especially b types) that were nominally CMPX but might actually be integers.  Those were
    // stored with the real part being the actual integer value and the imaginary part as the special 'flag' value.  We
    // handle those here.  If all the imaginary parts were flags, we accept all the integer parts and change the type
    // to integer.  If none of the imaginary parts were flags, we leave the input unchanged.  If some were flags, we
    // convert the flagged values to float and keep the result as complex
    array result = w;
    if ((((AN(w) - 1) | (AT(w) & CMPX) - 1)) >= 0) {  // not empty AND complex
        Z *wv      = pointer_to_values<Z>(w);
        auto flags = std::transform_reduce(wv, wv + AN(w), int64_t{}, std::plus{}, isflag);
        if (flags) {
            I ipok = SGNIF(jtinplace, JTINPLACEWX) & AC(w);  // both sign bits set (<0) if inplaceable
            if (flags == AN(w)) {
                if (ipok >= 0) GATV(result, INT, AN(w), AR(w), AS(w));
                std::transform(wv, wv + AN(w), pointer_to_values(result), [&](auto const &z) { return as_integer(z.re); });
            } else {
                if (ipok >= 0) GATV(result, CMPX, AN(w), AR(w), AS(w));
                std::transform(wv, wv + AN(w), pointer_to_values<Z>(result), [&](auto const &z) -> Z {
                    if (isflag(z)) return {.re = (D)as_integer(z.re), .im = 0.0};
                    return z;  // copy floats, and converts any integers back to float
                });
            }
            w = result;  // this result is now eligible for further demotion
        }
    }
    // for all numerics, try Boolean/int/float in order, stopping when we find one that holds the data
    if (mode & 1 || !(AT(w) & XNUM + RAT)) {  // if we are not stopping at XNUM/RAT
        // To avoid a needless copy, suppress conversion to B01 if type is B01, to INT if type is INT, etc
        // set the NOFUZZ flag in jt to insist on an exact match so we won't lose precision
        array y;
        jtinplace = (J)((I)jt + JTNOFUZZ);  // demand exact match
        result    = !(mode & 14) && jtccvt(jtinplace, B01, w, &y)                             ? y
                    : (y = w, AT(w) & INT || (!(mode & 12) && jtccvt(jtinplace, INT, w, &y))) ? y
                    : (y = w, AT(w) & FL || (!(mode & 8) && jtccvt(jtinplace, FL, w, &y)))
                      ? y
                      : w;  // convert to enabled modes one by one, stopping when one works
    }
    RNE(result);
} /* convert to lowest type. 0=mode: don't convert XNUM/RAT to other types */

A
jticvt(J jt, A w) {
    auto const n = AN(w);
    auto const* v = pointer_to_values<double>(w);
    A z;
    GATV(z, INT, n, AR(w), AS(w));
    auto u = pointer_to_values(z);
    for (int64_t i = 0; i < n; ++i) {
        auto x = *v++;
        if (x < IMIN || FLIMAX <= x) return w;  // if conversion will fail, skip it
        *u++ = (I)x;
    }
    return z;
}

A
jtpcvt(J jt, I t, A w) {
    RANK2T oqr = jt->ranks;
    RESETRANK;
    A y;
    bool const b = jtccvt(jt, t, w, &y);
    jt->ranks    = oqr;
    return b ? y : w;
} /* convert w to type t, if possible, otherwise just return w */

A
jtcvt0(J jt, A w) {
    auto const t = AT(w);
    auto const n = (t & CMPX) ? 2 * AN(w) : AN(w);
    if (n && t & FL + CMPX) {
        auto u = pointer_to_values<double>(w);
        std::transform(u, u + n, u, [](auto v) { return v == 0.0 ? 0.0 : v; });
    }
    return w;
} /* convert -0 to 0 in place */

A
jtxco1(J jt, A w) {
    ASSERT(AT(w) & DENSE, EVNONCE);
    return jtcvt(jt, AT(w) & B01 + INT + XNUM ? XNUM : RAT, w);
}

A
jtxco2(J jt, A a, A w) {
    ASSERT(AT(w) & DENSE, EVNONCE);
    I j;
    RE(j = jti0(jt, a));
    switch (j) {
        case -2: return jtaslash1(jt, CDIV, w);
        case -1: return jtbcvt(jt, 1, w);
        case 1: return jtxco1(jt, w);
        case 2:
            if (!(AT(w) & RAT)) RZ(w = jtcvt(jt, RAT, w));
            {
                auto const n = AN(w);
                auto const r = AR(w);
                A z;
                GATV(z, XNUM, 2 * n, r + 1, AS(w));
                AS(z)[r] = 2;
                memcpy(pointer_to_values(z), pointer_to_values(w), 2 * n * SZI);
                return z;
            }
        default: ASSERT(0, EVDOMAIN);
    }
}
