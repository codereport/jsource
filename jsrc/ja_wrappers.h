/* Wrappers for stricter C++ compilers */

#ifdef __cplusplus
extern "C" {
#endif

A jtirs2_wrap_dyadic(J jt, A a, A w, A fs, I l, I r, AD* (*f2)(J, A, A));
A jtrank2ex_wrap_dyadic(J jt, AD *RESTRICT a, AD *RESTRICT w, A fs, I lr, I rr, I lcr, I rcr, AD* (*f2)(J, A, A));

#ifdef __cplusplus
}
#endif
