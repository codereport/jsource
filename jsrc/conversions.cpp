/* Copyright 1990-2010, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Conversions Amongst Internal Types                                      */

#include <algorithm>
#include <limits>
#include <numeric>
#include <optional>

#include "array.hpp"
extern "C" {
#include "verbs/vcomp.h"
}

#define CVCASE(a, b) (((a) << 3) + (b))  // The main cases fit in low 8 bits of mask

// fuzzy_equal() is used in bcvt, where FUZZ may be set to 0 to ensure only exact values are demoted to lower precision
// used when v is known to be exact integer. It's close enough, maybe ULP too small on the high end
[[nodiscard]] static auto
fuzzy_equal(double u, double v, double fuzz) -> bool {
    return std::abs(u - v) <= fuzz * std::abs(v);
}

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

template <typename T>
struct is_optional : std::false_type {};

template <typename T>
struct is_optional<std::optional<T>> : std::true_type {};

template <typename T>
[[nodiscard]] auto
value_if(bool cond, T value) -> std::optional<T> {
    return cond ? std::optional(value) : std::nullopt;
}

template <typename From, typename To, typename Transform>
[[nodiscard]] auto
convert(J jt, array w, void *yv, Transform t) -> bool {
    auto *v = pointer_to_values<From>(w);
    auto *result = static_cast<To *>(yv);
    if constexpr (is_optional<decltype(t(*v))>::value) {
        for (int64_t i = 0; i < AN(w); ++i) {
            auto opt = t(v[i]);
            if (!opt) return false;
            result[i] = opt.value();
        }
    } else {
        std::transform(v, v + AN(w), result, t);
    }
    return true;
}

template <typename From, typename To>
[[nodiscard]] auto
convert(J jt, array w, void *yv) -> bool {
    if constexpr (!in_range<To, From>()) {
        return convert<From, To>(jt, w, yv, [](auto v) { return value_if(in_range<To>(v), v); });
    }
    auto *v = pointer_to_values<From>(w);
    std::copy(v, v + AN(w), static_cast<To *>(yv));
    return true;
}

template <>
[[nodiscard]] auto
convert<D, bool>(J jt, array w, void *yv, D fuzz) -> bool {
    auto const infinity = [](auto p) { return p < -2 || 2 < p; };
    return convert<D, bool>(jt, w, yv, [&](auto p) {
        return value_if(!infinity(p) && (p == 0.0 || fuzzy_equal(p, 1.0, fuzz)), p != 0.0);
    });
}

template <>
[[nodiscard]] auto
convert<D, I>(J jt, array w, void *yv, D fuzz) -> bool {
    return convert<D, I>(jt, w, yv, [&](auto p) -> std::optional<I> {
        auto const q = jround(p);
        if (!(p == q || fuzzy_equal(p, q, fuzz))) {
            return std::nullopt;  // must equal int, possibly out of range
        }
        // out-of-range values don't convert, handle separately
        if (p < static_cast<D> IMIN) {
            return value_if(p >= IMIN * (1 + fuzz), IMIN);
        }  // if tolerantly < IMIN, error; else take IMIN
        else if (p >= FLIMAX) {
            return value_if(p <= -static_cast<D> IMIN * (1 + fuzz), IMAX);
        }  // if tolerantly > IMAX, error; else take IMAX
        return q;
    });
}

template <>
[[nodiscard]] auto
convert<Z, D>(J jt, array w, void *yv, D fuzz) -> bool {
    if (fuzz != 0.0) {
        return convert<Z, D>(jt, w, yv, [&](auto const &v) {
            auto const d = std::abs(v.im);
            return value_if(d != inf && d <= fuzz * std::abs(v.re), v.re);
        });
    }
    return convert<Z, D>(jt, w, yv, [](auto v) { return value_if(!v.im, v.re); });
}

