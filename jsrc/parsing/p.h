/* Copyright 1990-2005, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Parsing: Macros and Defined Constants                                   */

#define NCASES 9L /* # of rows in cases parses table    */

typedef TA (*TAF)();
typedef A *(*ACTF)();
typedef struct {
    I c[4];
    ACTF f;
    TAF vf;
    I b, e, k;
} PT;

/* c:  4-patterns for AT(x) of top 4 parser stack elements      */
/* f:  action                                                   */
/* vf: action for tacit verb translator                         */
/* b:  beginning index in stack that action applies to          */
/* e:  ending    index in stack that action applies to          */
/* k:  index in stack of error indicator                        */
// this is now used only for tacit translator

extern PT cases[];

extern TA jtvadv(J jt, I b, I e, TA *stack, A locsyms, I tmonad, I tsubst, TA *ttab, I *ttabi, I ttabi0);
extern TA jtvconj(J jt, I b, I e, TA *stack, A locsyms, I tmonad, I tsubst, TA *ttab, I *ttabi, I ttabi0);
extern TA jtvdyad(J jt, I b, I e, TA *stack, A locsyms, I tmonad, I tsubst, TA *ttab, I *ttabi, I ttabi0);
extern TA jtvfolk(J jt, I b, I e, TA *stack, A locsyms, I tmonad, I tsubst, TA *ttab, I *ttabi, I ttabi0);
extern TA jtvhook(J jt, I b, I e, TA *stack, A locsyms, I tmonad, I tsubst, TA *ttab, I *ttabi, I ttabi0);
extern TA jtvis(J jt, I b, I e, TA *stack, A locsyms, I tmonad, I tsubst, TA *ttab, I *ttabi, I ttabi0);
extern TA jtvmonad(J jt, I b, I e, TA *stack, A locsyms, I tmonad, I tsubst, TA *ttab, I *ttabi, I ttabi0);
extern TA jtvpunc(J jt, I b, I e, TA *stack, A locsyms, I tmonad, I tsubst, TA *ttab, I *ttabi, I ttabi0);
