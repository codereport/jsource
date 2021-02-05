
#include "array.hpp"

/** @file */

/**
 * @brief `#` Tally
 * @details In APL, this is also known as `tally` (≢). Known also as `len`, `length`, `size` and
 * `count`.
 * @param jt
 * @param w   Input array
 * @return    Output array (always scalar integer)
 */
[[nodiscard]] auto
tally(J jt, array w) -> array {
    return make_scalar_integer(jt, item_count(w));
}
