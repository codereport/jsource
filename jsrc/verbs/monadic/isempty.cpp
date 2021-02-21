#include "array.hpp"

[[nodiscard]] auto
jtisempty(J jt, array w) -> array {
    if ((AT(w) & SPARSE) != 0) return jteps(jt, num(0), shape(jt, w));
    return AN(w) == 0 ? jtrue : jfalse;
}  // 0 e. $
