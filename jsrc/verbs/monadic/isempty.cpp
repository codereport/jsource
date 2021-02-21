#include "array.hpp"

[[nodiscard]] A
jtisempty(J jt, A w) {
    if ((AT(w) & SPARSE) != 0) return jteps(jt, num(0), shape(jt, w));
    return AN(w) == 0 ? jtrue : jfalse;
}  // 0 e. $
