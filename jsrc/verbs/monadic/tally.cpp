
extern "C" {
#include "j.h"
}

/*

STYLEGUIDE

* Trailing return type
* East `const`

*/

using array = A;

// array (R, Julia, others)
// jarray <- suggestion from Beron
// ndarray (NumPy, Octave)
// mdarray (C++ proposal)

// TODO: will targ be a int64_t in all cases
// TODO: probably certain uses of the SETIC macro use targ after the function
//       will need to create second function in that case
auto item_count(array w) {
    // if you have a non-zero rank (aka not a scalar), take first element of shape
    // otherwise, (in the case of scalar) return 1
    return w->r ? w->s[0] : 1;
}

// # y TODO: add doxygen support
auto jttally(J jt, A w) -> array {
    array const z = sc(item_count(w));
    return AT(w) & XNUM + RAT ? xco1(z) : z;
}