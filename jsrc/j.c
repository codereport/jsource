/* Copyright 1990-2018, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Global Variables                                                        */

#include "j.h"
#include "jversion.h"
#include <math.h>
#pragma warning(disable : 4056)  // negative infinity overflow

// globals start - set by globinit in initialization
struct Bd2 {
    I hdr[AKXR(0) / SZI];
    D v[2];
};
#define CREBLOCKATOMV2(name, t, v1, v2) \
    struct Bd2 B##name = {{AKXR(0), (t)&TRAVERSIBLE, 0, (t), ACPERMANENT, 1, 0}, {v1, v2}};
CREBLOCKATOMV2(a0j1, CMPX, 0.0, 1.0)  // 0j1
// TODO: This should probably return an AD instead of the current I[8] that just happens to share
//       the memory layout of AD. The current implementation is bound to break if that layout is
//       ever changed.
#define CBAIVAL(t, v) \
    { 7 * SZI, (t)&TRAVERSIBLE, 0, (t), ACPERMANENT, 1, 0, (v) }
// TODO: Next version of CBAIVAL, create AD directly instead of I[8]
#define CBAIVAL2(t, v) \
    (AD){ .kchain = 7 * SZI, .flag = (t)&TRAVERSIBLE, .tproxy = (t), .c = ACPERMANENT, .n = 1, .s = (v) }
#define CREBLOCKATOMI(name, t, v) I B##name[8] = CBAIVAL(t, v);
#define CREBLOCKVEC0(name, t) \
    I B##name[8] = {          \
      8 * SZI, (t)&TRAVERSIBLE, 0, (t), ACPERMANENT, 0, 1, 0};  // no padding at end - no atoms should be referenced
CREBLOCKVEC0(aqq, LIT)                                          // ''
CREBLOCKVEC0(mtv, B01)                                          // i.0 boolean
#define CREBLOCKATOMV1(name, t, v1) struct Bd1 B##name = {{AKXR(0), (t)&TRAVERSIBLE, 0, (t), ACPERMANENT, 1, 0}, {v1}};
CREBLOCKATOMV1(onehalf, FL, 0.5)                                  // 0.5
CREBLOCKATOMV1(ainf, FL, INFINITY)                                // _
CREBLOCKATOMV1(pie, FL, PI)                                       // PI
CREBLOCKATOMI(asgnlocsimp, ASGN + ASGNLOCAL + ASGNTONAME, CASGN)  // =. flagged as LOCAL+NAME
CREBLOCKATOMI(asgngloname, ASGN + ASGNTONAME, CGASGN)             // =: flagged as NAME
CREBLOCKATOMI(asgnforcegloname, ASGN + ASGNTONAME, CASGN)         // =. converted to global+NAME
CREBLOCKATOMI(asgnforceglo, ASGN, CASGN)                          // =. converted to global
CREBLOCKATOMI(mark, MARK, 0)                                      // parser mark, also used generally as a special value
CREBLOCKATOMI(imax, INT, IMAX)                                    // max positive value
CREBLOCKATOMI(chrcolon, LIT, ':')                                 // the one character
CREBLOCKATOMI(chrspace, LIT, ' ')                                 // the one character
C breakdata = 0;
D inf       = INFINITY;  /* _                                    */
D infm      = -INFINITY; /* __                                   */
#define CREBLOCKVEC1I(name, t, v) I B##name[9] = {(7 + 1) * SZI, (t)&TRAVERSIBLE, 0, (t), ACPERMANENT, 1, 1, 1, (v)};
CREBLOCKVEC1I(iv0, INT, 0) /* ,0   also extended integer 0                                */
CREBLOCKVEC1I(iv1, INT, 1) /* ,1   also extended integer 1                                */
#define CREBLOCKVEC2I(name, t) I B##name[9] = {(7 + 2) * SZI, (t)&TRAVERSIBLE, 0, (t), ACPERMANENT, 0, 2, 0, 0};
CREBLOCKVEC2I(mtm, B01)               /* ,0   also extended integer 0                                */
D jnan         = NAN;                 /* _.                                   */
A mnuvxynam[6] = {0, 0, 0, 0, 0, 0};  // name blocks for all arg names
// NOTE: for fetching IDs we use the validitymask as a safe place to fetch 0s from.  We know that
// validitymask[15] will be 0 and we use &validitymask[12] as an A* with AT of 0 (a non-function) or an L* with val=0;
// and &validitymask[0] as a V* with ID of 0
I validitymask[16] = {-1, -1, 0, 0, -1, -1, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0};  // native ss2/neon register is s64x2

