
#include <array.hpp>

// return A-block for list of type t, length n, and values *v
// MUST NOT return virtual or fixed block, because we often modify the returned area
[[nodiscard]] auto
jtvec(J jt, int64_t t, int64_t n, void* v) -> array {
    array z = make_array(jt, t, n, 1, 0);
    memcpy(AV(z), v, n << bplg(t));
    return z;
}