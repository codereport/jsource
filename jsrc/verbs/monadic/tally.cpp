
extern "C" {
#include "j.h"
}

using array = A;  // potentially rename to j_array?

// TODO: will targ be a int64_t in all cases
// TODO: probably certain uses of the SETIC macro use targ after the function
//       will need to create second function in that case
[[nodiscard]] auto
item_count(array w) {
    // if you have a non-zero rank (aka not a scalar), take first element of shape
    // otherwise, (in the case of scalar) return 1
    return w->r ? w->s[0] : 1;
}

// TODO: rename (propogate_sign_bit)
[[nodiscard]] constexpr auto
replicate_sign(int64_t x) noexcept -> int64_t {
    return x < 0 ? -1 : 0;
}

// TODO: renamce
[[nodiscard]] constexpr auto
xor_replicate_sign(int64_t x) noexcept -> int64_t {
    return x < 0 ? (-1 * x) - 1 : x;
}

[[nodiscard]] constexpr auto
zero_or_one(int64_t n) noexcept -> bool {
    return n == 0 || n == 1;
}

// TODO: refactor me
[[nodiscard]] auto
refactorme_num(int64_t n) {
    return reinterpret_cast<array>(Bnum + 2 + n - NUMMIN);
}

// TODO: refactor me
[[nodiscard]] auto
refactorme_zeroionei(int64_t n) {
    return reinterpret_cast<array>(Bnum + (n));
}

[[nodiscard]] auto
pointer_to_ravel(array x) {
    return reinterpret_cast<int64_t*>(reinterpret_cast<C*>(x) + x->kchain.k);
}

// this is for "creating an integer atom with value k"
[[nodiscard]] auto
make_scalar_integer(J jt, int64_t k) -> array {
    if (xor_replicate_sign(k) <= NUMMAX) {
        return !zero_or_one(k) ? refactorme_num(k) : zeroionei(k);
    }
    array z;
    GAT0(z, INT, 1, 0);          // TODO: GA -> make_array refactoring
    pointer_to_ravel(z)[0] = k;  // TODO: set_value(..)
    return z;
}

[[nodiscard]] constexpr auto
extended_precision_or_rational() -> int64_t {
    return XNUM + RAT;
}

// # y TODO: add doxygen support
[[nodiscard]] auto
jttally(J jt, array w) -> array {
    array const z = make_scalar_integer(jt, item_count(w));
    return w->tproxy.t & extended_precision_or_rational() ? xco1(z) : z;  // TODO: xcol
}
