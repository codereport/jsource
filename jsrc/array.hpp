
#pragma once

extern "C" {
#include "j.h"
}

using array   = A;           // potentially rename to j_array?
using shape_t = long long*;  // TODO figure out how to turn this into int64_t
using rank_t  = unsigned short;

// TODO: probably certain uses of the SETIC macro use the second argument after
//       the function will need to create second function in that case
[[nodiscard]] inline auto
item_count(array w) {
    // if you have a non-zero rank (aka not a scalar), take first element of shape
    // otherwise, (in the case of scalar) return 1
    return AR(w) ? AS(w)[0] : 1;
}

// TODO: rename (propogate_sign_bit)
[[nodiscard]] constexpr inline auto
replicate_sign(int64_t x) noexcept -> int64_t {
    return x < 0 ? -1 : 0;
}

// TODO: rename
[[nodiscard]] constexpr auto
xor_replicate_sign(int64_t x) noexcept -> int64_t {
    return x < 0 ? (-1 * x) - 1 : x;
}

// TODO: refactor me
[[nodiscard]] inline auto
refactorme_num(int64_t n) {
    return reinterpret_cast<array>(Bnum + 2 + n - NUMMIN);
}

[[nodiscard]] inline auto
pointer_to_values(array x) -> int64_t* {
    return reinterpret_cast<int64_t*>(reinterpret_cast<C*>(x) + x->kchain.k);
}

// TODO: replace with `auto` concepts
template <typename T>
auto
set_value_at(array x, int32_t index, T const& value) -> void {
    pointer_to_values(x)[index] = value;
}

// TODO: remove eventually, temporary while c_types exist
template <typename Type>
constexpr auto
to_c_type() {
    if constexpr (std::is_same_v<Type, bool>) return B01;
    if constexpr (std::is_same_v<Type, double>) return FL;
    // TODO complete with the number of types we plan to support
    return INT;
}

struct copy_shape_0 {
    auto
    operator()(array name, int64_t atoms, rank_t rank) -> void {
        if (rank == 1) AS(name)[0] = atoms;
    }
};

template <typename Type, typename Copier>
[[nodiscard]] auto
make_array(J jt, int64_t atoms, rank_t rank) {
    int64_t const ctype = to_c_type<Type>();
    int64_t const bytes = ALLOBYTES(atoms, rank, sizeof(ctype), (ctype)&LAST0, (ctype)&NAME);

    array name = jtgaf(jt, ALLOBLOCK(bytes));
    AK(name)   = AKXR(rank);
    AT(name)   = ctype;
    AN(name)   = atoms;
    AR(name)   = rank;
    if (!((ctype)&DIRECT)) {
        if (rank == 0) AS(name)[0] = 0;
        memset(reinterpret_cast<C*>(AS(name) + 1), C0, (bytes - 32) & -32);
    }
    Copier{}(name, atoms, rank);
    return name;
}

template <typename Type>
[[nodiscard]] inline auto
make_array(J jt, int64_t n, rank_t r, shape_t s) -> array {
    return jtga(jt, to_c_type<Type>(), n, r, s);
}

// this is for "creating an integer atom with value k"
template <typename T>
[[nodiscard]] inline auto
make_scalar_integer(J jt, T k) -> array {
    if (xor_replicate_sign(k) <= NUMMAX) return refactorme_num(k);
    array z = make_array<T, copy_shape_0>(jt, 1, 0);
    set_value_at(z, 0, k);
    return z;
}
