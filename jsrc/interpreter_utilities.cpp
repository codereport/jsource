
#include <array.hpp>

// return A-block for list of type t, length n, and values *v
// MUST NOT return virtual or fixed block, because we often modify the returned area
A
jtvec(J jt, I t, I n, void* v) {
    A z;
    GA(z, t, n, 1, 0);
    MC(AV(z), v, n << bplg(t));
    return z;
}