
#include "array.hpp"

/** @file */

/**
 * @brief # :: Tally
 * @param jt
 * @param w   Input array
 * @return    Output array (always scalar integer)
 */
[[nodiscard]] auto
tally(J jt, array w) -> array {
    return make_scalar_integer(jt, item_count(w));
}
