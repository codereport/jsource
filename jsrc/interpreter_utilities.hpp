
#include <array.hpp>

// return A-block for list of type t, length n, and values *v
// MUST NOT return virtual or fixed block, because we often modify the returned area
template <typename Type>
[[nodiscard]] auto
make_vector(J jt, int64_t n, void* v) -> array {
    array z = make_array<Type>(jt, n, 1, 0);
    memcpy(AV(z), v, n << bplg(to_c_type<Type>()));
    return z;
}