template <>
[[nodiscard]] auto
convert<bool, X>(J jt, array w, void *yv) -> bool {
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
convert<I, X>(J jt, array w, void *yv) -> bool {
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
        if (0 > c) { inplace_negate(u, XIDIG); }
        return jtvec(jt, INT, length, u);
    };
    return convert<I, X>(jt, w, yv, convert_one) && !jt->jerr;
}

static auto
jtxd1(J jt, D p, I mode) -> X {
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
            if (!TEQ(p, e)) { return jtvec(jt, INT, 0L, &iotavec[-IOTAVECBEGIN]); }
    }
    if (p == inf) { return jtvci(jt, XPINF); }
    if (p == -inf) { return jtvci(jt, XNINF); }
    array t = make_array<int64_t, copy_shape_0>(jt, 30, 1);
    if (!t) return 0;
    auto *u   = pointer_to_values<int64_t>(t);
    int64_t m = 0;
    auto d    = std::abs(p);
    while (0 < d) {
        auto const q = floor(d / XBASE);
        auto const r = d - q * XBASE;
        u[m++]       = static_cast<I>(r);
        d            = q;
        if (m == AN(t)) {
            RZ(t = jtext(jt, 0, t));
            u = pointer_to_values<int64_t>(t);
        }
    }
    if (m == 0) {
        u[0] = 0;
        ++m;
    } else if (0 > p) {
        inplace_negate(u, m);
    }
    array z = jtxstd(jt, jtvec(jt, INT, m, u));
    EPILOG(z);
}

template <>
[[nodiscard]] auto
convert<D, X>(J jt, array w, void *yv, I mode) -> bool {
    return convert<D, X>(jt, w, yv, [=](auto v) { return jtxd1(jt, v, mode); }) && !jt->jerr;
}

template <>
[[nodiscard]] auto
convert<X, bool>(J jt, array w, void *yv) -> bool {
    return convert<X, bool>(jt, w, yv, [](auto q) {
        auto const e = pointer_to_values<int64_t>(q)[0];
        return value_if(!((AN(q) ^ 1) | (e & -2)), e);
    });
}

template <typename T>
[[nodiscard]] static auto
value_from_X(X p) -> T {
    auto const n = AN(p);
    auto const v = std::reverse_iterator(pointer_to_values<int64_t>(p) + n);
    return std::accumulate(v, v + n, T{}, [](auto d, auto v) { return v + d * XBASE; });
}

template <>
[[nodiscard]] auto
convert<X, I>(J jt, array w, void *yv) -> bool {
    X p = jtxc(jt, IMAX);
    if (!p) return false;
    X q = jtxminus(jt, jtnegate(jt, p), jtxc(jt, 1L));
    if (!q) return false;
    return convert<X, I>(jt, w, yv, [&](auto c) -> std::optional<int64_t> {
        if (!(1 != jtxcompare(jt, q, c) && 1 != jtxcompare(jt, c, p))) return std::nullopt;
        return value_from_X<int64_t>(c);
    });
}

template <>
[[nodiscard]] auto
convert<X, D>(J jt, array w, void *yv) -> bool {
    return convert<X, D>(jt, w, yv, [](auto p) {
        auto const c = pointer_to_values<int64_t>(p)[AN(p) - 1];
        if (c == XPINF) { return inf; }
        if (c == XNINF) { return infm; }
        return value_from_X<double>(p);
    });
}

template <>
[[nodiscard]] auto
convert<X, Q>(J jt, array w, void *yv) -> bool {
    return convert<X, Q>(jt, w, yv, [](auto v) -> Q { return {v, iv1}; });
}

