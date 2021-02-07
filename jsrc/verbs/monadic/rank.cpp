
#include "array.hpp"

/** @file */

/**
 * @brief `#@$` Rank
 * @param jt
 * @param w  Input array
 * @return   Output array
 */
[[nodiscard]] auto
rank(J jt, array w) -> array {
    return make_scalar_integer(jt, AR(w));
}