AD Bnum[20] =
  {  // the numbers we keep at hand.
    CBAIVAL2(INT, -10), CBAIVAL2(INT, -9), CBAIVAL2(INT, -8), CBAIVAL2(INT, -7), CBAIVAL2(INT, -6), CBAIVAL2(INT, -5),
    CBAIVAL2(INT, -4),  CBAIVAL2(INT, -3), CBAIVAL2(INT, -2), CBAIVAL2(INT, -1), CBAIVAL2(INT, 0),  CBAIVAL2(INT, 1),
    CBAIVAL2(INT, 2),   CBAIVAL2(INT, 3),  CBAIVAL2(INT, 4),  CBAIVAL2(INT, 5),  CBAIVAL2(INT, 6),  CBAIVAL2(INT, 7),
    CBAIVAL2(INT, 8),   CBAIVAL2(INT, 9)};

// The booleans
A const jfalse = (A)(I[8])CBAIVAL(B01, 0);
A const jtrue = (A)(I[8])CBAIVAL(B01, 1);

struct Bd1 Bnumvr[3] = {  // floating-point 0, 1, and 2, used for constants
  {{AKXR(0), FL& TRAVERSIBLE, 0, FL, ACPERMANENT, 1, 0}, 0.0},
  {{AKXR(0), FL& TRAVERSIBLE, 0, FL, ACPERMANENT, 1, 0}, 1.0},
  {{AKXR(0), FL& TRAVERSIBLE, 0, FL, ACPERMANENT, 1, 0}, 2.0}};
D pf                 = 0;           /* performance frequency                */
Q zeroQ              = {iv0, iv1};  /* 0r1                                  */
Z zeroZ              = {0, 0};      /* 0j0                                  */
A zpath              = 0;           /* default locale search path           */
I iotavec[IOTAVECLEN];              // return values for i. small
// globals end

// global const start -  do not need globinit
const double dzero = 0.0;     // used by verbs/ve.c
I oneone[2]        = {1, 1};  // used by PROD
// global const end

#define bits "64"
#define hw ""

const char jeversion[] = "je9!:14 j" jversion "/j" bits "" hw "/" jplatform "/" jtype "/" jlicense "/" jbuilder
                         "/"__DATE__
                         "T"__TIME__;

A
jtversq(J jt, A w) {
    char m[1000];
    char d[21];
    char months[] = "Jan01Feb02Mar03Apr04May05Jun06Jul07Aug08Sep09Oct10Nov11Dec12";
    C* p;
    C* q;
    ASSERTMTV(w);
    strcpy(m, jeversion + 8);
    p = m + strlen(m) - 20;
    strcpy(d, p);
    *p = 0;
    if (' ' == d[4]) d[4] = '0';
    strncat(p, d + 7, 4);
    strcat(p, "-");
    d[3] = 0;
    q    = strstr(months, d);
    strncat(p, 3 + strstr(months, d), 2);
    strcat(p, "-");
    strncat(p, d + 4, 2);
    strcat(p, d + 11);
#if defined(__clang__)
    strcat(p, "/clang-");
    sprintf(p + strlen(p), "%i", __clang_major__);
    strcat(p, "-");
    sprintf(p + strlen(p), "%i", __clang_minor__);
    strcat(p, "-");
    sprintf(p + strlen(p), "%i", __clang_patchlevel__);
#elif defined(_MSC_FULL_VER)
    strcat(p, "/ms-");
    sprintf(p + strlen(p), "%i", _MSC_FULL_VER);
#elif defined(__GNUC__)
    strcat(p, "/gcc-");
    sprintf(p + strlen(p), "%i", __GNUC__);
    strcat(p, "-");
    sprintf(p + strlen(p), "%i", __GNUC_MINOR__);
#endif

    return jtcstr(jt, m);
}
