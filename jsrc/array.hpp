
#pragma once

extern "C" {
#include "j.h"
}

// C macros that have been replaced by C++ inline methods
#undef num

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

[[nodiscard]] constexpr auto
applicable_for_num(int64_t n) noexcept -> bool {
    return NUMMIN <= n && n <= NUMMAX;
}

/**
 * @param n C representation of number, valid range [NUMMIN, NUMMAX]
 * @return  The J representation of the integer
 */
[[nodiscard]] inline auto
num(int64_t n) {
    return reinterpret_cast<array>(Bnum[n - NUMMIN]);
}

template <typename Value>
[[nodiscard]] inline auto
pointer_to_values(array x) -> Value* {
    return reinterpret_cast<Value*>(reinterpret_cast<C*>(x) + x->kchain.k);
}

[[nodiscard]] constexpr auto
is_sparse(array x) noexcept -> bool {
    return (AT(x) & SPARSE) != 0;
}

// TODO: replace with `auto` concepts
template <typename T>
auto
set_value_at(array x, int32_t index, T const& value) -> void {
    pointer_to_values<T>(x)[index] = value;
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
    int64_t const bytes = ALLOBYTES(atoms, rank, sizeof(Type), ctype & LAST0, ctype & NAME);

    array name = jtgaf(jt, ALLOBLOCK(bytes));
    AK(name)   = AKXR(rank);
    AT(name)   = ctype;
    AN(name)   = atoms;
    AR(name)   = rank;
    if (!(ctype & DIRECT)) {
        if (rank == 0) AS(name)[0] = 0;
        memset(reinterpret_cast<C*>(AS(name) + 1), C0, (bytes - 32) & -32);
    }
    Copier{}(name, atoms, rank);
    return name;
}

template <typename Type>
[[nodiscard]] inline auto
make_array(J jt, int64_t atoms, rank_t rank, shape_t shape) -> array {
    return jtga(jt, to_c_type<Type>(), atoms, rank, shape);
}

// this is for "creating an integer atom with value k"
template <typename T>
[[nodiscard]] inline auto
make_scalar_integer(J jt, T k) -> array {
    if (applicable_for_num(k)) return num(k);
    array z = make_array<T, copy_shape_0>(jt, 1, 0);
    set_value_at(z, 0, k);
    return z;
}
