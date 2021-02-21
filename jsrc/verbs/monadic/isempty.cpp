#include "array.hpp"

/** @file */

/**
 * @brief `0 e. $` IsEmpty
 * @param jt       JST (J Syntax Tree)
 * @param w        Input aray
 * @return         boolean jtrue or jfalse
 */
[[nodiscard]] auto
jtisempty(J jt, array w) -> array {
    if ((AT(w) & SPARSE) != 0) return jteps(jt, num(0), shape(jt, w));
    return AN(w) == 0 ? jtrue : jfalse;
}
