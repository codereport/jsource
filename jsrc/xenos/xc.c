/* Copyright 1990-2007, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Xenos: Custom                                                           */

#include "j.h"

// create explicit equivalent of verb w
// a is flags to handle lower recursions:
//  0 to begin with
//  1, 2, or 3 to indicate that a piece of the original input has been processed, and any $: found in a name must
//   be replaced by an explicit equivalent with the indicated valence(s)
A
jtfixrecursive(J jt, A a, A w) {
    A z;
    df1(z, w, jteval(jt, "1 : 'u y\n:\nx u y'"));
    return z;
}
