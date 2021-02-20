#include "j.h"
#include "ja.h"
#include "ja_wrappers.h"

A jtirs2_wrap_dyadic(J jt, A a, A w, A fs, I l, I r, AD* (*f2)(J, A, A)) {
    return jtirs2(jt, a, w, fs, l, r, (AF)f2);
}

A jtrank2ex_wrap_dyadic(J jt, AD *RESTRICT a, AD *RESTRICT w, A fs, I lr, I rr, I lcr, I rcr, AD* (*f2)(J, A, A)) {
    return jtrank2ex(jt, a, w, fs, REX2R(lr, rr, lcr, rcr), (AF)f2);
}
