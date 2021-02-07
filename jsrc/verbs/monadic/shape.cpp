
#include "array.hpp"

/** @file */

/**
 * @brief `$` Shape
 * @details In APL, this is also known as `shape` (⍴).
 * @param jt
 * @param w   Input array
 * @return    Output array
 */
[[nodiscard]] auto
shape(J jt, array w) -> array {
    return vec(INT, AR(w), AS(w));
}  // $ y