template <>
[[nodiscard]] auto
convert<D, Q>(J jt, array w, void *yv, I mode) -> bool {
    if ((w) == nullptr) return false;
    auto const n   = AN(w);
    auto *const wv = pointer_to_values<double>(w);
    auto *x        = static_cast<Q *>(yv);
    D t            = NAN;
    auto *tv       = 3 + reinterpret_cast<S *>(&t);
    Q q;
    for (int64_t i = 0; i < n; ++i) {
        t = wv[i];
        ASSERT(!_isnan(t), EVNAN);
        bool const neg = 0 > t;
        if (neg) { t = -t; }
        q.d = iv1;
        if (t == inf) {
            q.n = jtvci(jt, XPINF);
        } else if (t == 0.0) {
            q.n = iv0;
        } else if (1.1102230246251565e-16 < t && t < 9.007199254740992e15) {
            auto const d = jround(1 / jtdgcd(jt, 1.0, t));
            auto const c = jround(d * t);
            q            = jtqstd(jt,
                       {
                         .n = jtxd1(jt, c, mode),
                         .d = jtxd1(jt, d, mode),
                       });
        } else {
            bool const recip = 1 > t;
            if (recip) { t = 1.0 / t; }
            auto e = static_cast<I>(0xfff0 & *tv);
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
        if (neg) { inplace_negate(pointer_to_values<int64_t>(q.n), AN(q.n)); }
        *x++ = q;
    }
    return !jt->jerr;
}

template <>
[[nodiscard]] auto
convert<Q, D>(J jt, array w, void *yv) -> bool {
    auto const xb  = static_cast<D>(XBASE);
    auto const nn  = 308 / XBASEN;

    // TODO: figure out nice algorithm for this
    auto const add_digits = [&](auto n, auto v) {
        auto f = 1.0;
        auto d = 0.0;
        std::for_each(v, v + n, [&](auto i) {
            d += f * i;
            f *= xb;
        });
        return d;
    };

    X x2 = nullptr;
    return convert<Q, D>(jt, w, yv, [&](auto nd) -> std::optional<D> {
        auto *const p = nd.n;
        auto const pn = AN(p);
        auto const kk = 1 == pn ? pointer_to_values<int64_t>(p)[0] : 0;
        if (kk == XPINF) return inf;
        if (kk == XNINF) return infm;
        auto *const q = nd.d;
        auto const qn = AN(q);
        if (pn <= nn && qn <= nn) {
            auto const n = add_digits(pn, pointer_to_values<int64_t>(p));
            auto const d = add_digits(qn, pointer_to_values<int64_t>(q));
            return n / d;
        }
        if (!x2 && !(x2 = jtxc(jt, 2L))) return std::nullopt;
        auto const k = 5 + qn;
        auto *c      = jtxdiv(jt, jttake(jt, jtsc(jt, -(k + pn)), p), q, XMFLR);
        if (!c) return std::nullopt;
        auto const cn = AN(c);
        auto const m  = MIN(cn, 5);
        auto const r  = cn - (m + k);
        auto const n  = add_digits(m, pointer_to_values<int64_t>(c) + cn - m);
        auto d        = std::pow(xb, std::abs(r));
        return 0 > r ? n / d : n * d;
    });
}

template <>
[[nodiscard]] auto
convert<Q, X>(J jt, array w, void *yv) -> bool {
    return convert<Q, X>(jt, w, yv, [&](auto v) { return value_if(jtequ(jt, iv1, v.d), v.n); }) && !jt->jerr;
}

template <typename T>
auto
set_real_part(Z *z, int64_t n, T *t) {
    for (int64_t i = 0; i < n; ++i) { z[i].re = t[i]; }
}

// Imaginary parts have already been cleared
template <>
[[nodiscard]] auto
convert<D, Z>(J jt, array w, void *yv) -> bool {
    set_real_part(static_cast<Z *>(yv), AN(w), pointer_to_values<double>(w));
    return true;
}

// Convert the data in w to the type t.  w and t must be noun types.  A new buffer is always created (with a
// copy of the data if w is already of the right type), and returned in *y.  Result is
// 0 if error, 1 if success.  If the conversion loses precision, error is returned
// Calls through bcvt are tagged with a flag in jt, indicating to set fuzz=0
auto
jtccvt(J jt, I tflagged, array w, array *y) -> bool {
    FPREFIP;
    I const t = tflagged & NOUN;
    if (w == nullptr) return false;
    auto const r  = AR(w);
    auto *const s = AS(w);
    if (((t | AT(w)) & SPARSE) != 0) {
        // Handle sparse
        RANK2T oqr = jt->ranks;
        RESETRANK;
        switch (((t & SPARSE) != 0 ? 2 : 0) + (AT(w) & SPARSE ? 1 : 0)) {
            case 1: RZ(w = jtdenseit(jt, w)); break;  // sparse to dense
            case 2:
                RZ(*y = jtsparseit(jt, jtcvt(jt, DTYPE(t), w), IX(r), jtcvt(jt, DTYPE(t), jfalse)));
                jt->ranks = oqr;
                return true;  // dense to sparse; convert type first (even if same dtype)
            case 3:           // sparse to sparse
                I t1 = DTYPE(t);
                GASPARSE(*y, t, 1, r, s);
                P *yp = pointer_to_values<P>(*y);
                P *wp = pointer_to_values<P>(w);
                SPB(yp, a, jtca(jt, SPA(wp, a)));
                SPB(yp, i, jtca(jt, SPA(wp, i)));
                SPB(yp, e, jtcvt(jt, t1, SPA(wp, e)));
                SPB(yp, x, jtcvt(jt, t1, SPA(wp, x)));
                jt->ranks = oqr;
                return true;
        }
        jt->ranks = oqr;
    }
    // Now known to be non-sparse
    auto const wt = AT(w);
    // If type is already correct, return a clone - used to force a copy.  Should get rid of this kludge
    if (TYPESEQ(t, wt)) {
        *y = jtca(jt, w);
        return *y != nullptr;
    }
    // Kludge on behalf of result assembly: we want to be able to stop converting after the valid cells.  If
    // NOUNCVTVALIDCT is set in the type, we use the input *y as as override on the # cells to convert.  We use it to
    // replace n (for use here) and yv, and AK(w) and AN(w) for the subroutines. If NOUNCVTVALIDCT is set, w is
    // modified: the caller must restore AN(w) and AK(w) if it needs it
    // TODO: same-length conversion could be done in place
    auto n  = AN(w);
    array d = jtga(jt, t, n, r, s);
    if (!d) return false;
    auto *yv = pointer_to_values<void>(d);  // allocate the same # atoms, even if we will convert fewer
    if ((tflagged & NOUNCVTVALIDCT) != 0) {
        I inputn = *reinterpret_cast<I *>(y);  // fetch input, in case it is called for
        if (inputn > 0) {                      // if converting the leading values, just update the counts
            n = inputn;                        // set the counts for local use, and in the block to be converted
        } else {                               // if converting trailing values...
            AK(w) += (n + inputn) << bplg(wt);
            yv = reinterpret_cast<I *>(static_cast<C *>(yv) +
                                       ((n + inputn) << bplg(t)));  // advance input and output pointers to new area
            n  = -inputn;                                           // get positive # atoms to convert
        }
        AN(w) = n;  // change atomct of w to # atoms to convert
    }
    // If n and AN have been modified, it doesn't matter for rank-1 arguments whether the shape of the result is listed
    // as n or s[0] since only n atoms will be used.  For higher ranks, we need the shape from s.  So it's just as well
    // that we take the shape from s now
    *y       = d;
    if ((t & CMPX) != 0) {
        jtfillv(jt, t, n, static_cast<C *>(yv));  // why??  just fill in imaginary parts as we need to
    }
    if (n == 0) return true;
    // Perform the conversion based on data types
    // For branch-table efficiency, we split the C2T and C4T and BIT conversions into one block, and
    // the rest in another
    if (((t | wt) & (C2T + C4T + BIT + SBT)) != 0) {
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
        case CVCASE(INTX, B01X): std::copy_n(pointer_to_values<B>(w), n, static_cast<I *>(yv)); return true;
        case CVCASE(XNUMX, B01X): return convert<bool, X>(jt, w, yv);
        case CVCASE(RATX, B01X):
            GATV(d, XNUM, n, r, s);
            return convert<bool, X>(jt, w, pointer_to_values<int64_t>(d)) && convert<X, Q>(jt, d, yv);
        case CVCASE(FLX, B01X): std::copy_n(pointer_to_values<B>(w), n, static_cast<D *>(yv)); return true;
        case CVCASE(CMPXX, B01X): set_real_part(static_cast<Z *>(yv), n, pointer_to_values<B>(w)); return true;
        case CVCASE(B01X, INTX): return convert<I, bool>(jt, w, yv);
        case CVCASE(XNUMX, INTX): return convert<I, X>(jt, w, yv);
        case CVCASE(RATX, INTX):
            GATV(d, XNUM, n, r, s);
            return convert<I, X>(jt, w, pointer_to_values<int64_t>(d)) && convert<X, Q>(jt, d, yv);
        case CVCASE(FLX, INTX): std::copy_n(pointer_to_values<I>(w), n, static_cast<D *>(yv)); return true;
        case CVCASE(CMPXX, INTX): set_real_part(static_cast<Z *>(yv), n, pointer_to_values<I>(w)); return true;
        case CVCASE(B01X, FLX): return convert<D, bool>(jt, w, yv, ((I)jtinplace & JTNOFUZZ) != 0 ? 0.0 : FUZZ);
        case CVCASE(INTX, FLX): return convert<D, I>(jt, w, yv, ((I)jtinplace & JTNOFUZZ) != 0 ? 0.0 : FUZZ);
        case CVCASE(XNUMX, FLX):
            return convert<D, X>(
              jt, w, yv, (jt->xmode & REPSGN(SGNIFNOT(tflagged, XCVTXNUMORIDEX))) | (tflagged >> XCVTXNUMCVX));
        case CVCASE(RATX, FLX):
            return convert<D, Q>(
              jt, w, yv, (jt->xmode & REPSGN(SGNIFNOT(tflagged, XCVTXNUMORIDEX))) | (tflagged >> XCVTXNUMCVX));
        case CVCASE(CMPXX, FLX): return convert<D, Z>(jt, w, yv);
        case CVCASE(B01X, CMPXX):
            GATV(d, FL, n, r, s);
            return convert<Z, D>(jt, w, pointer_to_values<int64_t>(d), ((I)jtinplace & JTNOFUZZ) != 0 ? 0.0 : FUZZ) &&
                   convert<D, bool>(jt, d, yv, ((I)jtinplace & JTNOFUZZ) != 0 ? 0.0 : FUZZ);
        case CVCASE(INTX, CMPXX):
            GATV(d, FL, n, r, s);
            return convert<Z, D>(jt, w, pointer_to_values<int64_t>(d), ((I)jtinplace & JTNOFUZZ) != 0 ? 0.0 : FUZZ) &&
                   convert<D, I>(jt, d, yv, ((I)jtinplace & JTNOFUZZ) != 0 ? 0.0 : FUZZ);
        case CVCASE(XNUMX, CMPXX):
            GATV(d, FL, n, r, s);
            return convert<Z, D>(jt, w, pointer_to_values<int64_t>(d), ((I)jtinplace & JTNOFUZZ) != 0 ? 0.0 : FUZZ) &&
                   convert<D, X>(
                     jt, d, yv, (jt->xmode & REPSGN(SGNIFNOT(tflagged, XCVTXNUMORIDEX))) | (tflagged >> XCVTXNUMCVX));
        case CVCASE(RATX, CMPXX):
            GATV(d, FL, n, r, s);
            return convert<Z, D>(jt, w, pointer_to_values<int64_t>(d), ((I)jtinplace & JTNOFUZZ) != 0 ? 0.0 : FUZZ) &&
                   convert<D, Q>(
                     jt, d, yv, (jt->xmode & REPSGN(SGNIFNOT(tflagged, XCVTXNUMORIDEX))) | (tflagged >> XCVTXNUMCVX));
        case CVCASE(FLX, CMPXX): return convert<Z, D>(jt, w, yv, ((I)jtinplace & JTNOFUZZ) != 0 ? 0.0 : FUZZ);
        case CVCASE(B01X, XNUMX): return convert<X, bool>(jt, w, yv);
        case CVCASE(INTX, XNUMX): return convert<X, I>(jt, w, yv);
        case CVCASE(RATX, XNUMX): return convert<X, Q>(jt, w, yv);
        case CVCASE(FLX, XNUMX): return convert<X, D>(jt, w, yv);
        case CVCASE(CMPXX, XNUMX):
            GATV(d, FL, n, r, s);
            return convert<X, D>(jt, w, pointer_to_values<int64_t>(d)) && convert<D, Z>(jt, d, yv);
        case CVCASE(B01X, RATX):
            GATV(d, XNUM, n, r, s);
            return convert<Q, X>(jt, w, pointer_to_values<int64_t>(d)) && convert<X, bool>(jt, d, yv);
        case CVCASE(INTX, RATX):
            GATV(d, XNUM, n, r, s);
            return convert<Q, X>(jt, w, pointer_to_values<int64_t>(d)) && convert<X, I>(jt, d, yv);
        case CVCASE(XNUMX, RATX): return convert<Q, X>(jt, w, yv);
        case CVCASE(FLX, RATX): return convert<Q, D>(jt, w, yv);
        case CVCASE(CMPXX, RATX):
            GATV(d, FL, n, r, s);
            return convert<Q, D>(jt, w, pointer_to_values<int64_t>(d)) && convert<D, Z>(jt, d, yv);
        default: ASSERT(0, EVDOMAIN);
    }
}

// clear rank before calling ccvt - needed for sparse arrays only but returns the block as the result
auto
jtcvt(J jt, I t, array w) -> array {
    array y      = nullptr;
    bool const b = jtccvt(jt, t, w, &y);
    ASSERT(b, EVDOMAIN);
    return y;
}

// Convert numeric type to lowest precision that fits.  Push fuzz/rank onto a stack,
// and use 'exact' and 'no rank' for them.  If mode=0, do not promote XNUM/RAT to fixed-length types.
// If mode bit 1 is set, minimum precision is INT; if mode bit 2 is set, minimum precision is FL; if mode bit 3 is set,
// minimum precision is CMPX Result is a new buffer, always
auto
jtbcvt(J jt, C mode, array w) -> array {
    FPREFIP;
    if (w == nullptr) { return nullptr; }

    auto const as_integer = [](auto const &v) { return *(I *)&v; };
    auto const isflag     = [&](auto const &z) { return as_integer(z.im) == NANFLAG; };

    // there may be values (especially b types) that were nominally CMPX but might actually be integers.  Those were
    // stored with the real part being the actual integer value and the imaginary part as the special 'flag' value.  We
    // handle those here.  If all the imaginary parts were flags, we accept all the integer parts and change the type
    // to integer.  If none of the imaginary parts were flags, we leave the input unchanged.  If some were flags, we
    // convert the flagged values to float and keep the result as complex
    array result = w;
    if ((((AN(w) - 1) | ((AT(w) & CMPX) - 1))) >= 0) {  // not empty AND complex
        Z *wv = pointer_to_values<Z>(w);
        // FIXME: get proper c++17 support
        // auto flags = std::transform_reduce(wv, wv + AN(w), int64_t{}, std::plus{}, isflag);
        auto flags = std::accumulate(wv, wv + AN(w), int64_t{}, [&](auto sum, auto v) { return sum + isflag(v); });
        if (flags != 0) {
            I ipok = SGNIF(jtinplace, JTINPLACEWX) & AC(w);  // both sign bits set (<0) if inplaceable
            if (flags == AN(w)) {
                if (ipok >= 0) GATV(result, INT, AN(w), AR(w), AS(w));
                std::transform(
                  wv, wv + AN(w), pointer_to_values<int64_t>(result), [&](auto const &z) { return as_integer(z.re); });
            } else {
                if (ipok >= 0) GATV(result, CMPX, AN(w), AR(w), AS(w));
                std::transform(wv, wv + AN(w), pointer_to_values<Z>(result), [&](auto const &z) -> Z {
                    if (isflag(z)) { return {.re = (D)as_integer(z.re), .im = 0.0}; };
                    return z;  // copy floats, and converts any integers back to float
                });
            }
            w = result;  // this result is now eligible for further demotion
        }
    }
    // for all numerics, try Boolean/int/float in order, stopping when we find one that holds the data
    if (((mode & 1) != 0) || ((AT(w) & (XNUM + RAT)) == 0)) {  // if we are not stopping at XNUM/RAT
        // To avoid a needless copy, suppress conversion to B01 if type is B01, to INT if type is INT, etc
        // set the NOFUZZ flag in jt to insist on an exact match so we won't lose precision
        array y   = nullptr;
        jtinplace = (J)((I)jt + JTNOFUZZ);  // demand exact match
        result    = ((mode & 14) == 0) && jtccvt(jtinplace, B01, w, &y)                             ? y
                    : (y = w, AT(w) & INT || (((mode & 12) == 0) && jtccvt(jtinplace, INT, w, &y))) ? y
                    : (y = w, AT(w) & FL || (((mode & 8) == 0) && jtccvt(jtinplace, FL, w, &y)))
                      ? y
                      : w;  // convert to enabled modes one by one, stopping when one works
    }
    RNE(result);
} /* convert to lowest type. 0=mode: don't convert XNUM/RAT to other types */

auto
jticvt(J jt, array w) -> array {
    array z = nullptr;
    GATV(z, INT, AN(w), AR(w), AS(w));
    return convert<double, int64_t>(
             jt, w, pointer_to_values<void>(z), [](auto x) { return value_if(IMIN <= x && x < FLIMAX, x); })
             ? z
             : w;  // if conversion will fail, skip it
}

auto
jtpcvt(J jt, I t, array w) -> array {
    RANK2T oqr = jt->ranks;
    RESETRANK;
    array y      = nullptr;
    bool const b = jtccvt(jt, t, w, &y);
    jt->ranks    = oqr;
    return b ? y : w;
} /* convert w to type t, if possible, otherwise just return w */

auto
jtcvt0(J jt, array w) -> array {
    auto const t = AT(w);
    auto const n = (t & CMPX) != 0 ? 2 * AN(w) : AN(w);
    if ((n != 0) && ((t & (FL + CMPX)) != 0)) {
        auto *u = pointer_to_values<double>(w);
        std::transform(u, u + n, u, [](auto v) { return v == 0.0 ? 0.0 : v; });
    }
    return w;
} /* convert -0 to 0 in place */

auto
jtxco1(J jt, array w) -> array {
    ASSERT(AT(w) & DENSE, EVNONCE);
    return jtcvt(jt, AT(w) & (B01 + INT + XNUM) ? XNUM : RAT, w);
}

auto
jtxco2(J jt, array a, array w) -> array {
    ASSERT(AT(w) & DENSE, EVNONCE);
    I j = jti0(jt, a);
    if (jt->jerr != 0) return nullptr;
    switch (j) {
        case -2: return jtaslash1(jt, CDIV, w);
        case -1: return jtbcvt(jt, 1, w);
        case 1: return jtxco1(jt, w);
        case 2:
            if ((AT(w) & RAT) == 0) {
                w = jtcvt(jt, RAT, w);
                if (!w) return nullptr;
            }
            {
                auto const n = AN(w);
                auto const r = AR(w);
                array z      = nullptr;
                GATV(z, XNUM, 2 * n, r + 1, AS(w));
                AS(z)[r] = 2;
                memcpy(pointer_to_values<void>(z), pointer_to_values<void>(w), 2 * n * SZI);
                return z;
            }
        default: ASSERT(0, EVDOMAIN);
    }
}
